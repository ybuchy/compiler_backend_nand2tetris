#ifndef CODEWRITER_H
#define CODEWRITER_H

#include "parser.hpp"
#include <fstream>

class CodeWriter {
    std::ofstream file;
public:
    CodeWriter(const std::string filename);
    void setFileName(const std::string filename);
    void writeArithmetic(const std::string command);
    void writePushPop(const Command c, const std::string segment, const int index);
};

#endif // CODEWRITER_H
