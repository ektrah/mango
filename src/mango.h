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
#include <string.h>

#define MANGO_VERSION_MAJOR 0
#define MANGO_VERSION_MINOR 32

#define MANGO_VERSION_STRING "0.32-dev"

#ifndef MANGO_API
#ifdef MANGO_EXPORTS
#ifdef _WIN32
#define MANGO_API __declspec(dllexport)
#else
#define MANGO_API __attribute__((visibility("default")))
#endif
#else
#define MANGO_API
#endif
#endif

////////////////////////////////////////////////////////////////////////////////

typedef enum MangoResult {
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
} MangoResult;

typedef enum MangoFeatures {
  MANGO_FEATURE_I64 = 0x1000,
  MANGO_FEATURE_F32 = 0x2000,
  MANGO_FEATURE_F64 = 0x4000,
} MangoFeatures;

typedef enum MangoAllocFlags {
  MANGO_ALLOC_ZERO_MEMORY = 0x8,
} MangoAllocFlags;

typedef enum MangoImportFlags {
  // 0x1 is reserved
  MANGO_IMPORT_TRUSTED_MODULE = 0x2,
  MANGO_IMPORT_SKIP_VERIFICATION = 0x100,
} MangoImportFlags;

typedef struct MangoVM MangoVM;

MANGO_API uint32_t MangoVersionMajor(void);

MANGO_API uint32_t MangoVersionMinor(void);

MANGO_API const char *MangoVersionString(void);

MANGO_API uint32_t MangoQueryFeatures(void);

MANGO_API MangoVM *MangoInitialize(const void *base, void *address,
                                   uint32_t size, void *context);

MANGO_API void *MangoHeapAlloc(MangoVM *vm, uint32_t count, uint32_t size,
                               uint32_t alignment, uint32_t flags);

MANGO_API uint32_t MangoHeapSize(const MangoVM *vm);

MANGO_API uint32_t MangoHeapAvailable(const MangoVM *vm);

MANGO_API MangoResult MangoStackCreate(MangoVM *vm, uint32_t size);

MANGO_API void *MangoStackAlloc(MangoVM *vm, uint32_t size, uint32_t flags);

MANGO_API MangoResult MangoStackFree(MangoVM *vm, uint32_t size);

MANGO_API void *MangoStackTop(const MangoVM *vm);

MANGO_API uint32_t MangoStackSize(const MangoVM *vm);

MANGO_API uint32_t MangoStackAvailable(const MangoVM *vm);

MANGO_API MangoResult MangoModuleImport(MangoVM *vm, const uint8_t *name,
                                        const uint8_t *image, uint32_t size,
                                        void *context, uint32_t flags);

MANGO_API const uint8_t *MangoModuleMissing(const MangoVM *vm);

MANGO_API MangoResult MangoExecute(MangoVM *vm);

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 4)

MANGO_API inline int32_t MangoReadI32(const void *ref, uint32_t offset) {
  return *(const int32_t *)((uintptr_t)ref + offset);
}

MANGO_API inline int64_t MangoReadI64(const void *ref, uint32_t offset) {
  const struct {
    int64_t value;
  } *ptr = (const void *)((uintptr_t)ref + offset);
  return ptr->value;
}

MANGO_API inline void MangoWriteI32(void *ref, uint32_t offset, int32_t value) {
  *(int32_t *)((uintptr_t)ref + offset) = value;
}

MANGO_API inline void MangoWriteI64(void *ref, uint32_t offset, int64_t value) {
  struct {
    int64_t value;
  } *ptr = (void *)((uintptr_t)ref + offset);
  ptr->value = value;
}

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

#undef MANGO_API
