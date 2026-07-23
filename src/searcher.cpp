#include "searcher.hpp"

#include <stdexcept>

namespace hexpeek {

namespace {

uint8_t hex_nibble(char c) {
    if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - '0');
    if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(c - 'a' + 10);
    if (c >= 'A' && c <= 'F') return static_cast<uint8_t>(c - 'A' + 10);
    throw std::invalid_argument(std::string("invalid hex character: ") + c);
}

}  // namespace

std::vector<uint8_t> parse_pattern(const std::string& pattern_arg) {
    const std::string text_prefix = "text:";
    const std::string hex_prefix = "hex:";

    if (pattern_arg.rfind(text_prefix, 0) == 0) {
        std::string text = pattern_arg.substr(text_prefix.size());
        if (text.empty()) {
            throw std::invalid_argument("text: pattern cannot be empty");
        }
        return std::vector<uint8_t>(text.begin(), text.end());
    }

    if (pattern_arg.rfind(hex_prefix, 0) == 0) {
        std::string hex = pattern_arg.substr(hex_prefix.size());
        if (hex.empty() || hex.size() % 2 != 0) {
            throw std::invalid_argument("hex: pattern must be a non-empty, even-length hex string");
        }
        std::vector<uint8_t> bytes;
        bytes.reserve(hex.size() / 2);
        for (size_t i = 0; i < hex.size(); i += 2) {
            uint8_t high = hex_nibble(hex[i]);
            uint8_t low = hex_nibble(hex[i + 1]);
            bytes.push_back(static_cast<uint8_t>((high << 4) | low));
        }
        return bytes;
    }

    throw std::invalid_argument("pattern must start with 'text:' or 'hex:', got: " + pattern_arg);
}

std::vector<size_t> find_all(const std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern) {
    std::vector<size_t> matches;
    if (pattern.empty() || pattern.size() > data.size()) {
        return matches;
    }

    for (size_t i = 0; i <= data.size() - pattern.size(); ++i) {
        bool match = true;
        for (size_t j = 0; j < pattern.size(); ++j) {
            if (data[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            matches.push_back(i);
        }
    }
    return matches;
}

}  // namespace hexpeek
