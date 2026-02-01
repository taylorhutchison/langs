# Language Learning

Small projects implemented in C#, C++, and Rust to explore and learn the differences between languages.

By default the projects are built this way:
- C#
  - dotnet run Program.cs
- Rust
  - cargo run
- C++
  - Windows: cl /EHsc /std:c++20 /Fe:main.exe main.cpp
  - Linux: g++ -std=c++20 main.cpp -o main

## Projects
### bmp_reader
Read a BMP file and report back on the colors found. Practices reading a binary format.

### prime_sieve
Find prime numbers using the sieve approach.

### Conway's game of life
Generational life simulation in the console
