# Mango Virtual Machine

[![Maintenance](https://img.shields.io/maintenance/yes/2017.svg)](https://github.com/ektrah/mango)
[![MIT License](https://img.shields.io/github/license/ektrah/mango.svg?maxAge=2592000)](LICENSE)

Mango is a tiny, yet powerful virtual machine designed to run in very constrained memory space (<1 KB RAM).

#### Features

* A fast stack-based bytecode interpreter
* ~2000 source lines of C11 code
* Minimal dependencies
* A real-world instruction set
  * Arithmetic, bitwise, conversion, comparison, branches, function calls, ...
* An expressive type system
  * i8, i16, i32, i64, u8, u16, u32, u64, f32, f64
  * Algebraic data types (structs and tagged unions)
  * Arrays and slices
  * References
* A module system
* Code access security (security critical and security safe-critical code)
* Memory safety and type safety
* No garbage collector
* An easy-to-use C API (event-loop friendly)
* A minimal runtime
