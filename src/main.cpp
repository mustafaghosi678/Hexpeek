#include <iostream>

#include "file_reader.hpp"
#include "hex_formatter.hpp"
#include "searcher.hpp"

void print_usage() {
    std::cerr << "Usage: hexpeek <file> [--search <text:PATTERN|hex:PATTERN>]\n";
    std::cerr << "  Examples:\n";
    std::cerr << "    hexpeek sample.bin\n";
    std::cerr << "    hexpeek sample.bin --search text:HELLO\n";
    std::cerr << "    hexpeek sample.bin --search hex:deadbeef\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string file_path = argv[1];
    std::string search_arg;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--search" && i + 1 < argc) {
            search_arg = argv[++i];
        } else {
            std::cerr << "Unrecognised argument: " << arg << "\n";
            print_usage();
            return 1;
        }
    }

    try {
        std::vector<uint8_t> data = hexpeek::read_file(file_path);

        if (!search_arg.empty()) {
            std::vector<uint8_t> pattern = hexpeek::parse_pattern(search_arg);
            std::vector<size_t> matches = hexpeek::find_all(data, pattern);

            if (matches.empty()) {
                std::cout << "Pattern not found.\n";
            } else {
                std::cout << "Found " << matches.size() << " match(es) at offset(s): ";
                for (size_t i = 0; i < matches.size(); ++i) {
                    std::cout << "0x" << std::hex << matches[i];
                    if (i + 1 < matches.size()) std::cout << ", ";
                }
                std::cout << std::dec << "\n";
            }
        } else {
            std::cout << hexpeek::format_dump(data);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
