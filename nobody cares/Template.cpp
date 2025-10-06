
#include "Template.hpp"

// Default constructor
Template::Template()
{
	// std::cout << "Default constructor called" << std::endl;
}

// Copy constructor
Template::Template(const Template &other)
	:_atribute(other._atribute)
{
	// std::cout << "Copy constructor called" << std::endl;
}

// Copy Assignment operator overload
Template &Template::operator=(const Template &other)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other)
		this->member = other.member();
	return (*this);
}

// Destructor
Template::~Template(void)
{
	// std::cout << "Destructor called" << std::endl;
}


