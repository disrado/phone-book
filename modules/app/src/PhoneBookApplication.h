#pragma once

#include <QApplication>

#include <memory>

namespace app
{

class MainWindow;

class PhoneBookApplication : public QApplication
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	PhoneBookApplication(int argc, char* argv[]);

	//
	// Private data members.
	//
private:
	//! Main window instance.
	std::shared_ptr<MainWindow> mainWindow_;
};

}	// namespace app