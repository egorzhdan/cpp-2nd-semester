#include <iostream>
#include <algorithm>
#include <string>

void print_all_occurrence(const std::string &s, const std::string &sub, size_t offset) {
    size_t pos = s.find(sub, 0);
    while (pos != std::string::npos) {
        printf("%zu\n", pos + offset);
        pos = s.find(sub, pos + 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Wrong number of arguments: %d got, 3 expected", argc);
        return 1;
    }

    auto word = std::string(argv[argc - 1]);
    auto file_name = argv[1];

    const size_t BUF_SIZE = std::max((size_t) 100, word.length());

    FILE *file = fopen(file_name, "r");
    char buf[BUF_SIZE];

    std::string cur_str, prev_str;

    auto read_cnt = BUF_SIZE;
    for (size_t i = 0; read_cnt == BUF_SIZE; ++i) {
        read_cnt = fread(buf, sizeof(char), BUF_SIZE, file);
        cur_str = std::string(buf, read_cnt);

        auto overlap = prev_str.substr(std::max(prev_str.length(), word.length()) - word.length()) +
                       cur_str.substr(0, word.length() - 1);

        print_all_occurrence(overlap, word, i * BUF_SIZE - word.length());
        print_all_occurrence(cur_str, word, i * BUF_SIZE);

        prev_str = cur_str;
    }

    return 0;
}