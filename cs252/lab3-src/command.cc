/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "command.hh"
#include "shell.hh"

std::string lastCommand = "";

Command::Command() {
    // Initialize a new vector of Simple Commands
	_simpleCommands = std::vector<SimpleCommand *>();

	_outFile = NULL;
	_inFile = NULL;
	_errFile = NULL;
	_background = false;
	_append = false;
	_yacc_Err = false;
}

void Command::insertSimpleCommand( SimpleCommand * simpleCommand ) {
	// add the simple command to the vector
	_simpleCommands.push_back(simpleCommand);
}

void Command::clear() {
	// deallocate all the simple commands in the command vector
	for (auto simpleCommand : _simpleCommands) {
	       delete simpleCommand;
	}
	// remove all references to the simple commands we've deallocated
	// (basically just sets the size to 0)
	_simpleCommands.clear();

	if ( _outFile ) {
        	delete _outFile;
	}
	_outFile = NULL;

	if ( _inFile ) {
        	delete _inFile;
	}
	_inFile = NULL;

	if ( _errFile ) {
        	delete _errFile;
	}
	_errFile = NULL;

	_background = false;
	_append = false;
	_yacc_Err = false;
}

void Command::print() {
    printf("\n\n");
    printf("              COMMAND TABLE                \n");
    printf("\n");
    printf("  #   Simple Commands\n");
    printf("  --- ----------------------------------------------------------\n");

    int i = 0;
    
    // iterate over the simple commands and print them nicely
    for ( auto & simpleCommand : _simpleCommands ) {
        printf("  %-3d ", i++ );
        simpleCommand->print();
    }
    
    printf( "\n\n" );
    printf( "  Output       Input        Error        Background\n" );
    printf( "  ------------ ------------ ------------ ------------\n" );
    printf( "  %-12s %-12s %-12s %-12s\n",
            _outFile?_outFile->c_str():"default",
            _inFile?_inFile->c_str():"default",
            _errFile?_errFile->c_str():"default",
            _background?"YES":"NO");
    printf( "\n\n" ); 
}

void Command::execute() {
	// Don't do anything if there are no simple commands
	if ( _simpleCommands.size() == 0 ) {
		Shell::prompt();
		return;
	}
	
	/*if (_yacc_Err){
		clear();
		return;
	}*/
	//printf("stillRunning\n");
	
	int defaultin = dup( 0 );
	int defaultout = dup( 1 );
	int defaulterr = dup( 2 );

	//print(); //printing shit

	
	//Handle builtin Commands

	//exit
	if (!(strcmp((_simpleCommands.at(0)->_arguments.at(0))->c_str(), "exit"))){
		//printf("\nSee you space cowboy...\n\n");
		printf("\nGood bye!!\n\n");
		exit(0);
	}
	
	if (!strcmp((_simpleCommands.at(0)->_arguments.at(0))->c_str(), "exit_nomsg")){
		exit(0);
	}
	
	//int errfd;
	/*
	if (_errFile != NULL){
		if (_append){
			errfd = open(_errFile->c_str(), O_WRONLY|O_APPEND|O_CREAT,0060);
		}else{
			errfd = open(_errFile->c_str(), O_CREAT|O_WRONLY|O_TRUNC, 0600);
		}
		if (errfd < 0){
			printf("broke something erroroutput ");
			printf("%s\n", _errFile->c_str());
		}
	}else{
		errfd = dup(defaulterr);
	}	
	dup2(errfd,2);
	close(errfd);
	*/

	
	//cd <dir>
	if (!(strcmp((_simpleCommands.at(0)->_arguments.at(0))->c_str(), "cd"))){
	
		int ret = -1;
		if (_simpleCommands.at(0)->_arguments.size() == 1){
			ret = chdir(getenv("HOME"));
		}else{
			if (!strcmp(_simpleCommands.at(0)->_arguments.at(1)->c_str(), "${HOME}")){
				ret = chdir(getenv("HOME"));
			}else{
				ret = chdir(_simpleCommands.at(0)->_arguments.at(1)->c_str());
			}
			
		}
		if (ret < 0) {
			printf("cd: can't cd to notfound\n");
		}
		clear();
		return ;
	}
	
	//printenv --> moved because pipes are cocks
	/*
	if (!(strcmp((_simpleCommands.at(0)->_arguments.at(0))->c_str(), "printenv"))){
		int ret = -1;
		if (_simpleCommands.at(0)->_arguments.size() >= 1){
			printf("maybe broke: printenv");
		}else{
			
		}
	}*/

	//setenv <A> <B> 
	if (!(strcmp((_simpleCommands.at(0)->_arguments.at(0))->c_str(), "setenv"))){
		int ret = -1;
		if (_simpleCommands.at(0)->_arguments.size() != 3){
			printf("setenv: requires vars A and B");
		}else{
			ret = setenv(_simpleCommands.at(0)->_arguments.at(1)->c_str(),
				_simpleCommands.at(0)->_arguments.at(2)->c_str(), 1);
		}
		if (ret < 0){
			printf("something broke: setenv");
		}
		clear();
		return ;
	}
	
	//unsetenv <A>
	if (!(strcmp((_simpleCommands.at(0)->_arguments.at(0))->c_str(), "unsetenv"))){
		int ret = -1;
		if (_simpleCommands.at(0)->_arguments.size() != 2){
			printf("unsetenv: requires var A");
		}else{
			ret = unsetenv(_simpleCommands.at(0)->_arguments.at(1)->c_str());
		}
		if (ret < 0){
			printf("something broke: unsetenv");
		}
		clear();
		return ;
	}
	
	int infd;
	int outfd;
	int errfd;
	if (_inFile != NULL){
		infd = open(_inFile->c_str(), O_RDONLY);
		if (infd < 0){
			printf("broke something input");
			printf("%s\n", _inFile->c_str());
		}
	}else{
		infd = dup(defaultin);
	}
	

	// Add execution here
	// For every simple command fork a new process
	// Setup i/o redirection
	// and call exec
	int loopindex = 0;
	//int pipefd[2];
	for ( auto & simpleCommand : _simpleCommands ){
		dup2(infd,0);
		close(infd);
		
		/*
		int yc = 0;
		bool hasQuote = false;
		for (int y = 0; y < simpleCommand->_arguments.size(); y++ ){
			if ((simpleCommand->_arguments[y])->at(0) == '"'){
				hasQuote = true;
			}
			if ((simpleCommand->_arguments[y])->back() == '"'){
				hasQuote = false;
			}
			if (hasQuote){ yc++; }
		}*/

		//char * ptr[simpleCommand->_arguments.size()+1-yc];
		char * ptr[simpleCommand->_arguments.size()+1];		

		//Put the arguments in array format
		for (int z = 0; z < simpleCommand->_arguments.size(); z++ ){
			/*if ((simpleCommand->_arguments[z])->at(0) == '"'){ //handle quotes
				for (int zs = z+1; zs < simpleCommand->_arguments.size(); zs++){
					if ((simpleCommand->_arguments[zs]->back() == '"')){
						std::string s = "";
						for(int zz = z; zz <= zs; zz++){
							s.append(simpleCommand->_arguments[zz]->c_str());
							if (zz != zs) { s.push_back(' '); }
						}
						s.erase(0,1);
						s.pop_back();
						ptr[z] = const_cast<char *> (s.c_str());
						z = zs+1;
						break ;
					}
				}
			}else{*/
				/*
				for (int zc = 0; zc < simpleCommand->_argument[z]->size(); zc++){
					if (simpleCommand->_argument[z]->at(zc) == '\\'){
						simpleCommand->_argument[z]->push_back('"');
						simpleCommand->_argument[z]->insert(0, "\"");
						if (simpleCommand->_argument[z]->at(zc+1) == '\\'){
							simpleCommand->_argument[z]->erase(zc,2);
						}else{ }
					} 
				}*/
			if (!strcmp(simpleCommand->_arguments.at(z)->c_str(), "{SHELL}")){
				ptr[z] = const_cast<char *> (realpath("../shell", NULL));
			}else{
				if (!strcmp(simpleCommand->_arguments.at(z)->c_str(), "_")){
					if (strcmp(lastCommand.c_str(), "") != 0){
						ptr[z] = const_cast<char *>(lastCommand.c_str());
					}
				}else{	
					ptr[z] = const_cast<char *> (simpleCommand->_arguments.at(z)->c_str());
				}
			}
			//}
		}
		//ptr[simpleCommand->_arguments.size()-yc] = NULL;
		ptr[simpleCommand->_arguments.size()] = NULL;		

		//setup output
		if (loopindex == _simpleCommands.size()-1){
			if (_outFile != NULL){
				if (_append){
				        outfd = open(_outFile->c_str(), O_WRONLY|O_APPEND|O_CREAT, 0600);	
				}else{
					outfd = open(_outFile->c_str(), O_CREAT|O_WRONLY|O_TRUNC, 0600);
				}
				if (outfd < 0){
					printf("broke something output ");
					printf("%s\n", _outFile->c_str());
				}
				//dup2(outfd,1); 
				//close(outfd);
			}else{
				//if(_simpleCommands.size() > 1) infd = pipefd[0];
				//no output redirect, no pipes
				outfd = dup(defaultout);
			}
			if (_errFile != NULL){
				if (_append){
					errfd = open(_errFile->c_str(), O_WRONLY|O_APPEND|O_CREAT,0060);
				}else{
					errfd = open(_errFile->c_str(), O_CREAT|O_WRONLY|O_TRUNC, 0600);
				}
				if (errfd < 0){
					printf("broke something erroroutput ");
					printf("%s\n", _errFile->c_str());
				}
				//dup2(errfd, 2);
				//close(errfd);
			}else{
				errfd = dup(defaulterr);
			}
		}else{
			//pipes, there are more commands
			//printf("Pipes\n");
			if(_simpleCommands.size() > 1){
				int pipefd[2];
				pipe(pipefd);
				outfd = pipefd[1];
				infd = pipefd[0];
			}
			//dup2(outfd,1);
			//close(outfd);
		}
		
		dup2(outfd,1);
		close(outfd);
		
		dup2(errfd,2);
		close(errfd);
		
		int ret = fork();
		if (ret == 0){
			//Handle printenv here because pipes?
			
			if (!(strcmp((_simpleCommands.at(0)->_arguments.at(0))->c_str(), "printenv"))){
				//char*n = (_simpleCommands.at(0)->_arguments.at(0))->c_str() ? 
				char **envc = environ;
				while (*envc != NULL){
					printf("%s\n", *envc);
					envc++;
				}
				exit(0);
			}else{
				if(loopindex == _simpleCommands.size()-1){
					//printf("last\n");
				}

				execvp(ptr[0], ptr);
			
				//execvp is not suppose to return if it does then
				perror("execvp");
				exit(1);
			}

		}else if(ret < 0) {
			perror("fork");
			return;
		}
		loopindex++;
		//printf("%d\n", loopindex);
	
		if (!_background){
			//if not a background task(?) then wait for them
			waitpid(ret, NULL, 0);
		}	
	}//end of loop
	
	//restore input/output/error
	dup2( defaultin, 0 );
	dup2( defaultout, 1 );
	dup2( defaulterr, 2 );
	
	//close those bitches
	close( defaultin );
	close ( defaultout );
	close ( defaulterr );	
	
	lastCommand = "";
	int li = _simpleCommands.at(0)->_arguments.size()-1;
	lastCommand.append(_simpleCommands.at(0)->_arguments.at(li)->c_str());

	// Clear to prepare for next command
	clear();
	
	// Print new prompt
	Shell::prompt();
}

SimpleCommand * Command::_currentSimpleCommand;
