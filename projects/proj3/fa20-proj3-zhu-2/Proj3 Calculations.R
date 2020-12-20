<<<<<<< HEAD
Helper Notes:
For Peter
1) csr not done - look into this
2) I type imm select double check bits are 0/1
3) redo pipelining cuz it only supports addi (PC, P+1, add another register)

=======
>>>>>>> fd337f974cfb826712db1570d5a4a3bf6ffbbdb0
R-TYPE

0: PCSel = 0, 1: RegWEn = 1, 2-4: ImmSel = 1, 5: BrUn = 0, 6: ASel = 0, 7: BSel = 1
8-11: ALUSel = OPERATION, 12: MemRW = 0, 13-14: WBSel = 1, 15: CSRSel = 0, 16: CSRWEn = 0

FORMAT: 000010[XXXX]100[000]10
000010[XXXX]10000010


I-TYPE: Main arithmetic operations

0: PCSel = 0, 1: RegWEn = 1, 2-4: ImmSel = 1, 5: BrUn = 0, 6: ASel = 0, 7: BSel = 1
8-11: ALUSel = OPERATION, 12: MemRW = 0, 13-14: WBSel = 1, 15: CSRSel = 0, 16: CSRWEn = 0

FORMAT: 000010[XXXX]100[001]10
00010[XXXX]10000110


I-TYPE: SRLI, SRAI, SLLI

0: PCSel = 0, 1: RegWEn = 1, 2-4: ImmSel = 0/1, 5: BrUn = 0, 6: ASel = 0, 7: BSel = 1
8-11: ALUSel = OP, 12: MemRW = 0, 13-14: WBSel = 1, 15: CSRSel = 0, 16: CSRWEn = 0

format: 00010[OP]100[000]10
SRLI = 4 (func3 = 5, Imm10 = 0)
SRAI = 5 (func3 = 5, Imm10 = 1)
SLLI = 6 (func3 = 1)

SRLI: 00010[0100]100[000]10
0 0010 0100 1000 0010
0[0010][0100][1000][0010] --> 02482 (if I-Type = 0), 02486 (if I-Type = 1)

SRAI: 00010[0101]100[000]10
0 0010 0101 1000 0010
0[0010][0101][1000][0010] --> 02582 (if I-Type = 0), 02586 (if I-Type = 1)

SLLI: 000010[0110]100[000]10
0[0010][0110][1000][0010] --> 02682 (if I-Type = 0), 02686 (if I-Type = 1)



Load Instructions: lb, lh, lw (opcode = 03)

0: PCSel = 0, 1: RegWEn = 1, 2-4: ImmSel = 1, 5: BrUn = 0, 6: ASel = 0, 7: BSel = 1
8-11: ALUSel = 0 (add), 12: MemRW = 0, 13-14: WBSel = 2, 15: CSRSel = 0, 16: CSRWEn = 0

Format: 00100[0000]100[001]10
0 0100 0000 1000 0110
0[0100][0000][1000][0110]
Result: 0x04086

lb func3 = 000 --> Pick out the precise byte
lh func3 = 001
lw func3 = 010 --> Load all values at specified immediate




Store Instructions: sb, sh, sw (opcode = 23)
0: PCSel = 0, 1: RegWEn = 0, 2-4: ImmSel = 2(S), 5: BrUn = 0, 6: ASel = 0, 7: BSel = 1
8-11: ALUSel = 0 (add), 12: MemRW = 1, 13-14: WBSel = 0, 15: CSRSel = 0, 16: CSRWEn = 0

Format: 00001[0000]100[010]00
0 0001 0000 1000 1000
0[0001][0000][1000][1000]
Result: 0x01088

sb func3 = 000 --> Pick the byte from immediate
sh func3 = 001 --> Pick 2 bytes from immediate
sw func3 = 010 --> WEn = 1111

Format: 25-31: offset part1, 20-24: rs2, 15-19: rs1, 12-14: funct3, 7-11: offset part2, 0-6: opcode
sb: bits 7-8 correspond to byte we are trying to access (00: 0001 = 1, 01: 0010 = 2, 10: 0100 = 4, 11 = 1000 = 8)
sh: bits 7-8 correspond to byte we are trying to access (00: 0011 = 3, 01: 0110 = 6, 10: 1100 = 0xC)

Branch Instructions (SB): beq, bne, blt, bge, bltu, bgeu
OPCODE = 63
func3: beq = 0, bne = 1, blt = 4, bge = 5, bltu = 6, bgeu = 7
- Take branch if beq, bne... condition met (PCSel = 1)
- func3 > 5 --> BrUN = 1, else: BrUN = 0

0: PCSel = ?, 1: RegWEn = 0, 2-4: ImmSel = 3(B), 5: BrUn = ?, 6: ASel = 1, 7: BSel = 1
8-11: ALUSel = 0 (add), 12: MemRW = 0, 13-14: WBSel = 0, 15: CSRSel = 0, 16: CSRWEn = 0

Format: 00000[0000]11?[011]0?

AUIPC (opcode = 17)

0: PCSel = 0, 1: RegWEn = 1, 2-4: ImmSel = 4(U), 5: BrUn = 0, 6: ASel = 1, 7: BSel = 1
8-11: ALUSel = 0 (add), 12: MemRW = 0, 13-14: WBSel = 1, 15: CSRSel = 0, 16: CSRWEn = 0

Format: 00010[0000]110[100]00
0[0010][0000][1101][0010]
Result: 0x020D2

JAL (opcode = 0x6f) [hardcode]

FIXME
0: PCSel = 1, 1: RegWEn = 1, 2-4: ImmSel = 5(UJ), 5: BrUn = 0, 6: ASel = 1, 7: BSel = 1
8-11: ALUSel = 0 (add), 12: MemRW = 0, 13-14: WBSel = 0, 15: CSRSel = 0, 16: CSRWEn = 0

Format: 00000[0000]110[101]01
0[0000][0000][1101][0111]
Result: 0x000D7


JALR (opcode = 0x67) [hardcode]

FIXME
0: PCSel = 1, 1: RegWEn = 1, 2-4: ImmSel = 1(I), 5: BrUn = 0, 6: ASel = 0, 7: BSel = 1
8-11: ALUSel = 0 (add), 12: MemRW = 0, 13-14: WBSel = 0, 15: CSRSel = 0, 16: CSRWEn = 0

Format: 00000[0000]100[001]11
0[0000][0000][1000][0111]

Result: 0x00087

LUI (opcode = 0x37) [hardcode]

FIXME
0: PCSel = 0, 1: RegWEn = 1, 2-4: ImmSel = 4 (U), 5: BrUn = 0, 6: ASel = 0 (DNC), 7: BSel = 1
8-11: ALUSel = 13 (BSel), 12: MemRW = 0, 13-14: WBSel = 1, 15: CSRSel = 0, 16: CSRWEn = 0

FORMAT: 00010[1101]100[100]10
0[0010][1101[1001][0010]
RESULT: 0x2D92


CSR (opcode = 0x63): csrw, csrwi

FIXME
0: PCSel = 0, 1: RegWEn = 0, 2-4: ImmSel = 0, 5: BrUn = 0, 6: [ASel = *, 7: BSel = *]
8-11: ALUSel = 0 (add), 12: MemRW = 0, 13-14: WBSel = 0, 15: CSRSel = 1, 16: CSRWEn = ?
- CSRWEn = 1 IFF bits 20-31 = 0x51E for CSR instructions
- CSRSel = 0 for csrw, CSRSel = 1 for csrwi
- csrw: func3 = 1, csrwi: func3 = 5


WBSel
0-PC + 4
1-ALU
2-Mem

IMMEDIATE CODES
I-1
S-2
SB(Branch)-3
U-4 (AUIPC, LUI)
UJ-5 (jal)
