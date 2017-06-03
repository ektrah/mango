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

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MANGO_IMAGE_MAGIC 255

#pragma pack(push, 1)

typedef struct mango_module_name {
  uint8_t bytes[12];
} mango_module_name;

typedef enum mango_module_attributes {
  MANGO_MD_NONE = 0x0,
  MANGO_MD_EXECUTABLE = 0x2,
} mango_module_attributes;

typedef struct mango_module_def {
  uint8_t magic;
  uint8_t version;
  uint8_t attributes;
  uint8_t import_count;
  uint16_t static_size;
  uint16_t static_init;
  mango_module_name imports[];
} mango_module_def;

typedef struct mango_app_info {
  uint16_t features;
  uint16_t stack_size;
  uint8_t module_count;
  uint8_t halt;
  uint16_t main;
} mango_app_info;

typedef enum mango_func_attributes {
  MANGO_FD_NONE = 0x0,
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

#pragma pack(pop)

#ifdef __cplusplus
}
#endif
