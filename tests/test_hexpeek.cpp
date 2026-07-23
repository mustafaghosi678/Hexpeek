// Minimal self-contained test harness, same pattern as the MiniVM project — no external
// framework dependency, builds with just a C++17 compiler and CMake.
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../src/file_reader.hpp"
#include "../src/hex_formatter.hpp"
#include "../src/searcher.hpp"

using namespace hexpeek;

static int g_failures = 0;

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                          \
            std::cerr << "FAILED: " << #cond << " (line " << __LINE__ << ")\n"; \
            g_failures++;                                                       \
        }                                                                       \
    } while (0)

#define CHECK_EQ(a, b)                                                                      \
    do {                                                                                    \
        auto va = (a);                                                                      \
        auto vb = (b);                                                                      \
        if (!(va == vb)) {                                                                  \
            std::cerr << "FAILED: " << #a << " == " << #b << " (line " << __LINE__ << ")\n"; \
            g_failures++;                                                                   \
        }                                                                                    \
    } while (0)

#define CHECK_THROWS(expr)                                                 \
    do {                                                                   \
        bool threw = false;                                               \
        try {                                                             \
            expr;                                                         \
        } catch (const std::exception&) {                                 \
            threw = true;                                                 \
        }                                                                 \
        if (!threw) {                                                    \
            std::cerr << "FAILED: expected exception from " << #expr     \
                       << " (line " << __LINE__ << ")\n";                 \
            g_failures++;                                                 \
        }                                                                 \
    } while (0)

void test_is_printable() {
    CHECK(is_printable('A'));
    CHECK(is_printable(' '));
    CHECK(is_printable('~'));
    CHECK(!is_printable(0x00));
    CHECK(!is_printable(0x7F));
    CHECK(!is_printable(0x01));
}

void test_format_line_ascii() {
    std::vector<uint8_t> bytes = {'H', 'e', 'l', 'l', 'o'};
    std::string line = format_line(0, bytes, 16);
    CHECK(line.find("48 65 6c 6c 6f") != std::string::npos);
    CHECK(line.find("Hello") != std::string::npos);
    CHECK(line.substr(0, 8) == "00000000");
}

void test_format_line_non_printable_shows_dot() {
    std::vector<uint8_t> bytes = {0x00, 0x01, 'A'};
    std::string line = format_line(0, bytes, 16);
    // ASCII column should be "..A" (dots for non-printable, then the literal char)
    CHECK(line.find("..A") != std::string::npos);
}

void test_format_dump_multiple_lines() {
    std::vector<uint8_t> data(20, 'X');  // 20 bytes -> 2 lines at 16 bytes/line
    std::string dump = format_dump(data, 16);
    size_t newline_count = static_cast<size_t>(std::count(dump.begin(), dump.end(), '\n'));
    CHECK_EQ(newline_count, 2u);
    CHECK(dump.find("00000000") != std::string::npos);
    CHECK(dump.find("00000010") != std::string::npos);  // second line starts at offset 16 = 0x10
}

void test_parse_pattern_text() {
    auto bytes = parse_pattern("text:Hi");
    CHECK_EQ(bytes.size(), 2u);
    CHECK_EQ(bytes[0], static_cast<uint8_t>('H'));
    CHECK_EQ(bytes[1], static_cast<uint8_t>('i'));
}

void test_parse_pattern_hex() {
    auto bytes = parse_pattern("hex:deadbeef");
    CHECK_EQ(bytes.size(), 4u);
    CHECK_EQ(bytes[0], 0xde);
    CHECK_EQ(bytes[1], 0xad);
    CHECK_EQ(bytes[2], 0xbe);
    CHECK_EQ(bytes[3], 0xef);
}

void test_parse_pattern_rejects_bad_input() {
    CHECK_THROWS(parse_pattern("nope:something"));
    CHECK_THROWS(parse_pattern("hex:abc"));   // odd length
    CHECK_THROWS(parse_pattern("hex:zz"));    // invalid hex chars
    CHECK_THROWS(parse_pattern("text:"));     // empty
}

void test_find_all_finds_all_occurrences() {
    std::vector<uint8_t> data = {'a', 'b', 'c', 'a', 'b', 'c', 'x'};
    std::vector<uint8_t> pattern = {'a', 'b', 'c'};
    auto matches = find_all(data, pattern);
    CHECK_EQ(matches.size(), 2u);
    CHECK_EQ(matches[0], 0u);
    CHECK_EQ(matches[1], 3u);
}

void test_find_all_no_match() {
    std::vector<uint8_t> data = {'a', 'b', 'c'};
    std::vector<uint8_t> pattern = {'x', 'y'};
    auto matches = find_all(data, pattern);
    CHECK(matches.empty());
}

void test_find_all_pattern_longer_than_data() {
    std::vector<uint8_t> data = {'a'};
    std::vector<uint8_t> pattern = {'a', 'b', 'c'};
    auto matches = find_all(data, pattern);
    CHECK(matches.empty());
}

void test_read_file_end_to_end() {
    // Write a small temp file, read it back, confirm bytes round-trip exactly.
    const std::string path = "test_tmp_file.bin";
    std::vector<uint8_t> original = {0x00, 0xFF, 'H', 'i', 0x10};
    {
        std::ofstream out(path, std::ios::binary);
        out.write(reinterpret_cast<const char*>(original.data()),
                   static_cast<std::streamsize>(original.size()));
    }
    std::vector<uint8_t> read_back = read_file(path);
    CHECK_EQ(read_back, original);
    std::remove(path.c_str());
}

void test_read_file_missing_throws() {
    CHECK_THROWS(read_file("this_file_does_not_exist.bin"));
}

int main() {
    test_is_printable();
    test_format_line_ascii();
    test_format_line_non_printable_shows_dot();
    test_format_dump_multiple_lines();
    test_parse_pattern_text();
    test_parse_pattern_hex();
    test_parse_pattern_rejects_bad_input();
    test_find_all_finds_all_occurrences();
    test_find_all_no_match();
    test_find_all_pattern_longer_than_data();
    test_read_file_end_to_end();
    test_read_file_missing_throws();

    if (g_failures > 0) {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}
