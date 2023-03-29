#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
using namespace std;

// initial RAM, PC, and registers

uint16_t ram[8192];
uint16_t pc = 0;

const uint16_t r0 = 0;
uint16_t r1 = 0;
uint16_t r2 = 0;
uint16_t r3 = 0;
uint16_t r4 = 0;
uint16_t r5 = 0;
uint16_t r6 = 0;
uint16_t r7 = 0;

// opcode values

int op_add = 0;
int op_sub = 1;
int op_or = 2;
int op_and = 3;
int op_slt = 4;
int op_jr = 5;

int op_slti = 6;
int op_lw = 7;
int op_sw = 8;
int op_jeq = 9;
int op_addi = 10;

int op_j = 11;
int op_jal = 12;

int op_fill = 13;

int op_none = 14;

// arguments

int rgA; // also rgSrc, rgAddr, reg
int rgB;
int rgDst;
int imm;

void load_code(string filename);
int opcode(uint16_t code);

int main(int argc, char *argv[])
{
    bool halt = false;
    int op;
    uint16_t code;
    load_code(argv[1]);
    while (!halt)
    {
        
        code = ram[pc & 0b1111111111111];
        op = opcode(code);
        if (op == op_j && (code & 0b1111111111111) == imm)
        {
            halt = true;
        }
        cout << code << endl;
        pc++;
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

int opcode(uint16_t code)
{
    imm = code & 0b1111111;
    rgB = (code >> 7) & 0b111;
    rgA = (code >> 10) & 0b111;
    switch ((code >> 13) & 0b111)
    {
    // opcode in least sig 4 bits
    case (0):
        rgDst = (code >> 4) & 0b111;
        rgB = (code >> 7) & 0b111;
        rgA = (code >> 10) & 0b111;
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