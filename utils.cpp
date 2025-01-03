#include "utils.h"

std::string extractToken(const std::string& jsonResponse) {
    std::size_t tokenPos = jsonResponse.find("\"token\":\"");
    if (tokenPos != std::string::npos) {
        tokenPos += 9;
        std::size_t endPos = jsonResponse.find("\"", tokenPos);
        if (endPos != std::string::npos) {
            return jsonResponse.substr(tokenPos, endPos - tokenPos);
        }
    }
    return "";
}
