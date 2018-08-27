CREATE TABLE entries (
	id TEXT NOT NULL UNIQUE,
	name TEXT NOT NULL,
	surname TEXT NOT NULL,
	birthday TEXT NOT NULL,
	phone_number TEXT NOT NULL UNIQUE,
	address TEXT NOT NULL
)