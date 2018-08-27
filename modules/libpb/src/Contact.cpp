#include "pb/Contact.h"

namespace pb
{

Contact::Contact(
	const std::string& newName,
	const std::string& newSurname,
	const std::string& newBirthday,
	const std::string& newPhoneNumber,
	const std::string& newAddress)
	: name{ newName }
	, surname{ newSurname }
	, birthday{ newBirthday }
	, phoneNumber{ newPhoneNumber }
	, address{ newAddress }
{
}

}	// namespace pb