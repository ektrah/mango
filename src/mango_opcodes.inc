/*
 *  _____ _____ _____ _____ _____
 * |     |  _  |   | |   __|     |
 * | | | |     | | | |  |  |  |  |
 * |_|_|_|__|__|_|___|_____|_____|
 *
 * Mango Virtual Machine 0.33-dev
 *
 * Copyright (c) 2017 Klaus Hartke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

//  OpCode              Name                Pop     Push    Args    Byte
// ----------------------------------------------------------------------
OPCODE(NOP,             "nop",              0,      0,      0,      0x00)
OPCODE(BREAK,           "break",            0,      0,      0,      0x01)

OPCODE(UNUSED2,         "unused",           0,      0,      0,      0x02)
OPCODE(UNUSED3,         "unused",           0,      0,      0,      0x03)

OPCODE(POP_X32,         "pop.x32",          1,      0,      0,      0x04)
OPCODE(POP_X64,         "pop.x64",          2,      0,      0,      0x05)
OPCODE(DUP_X32,         "dup.x32",          1,      2,      0,      0x06)
OPCODE(DUP_X64,         "dup.x64",          2,      4,      0,      0x07)
OPCODE(SWAP_X32,        "swap.x32",         2,      2,      0,      0x08)
OPCODE(SWAP_X64,        "swap.x64",         4,      4,      0,      0x09)
OPCODE(OVER,            "over",             2,      3,      0,      0x0A)
OPCODE(ROT,             "rot",              3,      3,      0,      0x0B)
OPCODE(NIP,             "nip",              2,      1,      0,      0x0C)
OPCODE(TUCK,            "tuck",             2,      3,      0,      0x0D)

OPCODE(UNUSED14,        "unused",           0,      0,      0,      0x0E)
OPCODE(UNUSED15,        "unused",           0,      0,      0,      0x0F)

OPCODE(LDLOC_X32,       "ldloc.x32",        0,      1,      1,      0x10)
OPCODE(LDLOC_X64,       "ldloc.x64",        0,      2,      1,      0x11)
OPCODE(LDLOCA,          "ldloca",           0,      1,      1,      0x12)
OPCODE(STLOC_X32,       "stloc.x32",        1,      0,      1,      0x13)
OPCODE(STLOC_X64,       "stloc.x64",        2,      0,      1,      0x14)

OPCODE(UNUSED21,        "unused",           0,      0,      0,      0x15)
OPCODE(UNUSED22,        "unused",           0,      0,      0,      0x16)
OPCODE(UNUSED23,        "unused",           0,      0,      0,      0x17)

OPCODE(RET,             "ret",              0,      0,      0,      0x18)
OPCODE(RET_X32,         "ret.x32",          1,      0,      0,      0x19)
OPCODE(RET_X64,         "ret.x64",          2,      0,      0,      0x1A)
OPCODE(CALL,            "call",             0,      0,      3,      0x1B)
OPCODE(SYSCALL,         "syscall",          0,      0,      3,      0x1C)

OPCODE(UNUSED29,        "unused",           0,      0,      0,      0x1D)
OPCODE(UNUSED30,        "unused",           0,      0,      0,      0x1E)
OPCODE(UNUSED31,        "unused",           0,      0,      0,      0x1F)

OPCODE(ADD_I32,         "add.i32",          2,      1,      0,      0x20)
OPCODE(SUB_I32,         "sub.i32",          2,      1,      0,      0x21)
OPCODE(MUL_I32,         "mul.i32",          2,      1,      0,      0x22)
OPCODE(DIV_I32,         "div.i32",          2,      1,      0,      0x23)
OPCODE(DIV_I32_UN,      "div.i32.un",       2,      1,      0,      0x24)
OPCODE(REM_I32,         "rem.i32",          2,      1,      0,      0x25)
OPCODE(REM_I32_UN,      "rem.i32.un",       2,      1,      0,      0x26)
OPCODE(NEG_I32,         "neg.i32",          1,      1,      0,      0x27)

OPCODE(ADD_I64,         "add.i64",          4,      2,      0,      0x28)
OPCODE(SUB_I64,         "sub.i64",          4,      2,      0,      0x29)
OPCODE(MUL_I64,         "mul.i64",          4,      2,      0,      0x2A)
OPCODE(DIV_I64,         "div.i64",          4,      2,      0,      0x2B)
OPCODE(DIV_I64_UN,      "div.i64.un",       4,      2,      0,      0x2C)
OPCODE(REM_I64,         "rem.i64",          4,      2,      0,      0x2D)
OPCODE(REM_I64_UN,      "rem.i64.un",       4,      2,      0,      0x2E)
OPCODE(NEG_I64,         "neg.i64",          2,      2,      0,      0x2F)

OPCODE(ADD_F32,         "add.f32",          2,      1,      0,      0x30)
OPCODE(SUB_F32,         "sub.f32",          2,      1,      0,      0x31)
OPCODE(MUL_F32,         "mul.f32",          2,      1,      0,      0x32)
OPCODE(DIV_F32,         "div.f32",          2,      1,      0,      0x33)
OPCODE(REM_F32,         "rem.f32",          2,      1,      0,      0x34)
OPCODE(NEG_F32,         "neg.f32",          1,      1,      0,      0x35)

OPCODE(ADD_F64,         "add.f64",          4,      2,      0,      0x36)
OPCODE(SUB_F64,         "sub.f64",          4,      2,      0,      0x37)
OPCODE(MUL_F64,         "mul.f64",          4,      2,      0,      0x38)
OPCODE(DIV_F64,         "div.f64",          4,      2,      0,      0x39)
OPCODE(REM_F64,         "rem.f64",          4,      2,      0,      0x3A)
OPCODE(NEG_F64,         "neg.f64",          2,      2,      0,      0x3B)

OPCODE(SHL_I32,         "shl.i32",          2,      1,      0,      0x3C)
OPCODE(SHR_I32,         "shr.i32",          2,      1,      0,      0x3D)
OPCODE(SHR_I32_UN,      "shr.i32.un",       2,      1,      0,      0x3E)
OPCODE(AND_I32,         "and.i32",          2,      1,      0,      0x3F)
OPCODE(OR_I32,          "or.i32",           2,      1,      0,      0x40)
OPCODE(XOR_I32,         "xor.i32",          2,      1,      0,      0x41)
OPCODE(NOT_I32,         "not.i32",          1,      1,      0,      0x42)

OPCODE(SHL_I64,         "shl.i64",          3,      2,      0,      0x43)
OPCODE(SHR_I64,         "shr.i64",          3,      2,      0,      0x44)
OPCODE(SHR_I64_UN,      "shr.i64.un",       3,      2,      0,      0x45)
OPCODE(AND_I64,         "and.i64",          4,      2,      0,      0x46)
OPCODE(OR_I64,          "or.i64",           4,      2,      0,      0x47)
OPCODE(XOR_I64,         "xor.i64",          4,      2,      0,      0x48)
OPCODE(NOT_I64,         "not.i64",          2,      2,      0,      0x49)

OPCODE(UNUSED74,        "unused",           0,      0,      0,      0x4A)
OPCODE(UNUSED75,        "unused",           0,      0,      0,      0x4B)
OPCODE(UNUSED76,        "unused",           0,      0,      0,      0x4C)
OPCODE(UNUSED77,        "unused",           0,      0,      0,      0x4D)
OPCODE(UNUSED78,        "unused",           0,      0,      0,      0x4E)
OPCODE(UNUSED79,        "unused",           0,      0,      0,      0x4F)

OPCODE(LDC_I32_M1,      "ldc.i32.m1",       0,      1,      0,      0x50)
OPCODE(LDC_I32_0,       "ldc.i32.0",        0,      1,      0,      0x51)
OPCODE(LDC_I32_1,       "ldc.i32.1",        0,      1,      0,      0x52)
OPCODE(LDC_I32_2,       "ldc.i32.2",        0,      1,      0,      0x53)
OPCODE(LDC_I32_3,       "ldc.i32.3",        0,      1,      0,      0x54)
OPCODE(LDC_I32_4,       "ldc.i32.4",        0,      1,      0,      0x55)
OPCODE(LDC_I32_5,       "ldc.i32.5",        0,      1,      0,      0x56)
OPCODE(LDC_I32_6,       "ldc.i32.6",        0,      1,      0,      0x57)
OPCODE(LDC_I32_7,       "ldc.i32.7",        0,      1,      0,      0x58)
OPCODE(LDC_I32_8,       "ldc.i32.8",        0,      1,      0,      0x59)
OPCODE(LDC_I32_S,       "ldc.i32.s",        0,      1,      1,      0x5A)
OPCODE(LDC_I32,         "ldc.i32",          0,      1,      4,      0x5B)
OPCODE(LDC_I64,         "ldc.i64",          0,      2,      8,      0x5C)
OPCODE(LDC_F32,         "ldc.f32",          0,      1,      4,      0x5D)
OPCODE(LDC_F64,         "ldc.f64",          0,      2,      8,      0x5E)

OPCODE(UNUSED95,        "unused",           0,      0,      0,      0x5F)

OPCODE(CONV_I8_I32,     "conv.i8.i32",      1,      1,      0,      0x60)
OPCODE(CONV_U8_I32,     "conv.u8.i32",      1,      1,      0,      0x61)
OPCODE(CONV_I16_I32,    "conv.i16.i32",     1,      1,      0,      0x62)
OPCODE(CONV_U16_I32,    "conv.u16.i32",     1,      1,      0,      0x63)
OPCODE(CONV_I64_I32,    "conv.i64.i32",     1,      2,      0,      0x64)
OPCODE(CONV_U64_I32,    "conv.u64.i32",     1,      2,      0,      0x65)
OPCODE(CONV_F32_I32,    "conv.f32.i32",     1,      1,      0,      0x66)
OPCODE(CONV_F32_I32_UN, "conv.f32.i32.un",  1,      1,      0,      0x67)
OPCODE(CONV_F64_I32,    "conv.f64.i32",     1,      2,      0,      0x68)
OPCODE(CONV_F64_I32_UN, "conv.f64.i32.un",  1,      2,      0,      0x69)

OPCODE(CONV_I8_I64,     "conv.i8.i64",      2,      1,      0,      0x6A)
OPCODE(CONV_U8_I64,     "conv.u8.i64",      2,      1,      0,      0x6B)
OPCODE(CONV_I16_I64,    "conv.i16.i64",     2,      1,      0,      0x6C)
OPCODE(CONV_U16_I64,    "conv.u16.i64",     2,      1,      0,      0x6D)
OPCODE(CONV_I32_I64,    "conv.i32.i64",     2,      1,      0,      0x6E)
OPCODE(CONV_U32_I64,    "conv.u32.i64",     2,      1,      0,      0x6F)
OPCODE(CONV_F32_I64,    "conv.f32.i64",     2,      1,      0,      0x70)
OPCODE(CONV_F32_I64_UN, "conv.f32.i64.un",  2,      1,      0,      0x71)
OPCODE(CONV_F64_I64,    "conv.f64.i64",     2,      2,      0,      0x72)
OPCODE(CONV_F64_I64_UN, "conv.f64.i64.un",  2,      2,      0,      0x73)

OPCODE(CONV_I8_F32,     "conv.i8.f32",      1,      1,      0,      0x74)
OPCODE(CONV_U8_F32,     "conv.u8.f32",      1,      1,      0,      0x75)
OPCODE(CONV_I16_F32,    "conv.i16.f32",     1,      1,      0,      0x76)
OPCODE(CONV_U16_F32,    "conv.u16.f32",     1,      1,      0,      0x77)
OPCODE(CONV_I32_F32,    "conv.i32.f32",     1,      1,      0,      0x78)
OPCODE(CONV_U32_F32,    "conv.u32.f32",     1,      1,      0,      0x79)
OPCODE(CONV_I64_F32,    "conv.i64.f32",     1,      2,      0,      0x7A)
OPCODE(CONV_U64_F32,    "conv.u64.f32",     1,      2,      0,      0x7B)
OPCODE(CONV_F64_F32,    "conv.f64.f32",     1,      2,      0,      0x7C)

OPCODE(CONV_I8_F64,     "conv.i8.f64",      2,      1,      0,      0x7D)
OPCODE(CONV_U8_F64,     "conv.u8.f64",      2,      1,      0,      0x7E)
OPCODE(CONV_I16_F64,    "conv.i16.f64",     2,      1,      0,      0x7F)
OPCODE(CONV_U16_F64,    "conv.u16.f64",     2,      1,      0,      0x80)
OPCODE(CONV_I32_F64,    "conv.i32.f64",     2,      1,      0,      0x81)
OPCODE(CONV_U32_F64,    "conv.u32.f64",     2,      1,      0,      0x82)
OPCODE(CONV_I64_F64,    "conv.i64.f64",     2,      2,      0,      0x83)
OPCODE(CONV_U64_F64,    "conv.u64.f64",     2,      2,      0,      0x84)
OPCODE(CONV_F32_F64,    "conv.f32.f64",     2,      1,      0,      0x85)

OPCODE(UNUSED134,       "unused",           0,      0,      0,      0x86)
OPCODE(UNUSED135,       "unused",           0,      0,      0,      0x87)

OPCODE(CEQ_I32,         "ceq.i32",          2,      1,      0,      0x88)
OPCODE(CNE_I32,         "cne.i32",          2,      1,      0,      0x89)
OPCODE(CGT_I32,         "cgt.i32",          2,      1,      0,      0x8A)
OPCODE(CGT_I32_UN,      "cgt.i32.un",       2,      1,      0,      0x8B)
OPCODE(CGE_I32,         "cge.i32",          2,      1,      0,      0x8C)
OPCODE(CGE_I32_UN,      "cge.i32.un",       2,      1,      0,      0x8D)
OPCODE(CLT_I32,         "clt.i32",          2,      1,      0,      0x8E)
OPCODE(CLT_I32_UN,      "clt.i32.un",       2,      1,      0,      0x8F)
OPCODE(CLE_I32,         "cle.i32",          2,      1,      0,      0x90)
OPCODE(CLE_I32_UN,      "cle.i32.un",       2,      1,      0,      0x91)

OPCODE(CEQ_I64,         "ceq.i64",          4,      1,      0,      0x92)
OPCODE(CNE_I64,         "cne.i64",          4,      1,      0,      0x93)
OPCODE(CGT_I64,         "cgt.i64",          4,      1,      0,      0x94)
OPCODE(CGT_I64_UN,      "cgt.i64.un",       4,      1,      0,      0x95)
OPCODE(CGE_I64,         "cge.i64",          4,      1,      0,      0x96)
OPCODE(CGE_I64_UN,      "cge.i64.un",       4,      1,      0,      0x97)
OPCODE(CLT_I64,         "clt.i64",          4,      1,      0,      0x98)
OPCODE(CLT_I64_UN,      "clt.i64.un",       4,      1,      0,      0x99)
OPCODE(CLE_I64,         "cle.i64",          4,      1,      0,      0x9A)
OPCODE(CLE_I64_UN,      "cle.i64.un",       4,      1,      0,      0x9B)

OPCODE(CEQ_F32,         "ceq.f32",          2,      1,      0,      0x9C)
OPCODE(CEQ_F32_UN,      "ceq.f32.un",       2,      1,      0,      0x9D)
OPCODE(CNE_F32,         "cne.f32",          2,      1,      0,      0x9E)
OPCODE(CNE_F32_UN,      "cne.f32.un",       2,      1,      0,      0x9F)
OPCODE(CGT_F32,         "cgt.f32",          2,      1,      0,      0xA0)
OPCODE(CGT_F32_UN,      "cgt.f32.un",       2,      1,      0,      0xA1)
OPCODE(CGE_F32,         "cge.f32",          2,      1,      0,      0xA2)
OPCODE(CGE_F32_UN,      "cge.f32.un",       2,      1,      0,      0xA3)
OPCODE(CLT_F32,         "clt.f32",          2,      1,      0,      0xA4)
OPCODE(CLT_F32_UN,      "clt.f32.un",       2,      1,      0,      0xA5)
OPCODE(CLE_F32,         "cle.f32",          2,      1,      0,      0xA6)
OPCODE(CLE_F32_UN,      "cle.f32.un",       2,      1,      0,      0xA7)

OPCODE(CEQ_F64,         "ceq.f64",          4,      1,      0,      0xA8)
OPCODE(CEQ_F64_UN,      "ceq.f64.un",       4,      1,      0,      0xA9)
OPCODE(CNE_F64,         "cne.f64",          4,      1,      0,      0xAA)
OPCODE(CNE_F64_UN,      "cne.f64.un",       4,      1,      0,      0xAB)
OPCODE(CGT_F64,         "cgt.f64",          4,      1,      0,      0xAC)
OPCODE(CGT_F64_UN,      "cgt.f64.un",       4,      1,      0,      0xAD)
OPCODE(CGE_F64,         "cge.f64",          4,      1,      0,      0xAE)
OPCODE(CGE_F64_UN,      "cge.f64.un",       4,      1,      0,      0xAF)
OPCODE(CLT_F64,         "clt.f64",          4,      1,      0,      0xB0)
OPCODE(CLT_F64_UN,      "clt.f64.un",       4,      1,      0,      0xB1)
OPCODE(CLE_F64,         "cle.f64",          4,      1,      0,      0xB2)
OPCODE(CLE_F64_UN,      "cle.f64.un",       4,      1,      0,      0xB3)

OPCODE(UNUSED180,       "unused",           0,      0,      0,      0xB4)
OPCODE(UNUSED181,       "unused",           0,      0,      0,      0xB5)
OPCODE(UNUSED182,       "unused",           0,      0,      0,      0xB6)
OPCODE(UNUSED183,       "unused",           0,      0,      0,      0xB7)

OPCODE(BR,              "br",               0,      0,      2,      0xB8)
OPCODE(BR_S,            "br.s",             0,      0,      1,      0xB9)
OPCODE(BRFALSE,         "brfalse",          1,      0,      2,      0xBA)
OPCODE(BRFALSE_S,       "brfalse.s",        1,      0,      1,      0xBB)
OPCODE(BRTRUE,          "brtrue",           1,      0,      2,      0xBC)
OPCODE(BRTRUE_S,        "brtrue.s",         1,      0,      1,      0xBD)

OPCODE(UNUSED190,       "unused",           0,      0,      0,      0xBE)
OPCODE(UNUSED191,       "unused",           0,      0,      0,      0xBF)

OPCODE(NEWOBJ,          "newobj",           0,      1,      3,      0xC0)
OPCODE(NEWARR,          "newarr",           1,      2,      3,      0xC1)
OPCODE(MKSLICE,         "mkslice",          2,      2,      0,      0xC2)
OPCODE(SLICE,           "slice",            3,      2,      0,      0xC3)
OPCODE(SLICE2,          "slice2",           4,      2,      0,      0xC4)
OPCODE(LDLEN,           "ldlen",            2,      1,      0,      0xC5)

OPCODE(UNUSED198,       "unused",           0,      0,      0,      0xC6)
OPCODE(UNUSED199,       "unused",           0,      0,      0,      0xC7)

OPCODE(LDFLD_I8,        "ldfld.i8",         1,      1,      2,      0xC8)
OPCODE(LDFLD_U8,        "ldfld.u8",         1,      1,      2,      0xC9)
OPCODE(LDFLD_I16,       "ldfld.i16",        1,      1,      2,      0xCA)
OPCODE(LDFLD_U16,       "ldfld.u16",        1,      1,      2,      0xCB)
OPCODE(LDFLD_X32,       "ldfld.x32",        1,      1,      2,      0xCC)
OPCODE(LDFLD_X64,       "ldfld.x64",        1,      2,      2,      0xCD)

OPCODE(LDFLDA,          "ldflda",           1,      1,      2,      0xCE)

OPCODE(STFLD_I8,        "stfld.i8",         2,      0,      2,      0xCF)
OPCODE(STFLD_I16,       "stfld.i16",        2,      0,      2,      0xD0)
OPCODE(STFLD_X32,       "stfld.x32",        2,      0,      2,      0xD1)
OPCODE(STFLD_X64,       "stfld.x64",        3,      0,      2,      0xD2)

OPCODE(LDSFLD_I8,       "ldsfld.i8",        0,      1,      2,      0xD3)
OPCODE(LDSFLD_U8,       "ldsfld.u8",        0,      1,      2,      0xD4)
OPCODE(LDSFLD_I16,      "ldsfld.i16",       0,      1,      2,      0xD5)
OPCODE(LDSFLD_U16,      "ldsfld.u16",       0,      1,      2,      0xD6)
OPCODE(LDSFLD_X32,      "ldsfld.x32",       0,      1,      2,      0xD7)
OPCODE(LDSFLD_X64,      "ldsfld.x64",       0,      2,      2,      0xD8)

OPCODE(LDSFLDA,         "ldsflda",          0,      1,      2,      0xD9)

OPCODE(STSFLD_I8,       "stsfld.i8",        1,      0,      2,      0xDA)
OPCODE(STSFLD_I16,      "stsfld.i16",       1,      0,      2,      0xDB)
OPCODE(STSFLD_X32,      "stsfld.x32",       1,      0,      2,      0xDC)
OPCODE(STSFLD_X64,      "stsfld.x64",       2,      0,      2,      0xDD)

OPCODE(LDELEM_I8,       "ldelem.i8",        3,      1,      0,      0xDE)
OPCODE(LDELEM_U8,       "ldelem.u8",        3,      1,      0,      0xDF)
OPCODE(LDELEM_I16,      "ldelem.i16",       3,      1,      0,      0xE0)
OPCODE(LDELEM_U16,      "ldelem.u16",       3,      1,      0,      0xE1)
OPCODE(LDELEM_X32,      "ldelem.x32",       3,      1,      0,      0xE2)
OPCODE(LDELEM_X64,      "ldelem.x64",       3,      2,      0,      0xE3)

OPCODE(LDELEMA,         "ldelema",          3,      1,      3,      0xE4)
OPCODE(LDELEMA_X8,      "ldelema.x8",       3,      1,      0,      0xE5)
OPCODE(LDELEMA_X16,     "ldelema.x16",      3,      1,      0,      0xE6)
OPCODE(LDELEMA_X32,     "ldelema.x32",      3,      1,      0,      0xE7)
OPCODE(LDELEMA_X64,     "ldelema.x64",      3,      1,      0,      0xE8)

OPCODE(STELEM_I8,       "stelem.i8",        4,      0,      0,      0xE9)
OPCODE(STELEM_I16,      "stelem.i16",       4,      0,      0,      0xEA)
OPCODE(STELEM_X32,      "stelem.x32",       4,      0,      0,      0xEB)
OPCODE(STELEM_X64,      "stelem.x64",       5,      0,      0,      0xEC)

OPCODE(UNUSED237,       "unused",           0,      0,      0,      0xED)
OPCODE(UNUSED238,       "unused",           0,      0,      0,      0xEE)
OPCODE(UNUSED239,       "unused",           0,      0,      0,      0xEF)
OPCODE(UNUSED240,       "unused",           0,      0,      0,      0xF0)
OPCODE(UNUSED241,       "unused",           0,      0,      0,      0xF1)
OPCODE(UNUSED242,       "unused",           0,      0,      0,      0xF2)
OPCODE(UNUSED243,       "unused",           0,      0,      0,      0xF3)
OPCODE(UNUSED244,       "unused",           0,      0,      0,      0xF4)
OPCODE(UNUSED245,       "unused",           0,      0,      0,      0xF5)
OPCODE(UNUSED246,       "unused",           0,      0,      0,      0xF6)
OPCODE(UNUSED247,       "unused",           0,      0,      0,      0xF7)
OPCODE(UNUSED248,       "unused",           0,      0,      0,      0xF8)
OPCODE(UNUSED249,       "unused",           0,      0,      0,      0xF9)
OPCODE(UNUSED250,       "unused",           0,      0,      0,      0xFA)
OPCODE(UNUSED251,       "unused",           0,      0,      0,      0xFB)
OPCODE(UNUSED252,       "unused",           0,      0,      0,      0xFC)
OPCODE(UNUSED253,       "unused",           0,      0,      0,      0xFD)
OPCODE(UNUSED254,       "unused",           0,      0,      0,      0xFE)

OPCODE(HALT,            "halt",             0,      0,      0,      0xFF)
