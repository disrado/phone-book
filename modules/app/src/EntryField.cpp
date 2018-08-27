#include "EntryField.h"

namespace app
{

EntryField::EntryField(const QString& placeholder, QWidget* parent)
	: QLineEdit{ parent }
{
	this->setFixedSize(parent->width() * 90 / 100, parent->height() * 10 / 100);
	this->setPlaceholderText(placeholder);

	this->setStyleSheet(QStringLiteral(
		"QLineEdit{"
			"font: 13px;"
			"background-color: "
				"qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 white, stop: 1 yellow);"
			"border: 1px solid grey;"
			"border-radius: 6px;"
		"}"
	));
}

}	// namespace app