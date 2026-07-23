# HexPeek

A small command-line hex dump and binary search tool, written in C++17 — like the classic
`xxd`/`hexdump` utilities, plus a simple byte-pattern search.

## Why this exists

 HexPeek is a
smaller, more everyday systems tool — the kind of thing genuinely useful when inspecting an
unfamiliar file, and a natural companion to reverse-engineering-adjacent work, where looking at
raw bytes is a routine first step.

## Example

```
$ ./hexpeek sample_files/sample.bin
00000000  48 65 6c 6c 6f 2c 20 48  65 78 50 65 65 6b 21 00  Hello, HexPeek!.
00000010  01 02 de ad be ef 0a 54  68 69 73 20 69 73 20 61  .......This is a
00000020  20 73 6d 61 6c 6c 20 73  61 6d 70 6c 65 20 66 69   small sample fi
00000030  6c 65 20 66 6f 72 20 74  68 65 20 52 45 41 44 4d  le for the READM
00000040  45 20 64 65 6d 6f 2e 0a                           E demo..
```

Searching for a byte pattern:
```
$ ./hexpeek sample_files/sample.bin --search hex:deadbeef
Found 1 match(es) at offset(s): 0x12

$ ./hexpeek sample_files/sample.bin --search text:HexPeek
Found 1 match(es) at offset(s): 0x7
```

## Building

Requires a C++17 compiler and CMake 3.16+.

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Running

```bash
./build/hexpeek sample_files/sample.bin
./build/hexpeek sample_files/sample.bin --search hex:deadbeef
./build/hexpeek sample_files/sample.bin --search text:HexPeek
```

## Running the tests

```bash
cd build
ctest --output-on-failure
```

12 test cases cover the hex formatting logic, pattern parsing (both `text:` and `hex:` forms,
including malformed input), substring search, and real file I/O round-tripping.

## Architecture

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for the component breakdown and reasoning
behind key design decisions.

## Project status / Roadmap

Active portfolio project. Tracked as GitHub Issues:
- [ ] `--offset`/`--length` flags to dump a byte range instead of the whole file
- [ ] Colourised output highlighting matched search patterns within the hex view
- [ ] Faster search algorithm (Boyer-Moore or similar) if larger files become a real use case

## License
MIT — see [LICENSE](LICENSE).
