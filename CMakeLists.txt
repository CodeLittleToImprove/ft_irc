cmake_minimum_required(VERSION 3.30)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # don't silently upgrade
set(CMAKE_CXX_EXTENSIONS OFF)

# Add strict warnings
add_compile_options(-Wall -Wextra -Werror)

add_executable(ft_irc_server
#        test/Template.cpp
        server/Server.cpp
        server/Server.cpp
        server/Client.cpp
        server/Client.hpp
        server/main.cpp
        server/main.cpp
        parser/ACommand.hpp
        parser/ACommand.cpp
        parser/Cap.cpp
        parser/Invite.cpp
        parser/Join.cpp
        parser/Kick.cpp
        parser/Mode.cpp
        parser/Nick.cpp
        parser/Notice.cpp
        parser/Oper.cpp
        parser/Pass.cpp
        parser/Privmsg.cpp
        parser/Quit.cpp
        parser/Tokenizer.cpp
        parser/Topic.cpp
        parser/User.cpp
        parser/Who.cpp
        utils/utils.hpp
        utils/utils.cpp
        channel/Channel.cpp

        parser/Part.cpp
        parser/Ping.cpp)
