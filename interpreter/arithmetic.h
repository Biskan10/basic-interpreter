#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <iterator>

class NumericExpression {
public:
    virtual ~NumericExpression();
    
    virtual int getValue() = 0;
    virtual int getAddress() = 0;

    virtual std::string getName() = 0;
    virtual std::string format() const = 0;

    virtual void setValue(int setter) = 0;
    virtual void remDelete() = 0;  
};

class BinaryExpression : public NumericExpression {
public:
    BinaryExpression(NumericExpression* left, NumericExpression* right, 
    	char sign, std::map<std::string,std::map<int,int> >* map);

    ~BinaryExpression();

    int getValue();
    int getAddress();

	std::string getName();
    std::string format() const;
    
    void setValue(int setter);
    void remDelete();

private:
    NumericExpression* left;
    NumericExpression* right;
    char sign;

    std::map<std::string,std::map<int,int> >* map;
    std::map<std::string,std::map<int,int> >::iterator it;
};

class Constant : public NumericExpression{
public:
	Constant(int value);

	~Constant();
	
	int getValue();
	int getAddress();

	std::string getName();
    std::string format() const;
    
    void setValue(int setter);
    void remDelete();

private:
	int value;
};

class IntVar : public NumericExpression{
public:
	IntVar(std::string Varname,  
		std::map<std::string,std::map<int,int> >* map);

	~IntVar(); 
	
	int getValue();
	int getAddress();

	std::string getName();
    std::string format() const;
    
    void setValue(int setter);
    void remDelete();

private:
	std::string Varname;

    std::map<std::string,std::map<int,int> >* map;
    std::map<std::string,std::map<int,int> >::iterator it;
};

class ArrayVar : public NumericExpression{
public:
	ArrayVar(std::string Varname, NumericExpression* address, 
		std::map<std::string,std::map<int,int> >* map);

	~ArrayVar(); 
	
	int getValue();
	int getAddress();

	std::string getName();
    std::string format() const;
    
    void setValue(int setter);
    void remDelete();

private:
	std::string Varname;
	NumericExpression* address;

    std::map<std::string,std::map<int,int> >* map;
    std::map<std::string,std::map<int,int> >::iterator it;
};

class BoolExpression{
public:
	BoolExpression(NumericExpression* left, NumericExpression* right, 
		char sign);

	~BoolExpression();

	std::string format() const;

	bool boolVal();
	void remDelete();
    
private:
	NumericExpression* left;
	NumericExpression* right;
	char sign;
};

#endif