cmake_minimum_required(VERSION 3.30)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # don't silently upgrade
set(CMAKE_CXX_EXTENSIONS OFF)

# Add strict warnings
#add_compile_options(-Wall -Wextra -Werror)


add_executable(ft_irc_server
#        test/Template.cpp
        server/server.cpp
        server/server.cpp)

add_executable(ft_irc_client
        #        test/Template.cpp
        client/client.cpp
        client/client.cpp)