#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <string.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#include "simpleCommand.hh"

SimpleCommand::SimpleCommand() {
  _arguments = std::vector<std::string *>();
}

SimpleCommand::~SimpleCommand() {
  // iterate over all the arguments and delete them
  for (auto & arg : _arguments) {
    delete arg;
  }
}

//void SimpleCommand::setSHELL(char * pathrea){
	//SHELL = pathrea;
//}

void SimpleCommand::insertArgument( std::string * argument ) {
	//Tilde~ expansion	
	if (argument->at(0) == '~'){
		if (argument->size() > 1){
			std::string temp = *argument;
			temp.erase(0,1);
			*argument = "/homes/";
			argument->append(temp);
		}else{
			*argument = "${HOME}";
		}
	}
	

	char * buffer = "^.*${[^}][^}]*}.*$";
	//cha * expbuf = compile(buffer, 0, 0);
	regex_t r;
	regmatch_t match;
	int re = regcomp( &r, buffer, 0);
	if (re != 0){
		printf("Broke something: regex");	
	}
	while (!(regexec(&r, argument->c_str(), 1, &match, 0))){
		int start = -1;
		int end = -1;
		for (int i = 0; i < argument->length(); i++){
			if ((argument->at(i) == '$') && (argument->at(i+1) == '{')){ start = i; }
			if (argument->at(i) == '}'){
				end = i;
				break ;
			}
		}
		if (start == -1 || end == -1){
			printf("broke something: insetArgs");	
		}
		int ind = end - start - 2;
		std::string enVar = argument->substr(start+2, ind);

		//Handle Special Enviroment Variables
		if (!(strcmp(enVar.c_str(), "?"))){
			int status;
			waitpid(-1, &status, 0);
			int stat = WEXITSTATUS(status);
			std::string temp = std::to_string(stat);
			std::string * tempa = &temp;
			*argument = *tempa;
			break;
		}
		if (!(strcmp(enVar.c_str(), "$"))){
			int pid = getpid();
			std::string temp = std::to_string(pid);
			std::string * tempa = &temp;
			*argument = *tempa;
			break;
		}
		if (!(strcmp(enVar.c_str(), "!"))){
			int back;
			int pid = wait3(&back, WNOHANG, (struct rusage *)NULL);
			std::string temp = std::to_string(pid);
			std::string * tempa = &temp;
			*argument = *tempa;
			break;		
		}
		if (!(strcmp(enVar.c_str(), "_"))){
			*argument = "_";
			break;	
		}
		if (!(strcmp(enVar.c_str(), "SHELL"))){
			//need to make a global argument with this
			//std::string temp = shell.pathrel;
			*argument = "{SHELL}";	
			break;
		}
		char* enviroment = getenv(enVar.c_str()); //Causes segfault in certain things
		std::string temp0 = argument->substr(0, start);
		std::string * temp = &temp0;
		temp->append(enviroment);
		temp->append(argument->substr(end+1, argument->length()));
		*argument = *temp; 
	}
	

	_arguments.push_back(argument);
}

// Print out the simple command
void SimpleCommand::print() {
  for (auto & arg : _arguments) {
    std::cout << "\"" << arg << "\" \t";
  }
  // effectively the same as printf("\n\n");
  std::cout << std::endl;
}
