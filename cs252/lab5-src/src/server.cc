/**
 * This file contains the primary logic for your server. It is responsible for
 * handling socket communication - parsing HTTP requests and sending HTTP responses
 * to the client. 
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <thread>
#include <mutex>

#include "server.hh"
#include "http_messages.hh"
#include "errors.hh"
#include "misc.hh"
#include "routes.hh"


std::string AUTH =  "dXNlcjpwYXNz";
std::mutex mtx;	
//std::string AUTH = "base64code";

Server::Server(SocketAcceptor const& acceptor) : _acceptor(acceptor) {}

void Server::run_linear() const {
	//What they had and idk what I need to do with it
	while (1) {
		//a pointer to a socket accepts a connection
		Socket_t sock = _acceptor.accept_connection(); 
		handle(sock);
	}
}

void Server::run_fork() const {
	//Task 1.4
	//TODO Look at how the handle(sock) method works and see what we have to do
	//Used if the server is started with -f 
	//Handle each request in a new process
	//After accepting connection off of the socket acceptor, call fork and execute norm in child
	while(1){
		Socket_t sock = _acceptor.accept_connection();
		pid_t p = fork();
		if (p == 0){
			handle(sock);
			exit(0);
		}
		waitpid(p, NULL, 0);
	}
}

void Server::run_thread() const {
	//Task 1.4
	//Used if the server is started with -t
	//Handle each request in a new thread
	//Accept connection off socket acceptor, create a new thread and execute norm within child
	//use std::thread or std::pthread
	while(1){
		//accept socket;
		//std::thread t1(foo, args) --> t1(&Server::handle, std::move(socket));  
		//t1.detach();
		/*pthread_t t;
		pthread_create(&t, NULL, (void*(*)(void*))handle(), (void*)sock);
		*/
		
		mtx.lock();
		Socket_t sock = _acceptor.accept_connection();
		mtx.unlock();
		std::thread t (&Server::handle, this, std::move(sock));
		//std::thread t = (&Server::handle_thread, this);
		t.detach();
	}
}

void Server::handle_thread() const {
	//helper method for thread pools
	while(1){
		mtx.lock();
		Socket_t sock = _acceptor.accept_connection();
		mtx.unlock();
		handle(sock);
	}
}

void Server::run_thread_pool(const int num_threads) const {
	//Task 1.4
	//Used if the server is started with -p
	//handle each request in a pool of n workers
	//using n+1 threads during execution
	//thread that starts prog should be used to create the n workers and wait for finish
	//each worker is in infinite loop --> they will only finish when program ends or error occurs
	std::thread t_pool[num_threads];
	for (int i = 0; i < num_threads; i++){
		t_pool[i] = std::thread(&Server::handle_thread, this);
	}
	for (int i = 0; i < num_threads; i++){
		t_pool[i].join();
	}
}


/*//Used if you wanna make a map for handling file paths
std::vector<Route_t> route_map = {
  std::make_pair("/cgi-bin", handle_cgi_bin),
  std::make_pair("/", handle_htdocs),
  std::make_pair("", handle_default)
};
*/


void Server::handle(const Socket_t& sock) const {
	HttpRequest request;
	// instr recommendation:
	// void parse_request(const Socket_t& sock, HttpRequest* const request);
	std::string req_line;
	std::vector<std::string> reqs;
	while(req_line.compare( "\r\n" ) != 0){
		req_line = sock->readline();
		std::cout << req_line << std::endl;
		reqs.push_back(req_line);
	}
	bool authorized = false;
	std::string temp = "";
	int p_i = 0;
	for (int i = 0; i < reqs.at(0).length()-1; i++){
		char c = reqs.at(0).at(i);
		if (c == ' ' || i == reqs.at(0).length()-2){
			if (p_i == 0) { request.method = temp; }
			if (p_i == 1) { request.request_uri = temp; }
			if (p_i == 2) {	request.http_version = temp; }
			temp = "";
			p_i++;
		}else{ temp.push_back(c); }
	}
	
	for (int i = 0; i < reqs.size(); i++){
		std::string l = reqs.at(i);
		if ((l.length() >= 23) && (l.substr(0, 21).compare("Authorization: Basic ") == 0)){
			if ((l.substr(21, l.length()-23)).compare(AUTH) == 0) {
				authorized = true;
			}
		}
		std::size_t found = l.find(':');
		if (found != std::string::npos){
			request.headers[l.substr(0,found)] = l.substr(found+2, l.length()-found-1);
		}
	} 
	
	
	//need to read the string and see what is needed
	request.print();

	HttpResponse resp;
	//default response values
	int defaultStatusCode = 403;
	std::string defaultConnection = "close";
	std::string defaultMessage = "No message\r\n";
	int defaultContentLength = defaultMessage.length();
	std::string basicRealm="Basic realm= \"nmeier's httpd-cs252\"";
	
	//handles authorization and default settings
	resp.http_version = request.http_version;
	if (!authorized){
		resp.status_code = 401;
		resp.headers["WWW-Authenticate"] = basicRealm;
	}else{
		resp.status_code = defaultStatusCode;
		resp.headers["Connection"] = defaultConnection;
		resp.message_body = defaultMessage;
		resp.headers["Content-Length"] = std::to_string(defaultMessage.length());
	
		//handles unique hello message
		if (request.request_uri.compare("/hello") == 0){
			resp.status_code = 200;
			resp.message_body = "Hello Traveler!\r\n";
			resp.headers["Content-Length"] = "17";
		}

		//handles Simple GET requests
		if (request.method.compare("GET") == 0){
			if ((request.request_uri.length() > 9) && ((request.request_uri.substr(0,9)).compare("/cgi-bin/") == 0)){
				resp = handle_cgi_bin(request);	
			}else{ 
				//handle normal GET (1.3)
				resp = handle_htdocs(request);
			}
		}
	}	
	//send that shit out boi
	//NOTE:: running the server on a ssh-ed windows device lags the server output too much
	std::cout << "Start of HTTP response{ " << std::endl;
	std::cout << resp.to_string() << "}" << std::endl;
	sock->write(resp.to_string());
}
