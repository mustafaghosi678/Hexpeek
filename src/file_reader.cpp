#include "file_reader.hpp"

#include <fstream>

namespace hexpeek {

std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw FileReadError("could not open file: " + path);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(static_cast<size_t>(size));
    if (size > 0 && !file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw FileReadError("failed reading file contents: " + path);
    }

    return buffer;
}

}  // namespace hexpeek
