/*
 *  _____ _____ _____ _____ _____
 * |     |  _  |   | |   __|     |
 * | | | |     | | | |  |  |  |  |
 * |_|_|_|__|__|_|___|_____|_____|
 *
 * Mango Virtual Machine 1.0-dev
 *
 * Copyright (c) 2018 Klaus Hartke
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
#include <string.h>

////////////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wunused-parameter"

MANGO_DECLARE_REF_TYPE(void)
MANGO_DECLARE_REF_TYPE(uint8_t)
MANGO_DECLARE_REF_TYPE(mango_module)

#pragma pack(push, 4)

typedef enum opcode {
#define OPCODE(c, s, pop, push, args, i) c,
#include "mango_opcodes.inc"
#undef OPCODE
} opcode;

typedef struct stack_frame {
  uint8_t pop;
  uint8_t module;
  uint16_t ip;
} stack_frame;

typedef struct function_token {
  uint8_t _reserved;
  uint8_t module;
  uint16_t offset;
} function_token;

typedef union stackval {
  stack_frame sf;
  function_token ftn;
  int32_t i32;
  uint32_t u32;
#if !defined(MANGO_NO_F32)
  float f32;
#endif
#if !defined(MANGO_NO_REFS)
  void_ref ref;
#endif
} stackval;

typedef union stackval2 {
  struct {
    void_ref address;
    uint32_t length;
  } slice;
#if !defined(MANGO_NO_I64)
  int64_t i64;
  uint64_t u64;
#endif
#if !defined(MANGO_NO_F64)
  double f64;
#endif
} stackval2;

typedef struct mango_vm {
  uint8_t version;

  uint8_t result;
  uint16_t syscall;

  uint32_t heap_size;
  uint32_t heap_used;

  mango_module_name startup_module_name;
  mango_module_ref modules;
  uint8_t modules_created;
  uint8_t modules_imported;

  uint8_t init_head;
  uint8_t init_flags;

  uint16_t stack_size;
  uint16_t rp;
  uint16_t sp;
  uint16_t sp_expected;
  stack_frame sf;

  void_ref base;

  uint32_t _reserved[2];

  union {
    void *context;
    uint8_t _context[8];
  };

  stackval stack[];
} mango_vm;

typedef struct mango_module {
  union {
    const uint8_t *image;
    uint8_t _image[8];
  };

  uint16_t image_size;

  uint8_t name_module;
  uint8_t name_index;

  uint8_t init_next;
  uint8_t init_prev;
  uint8_t init_flags;

  uint8_t import_count;
  uint8_t_ref imports;

  uint32_t _reserved[1];

  union {
    void *context;
    uint8_t _context[8];
  };
} mango_module;

#pragma pack(pop)

MANGO_DEFINE_REF_TYPE(void, )
MANGO_DEFINE_REF_TYPE(uint8_t, const)
MANGO_DEFINE_REF_TYPE(mango_module, )

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

#pragma pack(push, 1)

typedef struct packed_i8 {
  int8_t i8;
} packed_i8;
typedef struct packed_u8 {
  uint8_t u8;
} packed_u8;
typedef struct packed_i16 {
  int16_t i16;
} packed_i16;
typedef struct packed_u16 {
  uint16_t u16;
} packed_u16;
typedef struct packed_u32 {
  uint32_t u32;
} packed_u32;

#define FETCH(Address, Type) (((const packed_##Type *)(Address))->Type)

#pragma pack(pop)

#if !defined(__EDG__)
_Static_assert(sizeof(stack_frame) == 4, "Incorrect layout");
_Static_assert(__alignof(stack_frame) == 2, "Incorrect layout");
_Static_assert(sizeof(function_token) == 4, "Incorrect layout");
_Static_assert(__alignof(function_token) == 2, "Incorrect layout");
_Static_assert(sizeof(stackval) == 4, "Incorrect layout");
_Static_assert(__alignof(stackval) == 4, "Incorrect layout");
_Static_assert(sizeof(stackval2) == 8, "Incorrect layout");
_Static_assert(__alignof(stackval2) == 4, "Incorrect layout");
_Static_assert(sizeof(mango_vm) == 64, "Incorrect layout");
_Static_assert(__alignof(mango_vm) == 4, "Incorrect layout");
_Static_assert(sizeof(mango_module) == 32, "Incorrect layout");
_Static_assert(__alignof(mango_module) == 4, "Incorrect layout");
_Static_assert(sizeof(packed_i8) == 1, "Incorrect layout");
_Static_assert(__alignof(packed_i8) == 1, "Incorrect layout");
_Static_assert(sizeof(packed_u8) == 1, "Incorrect layout");
_Static_assert(__alignof(packed_u8) == 1, "Incorrect layout");
_Static_assert(sizeof(packed_i16) == 2, "Incorrect layout");
_Static_assert(__alignof(packed_i16) == 1, "Incorrect layout");
_Static_assert(sizeof(packed_u16) == 2, "Incorrect layout");
_Static_assert(__alignof(packed_u16) == 1, "Incorrect layout");
_Static_assert(sizeof(packed_u32) == 4, "Incorrect layout");
_Static_assert(__alignof(packed_u32) == 1, "Incorrect layout");
#endif

////////////////////////////////////////////////////////////////////////////////

int mango_version_major(void) { return MANGO_VERSION_MAJOR; }

int mango_version_minor(void) { return MANGO_VERSION_MINOR; }

const char *mango_version_string(void) { return MANGO_VERSION_STRING; }

int mango_features(void) {
  int features = 0;
#if !defined(MANGO_NO_I64)
  features |= MANGO_FEATURE_I64;
#endif
#if !defined(MANGO_NO_F32)
  features |= MANGO_FEATURE_F32;
#endif
#if !defined(MANGO_NO_F64)
  features |= MANGO_FEATURE_F64;
#endif
#if !defined(MANGO_NO_REFS)
  features |= MANGO_FEATURE_REFS;
#endif
  return features;
}

////////////////////////////////////////////////////////////////////////////////

mango_vm *mango_initialize(void *address, size_t heap_size, size_t stack_size,
                           void *context) {
  if (!address || ((uintptr_t)address & (__alignof(mango_vm) - 1)) != 0) {
    return NULL;
  }
  if (stack_size > UINT16_MAX * sizeof(stackval)) {
    return NULL;
  }
  if ((stack_size & (sizeof(stackval) - 1)) != 0) {
    return NULL;
  }
  if (heap_size < stack_size || heap_size - stack_size < sizeof(mango_vm)) {
    return NULL;
  }
#if SIZE_MAX > UINT32_MAX
  if (heap_size > UINT32_MAX) {
    return NULL;
  }
#endif

  mango_vm *vm = (mango_vm *)address;
  memset(vm, 0, sizeof(mango_vm));
  vm->version = MANGO_VERSION_MAJOR;
  vm->heap_size = (uint32_t)heap_size;
  vm->heap_used = (uint32_t)(sizeof(mango_vm) + stack_size);
  vm->stack_size = (uint16_t)(stack_size / sizeof(stackval));
  vm->sp_expected = vm->sp = vm->stack_size;
  vm->sf = (stack_frame){0, 0, (uint16_t)(sizeof(mango_module_def) - 1)};
  vm->base = void_as_ref(vm, vm);
  vm->context = context;
  return vm;
}

mango_result mango_error(mango_vm *vm, mango_result error) {
  if (!vm) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (!(error > MANGO_E_SUCCESS && error < MANGO_E_BREAKPOINT)) {
    return MANGO_E_ARGUMENT;
  }
  if (vm->result > MANGO_E_SUCCESS && vm->result < MANGO_E_BREAKPOINT) {
    return MANGO_E_INVALID_OPERATION;
  }

  vm->result = (uint8_t)error;
  return MANGO_E_SUCCESS;
}

void *mango_context(const mango_vm *vm) { return vm ? vm->context : NULL; }

////////////////////////////////////////////////////////////////////////////////

void *mango_heap_alloc(mango_vm *vm, size_t count, size_t size,
                       size_t alignment, int flags) {
  size_t total_size;
  size_t offset;
  size_t available;

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

  vm->heap_used = (uint32_t)(offset + total_size);
  void *block = (void *)((uintptr_t)vm + offset);

  if ((flags & MANGO_ALLOC_ZERO_MEMORY) != 0) {
    memset(block, 0, total_size);
  }

  return block;
}

size_t mango_heap_size(const mango_vm *vm) {
  return vm ? (size_t)vm->heap_size : 0;
}

size_t mango_heap_available(const mango_vm *vm) {
  return vm ? (size_t)vm->heap_size - (size_t)vm->heap_used : 0;
}

////////////////////////////////////////////////////////////////////////////////

void *mango_stack_alloc(mango_vm *vm, size_t size, int flags) {
  if (!vm) {
    return NULL;
  }

  size_t count = (size + (sizeof(stackval) - 1)) / sizeof(stackval);

  if ((size_t)vm->sp - (size_t)vm->rp < count) {
    return NULL;
  }

  vm->sp -= count;

  stackval *block = vm->stack + vm->sp;

  if ((flags & MANGO_ALLOC_ZERO_MEMORY) != 0) {
    memset(block, 0, size);
  }

  return block;
}

mango_result mango_stack_free(mango_vm *vm, size_t size) {
  if (!vm) {
    return MANGO_E_ARGUMENT_NULL;
  }

  size_t count = (size + (sizeof(stackval) - 1)) / sizeof(stackval);

  if ((size_t)vm->stack_size - (size_t)vm->sp < count) {
    return MANGO_E_INVALID_OPERATION;
  }

  vm->sp += count;
  return MANGO_E_SUCCESS;
}

void *mango_stack_top(mango_vm *vm) {
  if (!vm || vm->sp == vm->stack_size) {
    return NULL;
  }

  return vm->stack + vm->sp;
}

size_t mango_stack_size(const mango_vm *vm) {
  return vm ? (size_t)vm->stack_size * sizeof(stackval) : 0;
}

size_t mango_stack_available(const mango_vm *vm) {
  return vm ? ((size_t)vm->sp - (size_t)vm->rp) * sizeof(stackval) : 0;
}

////////////////////////////////////////////////////////////////////////////////

#define INVALID_MODULE 255

static inline mango_module *_mango_get_modules(const mango_vm *vm) {
  return mango_module_as_ptr(vm, vm->modules);
}

static inline mango_module *_mango_get_module(const mango_vm *vm,
                                              uint8_t index) {
  return mango_module_as_ptr(vm, vm->modules) + index;
}

static inline const uint8_t *
_mango_get_module_imports(const mango_vm *vm, const mango_module *module) {
  return uint8_t_as_ptr(vm, module->imports);
}

static inline const mango_module_name *
_mango_get_module_name(const mango_vm *vm, const mango_module *module) {
  if (module->name_module == INVALID_MODULE) {
    return &vm->startup_module_name;
  } else {
    const uint8_t *image = _mango_get_module(vm, module->name_module)->image;
    return ((const mango_module_def *)image)->imports + module->name_index;
  }
}

static uint8_t _mango_get_or_create_module(mango_vm *vm,
                                           const mango_module_name *name,
                                           uint8_t name_module,
                                           uint8_t name_index) {
  mango_module *modules = _mango_get_modules(vm);

  for (uint_fast8_t i = 0; i < vm->modules_created; i++) {
    const mango_module_name *n = _mango_get_module_name(vm, &modules[i]);
    if (memcmp(name, n, sizeof(mango_module_name)) == 0) {
      return i;
    }
  }

  uint8_t index = vm->modules_created++;
  modules[index].name_module = name_module;
  modules[index].name_index = name_index;
  return index;
}

static mango_result _mango_initialize_module(mango_vm *vm, uint8_t index,
                                             mango_module *module) {
  const mango_module_def *m = (const mango_module_def *)(module->image);

  if (m->import_count != 0) {
    uint8_t *imports = (uint8_t *)mango_heap_alloc(
        vm, m->import_count, sizeof(uint8_t), __alignof(uint8_t), 0);

    if (!imports) {
      return MANGO_E_OUT_OF_MEMORY;
    }

    for (uint_fast8_t i = 0; i < m->import_count; i++) {
      imports[i] = _mango_get_or_create_module(vm, &m->imports[i], index, i);
    }

    module->import_count = m->import_count;
    module->imports = uint8_t_as_ref(vm, imports);
  } else {
    module->import_count = 0;
    module->imports = uint8_t_null();
  }

  module->_reserved[0] = 0;

  return MANGO_E_SUCCESS;
}

static mango_result _mango_import_startup_module(mango_vm *vm,
                                                 const uint8_t *name,
                                                 const uint8_t *image,
                                                 size_t size, void *context) {
  const mango_module_def *m = (const mango_module_def *)image;

  mango_module *modules = (mango_module *)mango_heap_alloc(
      vm, m->module_count, sizeof(mango_module), __alignof(mango_module), 0);

  if (!modules) {
    return MANGO_E_OUT_OF_MEMORY;
  }

  memcpy(&vm->startup_module_name, name, sizeof(mango_module_name));
  vm->modules = mango_module_as_ref(vm, modules);
  vm->modules_created = 1;
  vm->modules_imported = 1;

  mango_module *module = &modules[0];
  module->image = image;
  module->image_size = (uint16_t)size;
  module->name_module = INVALID_MODULE;
  module->name_index = INVALID_MODULE;
  module->init_next = INVALID_MODULE;
  module->init_prev = INVALID_MODULE;
  module->init_flags = 0;
  module->context = context;

  return _mango_initialize_module(vm, 0, module);
}

static mango_result _mango_import_missing_module(mango_vm *vm,
                                                 const uint8_t *name,
                                                 const uint8_t *image,
                                                 size_t size, void *context) {
  uint8_t index = vm->modules_imported;
  mango_module *module = _mango_get_module(vm, index);
  const mango_module_name *n = _mango_get_module_name(vm, module);

  if (memcmp(name, n, sizeof(mango_module_name)) != 0) {
    return MANGO_E_INVALID_OPERATION;
  }

  vm->modules_imported++;

  module->image = image;
  module->image_size = (uint16_t)size;
  module->init_next = INVALID_MODULE;
  module->init_prev = INVALID_MODULE;
  module->init_flags = 0;
  module->context = context;

  return _mango_initialize_module(vm, index, module);
}

mango_result mango_module_import(mango_vm *vm, const uint8_t *name,
                                 const uint8_t *image, size_t size,
                                 void *context) {
  if (!vm || !name || !image) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (size < sizeof(mango_module_def) || size > UINT16_MAX) {
    return MANGO_E_ARGUMENT;
  }

  const mango_module_def *m = (const mango_module_def *)image;

  if (m->version != MANGO_VERSION_MAJOR ||
      m->entry_point[sizeof(m->entry_point) - 1] != HALT ||
      m->module_count == 0 || m->import_count > m->module_count) {
    return MANGO_E_BAD_IMAGE_FORMAT;
  }
  if ((m->features & mango_features()) != m->features) {
    return MANGO_E_NOT_SUPPORTED;
  }

  if (vm->modules_imported == 0) {
    return _mango_import_startup_module(vm, name, image, size, context);
  } else if (vm->modules_imported < vm->modules_created) {
    return _mango_import_missing_module(vm, name, image, size, context);
  } else {
    return MANGO_E_INVALID_OPERATION;
  }
}

const uint8_t *mango_module_missing(const mango_vm *vm) {
  if (!vm || vm->modules_imported >= vm->modules_created) {
    return NULL;
  }

  mango_module *module = _mango_get_module(vm, vm->modules_imported);
  return (const uint8_t *)_mango_get_module_name(vm, module);
}

void *mango_module_context(const mango_vm *vm) {
  if (!vm || vm->sf.module >= vm->modules_imported) {
    return NULL;
  }

  return _mango_get_module(vm, vm->sf.module)->context;
}

////////////////////////////////////////////////////////////////////////////////

#define VISITED 1

static mango_result _mango_interpret(mango_vm *vm);

mango_result mango_run(mango_vm *vm) {
  if (!vm) {
    return MANGO_E_ARGUMENT_NULL;
  }
  if (vm->modules_imported == 0 ||
      vm->modules_imported != vm->modules_created) {
    return MANGO_E_INVALID_OPERATION;
  }
  if (vm->result > MANGO_E_SUCCESS && vm->result < MANGO_E_BREAKPOINT) {
    return vm->result;
  }
  if (vm->sp != vm->sp_expected) {
    return vm->result = MANGO_E_STACK_IMBALANCE;
  }

  mango_result result = _mango_interpret(vm);
  if (result != MANGO_E_SUCCESS) {
    return vm->result = (uint8_t)result;
  }

  mango_module *modules = _mango_get_modules(vm);
  uint8_t head = vm->init_head;

  while (head != INVALID_MODULE) {
    mango_module *module = &modules[head];

    if ((module->init_flags & VISITED) == 0) {
      module->init_flags |= VISITED;

      const uint8_t *imports = _mango_get_module_imports(vm, module);

      for (uint_fast8_t i = 0; i < module->import_count; i++) {
        uint8_t p = imports[i];
        mango_module *import = &modules[p];

        if ((import->init_flags & VISITED) == 0) {
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
      vm->sf = (stack_frame){0, head,
                             (uint16_t)offsetof(mango_module_def, entry_point)};

      head = module->init_next;
      vm->init_head = head;
      module->init_next = INVALID_MODULE;
      module->init_prev = INVALID_MODULE;
      if (head != INVALID_MODULE) {
        modules[head].init_prev = INVALID_MODULE;
      }

      result = _mango_interpret(vm);
      if (result != MANGO_E_SUCCESS) {
        return vm->result = (uint8_t)result;
      }
    }
  }

  return MANGO_E_SUCCESS;
}

int mango_syscall(const mango_vm *vm) { return vm ? vm->syscall : 0; }

////////////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-label-as-value"
#pragma clang diagnostic ignored "-Wunused-macros"
#pragma clang diagnostic ignored "-Wfloat-equal"

#pragma region macros

#if !defined(__EDG__)
#define NEXT goto *dispatch_table[*ip]
#else
#define NEXT goto invalid
#endif

#define INVALID goto invalid

#define YIELD(Result)                                                          \
  result = (Result);                                                           \
  goto yield

#define RETURN(Result)                                                         \
  result = (Result);                                                           \
  goto done

#define RETURN_IF(Result, Condition)                                           \
  result = (Result);                                                           \
  if (Condition)                                                               \
  goto done

#define BINARY1(Type, Operator)                                                \
  do {                                                                         \
    sp[1].Type = sp[1].Type Operator sp[0].Type;                               \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define BINARY1I(Operator)                                                     \
  do {                                                                         \
    RETURN_IF(MANGO_E_DIVIDE_BY_ZERO, sp[0].i32 == 0);                         \
    RETURN_IF(MANGO_E_ARITHMETIC, sp[0].i32 == -1 && sp[1].i32 == INT32_MIN);  \
    sp[1].i32 = sp[1].i32 Operator sp[0].i32;                                  \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define BINARY1U(Operator)                                                     \
  do {                                                                         \
    RETURN_IF(MANGO_E_DIVIDE_BY_ZERO, sp[0].u32 == 0);                         \
    sp[1].u32 = sp[1].u32 Operator sp[0].u32;                                  \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define BINARY2(Type, Operator)                                                \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[1].Type = sp2[1].Type Operator sp2[0].Type;                            \
    sp += 2;                                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define BINARY2I(Operator)                                                     \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    RETURN_IF(MANGO_E_DIVIDE_BY_ZERO, sp2[0].i64 == 0);                        \
    RETURN_IF(MANGO_E_ARITHMETIC,                                              \
              sp2[0].i64 == -1 && sp2[1].i64 == INT64_MIN);                    \
    sp2[1].i64 = sp2[1].i64 Operator sp2[0].i64;                               \
    sp += 2;                                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define BINARY2U(Operator)                                                     \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    RETURN_IF(MANGO_E_DIVIDE_BY_ZERO, sp2[0].u64 == 0);                        \
    sp2[1].u64 = sp2[1].u64 Operator sp2[0].u64;                               \
    sp += 2;                                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define UNARY1(Type, Operator)                                                 \
  do {                                                                         \
    sp[0].Type = Operator(sp[0].Type);                                         \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define UNARY2(Type, Operator)                                                 \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[0].Type = Operator(sp2[0].Type);                                       \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define SHIFT1(Type, Operator)                                                 \
  do {                                                                         \
    sp[1].Type = sp[1].Type Operator(sp[0].i32 & 31);                          \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define SHIFT2(Type, Operator)                                                 \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)(sp + 1);                                    \
    sp2[0].Type = sp2[0].Type Operator(sp[0].i32 & 63);                        \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define CONVERT1(Cast, Destination, Source)                                    \
  do {                                                                         \
    sp[0].Destination = (Cast)sp[0].Source;                                    \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define CONVERT21(Cast, Destination, Source)                                   \
  do {                                                                         \
    Cast tmp = (Cast)sp[0].Source;                                             \
    sp--;                                                                      \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[0].Destination = tmp;                                                  \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define CONVERT12(Cast, Destination, Source)                                   \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    Cast tmp = (Cast)sp2[0].Source;                                            \
    sp++;                                                                      \
    sp[0].Destination = tmp;                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define CONVERT2(Cast, Destination, Source)                                    \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    sp2[0].Destination = (Cast)sp2[0].Source;                                  \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define COMPARE1(Type, Operator)                                               \
  do {                                                                         \
    sp[1].i32 = sp[1].Type Operator sp[0].Type;                                \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define COMPARE1F(Type, Operator)                                              \
  do {                                                                         \
    sp[1].i32 = Operator(sp[1].Type, sp[0].Type);                              \
    sp++;                                                                      \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define COMPARE2(Type, Operator)                                               \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    int tmp = sp2[1].Type Operator sp2[0].Type;                                \
    sp += 3;                                                                   \
    sp[0].i32 = tmp;                                                           \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#define COMPARE2F(Type, Operator)                                              \
  do {                                                                         \
    stackval2 *sp2 = (stackval2 *)sp;                                          \
    int tmp = Operator(sp2[1].Type, sp2[0].Type);                              \
    sp += 3;                                                                   \
    sp[0].i32 = tmp;                                                           \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

#pragma endregion

static mango_result _mango_interpret(mango_vm *vm) {
  static const void *const dispatch_table[] = {
#define OPCODE(c, s, pop, push, args, i) &&c,
#include "mango_opcodes.inc"
#undef OPCODE
  };

  mango_result result;
  stackval *rp = vm->stack + vm->rp;
  stackval *sp = vm->stack + vm->sp;
  stack_frame sf = vm->sf;
  const uint8_t *ip = _mango_get_module(vm, sf.module)->image + sf.ip;

  NEXT;

#pragma region basic

HALT: // ... -> ...
  RETURN_IF(MANGO_E_APPLICATION, rp != vm->stack);
  RETURN_IF(MANGO_E_STACK_IMBALANCE, sp != vm->stack + vm->stack_size);
  RETURN(MANGO_E_SUCCESS);

NOP: // ... -> ...
  ++ip;
  NEXT;

BREAK: // ... -> ...
  ++ip;
  RETURN(MANGO_E_BREAKPOINT);

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
  } while (0);

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
  } while (0);

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

#pragma endregion

#pragma region locals

#define LOAD_LOCAL(Cast, Type)                                                 \
  do {                                                                         \
    uint8_t slot = FETCH(ip + 1, u8);                                          \
    Cast value = (Cast)sp[slot].Type;                                          \
    sp--;                                                                      \
    sp[0].Type = value;                                                        \
    ip += 2;                                                                   \
    NEXT;                                                                      \
  } while (0)

LDLOC_I8: // ... -> value ...
  LOAD_LOCAL(int8_t, i32);

LDLOC_U8: // ... -> value ...
  LOAD_LOCAL(uint8_t, u32);

LDLOC_I16: // ... -> value ...
  LOAD_LOCAL(int16_t, i32);

LDLOC_U16: // ... -> value ...
  LOAD_LOCAL(uint16_t, u32);

LDLOC_X32: // ... -> value ...
  LOAD_LOCAL(uint32_t, u32);

LDLOC_X64: // ... -> value ...
  do {
    uint8_t slot = FETCH(ip + 1, u8);
    uint32_t value1 = sp[slot + 0].u32;
    uint32_t value2 = sp[slot + 1].u32;
    sp -= 2;
    sp[0].u32 = value1;
    sp[1].u32 = value2;
    ip += 2;
    NEXT;
  } while (0);

LDLOCA: // ... -> address ...
#if !defined(MANGO_NO_REFS)
  do {
    uint8_t slot = FETCH(ip + 1, u8);
    void *object = &sp[slot];
    sp--;
    sp[0].ref = void_as_ref(vm, object);
    ip += 2;
    NEXT;
  } while (0);
#else
  INVALID;
#endif

STLOC_X32: // value ... -> ...
  do {
    uint8_t slot = FETCH(ip + 1, u8);
    sp[slot].u32 = sp[0].u32;
    sp++;
    ip += 2;
    NEXT;
  } while (0);

STLOC_X64: // value ... -> ...
  do {
    uint8_t slot = FETCH(ip + 1, u8);
    sp[slot + 0].u32 = sp[0].u32;
    sp[slot + 1].u32 = sp[1].u32;
    sp += 2;
    ip += 2;
    NEXT;
  } while (0);

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
  sf = rp->sf;
  ip = _mango_get_module(vm, sf.module)->image + sf.ip;
  NEXT;

CALLI: // ftn argumentN ... argument1 argument0 ... -> result ...
  do {
    uint8_t module = sp[0].ftn.module;
    uint16_t offset = sp[0].ftn.offset;

    const mango_module *modules = _mango_get_modules(vm);
    const mango_module *caller = modules + sf.module;
    const mango_module *callee = modules + module;
    const mango_func_def *f = (const mango_func_def *)(callee->image + offset);

    RETURN_IF(MANGO_E_STACK_OVERFLOW,
              sp - rp < 1 + f->loc_count + f->max_stack);
    sp++;
    ip++;

    if (!(sf.pop == 0 && *ip == RET)) {
      rp->sf = (stack_frame){sf.pop, sf.module, (uint16_t)(ip - caller->image)};
      rp++;
    }

    sf = (stack_frame){(uint8_t)(f->arg_count + f->loc_count), module, 0};
    sp -= f->loc_count;
    ip = f->code;

    if (f->loc_count != 0) {
      for (uint_fast8_t i = 0, n = f->loc_count; i < n; i++) {
        sp[i].u32 = 0;
      }
    }

    NEXT;
  } while (0);

CALL_S: // argumentN ... argument1 argument0 ... -> result ...
  do {
    uint16_t offset = FETCH(ip + 1, u16);

    const mango_module *modules = _mango_get_modules(vm);
    const mango_module *caller = modules + sf.module;
    const mango_module *callee = modules + sf.module;
    const mango_func_def *f = (const mango_func_def *)(callee->image + offset);

    RETURN_IF(MANGO_E_STACK_OVERFLOW,
              sp - rp < 1 + f->loc_count + f->max_stack);
    ip += 3;

    if (!(sf.pop == 0 && *ip == RET)) {
      rp->sf = (stack_frame){sf.pop, sf.module, (uint16_t)(ip - caller->image)};
      rp++;
    }

    sf = (stack_frame){(uint8_t)(f->arg_count + f->loc_count), sf.module, 0};
    sp -= f->loc_count;
    ip = f->code;

    if (f->loc_count != 0) {
      for (uint_fast8_t i = 0, n = f->loc_count; i < n; i++) {
        sp[i].u32 = 0;
      }
    }

    NEXT;
  } while (0);

CALL: // argumentN ... argument1 argument0 ... -> result ...
  do {
    uint8_t import = FETCH(ip + 1, u8);
    uint16_t offset = FETCH(ip + 2, u16);

    const mango_module *modules = _mango_get_modules(vm);
    const mango_module *caller = modules + sf.module;
    uint8_t module = import == INVALID_MODULE
                         ? sf.module
                         : _mango_get_module_imports(vm, caller)[import];
    const mango_module *callee = modules + module;
    const mango_func_def *f = (const mango_func_def *)(callee->image + offset);

    RETURN_IF(MANGO_E_STACK_OVERFLOW,
              sp - rp < 1 + f->loc_count + f->max_stack);
    ip += 4;

    if (!(sf.pop == 0 && *ip == RET)) {
      rp->sf = (stack_frame){sf.pop, sf.module, (uint16_t)(ip - caller->image)};
      rp++;
    }

    sf = (stack_frame){(uint8_t)(f->arg_count + f->loc_count), module, 0};
    sp -= f->loc_count;
    ip = f->code;

    if (f->loc_count != 0) {
      for (uint_fast8_t i = 0, n = f->loc_count; i < n; i++) {
        sp[i].u32 = 0;
      }
    }

    NEXT;
  } while (0);

SYSCALL: // argumentN ... argument1 argument0 ... -> result ...
  do {
    int8_t adjustment = FETCH(ip + 1, i8);
    uint16_t syscall = FETCH(ip + 2, u16);

    ip += 4;
    vm->sp_expected = (uint16_t)((sp - vm->stack) + adjustment);
    vm->syscall = syscall;

    YIELD(MANGO_E_SYSTEM_CALL);
  } while (0);

UNUSED31:
  INVALID;

#pragma endregion

#pragma region branches

BR_S: // ... -> ...
  ip += 2 + FETCH(ip + 1, i8);
  NEXT;

BRFALSE_S: // value ... -> ...
  ip += 2 + (sp[0].u32 == 0 ? FETCH(ip + 1, i8) : 0);
  sp++;
  NEXT;

BRTRUE_S: // value ... -> ...
  ip += 2 + (sp[0].u32 != 0 ? FETCH(ip + 1, i8) : 0);
  sp++;
  NEXT;

BR: // ... -> ...
  ip += 3 + FETCH(ip + 1, i16);
  NEXT;

BRFALSE: // value ... -> ...
  ip += 3 + (sp[0].u32 == 0 ? FETCH(ip + 1, i16) : 0);
  sp++;
  NEXT;

BRTRUE: // value ... -> ...
  ip += 3 + (sp[0].u32 != 0 ? FETCH(ip + 1, i16) : 0);
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
  sp[0].i32 = (int)(*ip) - LDC_I32_0;
  ip++;
  NEXT;

LDC_I32_S: // ... -> value ...
  sp--;
  sp[0].i32 = FETCH(ip + 1, i8);
  ip += 2;
  NEXT;

LDC_X32: // ... -> value ...
  sp--;
  sp[0].u32 = FETCH(ip + 1, u32);
  ip += 5;
  NEXT;

LDC_X64: // ... -> value ...
  sp -= 2;
  sp[0].u32 = FETCH(ip + 1, u32);
  sp[1].u32 = FETCH(ip + 5, u32);
  ip += 9;
  NEXT;

LDFTN: // ... -> ftn ...
  do {
    uint8_t import = FETCH(ip + 1, u8);
    uint16_t offset = FETCH(ip + 2, u16);

    uint8_t module = import == INVALID_MODULE
                         ? sf.module
                         : _mango_get_module_imports(
                               vm, _mango_get_module(vm, sf.module))[import];

    sp--;
    sp[0].ftn = (function_token){0, module, offset};
    ip += 4;
    NEXT;
  } while (0);

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
  BINARY1I(/);

DIV_I32_UN: // value2 value1 ... -> result ...
  BINARY1U(/);

REM_I32: // value2 value1 ... -> result ...
  BINARY1I(%);

REM_I32_UN: // value2 value1 ... -> result ...
  BINARY1U(%);

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
    uint32_t size = FETCH(ip + 1, u16);
    void *object = mango_heap_alloc(vm, 1, size, __alignof(stackval),
                                    MANGO_ALLOC_ZERO_MEMORY);
    RETURN_IF(MANGO_E_OUT_OF_MEMORY, !object);
    sp--;
    sp[0].ref = void_as_ref(vm, object);
    ip += 3;
    NEXT;
  } while (0);

NEWARR: // length ... -> array length ...
  do {
    uint32_t length = sp[0].u32;
    RETURN_IF(MANGO_E_ARGUMENT, (int32_t)length < 0);
    uint32_t size = FETCH(ip + 1, u16);
    void *array = mango_heap_alloc(vm, length, size, __alignof(stackval),
                                   MANGO_ALLOC_ZERO_MEMORY);
    RETURN_IF(MANGO_E_OUT_OF_MEMORY, !array);
    sp--;
    sp[0].ref = void_as_ref(vm, array);
    ip += 3;
    NEXT;
  } while (0);

SLICE1: // start array length ... -> array length ...
  do {
    uint32_t start = sp[0].u32;
    RETURN_IF(MANGO_E_ARGUMENT, start > sp[2].u32);
    sp++;
    sp[0].ref.address += start;
    sp[1].u32 -= start;
    ip++;
    NEXT;
  } while (0);

SLICE2: // length' start array length ... -> array length' ...
  do {
    uint32_t length = sp[0].u32;
    uint32_t start = sp[1].u32;
    RETURN_IF(MANGO_E_ARGUMENT,
              start > sp[3].u32 || length > sp[3].u32 - start);
    sp += 2;
    sp[0].ref.address += start;
    sp[1].u32 = length;
    ip++;
    NEXT;
  } while (0);

UNUSED100:
UNUSED101:
UNUSED102:
UNUSED103:
  INVALID;

#define LOAD_FIELD(Cast, Type)                                                 \
  do {                                                                         \
    RETURN_IF(MANGO_E_NULL_REFERENCE, void_is_null(sp[0].ref));                \
    uintptr_t object = (uintptr_t)(void_as_ptr(vm, sp[0].ref));                \
    const Cast *field = (const Cast *)(object + FETCH(ip + 1, u16));           \
    sp[0].Type = field[0];                                                     \
    ip += 3;                                                                   \
    NEXT;                                                                      \
  } while (0)

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
    RETURN_IF(MANGO_E_NULL_REFERENCE, void_is_null(sp[0].ref));
    uintptr_t object = (uintptr_t)(void_as_ptr(vm, sp[0].ref));
    const uint32_t *field = (const uint32_t *)(object + FETCH(ip + 1, u16));
    sp--;
    sp[0].u32 = field[0];
    sp[1].u32 = field[1];
    ip += 3;
    NEXT;
  } while (0);

LDFLDA: // address ... -> address ...
  do {
    RETURN_IF(MANGO_E_NULL_REFERENCE, void_is_null(sp[0].ref));
    sp[0].ref.address += FETCH(ip + 1, u16);
    ip += 3;
    NEXT;
  } while (0);

#define STORE_FIELD(Cast, Type)                                                \
  do {                                                                         \
    RETURN_IF(MANGO_E_NULL_REFERENCE, void_is_null(sp[1].ref));                \
    uintptr_t object = (uintptr_t)(void_as_ptr(vm, sp[1].ref));                \
    Cast *field = (Cast *)(object + FETCH(ip + 1, u16));                       \
    field[0] = (Cast)sp[0].Type;                                               \
    sp += 2;                                                                   \
    ip += 3;                                                                   \
    NEXT;                                                                      \
  } while (0)

STFLD_X8: // value address ... -> ...
  STORE_FIELD(uint8_t, u32);

STFLD_X16: // value address ... -> ...
  STORE_FIELD(uint16_t, u32);

STFLD_X32: // value address ... -> ...
  STORE_FIELD(uint32_t, u32);

STFLD_X64: // value address -> ...
  do {
    RETURN_IF(MANGO_E_NULL_REFERENCE, void_is_null(sp[2].ref));
    uintptr_t object = (uintptr_t)(void_as_ptr(vm, sp[2].ref));
    uint32_t *field = (uint32_t *)(object + FETCH(ip + 1, u16));
    field[0] = sp[0].u32;
    field[1] = sp[1].u32;
    sp += 3;
    ip += 3;
    NEXT;
  } while (0);

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

#define LOAD_ELEMENT(Cast, Type)                                               \
  do {                                                                         \
    uint32_t index = sp[0].u32;                                                \
    RETURN_IF(MANGO_E_INDEX_OUT_OF_RANGE, index >= sp[2].u32);                 \
    const Cast *array = (const Cast *)void_as_ptr(vm, sp[1].ref);              \
    sp += 2;                                                                   \
    sp[0].Type = array[index];                                                 \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

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
    RETURN_IF(MANGO_E_INDEX_OUT_OF_RANGE, index >= sp[2].u32);
    const uint32_t *array = (const uint32_t *)void_as_ptr(vm, sp[1].ref);
    sp += 1;
    sp[0].u32 = array[2 * index + 0];
    sp[1].u32 = array[2 * index + 1];
    ip++;
    NEXT;
  } while (0);

LDELEMA: // index array length ... -> address ...
  do {
    uint32_t index = sp[0].u32;
    RETURN_IF(MANGO_E_INDEX_OUT_OF_RANGE, index >= sp[2].u32);
    uint32_t address = sp[1].ref.address;
    uint32_t size = FETCH(ip + 1, u16);
    sp += 2;
    sp[0].ref.address = address + index * size;
    ip += 3;
    NEXT;
  } while (0);

#define STORE_ELEMENT(Cast)                                                    \
  do {                                                                         \
    uint32_t index = sp[1].u32;                                                \
    RETURN_IF(MANGO_E_INDEX_OUT_OF_RANGE, index >= sp[3].u32);                 \
    Cast *array = (Cast *)void_as_ptr(vm, sp[2].ref);                          \
    array[index] = (Cast)sp[0].u32;                                            \
    sp += 4;                                                                   \
    ip++;                                                                      \
    NEXT;                                                                      \
  } while (0)

STELEM_X8: // value index array length ... -> ...
  STORE_ELEMENT(uint8_t);

STELEM_X16: // value index array length ... -> ...
  STORE_ELEMENT(uint16_t);

STELEM_X32: // value index array length ... -> ...
  STORE_ELEMENT(uint32_t);

STELEM_X64: // value index array length ... -> ...
  do {
    uint32_t index = sp[2].u32;
    RETURN_IF(MANGO_E_INDEX_OUT_OF_RANGE, index >= sp[4].u32);
    uint32_t *array = (uint32_t *)void_as_ptr(vm, sp[3].ref);
    array[2 * index + 0] = sp[0].u32;
    array[2 * index + 1] = sp[1].u32;
    sp += 5;
    ip++;
    NEXT;
  } while (0);

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

#endif

#if !defined(MANGO_NO_I64)

#pragma region i64 arithmetic

ADD_I64: // value2 value1 ... -> result ...
  BINARY2(u64, +);

SUB_I64: // value2 value1 ... -> result ...
  BINARY2(u64, -);

MUL_I64: // value2 value1 ... -> result ...
  BINARY2(u64, *);

DIV_I64: // value2 value1 ... -> result ...
  BINARY2I(/);

DIV_I64_UN: // value2 value1 ... -> result ...
  BINARY2U(/);

REM_I64: // value2 value1 ... -> result ...
  BINARY2I(%);

REM_I64_UN: // value2 value1 ... -> result ...
  BINARY2U(%);

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
  } while (0);

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
  result = MANGO_E_INVALID_PROGRAM;

done:
  vm->sp_expected = (uint16_t)(sp - vm->stack);
  vm->syscall = 0;

yield:
  vm->sf =
      (stack_frame){sf.pop, sf.module,
                    (uint16_t)(ip - _mango_get_module(vm, sf.module)->image)};
  vm->rp = (uint16_t)(rp - vm->stack);
  vm->sp = (uint16_t)(sp - vm->stack);
  return result;
}

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

////////////////////////////////////////////////////////////////////////////////
