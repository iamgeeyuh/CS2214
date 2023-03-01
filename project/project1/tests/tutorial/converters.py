def add_sub_etc(opcode, arguments):
    """
    Converts instructions with opcodes 'add', 'sub', 'or', 'and', and 'slt' to machine code.

    This function takes an opcode and a list of arguments as input and returns the corresponding machine code
    instruction. The opcode and the arguments are placed in the machine code format of srcA, srcB, dst, opcode. 
    The resulting machine code instruction is returned as a 16-bit binary string.

    Parameters
    ----------
    opcode: str
        The opcode of the instruction to be converted.
    arguments: list
        A list of arguments for the instruction.

    Returns
    -------
    str
        The machine code representation of the instruction.
    """
    match opcode:
        case "add":
            decode = 0b0000
        case "sub":
            decode = 0b0001
        case "or":
            decode = 0b0010
        case "and":
            decode = 0b0011
        case "slt":
            decode = 0b0100
    dst = int(arguments[0][1])
    srcA = int(arguments[1][1])
    srcB = int(arguments[2][1])
    # decode = srcA, srcB, dst, opcode
    decode += (dst << 4) + (srcB << 7) + (srcA << 10)
    return format(decode, "016b")


def lw_sw(opcode, arguments, labels):
    """
    Converts instructions with opcodes 'lw' and 'sw' to machine code.

    This function takes an opcode, a list of arguments, and a dictionary of labels as inputs and returns the
    corresponding machine code instruction. The opcode and the arguments are placed in the machine code format 
    of opcode, addr, reg, imm. The labels dictionary is used to resolve any labels in the instruction. The resulting 
    machine code instruction is returned as a 16-bit binary string.

    Parameters
    ----------
    opcode: str
        The opcode of the instruction to be converted.
    arguments: list
        A list of arguments for the instruction.
    labels: dict
        A dictionary of labels and their corresponding addresses.

    Returns
    -------
    str
        The machine code representation of the instruction.
    """
    match opcode:
        case "lw":
            decode = 0b100 << 13
        case "sw":
            decode = 0b101 << 13
    imm_addr = arguments[-1].split("(")
    reg = int(arguments[0][1])
    imm = imm_addr[0]
    addr = int(imm_addr[-1][-2])
    # checks if imm is a label
    if imm in labels:
        imm = labels[imm]
    # decode = opcode, addr, reg, imm
    decode += (int(imm)) + (reg << 7) + (addr << 10)
    return format(decode, "016b")


def slti_addi(opcode, arguments, labels):
    """
    Converts instructions with opcodes 'slti' and 'addi' to machine code.

    This function takes an opcode, a list of arguments, and a dictionary of labels as inputs and returns the
    corresponding machine code instruction. The opcode and the arguments are placed in the machine code format 
    of opcode, src, dst, imm. The labels dictionary is used to resolve any labels in the instruction. The resulting 
    machine code instruction is returned as a 16-bit binary string.

    Parameters
    ----------
    opcode: str
        The opcode of the instruction to be converted.
    arguments: list
        A list of arguments for the instruction.
    labels: dict
        A dictionary of labels and their corresponding addresses.

    Returns
    -------
    str
        The machine code representation of the instruction.
    """
    match opcode:
        case "slti":
            decode = 0b111 << 13
        case "addi":
            decode = 0b001 << 13
    dst = int(arguments[0][1])
    src = int(arguments[1][1])
    imm = arguments[2]
    # checks if imm is a label
    if imm in labels:
        imm = labels[imm]
    imm = int(imm)
    # decode = opcode, src, dst, imm
    decode = decode | abs(imm) | (dst << 7) | (src << 10)
    000010
    111110
    return format(decode, "016b")


def jeq(arguments, labels, pc):
    """
    Converts instructions with opcode 'jeq' to machine code.

    This function takes a list of arguments, a dictionary of labels, and the current value of the program counter
    as inputs and returns the corresponding machine code instruction. The arguments and pc are placed in the machine 
    code format of opcode, regA, regB, imm-pc-1. The program counter is used to calculate the relative jump offset. 
    The resulting machine code instruction is returned as a 16-bit

    Parameters
    ----------
    arguments: list
        A list of arguments for the instruction.
    labels: dict
        A dictionary of labels and their corresponding addresses.
    pc: int
        The current value of the program counter.

    Returns
    -------
    str
        The machine code representation of the instruction.
    """
    regA = int(arguments[0][1])
    regB = int(arguments[1][1])
    imm = arguments[2]
    # checks if imm is a label
    if imm in labels:
        imm = labels[imm]
    rel_imm = int(imm) - pc - 1
    # decode = opcode, regA, regB, rel_imm
    decode = (rel_imm) + (regB << 7) + (regA << 10) + (0b110 << 13)
    return format(decode, "016b")


def jr(arguments):
    """
    Converts instructions with opcode 'jr' to machine code.

    This function takes a list of arguments as input and returns the corresponding machine code instruction. The 
    arguments are placed in the machine code format of reg, opcode. The resulting machine code instruction is
    returned as a 16-bit binary string.

    Parameters
    ----------
    arguments: list
        A list of arguments for the instruction.

    Returns
    -------
    str
        The machine code representation of the instruction.
    """
    reg = int(arguments[0][1])
    # decode = reg, opcode
    decode = (reg << 10) + 0b1000
    return format(decode, "016b")


def jal_j(opcode, arguments, labels):
    """
    Converts instructions with opcodes 'j' and 'jal' to machine code.

    This function takes an opcode, a list of arguments, and a dictionary of labels as inputs and returns the
    corresponding machine code instruction. The opcode and the arguments are placed in the machine code format 
    of opcode, imm. The labels dictionary is used to resolve any labels in the instruction. The resulting machine 
    code instruction is returned as a 16-bit binary string.

    Parameters
    ----------
    opcode: str
        The opcode of the instruction to be converted.
    arguments: list
        A list of arguments for the instruction.
    labels: dict
        A dictionary of labels and their corresponding addresses.

    Returns
    -------
    str
        The machine code representation of the instruction.
    """

    match opcode:
        case "j":
            decode = 0b010 << 13
        case "jal":
            decode = 0b011 << 13
    imm = arguments[0]
    # checks if imm is a label
    if imm in labels:
        imm = labels[imm]
    # decode = opcode, imm
    decode += int(imm)
    return format(decode, "016b")


def movi(arguments, labels):
    """
    Converts instructions with opcode 'movi' to machine code.

    This function takes a list of arguments and a dictionary of labels as inputs and returns the corresponding
    machine code instruction. Movi reg, imm is equivalent to addi reg, $0, imm. The resulting machine code
    instruction is returned as a 16-bit binary string.

    Parameters
    ----------
    arguments: list
        A list of arguments for the instruction.
    labels: dict
        A dictionary of labels and their corresponding addresses.

    Returns
    -------
    str
        The machine code representation of the instruction.
    """
    # movi reg, imm = addi reg, $0, imm
    dst = arguments[0]
    src = "$0"
    imm = arguments[1]
    addi_arguments = [dst, src, imm]
    return slti_addi("addi", addi_arguments, labels)


def fill(arguments, labels):
    """
    Converts instructions with opcode '.fill' to machine code.

    This function takes a list of arguments and a dictionary of labels as inputs and returns the corresponding
    machine code instruction. The resulting machine code instruction is imm returned as a 16-bit binary string.

    Parameters
    ----------
    arguments: list
        A list of arguments for the instruction.
    labels: dict
        A dictionary of labels and their corresponding addresses.

    Returns
    -------
    str
        The machine code representation of the instruction.
    """
    imm = arguments[0]
    # checks if imm is a label
    if imm in labels:
        imm = labels[imm]
    # decode = imm
    return format(int(imm), "016b")
