#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <iostream>
#include <stack>
#include <map>
#include <iterator>
#include "arithmetic.h"

class Command {
public:
	virtual ~Command();
    virtual std::string format() const = 0;
    virtual void remDelete() = 0;

    virtual void runCommand(std::map<int,Command*>::iterator &it, 
		std::map<int,Command*>* map, std::stack<int>* log, 
		std::ostream& out) = 0;
};

class Print : public Command{
public:
	Print(NumericExpression* exp);
	~Print();
	std::string format() const;
	void remDelete();

	void runCommand(std::map<int,Command*>::iterator &it, 
		std::map<int,Command*>* map, std::stack<int>* log, 
		std::ostream& out);
private:
	NumericExpression* exp;
};

class LetInt : public Command{
public:
	LetInt(NumericExpression* var, NumericExpression* value);
	~LetInt();
	std::string format() const;
	void remDelete();

	void runCommand(std::map<int,Command*>::iterator &it, 
		std::map<int,Command*>* map, std::stack<int>* log, 
		std::ostream& out);
private:
	NumericExpression* var;
	NumericExpression* value;
};

class LetArr : public Command{
public:
	LetArr(NumericExpression* array, NumericExpression* value);
	~LetArr();
	std::string format() const;
	void remDelete();

	void runCommand(std::map<int,Command*>::iterator &it, 
		std::map<int,Command*>* map, std::stack<int>* log, 
		std::ostream& out);
private:
	NumericExpression* array;
	NumericExpression* value;

};

class Goto : public Command{
public:
	Goto(int dest_line);
	~Goto();
	std::string format() const;
	void remDelete();

	void runCommand(std::map<int,Command*>::iterator &it, 
		std::map<int,Command*>* map, std::stack<int>* log, 
		std::ostream& out);
private:
	int dest_line;
};

class Gosub : public Command{
public:
	Gosub(int dest_line);
	~Gosub();
	std::string format() const;
	void remDelete();

	void runCommand(std::map<int,Command*>::iterator &it, 
		std::map<int,Command*>* map, std::stack<int>* log, 
		std::ostream& out);
private:
	int dest_line;
	int origin_line;
};

class IfThen : public Command{
public:
	IfThen(BoolExpression* if_bool, int dest_line);
	~IfThen();
	std::string format() const;
	void remDelete();

	void runCommand(std::map<int,Command*>::iterator &it, 
		std::map<int,Command*>* map, std::stack<int>* log, 
		std::ostream& out);
private:
	BoolExpression* if_bool;
	int dest_line;
};

class Return : public Command{
public:
	Return();
	~Return();
	std::string format() const;
	void remDelete();

	void runCommand(std::map<int,Command*>::iterator &it, 
		std::map<int,Command*>* map, std::stack<int>* log, 
		std::ostream& out);
};

class End : public Command{
public:
	End();
	~End();
	std::string format() const;
	void remDelete();

	void runCommand(std::map<int,Command*>::iterator &it, 
		std::map<int,Command*>* map, std::stack<int>* log, 
		std::ostream& out);
};

#endif