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
            op = 0b0000
        case "sub":
            op = 0b0001
        case "or":
            op = 0b0010
        case "and":
            op = 0b0011
        case "slt":
            op = 0b0100
    dst = int(arguments[0][1])
    srcA = int(arguments[1][1])
    srcB = int(arguments[2][1])
    # decode = srcA, srcB, dst, opcode
    decode = op | (dst << 4) | (srcB << 7) | (srcA << 10)
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
            op = 0b100
        case "sw":
            op = 0b101
    imm_addr = arguments[-1].split("(")
    reg = int(arguments[0][1])
    addr = int(imm_addr[-1][-2])
    imm = imm_preparer(imm_addr[0], 7, labels)
    # decode = opcode, addr, reg, imm
    decode = imm | (reg << 7) | (addr << 10) | (op << 13)
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
            op = 0b111
        case "addi":
            op = 0b001
    dst = int(arguments[0][1])
    src = int(arguments[1][1])
    imm = imm_preparer(arguments[2], 7, labels)
    # decode = opcode, src, dst, imm
    decode = imm | (dst << 7) | (src << 10) | (op << 13)
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
    # if rel_imm is negative then convert to twos comp
    if rel_imm < 0:
        rel_imm = twos_comp(rel_imm, 7)
    # decode = opcode, regA, regB, rel_imm
    decode = (rel_imm) | (regB << 7) | (regA << 10) | (0b110 << 13)
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
    decode = (reg << 10) | 0b1000
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
            op = 0b010
        case "jal":
            op = 0b011
    imm = imm_preparer(arguments[0], 13, labels)
    # decode = opcode, imm
    decode = imm | (op << 13)
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
    imm = imm_preparer(arguments[0], 16, labels)
    return format(imm, "016b")


def twos_comp(num, bits):
    """
    Computes the two's complement of a given integer with a specified number of bits.

    This function takes a the absolute value of a given integer and adds 1 to its negation to find the twos
    complement. The resulting twos complement representation of the given integer is returned.

    Parameters
    ----------
    num: int
        The integer to compute the two's complement of.
    bits: int
        The number of bits to use in the two's complement representation.

    Returns
    -------
    int
        The two's complement of the input integer.
    """
    # converts num to absolute value with specified number of bits
    abs_num = format(abs(num), "0{}b".format(bits))
    # negates the absolute value
    neg_abs_num = ""
    for bit in abs_num:
        if bit == "1":
            neg_abs_num += "0"
        else:
            neg_abs_num += "1"
    # twos_comp = ~(abs(num)) + 1
    twos_comp = int("0b{}".format(neg_abs_num), 2) + 1
    return twos_comp


def imm_preparer(imm, bits, labels):
    """
    Prepares an immediate value for use in an instruction.

    This function prepares a given immediate value for use in an instruction by converting it to an integer
    and converting it to two's complement if it is negative. If the immediate is a label, the corresponding
    value from the labels dictionary is used.

    Parameters
    ----------
    imm: str
        The immediate value to prepare.
    bits: int
        The number of bits available to represent the immediate value.
    labels: dict
        A dictionary mapping label names to their corresponding values.

    Returns
    -------
    int
        The prepared immediate value.
    """
    # checks if imm is a label
    if imm in labels:
        imm = labels[imm]
    imm = int(imm)
    # converts to twos complement is imm is negative
    if imm < 0:
        imm = twos_comp(imm, bits)
    return imm
