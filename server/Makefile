# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 16:12:00 by tbui-quo          #+#    #+#              #
#    Updated: 2025/10/20 19:51:18 by pschmunk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler
CXX = c++

# Compiler flags
CXXFLAGS = -Wall -Wextra -Werror #-std=c++98

# Target executable
TARGET = basic_server

# Source files (current + parser directory)
SRC = $(wildcard *.cpp) $(wildcard ../parser/*.cpp) $(wildcard ../utils/*.cpp)

# Object files (replace .cpp with .o)
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Link all object files into final binary
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Compile rule for any .cpp file (works in subdirectories)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJ)

# Full clean (objects + executable)
fclean: clean
	rm -f $(TARGET)

# Rebuild everything
re: fclean all

# Disable built-in rules and suffixes for speed
.SUFFIXES:
.PHONY: all clean fclean re

# for faster compiling use make -j