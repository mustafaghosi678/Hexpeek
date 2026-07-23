#include "hex_formatter.hpp"

#include <sstream>
#include <iomanip>

namespace hexpeek {

bool is_printable(uint8_t byte) {
    return byte >= 0x20 && byte <= 0x7E;
}

std::string format_line(size_t offset, const std::vector<uint8_t>& bytes, size_t bytes_per_line) {
    std::ostringstream out;

    // Offset column, e.g. "00000000  "
    out << std::hex << std::setw(8) << std::setfill('0') << offset << "  ";

    // Hex byte columns, with an extra space after every 8th byte for readability.
    for (size_t i = 0; i < bytes_per_line; ++i) {
        if (i < bytes.size()) {
            out << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]) << " ";
        } else {
            out << "   ";  // padding for a short final line, keeps ASCII column aligned
        }
        if (i == bytes_per_line / 2 - 1) {
            out << " ";
        }
    }

    out << " ";

    // ASCII column.
    for (uint8_t b : bytes) {
        out << (is_printable(b) ? static_cast<char>(b) : '.');
    }

    return out.str();
}

std::string format_dump(const std::vector<uint8_t>& data, size_t bytes_per_line) {
    std::ostringstream out;
    for (size_t offset = 0; offset < data.size(); offset += bytes_per_line) {
        size_t line_len = std::min(bytes_per_line, data.size() - offset);
        std::vector<uint8_t> line(data.begin() + static_cast<long>(offset),
                                   data.begin() + static_cast<long>(offset + line_len));
        out << format_line(offset, line, bytes_per_line) << "\n";
    }
    return out.str();
}

}  // namespace hexpeek
