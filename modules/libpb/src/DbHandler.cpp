#include "pb/Database/DbHandler.h"

#include <fmt/format.h>

#include <iostream>
#include <regex>

#define THROW_LOGIC_ERROR(arg) BOOST_THROW_EXCEPTION(std::logic_error{ arg })

namespace pb
{
namespace db
{

ConnectionUnit::ConnectionUnit(Connection& connection)
	: connection_{ connection }
{
	connection_.second = ConnectionState::Busy;
}

ConnectionUnit::~ConnectionUnit()
{
	connection_.second = ConnectionState::Free;
}

sqlite3* ConnectionUnit::Get() const
{
	return connection_.first.get();
}

DbHandler::DbHandler()
	: poolSize_{ 5 }
{
	connectionDeleter_ = [](sqlite3* db) -> void { 
		sqlite3_close(db);
	};
}

DbHandler& DbHandler::Instance()
{
	static DbHandler instance{};
	return instance;
}

void DbHandler::Init(const fs::path& path, size_t poolSize)
{
	const std::regex regular{ ".*[\\w]\\.db" };
	const bool matchResult{ std::regex_match(path.string().c_str(), regular) };

	if (!matchResult) {
		THROW_LOGIC_ERROR(
			fmt::format("\"{}\" - invalid name for databse. Should be name.db", path.string()));
	}

	dbPath_ = path;
	poolSize_ = poolSize;

	if (pool_.empty()) {
		for (size_t i{ 0 }; i < poolSize_; ++i) {
			CreateConnection();
		}
	}
}

Connection DbHandler::GetFreeConnection()
{
	for (const auto& connection : pool_)
	{
		if (connection.second == ConnectionState::Free) {
			return connection;
		}
	}
	return CreateConnection();
}

Connection DbHandler::CreateConnection()
{
	sqlite3* newConnection;
	const int openDbResult{ sqlite3_open(dbPath_.string().c_str(), &newConnection) };
	
	if (openDbResult != SQLITE_OK) {
		THROW_LOGIC_ERROR(fmt::format("Cannot open db {}", dbPath_.string()));
	}

	const auto connectionPtr = std::shared_ptr<sqlite3>(newConnection, connectionDeleter_);
	const auto result{ pool_.insert(std::make_pair(connectionPtr, ConnectionState::Free)) };

	if (!result.second) {
		THROW_LOGIC_ERROR("Cannot create db connection");
	}

	return *(result.first);
}

DbHandler::ConnectionUnitShPtr DbHandler::Acquire()
{
	return std::make_shared<ConnectionUnit>(GetFreeConnection());
}

}	// namespace db
}	// namespace pb