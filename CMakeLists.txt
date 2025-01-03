cmake_minimum_required(VERSION 3.29)
project(winService)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/solod/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(Boost REQUIRED COMPONENTS asio beast)
find_package(nlohmann_json 3.10.0 REQUIRED)

set(SOURCE_FILES
        main.cpp
        service.cpp
        named_pipe.cpp
        http_client.cpp
        utils.cpp
        globals.cpp
)

set(HEADER_FILES
        service.h
        named_pipe.h
        http_client.h
        utils.h
        globals.h
)

add_executable(winService ${SOURCE_FILES} ${HEADER_FILES})

target_compile_definitions(winService PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0601)

target_link_libraries(winService Boost::asio Boost::beast nlohmann_json::nlohmann_json)
