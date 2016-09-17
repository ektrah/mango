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

#ifdef __cplusplus
extern "C" {
#endif

#define MANGO_HEADER_MAGIC 255
#define MANGO_HEADER_SIZE 2

#pragma pack(push, 1)

typedef struct mango_module_name {
  uint8_t bytes[12];
} mango_module_name;

typedef enum mango_module_attributes {
  MANGO_MD_NONE = 0x0,
  MANGO_MD_TINY = 0x1,
  MANGO_MD_EXECUTABLE = 0x2,
} mango_module_attributes;

typedef struct mango_module_def {
  uint8_t attributes;
  uint8_t import_count;
  uint16_t static_size;
  uint16_t static_init;
  mango_module_name imports[];
} mango_module_def;

typedef struct mango_startup_def {
  uint16_t features;
  uint16_t stack_size;
  uint8_t module_count;
} mango_startup_def;

typedef enum mango_type_attributes {
  MANGO_TD_NONE = 0x0,
  MANGO_TD_PUBLIC = 0x100,
  MANGO_TD_ENUM = 0x200,
  MANGO_TD_FLAGS = 0x400,
  MANGO_TD_SEQUENTIAL_LAYOUT = 0x800,
  MANGO_TD_OBSOLETE = 0x1000,
} mango_type_attributes;

typedef struct mango_type_def {
  uint8_t attributes;
  uint16_t size;
} mango_type_def;

typedef struct mango_const_def {
  uint8_t attributes;
  uint8_t value[];
} mango_const_def;

typedef enum mango_function_attributes {
  MANGO_FD_NONE = 0x0,
  MANGO_FD_NAKED = 0x1,
  MANGO_FD_SECURITY_CRITICAL = 0x2,
  MANGO_FD_SECURITY_SAFE_CRITICAL = 0x4,
} mango_function_attributes;

typedef struct mango_func_def {
  uint8_t attributes;
  uint8_t max_stack;
  uint8_t arg_count;
  uint8_t loc_count;
  uint8_t code[];
} mango_func_def;

typedef struct mango_syscall_def {
  uint8_t attributes;
  uint8_t arg_count;
  uint8_t ret_count;
  uint16_t function;
} mango_syscall_def;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
