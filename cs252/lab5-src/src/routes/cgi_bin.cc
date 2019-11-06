#include "http_messages.hh"
#include "misc.hh"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// You could implement your logic for handling /cgi-bin requests here

HttpResponse handle_cgi_bin(const HttpRequest& request) {
	HttpResponse resp;
	resp.http_version = request.http_version;
	bool fileExists = false;
	FILE *file;
	std::size_t found = request.request_uri.find('?');
	std::string temp;
	std::string r_s = "REQUEST_METHOD=" + request.method;
	char * q_s;
	char * method = const_cast<char*>(r_s.c_str());
	std::string f_part = request.request_uri.substr(0, found);
	putenv(method);
	if (found != std::string::npos){
		temp = "QUERY_STRING=" + request.request_uri.substr(found+1, request.request_uri.length()-found-1);
		q_s = const_cast<char*>(temp.c_str());
		putenv(q_s);
			
		/*std::string temp;
		//maybe not right
		for (int i = found; i < request.request_uri.length(), i++){
			char c = request.request_uri.at(i);
			if (c == '&' || i = request.request_uri.length()-1){
				putenv(temp.c_str());
				temp = "";	
			}
			temp.push_back(c);	
		}*/
	}
	std::string f_addr = "http-root-dir" + f_part;
	//change this maybe needs to be fixed not sure
	if (file = fopen(f_addr.c_str(), "r")){
		fclose(file);
		fileExists = true;
	}
	std::string content_type;
	std::string content;
	if (fileExists){
		FILE *in;
		in = popen(const_cast<char*> (f_addr.c_str()), "r");
		char data[1024];
		//fgets(data, 1024, in);
		char c;
		do{
			c = (char)fgetc(in);
			content.push_back(c);
		}while(c != EOF);
		//resp.headers["Content-Type"] = get_content_type(f_addr);
		pclose(in);
		resp.status_code = 200;
		resp.message_body = content;
		resp.headers["Content-Length"] = std::to_string(content.length());
		resp.headers["Content-Type"] = get_content_type(f_addr);
	}else{
		resp.status_code = 404;
		resp.message_body = "Error 404: File Not Found\r\n";
		resp.headers["Content-Length"] = "27";
	}
	return resp;
}
