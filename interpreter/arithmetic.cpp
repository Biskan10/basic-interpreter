#include "arithmetic.h"

using namespace std;

NumericExpression::~NumericExpression() { }

BinaryExpression::BinaryExpression(NumericExpression* left, 
	NumericExpression* right, char sign, 
	std::map<std::string,std::map<int,int> >* map) : 
	left(left), right(right), sign(sign), map(map){
}

BinaryExpression::~BinaryExpression() { }

int BinaryExpression::getValue(){
	int target;
    if(sign=='+'){
    	target = this->left->getValue() + this->right->getValue();
	}
	else if(sign=='-'){
    	target = this->left->getValue() - this->right->getValue();
	}
	else if(sign=='*'){
    	target = this->left->getValue() * this->right->getValue();
	}
	else if(sign=='/'){

		if(right->getValue()==0){
			stringstream s1, s2;
			s1 << left->getValue();
			s2 << right->getValue();
			string str1 = s1.str();
			string str2 = s2.str();
			throw invalid_argument("Division by 0: "+left->format()
				+" = "+str1+", "+right->format()+" = "+str2+".");
		}

    	target = this->left->getValue() / this->right->getValue();
	}
	
	for(it = map->begin(); it != map->end(); it++){
		(it->second).insert(std::pair<int,int>(target, 0));
	}
	
	return target;
}
int BinaryExpression::getAddress() { }

std::string BinaryExpression::getName(){ }

std::string BinaryExpression::format() const {
	string target;
	if(sign=='+'){
    	target = "(" + this->left->format() + " + " + 
    	this->right->format() + ")";
	}
	else if(sign=='-'){
    	target = "(" + this->left->format() + " - " + 
    	this->right->format() + ")";
	}
	else if(sign=='*'){
    	target = "(" + this->left->format() + " * " + 
    	this->right->format() + ")";
	}
	else if(sign=='/'){
    	target = "(" + this->left->format() + " / " + 
    	this->right->format() + ")";
	}
	return target;
}

void BinaryExpression::setValue(int setter){ }

void BinaryExpression::remDelete(){
	this->left->remDelete();
	this->right->remDelete();
	delete this->left;
	delete this->right;
}


Constant::Constant(int value) : 
	value(value){
}
Constant::~Constant(){ }

int Constant::getValue() {
	return value;
}

int Constant::getAddress() { }

std::string Constant::getName() { }

std::string Constant::format() const {
	stringstream ss;
	ss << this->value;
	string str = ss.str();
	return str;
}

void Constant::setValue(int setter){ }

void Constant::remDelete(){ }


IntVar::IntVar(std::string Varname, 
	std::map<std::string,std::map<int,int> >* map) : 
	Varname(Varname), map(map){
}

IntVar:: ~IntVar(){ } 	

int IntVar::getValue(){
	std::map<int,int>::iterator it_return;
	it = map->find(Varname);
	it_return = (it->second).begin();
	return it_return->second;
}

int IntVar::getAddress(){
	return 0;
}

std::string IntVar::getName(){
	return this->Varname;
}

std::string IntVar::format() const {
	return this->Varname;
}

void IntVar::setValue(int setter){
	std::map<int,int>::iterator it_return;
	it = map->find(Varname);

	it_return = (it->second).begin();
	it_return->second = setter;
}

void IntVar::remDelete(){ }


ArrayVar::ArrayVar(std::string Varname, NumericExpression* address, 
	std::map<std::string,std::map<int,int> >* map) : Varname(Varname), 
	address(address), map(map){
}
ArrayVar::~ArrayVar(){ }

int ArrayVar::getValue(){
	std::map<int,int>::iterator it_return;
	it = map->find(getName());
	it_return = (it->second).find(address->getValue());

	return it_return->second;
}

int ArrayVar::getAddress(){
	return address->getValue();
}

std::string ArrayVar::getName(){
	return this->Varname + "[]";
}

std::string ArrayVar::format() const {
	return this->Varname + "[" + this->address->format() + "]";
}

void ArrayVar::setValue(int setter){
	std::map<int,int>::iterator it_return;
	it = map->find(getName());

	it_return = (it->second).find(address->getValue());
	it_return->second = setter;
}

void ArrayVar::remDelete(){
	this->address->remDelete();
	delete this->address;
}


BoolExpression::BoolExpression(NumericExpression* left, 
	NumericExpression* right, char sign) : left(left), 
	right(right), sign(sign){
}

BoolExpression::~BoolExpression(){ }

std::string BoolExpression::format() const {
	string target;
	if(sign=='='){
    	target = "[" + this->left->format() + " = " + 
    	this->right->format() + "]";
	}
	else if(sign=='<'){
    	target = "[" + this->left->format() + " < " + 
    	this->right->format() + "]";
	}
	else if(sign=='>'){
    	target = "[" + this->right->format() + " < " + 
    	this->left->format() + "]";
	}
	return target;
}

bool BoolExpression::boolVal(){
	bool target;
	if(sign=='='){
    	target = (this->left->getValue() == this->right->getValue());
	}
	else if(sign=='<'){
    	target = (this->left->getValue() < this->right->getValue());
	}
	else if(sign=='>'){
    	target = (this->left->getValue() > this->right->getValue());
	}
	return target;
}

void BoolExpression::remDelete(){
	this->left->remDelete();
	this->right->remDelete();
	delete this->left;
	delete this->right;
}