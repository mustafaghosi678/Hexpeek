#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace hexpeek {

// Parses a search pattern given on the command line into raw bytes to search for.
// Two forms are supported:
//   "text:hello"  -> the literal ASCII bytes of "hello"
//   "hex:48656c6c6f" -> the bytes 0x48 0x65 0x6c 0x6c 0x6f (same bytes, different input form)
// Throws std::invalid_argument for a malformed pattern (odd-length hex, unknown prefix, etc).
std::vector<uint8_t> parse_pattern(const std::string& pattern_arg);

// Returns every offset in `data` where `pattern` occurs (naive substring search — fine for
// the file sizes this tool is meant to inspect; not optimised for huge files).
std::vector<size_t> find_all(const std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern);

}  // namespace hexpeek
