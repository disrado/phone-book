# Phone-book application

master|dev
:---:|:---:
|[![Build Status](https://travis-ci.org/disrado/phone-book.svg?branch=master)](https://travis-ci.org/disrado/phone-book)|[![Build Status](https://travis-ci.org/disrado/phone-book.svg?branch=dev)](https://travis-ci.org/disrado/phone-book)
|[![CodeFactor](https://www.codefactor.io/repository/github/disrado/phone-book/badge)](https://www.codefactor.io/repository/github/disrado/phone-book)|[![CodeFactor](https://www.codefactor.io/repository/github/disrado/phone-book/badge)](https://www.codefactor.io/repository/github/disrado/phone-book)
|[![codecov](https://codecov.io/gh/disrado/phone-book/branch/master/graph/badge.svg)](https://codecov.io/gh/disrado/phone-book)|[![codecov](https://codecov.io/gh/disrado/phone-book/branch/dev/graph/badge.svg)](https://codecov.io/gh/disrado/phone-book)

# Modules structure.

## app
Executable module which contains gui application based on Qt framework.

## libpb
Library module which contains logic for working with database.

# How to build this project?
1. Add Qt5_DIR variable to system variables list which will be contains path to qt.
For example: Qt5_DIR "C:\Qt\5.11.1\msvc2017_64"
2. Go to the "sripts" directory and run script what you need

# Run executable
1. For running executable you need add path to qt dynamic libraries to your PATH variable
For example: PATH "C:\Qt\5.11.1\msvc2017_64\bin"

# Tests

Before running this tests you should create "pbTests.db" database in tests/TestDatabase
and execute content of scripts/db.sql for this database.