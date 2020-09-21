#include "parser.hpp"
#include "codewriter.hpp"
#include <fstream>
#include <string>
#include <algorithm>

Parser::Parser(const std::string file) {
    std::ifstream f;
    f.open(file);
    std::string line;
    while (std::getline(f, line)) {
        size_t ind = line.find("//");
        if (ind != std::string::npos) line = line.substr(0, ind);
        bool lastWasSpace = false;
        // Only one space between words
        for (int i = 0; i < line.size(); ++i) {
            if (line[i] == ' ' && lastWasSpace) {
                line.erase(line.begin() + i);
                i--;
            }
            if (line[i] == ' ' && !lastWasSpace) lastWasSpace = true;
            if (line[i] != ' ') lastWasSpace = false;
        }
        // No whitespace after the last word
        int i = 0;
        while (line[line.size() - 2 - i] == ' ') {
            i++;
        }
        if (i > 0) line = line.substr(0, line.size() - i - 1);
        line.erase(std::remove(line.begin(), line.end(), (char) 13),
                   line.end());
        if (!line.empty()) lines.push(line);
    }
}

bool Parser::hasMoreCommands(void) {
    return !lines.empty();
}

void Parser::advance(void) {
    cur_line = lines.front();
    lines.pop();
}

Command Parser::commandType(void) {
    if (cur_line.find("push") != std::string::npos) return C_Push;
    else if (cur_line.find("pop") != std::string::npos) return C_Pop;
    else if (cur_line.find("label") != std::string::npos) return C_Label;
    else if (cur_line.find("if") != std::string::npos) return C_If;
    else if (cur_line.find("goto") != std::string::npos) return C_Goto;
    else if (cur_line.find("function") != std::string::npos) return C_Function;
    else if (cur_line.find("return") != std::string::npos) return C_Return;
    else if (cur_line.find("call") != std::string::npos) return C_Call;
    else return C_Arithmetic;
}

std::string Parser::arg1(void) {
    size_t starting_ind = cur_line.find(' ');
    size_t ending_ind = cur_line.rfind(' ');
    if (starting_ind != std::string::npos) {
        if (ending_ind != std::string::npos) {
            return cur_line.substr(starting_ind + 1,
                                   ending_ind - (starting_ind + 1));
        } else {
            return cur_line.substr(starting_ind + 1);
        }
    } else {
        return cur_line;
    }
}

int Parser::arg2(void) {
    size_t starting_ind = cur_line.rfind(' ');
    if (starting_ind != std::string::npos &&
        starting_ind != cur_line.find(' ')) {
        return std::stoi(cur_line.substr(starting_ind + 1));
    } else {
        return -1;
    }
}
