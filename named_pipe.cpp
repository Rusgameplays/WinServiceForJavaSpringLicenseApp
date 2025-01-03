#include "named_pipe.h"
#include "http_client.h"
#include "globals.h"
#include <iostream>
#include <sstream>
#include <string>
#include <nlohmann/json.hpp>
#include <windows.h>

#include "utils.h"

HANDLE CreateNamedPipeInstance() {
    SECURITY_ATTRIBUTES sa = { sizeof(sa) };
    SECURITY_DESCRIPTOR sd;

    if (InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION) &&
        SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE)) {
        sa.lpSecurityDescriptor = &sd;
        }

    return CreateNamedPipe(
        L"\\\\.\\pipe\\MySimpleServicePipe",
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        512, 512, 0, &sa);
}

void HandleClient(HANDLE pipe) {
    char buffer[512];
    DWORD bytesRead, bytesWritten;

    while (ReadFile(pipe, buffer, sizeof(buffer), &bytesRead, NULL)) {
        std::string message(buffer, bytesRead);
        std::cout << "Received: " << message << std::endl;

        if (message.rfind("register", 0) == 0) {
            std::istringstream stream(message);
            std::string command, email, name, password;
            stream >> command >> email >> name >> password;

            nlohmann::json j;
            j["email"] = email;
            j["name"] = name;
            j["password"] = password;

            std::string json = j.dump();
            std::string response = sendRequestToSpring("/auth/reg", json);

            WriteFile(pipe, response.c_str(), response.size(), &bytesWritten, NULL);
        }
        else if (message.rfind("login", 0) == 0) {
            std::istringstream stream(message);
            std::string command, email, password;
            stream >> command >> email >> password;

            nlohmann::json j;
            j["email"] = email;
            j["password"] = password;

            std::string json = j.dump();
            std::string response = sendRequestToSpring("/auth/login", json);

            std::string token = extractToken(response);
            userTokens = token;
            std::string result = "Bearer " + token;
            WriteFile(pipe, result.c_str(), result.size(), &bytesWritten, NULL);
        }
        else if (message.rfind("activate_license", 0) == 0) {
            if (userTokens.empty()) {
                std::string response = "Error: Unauthorized. Please login first.";
                WriteFile(pipe, response.c_str(), response.size(), &bytesWritten, NULL);
                continue;
            }

            std::istringstream stream(message);
            std::string command, key, name, mac;
            long userId;

            stream >> command >> key >> name >> mac >> userId;

            nlohmann::json j;
            j["key"] = key;
            j["name"] = name;
            j["mac"] = mac;
            j["userId"] = userId;

            std::string jsonBody = j.dump();
            std::string response = sendRequestToSpring("/api/activate", jsonBody);

            WriteFile(pipe, response.c_str(), response.size(), &bytesWritten, NULL);
        }
        else {
            std::string response = "Unknown command";
            WriteFile(pipe, response.c_str(), response.size(), &bytesWritten, NULL);
        }
    }

    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);
    std::cout << "Client disconnected." << std::endl;
}
