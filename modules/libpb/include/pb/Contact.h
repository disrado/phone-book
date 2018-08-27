#pragma once

#include <string>

namespace pb
{

class Contact
{
	//
	// Public interface.
	//
public:
	//! Constructor.
	Contact(
		const std::string& name,
		const std::string& surname,
		const std::string& birthday,
		const std::string& phoneNumber,
		const std::string& address);

	//
	// Public data members.
	//
public:
	//! Name of contact.
	std::string name;
	//! Surname of contact.
	std::string surname;
	//! Birthday of contact.
	std::string birthday;
	//! Phone number of contact.
	std::string phoneNumber;
	//! Address of contact.
	std::string address;
};

}	// namespace pb