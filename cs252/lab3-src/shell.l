
/*
 *
 * CS-252 Fall 2017
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include <cstring>
#include "y.tab.hh"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static  void yyunput (int c,char *buf_ptr  );

void escapeHatch(); //escape chars (probably)

void myunputc(int c) {
	unput(c);
}

extern "C" char * read_line();

int mygetc(FILE * f){ // Edit mode 3.4 Imp.
	static char *p;
	char ch;
	if (!isatty(0)){
		//stdin is not a tty. call the real getc
		return getc(f);
	}
	
	if (p == NULL || *p == 0){
		char * s = read_line();
		p = s;
	}
	ch = *p;
	p++;
	return ch;
		
}
#undef getc
#define getc(f) mygetc(f)

int BUFFER = 1024;

%}

%%

\n {
	return NEWLINE;
}

[ \t] {
  /* Discard spaces and tabs */
}

">" {
	return GREAT;
}

">2" {
	return GREATTWO;
}

">>" {
	return GREATGREAT;
}

"<" {
	return LESS;
}

"|" {
	return PIPE;
}

"&" {
	return AMPERSAND;	
}

">&" {
	return GREATAMPERSAND;
}

">>&" {
	return GREATGREATAMPERSAND;
}



[^ \t\n]*(\\[^\s])[^ \t\n]*  {
	/* Assume that file names have only alpha chars */
	escapeHatch();
	yylval.cpp_string = new std::string(yytext);
	return WORD;

	/*yylval.string_vl = strdup(yytext);
	int len = strlen(yylval.string_val);
	std::string cmd = "";
	for (int i = 0; i < len; i++){
		if (yylval.string_val[i] != '\\'){ cmd.push_back(yylval.string_val[i]); }
	}
	strcpy(yylval.string_val, cmd.c_str());
	return WORD;*/
}

[^ (\|)(>>)(>)(>&)(>>&)(<)(&)\t\n][^ (\|)(>>)(>)(>&)(>>&)(&)(<)\t\n]* {
		yylval.cpp_string = new std::string(yytext);
		return WORD;
	}

\"[^\n\"]*\" {
	yylval.string_val = strdup(yytext);
	int len = strlen(yylval.string_val);
	std::string cmd = "";
	for (int i; i < len; i++){
		if(yylval.string_val[i] != '\"'){ cmd.push_back(yylval.string_val[i]); }
	}
	strcpy(yylval.string_val, cmd.c_str());
	return WORD;
	/*
	char* qq = yytext;
	qq++;
	//yytext = qq;
	strcpy(yytext, qq);
	yytext[strlen(yytext)-1] = '\0';
	yylval.string_val = strdup(yytext);
	return WORD;
	*/
}


\$"("[^\n]*")" { //subshell fuckery
	std::string cmd = strdup(yytext);
	cmd.pop_back();
	cmd.erase(0,2);
	int pin[2];
	int pout[2];
	int defaultin = dup(0);
	int defaultout = dup(1);
	
	char c = 0;	
	std::string output = "";	

	pipe(pin);
	pipe(pout);
	/*int flags = fcntl(pout[0], F_GETFL);
	fcntl(pout[0], F_SETFL, flags | O_NONBLOCK);
	dup2(pin[1], 1);
	close(pin[1]);
	printf("%s\nexit_nomsg\n", cmd.c_str());
	*/
	int ret = fork();
	
	/*
	if (!ret){
		dup2(pin[0], 0);
		dup2(pout[1], 1);
		close(pin[0]);
		close(pout[1]); 
	}else{
		close(pin[1]);
		waitpid(ret, NULL, 0);
		//close(pin[1]);
		int count = 0;
		while(read(pout[0], &c, 1) > 0){
			if (c == '\n'){c = ' ';}
			output.push_back(c);
		}
		close(pout[1]);

		dup2(defaultin, 0);
		dup2(defaultout, 1);
		close(defaultin);
		close(defaultout);
		prctl(PR_SET_PDEATHSIG, SIGHUP);
		for (int i = output.size()-1; i > -1; i--){
			unput(output.at(i));
		}
	}*/
	
	if (!(ret)){
		write(pin[1], cmd.c_str(), strlen(cmd.c_str()));
		write(pin[1], "\nexit_nomsg\n", 12); 
		close(pin[1]);
		close(pout[0]);
		dup2(pin[0], 0);
		dup2(pout[1], 1);
	}else{
		close(pin[1]);
		waitpid(ret, NULL, 0);
		close(pout[1]);
		close(pin[0]);
		read(pout[0], &c, 1);
		output.push_back(c);
		while(read(pout[0], &c, 1) > 0){
			output.push_back(c);
		}
		close(pout[0]);
		dup2(defaultin, 0);
		dup2(defaultout, 1);
		for (int i = output.size()-1; i > -1; i--){
			if (output.at(i) == '\n'){ output.at(i) = ' '; }
			unput(output.at(i));
		}	
	}
	
}

%%

void escapeHatch(){
	bool opened = false;
	char* asdf = (char*)malloc(strlen(yytext)+1);
	int y = 0;
	
	for(int i = 0; i < strlen(yytext); i++){
		if(yytext[i] == '\\' && opened){
			opened = true;
		}else if(opened){
			asdf[y] = yytext[i];
			y++;
			opened = false;
		}else{
			asdf[y] = yytext[i];
			y++;	
		}
	}
	asdf[y] = '\0';
	strcpy(yytext, asdf);
	free(asdf);
}


