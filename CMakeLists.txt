cmake_minimum_required(VERSION 3.30)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # don't silently upgrade
set(CMAKE_CXX_EXTENSIONS OFF)

# Add strict warnings
#add_compile_options(-Wall -Wextra -Werror)


add_executable(ft_irc_server
#        test/Template.cpp
        server/Server.cpp
        server/Server.cpp
        server/Client.cpp
        server/Client.hpp
        server/utils.cpp
        server/utils.hpp
        server/main.cpp
        server/main.cpp)

add_executable(ft_irc_client
        #        test/Template.cpp
        testClient/TestClient.cpp)