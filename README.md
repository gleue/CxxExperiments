# C++ Experiments

Coding adventures to explore modern C++ features.

## 01-SemanticVersioning

This exercise started as an investigation of [three-way comparisons](https://en.cppreference.com/w/cpp/language/operator_comparison#Three-way_comparison) (aka the spaceship operator) in C++20.

For details see [01-SemanticVersioning/README.md](01-SemanticVersioning/README.md)

## 02-RangesAndViews

An investigation of the [Ranges library](https://en.cppreference.com/w/cpp/ranges) in C++20.

For details see [02-RangesAndViews/README.md](02-RangesAndViews/README.md)

## 03-MoveSematics

An investigation of move semantics especially in C++20 and later.

For details see [03-MoveSemantics/README.md](03-MoveSemantics/README.md)

## 04-Initialization

How to provide initial values to variables during construction.

For details see [04-Initialization/README.md](04-Initialization/README.md)

## 05-SellingWood

Kata from [leetcode.com](https://leetcode.com/problems/selling-pieces-of-wood/).

For details see [05-SellingWood/README.md](05-SellingWood/README.md)

## 06-N_Back

Kata from [Clean Code Developer School](https://ccd-school.de/coding-dojo/application-katas/n-back/).

For details see [06-N_Back/README.md](06-N_Back/README.md)

## 07-Constness

An investigation of the different flavors of const'ness -- constexpr, consteval, constinit.

For details see [07-Constness](07-Constness/README.md)

## 08-Templates

For details see [08-Templates](08-Templates/README.md)

## 09-Coroutines

For details see [09-Coroutines](09-Coroutines/README.md)

## 10-CakeMachine

A Kata about multithreading & concurrency.

For details see [10-CakeMachine](10-CakeMachine/README.md)

## More to come...

## Requirements

- [CMake](https://cmake.org) >= 3.10
- plus project specific requirements

## Building

- Clone this repository
  ```console
  $ git clone https://github.com/gleue/CxxExperiments.git <repo-dir>
  ```
- Use CMake to configure a build system
  ```console
  $ cd <repo-dir>
  $ cmake -S . -B <build-dir>
  ```
  Alternatively using GCC 14 installed via [Homebrew](https://brew.sh) on macOS:
  ```console
  $ brew install gcc
  $ cmake -S . -B ./build -D CMAKE_C_COMPILER=$(brew --prefix gcc)/bin/gcc-14 -D CMAKE_CXX_COMPILER=$(brew --prefix gcc)/bin/g++-14
  ```

- Build the project
  ```console
  $ cmake --build <build-dir>
  ```

# License

This code is published under [The MIT License](https://opensource.org/license/mit/):

```
Copyright 2025 Tim Gleue <tim@gleue-interactive.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
```
