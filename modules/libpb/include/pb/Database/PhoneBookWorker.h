#pragma once

#include "pb/Contact.h"
#include "pb/Database/DbHandler.h"

namespace pb
{
namespace db
{

//
// Alias declaration.
//
using ContactId = std::string;
using ContactShPtr = std::shared_ptr<Contact>;
using CallBack = int(*)(void*, int, char**, char**);
using Row = std::vector<std::string>;
using Rows = std::vector<Row>;

class PhoneBookWorker
{
	//
	// Construction and destruction.
	//
public:
	// Constructor.
	PhoneBookWorker(DbHandler::ConnectionUnitShPtr cUnitPtr);

	//
	// Public interface.
	//
public:
	//! Inerts contact info to database and local storage.
	void InsertContact(const ContactId& id, ContactShPtr contact) const;
	//! Removes contact info from database and local storage.
	void RemoveContact(const ContactId&) const;
	//! Updates contact info in databse and  and local storage.
	void UpdateContact(const ContactId& id, ContactShPtr contact) const;
	//! Retruns row with such id.
	Row SelectRow(const ContactId& id) const;
	//! Retruns all rows from database.
	Rows SelectAll() const;

	//
	// Private methods.
	//
private:
	// Executes query to database.
	void ExecuteQuery(const std::string& query) const;

	//
	// Private data members.
	//
private:
	// Pointer to connectionUnit
	DbHandler::ConnectionUnitShPtr cUnitPtr_;
};

}	// namespace db
}	// namespace pb