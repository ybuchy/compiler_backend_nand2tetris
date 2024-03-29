#include "parser.hpp"
#include "codewriter.hpp"
#include <string>
#include <filesystem>

void translateFile(Parser &p, CodeWriter &c) {
    while (p.hasMoreCommands()) {
        p.advance();
        switch (p.commandType()) {
            case (C_Arithmetic):
                c.writeArithmetic(p.arg1());
                break;
            case (C_Push):
                c.writePushPop(p.commandType(), p.arg1(), p.arg2());
                break;
            default:
                break;
        }
    }
}

int main (int argc, char **argv) {
    if (argc != 2) {
        printf("usage: ./vmtranslator source");
        return 1;
    }
    const size_t ind = std::string(argv[1]).find('.');
    std::vector<std::string> files;
    if (ind == std::string::npos) {
        for (auto &p : std::filesystem::directory_iterator(argv[1])) {
            const std::string path = p.path();
            const size_t index = path.find(".vm");
            if (index != std::string::npos) {
                files.push_back(path.substr(0, index));
            }
        }
    } else {
        files.push_back(std::string(argv[1]).substr(0, ind));
    }
    size_t i = 0;
    do {
        CodeWriter c = CodeWriter(files[i] + ".asm");
        Parser p = Parser(files[i] + ".vm");
        translateFile(p, c);
        i++;
    } while (i < files.size());
}
