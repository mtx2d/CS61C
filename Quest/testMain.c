#include <inttypes.h>
#include <stdio.h>
//extern uint64_t bit_manip(uint64_t);

uint64_t bit_manip(uint64_t num) {

	// Separate into 3 groups
    uint64_t lower = num & 0x7FFFFFFF;
    uint64_t mid = (num >> 31) & 0x7FFFFFFF;
    uint64_t upper = (num >> 62) & 0x7FFFFFFF;
    printf("%llu %llu %llu\n", upper, mid, lower);

    // Rotate left 16 bits
    uint64_t lowerBot15 = lower & 0x7FFF;
    uint64_t lowerTop16 = (lower >> 15) & 0xFFFF;
    lower = (lowerBot15 << 16) | lowerTop16;

    uint64_t midBot15 = mid & 0x7FFF;
    uint64_t midTop16 = (mid >> 15) & 0xFFFF;
    mid = (midBot15 << 16) | midTop16;

    uint64_t upperBot15 = upper & 0x7FFF;
    uint64_t upperTop16 = (upper >> 15) & 0xFFFF;
    upper = (upperBot15 << 16) | upperTop16;

    printf("%llu %llu %llu\n", upper, mid, lower);

    // Turn on bit 17
    lower = lower | (1 << 17);
    mid = mid | (1 << 17);
    upper = upper | (1 << 17);

    printf("%llu %llu %llu\n", upper, mid, lower);

    // Turn off bit 27
    lower = lower & ~(1 << 27);
    mid = mid & ~(1 << 27);
    upper = upper & ~(1 << 27);

    printf("%llu %llu %llu\n", upper, mid, lower);

    // Return final value--Can only return 2 least significant bits of the new upper value
    upper = upper & 3;

    printf("%llu %llu %llu\n", upper, mid, lower);
    printf("%llu\n", lower | (mid << 31) | (upper << 62));
    uint64_t result = lower | (mid << 31) | (upper << 62);
    return lower | (mid << 31) | (upper << 62);
}


int main(int argc, char *argv[]) {
	uint64_t t1 = 0xFFFFFFFF30000000;
    bit_manip(t1);
}