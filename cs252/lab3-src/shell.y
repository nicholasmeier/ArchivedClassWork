
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 * WHY DO YOU FUCKING TAS KEEP USING DOUBLE SPACE TO MAKE THIS SHIT USE A GOD DAMN TAB FOR FUCKS SAKE
 * you must extend it to understand the complete shell grammar
 *
 */

%code requires 
{
#include <string>

#if __cplusplus > 199711L
#define register      // Deprecated in C++11 so remove the keyword
#endif
}

%union
{
  char        *string_val;
  // Example of using a c++ type in yacc
  std::string *cpp_string;
}

%token <cpp_string> WORD
%token NOTOKEN GREAT NEWLINE GREATGREAT LESS PIPE AMPERSAND GREATAMPERSAND GREATGREATAMPERSAND GREATTWO

%{
//#define yylex yylex
#include <cstdio>
#include <string.h>
#include <stdio.h>
#include "shell.hh"

void yyerror(const char * s);
void expandWildCards(std::string * c);
int yylex();

%}

%%

goal:
	commands
	;

commands:
	command
	| commands command
	;

command: simple_command
	;


simple_command:
	command_and_args NEWLINE {
		//printf("Yacc: Execute command\n");
		Shell::_currentCommand.execute();
	}
	| NEWLINE
	| error NEWLINE { yyerrok; }
	;


simple_command:	
	pipe_list io_modifier_list background_optional NEWLINE {
		/*printf("   Yacc: Execute command\n");*/
		Shell::_currentCommand.execute();
	}
	| NEWLINE 
	| error NEWLINE { yyerrok; }
	;

pipe_list:
/*	pipe_list PIPE command_and_args*/
	command_and_args PIPE pipe_list;
	| command_and_args
	;

command_and_args:
	command_word argument_list {
		Shell::_currentCommand.insertSimpleCommand( Command::_currentSimpleCommand );
  	}
	;

argument_list:
	argument_list argument
	| /* can be empty */
	;

argument:
	WORD {
		/*printf("   Yacc: insert argument \"%s\"\n", $1->c_str());*/
		//Command::_currentSimpleCommand = new SimpleCommand();
		Command::_currentSimpleCommand->insertArgument( $1 );
		/*if (strchr($1->c_str(), '*') == NULL && strchr($1->c_str(), '?') == NULL){
			Command::_currentSimpleCommand->insertArgument($1);
		}else{
			expandWildCards($1);
		}*/
	}
	;

command_word:
	WORD {
		/*printf("   Yacc: insert command \"%s\"\n", $1->c_str());*/
		Command::_currentSimpleCommand = new SimpleCommand();
		Command::_currentSimpleCommand->insertArgument( $1 );
	}
	;

iomodifier_opt:
	GREAT WORD {
	/*printf("   Yacc: insert output \"%s\"\n", $2->c_str());*/
		if (Shell::_currentCommand._outFile != NULL){
      			yyerror("Ambiguous output redirect.\n");
		}
		Shell::_currentCommand._outFile = $2;
	}
	| GREATTWO WORD {
		Shell::_currentCommand._errFile = $2;
	}
	| GREATGREAT WORD {
		Shell::_currentCommand._outFile = $2;
		Shell::_currentCommand._append = true;
	}
	| GREATGREATAMPERSAND WORD {
		Shell::_currentCommand._errFile = $2;
		Shell::_currentCommand._outFile = $2;
		Shell::_currentCommand._append = true;
	}
	| GREATAMPERSAND WORD{
		Shell::_currentCommand._errFile = $2;
		Shell::_currentCommand._outFile = $2;
	}
	| LESS WORD {
		Shell::_currentCommand._inFile = $2;
	}
	;

io_modifier_list:
	io_modifier_list iomodifier_opt
	| /*//iomodifier_opt;*/
	;

background_optional:
	AMPERSAND {
		Shell::_currentCommand._background = true;
	}
	|
	;

/*
command_line:
  pipe_list io_modifier_list background_optional NEWLINE
  | NEWLINE  
  | error NEWLINE { yyerrok; }
  ;


command_list:
  command_line
  | command_list command_line
  ;
*/
%%

/*
void expandWildCards(std::string * c){
	bool dirs = false;
	if(c->at(0) == '/'){
		dirs = true;
	}
	std::string * backup = *c; 
}*/

void
yyerror(const char * s)
{
  //Shell::_currentCommand._yacc_Err = true;
  fprintf(stderr,"%s", s);
}

#if 0
main()
{
	yyparse();
}
#endif
