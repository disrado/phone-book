#include <pb/Database/DbHandler.h>
#include <pb/Database/PhoneBookWorker.h>
#include <pb/Contact.h>
#include <pb/PhoneBook.h>

#include <gtest/gtest.h>

#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>

#include <cstdio>
#include <ctime>
#include <memory>

namespace details
{

const std::string DEFAULT_PATH{ "../../../tests/TestDatabase/" };

//! When tests will be finished each Contact with id which contains in this vector
//! will be removed from database
std::vector<pb::ContactId> insertedContacts;

//! If you need created a Contact object, you should use this function
//! If you created Contact object with make_shared etc. and make worker->InsertContact or
//! phoneBook->AddContact You should manualy call function details::RemoveWhenTestsCompleted(id)
pb::ContactShPtr CreateContact(
	pb::ContactId id,
	std::string name,
	std::string surname,
	std::string birthday,
	std::string phoneNumber,
	std::string address)
{
	const auto contact{ std::make_shared<pb::Contact>(
		name, surname, birthday, phoneNumber, address
	) };

	details::insertedContacts.push_back(id);

	return contact;
}

void RemoveWhenTestsCompleted(pb::ContactId id)
{
	insertedContacts.push_back(id);
}

void ClearDb()
{
	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };
	auto worker{ std::make_shared<pb::db::PhoneBookWorker>(connectionUnit) };
	
	for(const auto contactId : insertedContacts) {
		worker->RemoveContact(contactId);
	}
}

}	// namespace details

TEST(Init, init)
{
	ASSERT_THROW(
		pb::db::DbHandler::Instance().Init("InvalidDbName", 4),
		std::logic_error
	);

	const std::string dbName{ details::DEFAULT_PATH + "pbTest.db" };

	pb::db::DbHandler::Instance().Init(dbName, 4);

	srand((unsigned int)time(0));
}

TEST(DbHandler, Acquire)
{
	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };

	ASSERT_TRUE(connectionUnit != nullptr);
}

TEST(PhoneBookWorker, SelectRow)
{
	const pb::ContactId id{ std::to_string(rand()) };
	const auto contact{ details::CreateContact(
		id, "name", "surname", "birthday", std::to_string(rand()) + "1", "address"
	) };

	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };
	auto worker{ std::make_shared<pb::db::PhoneBookWorker>(connectionUnit) };

	worker->InsertContact(id, contact);

	const auto row{ worker->SelectRow(id) };

	ASSERT_FALSE(row.empty());

	ASSERT_EQ(contact->name, row[1]);
	ASSERT_EQ(contact->surname, row[2]);
	ASSERT_EQ(contact->birthday, row[3]);
	ASSERT_EQ(contact->phoneNumber, row[4]);
	ASSERT_EQ(contact->address, row[5]);
}

TEST(PhoneBookWorker, InsertContact)
{
	const pb::ContactId id{ std::to_string(rand()) };
	const auto contact{ details::CreateContact(
		id, "name", "surname", "birthday", std::to_string(rand() + rand()), "address"
	) };

	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };
	auto worker{ std::make_shared<pb::db::PhoneBookWorker>(connectionUnit) };
	
	worker->InsertContact(id, contact);

	const auto row{ worker->SelectRow(id) };

	ASSERT_FALSE(row.empty());

	ASSERT_EQ(contact->name, row[1]);
	ASSERT_EQ(contact->surname, row[2]);
	ASSERT_EQ(contact->birthday, row[3]);
	ASSERT_EQ(contact->phoneNumber, row[4]);
	ASSERT_EQ(contact->address, row[5]);

	ASSERT_THROW(worker->InsertContact(id, contact), std::logic_error);
}

TEST(PhoneBookWorker, RemoveContact)
{
	srand((unsigned int)time(NULL));

	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };
	auto worker{ std::make_shared<pb::db::PhoneBookWorker>(connectionUnit) };

	const pb::ContactId id{ std::to_string(rand() + rand()) };
	const auto contact{ details::CreateContact(
		id, "name", "surname", "birthday", std::to_string(rand() + rand()), "address"
	) };

	worker->InsertContact(id, contact);
	worker->RemoveContact(id);

	const auto row{ worker->SelectRow(id) };

	ASSERT_TRUE(row.empty());
}

TEST(PhoneBookWorker, UpdateContact)
{
	srand((unsigned int)time(NULL));

	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };
	auto worker{ std::make_shared<pb::db::PhoneBookWorker>(connectionUnit) };

	const pb::ContactId id{ std::to_string(rand() + rand()) };
	const auto contact{ details::CreateContact(
		id, "name", "surname", "birthday", std::to_string(rand() + rand()) + "1", "address"
	) };
	const auto contact2{ details::CreateContact(
		id, "name", "surname", "birthday", std::to_string(rand()), "address"
	) };

	worker->InsertContact(id, contact);
	worker->UpdateContact(id, contact2);

	const auto row{ worker->SelectRow(id) };

	ASSERT_TRUE(row.size() > 0);

	ASSERT_EQ(contact2->name, row[1]);
	ASSERT_EQ(contact2->surname, row[2]);
	ASSERT_EQ(contact2->birthday, row[3]);
	ASSERT_EQ(contact2->phoneNumber, row[4]);
	ASSERT_EQ(contact2->address, row[5]);
}

TEST(PhoneBookWorker, SelectAll)
{
	const pb::ContactId id1{ std::to_string(rand()) };
	const auto contact1{ details::CreateContact(
		id1, "name", "surname", "birthday", std::to_string(rand()) + "1", "address"
	) };

	const pb::ContactId id2{ std::to_string(rand()) };
	const auto contact2{ details::CreateContact(
		id2, "name", "surname", "birthday", std::to_string(rand()) + "1", "address"
	) };

	const pb::ContactId id3{ std::to_string(rand()) };
	const auto contact3{ details::CreateContact(
		id3, "name", "surname", "birthday", std::to_string(rand()) + "1", "address"
	) };

	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };
	auto worker{ std::make_shared<pb::db::PhoneBookWorker>(connectionUnit) };

	worker->InsertContact(id1, contact1);
	worker->InsertContact(id2, contact2);
	worker->InsertContact(id3, contact3);

	const auto rows{ worker->SelectAll() };

	ASSERT_TRUE(rows.size() != 0);
}

TEST(PhoneBook, AddContact)
{
	const auto contact{ std::make_shared<pb::Contact>(
		"name", "surname", "birthday", std::to_string(rand()) + "1", "address"
	) };

	auto phobeBook = std::make_shared<pb::PhoneBook>();
	const auto result{ phobeBook->AddContact(contact) };

	const pb::ContactId addedContactId{ result.first };
	details::RemoveWhenTestsCompleted(addedContactId);

	const auto addedContact{ result.second };

	ASSERT_EQ(addedContact, contact);

	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };
	auto worker{ std::make_shared<pb::db::PhoneBookWorker>(connectionUnit) };
	
	const auto row{ worker->SelectRow(addedContactId) };

	ASSERT_FALSE(row.empty());

	ASSERT_EQ(contact->name, row[1]);
	ASSERT_EQ(contact->surname, row[2]);
	ASSERT_EQ(contact->birthday, row[3]);
	ASSERT_EQ(contact->phoneNumber, row[4]);
	ASSERT_EQ(contact->address, row[5]);
}

TEST(PhoneBook, RemoveContact)
{
	const auto contact{ std::make_shared<pb::Contact>(
		"name", "surname", "birthday", std::to_string(rand()) + "2", "address"
	) };

	auto phobeBook = std::make_shared<pb::PhoneBook>();
	
	const auto id{ (phobeBook->AddContact(contact)).first };
	phobeBook->RemoveContact(id);

	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };
	auto worker{ std::make_shared<pb::db::PhoneBookWorker>(connectionUnit) };
	
	const auto row{ worker->SelectRow(id) };

	ASSERT_TRUE(row.empty());
}

TEST(PhoneBook, UpdateContact)
{
	srand((unsigned int)time(NULL));

	const auto contact{ std::make_shared<pb::Contact>(
		"name", "surname", "birthday", std::to_string(rand()) + "1", "address"
	) };
	
	const auto contact2{ std::make_shared<pb::Contact>(
		"name", "surname", "birthday", std::to_string(rand()), "address"
	) };

	auto phobeBook = std::make_shared<pb::PhoneBook>();

	const auto id{ (phobeBook->AddContact(contact)).first };

	details::RemoveWhenTestsCompleted(id);

	const auto updatedContact{ phobeBook->UpdateContact(id, contact2) };

	ASSERT_EQ(contact2->name, updatedContact.second->name);
	ASSERT_EQ(contact2->surname, updatedContact.second->surname);
	ASSERT_EQ(contact2->birthday, updatedContact.second->birthday);
	ASSERT_EQ(contact2->phoneNumber, updatedContact.second->phoneNumber);
	ASSERT_EQ(contact2->address, updatedContact.second->address);
}

TEST(PhoneBook, GetContactsInfo)
{
	details::ClearDb();

	auto phobeBook = std::make_shared<pb::PhoneBook>();

	const auto contact{ std::make_shared<pb::Contact>(
		"namsdfe", "susdafrname", "birasfthday", std::to_string(rand()) + "1", "aas"
	) };
	const auto contact2{ std::make_shared<pb::Contact>(
		"nadsfme", "surnasdafme", "birthdaasfy", std::to_string(rand()) + "38", "add"
	) };

	const auto id{ (phobeBook->AddContact(contact)).first };
	details::RemoveWhenTestsCompleted(id);
	const auto id2{ (phobeBook->AddContact(contact2)).first };
	details::RemoveWhenTestsCompleted(id2);

	const auto rows{ phobeBook->GetContactsInfo() };

	ASSERT_TRUE(rows.size() == 2);
}

TEST(PhoneBook, GetEntryById)
{
	const auto contact{ std::make_shared<pb::Contact>(
		"ContactName", "surname", "birthday", std::to_string(rand()) + "8", "address"
	) };

	auto phobeBook = std::make_shared<pb::PhoneBook>();
	
	const pb::ContactId id{ (phobeBook->AddContact(contact)).first };
	details::RemoveWhenTestsCompleted(id);

	ASSERT_EQ(contact, phobeBook->GetEntryById(id).second);

	auto connectionUnit{ pb::db::DbHandler::Instance().Acquire() };
	auto worker{ std::make_shared<pb::db::PhoneBookWorker>(connectionUnit) };
	
	const auto row{ worker->SelectRow(id) };

	ASSERT_FALSE(row.empty());

	ASSERT_EQ(contact->name, row[1]);
	ASSERT_EQ(contact->surname, row[2]);
	ASSERT_EQ(contact->birthday, row[3]);
	ASSERT_EQ(contact->phoneNumber, row[4]);
	ASSERT_EQ(contact->address, row[5]);
}

TEST(ClearDb, clearDb)
{
	details::ClearDb();
}
