#include "command.h"

using namespace std;

Command::~Command() {}
void Command::remDelete() {}

Print::Print(NumericExpression* exp) : exp(exp){ }

Print::~Print(){ }

string Print::format() const{
	return "PRINT " + this->exp->format();
}
void Print::remDelete(){
	this->exp->remDelete();
	delete this->exp;
}
void Print::runCommand(std::map<int,Command*>::iterator &it, 
	std::map<int,Command*>* map, std::stack<int>* log, 
	std::ostream& out){

	out << exp->getValue() << endl;
	it++;
}


LetInt::LetInt(NumericExpression* var, NumericExpression* value) : 
	var(var), value(value){
}

LetInt::~LetInt(){ }

string LetInt::format() const{
	return "LET " + this->var->format() + " " + this->value->format();
}
void LetInt::remDelete(){
	this->var->remDelete();
	this->value->remDelete();
	delete this->var;
	delete this->value;
}
void LetInt::runCommand(std::map<int,Command*>::iterator &it, 
	std::map<int,Command*>* map, std::stack<int>* log, 
	std::ostream& out){

	this->var->setValue(this->value->getValue());
	it++;
}

LetArr::LetArr(NumericExpression* array, NumericExpression* value) : 
	array(array), value(value){
}

LetArr::~LetArr(){ }

string LetArr::format() const{
	return "LET " + this->array->format() + " " + this->value->format();
}
void LetArr::remDelete(){
	this->array->remDelete();
	this->value->remDelete();
	delete this->array;
	delete this->value;
}
void LetArr::runCommand(std::map<int,Command*>::iterator &it, 
	std::map<int,Command*>* map, std::stack<int>* log, 
	std::ostream& out){
	
	this->array->setValue(this->value->getValue());
	it++;
}

Goto::Goto(int dest_line) : dest_line(dest_line){ }

Goto::~Goto(){ }

string Goto::format() const{
	stringstream ss;
	ss << this->dest_line;
	string str = ss.str();
	return "GOTO <" + str + ">";
}

void Goto::remDelete(){ }

void Goto::runCommand(std::map<int,Command*>::iterator &it, 
	std::map<int,Command*>* map, std::stack<int>* log, 
	std::ostream& out){

	std::map<int,Command*>::iterator it_check = map->find(dest_line);

	//error catching
	stringstream ss;
	ss << dest_line;
	string str = ss.str();
	if(it_check == map->end()){
		throw out_of_range("GOTO to non-existent line " + str + ".");
	}

	it = map->find(dest_line);
}

Gosub::Gosub(int dest_line) : dest_line(dest_line){ }

Gosub::~Gosub(){ }

string Gosub::format() const{
	stringstream ss;
	ss << this->dest_line;
	string str = ss.str();
	return "GOSUB <" + str + ">";
}
void Gosub::remDelete(){ }

void Gosub::runCommand(std::map<int,Command*>::iterator &it, 
	std::map<int,Command*>* map, std::stack<int>* log, 
	std::ostream& out){

	std::map<int,Command*>::iterator it_check = map->find(dest_line);
	
	//error catching
	stringstream ss;
	ss << dest_line;
	string str = ss.str();
	if(it_check == map->end()){
		throw out_of_range("GOSUB to non-existent line " + str + ".");
	}

	log->push(it->first);
	it = map->find(dest_line);
}

IfThen::IfThen(BoolExpression* if_bool, int dest_line) : 
	if_bool(if_bool), dest_line(dest_line){
}

IfThen::~IfThen(){ }

string IfThen::format() const{
	stringstream ss;
	ss << this->dest_line;
	string str = ss.str();
	return "IF " + this->if_bool->format() + " THEN <" + str + ">";
}

void IfThen::remDelete(){
	this->if_bool->remDelete();
	delete this->if_bool;
}

void IfThen::runCommand(std::map<int,Command*>::iterator &it, 
	std::map<int,Command*>* map, std::stack<int>* log, 
	std::ostream& out){

	std::map<int,Command*>::iterator it_check = map->find(dest_line);

	if(this->if_bool->boolVal()){

		//error catching (only if bool condition is true)
		stringstream ss;
		ss << dest_line;
		string str = ss.str();
		if(it_check == map->end()){
			throw out_of_range("IF jump to non-existent line " + str + ".");
		}

		it = map->find(dest_line);
	}
	else{ it++;}
}

Return::Return(){ }

Return::~Return(){ }

string Return::format() const{
	string target = "RETURN";
	return target;
}
void Return::remDelete(){ }

void Return::runCommand(std::map<int,Command*>::iterator &it, 
	std::map<int,Command*>* map, std::stack<int>* log, 
	std::ostream& out){

	//error catching, checks if go_sub log is empty
	if(log->size() == 0){
		throw out_of_range("No matching GOSUB for RETURN.");
	}

	it = map->find(log->top());
	log->pop();
	it++;
}

End::End(){ }

End::~End(){ }

string End::format() const{
	string target = "END";
	return target;
}

void End::remDelete(){ }

void End::runCommand(std::map<int,Command*>::iterator &it, 
	std::map<int,Command*>* map, std::stack<int>* log, 
	std::ostream& out){

	it = map->end();
}