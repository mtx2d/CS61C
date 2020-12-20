#include <inttypes.h>
#include <stdio.h>
//extern uint64_t bit_manip(uint64_t);

uint64_t bit_manip(uint64_t num) {

	// Separate into 3 groups
    uint64_t lower = num & 0x7FFFFFFF;
    uint64_t mid = (num >> 31) & 0x7FFFFFFF;
    uint64_t upper = (num >> 62) & 0x7FFFFFFF;

    // Rotate left 16 bits
    lowerBot15 = lower & 0x7FFF;
    lowerTop16 = (lower >> 15) & 0xFFFF;
    lower = (lowerBot15 >> 16) | lowerTop16;

    midBot15 = mid & 0x7FFF;
    midTop16 = (mid >> 15) & 0xFFFF;
    mid = (midBot15 >> 16) | midTop16;

    upperBot15 = upper & 0x7FFF;
    upperTop16 = (upper >> 15) & 0xFFFF;
    upper = (upperBot15 >> 16) | upperTop16;

    // Turn on bit 17
    lower = lower |  (1 << 17);
    mid = mid | (1 << 17);
    upper = upper | (1 << 17);

    // Turn off bit 27
    lower = lower & ~(1 << 27);
    mid = mid & ~(1 <, 27);
    upper = upper & ~(1 << 27);

    // Return final value--Can only return 2 least significant bits of the new upper value
    upper = upper & 3;
    return lower | (mid << 31) | (upper << 62);
}


int main(int argc, char *argv[]) {
    bit_manip(20);
}