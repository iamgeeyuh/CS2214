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

void load_code(string filename);

int main(int argc, char *argv[])
{
    load_code(argv[1]);
    for (int i = 0; i < 8192; i++)
    {
        if (ram[i] != 0)
        {
            cout << "ram[" << i << "] = " << ram[i] << endl;
        }
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