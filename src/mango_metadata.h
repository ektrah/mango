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

#pragma once

#include <stdint.h>

#define MANGO_HEADER_MAGIC 127
#define MANGO_HEADER_SIZE 2

#pragma pack(push, 1)

typedef struct ModuleName {
  uint8_t bytes[12];
} ModuleName;

typedef enum ModuleFlags {
  MANGO_MF_NONE = 0,
  MANGO_MF_TINY = 1,
  MANGO_MF_EXECUTABLE = 2,
} ModuleFlags;

typedef struct ModuleDef {
  uint8_t flags;
  uint8_t import_count;
  uint16_t static_size;
  uint16_t static_init;
  ModuleName imports[];
} ModuleDef;

typedef struct StartupDef {
  uint16_t features;
  uint16_t stack_size;
  uint8_t module_count;
  uint8_t exit_stub[1];
} StartupDef;

typedef enum FuncFlags {
  MANGO_FF_NONE = 0,
  MANGO_FF_NAKED = 1,
  MANGO_FF_SECURITY_CRITICAL = 2,
  MANGO_FF_SECURITY_SAFE_CRITICAL = 4,
} FuncFlags;

typedef struct FuncDef {
  uint8_t flags;
  uint8_t max_stack;
  uint8_t arg_count;
  uint8_t loc_count;
  uint8_t code[];
} FuncDef;

typedef struct SysCallDef {
  uint8_t flags;
  uint8_t arg_count;
  uint8_t ret_count;
  uint16_t function;
} SysCallDef;

#pragma pack(pop)

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error Unsupported byte order
#endif
