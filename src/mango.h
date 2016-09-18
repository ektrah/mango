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

#define MANGO_VERSION_MAJOR 0
#define MANGO_VERSION_MINOR 32

#define MANGO_VERSION_STRING "0.32-dev"

#ifdef MANGO_EXPORTS
#ifdef _WIN32
#define MANGO_API __declspec(dllexport)
#else
#define MANGO_API __attribute__((visibility("default")))
#endif
#else
#define MANGO_API
#endif

////////////////////////////////////////////////////////////////////////////////

typedef enum mango_result {
  MANGO_E_SUCCESS = 0,
  MANGO_E_ARGUMENT = 64,
  MANGO_E_ARGUMENT_NULL = 65,
  MANGO_E_INVALID_OPERATION = 66,
  MANGO_E_NOT_SUPPORTED = 67,
  MANGO_E_NOT_IMPLEMENTED = 68,
  MANGO_E_BAD_IMAGE_FORMAT = 69,
  MANGO_E_VERIFICATION = 70,
  MANGO_E_OUT_OF_MEMORY = 80,
  MANGO_E_SECURITY = 81,
  MANGO_E_STACK_OVERFLOW = 82,
  MANGO_E_STACK_IMBALANCE = 83,
  MANGO_E_ARITHMETIC = 84,
  MANGO_E_DIVIDE_BY_ZERO = 85,
  MANGO_E_INDEX_OUT_OF_RANGE = 86,
  MANGO_E_INVALID_PROGRAM = 87,
  MANGO_E_BREAKPOINT = 110,
  MANGO_E_TIMEOUT = 111,
  MANGO_E_SYSCALL = 112,
} mango_result;

typedef enum mango_feature_flags {
  MANGO_FEATURE_I64 = 0x1000,
  MANGO_FEATURE_F32 = 0x2000,
  MANGO_FEATURE_F64 = 0x4000,
} mango_feature_flags;

typedef enum mango_alloc_flags {
  MANGO_ALLOC_ZERO_MEMORY = 0x8,
} mango_alloc_flags;

typedef enum mango_import_flags {
  // 0x1 is reserved
  MANGO_IMPORT_TRUSTED_MODULE = 0x2,
  MANGO_IMPORT_SKIP_VERIFICATION = 0x100,
} mango_import_flags;

typedef struct mango_vm mango_vm;

MANGO_API uint32_t mango_version_major(void);

MANGO_API uint32_t mango_version_minor(void);

MANGO_API const char *mango_version_string(void);

MANGO_API uint32_t mango_features(void);

MANGO_API mango_vm *mango_initialize(void *address, uint32_t size,
                                     void *context);

MANGO_API void *mango_context(const mango_vm *vm);

MANGO_API void *mango_heap_alloc(mango_vm *vm, uint32_t count, uint32_t size,
                                 uint32_t alignment, uint32_t flags);

MANGO_API uint32_t mango_heap_size(const mango_vm *vm);

MANGO_API uint32_t mango_heap_available(const mango_vm *vm);

MANGO_API mango_result mango_stack_create(mango_vm *vm, uint32_t size);

MANGO_API void *mango_stack_alloc(mango_vm *vm, uint32_t size, uint32_t flags);

MANGO_API mango_result mango_stack_free(mango_vm *vm, uint32_t size);

MANGO_API void *mango_stack_top(const mango_vm *vm);

MANGO_API uint32_t mango_stack_size(const mango_vm *vm);

MANGO_API uint32_t mango_stack_available(const mango_vm *vm);

MANGO_API mango_result mango_module_import(mango_vm *vm, const uint8_t *name,
                                           const uint8_t *image, uint32_t size,
                                           void *context, uint32_t flags);

MANGO_API const uint8_t *mango_module_missing(const mango_vm *vm);

MANGO_API void *mango_module_context(const mango_vm *vm);

MANGO_API mango_result mango_execute(mango_vm *vm);

MANGO_API uint32_t mango_syscall_function(const mango_vm *vm);

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 4)

typedef union mango_value {
  int8_t i8;
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
  float f32;
  double f64;
} mango_value;

#pragma pack(pop)

#define MANGO_DEFINE_READ_WRITE(Type, Member)                                  \
  MANGO_API inline Type mango_read_##Member(const void *ref,                   \
                                            uint32_t offset) {                 \
    return ((const mango_value *)((uintptr_t)ref + offset))->Member;           \
  }                                                                            \
                                                                               \
  MANGO_API inline void mango_write_##Member(void *ref, uint32_t offset,       \
                                             Type value) {                     \
    ((mango_value *)((uintptr_t)ref + offset))->Member = value;                \
  }

MANGO_DEFINE_READ_WRITE(int8_t, i8)
MANGO_DEFINE_READ_WRITE(int16_t, i16)
MANGO_DEFINE_READ_WRITE(int32_t, i32)
MANGO_DEFINE_READ_WRITE(int64_t, i64)
MANGO_DEFINE_READ_WRITE(uint8_t, u8)
MANGO_DEFINE_READ_WRITE(uint16_t, u16)
MANGO_DEFINE_READ_WRITE(uint32_t, u32)
MANGO_DEFINE_READ_WRITE(uint64_t, u64)
MANGO_DEFINE_READ_WRITE(float, f32)
MANGO_DEFINE_READ_WRITE(double, f64)

#undef MANGO_DEFINE_READ_WRITE

////////////////////////////////////////////////////////////////////////////////

#undef MANGO_API

#ifdef __cplusplus
}
#endif
