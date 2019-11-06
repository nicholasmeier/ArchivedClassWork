#ifndef simplcommand_hh
#define simplecommand_hh

#include <string>
#include <vector>

struct SimpleCommand {

  // Simple command is simply a vector of strings
  //char * SHELL;
  std::vector<std::string *> _arguments;

  SimpleCommand();
  ~SimpleCommand();
  void insertArgument( std::string * argument );
  void print();
  //void setSHELL(char * pathrea);
};

#endif
