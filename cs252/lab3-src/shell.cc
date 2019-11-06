#include <cstdio>

#include <signal.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include <unistd.h>
#include "shell.hh"

int yyparse(void);
char * pathrel;
char * pathrea;
/*
std::string prompt = "myshell>";

void Shell::setprompt(std::string s){
	prompt = s;
}
*/
void Shell::prompt() {
	if (isatty(0)){
		//rintf("%s", prompt);
		printf("myshell>");
		fflush(stdout);
	}
}

extern "C" void disp(int sig){
	//Whatever the fuck they did for the example \/
	//fprintf( stderr, "\nsig:%d	Ouch!\n", sig);
	
	printf("\n");
}

extern "C" void zombiesMustDie(int sig){
	int zomble;
	pid_t pid;
	while(1) {
		pid = wait3(&zomble, WNOHANG, (struct rusage *)NULL);
		//lastbackid = pid;
		switch(pid){
			case 0:
				return ;
			case -1:
				return ;
			default:
				printf ("%d exited.\n", pid);
		}		
	}
}

int main(int argc, char *argv[]) {
	pathrel = argv[0];
	//pathrea = realpath(pathrel);
	struct sigaction sa1;
	sa1.sa_handler = disp;
	sigemptyset(&sa1.sa_mask);
	sa1.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa1, NULL)){
		perror("sigaction: out of control (c)");
		exit(2);
	}
	
	struct sigaction sa2;
	sa2.sa_handler = zombiesMustDie;
	sigemptyset(&sa2.sa_mask);
	sa1.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa2, NULL)){
		perror("sigaction: the zombies killed you");
		exit(2);
	}

	
		
	Shell::prompt();
	yyparse();
}

Command Shell::_currentCommand;
