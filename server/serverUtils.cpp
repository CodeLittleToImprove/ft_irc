#include <iostream>
#include <iomanip> // for setw and setfill

// debug function
// void printEscapedBuffer(const std::string &buffer)
// {
// 	std::cout << "unfiltered buffer (escaped): ";
// 	for (size_t i = 0; i < buffer.size(); ++i)
// 	{
// 		unsigned char c = buffer[i];
// 		if (std::isprint(c))
// 			std::cout << c;
// 		else
// 			std::cout << "\\x"
// 					<< std::hex << std::setw(2) << std::setfill('0') << (int) c
// 					<< std::dec;
// 	}
// 	std::cout << std::endl;
// }
