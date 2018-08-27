#pragma once

#include <sqlite3.h>

#include <boost/filesystem.hpp>

#include <functional>
#include <memory>
#include <unordered_map>

namespace pb
{
namespace db
{

//! Shows the current state of connection.
enum class ConnectionState
{
	Free,
	Busy
};

//
// Alias declaration.
//
using SqliteShPtr = std::shared_ptr<sqlite3>;
using Connection = std::pair<SqliteShPtr, ConnectionState>;
namespace fs = boost::filesystem;

class ConnectionUnit
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	ConnectionUnit(Connection& connection);
	//! Destructor.
	~ConnectionUnit();
	//! Copy constructor.
	ConnectionUnit(const ConnectionUnit&) = delete;
	//! Move constructor.
	ConnectionUnit(const ConnectionUnit&&) = delete;
	//! Copy assignment operator.
	ConnectionUnit& operator=(const ConnectionUnit&) = delete;
	//! Move assignment operator.
	ConnectionUnit& operator=(const ConnectionUnit&&) = delete;

	//
	// Public interface.
	//
public:
	//! Returns db connection.
	sqlite3* Get() const;

	//
	// Private data members.
	//
private:
	//! Shared pointer to db connection.
	Connection connection_;
};

class DbHandler final
{
	//
	// Alias declaration.
	//
public:
	using ConnectionDeleter = std::function<void(sqlite3*)>;
	using ConnectionPool = std::unordered_map<SqliteShPtr, ConnectionState>;
	using ConnectionUnitShPtr = std::shared_ptr<ConnectionUnit>;

	//
	// Public interface.
	//
public:
	//! Returns DbHandler instance.
	static DbHandler& Instance();
	//! Fills the pull of connections.
	void Init(const fs::path& path, size_t poolSize);
	//! Returns database connection.
	ConnectionUnitShPtr Acquire();

	//
	// Construction and destruction.
	//
private:
	// Constructor.
	DbHandler();

	//
	// Private methods.
	//
private:
	//! Returns free connection.
	Connection GetFreeConnection();
	//! Creates a new connection.
	Connection CreateConnection();

	//
	// Private data members.
	//
private:
	//! Pool of the database connections.
	ConnectionPool pool_;
	//! Pointer to a database object.
	ConnectionDeleter connectionDeleter_;
	//! Size of the connection pool.
	size_t poolSize_;
	//! Pool of the database connections.
	fs::path dbPath_;
};

}	// namespace db
}	// namespace pb