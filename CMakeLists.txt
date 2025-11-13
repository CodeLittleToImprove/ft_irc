cmake_minimum_required(VERSION 3.30)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # don't silently upgrade
set(CMAKE_CXX_EXTENSIONS OFF)

# Add strict warnings
add_compile_options(-Wall -Wextra -Werror)

add_executable(ft_irc_server
#        test/Template.cpp
        channel/Channel.cpp
        server/Server.cpp
        server/Server.cpp
        server/Client.cpp
        server/Client.hpp
        server/main.cpp
        server/main.cpp
        commands/ACommand.hpp
        commands/ACommand.cpp
        commands/Cap.cpp
        commands/Invite.cpp
        commands/Join.cpp
        commands/Kick.cpp
        commands/Mode.cpp
        commands/Nick.cpp
        commands/Notice.cpp
        commands/Oper.cpp
        commands/Part.cpp
        commands/Pass.cpp
        commands/Ping.cpp
        commands/Privmsg.cpp
        commands/Quit.cpp
        parser/Tokenizer.cpp
        commands/Topic.cpp
        commands/Unoper.cpp
        commands/User.cpp
        commands/Who.cpp
        utils/utils.hpp
        utils/utils.cpp
        )
