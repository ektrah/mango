/*
 * Mango Virtual Machine 0.32-dev
 *
 * Copyright (c) 2016 Klaus Hartke
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
OPCODE(BREAK,           "break",            0,      0,      0,      0x00)
OPCODE(HALT,            "halt",             0,      0,      0,      0x00)

OPCODE(POP,             "pop",              1,      0,      0,      0x00)
OPCODE(POP2,            "pop2",             2,      0,      0,      0x00)
OPCODE(DUP,             "dup",              1,      2,      0,      0x00)
OPCODE(DUP2,            "dup2",             2,      4,      0,      0x00)
OPCODE(SWAP,            "swap",             2,      2,      0,      0x00)
OPCODE(SWAP2,           "swap2",            4,      4,      0,      0x00)
OPCODE(OVER,            "over",             2,      3,      0,      0x00)
OPCODE(ROT,             "rot",              3,      3,      0,      0x00)
OPCODE(NIP,             "nip",              2,      1,      0,      0x00)
OPCODE(TUCK,            "tuck",             2,      3,      0,      0x00)

OPCODE(LDLOC,           "ldloc",            0,      1,      1,      0x00)
OPCODE(LDLOC2,          "ldloc2",           0,      2,      1,      0x00)
OPCODE(STLOC,           "stloc",            1,      0,      1,      0x00)
OPCODE(STLOC2,          "stloc2",           2,      0,      1,      0x00)

OPCODE(RET,             "ret",              0,      0,      0,      0x00)
OPCODE(RET1,            "ret1",             1,      0,      0,      0x00)
OPCODE(RET2,            "ret2",             2,      0,      0,      0x00)
OPCODE(CALL,            "call",             0,      0,      3,      0x00)
OPCODE(SYSCALL,         "syscall",          0,      0,      2,      0x00)

OPCODE(ADD_I32,         "add.i32",          2,      1,      0,      0x00)
OPCODE(SUB_I32,         "sub.i32",          2,      1,      0,      0x00)
OPCODE(MUL_I32,         "mul.i32",          2,      1,      0,      0x00)
OPCODE(DIV_I32,         "div.i32",          2,      1,      0,      0x00)
OPCODE(DIV_U32,         "div.u32",          2,      1,      0,      0x00)
OPCODE(REM_I32,         "rem.i32",          2,      1,      0,      0x00)
OPCODE(REM_U32,         "rem.u32",          2,      1,      0,      0x00)
OPCODE(NEG_I32,         "neg.i32",          1,      1,      0,      0x00)

OPCODE(ADD_I64,         "add.i64",          4,      2,      0,      0x00)
OPCODE(SUB_I64,         "sub.i64",          4,      2,      0,      0x00)
OPCODE(MUL_I64,         "mul.i64",          4,      2,      0,      0x00)
OPCODE(DIV_I64,         "div.i64",          4,      2,      0,      0x00)
OPCODE(DIV_U64,         "div.u64",          4,      2,      0,      0x00)
OPCODE(REM_I64,         "rem.i64",          4,      2,      0,      0x00)
OPCODE(REM_U64,         "rem.u64",          4,      2,      0,      0x00)
OPCODE(NEG_I64,         "neg.i64",          2,      2,      0,      0x00)

OPCODE(ADD_F32,         "add.f32",          2,      1,      0,      0x00)
OPCODE(SUB_F32,         "sub.f32",          2,      1,      0,      0x00)
OPCODE(MUL_F32,         "mul.f32",          2,      1,      0,      0x00)
OPCODE(DIV_F32,         "div.f32",          2,      1,      0,      0x00)
OPCODE(REM_F32,         "rem.f32",          2,      1,      0,      0x00)
OPCODE(NEG_F32,         "neg.f32",          1,      1,      0,      0x00)

OPCODE(ADD_F64,         "add.f64",          4,      2,      0,      0x00)
OPCODE(SUB_F64,         "sub.f64",          4,      2,      0,      0x00)
OPCODE(MUL_F64,         "mul.f64",          4,      2,      0,      0x00)
OPCODE(DIV_F64,         "div.f64",          4,      2,      0,      0x00)
OPCODE(REM_F64,         "rem.f64",          4,      2,      0,      0x00)
OPCODE(NEG_F64,         "neg.f64",          2,      2,      0,      0x00)

OPCODE(SHL_I32,         "shl.i32",          2,      1,      0,      0x00)
OPCODE(SHR_I32,         "shr.i32",          2,      1,      0,      0x00)
OPCODE(SHR_U32,         "shr.u32",          2,      1,      0,      0x00)
OPCODE(AND_I32,         "and.i32",          2,      1,      0,      0x00)
OPCODE(OR_I32,          "or.i32",           2,      1,      0,      0x00)
OPCODE(XOR_I32,         "xor.i32",          2,      1,      0,      0x00)
OPCODE(NOT_I32,         "not.i32",          1,      1,      0,      0x00)

OPCODE(SHL_I64,         "shl.i64",          3,      2,      0,      0x00)
OPCODE(SHR_I64,         "shr.i64",          3,      2,      0,      0x00)
OPCODE(SHR_U64,         "shr.u64",          3,      2,      0,      0x00)
OPCODE(AND_I64,         "and.i64",          4,      2,      0,      0x00)
OPCODE(OR_I64,          "or.i64",           4,      2,      0,      0x00)
OPCODE(XOR_I64,         "xor.i64",          4,      2,      0,      0x00)
OPCODE(NOT_I64,         "not.i64",          2,      2,      0,      0x00)

OPCODE(LDC_I32_M1,      "ldc.i32.m1",       0,      1,      0,      0x00)
OPCODE(LDC_I32_0,       "ldc.i32.0",        0,      1,      0,      0x00)
OPCODE(LDC_I32_1,       "ldc.i32.1",        0,      1,      0,      0x00)
OPCODE(LDC_I32_2,       "ldc.i32.2",        0,      1,      0,      0x00)
OPCODE(LDC_I32_3,       "ldc.i32.3",        0,      1,      0,      0x00)
OPCODE(LDC_I32_4,       "ldc.i32.4",        0,      1,      0,      0x00)
OPCODE(LDC_I32_5,       "ldc.i32.5",        0,      1,      0,      0x00)
OPCODE(LDC_I32_6,       "ldc.i32.6",        0,      1,      0,      0x00)
OPCODE(LDC_I32_7,       "ldc.i32.7",        0,      1,      0,      0x00)
OPCODE(LDC_I32_8,       "ldc.i32.8",        0,      1,      0,      0x00)
OPCODE(LDC_I32_S,       "ldc.i32.s",        0,      1,      1,      0x00)
OPCODE(LDC_I32,         "ldc.i32",          0,      1,      4,      0x00)
OPCODE(LDC_I64,         "ldc.i64",          0,      2,      8,      0x00)
OPCODE(LDC_F32,         "ldc.f32",          0,      1,      4,      0x00)
OPCODE(LDC_F64,         "ldc.f64",          0,      2,      8,      0x00)

OPCODE(CONV_I8_I32,     "conv.i8.i32",      1,      1,      0,      0x00)
OPCODE(CONV_I16_I32,    "conv.i16.i32",     1,      1,      0,      0x00)
OPCODE(CONV_I64_I32,    "conv.i64.i32",     1,      2,      0,      0x00)
OPCODE(CONV_F32_I32,    "conv.f32.i32",     1,      1,      0,      0x00)
OPCODE(CONV_F64_I32,    "conv.f64.i32",     1,      2,      0,      0x00)

OPCODE(CONV_I8_I64,     "conv.i8.i64",      2,      1,      0,      0x00)
OPCODE(CONV_I16_I64,    "conv.i16.i64",     2,      1,      0,      0x00)
OPCODE(CONV_I32_I64,    "conv.i32.i64",     2,      1,      0,      0x00)
OPCODE(CONV_F32_I64,    "conv.f32.i64",     2,      1,      0,      0x00)
OPCODE(CONV_F64_I64,    "conv.f64.i64",     2,      2,      0,      0x00)

OPCODE(CONV_I8_F32,     "conv.i8.f32",      1,      1,      0,      0x00)
OPCODE(CONV_I16_F32,    "conv.i16.f32",     1,      1,      0,      0x00)
OPCODE(CONV_I32_F32,    "conv.i32.f32",     1,      1,      0,      0x00)
OPCODE(CONV_I64_F32,    "conv.i64.f32",     1,      2,      0,      0x00)
OPCODE(CONV_F64_F32,    "conv.f64.f32",     1,      2,      0,      0x00)
OPCODE(CONV_U8_F32,     "conv.u8.f32",      1,      1,      0,      0x00)
OPCODE(CONV_U16_F32,    "conv.u16.f32",     1,      1,      0,      0x00)
OPCODE(CONV_U32_F32,    "conv.u32.f32",     1,      1,      0,      0x00)
OPCODE(CONV_U64_F32,    "conv.u64.f32",     1,      2,      0,      0x00)

OPCODE(CONV_I8_F64,     "conv.i8.f64",      2,      1,      0,      0x00)
OPCODE(CONV_I16_F64,    "conv.i16.f64",     2,      1,      0,      0x00)
OPCODE(CONV_I32_F64,    "conv.i32.f64",     2,      1,      0,      0x00)
OPCODE(CONV_I64_F64,    "conv.i64.f64",     2,      2,      0,      0x00)
OPCODE(CONV_F32_F64,    "conv.f32.f64",     2,      1,      0,      0x00)
OPCODE(CONV_U8_F64,     "conv.u8.f64",      2,      1,      0,      0x00)
OPCODE(CONV_U16_F64,    "conv.u16.f64",     2,      1,      0,      0x00)
OPCODE(CONV_U32_F64,    "conv.u32.f64",     2,      1,      0,      0x00)
OPCODE(CONV_U64_F64,    "conv.u64.f64",     2,      2,      0,      0x00)

OPCODE(CONV_F32_U32,    "conv.f32.u32",     1,      1,      0,      0x00)
OPCODE(CONV_F64_U32,    "conv.f64.u32",     1,      2,      0,      0x00)
OPCODE(CONV_U8_U32,     "conv.u8.u32",      1,      1,      0,      0x00)
OPCODE(CONV_U16_U32,    "conv.u16.u32",     1,      1,      0,      0x00)
OPCODE(CONV_U64_U32,    "conv.u64.u32",     1,      2,      0,      0x00)

OPCODE(CONV_F32_U64,    "conv.f32.u64",     2,      1,      0,      0x00)
OPCODE(CONV_F64_U64,    "conv.f64.u64",     2,      2,      0,      0x00)
OPCODE(CONV_U8_U64,     "conv.u8.u64",      2,      1,      0,      0x00)
OPCODE(CONV_U16_U64,    "conv.u16.u64",     2,      1,      0,      0x00)
OPCODE(CONV_U32_U64,    "conv.u32.u64",     2,      1,      0,      0x00)

OPCODE(CMP_EQ_I32,      "cmp.eq.i32",       2,      1,      0,      0x00)
OPCODE(CMP_GT_I32,      "cmp.gt.i32",       2,      1,      0,      0x00)
OPCODE(CMP_GE_I32,      "cmp.ge.i32",       2,      1,      0,      0x00)
OPCODE(CMP_LT_I32,      "cmp.lt.i32",       2,      1,      0,      0x00)
OPCODE(CMP_LE_I32,      "cmp.le.i32",       2,      1,      0,      0x00)
OPCODE(CMP_NE_I32,      "cmp.ne.i32",       2,      1,      0,      0x00)
OPCODE(CMP_GT_U32,      "cmp.gt.u32",       2,      1,      0,      0x00)
OPCODE(CMP_GE_U32,      "cmp.ge.u32",       2,      1,      0,      0x00)
OPCODE(CMP_LT_U32,      "cmp.lt.u32",       2,      1,      0,      0x00)
OPCODE(CMP_LE_U32,      "cmp.le.u32",       2,      1,      0,      0x00)

OPCODE(CMP_EQ_I64,      "cmp.eq.i64",       4,      1,      0,      0x00)
OPCODE(CMP_GT_I64,      "cmp.gt.i64",       4,      1,      0,      0x00)
OPCODE(CMP_GE_I64,      "cmp.ge.i64",       4,      1,      0,      0x00)
OPCODE(CMP_LT_I64,      "cmp.lt.i64",       4,      1,      0,      0x00)
OPCODE(CMP_LE_I64,      "cmp.le.i64",       4,      1,      0,      0x00)
OPCODE(CMP_NE_I64,      "cmp.ne.i64",       4,      1,      0,      0x00)
OPCODE(CMP_GT_U64,      "cmp.gt.u64",       4,      1,      0,      0x00)
OPCODE(CMP_GE_U64,      "cmp.ge.u64",       4,      1,      0,      0x00)
OPCODE(CMP_LT_U64,      "cmp.lt.u64",       4,      1,      0,      0x00)
OPCODE(CMP_LE_U64,      "cmp.le.u64",       4,      1,      0,      0x00)

OPCODE(CMP_OEQ_F32,     "cmp.oeq.f32",      2,      1,      0,      0x00)
OPCODE(CMP_ONE_F32,     "cmp.one.f32",      2,      1,      0,      0x00)
OPCODE(CMP_OGT_F32,     "cmp.ogt.f32",      2,      1,      0,      0x00)
OPCODE(CMP_OGE_F32,     "cmp.oge.f32",      2,      1,      0,      0x00)
OPCODE(CMP_OLT_F32,     "cmp.olt.f32",      2,      1,      0,      0x00)
OPCODE(CMP_OLE_F32,     "cmp.ole.f32",      2,      1,      0,      0x00)
OPCODE(CMP_UEQ_F32,     "cmp.ueq.f32",      2,      1,      0,      0x00)
OPCODE(CMP_UNE_F32,     "cmp.une.f32",      2,      1,      0,      0x00)
OPCODE(CMP_UGT_F32,     "cmp.ugt.f32",      2,      1,      0,      0x00)
OPCODE(CMP_UGE_F32,     "cmp.uge.f32",      2,      1,      0,      0x00)
OPCODE(CMP_ULT_F32,     "cmp.ult.f32",      2,      1,      0,      0x00)
OPCODE(CMP_ULE_F32,     "cmp.ule.f32",      2,      1,      0,      0x00)

OPCODE(CMP_OEQ_F64,     "cmp.oeq.f64",      4,      1,      0,      0x00)
OPCODE(CMP_ONE_F64,     "cmp.one.f64",      4,      1,      0,      0x00)
OPCODE(CMP_OGT_F64,     "cmp.ogt.f64",      4,      1,      0,      0x00)
OPCODE(CMP_OGE_F64,     "cmp.oge.f64",      4,      1,      0,      0x00)
OPCODE(CMP_OLT_F64,     "cmp.olt.f64",      4,      1,      0,      0x00)
OPCODE(CMP_OLE_F64,     "cmp.ole.f64",      4,      1,      0,      0x00)
OPCODE(CMP_UEQ_F64,     "cmp.ueq.f64",      4,      1,      0,      0x00)
OPCODE(CMP_UNE_F64,     "cmp.une.f64",      4,      1,      0,      0x00)
OPCODE(CMP_UGT_F64,     "cmp.ugt.f64",      4,      1,      0,      0x00)
OPCODE(CMP_UGE_F64,     "cmp.uge.f64",      4,      1,      0,      0x00)
OPCODE(CMP_ULT_F64,     "cmp.ult.f64",      4,      1,      0,      0x00)
OPCODE(CMP_ULE_F64,     "cmp.ule.f64",      4,      1,      0,      0x00)

OPCODE(BR,              "br",               0,      0,      2,      0x00)
OPCODE(BR_S,            "br.s",             0,      0,      1,      0x00)
OPCODE(BRFALSE,         "brfalse",          1,      0,      2,      0x00)
OPCODE(BRFALSE_S,       "brfalse.s",        1,      0,      1,      0x00)
OPCODE(BRTRUE,          "brtrue",           1,      0,      2,      0x00)
OPCODE(BRTRUE_S,        "brtrue.s",         1,      0,      1,      0x00)

OPCODE(NEWOBJ,          "newobj",           0,      1,      2,      0x00)
OPCODE(NEWARR,          "newarr",           1,      2,      2,      0x00)

OPCODE(LDFLD_I32,       "ldfld.i32",        1,      1,      2,      0x00)

OPCODE(STFLD_I32,       "stfld.i32",        2,      0,      2,      0x00)

OPCODE(LDSFLD_I32,      "ldsfld.i32",       0,      1,      2,      0x00)

OPCODE(STSFLD_I32,      "stsfld.i32",       1,      0,      2,      0x00)

OPCODE(UNUSED,          "unused",           0,      0,      0,      0x00)
