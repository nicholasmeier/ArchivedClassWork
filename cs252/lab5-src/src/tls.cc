/**
 * This file contains your implementation of a TLS socket and socket acceptor. The TLS socket uses
 * the OpenSSL library to handle all socket communication, so you need to configure OpenSSL and use the
 * OpenSSL functions to read/write to the socket. src/tcp.cc is provided for your reference on 
 * Sockets and SocketAdaptors and examples/simple_tls_server.c is provided for your reference on OpenSSL.
 */

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>
#include <sstream>
#include <cstring>
#include <memory>

#include "tls.hh"
#include "errors.hh"

//Reading and Writing --> more like tcp.cc
//initialization and accepting --> more like tls example

TLSSocket::TLSSocket(int port_no, struct sockaddr_in addr, SSL* ssl) : _socket(port_no), _addr(addr), _ssl(ssl) {
	// TODO: Task 2.1
	char inet_pres[INET_ADDRSTRLEN];
	if (inet_ntop(addr.sin_family, &(addr.sin_addr), inet_pres, INET_ADDRSTRLEN)){
		std::cout << "Received a connection from " << inet_pres << std::endl;
	}	
}
TLSSocket::~TLSSocket() noexcept {
	// TODO: Task 2.1
	/*
	std::cout << "Closing TLS socket fd" << _socket;
	char inet_ntop[INET_ADDRSTRLEN];
	if (inet_ntop(_addr.sin_family, &(_addr.sin_addr),inet_ntop, INET_ADDRSTRLEN)){
		std::cout << "from " << inet_ntop;
	}
	std::cout << std::endl;
	close(_socket);*/
}

char TLSSocket::getc() {
	// TODO: Task 2.1
	char c = '\0';
	return c;
}

ssize_t TLSSocket::read(char *buf, size_t buf_len) {
	// TODO: Task 2.1
	ssize_t r = recv(_socket, buf, buf_len, 0);
	if (r == -1) { 
		throw ConnectionError("Unable to read a character: " + std::string(strerror(errno)));
	}
	return r;
}

std::string TLSSocket::readline() {
	std::string str;
	char c;
	while ((c = getc()) != '\n' && c != EOF) {
		str.append(1, c);
	}
	if (c == '\n') {
       		str.append(1, '\n');
	}
	return str;
}

void TLSSocket::write(std::string const &str) {
    write(str.c_str(), str.length());
}

void TLSSocket::write(char const *const buf, const size_t buf_len) {
	//Task 2.1
	if (buf == NULL) {return;}
	int ret_code = send(_socket, buf, buf_len, 0);
	if (ret_code == -1){ 
		throw ConnectionError("Unable to write: " + std::string(strerror(errno)));
	}else if ((size_t)ret_code != buf_len){
		size_t i;
		std::stringstream buf_hex_stream;
		for (i = 0; i < buf_len; i++){
			buf_hex_stream << std::hex << buf[i];	
		}
		throw ConnectionError("Could not write all bytes of: \'" + buf_hex_stream.str() + 
			"\'. Expected " + std::to_string(buf_len) + " but actually sent " + 
			std::to_string(ret_code));
	}
}

TLSSocketAcceptor::TLSSocketAcceptor(const int portno) {
	// TODO: Task 2.1
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portno);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_master_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_master_socket < 0){
		throw ConnectionError("Unable to create socket: " + std::string(strerror(errno)));
	}
}

Socket_t TLSSocketAcceptor::accept_connection() const {
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	int s = accept(_master_socket, (struct sockaddr*)&addr, &addr_len);
	if (s == -1){
		throw ConnectionError("Unable to accept connection: " + std::string(strerror(errno)));
	}
	SSL_CTX *ctx;
	
	//initilize open ssl
	SSL load_error_strings();
	OpenSSL_add_ssl_algorithms();	

	SSL_METHOD *method;
	ctx = SSL_CTX_new(method);	
	if (!ctx){
		throw ConnectionError("Unable to create SSL context: " + std::string(strerror(errno)));
	}
	//configure context
	SSL_CTX_set_ecdh_auto(ctx, 1);
	if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0){
		throw ConnectionError("SSL Error: " + std::string(strerror(errno)));
	}
	if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0){
		throw ConnectionError("SSL Error: " + std::string(strerror(errno)));
	}

	SSL *ssl = SSL_new(ctx);
	SSL_set_fd(ssl, s);
	if (SSL_accept(ssl) <= 0){
		ERR_print_errors_fp(stderr);
	}
	return std::make_unique<TLSSocket>(s, addr, ssl);
	// TODO: Task 2.1
}

TLSSocketAcceptor::~TLSSocketAcceptor() noexcept {
	// TODO: Task 2.1
}
