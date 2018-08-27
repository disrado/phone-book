#include "pb/Database/PhoneBookWorker.h"

#include <fmt/format.h>

#include <boost/exception/all.hpp>

#define THROW_LOGIC_ERROR(arg) BOOST_THROW_EXCEPTION(std::logic_error{ arg })

namespace pb
{
namespace db
{
namespace details
{

int SelectAllCallback(void* data, int numFields, char** fields, char** colNames)
{
	Rows* rows{ static_cast<Rows*>(data) };

	rows->emplace_back(fields, fields + numFields);
	
	return 0;
}

int SelectRowCallback(void* data, int numFields, char** fields, char** colNames)
{
	Row* row{ static_cast<Row*>(data) };

	for (size_t i{ 0 }; i < numFields; ++i) {
		row->push_back(fields[i]);
	}
	
	return 0;
}

}	// namespace details

PhoneBookWorker::PhoneBookWorker(DbHandler::ConnectionUnitShPtr cUnitPtr)
	: cUnitPtr_{ cUnitPtr }
{
}

void PhoneBookWorker::InsertContact(const ContactId& id, ContactShPtr contact) const
{
	const std::string query{ fmt::format(
		std::string("INSERT INTO entries(id, name, surname, birthday, phone_number, address) ") +
		"VALUES('{0}', '{1}', '{2}', '{3}', '{4}', '{5}')",
		id,
		contact->name,
		contact->surname,
		contact->birthday,
		contact->phoneNumber,
		contact->address
	) };

	ExecuteQuery(query);
}

void PhoneBookWorker::RemoveContact(const ContactId& id) const
{
	const std::string query{ fmt::format("DELETE FROM entries WHERE id='{}'", id) };

	ExecuteQuery(query);
}

void PhoneBookWorker::UpdateContact(const ContactId& id, ContactShPtr contact) const
{
	const std::string query{ fmt::format(
		std::string("UPDATE entries ") +
		"SET name='{0}', surname='{1}', birthday='{2}', phone_number='{3}', address='{4}' " +
		"WHERE id='{5}'",
		contact->name,
		contact->surname,
		contact->birthday,
		contact->phoneNumber,
		contact->address,
		id
	) };

	ExecuteQuery(query);
}

Row PhoneBookWorker::SelectRow(const ContactId& id) const
{
	const std::string query{ fmt::format("SELECT * FROM entries WHERE id='{}'", id) };

	Row row;
	const auto result{ sqlite3_exec(cUnitPtr_->Get(), query.c_str(), details::SelectRowCallback, &row, NULL) };

	if (result != SQLITE_OK) {
		THROW_LOGIC_ERROR(sqlite3_errmsg(cUnitPtr_->Get()));
	}

	return row;
}

Rows PhoneBookWorker::SelectAll() const
{
	const std::string query{ fmt::format("SELECT * FROM entries") };
	
	Rows rows;
	const auto result{ sqlite3_exec(cUnitPtr_->Get(), query.c_str(), details::SelectAllCallback, &rows, NULL) };

	if (result != SQLITE_OK) {
		THROW_LOGIC_ERROR(sqlite3_errmsg(cUnitPtr_->Get()));
	}

	return rows;
}

void PhoneBookWorker::ExecuteQuery(const std::string& query) const
{
	const auto result{ sqlite3_exec(cUnitPtr_->Get(), query.c_str(), NULL, 0, NULL) };

	if (result != SQLITE_OK) {
		THROW_LOGIC_ERROR(sqlite3_errmsg(cUnitPtr_->Get()));
	}
}

}	// namespace db
}	// namespace pb