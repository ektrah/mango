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

#include "mango.h"
#include "mango_metadata.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef _DEBUG
#include <stdio.h>
#else
#define printf(...) (void)0
#endif

////////////////////////////////////////////////////////////////////////////////

#if UINTPTR_MAX == UINT32_MAX

#define MANGO_DECLARE_REF_TYPE(Type)                                           \
                                                                               \
  typedef struct Type##Ref { uint32_t address; } Type##Ref;

#define MANGO_DEFINE_REF_TYPE(Type, Const)                                     \
                                                                               \
  static inline Const Type *Type##_as_ptr(const MangoVM *vm, Type##Ref ref) {  \
    (void)vm;                                                                  \
    return (Const Type *)ref.address;                                          \
  }                                                                            \
                                                                               \
  static inline Type##Ref Type##_as_ref(const MangoVM *vm, Const Type *ptr) {  \
    (void)vm;                                                                  \
    return (Type##Ref){(uint32_t)ptr};                                         \
  }

#elif UINTPTR_MAX == UINT64_MAX

#define MANGO_DECLARE_REF_TYPE(Type)                                           \
                                                                               \
  typedef struct Type##Ref { uint32_t address; } Type##Ref;

#define MANGO_DEFINE_REF_TYPE(Type, Const)                                     \
                                                                               \
  static inline Const Type *Type##_as_ptr(const MangoVM *vm, Type##Ref ref) {  \
    return (Const Type *)(vm->base + ref.address);                             \
  }                                                                            \
                                                                               \
  static inline Type##Ref Type##_as_ref(const MangoVM *vm, Const Type *ptr) {  \
    return (Type##Ref){(uint32_t)((uintptr_t)ptr - vm->base)};                 \
  }

#else
#error Unsupported bitness
#endif

////////////////////////////////////////////////////////////////////////////////

MANGO_DECLARE_REF_TYPE(void)
MANGO_DECLARE_REF_TYPE(uint8_t)
MANGO_DECLARE_REF_TYPE(stackval)
MANGO_DECLARE_REF_TYPE(Module)
MANGO_DECLARE_REF_TYPE(ModuleName)

#pragma pack(push, 4)

typedef enum OpCode {
#define OPCODE(c, s, pop, push, args, i) c,
#include "mango_opcodes.inc"
#undef OPCODE
} OpCode;

typedef struct StackFrame {
  uint8_t in_full_trust : 1;
  uint8_t pop : 7;
  uint8_t module;
  uint16_t ip;
} StackFrame;

typedef union Value {
  StackFrame sf;
  int32_t i32;
  uint32_t u32;
#ifndef MANGO_NO_F32
  float f32;
#endif
  voidRef ref;
} stackval;

typedef union Value2 {
  struct {
    voidRef ref;
    int32_t length;
  } slice;
#ifndef MANGO_NO_I64
  int64_t i64;
  uint64_t u64;
#endif
#ifndef MANGO_NO_F64
  double f64;
#endif
} stackval2;

typedef struct MangoVM {
  union {
    uintptr_t base;
    uint64_t _base;
  };
  union {
    void *context;
    uint64_t _context;
  };

  uint32_t heap_size;
  uint32_t heap_used;

  ModuleName startup_module;

  ModuleRef modules;
  uint8_t modules_created;
  uint8_t modules_imported;
  uint8_t module_init_head;
  uint8_t _reserved1;

  StackFrame sf;
  stackvalRef stack;
  uint16_t stack_size;
  uint16_t rp;
  uint16_t sp;
  uint16_t sp_expected;

  uint32_t _reserved2;
} MangoVM;

typedef struct Module {
  union {
    const uint8_t *image;
    uint64_t _image;
  };
  union {
    void *context;
    uint64_t _context;
  };

  uint8_t index;
  uint8_t flags;
  uint8_t init_next;
  uint8_t init_prev;

  ModuleNameRef name;

  uint8_tRef imports;
  voidRef static_data;
} Module;

#pragma pack(pop)

#pragma pack(push, 1)

typedef union packed {
  int8_t i8;
  uint8_t u8;
  int16_t i16;
  uint16_t u16;
  int32_t i32;
  uint32_t u32;
  int64_t i64;
  uint64_t u64;
} packed;

#pragma pack(pop)

MANGO_DEFINE_REF_TYPE(void, )
MANGO_DEFINE_REF_TYPE(uint8_t, const)
MANGO_DEFINE_REF_TYPE(stackval, )
MANGO_DEFINE_REF_TYPE(Module, )
MANGO_DEFINE_REF_TYPE(ModuleName, const)

#if defined(__clang__) || defined(__GNUC__)
_Static_assert(sizeof(StackFrame) == 4, "Incorrect layout");
_Static_assert(__alignof(StackFrame) == 2, "Incorrect layout");
_Static_assert(sizeof(stackval) == 4, "Incorrect layout");
_Static_assert(__alignof(stackval) == 4, "Incorrect layout");
_Static_assert(sizeof(stackval2) == 8, "Incorrect layout");
_Static_assert(__alignof(stackval2) == 4, "Incorrect layout");
_Static_assert(sizeof(MangoVM) == 64, "Incorrect layout");
_Static_assert(__alignof(MangoVM) == 4, "Incorrect layout");
_Static_assert(sizeof(Module) == 32, "Incorrect layout");
_Static_assert(__alignof(Module) == 4, "Incorrect layout");
_Static_assert(sizeof(packed) == 8, "Incorrect layout");
_Static_assert(__alignof(packed) == 1, "Incorrect layout");
#endif

////////////////////////////////////////////////////////////////////////////////

uint32_t MangoVersionMajor(void) { return MANGO_VERSION_MAJOR; }

uint32_t MangoVersionMinor(void) { return MANGO_VERSION_MINOR; }

const char *MangoVersionString(void) { return MANGO_VERSION_STRING; }

uint32_t MangoQueryFeatures(void) {
  uint32_t features = 0;
#ifndef MANGO_NO_I64
  features |= MANGO_FEATURE_I64;
#endif
#ifndef MANGO_NO_F32
  features |= MANGO_FEATURE_F32;
#endif
#ifndef MANGO_NO_F64
  features |= MANGO_FEATURE_F64;
#endif
  return features;
}

MangoVM *MangoInitialize(const void *base, void *address, uint32_t size,
                         void *context) {
  uintptr_t base_end;
  uintptr_t address_end;

  if (!address) {
    return NULL;
  }
  if (address < base) {
    return NULL;
  }
  if ((uintptr_t)address & (__alignof(MangoVM) - 1)) {
    return NULL;
  }
  if (size < sizeof(MangoVM)) {
    return NULL;
  }
  if (__builtin_add_overflow((uintptr_t)base, UINT32_MAX, &base_end)) {
    base_end = UINTPTR_MAX;
  }
  if (__builtin_add_overflow((uintptr_t)address, size - 1, &address_end)) {
    return NULL;
  }
  if (address_end > base_end) {
    return NULL;
  }

  printf("allocate %u bytes\n", (uint32_t)sizeof(MangoVM));

  MangoVM *vm = address;
  memset(vm, 0, sizeof(MangoVM));
  vm->base = (uintptr_t)base;
  vm->context = context;
  vm->heap_size = size;
  vm->heap_used = sizeof(MangoVM);
  return vm;
}

////////////////////////////////////////////////////////////////////////////////

void *MangoHeapAlloc(MangoVM *vm, uint32_t count, uint32_t size,
                     uint32_t alignment, uint32_t flags) {
  uint32_t total_size;
  uint32_t offset;
  uint32_t available;

  if (!vm) {
    return NULL;
  }
  if (!(alignment == 1 || alignment == 2 || alignment == 4)) {
    return NULL;
  }
  if (__builtin_mul_overflow(count, size, &total_size)) {
    return NULL;
  }
  if (__builtin_add_overflow(vm->heap_used, alignment - 1, &offset)) {
    return NULL;
  }
  offset &= ~(alignment - 1);
  if (__builtin_sub_overflow(vm->heap_size, offset, &available)) {
    return NULL;
  }
  if (total_size > available) {
    return NULL;
  }

  printf("allocate %u bytes\n", total_size);

  vm->heap_used = offset + total_size;
  void *block = (void *)((uintptr_t)vm + offset);

  if ((flags & MANGO_ALLOC_ZERO_MEMORY) != 0) {
    memset(block, 0, total_size);
  }

  return block;
}

uint32_t MangoHeapSize(const MangoVM *vm) {
  if (!vm) {
    return 0;
  }

  return vm->heap_size;
}

uint32_t MangoHeapAvailable(const MangoVM *vm) {
  if (!vm) {
    return 0;
  }

  return vm->heap_size - vm->heap_used;
}

////////////////////////////////////////////////////////////////////////////////

MangoResult MangoStackCreate(MangoVM *vm, uint32_t size) {
  if (!vm) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (size > UINT16_MAX * sizeof(stackval)) {
    return MANGO_E_ARGUMENT;
  }
  if (vm->stack.address) {
    return MANGO_E_INVALID_OPERATION;
  }

  uint16_t count = (uint16_t)((size + sizeof(stackval) - 1) / sizeof(stackval));

  stackval *stack =
      MangoHeapAlloc(vm, count, sizeof(stackval), __alignof(stackval), 0);

  if (!stack) {
    return MANGO_E_OUT_OF_MEMORY;
  }

  vm->stack = stackval_as_ref(vm, stack);
  vm->stack_size = count;
  vm->rp = 0;
  vm->sp = count;
  vm->sp_expected = count;
  return MANGO_E_SUCCESS;
}

void *MangoStackAlloc(MangoVM *vm, uint32_t size, uint32_t flags) {
  if (!vm) {
    return NULL;
  }
  if (size > UINT16_MAX * sizeof(stackval)) {
    return NULL;
  }
  if (!vm->stack.address) {
    return NULL;
  }

  uint16_t count = (uint16_t)((size + sizeof(stackval) - 1) / sizeof(stackval));

  if (vm->sp - vm->rp < count) {
    return NULL;
  }

  vm->sp -= count;

  stackval *block = stackval_as_ptr(vm, vm->stack) + vm->sp;

  if ((flags & MANGO_ALLOC_ZERO_MEMORY) != 0) {
    memset(block, 0, size);
  }

  return block;
}

MangoResult MangoStackFree(MangoVM *vm, uint32_t size) {
  if (!vm) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (size > UINT16_MAX * sizeof(stackval)) {
    return MANGO_E_ARGUMENT;
  }
  if (!vm->stack.address) {
    return MANGO_E_INVALID_OPERATION;
  }

  uint16_t count = (uint16_t)((size + sizeof(stackval) - 1) / sizeof(stackval));

  if (vm->stack_size - vm->sp < count) {
    return MANGO_E_STACK_OVERFLOW;
  }

  vm->sp += count;
  return MANGO_E_SUCCESS;
}

void *MangoStackTop(const MangoVM *vm) {
  if (!vm) {
    return NULL;
  }
  if (!vm->stack.address) {
    return NULL;
  }

  return stackval_as_ptr(vm, vm->stack) + vm->sp;
}

uint32_t MangoStackSize(const MangoVM *vm) {
  if (!vm) {
    return 0;
  }

  return vm->stack_size;
}

uint32_t MangoStackAvailable(const MangoVM *vm) {
  if (!vm) {
    return 0;
  }
  if (!vm->stack.address) {
    return 0;
  }

  return (vm->sp - vm->rp) * sizeof(stackval);
}

////////////////////////////////////////////////////////////////////////////////

#define INVALID_MODULE 255

static uint8_t GetOrCreateModule(MangoVM *vm, const ModuleName *name) {
  Module *modules = Module_as_ptr(vm, vm->modules);

  for (uint8_t i = 0; i < vm->modules_created; i++) {
    const ModuleName *n = ModuleName_as_ptr(vm, modules[i].name);
    if (memcmp(name, n, sizeof(ModuleName)) == 0) {
      return i;
    }
  }

  uint8_t index = vm->modules_created++;
  modules[index].index = index;
  modules[index].name = ModuleName_as_ref(vm, name);
  return index;
}

static MangoResult InitializeModule(MangoVM *vm, Module *module) {
  const ModuleDef *m = (const ModuleDef *)(module->image + MANGO_HEADER_SIZE);

  if (m->import_count != 0) {
    uint8_t *imports = MangoHeapAlloc(vm, m->import_count, sizeof(uint8_t),
                                      __alignof(uint8_t), 0);

    if (!imports) {
      return MANGO_E_OUT_OF_MEMORY;
    }

    for (uint8_t i = 0; i < m->import_count; i++) {
      imports[i] = GetOrCreateModule(vm, &m->imports[i]);
    }

    module->imports = uint8_t_as_ref(vm, imports);
  }

  if (m->static_size != 0) {
    void *static_data = MangoHeapAlloc(vm, m->static_size, sizeof(uint8_t),
                                       __alignof(stackval), 0);

    if (!static_data) {
      return MANGO_E_OUT_OF_MEMORY;
    }

    module->static_data = void_as_ref(vm, static_data);
  }

  return MANGO_E_SUCCESS;
}

static MangoResult ImportStartupModule(MangoVM *vm, const uint8_t *name,
                                       const uint8_t *image, void *context,
                                       uint32_t flags) {
  const ModuleDef *m = (const ModuleDef *)(image + MANGO_HEADER_SIZE);

  if ((m->flags & MANGO_MF_EXECUTABLE) == 0) {
    return MANGO_E_BAD_IMAGE_FORMAT;
  }

  const StartupDef *s =
      (const StartupDef *)(image + MANGO_HEADER_SIZE + sizeof(ModuleDef) +
                           m->import_count * sizeof(ModuleName));

  if ((s->features & MangoQueryFeatures()) != s->features) {
    return MANGO_E_NOT_SUPPORTED;
  }

  if (!vm->stack.address) {
    MangoResult result = MangoStackCreate(vm, s->stack_size * sizeof(stackval));
    if (result != MANGO_E_SUCCESS) {
      return result;
    }
  }

  Module *modules = MangoHeapAlloc(vm, s->module_count, sizeof(Module),
                                   __alignof(Module), MANGO_ALLOC_ZERO_MEMORY);

  if (!modules) {
    return MANGO_E_OUT_OF_MEMORY;
  }

  memcpy(&vm->startup_module, name, sizeof(ModuleName));
  vm->modules = Module_as_ref(vm, modules);
  vm->modules_created = 1;
  vm->modules_imported = 1;

  Module *module = &modules[0];
  module->image = image;
  module->context = context;
  module->index = 0;
  module->flags = (uint8_t)flags;
  module->init_next = INVALID_MODULE;
  module->init_prev = INVALID_MODULE;
  module->name = ModuleName_as_ref(vm, &vm->startup_module);

  return InitializeModule(vm, module);
}

static MangoResult ImportMissingModule(MangoVM *vm, const uint8_t *name,
                                       const uint8_t *image, void *context,
                                       uint32_t flags) {
  Module *modules = Module_as_ptr(vm, vm->modules);
  Module *module = &modules[vm->modules_imported];
  const ModuleName *n = ModuleName_as_ptr(vm, module->name);

  if (memcmp(name, n, sizeof(ModuleName)) != 0) {
    return MANGO_E_INVALID_OPERATION;
  }

  vm->modules_imported++;

  module->image = image;
  module->context = context;
  module->flags = (uint8_t)flags;
  module->init_next = INVALID_MODULE;
  module->init_prev = INVALID_MODULE;

  return InitializeModule(vm, module);
}

static MangoResult VerifyModule(const uint8_t *image, uint32_t size) {
  (void)image;
  (void)size;

#ifndef MANGO_NO_VERIFICATION
  return MANGO_E_NOT_IMPLEMENTED;
#else
  return MANGO_E_NOT_SUPPORTED;
#endif
}

MangoResult MangoModuleImport(MangoVM *vm, const uint8_t *name,
                              const uint8_t *image, uint32_t size,
                              void *context, uint32_t flags) {
  uintptr_t base_end;

  if (!vm || !name || !image) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (__builtin_add_overflow(vm->base, UINT32_MAX, &base_end)) {
    base_end = UINTPTR_MAX;
  }
  if ((uintptr_t)image < vm->base || (uintptr_t)image > base_end) {
    return MANGO_E_ARGUMENT;
  }
  if (size < 2 || size > UINT16_MAX || size - 1 > base_end - (uintptr_t)image) {
    return MANGO_E_ARGUMENT;
  }
  if (image[0] != MANGO_HEADER_MAGIC || image[1] != MANGO_VERSION_MAJOR) {
    return MANGO_E_BAD_IMAGE_FORMAT;
  }

  if ((flags & MANGO_IMPORT_SKIP_VERIFICATION) == 0) {
    MangoResult result = VerifyModule(image, size);
    if (result != MANGO_E_SUCCESS) {
      return result;
    }
  }

  if (vm->modules_imported == 0) {
    return ImportStartupModule(vm, name, image, context, flags);
  } else if (vm->modules_imported != vm->modules_created) {
    return ImportMissingModule(vm, name, image, context, flags);
  } else {
    return MANGO_E_INVALID_OPERATION;
  }
}

const uint8_t *MangoModuleMissing(const MangoVM *vm) {
  if (!vm) {
    return NULL;
  }
  if (vm->modules_imported == vm->modules_created) {
    return NULL;
  }

  Module *modules = Module_as_ptr(vm, vm->modules);
  Module *module = &modules[vm->modules_imported];
  const ModuleName *n = ModuleName_as_ptr(vm, module->name);

  return (const uint8_t *)n;
}

////////////////////////////////////////////////////////////////////////////////

#define VISITED 1

static MangoResult Execute(MangoVM *vm);

static MangoResult SetEntryPoint(MangoVM *vm, Module *module, uint32_t offset) {
  const FuncDef *f = (const FuncDef *)(module->image + offset);

  bool in_full_trust = false;
  if ((f->flags &
       (MANGO_FF_SECURITY_CRITICAL | MANGO_FF_SECURITY_SAFE_CRITICAL)) != 0) {
    if ((f->flags & MANGO_FF_SECURITY_SAFE_CRITICAL) == 0 ||
        (module->flags & MANGO_IMPORT_TRUSTED_MODULE) == 0) {
      printf("<< SECURITY VIOLATION >>\n");
      return MANGO_E_SECURITY;
    }
    printf("------------------------------ ENTER FULL TRUST "
           "------------------------------\n");
    in_full_trust = true;
  }

  if (vm->stack_size < 1 + f->loc_count + f->max_stack) {
    printf("<< STACK OVERFLOW >>\n");
    return MANGO_E_STACK_OVERFLOW;
  }

  stackval *rp = stackval_as_ptr(vm, vm->stack);
  rp->sf = (StackFrame){false, 0, 0, 0};
  vm->rp = 1;

  uint16_t ip = (uint16_t)(f->code - module->image);
  vm->sf = (StackFrame){in_full_trust, f->loc_count, module->index, ip};
  vm->sp -= f->loc_count;
  return MANGO_E_SUCCESS;
}

MangoResult MangoExecute(MangoVM *vm) {
  if (!vm) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (vm->modules_imported == 0) {
    return MANGO_E_INVALID_OPERATION;
  }
  if (vm->modules_imported != vm->modules_created) {
    return MANGO_E_INVALID_OPERATION;
  }
  if (vm->sp != vm->sp_expected) {
    return MANGO_E_STACK_IMBALANCE;
  }

  MangoResult result = Execute(vm);
  if (result != MANGO_E_SUCCESS) {
    return result;
  }

  Module *modules = Module_as_ptr(vm, vm->modules);
  uint8_t head = vm->module_init_head;

  while (head != INVALID_MODULE) {
    Module *module = &modules[head];
    const ModuleDef *m = (const ModuleDef *)(module->image + MANGO_HEADER_SIZE);

    if ((module->flags & VISITED) == 0) {
      module->flags |= VISITED;

      const uint8_t *imports = uint8_t_as_ptr(vm, module->imports);

      for (uint8_t i = 0; i < m->import_count; i++) {
        uint8_t p = imports[i];
        Module *import = &modules[p];

        if ((import->flags & VISITED) == 0) {
          if (head != p) {
            if (import->init_prev != INVALID_MODULE) {
              modules[import->init_prev].init_next = import->init_next;
            }
            if (import->init_next != INVALID_MODULE) {
              modules[import->init_next].init_prev = import->init_prev;
            }
            if (head != INVALID_MODULE) {
              modules[head].init_prev = p;
            }
            import->init_next = head;
            import->init_prev = INVALID_MODULE;
            head = p;
          }
        }
      }
    } else {
      head = module->init_next;
      vm->module_init_head = head;
      module->init_next = INVALID_MODULE;
      module->init_prev = INVALID_MODULE;
      if (head != INVALID_MODULE) {
        modules[head].init_prev = INVALID_MODULE;
      }

      printf("initialize module %u\n", module->index);
      if (m->static_init != 0) {
        result = SetEntryPoint(vm, module, m->static_init);
        if (result != MANGO_E_SUCCESS) {
          return result;
        }
        result = Execute(vm);
        if (result != MANGO_E_SUCCESS) {
          return result;
        }
      }
    }
  }

#ifdef _DEBUG
  printf("\n  stack\n  -----\n");
  for (const stackval *stack = stackval_as_ptr(vm, vm->stack),
                      *sp = stack + vm->sp, *rp = stack + vm->rp, *p = stack;
       p != stack + vm->stack_size; p++) {
    printf("   %3u %c   %8X\n", (uint32_t)(p - stack),
           (p == sp || p == rp) ? '*' : ' ', p->u32);
  }
#endif

  return MANGO_E_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-label-as-value"
#pragma clang diagnostic ignored "-Wunused-macros"
#pragma clang diagnostic ignored "-Wfloat-equal"

#pragma region macros

#if defined(__clang__) || defined(__GNUC__)
#define NEXT                                                                   \
  do {                                                                         \
    printf("* %s\n", opcodes[*ip]);                                            \
    goto *dispatch_table[*ip];                                                 \
  } while (0)
#elif defined(_MSC_VER)
#error Unsupported compiler
#define NEXT
#else
#error Unsupported compiler
#endif

#define FETCH(offset, ty) (((const packed *)(ip + (offset)))->ty)

#define UNPACK_STATE(sf)                                                       \
  do {                                                                         \
    StackFrame sf_ = (sf);                                                     \
    mp = &Module_as_ptr(vm, vm->modules)[sf_.module];                          \
    ip = mp->image + sf_.ip;                                                   \
    pop = sf_.pop;                                                             \
    in_full_trust = sf_.in_full_trust;                                         \
  } while (0)

#define PACK_STATE()                                                           \
  (StackFrame) { in_full_trust, pop, mp->index, (uint16_t)(ip - mp->image) }

#define RETURN(Result)                                                         \
  do {                                                                         \
    result = Result;                                                           \
    goto done;                                                                 \
  } while (0)

#define INVALID                                                                \
  do {                                                                         \
    goto invalid;                                                              \
  } while (0)

#define BINARY1(ty, op)                                                        \
  do {                                                                         \
    sp[1].ty = sp[1].ty op sp[0].ty;                                           \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define BINARY1F(ty, op)                                                       \
  do {                                                                         \
    op(sp[1].ty, sp[0].ty, &sp[1].ty);                                         \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define BINARY1D(ty, op)                                                       \
  do {                                                                         \
    if (sp[0].ty == 0) {                                                       \
      RETURN(MANGO_E_DIVIDE_BY_ZERO);                                          \
    }                                                                          \
    sp[1].ty = sp[1].ty op sp[0].ty;                                           \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define BINARY2(ty, op)                                                        \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[1].ty = sp2[1].ty op sp2[0].ty;                                        \
    sp += 2;                                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define BINARY2F(ty, op)                                                       \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    op(sp2[1].ty, sp2[0].ty, &sp2[1].ty);                                      \
    sp += 2;                                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define BINARY2D(ty, op)                                                       \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    if (sp2[0].ty == 0) {                                                      \
      RETURN(MANGO_E_DIVIDE_BY_ZERO);                                          \
    }                                                                          \
    sp2[1].ty = sp2[1].ty op sp2[0].ty;                                        \
    sp += 2;                                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define UNARY1(ty, op)                                                         \
  do {                                                                         \
    sp[0].ty = op(sp[0].ty);                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define UNARY2(ty, op)                                                         \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[0].ty = op(sp2[0].ty);                                                 \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define SHIFT1(ty, op)                                                         \
  do {                                                                         \
    sp[1].ty = sp[1].ty op sp[0].i32;                                          \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define SHIFT2(ty, op)                                                         \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)(sp + 1);                                    \
    sp2[0].ty = sp2[0].ty op sp[0].i32;                                        \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define CONVERT1(cast, dest, src)                                              \
  do {                                                                         \
    sp[0].dest = (cast)sp[0].src;                                              \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define CONVERT21(cast, dest, src)                                             \
  do {                                                                         \
    cast tmp = (cast)sp[0].src;                                                \
    sp--;                                                                      \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[0].dest = tmp;                                                         \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define CONVERT12(cast, dest, src)                                             \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    cast tmp = (cast)sp2[0].src;                                               \
    sp++;                                                                      \
    sp[0].dest = tmp;                                                          \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define CONVERT2(cast, dest, src)                                              \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[0].dest = (cast)sp2[0].src;                                            \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define COMPARE1(mem, op)                                                      \
  do {                                                                         \
    sp[1].i32 = sp[1].mem op sp[0].mem;                                        \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define COMPARE1F(mem, op)                                                     \
  do {                                                                         \
    sp[1].i32 = op(sp[1].mem, sp[0].mem);                                      \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define COMPARE2(mem, op)                                                      \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    int32_t tmp = sp2[1].mem op sp2[0].mem;                                    \
    sp += 3;                                                                   \
    sp[0].i32 = tmp;                                                           \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define COMPARE2F(mem, op)                                                     \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    int32_t tmp = op(sp2[1].mem, sp2[0].mem);                                  \
    sp += 3;                                                                   \
    sp[0].i32 = tmp;                                                           \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define LOAD_FIELD(ref, cast, ty)                                              \
  do {                                                                         \
    uint32_t offset = FETCH(1, u16);                                           \
    void *obj = void_as_ptr(vm, ref);                                          \
    sp[0].ty = ((cast *)((uintptr_t)obj + offset))[0];                         \
    ip += 3;                                                                   \
    NEXT;                                                                      \
  } while (0)

#define LOAD_FIELD2(ref)                                                       \
  do {                                                                         \
    uint32_t offset = FETCH(1, u16);                                           \
    void *obj = void_as_ptr(vm, ref);                                          \
    sp--;                                                                      \
    sp[0].i32 = ((int32_t *)((uintptr_t)obj + offset))[0];                     \
    sp[1].i32 = ((int32_t *)((uintptr_t)obj + offset))[1];                     \
    ip += 3;                                                                   \
    NEXT;                                                                      \
  } while (0)

#define STORE_FIELD(ref, cast, ty, pop)                                        \
  do {                                                                         \
    uint32_t offset = FETCH(1, u16);                                           \
    void *obj = void_as_ptr(vm, ref);                                          \
    ((cast *)((uintptr_t)obj + offset))[0] = (cast)sp[0].ty;                   \
    sp += 1 + pop;                                                             \
    ip += 3;                                                                   \
    NEXT;                                                                      \
  } while (0)

#define STORE_FIELD2(ref, pop)                                                 \
  do {                                                                         \
    uint32_t offset = FETCH(1, u16);                                           \
    void *obj = void_as_ptr(vm, ref);                                          \
    ((int32_t *)((uintptr_t)obj + offset))[0] = sp[0].i32;                     \
    ((int32_t *)((uintptr_t)obj + offset))[1] = sp[1].i32;                     \
    sp += 2 + pop;                                                             \
    ip += 3;                                                                   \
  } while (0)

#define LOAD_ELEMENT(cast, ty)                                                 \
  do {                                                                         \
    int32_t index = sp[0].i32;                                                 \
    if (index < 0 || index >= sp[2].i32) {                                     \
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);                                      \
    }                                                                          \
    void *arr = void_as_ptr(vm, sp[1].ref);                                    \
    sp += 2;                                                                   \
    sp[0].ty = ((cast *)arr)[index];                                           \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#define STORE_ELEMENT(cast)                                                    \
  do {                                                                         \
    int32_t value = sp[0].i32;                                                 \
    int32_t index = sp[1].i32;                                                 \
    if (index < 0 || index >= sp[3].i32) {                                     \
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);                                      \
    }                                                                          \
    void *arr = void_as_ptr(vm, sp[2].ref);                                    \
    sp += 4;                                                                   \
    ((cast *)arr)[index] = (cast)value;                                        \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0);

#pragma endregion

static MangoResult Execute(MangoVM *vm) {
  static const void *const dispatch_table[] = {
#define OPCODE(c, s, pop, push, args, i) &&c,
#include "mango_opcodes.inc"
#undef OPCODE
  };
#ifdef _DEBUG
  static const char *const opcodes[] = {
#define OPCODE(c, s, pop, push, args, i) s,
#include "mango_opcodes.inc"
#undef OPCODE
  };
#endif

  stackval *rp;
  stackval *sp;
  bool in_full_trust;
  uint8_t pop;
  Module *mp;
  const uint8_t *ip;
  MangoResult result;

  vm->sp_expected = vm->stack_size;

  rp = stackval_as_ptr(vm, vm->stack) + vm->rp;
  sp = stackval_as_ptr(vm, vm->stack) + vm->sp;
  UNPACK_STATE(vm->sf);
  NEXT;

#pragma region basic

HALT:
  if (sp - stackval_as_ptr(vm, vm->stack) < vm->stack_size) {
    RETURN(MANGO_E_STACK_IMBALANCE);
  }
  RETURN(MANGO_E_SUCCESS);

NOP:
  ++ip;
  NEXT;

BREAK:
  ++ip;
  vm->sp_expected = (uint16_t)(sp - stackval_as_ptr(vm, vm->stack));
  RETURN(MANGO_E_BREAKPOINT);

POP:
  sp++;
  ip++;
  NEXT;

POP2:
  sp += 2;
  ip++;
  NEXT;

DUP:
  sp--;
  sp[0].i32 = sp[1].i32;
  ip++;
  NEXT;

DUP2:
  sp -= 2;
  sp[0].i32 = sp[2].i32;
  sp[1].i32 = sp[3].i32;
  ip++;
  NEXT;

SWAP:
  do {
    int32_t tmp = sp[0].i32;
    sp[0].i32 = sp[1].i32;
    sp[1].i32 = tmp;
    ip++;
    NEXT;
  } while (0);

SWAP2:
  do {
    int32_t tmp0 = sp[0].i32;
    int32_t tmp1 = sp[1].i32;
    sp[0].i32 = sp[2].i32;
    sp[1].i32 = sp[3].i32;
    sp[2].i32 = tmp0;
    sp[3].i32 = tmp1;
    ip++;
    NEXT;
  } while (0);

OVER:
  sp--;
  sp[0].i32 = sp[2].i32;
  ip++;
  NEXT;

ROT:
  do {
    int32_t tmp = sp[0].i32;
    sp[0].i32 = sp[1].i32;
    sp[1].i32 = sp[2].i32;
    sp[2].i32 = tmp;
    ip++;
    NEXT;
  } while (0);

NIP:
  sp[1].i32 = sp[0].i32;
  sp++;
  ip++;
  NEXT;

TUCK:
  sp--;
  sp[0].i32 = sp[1].i32;
  sp[1].i32 = sp[2].i32;
  sp[2].i32 = sp[0].i32;
  ip++;
  NEXT;

#pragma endregion

#pragma region locals

LDLOC:
  do {
    uint8_t index = FETCH(1, u8);
    sp--;
    sp[0].i32 = sp[1 + index].i32;
    ip += 2;
    NEXT;
  } while (0);

LDLOC2:
  do {
    uint8_t index = FETCH(1, u8);
    sp -= 2;
    sp[0].i32 = sp[2 + index + 0].i32;
    sp[1].i32 = sp[2 + index + 1].i32;
    ip += 2;
    NEXT;
  } while (0);

LDLOCA:
  do {
    uint8_t index = FETCH(1, u8);
    sp--;
    sp[0].ref = void_as_ref(vm, &sp[1 + index]);
    ip += 2;
    NEXT;
  } while (0);

STLOC:
  do {
    uint8_t index = FETCH(1, u8);
    sp[index].i32 = sp[0].i32;
    sp++;
    ip += 2;
    NEXT;
  } while (0);

STLOC2:
  do {
    uint8_t index = FETCH(1, u8);
    sp[index + 0].i32 = sp[0].i32;
    sp[index + 1].i32 = sp[1].i32;
    sp += 2;
    ip += 2;
    NEXT;
  } while (0);

#pragma endregion

#pragma region calls

RET2:
  sp[pop + 1].i32 = sp[1].i32;

RET1:
  sp[pop + 0].i32 = sp[0].i32;

RET:
  sp += pop;
  --rp;

#ifdef _DEBUG
  if (in_full_trust && !rp->sf.in_full_trust) {
    printf("------------------------------ LEAVE FULL TRUST "
           "------------------------------\n");
  }
#endif

  UNPACK_STATE(rp->sf);
  NEXT;

CALL:
  do {
    uint8_t index = FETCH(1, u8);
    Module *module;

    if (index == INVALID_MODULE) {
      module = mp;
    } else {
      const uint8_t *imports = uint8_t_as_ptr(vm, mp->imports);
      module = &Module_as_ptr(vm, vm->modules)[imports[index]];
    }

    const FuncDef *f = (const FuncDef *)(module->image + FETCH(2, u16));

    bool in_full_trust_new = in_full_trust;
    if (!in_full_trust &&
        (f->flags &
         (MANGO_FF_SECURITY_CRITICAL | MANGO_FF_SECURITY_SAFE_CRITICAL)) != 0) {
      if ((f->flags & MANGO_FF_SECURITY_SAFE_CRITICAL) == 0 ||
          (module->flags & MANGO_IMPORT_TRUSTED_MODULE) == 0) {
        printf("<< SECURITY VIOLATION >>\n");
        RETURN(MANGO_E_SECURITY);
      }
      printf("------------------------------ ENTER FULL TRUST "
             "------------------------------\n");
      in_full_trust_new = true;
    }

    if (sp - rp < 1 + f->loc_count + f->max_stack) {
      printf("<< STACK OVERFLOW >>\n");
      RETURN(MANGO_E_STACK_OVERFLOW);
    }

    ip += 4;
    if (!(pop == 0 && *ip == RET)) {
      rp->sf = PACK_STATE();
      rp++;
    }
    in_full_trust = in_full_trust_new;
    pop = (f->flags & MANGO_FF_NAKED) != 0 ? 0 : f->arg_count + f->loc_count;
    mp = module;
    ip = f->code;
    sp -= f->loc_count;

    NEXT;
  } while (0);

SYSCALL:
  do {
    uint8_t index = FETCH(1, u8);
    Module *module;

    if (index == INVALID_MODULE) {
      module = mp;
    } else {
      const uint8_t *imports = uint8_t_as_ptr(vm, mp->imports);
      module = &Module_as_ptr(vm, vm->modules)[imports[index]];
    }

    const SysCallDef *f = (const SysCallDef *)(module->image + FETCH(2, u16));

    if (!in_full_trust) {
      if ((f->flags & MANGO_FF_SECURITY_SAFE_CRITICAL) == 0 ||
          (mp->flags & MANGO_IMPORT_TRUSTED_MODULE) == 0) {
        printf("<< SECURITY VIOLATION >>\n");
        RETURN(MANGO_E_SECURITY);
      }
    }

    if ((sp - rp) + f->arg_count < f->ret_count) {
      printf("<< STACK OVERFLOW >>\n");
      RETURN(MANGO_E_STACK_OVERFLOW);
    }

    ip += 4;
    vm->sp_expected = (uint16_t)(
        ((sp - stackval_as_ptr(vm, vm->stack)) + f->arg_count) - f->ret_count);
    RETURN(MANGO_E_SYSCALL);
  } while (0);

#pragma endregion

#pragma region i32 arithmetic

ADD_I32:
  BINARY1F(u32, __builtin_add_overflow);

SUB_I32:
  BINARY1F(u32, __builtin_sub_overflow);

MUL_I32:
  BINARY1F(u32, __builtin_mul_overflow);

DIV_I32:
  BINARY1D(i32, /);

DIV_U32:
  BINARY1D(u32, /);

REM_I32:
  BINARY1D(i32, %);

REM_U32:
  BINARY1D(u32, %);

NEG_I32:
  __builtin_sub_overflow(0, sp[0].i32, &sp[0].i32);
  ip++;
  NEXT;

#pragma endregion

#pragma region i64 arithmetic
#ifndef MANGO_NO_I64

ADD_I64:
  BINARY2F(u64, __builtin_add_overflow);

SUB_I64:
  BINARY2F(u64, __builtin_sub_overflow);

MUL_I64:
  BINARY2F(u64, __builtin_mul_overflow);

DIV_I64:
  BINARY2D(i64, /);

DIV_U64:
  BINARY2D(u64, /);

REM_I64:
  BINARY2D(i64, %);

REM_U64:
  BINARY2D(u64, %);

NEG_I64:
  do {
    stackval2 *sp2 = (stackval2 *)sp;
    __builtin_sub_overflow(0, sp2[0].i64, &sp2[0].i64);
    ip++;
    NEXT;
  } while (0);

#else
ADD_I64:
SUB_I64:
MUL_I64:
DIV_I64:
DIV_U64:
REM_I64:
REM_U64:
NEG_I64:
  INVALID;
#endif
#pragma endregion

#pragma region f32 arithmetic
#ifndef MANGO_NO_F32

ADD_F32:
  BINARY1(f32, +);

SUB_F32:
  BINARY1(f32, -);

MUL_F32:
  BINARY1(f32, *);

DIV_F32:
  BINARY1(f32, /);

REM_F32:
  sp[1].f32 = fmodf(sp[1].f32, sp[0].f32);
  sp++;
  ip++;
  NEXT;

NEG_F32:
  UNARY1(f32, -);

#else
ADD_F32:
SUB_F32:
MUL_F32:
DIV_F32:
REM_F32:
NEG_F32:
  INVALID;
#endif
#pragma endregion

#pragma region f64 arithmetic
#ifndef MANGO_NO_F64

ADD_F64:
  BINARY2(f64, +);

SUB_F64:
  BINARY2(f64, -);

MUL_F64:
  BINARY2(f64, *);

DIV_F64:
  BINARY2(f64, /);

REM_F64:
  do {
    stackval2 *sp2 = (stackval2 *)sp;
    sp2[1].f64 = fmod(sp2[1].f64, sp2[0].f64);
    sp += 2;
    ip++;
    NEXT;
  } while (0);

NEG_F64:
  UNARY2(f64, -);

#else
ADD_F64:
SUB_F64:
MUL_F64:
DIV_F64:
REM_F64:
NEG_F64:
  INVALID;
#endif
#pragma endregion

#pragma region i32 bitwise

SHL_I32:
  SHIFT1(u32, <<);

SHR_I32:
  SHIFT1(i32, >>);

SHR_U32:
  SHIFT1(u32, >>);

AND_I32:
  BINARY1(u32, &);

OR_I32:
  BINARY1(u32, |);

XOR_I32:
  BINARY1(u32, ^);

NOT_I32:
  UNARY1(u32, ~);

#pragma endregion

#pragma region i64 bitwise
#ifndef MANGO_NO_I64

SHL_I64:
  SHIFT2(u64, <<);

SHR_I64:
  SHIFT2(i64, >>);

SHR_U64:
  SHIFT2(u64, >>);

AND_I64:
  BINARY2(u64, &);

OR_I64:
  BINARY2(u64, |);

XOR_I64:
  BINARY2(u64, ^);

NOT_I64:
  UNARY2(u64, ~);

#else
SHL_I64:
SHR_I64:
SHR_U64:
AND_I64:
OR_I64:
XOR_I64:
NOT_I64:
  INVALID;
#endif
#pragma endregion

#pragma region constants

LDC_I32_M1:
LDC_I32_0:
LDC_I32_1:
LDC_I32_2:
LDC_I32_3:
LDC_I32_4:
LDC_I32_5:
LDC_I32_6:
LDC_I32_7:
LDC_I32_8:
  sp--;
  sp[0].i32 = *ip - LDC_I32_0;
  ip++;
  NEXT;

LDC_I32_S:
  sp--;
  sp[0].i32 = FETCH(1, i8);
  ip += 2;
  NEXT;

LDC_I32:
LDC_F32:
  sp--;
  sp[0].i32 = FETCH(1, i32);
  ip += 5;
  NEXT;

LDC_I64:
LDC_F64:
#ifndef MANGO_NO_I64
  do {
    sp -= 2;
    stackval2 *sp2 = (stackval2 *)sp;
    sp2[0].i64 = FETCH(1, i64);
    ip += 9;
    NEXT;
  } while (0);
#else
  INVALID;
#endif

LDCA:
  do {
    const ConstDef *c = (const ConstDef *)(mp->image + FETCH(1, u16));

    sp--;
    sp[0].ref = void_as_ref(vm, (void *)(uintptr_t)(c->value));
    ip += 3;
    NEXT;
  } while (0);

#pragma endregion

#pragma region i32 conversion

CONV_I8_I32:
  CONVERT1(int8_t, i32, i32);

CONV_I16_I32:
  CONVERT1(int16_t, i32, i32);

CONV_I64_I32:
#ifndef MANGO_NO_I64
  CONVERT21(int64_t, i64, i32);
#else
  INVALID;
#endif

CONV_F32_I32:
#ifndef MANGO_NO_F32
  CONVERT1(float, f32, i32);
#else
  INVALID;
#endif

CONV_F64_I32:
#ifndef MANGO_NO_F64
  CONVERT21(double, f64, i32);
#else
  INVALID;
#endif

#pragma endregion

#pragma region i64 conversion
#ifndef MANGO_NO_I64

CONV_I8_I64:
  CONVERT12(int8_t, i32, i64);

CONV_I16_I64:
  CONVERT12(int16_t, i32, i64);

CONV_I32_I64:
  CONVERT12(int32_t, i32, i64);

CONV_F32_I64:
#ifndef MANGO_NO_F32
  CONVERT12(float, f32, i64);
#else
  INVALID;
#endif

CONV_F64_I64:
#ifndef MANGO_NO_F64
  CONVERT2(double, f64, i64);
#else
  INVALID;
#endif

#else
CONV_I8_I64:
CONV_I16_I64:
CONV_I32_I64:
CONV_F32_I64:
CONV_F64_I64:
  INVALID;
#endif
#pragma endregion

#pragma region f32 conversion
#ifndef MANGO_NO_F32

CONV_I8_F32:
  CONVERT1(int8_t, i32, f32);

CONV_I16_F32:
  CONVERT1(int16_t, i32, f32);

CONV_I32_F32:
  CONVERT1(int32_t, i32, f32);

CONV_I64_F32:
#ifndef MANGO_NO_I64
  CONVERT21(int64_t, i64, f32);
#else
  INVALID;
#endif

CONV_F64_F32:
#ifndef MANGO_NO_F64
  CONVERT21(double, f64, f32);
#else
  INVALID;
#endif

CONV_U8_F32:
  CONVERT1(uint8_t, u32, f32);

CONV_U16_F32:
  CONVERT1(uint16_t, u32, f32);

CONV_U32_F32:
  CONVERT1(uint32_t, u32, f32);

CONV_U64_F32:
#ifndef MANGO_NO_I64
  CONVERT21(uint64_t, u64, f32);
#else
  INVALID;
#endif

#else
CONV_I8_F32:
CONV_I16_F32:
CONV_I32_F32:
CONV_I64_F32:
CONV_F64_F32:
CONV_U8_F32:
CONV_U16_F32:
CONV_U32_F32:
CONV_U64_F32:
  INVALID;
#endif
#pragma endregion

#pragma region f64 conversion
#ifndef MANGO_NO_F64

CONV_I8_F64:
  CONVERT12(int8_t, i32, f64);

CONV_I16_F64:
  CONVERT12(int16_t, i32, f64);

CONV_I32_F64:
  CONVERT12(int32_t, i32, f64);

CONV_I64_F64:
#ifndef MANGO_NO_I64
  CONVERT2(int64_t, i64, f64);
#else
  INVALID;
#endif

CONV_F32_F64:
#ifndef MANGO_NO_F32
  CONVERT12(float, f32, f64);
#else
  INVALID;
#endif

CONV_U8_F64:
  CONVERT12(uint8_t, u32, f64);

CONV_U16_F64:
  CONVERT12(uint16_t, u32, f64);

CONV_U32_F64:
  CONVERT12(uint32_t, u32, f64);

CONV_U64_F64:
#ifndef MANGO_NO_I64
  CONVERT2(uint64_t, u64, f64);
#else
  INVALID;
#endif

#else
CONV_I8_F64:
CONV_I16_F64:
CONV_I32_F64:
CONV_I64_F64:
CONV_F32_F64:
CONV_U8_F64:
CONV_U16_F64:
CONV_U32_F64:
CONV_U64_F64:
  INVALID;
#endif
#pragma endregion

#pragma region u32 conversion

CONV_F32_U32:
#ifndef MANGO_NO_F32
  CONVERT1(float, f32, u32);
#else
  INVALID;
#endif

CONV_F64_U32:
#ifndef MANGO_NO_F64
  CONVERT21(double, f64, u32);
#else
  INVALID;
#endif

CONV_U8_U32:
  CONVERT1(uint8_t, u32, u32);

CONV_U16_U32:
  CONVERT1(uint16_t, u32, u32);

CONV_U64_U32:
#ifndef MANGO_NO_I64
  CONVERT21(uint64_t, u64, u32);
#else
  INVALID;
#endif

#pragma endregion

#pragma region u64 conversion
#ifndef MANGO_NO_I64

CONV_F32_U64:
#ifndef MANGO_NO_F32
  CONVERT12(float, f32, u64);
#else
  INVALID;
#endif

CONV_F64_U64:
#ifndef MANGO_NO_F64
  CONVERT2(double, f64, u64);
#else
  INVALID;
#endif

CONV_U8_U64:
  CONVERT12(uint8_t, u32, u64);

CONV_U16_U64:
  CONVERT12(uint16_t, u32, u64);

CONV_U32_U64:
  CONVERT12(uint32_t, u32, u64);

#else
CONV_F32_U64:
CONV_F64_U64:
CONV_U8_U64:
CONV_U16_U64:
CONV_U32_U64:
  INVALID;
#endif
#pragma endregion

#pragma region i32 comparison

CMP_EQ_I32:
  COMPARE1(i32, ==);

CMP_GT_I32:
  COMPARE1(i32, >);

CMP_GE_I32:
  COMPARE1(i32, >=);

CMP_LT_I32:
  COMPARE1(i32, <);

CMP_LE_I32:
  COMPARE1(i32, <=);

CMP_NE_I32:
  COMPARE1(u32, !=);

CMP_GT_U32:
  COMPARE1(u32, >);

CMP_GE_U32:
  COMPARE1(u32, >=);

CMP_LT_U32:
  COMPARE1(u32, <);

CMP_LE_U32:
  COMPARE1(u32, <=);

#pragma endregion

#pragma region i64 comparison
#ifndef MANGO_NO_I64

CMP_EQ_I64:
  COMPARE2(i64, ==);

CMP_GT_I64:
  COMPARE2(i64, >);

CMP_GE_I64:
  COMPARE2(i64, >=);

CMP_LT_I64:
  COMPARE2(i64, <);

CMP_LE_I64:
  COMPARE2(i64, <=);

CMP_NE_I64:
  COMPARE2(u64, !=);

CMP_GT_U64:
  COMPARE2(u64, >);

CMP_GE_U64:
  COMPARE2(u64, >=);

CMP_LT_U64:
  COMPARE2(u64, <);

CMP_LE_U64:
  COMPARE2(u64, <=);

#else
CMP_EQ_I64:
CMP_GT_I64:
CMP_GE_I64:
CMP_LT_I64:
CMP_LE_I64:
CMP_NE_I64:
CMP_GT_U64:
CMP_GE_U64:
CMP_LT_U64:
CMP_LE_U64:
  INVALID;
#endif
#pragma endregion

#pragma region f32 comparison
#ifndef MANGO_NO_F32

CMP_OEQ_F32:
  COMPARE1(f32, ==);

CMP_ONE_F32:
  COMPARE1F(f32, islessgreater);

CMP_OGT_F32:
  COMPARE1F(f32, isgreater);

CMP_OGE_F32:
  COMPARE1F(f32, isgreaterequal);

CMP_OLT_F32:
  COMPARE1F(f32, isless);

CMP_OLE_F32:
  COMPARE1F(f32, islessequal);

CMP_UEQ_F32:
  COMPARE1F(f32, !islessgreater);

CMP_UNE_F32:
  COMPARE1(f32, !=);

CMP_UGT_F32:
  COMPARE1F(f32, !islessequal);

CMP_UGE_F32:
  COMPARE1F(f32, !isless);

CMP_ULT_F32:
  COMPARE1F(f32, !isgreaterequal);

CMP_ULE_F32:
  COMPARE1F(f32, !isgreater);

#else
CMP_OEQ_F32:
CMP_ONE_F32:
CMP_OGT_F32:
CMP_OGE_F32:
CMP_OLT_F32:
CMP_OLE_F32:
CMP_UEQ_F32:
CMP_UNE_F32:
CMP_UGT_F32:
CMP_UGE_F32:
CMP_ULT_F32:
CMP_ULE_F32:
  INVALID;
#endif
#pragma endregion

#pragma region f64 comparison
#ifndef MANGO_NO_F64

CMP_OEQ_F64:
  COMPARE2(f64, ==);

CMP_ONE_F64:
  COMPARE2F(f64, islessgreater);

CMP_OGT_F64:
  COMPARE2F(f64, isgreater);

CMP_OGE_F64:
  COMPARE2F(f64, isgreaterequal);

CMP_OLT_F64:
  COMPARE2F(f64, isless);

CMP_OLE_F64:
  COMPARE2F(f64, islessequal);

CMP_UEQ_F64:
  COMPARE2F(f64, !islessgreater);

CMP_UNE_F64:
  COMPARE2(f64, !=);

CMP_UGT_F64:
  COMPARE2F(f64, !islessequal);

CMP_UGE_F64:
  COMPARE2F(f64, !isless);

CMP_ULT_F64:
  COMPARE2F(f64, !isgreaterequal);

CMP_ULE_F64:
  COMPARE2F(f64, !isgreater);

#else
CMP_OEQ_F64:
CMP_ONE_F64:
CMP_OGT_F64:
CMP_OGE_F64:
CMP_OLT_F64:
CMP_OLE_F64:
CMP_UEQ_F64:
CMP_UNE_F64:
CMP_UGT_F64:
CMP_UGE_F64:
CMP_ULT_F64:
CMP_ULE_F64:
  INVALID;
#endif
#pragma endregion

#pragma region branches

BR:
  ip += 3 + FETCH(1, i16);
  NEXT;

BR_S:
  ip += 2 + FETCH(1, i8);
  NEXT;

BRFALSE:
  ip += 3 + (sp[0].i32 == 0 ? FETCH(1, i16) : 0);
  sp++;
  NEXT;

BRFALSE_S:
  ip += 2 + (sp[0].i32 == 0 ? FETCH(1, i8) : 0);
  sp++;
  NEXT;

BRTRUE:
  ip += 3 + (sp[0].i32 != 0 ? FETCH(1, i16) : 0);
  sp++;
  NEXT;

BRTRUE_S:
  ip += 2 + (sp[0].i32 != 0 ? FETCH(1, i8) : 0);
  sp++;
  NEXT;

#pragma endregion

#pragma region object model

NEWOBJ:
  do {
    const TypeDef *t = (const TypeDef *)(mp->image + FETCH(1, u16));

    void *obj = MangoHeapAlloc(vm, 1, t->size, __alignof(stackval), 0);
    if (!obj) {
      RETURN(MANGO_E_OUT_OF_MEMORY);
    }

    sp--;
    sp[0].ref = void_as_ref(vm, obj);
    ip += 3;
    NEXT;
  } while (0);

NEWARR:
  do {
    int32_t count = sp[0].i32;

    if (count < 0) {
      RETURN(MANGO_E_ARGUMENT);
    }

    const TypeDef *t = (const TypeDef *)(mp->image + FETCH(1, u16));

    void *arr =
        MangoHeapAlloc(vm, (uint32_t)count, t->size, __alignof(stackval), 0);

    if (!arr) {
      RETURN(MANGO_E_OUT_OF_MEMORY);
    }

    sp--;
    sp[0].ref = void_as_ref(vm, arr);
    ip += 3;
    NEXT;
  } while (0);

MKSLICE:
  goto NOP;

SLICE:
  do {
    int32_t start = sp[0].i32;

    if (start < 0 || start > sp[2].i32) {
      RETURN(MANGO_E_ARGUMENT);
    }

    sp++;
    sp[0].ref.address += (uint32_t)start;
    sp[1].i32 -= start;
    ip++;
    NEXT;
  } while (0);

SLICE2:
  do {
    int32_t start = sp[1].i32;
    int32_t length = sp[0].i32;

    if (start < 0 || start > sp[3].i32) {
      RETURN(MANGO_E_ARGUMENT);
    }
    if (length < 0 || length > sp[3].i32 - start) {
      RETURN(MANGO_E_ARGUMENT);
    }

    sp += 2;
    sp[0].ref.address += (uint32_t)start;
    sp[1].i32 = length;
    ip++;
    NEXT;
  } while (0);

LDLEN:
  goto POP;

LDFLD_I8:
  LOAD_FIELD(sp[0].ref, int8_t, i32);

LDFLD_I16:
  LOAD_FIELD(sp[0].ref, int16_t, i32);

LDFLD:
  LOAD_FIELD(sp[0].ref, int32_t, i32);

LDFLD2:
  LOAD_FIELD2(sp[0].ref);

LDFLDA:
  sp[0].ref.address += FETCH(1, u16);
  ip += 3;
  NEXT;

LDFLD_U8:
  LOAD_FIELD(sp[0].ref, uint8_t, u32);

LDFLD_U16:
  LOAD_FIELD(sp[0].ref, uint16_t, u32);

STFLD_I8:
  STORE_FIELD(sp[1].ref, int8_t, i32, 1);

STFLD_I16:
  STORE_FIELD(sp[1].ref, int16_t, i32, 1);

STFLD:
  STORE_FIELD(sp[1].ref, int32_t, i32, 1);

STFLD2:
  STORE_FIELD2(sp[2].ref, 1);

LDSFLD_I8:
  LOAD_FIELD(mp->static_data, int8_t, i32);

LDSFLD_I16:
  LOAD_FIELD(mp->static_data, int16_t, i32);

LDSFLD:
  LOAD_FIELD(mp->static_data, int32_t, i32);

LDSFLD2:
  LOAD_FIELD2(mp->static_data);

LDSFLDA:
  sp--;
  sp[0].ref.address = mp->static_data.address + FETCH(1, u16);
  ip += 3;
  NEXT;

LDSFLD_U8:
  LOAD_FIELD(mp->static_data, uint8_t, u32);

LDSFLD_U16:
  LOAD_FIELD(mp->static_data, uint16_t, u32);

STSFLD_I8:
  STORE_FIELD(mp->static_data, int8_t, i32, 0);

STSFLD_I16:
  STORE_FIELD(mp->static_data, int16_t, i32, 0);

STSFLD:
  STORE_FIELD(mp->static_data, int32_t, i32, 0);

STSFLD2:
  STORE_FIELD2(mp->static_data, 0);

LDELEM_I8: // index array length ... -> value ...
  LOAD_ELEMENT(int8_t, i32);

LDELEM_I16: // index array length ... -> value ...
  LOAD_ELEMENT(int16_t, i32);

LDELEM: // index array length ... -> value ...
  LOAD_ELEMENT(int32_t, i32);

LDELEM2: // index array length ... -> value1 value2 ...
  do {
    int32_t index = sp[0].i32;

    if (index < 0 || index >= sp[2].i32) {
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);
    }

    void *arr = void_as_ptr(vm, sp[1].ref);

    sp += 1;
    sp[0].i32 = ((int32_t *)arr)[2 * index + 0];
    sp[1].i32 = ((int32_t *)arr)[2 * index + 1];
    ip++;
    NEXT;
  } while (0);

LDELEM_U8: // index array length ... -> value ...
  LOAD_ELEMENT(uint8_t, u32);

LDELEM_U16: // index array length ... -> value ...
  LOAD_ELEMENT(uint16_t, u32);

LDELEMA1:
LDELEMA2:
LDELEMA4:
LDELEMA8: // index array length ... -> address ...
  do {
    int32_t index = sp[0].i32;

    if (index < 0 || index >= sp[2].i32) {
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);
    }

    uint32_t address = sp[1].ref.address;
    sp += 2;
    sp[0].ref.address = address + (uint32_t)index * (1U << (*ip - LDELEMA1));
    ip++;
    NEXT;
  } while (0);

LDELEMA: // index array length ... -> address ...
  do {
    int32_t index = sp[0].i32;

    if (index < 0 || index >= sp[2].i32) {
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);
    }

    const TypeDef *t = (const TypeDef *)(mp->image + FETCH(1, u16));

    uint32_t address = sp[1].ref.address;
    sp += 2;
    sp[0].ref.address = address + (uint32_t)index * t->size;
    ip += 3;
    NEXT;
  } while (0);

STELEM_I8: // value index array length ... -> ...
  STORE_ELEMENT(int8_t);

STELEM_I16: // value index array length ... -> ...
  STORE_ELEMENT(int16_t);

STELEM: // value index array length ... -> ...
  STORE_ELEMENT(int32_t);

STELEM2: // value1 value2 index array length ... -> ...
  do {
    int32_t value1 = sp[0].i32;
    int32_t value2 = sp[1].i32;
    int32_t index = sp[2].i32;

    if (index < 0 || index >= sp[4].i32) {
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);
    }

    void *arr = void_as_ptr(vm, sp[3].ref);

    sp += 5;
    ((int32_t *)arr)[2 * index + 0] = value1;
    ((int32_t *)arr)[2 * index + 1] = value2;
    ip++;
    NEXT;
  } while (0);

#pragma endregion

#pragma region unused

UNUSED2:
UNUSED3:
UNUSED14:
UNUSED15:
UNUSED21:
UNUSED22:
UNUSED23:
UNUSED29:
UNUSED30:
UNUSED31:
UNUSED74:
UNUSED75:
UNUSED76:
UNUSED77:
UNUSED78:
UNUSED79:
UNUSED134:
UNUSED135:
UNUSED180:
UNUSED181:
UNUSED182:
UNUSED183:
UNUSED190:
UNUSED191:
UNUSED198:
UNUSED199:
UNUSED237:
UNUSED238:
UNUSED239:
UNUSED240:
UNUSED241:
UNUSED242:
UNUSED243:
UNUSED244:
UNUSED245:
UNUSED246:
UNUSED247:
UNUSED248:
UNUSED249:
UNUSED250:
UNUSED251:
UNUSED252:
UNUSED253:
UNUSED254:
  INVALID;

#pragma endregion

invalid:
  printf("<< INVALID PROGRAM >>\n");
  result = MANGO_E_INVALID_PROGRAM;

done:
  vm->sf = PACK_STATE();
  vm->sp = (uint16_t)(sp - stackval_as_ptr(vm, vm->stack));
  vm->rp = (uint16_t)(rp - stackval_as_ptr(vm, vm->stack));
  return result;
}

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

////////////////////////////////////////////////////////////////////////////////
