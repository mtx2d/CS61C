#include <inttypes.h>

'''
Write a function bit_manip that takes in an arbitrary 
64-bit unsigned integer, and for every 
N = 31 set of bits starting from LSB (considering the
number zero-extended to a multiple of N bits), 

1. Rotate them left by R = 16 bits
2. Turn on bit 17
3. Turn off bit 27 (within each group of  31 bits)
4. Return the final value.

REMINDER: the score shown will be 0/0; that is not your final grade. 
The autograder for each question will be run after the exam period is over.

As an example, if our input were a 16-bit number whose bits were 0bABCDEFGHIJKLMPQS 
and N = 5, we rotate it right by R = 1 bit, and ON = 1 and OFF = 3 then:

0bABCDEFGHIJKLMPQS ...being thought of as groups of N = 5 bits would become

A BCDEF GHIJK LMPQS ...then zero-extended to a multiple of N = 5 bits would become

0000A BCDEF GHIJK LMPQS ...after the rotate right by 1 bit, R = 1, would become

A0000 FBCDE KGHIJ SLMPQ ...after turning on bit ON = 1

A0010 FBC1E KGH1J SLM1Q ...after turning off bit OFF = 3

A0010 F0C1E K0H1J S0M1Q ...and returning the lowest 16 bits means we’d return...

0b0F0C1EK0H1JS0M1Q

'''
uint64_t bit_manip(uint64_t num) {

	// Separate into 3 groups
    uint64_t lower = num & 0x7FFFFFFF;
    uint64_t mid = (num >> 31) & 0x7FFFFFFF;
    uint64_t upper = (num >> 62) & 0x7FFFFFFF;

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

    // Turn on bit 17
    lower = lower | (1 << 17);
    mid = mid | (1 << 17);
    upper = upper | (1 << 17);

    // Turn off bit 27
    lower = lower & ~(1 << 27);
    mid = mid & ~(1 << 27);
    upper = upper & ~(1 << 27);

    // Return final value--Can only return 2 least significant bits of the new upper value
    upper = upper & 3;
    return lower | (mid << 31) | (upper << 62);
}
