# Collatz Conjecture

This is a ~~highly optimized~~, profoundly ~~over-engineered~~ C++ program dedicated to doing exactly two things, dividing by two, and multiplying by three then adding one.

It does this using a custom-built **Number** class, bypassing standard libraries entirely, because apparently 64-bit integers are not large enough for our collective obsession with an unsolved math problem from 1937.

It will take a staggeringly large number, apply basic arithmetic to it millions of times, heat up your CPU, and (*probably*) eventually output the number 1. You are welcome.

## Building the Time Waster
You will need a modern C++ compiler and CMake.
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
>The Release flag is strictly necessary. We need -O3 optimization to ensure we waste your CPU cycles at maximum efficiency.

## Usage
Just run the executable with -h flag

## Contributing
Please do not submit pull requests attempting to solve the actual conjecture. If you have a rigorous mathematical proof for the Collatz Conjecture, publish it in an academic journal, collect your Fields Medal, and leave this repository alone.

If you want to optimize any function so it takes fewer nanoseconds, feel free.