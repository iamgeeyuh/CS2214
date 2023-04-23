#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <iomanip>
using namespace std;

// initial RAM, PC, and registers

uint16_t ram[8192];
uint16_t pc = 0;
uint16_t reg[8] = {0};

// opcode values

const int op_add = 0;
const int op_sub = 1;
const int op_or = 2;
const int op_and = 3;
const int op_slt = 4;
const int op_jr = 5;

const int op_slti = 6;
const int op_lw = 7;
const int op_sw = 8;
const int op_jeq = 9;
const int op_addi = 10;

const int op_j = 11;
const int op_jal = 12;

// arguments

int rgA;
int rgB;
int rgC;
uint16_t imm;

// cache classes

class Block
{

public:
    Block() : valid(false) {}

    Block(int tag, int used) : tag(tag), used(used), valid(true)
    {
    }

    // getters
    bool is_valid() const { return valid; }
    int get_tag() const { return tag; }
    int get_used() const { return used; }

    void set_used(int new_used) { used = new_used; }
    void set_tag(int new_tag) { tag = new_tag; }

private:
    bool valid;
    int tag;
    int used;
};

class Cache
{
    friend ostream &operator<<(ostream &os, const Cache &cache)
    {
        os << "Cache " << cache.name << " has size " << cache.size << ", associativity " << cache.associativity << ", blocksize " << cache.blocksize << ", rows " << cache.num_rows << endl;

        return os;
    }

public:
    Cache(const string &name) : name(name), exists(false) {}

    Cache(const string &name, int size, int associativity, int blocksize) : name(name), size(size), associativity(associativity), blocksize(blocksize), num_rows(size / (associativity * blocksize)), exists(true), recently_used(num_rows, 0)
    {
        row.resize(num_rows);
        for (int i = 0; i < num_rows; i++)
        {
            row[i].resize(associativity);
        }
    }

    // getters
    const string &get_name() const { return name; }
    bool get_exists() const { return exists; }

    void read_cache(int addr, Cache &L2)
    {
        int block_id = addr / blocksize;
        int row_ind = block_id % num_rows;
        int tag = block_id / num_rows;

        vector<Block> &set = row[row_ind]; // selected row

        int empty_ind = -1;

        recently_used[row_ind] += 1;

        for (int i = 0; i < set.size(); i++)
        {
            if (!set[i].is_valid()) // if empty block, miss
            {
                empty_ind = i;
                break;
            }
            else if (set[i].get_tag() == tag) // if tag matched, hit
            {
                print_log_entry("HIT", addr, row_ind);
                // update most recently used
                set[i].set_used(recently_used[row_ind]);
                return;
            }
        }

        print_log_entry("MISS", addr, row_ind);

        if (name == "L1" && L2.get_exists())
        {
            // get values from L2
            L2.read_cache(addr, L2);
        }

        if (empty_ind != -1)
        {
            // first encountered empty block
            set[empty_ind] = Block(tag, recently_used[row_ind]);
        }
        else
        {
            // find least recently used block
            int least = set[0].get_used();
            int least_ind = 0;

            for (int i = 0; i < set.size(); i++)
            {
                if (set[i].get_used() < least)
                {
                    least = set[i].get_used();
                    least_ind = i;
                }
            }
            set[least_ind].set_tag(tag);
            set[least_ind].set_used(recently_used[row_ind]);
        }
    }

    void write_cache(int addr, Cache &L2)
    {
        int block_id = addr / blocksize;
        int row_ind = block_id % num_rows;
        int tag = block_id / num_rows;

        vector<Block> &set = row[row_ind]; // selected row

        recently_used[row_ind] += 1;

        print_log_entry("SW", addr, row_ind);

        bool hit = false;

        for (int i = 0; i < set.size(); i++)
        {
            if (!set[i].is_valid()) // if empty block, write
            {
                set[i] = Block(tag, recently_used[row_ind]);
                hit = true;
                break;
            }
        }
        if (name == "L1" && L2.get_exists())
        {

            L2.write_cache(addr, L2);
        }

        if (!hit)
        {
            // find least recently used block
            int least = set[0].get_used();
            int least_ind = 0;

            for (int i = 0; i < set.size(); i++)
            {
                if (set[i].get_used() < least)
                {
                    least = set[i].get_used();
                    least_ind = i;
                }
            }
            set[least_ind].set_tag(tag);
            set[least_ind].set_used(recently_used[row_ind]);
        }
    }

    void print_log_entry(const string &status, int addr, int row)
    {
        cout << left << setw(8) << name + " " + status << right << " pc:" << setw(5) << pc << "\taddr:" << setw(5) << addr << "\trow:" << setw(4) << row << endl;
    }

private:
    string name;
    int size;
    int associativity;
    int blocksize;
    int num_rows;
    bool exists;
    vector<vector<Block>> row;
    vector<int> recently_used; // stores order of block access
};

Cache L1("L1");
Cache L2("L2");

// sim functions

void load_code(const string &filename);
int decode(uint16_t code);
int execute(int op);
void output();

// cache functions

void load_cache(const string &args);

int main(int argc, char *argv[])
{
    bool halt = false;
    int op;
    uint16_t code;
    int mem;
    int pc_inc;

    load_code(argv[1]);
    load_cache(argv[3]);

    while (!halt)
    {
        // current memory pointer = 13 least sig bits of pc
        mem = pc & 0b1111111111111;
        code = ram[mem];
        // assign opcode and appropriate arguments
        op = decode(code);
        // stop program when j jumps to current memory pointer AKA halt
        if (op == op_j && mem == imm)
        {
            halt = true;
        }
        // get pc_inc and perform appropriate tasks based on opcode
        pc_inc = execute(op);
        pc += pc_inc;
    }
    // output();
}

/**
Loads a binary code file into RAM.

@param filename A string representing the filename of the binary code file to be loaded.
@return void

The function opens the specified file, reads the instructions in binary format, and stores them in RAM starting at address 0.
If the file does not contain enough instructions to fill up all of RAM, the remaining RAM is initialized to 0.
*/
void load_code(const string &filename)
{
    string line;
    ifstream file(filename);
    // add all instructions to ram starting at ram[0]. remaining ram is 0
    for (int i = 0; i < 8192; i++)
    {
        if (getline(file, line))
        {
            int start = line.find('b') + 1;
            ram[i] = static_cast<uint16_t>(stoul(line.substr(start, 16), nullptr, 2));
        }
        else
        {
            ram[i] = 0;
        }
    }
    file.close();
}

/**
Decodes a 16-bit binary instruction code and returns the corresponding opcode.

@param code An unsigned 16-bit integer representing the binary instruction code to be decoded.
@return An integer representing the corresponding opcode for the given binary instruction code.

The function decodes the given binary instruction code and returns the corresponding opcode as an integer.
The function extracts various fields from the binary instruction code, including the opcode and up to three registers and an immediate value (if applicable).
The function also performs sign-extension on any 7-bit immediate values, extending them to 16 bits.
The function returns the corresponding opcode as an integer value.
*/
int decode(uint16_t code)
{
    // default 7bit imm sign-extended to 16bit
    imm = code & 0b1111111;
    if (imm & 0b1000000)
    {
        imm = imm | 0b1111111110000000;
    }
    rgB = (code >> 7) & 0b111;
    rgA = (code >> 10) & 0b111;
    switch ((code >> 13) & 0b111)
    {
    // opcode in least sig 4 bits
    case (0):
        // 3 arguments
        rgC = (code >> 4) & 0b111;
        switch (code & 0b1111)
        {
        case (0):
            return op_add;
        case (1):
            return op_sub;
        case (2):
            return op_or;
        case (3):
            return op_and;
        case (4):
            return op_slt;
        case (8):
            if (((code >> 4) & 0b111111) == 0)
            {
                return op_jr;
            }
        }
    // opcode in most sig 3 bits
    case (1):
        return op_addi;
    case (2):
        // 13bit imm
        imm = code & 0b1111111111111;
        return op_j;
    case (3):
        // 13bit imm
        imm = code & 0b1111111111111;
        return op_jal;
    case (4):
        return op_lw;
    case (5):
        return op_sw;
    case (6):
        return op_jeq;
    case (7):
        return op_slti;
    }
    return -1;
}

/**
Executes an instruction given by its opcode and any required arguments.

@param op An integer representing the opcode of the instruction to be executed.
@return An integer representing the amount by which the program counter should be incremented after executing the instruction.

The function executes the instruction corresponding to the given opcode with any required arguments, updating the state of the registers and RAM as necessary.
The function returns an integer representing the amount by which the program counter should be incremented after executing the instruction.
This value is usually 1, but may be different for certain instructions that modify the program counter directly.
*/
int execute(int op)
{
    switch (op)
    {
    // if value is stored in a register, only execute if that register is not 0
    case (op_add):
        if (rgC != 0)
        {
            reg[rgC] = reg[rgA] + reg[rgB];
        }
        break;
    case (op_sub):
        if (rgC != 0)
        {
            reg[rgC] = reg[rgA] - reg[rgB];
        }
        break;
    case (op_or):
        if (rgC != 0)
        {
            reg[rgC] = reg[rgA] | reg[rgB];
        }
        break;
    case (op_and):
        if (rgC != 0)
        {
            reg[rgC] = reg[rgA] & reg[rgB];
        }
        break;
    case (op_slt):
        if (rgC != 0)
        {
            reg[rgC] = reg[rgA] < reg[rgB];
        }
        break;
    case (op_jr):
        // sets pc as reg[rgA] and adds 0 to pc
        pc = (reg[rgA]);
        return 0;
    case (op_slti):
        if (rgB != 0)
        {
            reg[rgB] = reg[rgA] < imm;
        }
        break;
    case (op_lw):
        L1.read_cache((reg[rgA] + imm) & 0b1111111111111, L2);
        if (rgB != 0)
        {
            reg[rgB] = ram[(reg[rgA] + imm) & 0b1111111111111];
        }
        break;
    case (op_sw):
        L1.write_cache((reg[rgA] + imm) & 0b1111111111111, L2);
        ram[(imm + reg[rgA]) & 0b1111111111111] = reg[rgB];
        break;
    case (op_jeq):
        if (reg[rgA] == reg[rgB])
        {
            // pc_inc = imm + 1
            return (imm + 1);
        }
        break;
    case (op_addi):
        if (rgB != 0)
        {
            reg[rgB] = imm + reg[rgA];
        }
        break;
    case (op_j):
        pc = imm;
        return 0;
    case (op_jal):
        reg[7] = pc + 1;
        pc = imm;
        return 0;
    }
    // default pc_inc is 1
    return 1;
}

void load_cache(const string &args)
{
    vector<int> int_args;
    int curr_arg = 0;

    // add value to curr_arg until ',' is found. then push curr_arg to int_args
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i] == ',')
        {
            int_args.push_back(curr_arg);
            curr_arg = 0;
        }
        else
        {
            curr_arg = curr_arg * 10 + (args[i] - '0');
        }
    }
    int_args.push_back(curr_arg);

    // redefine L1 and L2
    L1 = Cache("L1", int_args[0], int_args[1], int_args[2]);
    cout << L1;
    if (int_args.size() == 6)
    {
        L2 = Cache("L2", int_args[3], int_args[4], int_args[5]);
        cout << L2;
    }
}

void output()
{
    int pc_output = 0;
    cout << "Final state:" << endl;
    cout << "\tpc=" << setw(5) << setfill(' ') << pc << endl;
    for (int i = 0; i < 8; i++)
    {
        cout << "\t$" << i << "=" << setw(5) << setfill(' ') << reg[i] << endl;
    }
    // output 8 memory pointers in 16 rows for a total of 128
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << hex << setw(4) << setfill('0') << ram[pc_output] << " ";
            pc_output++;
        }
        cout << endl;
    }
}