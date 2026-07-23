# Architecture

## Components

1. **File reader** (`file_reader.hpp/.cpp`) — reads an entire file into a `vector<uint8_t>`.
   Whole-file read rather than streaming, which is a deliberate scope choice for a tool meant
   to inspect small-to-medium files, not multi-gigabyte ones.
2. **Hex formatter** (`hex_formatter.hpp/.cpp`) — pure functions that turn raw bytes into the
   classic hex+ASCII dump format. No file I/O here at all, which is what makes these functions
   trivially unit-testable with hand-built byte vectors.
3. **Searcher** (`searcher.hpp/.cpp`) — parses a search pattern (`text:` or `hex:` prefixed)
   and does a naive substring search over the byte buffer, returning every matching offset.
4. **CLI** (`main.cpp`) — wires the above together, handling either a plain dump or a search,
   with clean error messages for anything that goes wrong.

## Why this shape

Same principle as the other two portfolio projects: **pure, testable logic separated from I/O
and CLI plumbing.** `hex_formatter.cpp` and `searcher.cpp` have zero dependency on the
filesystem, so almost the entire test suite runs with no file I/O at all — the one exception
being `test_read_file_end_to_end`, which deliberately writes and reads back a small temp file
to prove the file-reading path works against real file I/O, not just in-memory data.

## Known limitations (honest, not hidden)

- Whole-file read only — no streaming for very large files.
- Search is a naive O(n·m) substring scan, not a fast string-matching algorithm
  (Boyer-Moore/KMP) — completely fine at the file sizes this tool targets, and a natural
  "next step" if it ever needed to handle much larger inputs.
- No colourised terminal output — plain text only, to keep the tool portable across terminals
  and easy to redirect/pipe.

## Natural next steps (tracked as GitHub Issues)

- Support reading a byte range (`--offset`/`--length`) instead of always dumping the whole file
- Colourised output for matched search patterns within the hex dump view
- A faster search algorithm if very large files become a real use case
