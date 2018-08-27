#pragma once

#include "pb/PhoneBook.h"
#include "pb/Database/DbHandler.h"
#include "pb/Database/PhoneBookWorker.h"

#include <openssl/sha.h>

#include <boost/exception/all.hpp>

#include <random>
#include <sstream>

#define THROW_LOGIC_ERROR(arg) BOOST_THROW_EXCEPTION(std::logic_error{ arg })

namespace pb
{

PhoneBook::PhoneBook()
{
	auto connectionUnit{ db::DbHandler::Instance().Acquire() };
	{
		auto worker{ std::make_shared<db::PhoneBookWorker>(connectionUnit) };
		const auto rows{ worker->SelectAll() };

		for (size_t i{ 0 }; i < rows.size(); ++i)
		{
			const auto contact{ std::make_shared<pb::Contact>(
				rows[i][1],
				rows[i][2],
				rows[i][3],
				rows[i][4],
				rows[i][5]
			) };

			book_.insert(std::make_pair(rows[i][0], contact));
		}
	}
}

PhoneBookEntry PhoneBook::AddContact(ContactShPtr contact)
{
	auto connectionUnit{ db::DbHandler::Instance().Acquire() };
	{
		auto worker{ std::make_shared<db::PhoneBookWorker>(connectionUnit) };
		const auto id{ GenerateId(contact) };

		worker->InsertContact(id, contact);
		return *(book_.insert(std::make_pair(id, contact)).first);
	}
}

PhoneBookEntry PhoneBook::RemoveContact(ContactId id)
{
	auto connectionUnit{ db::DbHandler::Instance().Acquire() };
	{
		auto worker{ std::make_shared<db::PhoneBookWorker>(connectionUnit) };
		worker->RemoveContact(id);

		const auto itr{ book_.find(id) };
		book_.erase(itr);
		return *itr;
	}
}

PhoneBookEntry PhoneBook::UpdateContact(ContactId id , ContactShPtr contact)
{
	auto connectionUnit{ db::DbHandler::Instance().Acquire() };
	{
		auto worker{ std::make_shared<db::PhoneBookWorker>(connectionUnit) };
		worker->UpdateContact(id, contact);

		const auto itr{ book_.find(id) };
		itr->second = contact;
		return *itr;
	}
}

std::map<ContactId, ContactShPtr> PhoneBook::GetContactsInfo() const
{
	return book_;
}

PhoneBookEntry PhoneBook::GetEntryById(const ContactId& id) const
{
	const auto result{ book_.find(id) };

	if (result == book_.end()) {
		throw std::logic_error("Cannot find entry by this id: " + id);
	}

	return *result;
}

std::string PhoneBook::GenerateId(ContactShPtr contact) const
{
	const std::string strToHash{
		contact->name +
		contact->surname +
		contact->birthday +
		contact->phoneNumber +
		contact->address
	};

	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, strToHash.c_str(), strToHash.size());
	SHA256_Final(hash, &sha256);
	std::stringstream sstream;
	for (int i{ 0 }; i < SHA256_DIGEST_LENGTH; ++i) {
		sstream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}
	return sstream.str();
}

}	// namespace pb