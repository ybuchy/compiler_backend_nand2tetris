#ifndef PARSER_H
#define PARSER_H

#include <queue>
#include <string>

enum Command {
	      C_Arithmetic,
	      C_Push,
	      C_Pop,
	      C_Label,
	      C_Goto,
	      C_If,
	      C_Function,
	      C_Return,
	      C_Call
};

class Parser {
    std::queue<std::string> lines;
    std::string cur_line;
public:
    Parser(const std::string file);
    bool hasMoreCommands(void);
    void advance(void);
    Command commandType(void);
    std::string arg1(void);
    int arg2(void);
};

#endif // PARSER_H
