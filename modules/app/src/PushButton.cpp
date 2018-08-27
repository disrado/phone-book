#include "PushButton.h"

namespace app
{

PushButton::PushButton(const QString& label, QWidget* parent)
	: QPushButton{ label, parent }
{
	this->setFixedSize(parent->width() * 25 / 100, parent->height() * 16 / 100);
	
	this->setStyleSheet(QStringLiteral(
		"QPushButton{"
			"font: 13px;"
			"background-color: "
				"qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 white, stop: 1 yellow);"
			"border: 1px solid grey;"
			"border-radius: 6px;"
		"}"
	));
}

}	// namespace app