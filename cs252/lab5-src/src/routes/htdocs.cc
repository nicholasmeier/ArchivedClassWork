#include "http_messages.hh"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "misc.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

//namespace fs = std::experimental::filesystem;

HttpResponse handle_htdocs(const HttpRequest& request) {
	HttpResponse resp;
	resp.http_version = request.http_version;
	bool fileExists = false;
	bool browseDir = false;
	FILE *file;
	std::string f_addr = "http-root-dir/htdocs" + request.request_uri;
	
	struct stat buf;
	stat(f_addr.c_str(), &buf);

	if (request.request_uri.compare("/") == 0){
		f_addr = "http-root-dir/htdocs/index.html";
	}else if (request.request_uri.at(request.request_uri.length()-1) == '/'){
		browseDir = true;
	}else if (S_ISDIR(buf.st_mode)){
		f_addr += "/index.html";
	}
	if (file = fopen(f_addr.c_str(), "r")){
		fclose(file);
		fileExists = true;	
	}
	//Use for later (maybe cgi implementation
	//if (!fileExists){ }
	
	std::string content_type;
	std::string content;
	if (fileExists){
		if (browseDir){
			DIR * d = opendir(f_addr.c_str());
			if (NULL == d){
				//error	
			}
			for (dirent * ent = readdir(d); NULL != ent; ent = readdir(d)){
				std::string t = ent->d_name;
				content += t + "\r\n";
			}
			resp.message_body = content;
			resp.headers["Content-Length"] = std::to_string(content.length());
			resp.status_code = 200;
		}else{
			resp.headers["Content-Type"] = get_content_type(f_addr); 
			std::string line;
			std::ifstream f(f_addr);
			if (f.is_open()){
				while (getline(f, line)){
					content += line + "\n";
				}
				f.close();
			}
			resp.message_body = content;
			resp.headers["Content-Length"] = std::to_string(content.length());
			resp.status_code = 200;
		}
	}else{
		resp.status_code = 404;
		resp.message_body = "Error 404: File Not Found\r\n";
		resp.headers["Content-Length"] = "27";
	}
	
	// Get the request URI, verify the file exists and serve it
	
	return resp;
}
