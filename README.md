# Mango Virtual Machine

[![Build Status](https://travis-ci.org/ektrah/mango.svg?branch=master)](https://travis-ci.org/ektrah/mango)
[![MIT License](https://img.shields.io/github/license/ektrah/mango.svg?maxAge=2592000)](LICENSE)

Mango is a tiny, yet powerful virtual machine designed to run in very constrained memory space (<1 KB RAM).

#### Features

* A fast stack-based bytecode interpreter
* ~2200 lines of C11 code
* Minimal dependencies
* A module system
* Code access security (security critical and security safe-critical code)
* A comprehensive instruction set
  * Arithmetic, bitwise, conversion, comparison, branches, function calls, ...
* An expressive type system
  * i8, i16, i32, i64, u8, u16, u32, u64, f32, f64
  * Algebraic data types (structs and tagged unions)
  * Arrays and slices
  * Non-nullable references
* Memory safety and type safety
* No garbage collector
* Minimal runtime
