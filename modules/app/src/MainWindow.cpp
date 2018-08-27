#include "MainWindow.h"

#include "CentralWidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QStatusBar>

namespace app
{

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow{ parent }
{
	this->setWindowTitle(tr("PhoneBook"));
	this->setFixedSize(
		QApplication::desktop()->screenGeometry().width() * 50 / 100,
		QApplication::desktop()->screenGeometry().height() * 35 / 100);

	centralWidget_ = new CentralWidget(this);
}

}	// namespace app