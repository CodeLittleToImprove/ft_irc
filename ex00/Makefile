# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbui-quo <tbui-quo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 16:12:00 by tbui-quo          #+#    #+#              #
#    Updated: 2025/04/24 17:44:10 by tbui-quo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler
CXX = c++

# Compiler flags
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# Target executable
TARGET = template

# Source files (using wildcard)
SRC = $(wildcard *.cpp)

# Object files (replace .cpp with .o)
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Linking the object file(s) to create the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compiling each source file to object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up object files
clean:
	rm -f $(OBJ)

# Remove both object files and executable
fclean: clean
	rm -f $(TARGET)

# Optional: rebuild everything (full clean + rebuild)
re: fclean all
