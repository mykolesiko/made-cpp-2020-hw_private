#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


#define N_SIZE 64
#define EXP_SIZE 11
#define FRACT_SIZE 52
#define N_0     0x0000000000000000 // 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
#define N_0_7FF 0x00000000000007FF // 00000000 00000000 00000000 00000000 00000000 00000000 00000111 11111111
#define N_0_FFF 0x0000000000000FFF // 00000000 00000000 00000000 00000000 00000000 00000000 00001111 11111111
#define N_8_0   0x8000000000000000 // 10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
#define N_7FF_0 0x7FF0000000000000 // 01111111 11110000 00000000 00000000 00000000 00000000 00000000 00000000
#define N_FFF_0 0xFFF0000000000000 // 11111111 11110000 00000000 00000000 00000000 00000000 00000000 00000000
#define N_7FF8_0 0x7FF8000000000000 // 11111111 11111000 00000000 00000000 00000000 00000000 00000000 00000000


/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return (*(uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    /// Your code here...
    return (((1 << index) & number) >> index);
}


/**
 * Checkers here:
 */

bool checkForPlusZero (uint64_t number) {
    /// Your code here.
    return number == N_0;
}

bool checkForMinusZero (uint64_t number) {
    return number == N_8_0;
}

bool checkForPlusInf (uint64_t number) {
    /// Your code here.
    return (number == N_7FF_0);
}

bool checkForMinusInf (uint64_t number) {
    /// Your code here.
    return (number == N_FFF_0);
}

bool checkForPlusNormal (uint64_t number) {
    /// Your code here.
    if (!getBit (number, N_SIZE - 1))
    {
        uint64_t number_1 = (number >> FRACT_SIZE);
        number_1 = (number_1 & N_0_7FF);
        if ((number_1 != N_0) && (number_1 != N_0_7FF))return (true);
    }
    return(false);

}

bool checkForMinusNormal (uint64_t number) {
    /// Your code here.
    if (getBit (number, N_SIZE - 1))
    {
        uint64_t number_1 = (number >> FRACT_SIZE);
        number_1 = (number_1 & N_0_7FF);
        if ((number_1 != N_0) && (number_1 != N_0_7FF))return (true);
    }
    return(false);
}

bool checkForPlusDenormal (uint64_t number) {
    /// Your code here.

    if (!getBit (number, N_SIZE - 1)) {
         uint64_t number_1 =(number & (N_7FF_0));
         uint64_t number_2 =(number & (N_0_7FF));
         if ((number_1 == N_0 ) && (number_2 != 0)) return(true);
    }
    return (false);
}

bool checkForMinusDenormal (uint64_t number) {
    /// Your code here.
    if (getBit (number, N_SIZE - 1)) {
         uint64_t number_1 =(number & (N_7FF_0));
         uint64_t number_2 =(number & (N_0_7FF));
         if ((number_1 == N_0 ) && (number_2 != 0)) return(true);
    }
    return (false);
}

bool checkForSignalingNan (uint64_t number) {
    /// Your code here.
    uint64_t number_1 = number & N_7FF8_0;
    if (number_1  == N_7FF_0) return (true);
    else return(false);
}

bool checkForQuietNan (uint64_t number) {
    /// Your code here.
    if ((number & (N_7FF8_0) == N_7FF8_0)) return (true);
    else return (false);
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus regular\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus regular\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}


int main(int nargs, char** args)
{

    double number = strtod(args[1], NULL);
    classify (number);
    return 0;
}
