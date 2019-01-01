/*
 *  _____ _____ _____ _____ _____
 * |     |  _  |   | |   __|     |
 * | | | |     | | | |  |  |  |  |
 * |_|_|_|__|__|_|___|_____|_____|
 *
 * Mango Virtual Machine 1.0-dev
 *
 * Copyright (c) 2019 Klaus Hartke
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

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MANGO_VERSION_MAJOR 1
#define MANGO_VERSION_MINOR 0

#define MANGO_VERSION_STRING "1.0-dev"

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
  MANGO_E_OUT_OF_MEMORY = 80,
  MANGO_E_APPLICATION = 81,
  MANGO_E_STACK_OVERFLOW = 82,
  MANGO_E_STACK_IMBALANCE = 83,
  MANGO_E_ARITHMETIC = 84,
  MANGO_E_DIVIDE_BY_ZERO = 85,
  MANGO_E_INDEX_OUT_OF_RANGE = 86,
  MANGO_E_INVALID_PROGRAM = 87,
  MANGO_E_NULL_REFERENCE = 88,
  MANGO_E_SYSTEM_CALL_NOT_FOUND = 89,
  MANGO_E_BREAKPOINT = 110,
  MANGO_E_TIMEOUT = 111,
  MANGO_E_SYSTEM_CALL = 112,
} mango_result;

typedef enum mango_feature_flags {
  MANGO_FEATURE_I64 = 0x10,
  MANGO_FEATURE_F32 = 0x20,
  MANGO_FEATURE_F64 = 0x40,
  MANGO_FEATURE_REFS = 0x80,
} mango_feature_flags;

typedef enum mango_alloc_flags {
  MANGO_ALLOC_ZERO_MEMORY = 0x8,
} mango_alloc_flags;

typedef struct mango_vm mango_vm;

MANGO_API int mango_version_major(void);

MANGO_API int mango_version_minor(void);

MANGO_API const char *mango_version_string(void);

MANGO_API int mango_features(void);

MANGO_API mango_vm *mango_initialize(void *address, size_t heap_size,
                                     size_t stack_size, void *context);

MANGO_API mango_result mango_error(mango_vm *vm, mango_result error);

MANGO_API void *mango_context(const mango_vm *vm);

MANGO_API void *mango_heap_alloc(mango_vm *vm, size_t count, size_t size,
                                 size_t alignment, int flags);

MANGO_API size_t mango_heap_size(const mango_vm *vm);

MANGO_API size_t mango_heap_available(const mango_vm *vm);

MANGO_API void *mango_stack_alloc(mango_vm *vm, size_t size, int flags);

MANGO_API mango_result mango_stack_free(mango_vm *vm, size_t size);

MANGO_API void *mango_stack_top(mango_vm *vm);

MANGO_API size_t mango_stack_size(const mango_vm *vm);

MANGO_API size_t mango_stack_available(const mango_vm *vm);

MANGO_API mango_result mango_module_import(mango_vm *vm, const uint8_t *name,
                                           const uint8_t *image, size_t size,
                                           void *context);

MANGO_API const uint8_t *mango_module_missing(const mango_vm *vm);

MANGO_API void *mango_module_context(const mango_vm *vm);

MANGO_API mango_result mango_run(mango_vm *vm);

MANGO_API int mango_syscall(const mango_vm *vm);

////////////////////////////////////////////////////////////////////////////////

#if UINTPTR_MAX == UINT32_MAX

#define MANGO_DECLARE_REF_TYPE(Type)                                           \
                                                                               \
  typedef struct Type##_ref {                                                  \
    uintptr_t address;                                                         \
  } Type##_ref;

#define MANGO_DEFINE_REF_TYPE(Type, Const)                                     \
                                                                               \
  static inline Const Type *Type##_as_ptr(const mango_vm *vm,                  \
                                          Type##_ref ref) {                    \
    return (Const Type *)ref.address;                                          \
  }                                                                            \
                                                                               \
  static inline Type##_ref Type##_as_ref(const mango_vm *vm,                   \
                                         Const Type *ptr) {                    \
    return (Type##_ref){(uintptr_t)ptr};                                       \
  }                                                                            \
                                                                               \
  static inline Type##_ref Type##_null(void) { return (Type##_ref){0}; }       \
                                                                               \
  static inline int Type##_is_null(Type##_ref ref) { return !ref.address; }

#elif UINTPTR_MAX == UINT64_MAX

#define MANGO_DECLARE_REF_TYPE(Type)                                           \
                                                                               \
  typedef struct Type##_ref {                                                  \
    uint32_t address;                                                          \
  } Type##_ref;

#define MANGO_DEFINE_REF_TYPE(Type, Const)                                     \
                                                                               \
  static inline Const Type *Type##_as_ptr(const mango_vm *vm,                  \
                                          Type##_ref ref) {                    \
    return (Const Type *)((uintptr_t)vm + ref.address);                        \
  }                                                                            \
                                                                               \
  static inline Type##_ref Type##_as_ref(const mango_vm *vm,                   \
                                         Const Type *ptr) {                    \
    return (Type##_ref){(uint32_t)((uintptr_t)ptr - (uintptr_t)vm)};           \
  }                                                                            \
                                                                               \
  static inline Type##_ref Type##_null(void) { return (Type##_ref){0}; }       \
                                                                               \
  static inline int Type##_is_null(Type##_ref ref) { return !ref.address; }

#else
#error Unsupported bitness
#endif

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error Unsupported byte order
#endif

////////////////////////////////////////////////////////////////////////////////

#undef MANGO_API

#ifdef __cplusplus
}
#endif
