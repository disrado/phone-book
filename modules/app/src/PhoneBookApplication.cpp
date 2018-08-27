#include "PhoneBookApplication.h"

#include "MainWindow.h"

#include <pb/Database/DbHandler.h>

namespace app
{

PhoneBookApplication::PhoneBookApplication(int argc, char* argv[])
	: QApplication{ argc, argv }
{
	pb::db::DbHandler::Instance().Init("pb.db", 4);

	mainWindow_ = std::make_shared<app::MainWindow>();
	mainWindow_->show();
}

}	// namespace app