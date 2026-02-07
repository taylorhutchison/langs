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

### Calendar
Ouutput a simple monthly calendar given a month name and a year as CLI args

### Stopwatch
Simple stopwatch

### Text Search
Find text in a large document

### Web Crawl
Create a link tree given a URL

### Web Server
Simple web server for static files
