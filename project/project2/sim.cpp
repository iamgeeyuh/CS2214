#include <iostream>
#include <fstream>
#include <string>
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

const int op_none = 13;

// arguments

int rgA;
int rgB;
int rgC;
uint16_t imm;

void load_code(string filename);
int decode(uint16_t code);
int execute(int op);
void output();

int main(int argc, char *argv[])
{
    bool halt = false;
    int op;
    uint16_t code;
    int mem;
    int pc_inc;

    load_code(argv[1]);
    while (!halt)
    {
        mem = pc & 0b1111111111111;
        code = ram[mem];
        op = decode(code);
        if (op == op_j && mem == imm)
        {
            halt = true;
        }
        pc_inc = execute(op);
        pc += pc_inc;
    }
    output();
}

void load_code(string filename)
{
    string line;
    ifstream file(filename);
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
}

int decode(uint16_t code)
{
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
        imm = code & 0b1111111111111;
        return op_j;
    case (3):
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

    return op_none;
}

int execute(int op)
{
    switch (op)
    {
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
        pc = (reg[rgA]);
        return 0;
    case (op_slti):
        if (rgB != 0)
        {
            reg[rgB] = reg[rgA] < imm;
        }
        break;
    case (op_lw):
        if (rgB != 0)
        {
            reg[rgB] = ram[reg[rgA] + imm];
        }
        break;
    case (op_sw):
        ram[imm + reg[rgA]] = reg[rgB];
    case (op_jeq):
        if (reg[rgA] == reg[rgB])
        {
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
    return 1;
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