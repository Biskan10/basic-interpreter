#include "interpreter.h"

using namespace std;

Interpreter::Interpreter(std::istream& in) {
	isnextarr = false;
    this->parse(in);
    this->execute(cout);
}

Interpreter::~Interpreter() {
	this->remDelete();
}

void Interpreter::parse(std::istream& in) {
    string line;
    while (getline(in, line)) {

    	string command_name;
        int line_number;
        Command* current_command;

        stringstream stream(line);

        //get line number and command name
        stream >> line_number;
        stream >> command_name;

        this->skipspace(stream);

        string str;
		bool sing_dash = false;
		getline(stream, str);
		stream.clear();

		//puts a space before anand after all symbols for easy parsing
		for(int m=0; m<str.size();m++){
			if((str[m]=='=') || (str[m]=='<') || (str[m]=='>') || 
			(str[m]=='*' ) || (str[m]=='/') || (str[m]=='+') || 
			(str[m]=='-') || (str[m]=='(') || (str[m]==')') || 
			(str[m]=='[') ||(str[m]==']')){

				//checks for double minuses in case of negative numbers
				if(str[m]=='-' && str[m+1]=='-'){
					str.insert(m," ");
					str.insert(m+2," ");
					m+=3;
				}

				//doesn't space out for negative numbers
				else if(str[m]=='-' && isdigit(str[m+1])!=0){
					if(m!=0 && str[m-1]!='-'){
						str.insert(m," ");
						str.insert(m+2," ");
						m++;
					}
					else{
						m++;
					}
				}

				else{
					str.insert(m," ");
					str.insert(m+2," ");
					m++;
				}			
			}
		}

		stream.str(str);

        if(command_name=="PRINT"){
        	current_command = new Print(this->parsenum(stream));
        }
        else if(command_name=="LET"){
        	NumericExpression* exp1;
        	NumericExpression* exp2;

        	exp1 = this->parsenum(stream);
        	exp2 = this->parsenum(stream);

        	//decides whether to read as integer or array variable
			if(isnextarr){
				current_command = new LetArr(exp1, exp2);

				this->isnextarr=false;
			}
			else{
				current_command = new LetInt(exp1, exp2);
			}
        }
        else if(command_name=="IF"){
        	BoolExpression* condition;
        	int dest;

        	condition = this->parsebool(stream);

        	//skips space and the THEN command
        	while(stream.peek()==' ' || isalpha(stream.peek()) != 0){
				stream.seekg(1, ios::cur);
			}

        	stream >> dest;

        	current_command = new IfThen(condition, dest);
        }
		else if(command_name=="GOTO"){
			int dest;
			stream >> dest;
			current_command = new Goto(dest);
		}
		else if(command_name=="GOSUB"){
			int dest;
			stream >> dest;
			current_command = new Gosub(dest);
		}
		else if(command_name=="RETURN"){
			current_command = new Return;
		}
        else if(command_name=="END"){
        	current_command = new End;
        }

        basic_log.insert(std::pair<int,Command*>(line_number,current_command));
    }
}

void Interpreter::skipspace(std::stringstream& ss){

	//checks for whitespace or tabspace
	while(ss.peek()==' ' || ss.peek()=='	'){
		ss.seekg(1, ios::cur);
	}
}

NumericExpression* Interpreter::parsenum(std::stringstream& ss){
	NumericExpression* currexp;

	if(!ss.fail()){
	this->skipspace(ss);

	//decides what type of numeric expression the next character is
	if(ss.peek()=='('){
		currexp = this->parsebin(ss);
	}
	else if(isalpha(ss.peek()) != 0) {
		currexp = this->parsevar(ss);
	}
	else if((isdigit(ss.peek()) != 0) || (ss.peek()=='-')){
		currexp = this->parseconstant(ss);
	}

	this->skipspace(ss);

	return currexp;
	}
}

NumericExpression* Interpreter::parsebin(std::stringstream& ss){
	NumericExpression* one;
    NumericExpression* two;
    char op;

	ss.seekg(1, ios::cur);

    this->skipspace(ss);

	one = this->parsenum(ss);

	//parses operator
	ss >> op;

	this->skipspace(ss);

	two = this->parsenum(ss);

	this->skipspace(ss);

	if(ss.peek()==')'){
		ss.seekg(1, ios::cur);
	}

	NumericExpression* pointer = new BinaryExpression(one, two, op, &var_log);
	return pointer;
}


NumericExpression* Interpreter::parseconstant(std::stringstream& ss){
	int constant;
	ss >> constant;

	this->skipspace(ss);

	NumericExpression* pointer = new Constant(constant);
	return pointer;
}

NumericExpression* Interpreter::parsevar(std::stringstream& ss){
	string Vname;

	//parses variable or array name
	ss >> Vname;
	this->skipspace(ss);

	//if it is an array, read the numeric expression inside
	if(ss.peek()=='['){
		ss.seekg(1, ios::cur);
		this->isnextarr = true;

		this->skipspace(ss);

		NumericExpression* address = this->parsenum(ss);

		this->skipspace(ss);

		//skip last array bracket
		if(ss.peek()==']'){
		ss.seekg(1, ios::cur);
		}

		NumericExpression* pointer =  new ArrayVar(Vname, address, &var_log);
		
		std::map<std::string, std::map<int, int> >::iterator it_check;
		it_check = var_log.find(pointer->getName());

		//if the array to be inserted is new
		if(it_check == var_log.end()){

			std::map<int,int> variable;
			variable.insert(std::pair<int,int>(address->getValue(), 0));

			var_log.insert(std::pair<std::string, std::map<int,int> >(pointer->getName(), variable));

		}
		//if the array to be inserted exists
		else{
			(it_check->second).insert(std::pair<int,int>(address->getValue(), 0));
		}

		return pointer;
	}
	//parse only name
	else{
		NumericExpression* pointer =  new IntVar(Vname, &var_log);

		std::map<int,int> variable;
		variable.insert(std::pair<int,int>(0, 0));
		
		var_log.insert(std::pair<std::string, std::map<int,int> >(pointer->getName(), variable));

		return pointer;
	}

	this->skipspace(ss);
}

BoolExpression* Interpreter::parsebool(std::stringstream& ss){
	NumericExpression* one;
    NumericExpression* two;
    char op;

	this->skipspace(ss);

	one = this->parsenum(ss);

	//parse operator
	ss >> op;

	this->skipspace(ss);

	two = this->parsenum(ss);	

	this->skipspace(ss);

	BoolExpression* pointer = new BoolExpression(one, two, op);
	return pointer;
}

void Interpreter::write(std::ostream& out) {
	//keeps track of all created line objects
	for (it_basic = basic_log.begin(); it_basic != basic_log.end(); ++it_basic) { 
        
        stringstream ss;
		ss << it_basic->first;
		string target = ss.str() + " ";

        out <<  target << it_basic->second->format() << std::endl;
    }

}

void Interpreter::execute(std::ostream& out){
	it_basic = basic_log.begin();

	try{
		while(it_basic!=basic_log.end()){
			Command* current_command = it_basic->second;

			current_command->runCommand(it_basic, &basic_log, 
			&gosub_log, out);
		}
	}
	catch(exception &e){
		out << "Error in Line " << it_basic->first << ": "<< e.what() << endl;
	}    
}

void Interpreter::remDelete(){
	for (it_basic = basic_log.begin(); it_basic != basic_log.end(); ++it_basic) { 
        it_basic->second->remDelete();
        delete it_basic->second;
    }
}