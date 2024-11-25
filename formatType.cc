#include <iostream>
#include <iomanip> // for setw and setfill
#include "formatType.h"
// #include "toDecimal.h"

using namespace std;

void whatType(unsigned int *binaryCode) {
    // opcode는 32비트 명령어의 하위 7비트
    unsigned int opcode = (*binaryCode) & 0x7F;  // 하위 7비트 추출

    // opcode에 따라 명령어 형식 결정
    switch (opcode) {
        case 0x33:  // R-Type 명령어
            R_format(binaryCode);
            break;
        case 0x03:  // I-Type (Load 명령어)
            I_format(binaryCode, 1);
            break;
        case 0x13:  // I-Type (Immediate 명령어)
            I_format(binaryCode, 2);
            break;
        case 0x63:  // SB-Type (Branch 명령어)
            SB_format(binaryCode);
            break;
        case 0x6F:  // UJ-Type (JAL 명령어)
            UJ_format(binaryCode);
            break;
        case 0x37:  // U-Type (LUI 명령어)
            U_format(binaryCode, 1);
            break;
        case 0x17:  // U-Type (AUIPC 명령어)
            U_format(binaryCode, 2);
            break;
        default:
            cout << "unknown instruction" << endl;
            break;
    }
}

void R_format(unsigned int *binaryCode) {
    unsigned int instruction = *binaryCode;

    unsigned int func7 = (instruction >> 25) & 0x7F;
    unsigned int rs2 = (instruction >> 20) & 0x1F;
    unsigned int rs1 = (instruction >> 15) & 0x1F;
    unsigned int func3 = (instruction >> 12) & 0x07;
    unsigned int rd = (instruction >> 7)  & 0x1F;

    cout << std::dec;  // 10진수 출력 설정
    if (func7 == 0x00 && func3 == 0x00) {
        cout << "add x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
    } else if (func7 == 0x20 && func3 == 0x00) {
        cout << "sub x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
    } else if (func3 == 0x01) {
        cout << "sll x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
    } else if (func3 == 0x05 && func7 == 0x00) {
        cout << "srl x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
    } else if (func3 == 0x05 && func7 == 0x20) {
        cout << "sra x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
    } else if (func3 == 0x07) {
        cout << "and x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
    } else if (func3 == 0x06) {
        cout << "or x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
    } else {
        cout << "unknown instruction" << endl;
    }
}

void I_format(unsigned int *binaryCode, unsigned int opcodeType) {
    unsigned int instruction = *binaryCode;

    unsigned int imm = (instruction >> 20) & 0xFFF;
    unsigned int rs1 = (instruction >> 15) & 0x1F;
    unsigned int func3 = (instruction >> 12) & 0x07;
    unsigned int rd = (instruction >> 7)  & 0x1F;

    int immVal = static_cast<int>((imm << 20) >> 20);  // sign-extend 12-bit immediate

    cout << std::dec;  // 10진수 출력 설정
    if (opcodeType == 1) {  // Load 명령어
        if (func3 == 0x00) {
            cout << "lb x" << rd << ", " << immVal << "(x" << rs1 << ")" << endl;
        } else if (func3 == 0x01) {
            cout << "lh x" << rd << ", " << immVal << "(x" << rs1 << ")" << endl;
        } else if (func3 == 0x02) {
            cout << "lw x" << rd << ", " << immVal << "(x" << rs1 << ")" << endl;
        }
    } else if (opcodeType == 2) {  // Immediate 명령어
        if (func3 == 0x00) {
            cout << "addi x" << rd << ", x" << rs1 << ", " << immVal << endl;
        } else if (func3 == 0x02) {
            cout << "slti x" << rd << ", x" << rs1 << ", " << immVal << endl;
        } else if (func3 == 0x03) {
            cout << "sltiu x" << rd << ", x" << rs1 << ", " << immVal << endl;
        }
    } else if (opcodeType == 3) {  // JALR 명령어
        cout << "jalr x" << rd << ", " << immVal << "(x" << rs1 << ")" << endl;
    } else {
        cout << "unknown instruction" << endl;
    }
}

void S_format(unsigned int *binaryCode) {
    unsigned int instruction = *binaryCode;

    unsigned int imm4_0 = (instruction >> 7) & 0x1F;
    unsigned int func3 = (instruction >> 12) & 0x07;
    unsigned int rs1 = (instruction >> 15) & 0x1F;
    unsigned int rs2 = (instruction >> 20) & 0x1F;
    unsigned int imm11_5 = (instruction >> 25) & 0x7F;

    int immVal = (imm11_5 << 5) | imm4_0;  // 합친 12비트 immediate 값

    cout << std::dec;  // 10진수 출력 설정
    if (func3 == 0x00) {
        cout << "sb x" << rs2 << ", " << immVal << "(x" << rs1 << ")" << endl;
    } else if (func3 == 0x01) {
        cout << "sh x" << rs2 << ", " << immVal << "(x" << rs1 << ")" << endl;
    } else if (func3 == 0x02) {
        cout << "sw x" << rs2 << ", " << immVal << "(x" << rs1 << ")" << endl;
    } else {
        cout << "unknown instruction" << endl;
    }
}

void SB_format(unsigned int *binaryCode) {
    unsigned int instruction = *binaryCode;

    unsigned int imm11 = (instruction >> 7) & 0x01;
    unsigned int imm4_1 = (instruction >> 8) & 0x0F;
    unsigned int func3 = (instruction >> 12) & 0x07;
    unsigned int rs1 = (instruction >> 15) & 0x1F;
    unsigned int rs2 = (instruction >> 20) & 0x1F;
    unsigned int imm10_5 = (instruction >> 25) & 0x3F;
    unsigned int imm12 = (instruction >> 31) & 0x01;

    int immVal = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);

    cout << std::dec;  // 10진수 출력 설정
    if (func3 == 0x00) {
        cout << "beq x" << rs1 << ", x" << rs2 << ", " << immVal << endl;
    } else if (func3 == 0x01) {
        cout << "bne x" << rs1 << ", x" << rs2 << ", " << immVal << endl;
    } else {
        cout << "unknown instruction" << endl;
    }
}

void U_format(unsigned int *binaryCode, unsigned int opcodeType) {
    unsigned int instruction = *binaryCode;

    unsigned int imm = (instruction >> 12);
    unsigned int rd = (instruction >> 7) & 0x1F;

    cout << std::dec;  // 10진수 출력 설정
    if (opcodeType == 1) {
        cout << "lui x" << rd << ", " << imm << endl;
    } else if (opcodeType == 2) {
        cout << "auipc x" << rd << ", " << imm << endl;
    } else {
        cout << "unknown instruction" << endl;
    }
}

void UJ_format(unsigned int *binaryCode) {
    unsigned int instruction = *binaryCode;

    unsigned int rd = (instruction >> 7) & 0x1F;
    unsigned int imm = ((instruction >> 12) & 0xFF) |  // imm[19:12]
                       ((instruction >> 20) & 0x1) << 11 |  // imm[11]
                       ((instruction >> 21) & 0x3FF) << 1 |  // imm[10:1]
                       ((instruction >> 31) & 0x1) << 20;    // imm[20]

    cout << std::dec;  // 10진수 출력 설정
    cout << "jal x" << rd << ", " << imm << endl;
}