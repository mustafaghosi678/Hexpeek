#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace hexpeek {

class FileReadError : public std::runtime_error {
public:
    explicit FileReadError(const std::string& msg) : std::runtime_error(msg) {}
};

// Reads an entire file into memory as raw bytes. Throws FileReadError if the file can't be
// opened. Kept deliberately simple (whole-file read) rather than streaming/chunked reading —
// appropriate for a portfolio tool meant to inspect small-to-medium files, not multi-gigabyte
// ones.
std::vector<uint8_t> read_file(const std::string& path);

}  // namespace hexpeek
