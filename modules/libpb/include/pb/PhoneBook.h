#pragma once

#include "Contact.h"

#include <map>
#include <mutex>

namespace pb
{

//
// Alias declaration.
//
using ContactId = std::string;
using ContactShPtr = std::shared_ptr<Contact>;
using PhoneBookEntry = std::pair<std::string, std::shared_ptr<pb::Contact>>;

class PhoneBook
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	PhoneBook();

	//
	// Public interface.
	//
public:
	//! Adds new contact and returns her entry in map.
	PhoneBookEntry AddContact(ContactShPtr contact);
	//! Removes contact and returns her entry in map.
	PhoneBookEntry RemoveContact(ContactId id);
	//! Updates contact and returns her entry in map.
	PhoneBookEntry UpdateContact(ContactId id, ContactShPtr contact);
	//! Return list of contacts.
	std::map<ContactId, ContactShPtr> GetContactsInfo() const;
	//! Returns phone book entry with such id.
	PhoneBookEntry GetEntryById(const ContactId& id) const;

	//
	// Private methods.
	//
private:
	//! Generates and returns sha256 key from contacts fields.
	std::string GenerateId(ContactShPtr contact) const;

	//
	// Private data members.
	//
private:
	//! Map which contains contactId and pointer to contact object.
	std::map<ContactId, ContactShPtr> book_;
};

}	// namespace pb