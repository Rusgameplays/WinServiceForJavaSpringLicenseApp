#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>

std::string sendRequestToSpring(const std::string& endpoint, const std::string& jsonBody);

#endif // HTTP_CLIENT_H
