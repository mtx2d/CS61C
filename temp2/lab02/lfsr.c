#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */
    int zeroBit = ((*reg) & 1) != 0;
    int twoBit = ((*reg) & 4) != 0;
    int threeBit = ((*reg) & 8) != 0;
    int fiveBit = ((*reg) & 32) != 0;
    int fifteenBit = (((zeroBit ^ twoBit) ^ threeBit) ^ fiveBit);
    *reg = (*reg) >> 1;
    *reg = (*reg & ~(1 << 15)) | (fifteenBit << 15);
}

