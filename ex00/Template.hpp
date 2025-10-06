#pragma once

# include <iostream>

class Template
{
	private:

	public:
		Template();
		Template(const Template &other);
		Template& operator=(const Template &other);
		~Template();
};
