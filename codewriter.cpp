#include "codewriter.hpp"

#define writeLine(line) file << line << '\n'

/*
  A=M: A is now the stack pointer
  D=M: D is now the first item on the stack
  M=D: item is now in the given register
  M=M-1: Decrement stack pointer by one
  addr is where to save it to
 */
#define popStack(addr) do { \
    writeLine("// popStack"); \
    writeLine("@SP"); \
    writeLine("M=M-1"); \
    writeLine("A=M"); \
    writeLine("D=M"); \
    writeLine("@" << addr); \
    writeLine("M=D"); \
} while (false)

/*
  D=M: D is now what should be pushed onto the stack
  M=D: M is now pushed onto the stack
  M=M+1: Increment the stack pointer by one
  addr is what to push to the stack
*/
#define pushStack(addr) do { \
    writeLine("// pushStack"); \
    writeLine("@" << addr); \
    writeLine("D=M"); \
    writeLine("@SP"); \
    writeLine("A=M"); \
    writeLine("M=D"); \
    writeLine("@SP"); \
    writeLine("M=M+1"); \
} while (false)

// Takes the operand from R13 and performs the operation, saves to R15
#define oneOperandOperation(operation) do { \
    writeLine("// oneOperandOperation"); \
    writeLine("@R13"); \
    writeLine("D=" << operation << "M"); \
    writeLine("@R15"); \
    writeLine("M=D"); \
} while (false)

// Takes the operands from R13 and R14 and performs the operation, saves to R15
#define twoOperandOperation(operation) do { \
    writeLine("// twoOperandOperation"); \
    writeLine("@R13"); \
    writeLine("D=M"); \
    writeLine("@R14"); \
    writeLine("D=M" << operation << "D"); \
    writeLine("@R15"); \
    writeLine("M=D"); \
} while (false)

// uses jump to compare R13 and R14, if jump condition is met R15 is -1, else 0
#define compareTwoOperands(jump, nr) do { \
    writeLine("// compareTwoOperands"); \
    writeLine("@R13"); \
    writeLine("D=M"); \
    writeLine("@R14"); \
    writeLine("D=M-D"); \
    writeLine("@TRUE" << nr); \
    writeLine("D;" << jump); \
    writeLine("D=0"); \
    writeLine("@CONTINUE" << nr); \
    writeLine("0;JMP"); \
    writeLine("(TRUE" << nr << ")"); \
    writeLine("D=-1"); \
    writeLine("(CONTINUE" << nr << ")"); \
    writeLine("@R15"); \
    writeLine("M=D"); \
} while (false)


CodeWriter::CodeWriter(const std::string filename) {
    file.open(filename);
}

void CodeWriter::setFileName(const std::string filename) {
    file.close();
    file.open(filename);
}

int testing = 0;
void CodeWriter::writeArithmetic(const std::string command) {
    popStack("R13");
    if (command.compare("not") != 0 && command.compare("neg") != 0) {
        popStack("R14");
    }
    if (command.compare("add") == 0) {
        twoOperandOperation("+");
        pushStack("R15");
    } else if (command.compare("sub") == 0) {
        twoOperandOperation("-");
        pushStack("R15");
    } else if (command.compare("neg") == 0) {
        oneOperandOperation("-");
        pushStack("R15");
    } else if (command.compare("eq") == 0) {
        compareTwoOperands("JEQ", testing);
        pushStack("R15");
        testing++;
    } else if (command.compare("gt") == 0) {
        compareTwoOperands("JGT", testing);
        pushStack("R15");
        testing++;
    } else if (command.compare("lt") == 0) {
        compareTwoOperands("JLT", testing);
        pushStack("R15");
        testing++;
    } else if (command.compare("and") == 0) {
        twoOperandOperation("&");
        pushStack("R15");
    } else if (command.compare("or") == 0) {
        twoOperandOperation("|");
        pushStack("R15");
    } else if (command.compare("not") == 0) {
        oneOperandOperation("!");
        pushStack("R15");
    }
}

// argument
// local
// static
// constant
// this
// that
// pointer
// temp

void CodeWriter::writePushPop(const Command c,
                              const std::string segment,
                              const int index) {
    // Only need push constant x command rn for stage 1
    if (c == C_Push) {
        if (segment.compare("constant") == 0){
            writeLine("@" + std::to_string(index));
            writeLine("D=A");
            writeLine("@SP");
            writeLine("A=M");
            writeLine("M=D");
            writeLine("@SP");
            writeLine("M=M+1");
        }
    } else if (c == C_Pop) {

    }
}
