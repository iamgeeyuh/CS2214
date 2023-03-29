#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
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
int rgDst;
int imm;

void load_code(string filename);
int decode(uint16_t code);
int execute(int op);

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
        cout << code << endl;
        pc += pc_inc;
    }
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
    rgB = (code >> 7) & 0b111;
    rgA = (code >> 10) & 0b111;
    switch ((code >> 13) & 0b111)
    {
    // opcode in least sig 4 bits
    case (0):
        rgDst = (code >> 4) & 0b111;
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
        if (rgDst != 0)
        {
            reg[rgDst] = reg[rgA] + reg[rgB];
        }
    case (op_sub):
        if (rgDst != 0)
        {
            reg[rgDst] = reg[rgA] - reg[rgB];
        }
    case (op_or):
        if (rgDst != 0)
        {
            reg[rgDst] = reg[rgA] | reg[rgB];
        }
    case (op_and):
        if (rgDst != 0)
        {
            reg[rgDst] = reg[rgA] & reg[rgB];
        }
    case (op_slt):
        if (rgDst != 0)
        {
            reg[rgDst] = reg[rgA] < reg[rgB];
        }
    case (op_jr):
        return (reg[rgA]);
    }

    return 1;
}