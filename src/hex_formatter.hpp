#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace hexpeek {

// Returns true if a byte is a printable ASCII character (0x20-0x7E), used to decide whether
// to show the actual character or a '.' placeholder in the ASCII column.
bool is_printable(uint8_t byte);

// Formats one line of a hex dump: an 8-digit offset, up to `bytes_per_line` hex byte values
// (space-separated, in groups of 8 for readability), and an ASCII representation with
// non-printable bytes shown as '.'.
//
// Example output for offset=0, bytes={0x48,0x65,0x6c,0x6c,0x6f}, bytes_per_line=16:
// "00000000  48 65 6c 6c 6f                                    Hello"
std::string format_line(size_t offset, const std::vector<uint8_t>& bytes, size_t bytes_per_line = 16);

// Formats an entire buffer as a multi-line hex dump.
std::string format_dump(const std::vector<uint8_t>& data, size_t bytes_per_line = 16);

}  // namespace hexpeek
