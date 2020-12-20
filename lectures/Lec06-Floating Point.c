Lecture 6-Floating Point

Representation of Fractions
- Binary Point: Decimal point signals boundary
	- xx.yyyy
		- Largest possible # stored: 2 15/16
	- Example: 0b10.1010 = 1*2^(1) + 1*2^(-1) + 1 * 2^(-3) = 2.625
	- Addition made easy: 0b01.100 + 0b00.100 = 0b10.000 (1.5 + 0.5 = 2.0)
	- Multiplication operates similarly but need to track binary point
- Floating point passes in an input to represent where the binary point is
- Benefits of a floating binary point: allows more effective use of bits
	- Floating point: numerals have an exponent field to locate binary point
	- Binary point can be outside stored bits (allowing for small & large numbers)
- Normal scientific notation: 6.02 * 10^(23)
	- 6: mantissa, .: decimal point, 10: radix/base, 23: exponent
	- Normal form: there is 1 digit to the left of the decimal point
- Binary scientific notation: 1.01 * 2^(-1)
	-MSB 1: Mantissa, .: binary point, 2: radix/base, -1: exponent
- Format: +1.xxxxx*2^(yyyyy)
	- 1 is there by default (does not require bits to be allocated for it)
	- 31: sign bit, 23-30: exponents bit, 0-22: exponent (significand)
	- Can now represent positive and negative numbers from 1.2*10^-38 to 3.4*10^38
	- OVERFLOW: Trying to represent the range (3.4*10^38, inf) or (-inf, 3.4*10^38)
	- UNDERFLOW: Trying to represent [0, 1.2*10^-38) or [1.2*10^-38, 0)

IEEE 754 Floating Point Standard
- 0-22: significand, 23-30 exponent, 31: sign bit
- Has a leading 1 implicit for normalized # to allow more bits to be included
	- Single: 1 + 23 bits, Double: 1 + 52 bits
- Normalized numbers property: 0 < significand < 1
- All zeroes in the exponent is reserved exclusively to represent 0
- Biased exponent representation: used b/c Twos Complement made negative numbers look bigger
	- Uses bias of 127 for single precision (subtract 127 from exponent field to get value)
		- (-1)^s * (1+significant)*2^(exponent-127)
	- Uses bias of 1023 for double precision (subtract 1023 from exponent)

Special numbers
- Divide by 0 = +/- inf, not overflow, allowing for computation w/ infinity
- IEEE 754 standard: the highest positive exponent reserved for infinity, with significands all 0
- Representing 0: exponent all zeroes, significant all zeros, significand can be anything
- Exponent = 0, Significand = 0: Object = 0
- Exponent = 0, Significand != 0: Object = ??
- Exponent = (1-254), Significand = anything: Object = +/- floating point #
- Exponent = 255, Sigificand = 0: Object = +/- infinity
- Exponent = 255, Significand != 0: Object = ??
- Question: how to use the exponent = 255, significand != 0 cases more efficiently?
	- 0/0 or sqrt(-4) results in Not a Number(NaN)--Exponent = 255, Significand != 0
	- NaN will stay a NaN regardless of what operation is done to it
		- 23 significand bits can be used to represent different NaN sources, line where error occurred, etc.
- Problem: difficult to represent FP numbers close to zero (want representable numbers to occur at regular increments)
	- Smallest representatable positive number: 1.00000 * 2 ^-126
	- Second smallest representable positive #: 1.000...1 * 2^-126
	= (1+0.000...1)*(2^-126) = (1+2^-23)*2*-126=2^-126+2^-149
	- Gap b/w 0 and smallest = 2^-126
	- Gap b/w smallest & second smallest = 1.00000 * 2 ^-126 - (2^-126+2^-149) = 2^-149
		- We go from a gap of 2^-126 to a gap of 2^-149
		- Gaps b/w remaining numbers will be 2^-149
		- Why did the gap shrink so much?
- Solution: Use the exponent = 0, sigificand != 0 bits to fill in the nums near zero gaps
	- Denormalized number: no leading 1, implicit exponent = -126
		- Numbers take the form: 0.xxxxx * 2^-126
		- Smallest arithmetic: 0.0000...1 & 2^(-126) = 2^-23 * 2^-126 = 2^-149
	- Smallest representable positive number: 2^-149
	- Second smallest representable positive number: 2^-148
- Exponent = 0, Significand != 0 --> Object = Denormalized
- Exponent = 255, Significand != 0 --> Object = NaN

Examples
- Convert to decimal: 1, 1000 0001, 111 0000 0000...
	- (-1)^S * (1+Significand)*2^(exponent-127)
	= -1 * (1+0.111)base2*(2^(129-127))
	= -1 * (1.111)base2*(2^2)
	= -1 * 1.875 * 4 = -7.5
- Representing 1/3 = 0.3333... = 0.25 + 0.0625 + 0.015625...
	= 2^-2 + 2^-4 + 2^-6... 
	= 0.01010101... (base2)*  2^0
	= 1.010101... (base2) * 2^-2 (normalized)
	= 1, 01111101, 010101...
-8 9/16 --> -1000.1001 --> -1.0001001 * 2^(3) = -1.0001001 * 2^(130-127)
	- Sign: 1, exponent: 1000 0010, significand: 0001 0010 ...
	- 1100 0001 0000 1001 zeroes...
	- 0xC1090000
- 0x404C0000 = 0100 0000 0100 1100 ... = 0, 1000 0000, 1001 1000
	- Sign: 0, Exponent: 1000 000, Significant: 1001 1000
	--> +1.10011000* 2^(128-127) = 11.0011 = 3.1875

Floating Point Arithmetic & Precision/Accuracy
- Floating point addition is NOT associative b/c of approximation
- Example: x = -1.5*10^38, y = 1.5*10^38, z = 1.0
	- x+(y+z) = -1.5*10^38+(1.5*10^38+1.0) = -1.5*10^38 + 1.5*10^38 = 0
		- 1.0 is so much smaller that the result is rounded
	- (x+y)+z = (-1.5*10^38+1.5*10^38) + 1.0 = 0 + 1.0 = 1.0
- PRECISION: count of # of bits to represent a value
- ACCURACY: difference b/w a number and computer representation
- Generally, high precision allows for (but not guaranteeing) high accuracy
- IEEE Rounding modes: round towards +inf, round towards -inf, truncate, unbiased (gradeschool rounding)
	- Example: 0101xxx where xxx ranges from 000-111. Halfway: xxx = 100
	- 0101[100]--> rounding to even means looking at the bit to the right of the MSB of x.
	  Since it is 1 (odd #) --> We round up 0101[100] --> 0110[000]
	- 010[10] --> rounding to even means looking at bit position 3, seeing it is 0, so
	  round the number down: 010[10] --> 01000
	- This unbiased rounding allows for rounding up and rounding down--balancing inaccuracies
- Rounding also takes place w/ converting b/w double->single precision, floating point -> integer
- Adding with floating points: denormalize to match exponents, add sigificands
  keep the exponent, normalize
- (int) floatingPointExpr: forces conversion to nearest integer
- (float) integerExpr: forces conversion to nearest floating point 
- i == (int)((float)i) is not necessarily true b/c large int values do not have exact FP reps
- f == float((int)f) is not necessarily true b/c floats < 1 do not have int reps or have rounding errors

Alternative Representations
- binary64: 64 bits (contains 1 sign bit, 11 exponent bits, 52 sigificand bits)
- Double precision: represents # from [2*10^-308, 2*10^308]--greater accuracy from 52 bit significand
- Quad precision (binary128): 128 bits (15 exponent, 112 significand), Oct precision: 19 exponent, 236 significand
- Half precision: 1/5/10 bit scheme, bfloat16: 1/8/7 scheme, TF32: 1/8/18
- Dif domain accelerators support dif representations (need to be careful that rep is valid in system)
- Unum: creating a method to toggle ratio of exponent to significand bits
	- u-bit: bit that indicates if number is exact or range (0 if unrounded, 1 if rounded)


Key Takeaways
- Formula: (-1)^S * (1+significand)base2 * 2^(exponent-127)
- Exponent tells significant what to count by
- Larger exponent -> each incremental change in significand translates to larger decimal representation change


