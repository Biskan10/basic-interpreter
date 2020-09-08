#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <fstream>
#include <map>
#include <stack>
#include <iterator>

#include "command.h"
#include "arithmetic.h"

class Interpreter {
public:
    Interpreter(std::istream& in);
    virtual ~Interpreter();

    void write(std::ostream& out);
    void execute(std::ostream& out);

    std::map<int, Command*> basic_log;
    std::map<int, Command*>::iterator it_basic;

    std::map<std::string, std::map<int, int> > var_log;
    std::map<std::string, std::map<int, int> >::iterator it_var;

    std::stack<int> gosub_log;

private:
    void parse(std::istream& in);
    void skipspace(std::stringstream& ss);

    NumericExpression* parsenum(std::stringstream& ss);
    NumericExpression* parsebin(std::stringstream& ss);
    NumericExpression* parsevar(std::stringstream& ss);
    NumericExpression* parseconstant(std::stringstream& ss);

    BoolExpression* parsebool(std::stringstream& ss);

    void remDelete();
    
    bool isnextarr;
};

#endif