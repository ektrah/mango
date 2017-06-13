/*
 *  _____ _____ _____ _____ _____
 * |     |  _  |   | |   __|     |
 * | | | |     | | | |  |  |  |  |
 * |_|_|_|__|__|_|___|_____|_____|
 *
 * Mango Virtual Machine 0.34-dev
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

#include "mango.h"
#include "mango_metadata.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
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
  typedef struct Type##_ref { uint32_t address; } Type##_ref;

#define MANGO_DEFINE_REF_TYPE(Type, Const)                                     \
                                                                               \
  static inline Const Type *Type##_as_ptr(const mango_vm *vm,                  \
                                          Type##_ref ref) {                    \
    (void)vm;                                                                  \
    return (Const Type *)ref.address;                                          \
  }                                                                            \
                                                                               \
  static inline Type##_ref Type##_as_ref(const mango_vm *vm,                   \
                                         Const Type *ptr) {                    \
    (void)vm;                                                                  \
    return (Type##_ref){(uint32_t)ptr};                                        \
  }                                                                            \
                                                                               \
  static inline bool Type##_is_null(Type##_ref ref) { return !ref.address; }

#elif UINTPTR_MAX == UINT64_MAX

#define MANGO_DECLARE_REF_TYPE(Type)                                           \
                                                                               \
  typedef struct Type##_ref { uint32_t address; } Type##_ref;

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
  static inline bool Type##_is_null(Type##_ref ref) { return !ref.address; }

#else
#error Unsupported bitness
#endif

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error Unsupported byte order
#endif

////////////////////////////////////////////////////////////////////////////////

#define MANGO_MEMORY_MAGIC 127

MANGO_DECLARE_REF_TYPE(void)
MANGO_DECLARE_REF_TYPE(uint8_t)
MANGO_DECLARE_REF_TYPE(stackval)
MANGO_DECLARE_REF_TYPE(mango_module)

#pragma pack(push, 4)

typedef enum opcode {
#define OPCODE(c, s, pop, push, args, i) c,
#include "mango_opcodes.inc"
#undef OPCODE
} opcode;

typedef struct stack_frame {
  uint8_t in_full_trust : 1;
  uint8_t pop : 7;
  uint8_t module;
  uint16_t ip;
} stack_frame;

typedef struct function_token {
  uint8_t add_sp : 4;
  uint8_t add_ip : 4;
  uint8_t module;
  uint16_t ip;
} function_token;

typedef union stackval {
  stack_frame sf;
  function_token ftn;
  int32_t i32;
  uint32_t u32;
#ifndef MANGO_NO_F32
  float f32;
#endif
#ifndef MANGO_NO_REFS
  void_ref ref;
#endif
} stackval;

typedef union stackval2 {
  struct {
    void_ref ref;
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

typedef struct mango_vm {
  uint8_t magic;
  uint8_t version;

  uint16_t syscall;

  uint32_t heap_size;
  uint32_t heap_used;

  mango_module_name app_name;

  uint8_t flags;
  uint8_t modules_created;
  uint8_t modules_imported;
  uint8_t module_init_head;
  mango_module_ref modules;

  stackval_ref stack;
  uint16_t stack_size;
  uint16_t rp;
  uint16_t sp;
  uint16_t sp_expected;
  stack_frame sf;

  uint32_t _reserved2;
  uint32_t _reserved3;

  union {
    void *context;
    uint32_t _context[2];
  };
} mango_vm;

typedef struct mango_module {
  union {
    const uint8_t *image;
    uint32_t _image[2];
  };

  uint16_t image_size;
  uint8_t index;
  uint8_t flags;

  uint8_t init_next;
  uint8_t init_prev;
  uint8_t name_module;
  uint8_t name_index;

  uint8_t_ref imports;
  void_ref static_data;

  union {
    void *context;
    uint32_t _context[2];
  };
} mango_module;

#pragma pack(pop)

#pragma pack(push, 1)

typedef union packed {
  int8_t i8;
  uint8_t u8;
  int16_t i16;
  uint16_t u16;
  int32_t i32;
  uint32_t u32;
} packed;

#pragma pack(pop)

MANGO_DEFINE_REF_TYPE(void, )
MANGO_DEFINE_REF_TYPE(uint8_t, const)
MANGO_DEFINE_REF_TYPE(stackval, )
MANGO_DEFINE_REF_TYPE(mango_module, )

#if defined(__clang__) || defined(__GNUC__)
_Static_assert(sizeof(stack_frame) == 4, "Incorrect layout");
_Static_assert(__alignof(stack_frame) == 2, "Incorrect layout");
_Static_assert(sizeof(stackval) == 4, "Incorrect layout");
_Static_assert(__alignof(stackval) == 4, "Incorrect layout");
_Static_assert(sizeof(stackval2) == 8, "Incorrect layout");
_Static_assert(__alignof(stackval2) == 4, "Incorrect layout");
_Static_assert(sizeof(mango_vm) == 64, "Incorrect layout");
_Static_assert(__alignof(mango_vm) == 4, "Incorrect layout");
_Static_assert(sizeof(mango_module) == 32, "Incorrect layout");
_Static_assert(__alignof(mango_module) == 4, "Incorrect layout");
_Static_assert(__alignof(packed) == 1, "Incorrect layout");
#endif

////////////////////////////////////////////////////////////////////////////////

uint32_t mango_version_major(void) { return MANGO_VERSION_MAJOR; }

uint32_t mango_version_minor(void) { return MANGO_VERSION_MINOR; }

const char *mango_version_string(void) { return MANGO_VERSION_STRING; }

uint32_t mango_features(void) {
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
#ifndef MANGO_NO_REFS
  features |= MANGO_FEATURE_REFS;
#endif
  return features;
}

////////////////////////////////////////////////////////////////////////////////

mango_vm *mango_initialize(void *address, uint32_t size, void *context) {
  if (!address || size < sizeof(mango_vm)) {
    return NULL;
  }
  if (((uintptr_t)address & (__alignof(mango_vm) - 1)) != 0) {
    return NULL;
  }

  printf("allocate %u bytes\n", (uint32_t)sizeof(mango_vm));

  mango_vm *vm = address;
  memset(vm, 0, sizeof(mango_vm));
  vm->magic = MANGO_MEMORY_MAGIC;
  vm->version = MANGO_VERSION_MAJOR;
  vm->heap_size = size;
  vm->heap_used = sizeof(mango_vm);
  vm->context = context;
  return vm;
}

void *mango_context(const mango_vm *vm) { return vm ? vm->context : NULL; }

////////////////////////////////////////////////////////////////////////////////

void *mango_heap_alloc(mango_vm *vm, uint32_t count, uint32_t size,
                       uint32_t alignment, uint32_t flags) {
  uint32_t total_size;
  uint32_t offset;
  uint32_t available;

  if (!vm) {
    return NULL;
  }
  if (!(alignment == 1 || alignment == 2 || alignment == 4) ||
      __builtin_mul_overflow(count, size, &total_size) ||
      __builtin_add_overflow(vm->heap_used, alignment - 1, &offset)) {
    return NULL;
  }
  offset &= ~(alignment - 1);
  if (__builtin_sub_overflow(vm->heap_size, offset, &available) ||
      total_size > available) {
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

uint32_t mango_heap_size(const mango_vm *vm) { return vm ? vm->heap_size : 0; }

uint32_t mango_heap_available(const mango_vm *vm) {
  return vm ? vm->heap_size - vm->heap_used : 0;
}

////////////////////////////////////////////////////////////////////////////////

mango_result mango_stack_create(mango_vm *vm, uint32_t size) {
  if (!vm) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (size > UINT16_MAX * sizeof(stackval)) {
    return MANGO_E_ARGUMENT;
  }
  if ((size & (sizeof(stackval) - 1)) != 0) {
    return MANGO_E_ARGUMENT;
  }
  if (!stackval_is_null(vm->stack)) {
    return MANGO_E_INVALID_OPERATION;
  }

  stackval *stack = mango_heap_alloc(vm, 1, size, __alignof(stackval), 0);

  if (!stack) {
    return MANGO_E_OUT_OF_MEMORY;
  }

  uint16_t count = (uint16_t)(size / sizeof(stackval));

  vm->stack = stackval_as_ref(vm, stack);
  vm->stack_size = count;
  vm->rp = 0;
  vm->sp = count;
  vm->sp_expected = count;
  return MANGO_E_SUCCESS;
}

void *mango_stack_alloc(mango_vm *vm, uint32_t size, uint32_t flags) {
  if (!vm || stackval_is_null(vm->stack)) {
    return NULL;
  }

  uint32_t count = (size + sizeof(stackval) - 1) / sizeof(stackval);

  if ((uint32_t)vm->sp - (uint32_t)vm->rp < count) {
    return NULL;
  }

  vm->sp -= count;

  stackval *block = stackval_as_ptr(vm, vm->stack) + vm->sp;

  if ((flags & MANGO_ALLOC_ZERO_MEMORY) != 0) {
    memset(block, 0, size);
  }

  return block;
}

mango_result mango_stack_free(mango_vm *vm, uint32_t size) {
  if (!vm) {
    return MANGO_E_ARGUMENT_NULL;
  }

  uint32_t count = (size + sizeof(stackval) - 1) / sizeof(stackval);

  if ((uint32_t)vm->stack_size - (uint32_t)vm->sp < count) {
    return MANGO_E_STACK_OVERFLOW;
  }

  vm->sp += count;
  return MANGO_E_SUCCESS;
}

void *mango_stack_top(const mango_vm *vm) {
  if (!vm || stackval_is_null(vm->stack) || vm->sp == vm->stack_size) {
    return NULL;
  }

  return stackval_as_ptr(vm, vm->stack) + vm->sp;
}

uint32_t mango_stack_size(const mango_vm *vm) {
  return vm ? (uint32_t)vm->stack_size * sizeof(stackval) : 0;
}

uint32_t mango_stack_available(const mango_vm *vm) {
  return vm ? ((uint32_t)vm->sp - (uint32_t)vm->rp) * sizeof(stackval) : 0;
}

////////////////////////////////////////////////////////////////////////////////

#define INVALID_MODULE 255

static inline const mango_module_name *
_mango_get_module_name(const mango_module *modules,
                       const mango_module *module) {
  const mango_module_def *m =
      (const mango_module_def *)(modules[module->name_module].image);

  return &m->imports[module->name_index];
}

static uint8_t _mango_get_or_create_module(mango_vm *vm,
                                           const mango_module_name *name,
                                           uint8_t name_module,
                                           uint8_t name_index) {
  mango_module *modules = mango_module_as_ptr(vm, vm->modules);

  if (memcmp(name, &vm->app_name, sizeof(mango_module_name)) == 0) {
    return 0;
  }

  for (uint_fast8_t i = 1; i < vm->modules_created; i++) {
    const mango_module_name *n = _mango_get_module_name(modules, &modules[i]);
    if (memcmp(name, n, sizeof(mango_module_name)) == 0) {
      return i;
    }
  }

  uint8_t index = vm->modules_created++;
  modules[index].index = index;
  modules[index].name_module = name_module;
  modules[index].name_index = name_index;
  return index;
}

static mango_result _mango_initialize_module(mango_vm *vm,
                                             mango_module *module) {
  const mango_module_def *m = (const mango_module_def *)(module->image);

  if (m->import_count != 0) {
    uint8_t *imports = mango_heap_alloc(vm, m->import_count, sizeof(uint8_t),
                                        __alignof(uint8_t), 0);

    if (!imports) {
      return MANGO_E_OUT_OF_MEMORY;
    }

    for (uint_fast8_t i = 0; i < m->import_count; i++) {
      imports[i] =
          _mango_get_or_create_module(vm, &m->imports[i], module->index, i);
    }

    module->imports = uint8_t_as_ref(vm, imports);
  }

  return MANGO_E_SUCCESS;
}

static mango_result _mango_import_startup_module(mango_vm *vm,
                                                 const uint8_t *name,
                                                 const uint8_t *image,
                                                 uint32_t size, void *context,
                                                 uint32_t flags) {
  const mango_module_def *m = (const mango_module_def *)image;

  if ((m->attributes & MANGO_MD_EXECUTABLE) == 0) {
    return MANGO_E_BAD_IMAGE_FORMAT;
  }

  const mango_app_info *app =
      (const mango_app_info *)(image + (size - sizeof(mango_app_info)));

  if ((app->features & mango_features()) != app->features) {
    return MANGO_E_NOT_SUPPORTED;
  }
  if (app->entry_point[3] != HALT) {
    return MANGO_E_BAD_IMAGE_FORMAT;
  }

  if (stackval_is_null(vm->stack)) {
    mango_result result =
        mango_stack_create(vm, app->stack_size * sizeof(stackval));
    if (result != MANGO_E_SUCCESS) {
      return result;
    }
  }

  mango_module *modules = mango_heap_alloc(
      vm, app->module_count, sizeof(mango_module), __alignof(mango_module), 0);

  if (!modules) {
    return MANGO_E_OUT_OF_MEMORY;
  }

  memcpy(&vm->app_name, name, sizeof(mango_module_name));
  vm->modules_created = 1;
  vm->modules_imported = 1;
  vm->modules = mango_module_as_ref(vm, modules);
  vm->sf = (stack_frame){false, 0, 0, (uint16_t)(&app->entry_point[3] - image)};

  mango_module *module = &modules[0];
  module->image = image;
  module->image_size = (uint16_t)size;
  module->index = 0;
  module->flags = (uint8_t)flags;
  module->init_next = INVALID_MODULE;
  module->init_prev = INVALID_MODULE;
  module->name_module = INVALID_MODULE;
  module->name_index = INVALID_MODULE;
  module->imports = (uint8_t_ref){0};
  module->static_data = (void_ref){0};
  module->context = context;

  return _mango_initialize_module(vm, module);
}

static mango_result _mango_import_missing_module(mango_vm *vm,
                                                 const uint8_t *name,
                                                 const uint8_t *image,
                                                 uint32_t size, void *context,
                                                 uint32_t flags) {
  mango_module *modules = mango_module_as_ptr(vm, vm->modules);
  mango_module *module = &modules[vm->modules_imported];
  const mango_module_name *n = _mango_get_module_name(modules, module);

  if (memcmp(name, n, sizeof(mango_module_name)) != 0) {
    return MANGO_E_INVALID_OPERATION;
  }

  vm->modules_imported++;

  module->image = image;
  module->image_size = (uint16_t)size;
  module->flags = (uint8_t)flags;
  module->init_next = INVALID_MODULE;
  module->init_prev = INVALID_MODULE;
  module->imports = (uint8_t_ref){0};
  module->static_data = (void_ref){0};
  module->context = context;

  return _mango_initialize_module(vm, module);
}

mango_result mango_module_import(mango_vm *vm, const uint8_t *name,
                                 const uint8_t *image, uint32_t size,
                                 void *context, uint32_t flags) {
  if (!vm || !name || !image) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (size < sizeof(mango_module_def) || size > UINT16_MAX) {
    return MANGO_E_ARGUMENT;
  }
  if (image[0] != MANGO_IMAGE_MAGIC || image[1] != MANGO_VERSION_MAJOR) {
    return MANGO_E_BAD_IMAGE_FORMAT;
  }
  if ((flags & MANGO_IMPORT_SKIP_VERIFICATION) == 0) {
    return MANGO_E_VERIFICATION;
  }

  if (vm->modules_imported == 0) {
    return _mango_import_startup_module(vm, name, image, size, context, flags);
  } else if (vm->modules_imported != vm->modules_created) {
    return _mango_import_missing_module(vm, name, image, size, context, flags);
  } else {
    return MANGO_E_INVALID_OPERATION;
  }
}

const uint8_t *mango_module_missing(const mango_vm *vm) {
  if (!vm) {
    return NULL;
  }
  if (vm->modules_imported == vm->modules_created) {
    return NULL;
  }

  mango_module *modules = mango_module_as_ptr(vm, vm->modules);
  mango_module *module = &modules[vm->modules_imported];
  const mango_module_name *n = _mango_get_module_name(modules, module);

  return (const uint8_t *)n;
}

void *mango_module_context(const mango_vm *vm) {
  if (!vm) {
    return NULL;
  }
  if (vm->modules_imported == 0 ||
      vm->modules_imported != vm->modules_created) {
    return NULL;
  }

  mango_module *modules = mango_module_as_ptr(vm, vm->modules);
  mango_module *module = &modules[vm->sf.module];

  return module->context;
}

////////////////////////////////////////////////////////////////////////////////

#define VISITED 1

static mango_result _mango_execute(mango_vm *vm);

mango_result mango_execute(mango_vm *vm) {
  if (!vm) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (vm->modules_imported == 0 ||
      vm->modules_imported != vm->modules_created) {
    return MANGO_E_INVALID_OPERATION;
  }
  if (stackval_is_null(vm->stack)) {
    return MANGO_E_INVALID_OPERATION;
  }
  if (vm->sp != vm->sp_expected) {
    return MANGO_E_STACK_IMBALANCE;
  }

  mango_result result = _mango_execute(vm);
  if (result != MANGO_E_SUCCESS) {
    return result;
  }

  mango_module *modules = mango_module_as_ptr(vm, vm->modules);
  uint8_t head = vm->module_init_head;

  while (head != INVALID_MODULE) {
    mango_module *module = &modules[head];
    const mango_module_def *m = (const mango_module_def *)(module->image);

    if ((module->flags & VISITED) == 0) {
      module->flags |= VISITED;

      const uint8_t *imports = uint8_t_as_ptr(vm, module->imports);

      for (uint_fast8_t i = 0; i < m->import_count; i++) {
        uint8_t p = imports[i];
        mango_module *import = &modules[p];

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
      vm->sf = (stack_frame){false, 0, module->index,
                             (uint16_t)offsetof(mango_module_def, initializer)};
      result = _mango_execute(vm);
      if (result != MANGO_E_SUCCESS) {
        return result;
      }
    }
  }

  if ((vm->flags & VISITED) == 0) {
    vm->flags |= VISITED;

    printf("execute main\n");
    vm->sf = (stack_frame){false, 0, 0,
                           (uint16_t)(modules[0].image_size -
                                      (sizeof(mango_app_info) -
                                       offsetof(mango_app_info, entry_point)))};
    return _mango_execute(vm);
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

uint32_t mango_syscall(const mango_vm *vm) { return vm ? vm->syscall : 0; }

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
  } while (false)
#else
#error Unsupported compiler
#define NEXT
#endif

#define FETCH(offset, ty) (((const packed *)(ip + (offset)))->ty)

#define YIELD(Result)                                                          \
  do {                                                                         \
    result = Result;                                                           \
    goto yield;                                                                \
  } while (false)

#define RETURN(Result)                                                         \
  do {                                                                         \
    result = Result;                                                           \
    goto done;                                                                 \
  } while (false)

#define INVALID                                                                \
  do {                                                                         \
    goto invalid;                                                              \
  } while (false)

#define BINARY1(ty, op)                                                        \
  do {                                                                         \
    sp[1].ty = sp[1].ty op sp[0].ty;                                           \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define BINARY1D(ty, op)                                                       \
  do {                                                                         \
    if (sp[0].ty == 0) {                                                       \
      RETURN(MANGO_E_DIVIDE_BY_ZERO);                                          \
    }                                                                          \
    sp[1].ty = sp[1].ty op sp[0].ty;                                           \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define BINARY2(ty, op)                                                        \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[1].ty = sp2[1].ty op sp2[0].ty;                                        \
    sp += 2;                                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

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
  } while (false);

#define UNARY1(ty, op)                                                         \
  do {                                                                         \
    sp[0].ty = op(sp[0].ty);                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define UNARY2(ty, op)                                                         \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[0].ty = op(sp2[0].ty);                                                 \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define SHIFT1(ty, op)                                                         \
  do {                                                                         \
    sp[1].ty = sp[1].ty op(sp[0].i32 & 31);                                    \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define SHIFT2(ty, op)                                                         \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)(sp + 1);                                    \
    sp2[0].ty = sp2[0].ty op(sp[0].i32 & 63);                                  \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define CONVERT1(cast, dest, src)                                              \
  do {                                                                         \
    sp[0].dest = (cast)sp[0].src;                                              \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define CONVERT21(cast, dest, src)                                             \
  do {                                                                         \
    cast tmp = (cast)sp[0].src;                                                \
    sp--;                                                                      \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[0].dest = tmp;                                                         \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define CONVERT12(cast, dest, src)                                             \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    cast tmp = (cast)sp2[0].src;                                               \
    sp++;                                                                      \
    sp[0].dest = tmp;                                                          \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define CONVERT2(cast, dest, src)                                              \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[0].dest = (cast)sp2[0].src;                                            \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define COMPARE1(ty, op)                                                       \
  do {                                                                         \
    sp[1].i32 = sp[1].ty op sp[0].ty;                                          \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define COMPARE1F(ty, op)                                                      \
  do {                                                                         \
    sp[1].i32 = op(sp[1].ty, sp[0].ty);                                        \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define COMPARE2(ty, op)                                                       \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    int32_t tmp = sp2[1].ty op sp2[0].ty;                                      \
    sp += 3;                                                                   \
    sp[0].i32 = tmp;                                                           \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#define COMPARE2F(ty, op)                                                      \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    int32_t tmp = op(sp2[1].ty, sp2[0].ty);                                    \
    sp += 3;                                                                   \
    sp[0].i32 = tmp;                                                           \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false);

#pragma endregion

static mango_result _mango_execute(mango_vm *vm) {
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
  stack_frame sf;
  const uint8_t *ip;
  function_token ftn;
  mango_result result;

  rp = stackval_as_ptr(vm, vm->stack) + vm->rp;
  sp = stackval_as_ptr(vm, vm->stack) + vm->sp;
  sf = vm->sf;
  ip = mango_module_as_ptr(vm, vm->modules)[sf.module].image + sf.ip;

  NEXT;

#pragma region basic

HALT: // ... -> ...
  if (sp - stackval_as_ptr(vm, vm->stack) < vm->stack_size) {
    vm->sp_expected = vm->stack_size;
    vm->syscall = 0;
    YIELD(MANGO_E_STACK_IMBALANCE);
  }
  RETURN(MANGO_E_SUCCESS);

NOP: // ... -> ...
  ++ip;
  NEXT;

BREAK: // ... -> ...
  ++ip;
  RETURN(MANGO_E_BREAKPOINT);

UNUSED3:
  INVALID;

POP_X32: // value ... -> ...
  sp++;
  ip++;
  NEXT;

POP_X64: // value ... -> ...
  sp += 2;
  ip++;
  NEXT;

DUP_X32: // value ... -> value value ...
  sp--;
  sp[0].u32 = sp[1].u32;
  ip++;
  NEXT;

DUP_X64: // value ... -> value value ...
  sp -= 2;
  sp[0].u32 = sp[2].u32;
  sp[1].u32 = sp[3].u32;
  ip++;
  NEXT;

SWAP: // value1 value2 ... -> value2 value1 ...
  do {
    uint32_t tmp = sp[0].u32;
    sp[0].u32 = sp[1].u32;
    sp[1].u32 = tmp;
    ip++;
    NEXT;
  } while (false);

OVER: // value1 value2 ... -> value2 value1 value2 ...
  sp--;
  sp[0].u32 = sp[2].u32;
  ip++;
  NEXT;

ROT: // value1 value2 value3 ... -> value2 value3 value1 ...
  do {
    uint32_t tmp = sp[0].u32;
    sp[0].u32 = sp[1].u32;
    sp[1].u32 = sp[2].u32;
    sp[2].u32 = tmp;
    ip++;
    NEXT;
  } while (false);

NIP: // value1 value2 ... -> value1 ...
  sp[1].u32 = sp[0].u32;
  sp++;
  ip++;
  NEXT;

TUCK: // value1 value2 ... -> value1 value2 value1 ...
  sp--;
  sp[0].u32 = sp[1].u32;
  sp[1].u32 = sp[2].u32;
  sp[2].u32 = sp[0].u32;
  ip++;
  NEXT;

UNUSED13:
UNUSED14:
UNUSED15:
  INVALID;

#pragma endregion

#pragma region locals

LDLOC_X32: // ... -> value ...
  do {
    uint8_t slot = FETCH(1, u8);
    uint32_t value = sp[slot].u32;
    sp--;
    sp[0].u32 = value;
    ip += 2;
    NEXT;
  } while (false);

LDLOC_X64: // ... -> value ...
  do {
    uint8_t slot = FETCH(1, u8);
    uint32_t value1 = sp[slot + 0].u32;
    uint32_t value2 = sp[slot + 1].u32;
    sp -= 2;
    sp[0].u32 = value1;
    sp[1].u32 = value2;
    ip += 2;
    NEXT;
  } while (false);

LDLOCA: // ... -> address ...
#ifndef MANGO_NO_REFS
  do {
    uint8_t slot = FETCH(1, u8);
    void *obj = &sp[slot];
    sp--;
    sp[0].ref = void_as_ref(vm, obj);
    ip += 2;
    NEXT;
  } while (false);
#else
  INVALID;
#endif

STLOC_X32: // value ... -> ...
  do {
    uint8_t slot = FETCH(1, u8);
    sp[slot].u32 = sp[0].u32;
    sp++;
    ip += 2;
    NEXT;
  } while (false);

STLOC_X64: // value ... -> ...
  do {
    uint8_t slot = FETCH(1, u8);
    sp[slot + 0].u32 = sp[0].u32;
    sp[slot + 1].u32 = sp[1].u32;
    sp += 2;
    ip += 2;
    NEXT;
  } while (false);

UNUSED21:
UNUSED22:
UNUSED23:
  INVALID;

#pragma endregion

#pragma region calls

RET_X64: // value ... -> ...
  sp[sf.pop + 1].u32 = sp[1].u32;

RET_X32: // value ... -> ...
  sp[sf.pop + 0].u32 = sp[0].u32;

RET: // ... -> ...
  sp += sf.pop;
  --rp;

  if (sf.in_full_trust && !rp->sf.in_full_trust) {
    printf("------------------------------ LEAVE FULL TRUST "
           "------------------------------\n");
  }

  sf = rp->sf;
  ip = mango_module_as_ptr(vm, vm->modules)[sf.module].image + sf.ip;
  NEXT;

CALLI: // ftn argumentN ... argument1 argument0 ... -> result ...
  ftn = sp[0].ftn;
  goto call;

CALL_S: // argumentN ... argument1 argument0 ... -> result ...
  ftn = (function_token){0, 3, sf.module, FETCH(1, u16)};
  goto call;

CALL: // argumentN ... argument1 argument0 ... -> result ...
  do {
    uint8_t import = FETCH(1, u8);
    uint16_t offset = FETCH(2, u16);
    if (import == INVALID_MODULE) {
      ftn = (function_token){0, 4, sf.module, offset};
    } else {
      const mango_module *modules = mango_module_as_ptr(vm, vm->modules);
      const mango_module *mp = &modules[sf.module];
      const uint8_t *imports = uint8_t_as_ptr(vm, mp->imports);
      ftn = (function_token){0, 4, imports[import], offset};
    }
    goto call;
  } while (false);

call:
  do {
    const mango_module *modules = mango_module_as_ptr(vm, vm->modules);
    const mango_module *mp = &modules[sf.module];
    const mango_module *module = &modules[ftn.module];
    const mango_func_def *f = (const mango_func_def *)(module->image + ftn.ip);

    bool in_full_trust = sf.in_full_trust;
    if (!in_full_trust &&
        (f->attributes &
         (MANGO_FD_SECURITY_CRITICAL | MANGO_FD_SECURITY_SAFE_CRITICAL)) != 0) {
      if ((f->attributes & MANGO_FD_SECURITY_SAFE_CRITICAL) == 0 ||
          (module->flags & MANGO_IMPORT_TRUSTED_MODULE) == 0) {
        printf("<< SECURITY VIOLATION >>\n");
        RETURN(MANGO_E_SECURITY);
      }
      printf("------------------------------ ENTER FULL TRUST "
             "------------------------------\n");
      in_full_trust = true;
    }

    if (sp - rp < 1 + f->loc_count + f->max_stack) {
      printf("<< STACK OVERFLOW >>\n");
      RETURN(MANGO_E_STACK_OVERFLOW);
    }

    sp += ftn.add_sp;
    ip += ftn.add_ip;

    if (!(sf.pop == 0 && *ip == RET)) {
      sf.ip = (uint16_t)(ip - mp->image);
      rp->sf = sf;
      rp++;
    }

    sf.in_full_trust = in_full_trust;
    sf.pop = f->arg_count + f->loc_count;
    sf.module = module->index;
    sp -= f->loc_count;
    ip = f->code;

    if ((f->attributes & MANGO_FD_INIT_LOCALS) != 0) {
      for (uint_fast8_t i = 0, n = f->loc_count; i < n; i++) {
        sp[i].u32 = 0;
      }
    }

    NEXT;
  } while (false);

SYSCALL: // argumentN ... argument1 argument0 ... -> result ...
  do {
    if (!sf.in_full_trust) {
      printf("<< SECURITY VIOLATION >>\n");
      RETURN(MANGO_E_SECURITY);
    }

    int8_t adjustment = FETCH(1, i8);
    uint16_t syscall = FETCH(2, u16);

    ip += 4;
    vm->sp_expected =
        (uint16_t)((sp - stackval_as_ptr(vm, vm->stack)) + adjustment);
    vm->syscall = syscall;

    YIELD(MANGO_E_SYSCALL);
  } while (false);

UNUSED31:
  INVALID;

#pragma endregion

#pragma region branches

BR_S: // ... -> ...
  ip += 2 + FETCH(1, i8);
  NEXT;

BRFALSE_S: // value ... -> ...
  ip += 2 + (sp[0].u32 == 0 ? FETCH(1, i8) : 0);
  sp++;
  NEXT;

BRTRUE_S: // value ... -> ...
  ip += 2 + (sp[0].u32 != 0 ? FETCH(1, i8) : 0);
  sp++;
  NEXT;

BR: // ... -> ...
  ip += 3 + FETCH(1, i16);
  NEXT;

BRFALSE: // value ... -> ...
  ip += 3 + (sp[0].u32 == 0 ? FETCH(1, i16) : 0);
  sp++;
  NEXT;

BRTRUE: // value ... -> ...
  ip += 3 + (sp[0].u32 != 0 ? FETCH(1, i16) : 0);
  sp++;
  NEXT;

UNUSED38:
UNUSED39:
  INVALID;

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
LDC_I32_8: // ... -> value ...
  sp--;
  sp[0].i32 = (int32_t)(*ip) - LDC_I32_0;
  ip++;
  NEXT;

LDC_I32_S: // ... -> value ...
  sp--;
  sp[0].i32 = FETCH(1, i8);
  ip += 2;
  NEXT;

LDC_X32: // ... -> value ...
  sp--;
  sp[0].u32 = FETCH(1, u32);
  ip += 5;
  NEXT;

LDC_X64: // ... -> value ...
  sp -= 2;
  sp[0].u32 = FETCH(1, u32);
  sp[1].u32 = FETCH(5, u32);
  ip += 9;
  NEXT;

LDFTN: // ... -> ftn ...
  do {
    uint8_t import = FETCH(1, u8);
    uint16_t offset = FETCH(2, u16);
    if (import == INVALID_MODULE) {
      ftn = (function_token){1, 1, sf.module, offset};
    } else {
      const mango_module *modules = mango_module_as_ptr(vm, vm->modules);
      const mango_module *mp = &modules[sf.module];
      const uint8_t *imports = uint8_t_as_ptr(vm, mp->imports);
      ftn = (function_token){1, 1, imports[import], offset};
    }

    sp--;
    sp[0].ftn = ftn;
    ip += 4;
    NEXT;
  } while (false);

UNUSED54:
UNUSED55:
  INVALID;

UNUSED56:
UNUSED57:
UNUSED58:
UNUSED59:
UNUSED60:
UNUSED61:
UNUSED62:
UNUSED63:
  INVALID;

#pragma endregion

#pragma region i32 arithmetic

ADD_I32: // value2 value1 ... -> result ...
  BINARY1(u32, +);

SUB_I32: // value2 value1 ... -> result ...
  BINARY1(u32, -);

MUL_I32: // value2 value1 ... -> result ...
  BINARY1(u32, *);

DIV_I32: // value2 value1 ... -> result ...
  BINARY1D(i32, /);

DIV_I32_UN: // value2 value1 ... -> result ...
  BINARY1D(u32, /);

REM_I32: // value2 value1 ... -> result ...
  BINARY1D(i32, %);

REM_I32_UN: // value2 value1 ... -> result ...
  BINARY1D(u32, %);

NEG_I32: // value ... -> result ...
  UNARY1(u32, -);

#pragma endregion

#pragma region i32 bitwise

SHL_I32: // amount value ... -> result ...
  SHIFT1(u32, <<);

SHR_I32: // amount value ... -> result ...
  SHIFT1(i32, >>);

SHR_I32_UN: // amount value ... -> result ...
  SHIFT1(u32, >>);

AND_I32: // value2 value1 ... -> result ...
  BINARY1(u32, &);

OR_I32: // value2 value1 ... -> result ...
  BINARY1(u32, |);

XOR_I32: // value2 value1 ... -> result ...
  BINARY1(u32, ^);

NOT_I32: // value ... -> result ...
  UNARY1(u32, ~);

#pragma endregion

#pragma region i32 comparison

CEQ_I32: // value2 value1 ... -> result ...
  COMPARE1(u32, ==);

CNE_I32: // value2 value1 ... -> result ...
  COMPARE1(u32, !=);

CGT_I32: // value2 value1 ... -> result ...
  COMPARE1(i32, >);

CGT_I32_UN: // value2 value1 ... -> result ...
  COMPARE1(u32, >);

CGE_I32: // value2 value1 ... -> result ...
  COMPARE1(i32, >=);

CGE_I32_UN: // value2 value1 ... -> result ...
  COMPARE1(u32, >=);

CLT_I32: // value2 value1 ... -> result ...
  COMPARE1(i32, <);

CLT_I32_UN: // value2 value1 ... -> result ...
  COMPARE1(u32, <);

CLE_I32: // value2 value1 ... -> result ...
  COMPARE1(i32, <=);

CLE_I32_UN: // value2 value1 ... -> result ...
  COMPARE1(u32, <=);

#pragma endregion

#pragma region i32 conversion

CONV_I8_I32: // value ... -> result ...
  CONVERT1(int8_t, i32, i32);

CONV_U8_I32: // value ... -> result ...
  CONVERT1(uint8_t, u32, u32);

CONV_I16_I32: // value ... -> result ...
  CONVERT1(int16_t, i32, i32);

CONV_U16_I32: // value ... -> result ...
  CONVERT1(uint16_t, u32, u32);

UNUSED93:
UNUSED94:
UNUSED95:
  INVALID;

#pragma endregion

#if !defined(MANGO_NO_REFS)

#pragma region object model

NEWOBJ: // ... -> address ...
  do {
    uint32_t size = FETCH(1, u16);
    void *obj = mango_heap_alloc(vm, 1, size, __alignof(stackval),
                                 MANGO_ALLOC_ZERO_MEMORY);
    if (!obj) {
      RETURN(MANGO_E_OUT_OF_MEMORY);
    }
    sp--;
    sp[0].ref = void_as_ref(vm, obj);
    ip += 3;
    NEXT;
  } while (false);

NEWARR: // length ... -> array length ...
  do {
    uint32_t length = sp[0].u32;
    if ((int32_t)length < 0) {
      RETURN(MANGO_E_ARGUMENT);
    }
    uint32_t size = FETCH(1, u16);
    void *arr = mango_heap_alloc(vm, length, size, __alignof(stackval),
                                 MANGO_ALLOC_ZERO_MEMORY);
    if (!arr) {
      RETURN(MANGO_E_OUT_OF_MEMORY);
    }
    sp--;
    sp[0].ref = void_as_ref(vm, arr);
    ip += 3;
    NEXT;
  } while (false);

SLICE1: // start array length ... -> array length ...
  do {
    uint32_t start = sp[0].u32;
    if (start > sp[2].u32) {
      RETURN(MANGO_E_ARGUMENT);
    }
    if (void_is_null(sp[1].ref)) {
      RETURN(MANGO_E_NULL_REFERENCE);
    }
    sp++;
    sp[0].ref.address += start;
    sp[1].u32 -= start;
    ip++;
    NEXT;
  } while (false);

SLICE2: // length' start array length ... -> array length' ...
  do {
    uint32_t length = sp[0].u32;
    uint32_t start = sp[1].u32;
    if (start > sp[3].u32 || length > sp[3].u32 - start) {
      RETURN(MANGO_E_ARGUMENT);
    }
    if (void_is_null(sp[2].ref)) {
      RETURN(MANGO_E_NULL_REFERENCE);
    }
    sp += 2;
    sp[0].ref.address += start;
    sp[1].u32 = length;
    ip++;
    NEXT;
  } while (false);

UNUSED100:
UNUSED101:
UNUSED102:
UNUSED103:
  INVALID;

#define LOAD_FIELD(cast, ty)                                                   \
  do {                                                                         \
    if (void_is_null(sp[0].ref)) {                                             \
      RETURN(MANGO_E_NULL_REFERENCE);                                          \
    }                                                                          \
    const void *obj = void_as_ptr(vm, sp[0].ref);                              \
    const cast *fld = (const cast *)((uintptr_t)obj + FETCH(1, u16));          \
    sp[0].ty = fld[0];                                                         \
    ip += 3;                                                                   \
    NEXT;                                                                      \
  } while (false)

LDFLD_I8: // address ... -> value ...
  LOAD_FIELD(int8_t, i32);

LDFLD_U8: // address ... -> value ...
  LOAD_FIELD(uint8_t, u32);

LDFLD_I16: // address ... -> value ...
  LOAD_FIELD(int16_t, i32);

LDFLD_U16: // address ... -> value ...
  LOAD_FIELD(uint16_t, u32);

LDFLD_X32: // address ... -> value ...
  LOAD_FIELD(uint32_t, u32);

LDFLD_X64: // address ... -> value ...
  do {
    if (void_is_null(sp[0].ref)) {
      RETURN(MANGO_E_NULL_REFERENCE);
    }
    const void *obj = void_as_ptr(vm, sp[0].ref);
    const uint32_t *fld = (const uint32_t *)((uintptr_t)obj + FETCH(1, u16));
    sp--;
    sp[0].u32 = fld[0];
    sp[1].u32 = fld[1];
    ip += 3;
    NEXT;
  } while (false);

LDFLDA: // address ... -> address ...
  do {
    if (void_is_null(sp[0].ref)) {
      RETURN(MANGO_E_NULL_REFERENCE);
    }
    sp[0].ref.address += FETCH(1, u16);
    ip += 3;
    NEXT;
  } while (false);

#define STORE_FIELD(cast, ty)                                                  \
  do {                                                                         \
    if (void_is_null(sp[1].ref)) {                                             \
      RETURN(MANGO_E_NULL_REFERENCE);                                          \
    }                                                                          \
    void *obj = void_as_ptr(vm, sp[1].ref);                                    \
    cast *fld = (cast *)((uintptr_t)obj + FETCH(1, u16));                      \
    fld[0] = (cast)sp[0].ty;                                                   \
    sp += 2;                                                                   \
    ip += 3;                                                                   \
    NEXT;                                                                      \
  } while (false)

STFLD_X8: // value address ... -> ...
  STORE_FIELD(uint8_t, u32);

STFLD_X16: // value address ... -> ...
  STORE_FIELD(uint16_t, u32);

STFLD_X32: // value address ... -> ...
  STORE_FIELD(uint32_t, u32);

STFLD_X64: // value address -> ...
  do {
    if (void_is_null(sp[2].ref)) {
      RETURN(MANGO_E_NULL_REFERENCE);
    }
    void *obj = void_as_ptr(vm, sp[2].ref);
    uint32_t *fld = (uint32_t *)((uintptr_t)obj + FETCH(1, u16));
    fld[0] = sp[0].u32;
    fld[1] = sp[1].u32;
    sp += 3;
    ip += 3;
    NEXT;
  } while (false);

UNUSED115:
UNUSED116:
UNUSED117:
UNUSED118:
UNUSED119:
UNUSED120:
UNUSED121:
UNUSED122:
UNUSED123:
UNUSED124:
UNUSED125:
  INVALID;

#define LOAD_ELEMENT(cast, ty)                                                 \
  do {                                                                         \
    uint32_t index = sp[0].u32;                                                \
    if (index >= sp[2].u32) {                                                  \
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);                                      \
    }                                                                          \
    if (void_is_null(sp[1].ref)) {                                             \
      RETURN(MANGO_E_NULL_REFERENCE);                                          \
    }                                                                          \
    const void *arr = void_as_ptr(vm, sp[1].ref);                              \
    sp += 2;                                                                   \
    sp[0].ty = ((const cast *)arr)[index];                                     \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false)

LDELEM_I8: // index array length ... -> value ...
  LOAD_ELEMENT(int8_t, i32);

LDELEM_U8: // index array length ... -> value ...
  LOAD_ELEMENT(uint8_t, u32);

LDELEM_I16: // index array length ... -> value ...
  LOAD_ELEMENT(int16_t, i32);

LDELEM_U16: // index array length ... -> value ...
  LOAD_ELEMENT(uint16_t, u32);

LDELEM_X32: // index array length ... -> value ...
  LOAD_ELEMENT(uint32_t, u32);

LDELEM_X64: // index array length ... -> value ...
  do {
    uint32_t index = sp[0].u32;
    if (index >= sp[2].u32) {
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);
    }
    if (void_is_null(sp[1].ref)) {
      RETURN(MANGO_E_NULL_REFERENCE);
    }
    void *arr = void_as_ptr(vm, sp[1].ref);
    sp += 1;
    sp[0].u32 = ((uint32_t *)arr)[2 * index + 0];
    sp[1].u32 = ((uint32_t *)arr)[2 * index + 1];
    ip++;
    NEXT;
  } while (false);

LDELEMA: // index array length ... -> address ...
  do {
    uint32_t index = sp[0].u32;
    if (index >= sp[2].u32) {
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);
    }
    if (void_is_null(sp[1].ref)) {
      RETURN(MANGO_E_NULL_REFERENCE);
    }
    uint32_t address = sp[1].ref.address;
    uint32_t size = FETCH(1, u16);
    sp += 2;
    sp[0].ref.address = address + index * size;
    ip += 3;
    NEXT;
  } while (false);

#define STORE_ELEMENT(cast)                                                    \
  do {                                                                         \
    uint32_t value = sp[0].u32;                                                \
    uint32_t index = sp[1].u32;                                                \
    if (index >= sp[3].u32) {                                                  \
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);                                      \
    }                                                                          \
    if (void_is_null(sp[2].ref)) {                                             \
      RETURN(MANGO_E_NULL_REFERENCE);                                          \
    }                                                                          \
    void *arr = void_as_ptr(vm, sp[2].ref);                                    \
    ((cast *)arr)[index] = (cast)value;                                        \
    sp += 4;                                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (false)

STELEM_X8: // value index array length ... -> ...
  STORE_ELEMENT(uint8_t);

STELEM_X16: // value index array length ... -> ...
  STORE_ELEMENT(uint16_t);

STELEM_X32: // value index array length ... -> ...
  STORE_ELEMENT(uint32_t);

STELEM_X64: // value index array length ... -> ...
  do {
    uint32_t value1 = sp[0].u32;
    uint32_t value2 = sp[1].u32;
    uint32_t index = sp[2].u32;
    if (index >= sp[4].u32) {
      RETURN(MANGO_E_INDEX_OUT_OF_RANGE);
    }
    if (void_is_null(sp[3].ref)) {
      RETURN(MANGO_E_NULL_REFERENCE);
    }
    void *arr = void_as_ptr(vm, sp[3].ref);
    ((uint32_t *)arr)[2 * index + 0] = value1;
    ((uint32_t *)arr)[2 * index + 1] = value2;
    sp += 5;
    ip++;
    NEXT;
  } while (false);

UNUSED137:
UNUSED138:
UNUSED139:
UNUSED140:
UNUSED141:
UNUSED142:
UNUSED143:
  INVALID;

#pragma endregion

#elif !defined(MANGO_NO_I64) || !defined(MANGO_NO_F32) || !defined(MANGO_NO_F64)

NEWOBJ:
NEWARR:
SLICE1:
SLICE2:
UNUSED100:
UNUSED101:
UNUSED102:
UNUSED103:
LDFLD_I8:
LDFLD_U8:
LDFLD_I16:
LDFLD_U16:
LDFLD_X32:
LDFLD_X64:
LDFLDA:
STFLD_X8:
STFLD_X16:
STFLD_X32:
STFLD_X64:
UNUSED115:
UNUSED116:
UNUSED117:
UNUSED118:
UNUSED119:
UNUSED120:
UNUSED121:
UNUSED122:
UNUSED123:
UNUSED124:
UNUSED125:
LDELEM_I8:
LDELEM_U8:
LDELEM_I16:
LDELEM_U16:
LDELEM_X32:
LDELEM_X64:
LDELEMA:
STELEM_X8:
STELEM_X16:
STELEM_X32:
STELEM_X64:
UNUSED137:
UNUSED138:
UNUSED139:
UNUSED140:
UNUSED141:
UNUSED142:
UNUSED143:
  INVALID;

  (void)void_as_ptr;
  (void)void_as_ref;
  (void)void_is_null;

#else

  (void)void_as_ptr;
  (void)void_as_ref;
  (void)void_is_null;

#endif

  (void)uint8_t_is_null;
  (void)mango_module_is_null;

#if !defined(MANGO_NO_I64)

#pragma region i64 arithmetic

ADD_I64: // value2 value1 ... -> result ...
  BINARY2(u64, +);

SUB_I64: // value2 value1 ... -> result ...
  BINARY2(u64, -);

MUL_I64: // value2 value1 ... -> result ...
  BINARY2(u64, *);

DIV_I64: // value2 value1 ... -> result ...
  BINARY2D(i64, /);

DIV_I64_UN: // value2 value1 ... -> result ...
  BINARY2D(u64, /);

REM_I64: // value2 value1 ... -> result ...
  BINARY2D(i64, %);

REM_I64_UN: // value2 value1 ... -> result ...
  BINARY2D(u64, %);

NEG_I64: // value ... -> result ...
  UNARY2(u64, -);

#pragma endregion

#pragma region i64 bitwise

SHL_I64: // amount value ... -> result ...
  SHIFT2(u64, <<);

SHR_I64: // amount value ... -> result ...
  SHIFT2(i64, >>);

SHR_I64_UN: // amount value ... -> result ...
  SHIFT2(u64, >>);

AND_I64: // value2 value1 ... -> result ...
  BINARY2(u64, &);

OR_I64: // value2 value1 ... -> result ...
  BINARY2(u64, |);

XOR_I64: // value2 value1 ... -> result ...
  BINARY2(u64, ^);

NOT_I64: // value ... -> result ...
  UNARY2(u64, ~);

#pragma endregion

#pragma region i64 comparison

CEQ_I64: // value2 value1 ... -> result ...
  COMPARE2(u64, ==);

CNE_I64: // value2 value1 ... -> result ...
  COMPARE2(u64, !=);

CGT_I64: // value2 value1 ... -> result ...
  COMPARE2(i64, >);

CGT_I64_UN: // value2 value1 ... -> result ...
  COMPARE2(u64, >);

CGE_I64: // value2 value1 ... -> result ...
  COMPARE2(i64, >=);

CGE_I64_UN: // value2 value1 ... -> result ...
  COMPARE2(u64, >=);

CLT_I64: // value2 value1 ... -> result ...
  COMPARE2(i64, <);

CLT_I64_UN: // value2 value1 ... -> result ...
  COMPARE2(u64, <);

CLE_I64: // value2 value1 ... -> result ...
  COMPARE2(i64, <=);

CLE_I64_UN: // value2 value1 ... -> result ...
  COMPARE2(u64, <=);

#pragma endregion

#pragma region i64 conversion

CONV_I8_I64: // value ... -> result ...
  CONVERT12(int8_t, i32, i64);

CONV_U8_I64: // value ... -> result ...
  CONVERT12(uint8_t, u32, u64);

CONV_I16_I64: // value ... -> result ...
  CONVERT12(int16_t, i32, i64);

CONV_U16_I64: // value ... -> result ...
  CONVERT12(uint16_t, u32, u64);

CONV_I32_I64: // value ... -> result ...
  CONVERT12(int32_t, i32, i64);

CONV_U32_I64: // value ... -> result ...
  CONVERT12(uint32_t, u32, u64);

CONV_I64_I32: // value ... -> result ...
  CONVERT21(int64_t, i64, i32);

CONV_U64_I32: // value ... -> result ...
  CONVERT21(uint64_t, u64, u32);

CONV_I64_F32: // value ... -> result ...
#if !defined(MANGO_NO_F32)
  CONVERT21(int64_t, i64, f32);
#else
  INVALID;
#endif

CONV_U64_F32: // value ... -> result ...
#if !defined(MANGO_NO_F32)
  CONVERT21(uint64_t, u64, f32);
#else
  INVALID;
#endif

CONV_I64_F64: // value ... -> result ...
#if !defined(MANGO_NO_F64)
  CONVERT2(int64_t, i64, f64);
#else
  INVALID;
#endif

CONV_U64_F64: // value ... -> result ...
#if !defined(MANGO_NO_F64)
  CONVERT2(uint64_t, u64, f64);
#else
  INVALID;
#endif

UNUSED181:
UNUSED182:
UNUSED183:
  INVALID;

UNUSED184:
UNUSED185:
UNUSED186:
UNUSED187:
UNUSED188:
UNUSED189:
UNUSED190:
UNUSED191:
  INVALID;

#pragma endregion

#elif !defined(MANGO_NO_F32) || !defined(MANGO_NO_F64)

ADD_I64:
SUB_I64:
MUL_I64:
DIV_I64:
DIV_I64_UN:
REM_I64:
REM_I64_UN:
NEG_I64:
SHL_I64:
SHR_I64:
SHR_I64_UN:
AND_I64:
OR_I64:
XOR_I64:
NOT_I64:
CEQ_I64:
CNE_I64:
CGT_I64:
CGT_I64_UN:
CGE_I64:
CGE_I64_UN:
CLT_I64:
CLT_I64_UN:
CLE_I64:
CLE_I64_UN:
CONV_I8_I64:
CONV_U8_I64:
CONV_I16_I64:
CONV_U16_I64:
CONV_I32_I64:
CONV_U32_I64:
CONV_I64_I32:
CONV_U64_I32:
CONV_I64_F32:
CONV_U64_F32:
CONV_I64_F64:
CONV_U64_F64:
UNUSED181:
UNUSED182:
UNUSED183:
UNUSED184:
UNUSED185:
UNUSED186:
UNUSED187:
UNUSED188:
UNUSED189:
UNUSED190:
UNUSED191:
  INVALID;

#endif

#if !defined(MANGO_NO_F32)

#pragma region f32 arithmetic

ADD_F32: // value2 value1 ... -> result ...
  BINARY1(f32, +);

SUB_F32: // value2 value1 ... -> result ...
  BINARY1(f32, -);

MUL_F32: // value2 value1 ... -> result ...
  BINARY1(f32, *);

DIV_F32: // value2 value1 ... -> result ...
  BINARY1(f32, /);

REM_F32: // value2 value1 ... -> result ...
  sp[1].f32 = fmodf(sp[1].f32, sp[0].f32);
  sp++;
  ip++;
  NEXT;

NEG_F32: // value ... -> result ...
  UNARY1(f32, -);

#pragma endregion

#pragma region f32 comparison

CEQ_F32: // value2 value1 ... -> result ...
  COMPARE1(f32, ==);

CEQ_F32_UN: // value2 value1 ... -> result ...
  COMPARE1F(f32, !islessgreater);

CNE_F32: // value2 value1 ... -> result ...
  COMPARE1F(f32, islessgreater);

CNE_F32_UN: // value2 value1 ... -> result ...
  COMPARE1(f32, !=);

CGT_F32: // value2 value1 ... -> result ...
  COMPARE1F(f32, isgreater);

CGT_F32_UN: // value2 value1 ... -> result ...
  COMPARE1F(f32, !islessequal);

CGE_F32: // value2 value1 ... -> result ...
  COMPARE1F(f32, isgreaterequal);

CGE_F32_UN: // value2 value1 ... -> result ...
  COMPARE1F(f32, !isless);

CLT_F32: // value2 value1 ... -> result ...
  COMPARE1F(f32, isless);

CLT_F32_UN: // value2 value1 ... -> result ...
  COMPARE1F(f32, !isgreaterequal);

CLE_F32: // value2 value1 ... -> result ...
  COMPARE1F(f32, islessequal);

CLE_F32_UN: // value2 value1 ... -> result ...
  COMPARE1F(f32, !isgreater);

#pragma endregion

#pragma region f32 conversion

CONV_I8_F32: // value ... -> result ...
  CONVERT1(int8_t, i32, f32);

CONV_U8_F32: // value ... -> result ...
  CONVERT1(uint8_t, u32, f32);

CONV_I16_F32: // value ... -> result ...
  CONVERT1(int16_t, i32, f32);

CONV_U16_F32: // value ... -> result ...
  CONVERT1(uint16_t, u32, f32);

CONV_I32_F32: // value ... -> result ...
  CONVERT1(int32_t, i32, f32);

CONV_U32_F32: // value ... -> result ...
  CONVERT1(uint32_t, u32, f32);

CONV_F32_I32: // value ... -> result ...
  CONVERT1(float, f32, i32);

CONV_F32_I32_UN: // value ... -> result ...
  CONVERT1(float, f32, u32);

CONV_F32_I64: // value ... -> result ...
#if !defined(MANGO_NO_I64)
  CONVERT12(float, f32, i64);
#else
  INVALID;
#endif

CONV_F32_I64_UN: // value ... -> result ...
#if !defined(MANGO_NO_I64)
  CONVERT12(float, f32, u64);
#else
  INVALID;
#endif

CONV_F32_F64: // value ... -> result ...
#if !defined(MANGO_NO_F64)
  CONVERT12(float, f32, f64);
#else
  INVALID;
#endif

UNUSED221:
UNUSED222:
UNUSED223:
  INVALID;

#pragma endregion

#elif !defined(MANGO_NO_F64)

ADD_F32:
SUB_F32:
MUL_F32:
DIV_F32:
REM_F32:
NEG_F32:
CEQ_F32:
CEQ_F32_UN:
CNE_F32:
CNE_F32_UN:
CGT_F32:
CGT_F32_UN:
CGE_F32:
CGE_F32_UN:
CLT_F32:
CLT_F32_UN:
CLE_F32:
CLE_F32_UN:
CONV_I8_F32:
CONV_U8_F32:
CONV_I16_F32:
CONV_U16_F32:
CONV_I32_F32:
CONV_U32_F32:
CONV_F32_I32:
CONV_F32_I32_UN:
CONV_F32_I64:
CONV_F32_I64_UN:
CONV_F32_F64:
UNUSED221:
UNUSED222:
UNUSED223:
  INVALID;

#endif

#if !defined(MANGO_NO_F64)

#pragma region f64 arithmetic

ADD_F64: // value2 value1 ... -> result ...
  BINARY2(f64, +);

SUB_F64: // value2 value1 ... -> result ...
  BINARY2(f64, -);

MUL_F64: // value2 value1 ... -> result ...
  BINARY2(f64, *);

DIV_F64: // value2 value1 ... -> result ...
  BINARY2(f64, /);

REM_F64: // value2 value1 ... -> result ...
  do {
    stackval2 *sp2 = (stackval2 *)sp;
    sp2[1].f64 = fmod(sp2[1].f64, sp2[0].f64);
    sp += 2;
    ip++;
    NEXT;
  } while (false);

NEG_F64: // value ... -> result ...
  UNARY2(f64, -);

#pragma endregion

#pragma region f64 comparison

CEQ_F64: // value2 value1 ... -> result ...
  COMPARE2(f64, ==);

CEQ_F64_UN: // value2 value1 ... -> result ...
  COMPARE2F(f64, !islessgreater);

CNE_F64: // value2 value1 ... -> result ...
  COMPARE2F(f64, islessgreater);

CNE_F64_UN: // value2 value1 ... -> result ...
  COMPARE2(f64, !=);

CGT_F64: // value2 value1 ... -> result ...
  COMPARE2F(f64, isgreater);

CGT_F64_UN: // value2 value1 ... -> result ...
  COMPARE2F(f64, !islessequal);

CGE_F64: // value2 value1 ... -> result ...
  COMPARE2F(f64, isgreaterequal);

CGE_F64_UN: // value2 value1 ... -> result ...
  COMPARE2F(f64, !isless);

CLT_F64: // value2 value1 ... -> result ...
  COMPARE2F(f64, isless);

CLT_F64_UN: // value2 value1 ... -> result ...
  COMPARE2F(f64, !isgreaterequal);

CLE_F64: // value2 value1 ... -> result ...
  COMPARE2F(f64, islessequal);

CLE_F64_UN: // value2 value1 ... -> result ...
  COMPARE2F(f64, !isgreater);

#pragma endregion

#pragma region f64 conversion

CONV_I8_F64: // value ... -> result ...
  CONVERT12(int8_t, i32, f64);

CONV_U8_F64: // value ... -> result ...
  CONVERT12(uint8_t, u32, f64);

CONV_I16_F64: // value ... -> result ...
  CONVERT12(int16_t, i32, f64);

CONV_U16_F64: // value ... -> result ...
  CONVERT12(uint16_t, u32, f64);

CONV_I32_F64: // value ... -> result ...
  CONVERT12(int32_t, i32, f64);

CONV_U32_F64: // value ... -> result ...
  CONVERT12(uint32_t, u32, f64);

CONV_F64_I32: // value ... -> result ...
  CONVERT21(double, f64, i32);

CONV_F64_I32_UN: // value ... -> result ...
  CONVERT21(double, f64, u32);

CONV_F64_I64: // value ... -> result ...
#if !defined(MANGO_NO_I64)
  CONVERT2(double, f64, i64);
#else
  INVALID;
#endif

CONV_F64_I64_UN: // value ... -> result ...
#if !defined(MANGO_NO_I64)
  CONVERT2(double, f64, u64);
#else
  INVALID;
#endif

CONV_F64_F32: // value ... -> result ...
#if !defined(MANGO_NO_F32)
  CONVERT21(double, f64, f32);
#else
  INVALID;
#endif

UNUSED253:
UNUSED254:
UNUSED255:
  INVALID;

#pragma endregion

#endif

invalid:
  printf("<< INVALID PROGRAM >>\n");
  result = MANGO_E_INVALID_PROGRAM;

done:
  vm->sp_expected = (uint16_t)(sp - stackval_as_ptr(vm, vm->stack));
  vm->syscall = 0;

yield:
  do {
    const mango_module *modules = mango_module_as_ptr(vm, vm->modules);
    const mango_module *mp = &modules[sf.module];
    sf.ip = (uint16_t)(ip - mp->image);
    vm->sf = sf;
    vm->sp = (uint16_t)(sp - stackval_as_ptr(vm, vm->stack));
    vm->rp = (uint16_t)(rp - stackval_as_ptr(vm, vm->stack));
    return result;
  } while (false);
}

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

////////////////////////////////////////////////////////////////////////////////
