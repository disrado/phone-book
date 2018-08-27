#pragma once

#include <QMainWindow>
#include <QTableWidget>

namespace app
{

class CentralWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	explicit MainWindow(QWidget* parent = nullptr);

	//
	// Private data members.
	//
private:
	//! Pointer to CentralWidget.
	CentralWidget* centralWidget_;
};

}	// namespace app