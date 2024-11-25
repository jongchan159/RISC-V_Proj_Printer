#include "toDecimal.h"

unsigned int binaryArrToDecimal(unsigned int *binaryArr, int bits)
{
    int decimal = 0;
    int base = 1; // 2^0 = 1

    for (int i = 0; i < bits; i++) {
        decimal += binaryArr[i] * base;
        base *= 2;
    }

    return decimal;
}

int twosComplementArrToDecimal(unsigned int *twosComplement, int bits)
{
    int decimal = 0;
    int signBit = twosComplement[0]; // 첫 번째 비트는 부호 비트 (양수 또는 음수)

    // 음수인 경우 2의 보수로 바꾸기
    if (signBit == 1) {
        // 비트 반전 (1을 0으로, 0을 1로)
        for (int i = 0; i < bits; i++) {
            twosComplement[i] = (twosComplement[i] == 0) ? 1 : 0;
        }

        // 1을 더해서 2의 보수로 만들기
        int carry = 1;
        for (int i = bits - 1; i >= 0; i--) {
            int sum = twosComplement[i] + carry;
            twosComplement[i] = sum % 2;
            carry = sum / 2;
        }
    }

    // 10진수로 변환
    int base = 1;
    for (int i = bits - 1; i > 0; i--) {
        decimal += twosComplement[i] * base;
        base *= 2;
    }

    // 부호 처리
    if (signBit == 1) {
        decimal = -decimal;
    }

    return decimal;
}