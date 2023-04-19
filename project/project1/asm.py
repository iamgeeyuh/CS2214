#!/usr/bin/python3

import sys
import converters


def extract_code():
    """
    Extracts assembly instructions from a file and returns a list.

    This function filters the contents of the specified file to isolate the assembly instructions.
    Comments and empty lines are ignored. The resulting instructions are returned as a list.

    Returns
    -------
    list
        A list of assembly instructions extracted from the specified file.
    """
    filename = sys.argv[1]
    input_file = open(filename, "r")
    contents = []
    # filters and isolates the assembly code
    for line in input_file:
        if line[0] != "#" and line != "\n":
            # gets rid of comments
            if line.find("#") != -1:
                line = line[: line.find("#")]
            # gets rid of whitespace
            line = line.lstrip().rstrip()
            contents.append(line)
    input_file.close()
    return contents


def assembly_to_machine(assembly):
    """
    Converts assembly code to machine code.

    This function converts the input assembly code to machine code using appropriate conversion functions
    based on each instruction's opcode. Labels and their addresses are also resolved to handle jump or
    branch instructions. The resulting machine code instructions are returned as a list.

    Parameters
    ----------
    assembly: list
        A list of assembly instructions.

    Returns
    -------
    list
        A list of corresponding machine code instructions.
    """
    # dictionary of labels and their addresses
    labels = find_labels(assembly)
    machine = []
    pc = 0
    for i in range(len(assembly)):
        # skip label
        if assembly[i].find(":") != -1:
            if assembly[i][-1] == ":":
                continue
            # cut label out of instruction
            assembly[i] = assembly[i][assembly[i].rfind(":") + 1 :].lstrip()
        # separate opcode and argument
        instructions = assembly[i].split()
        opcode = instructions[0]
        arguments = "".join(instructions[1:]).split(",")
        # use converters function based on opcode
        if opcode in ["add", "sub", "or", "and", "slt"]:
            machine.append(converters.add_sub_etc(opcode, arguments))
        elif opcode in ["lw", "sw"]:
            machine.append(converters.lw_sw(opcode, arguments, labels))
        elif opcode in ["slti", "addi"]:
            machine.append(converters.slti_addi(opcode, arguments, labels))
        elif opcode == "jeq":
            machine.append(converters.jeq(arguments, labels, pc))
        elif opcode == "jr":
            machine.append(converters.jr(arguments))
        elif opcode in ["jal", "j"]:
            machine.append(converters.jal_j(opcode, arguments, labels))
        elif opcode == "movi":
            machine.append(converters.movi(arguments, labels))
        elif opcode == "nop":
            # nop = add $0, $0, $0
            machine.append(converters.add_sub_etc("add", ["$0", "$0", "$0"]))
        elif opcode == "halt":
            # halt = j pc
            machine.append(converters.jal_j("j", [pc], labels))
        elif opcode == ".fill":
            machine.append(converters.fill(arguments, labels))
        pc += 1
    return machine


def find_labels(assembly):
    """
    Finds the addresses of labels in given assembly code.

    This function takes a list of assembly instructions as input and returns a dictionary of labels and their
    corresponding addresses. The addresses are calculated as the line number of the label in the list, accounting
    for any labels that are not on the same line as an instruction. The resulting dictionary is returned by
    the function.

    Parameters
    ----------
    assembly: list
        A list of assembly instructions.

    Returns
    -------
    dict
        A dictionary of labels and their corresponding addresses.
    """
    labels = {}
    offset = 0
    # offset accounts for labels that aren't on the same line as an instruction
    for i in range(len(assembly)):
        if assembly[i].find(":") != -1:
            # list of labels
            found_labels = assembly[i][:assembly[i].rfind(":") + 1].split()
            for label in found_labels:
                # address of a label is its index - offset
                labels[label[:-1].lower()] = i - offset
            # offset increments when a label is found on its own line
            if assembly[i][-1] == ":":
                offset += 1
    return labels


def output_machine(code):
    """
    Prints formatted machine code instructions.

    This function takes a list of machine code instructions as input and prints them to the console. Each instruction is
    printed starting with the index of the instruction in the RAM followed by its 16-bit binary representation. The function
    does not return anything.

    Parameters
    ----------
    code: list
        A list of machine code instructions.
    """
    with open("C:\\Users\\Jia\\Desktop\\cs2214\\project\\test.bin", "wb") as f:
        for i in range(len(code)):
            # ram[i] = 16'b code[i] ;
            f.write(bytes("ram[{}] = 16'b{};\n".format(i, code[i]), "utf-8"))
            # print("ram[{}] = 16'b{};".format(i, code[i]))



def main():
    """
    Converts assembly code to machine code and outputs it to the console.

    This function reads the assembly code from a file, converts it to machine code, and outputs the resulting
    code to the console in a specific format. It uses the `extract_code()`, `assembly_to_machine()`, and
    `output_machine()` functions to perform the conversion and output. The function does not return anything.
    """
    assembly = extract_code()
    machine = assembly_to_machine(assembly)
    output_machine(machine)


if __name__ == "__main__":
    main()
