# Mango Virtual Machine

[![CircleCI](https://circleci.com/gh/ektrah/mango.svg?style=shield)](https://circleci.com/gh/ektrah/mango)
[![Maintenance](https://img.shields.io/maintenance/yes/2019.svg)](https://github.com/ektrah/mango)
[![MIT License](https://img.shields.io/github/license/ektrah/mango.svg?maxAge=2592000)](LICENSE)

Mango is a tiny, yet powerful virtual machine that runs type-safe mobile code in very little memory (<1 KiB RAM).

#### Features

* A fast stack-based bytecode interpreter
* ~2000 source lines of C11 code
* Minimal dependencies
* A real-world instruction set
  * Arithmetic, bitwise, conversion, comparison, branches, function calls, ...
* An expressive type system
  * i8, i16, i32, i64, u8, u16, u32, u64, f32, f64
  * Structs and discriminated unions
  * Arrays and slices
  * References
* A module system
* Memory safety and type safety
* No garbage collector
* No standard library
* An easy-to-use C API (event-loop friendly)
