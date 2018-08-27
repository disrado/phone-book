#include "MainWindow.h"

#include "PhoneBookApplication.h"

#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		app::PhoneBookApplication application{ argc, argv };
		return application.exec();
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}