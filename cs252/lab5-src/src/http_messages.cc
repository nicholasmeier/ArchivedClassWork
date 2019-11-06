#include "http_messages.hh"
#include <map>
#include <iostream>

// You may find this map helpful. You can implement HttpResponse::to_string() such that
// if no reason_phrase is set, then you try looking up a default_status_reason in this
// std::map (dictionary). These codes are copied from RFC2616 Sec. 6.1.1
const std::map<const int, const std::string> default_status_reasons = {
    {100, "Continue"}, {101, "Switching Protocols"},
    {200, "OK"}, {201, "Created"}, {202, "Accepted"}, {203, "Non-Authoritative Information"},
    {204, "No Content"}, {205, "Reset Content"}, {206, "Partial Content"},
    {300, "Multiple Choices"}, {301, "Moved Permanently"}, {302, "Found"}, {303, "See Other"},
    {304, "Not Modified"}, {305, "Use Proxy"}, {307,  "Temporary Redirect"}, {400, "Bad Request"},
    {401, "Unauthorized"}, {402, "Payment Required"}, {403, "Forbidden"}, {404, "Not Found"},
    {405, "Method Not Allowed"}, {406, "Not Acceptable"}, {407, "Proxy Authentication Required"},
    {408, "Request Time-out"}, {409, "Conflict"}, {410, "Gone"}, {411, "Length Required"},
    {412, "Precondition Failed"}, {413, "Request Entity Too Large"},
    {414, "Request-URI Too Large"}, {415,  "Unsupported Media Type"},
    {416, "Requested range not satisfiable"}, {417, "Expectation Failed"},
    {500, "Internal Server Error"}, {501, "Not Implemented"}, {502, "Bad Gateway"},
    {503, "Service Unavailable"}, {504, "Gateway Time-out"}, {505, "HTTP Version not supported"}
};

std::string HttpResponse::to_string() const {
	std::stringstream ss;
	// Look at RFC 2616 Section 6 for details on how a response message looks:
	// https://tools.ietf.org/html/rfc2616#section-6
	ss << http_version << " " << this->status_code << " " << default_status_reasons.at(this->status_code) << "\r\n";
	//std::map<std::string, std::string>::iterator it = headers.begin();
	for (std::map<std::string, std::string>::const_iterator it=headers.begin(); it!=headers.end(); ++it){
		ss << it->first << " : " << it->second << "\r\n";
	}
	ss << "\r\n";
	ss << this->message_body;
	return ss.str();
}

void HttpRequest::print() const {
    // Magic string to help with autograder
    std::cout << "\\\\==////REQ\\\\\\\\==////" << std::endl;

    std::cout << "Method: {" << method << "}" << std::endl;
    std::cout << "Request URI: {" << request_uri << "}" << std::endl;
    //TODO find out what a query string is
    std::cout << "Query string: {" << query << "}" << std::endl;
    std::cout << "HTTP Version: {" << http_version << "}" << std::endl;

    std::cout << "Headers: " << std::endl;
    for (auto kvp=headers.begin(); kvp != headers.end(); kvp++) {
        std::cout << "field-name: " << kvp->first << "; field-value: " << kvp->second << std::endl;
    }

    std::cout << "Message body length: " << message_body.length() << std::endl <<
      message_body << std::endl;

    // Magic string to help with autograder
    std::cout << "//==\\\\\\\\REQ////==\\\\" << std::endl;
}
