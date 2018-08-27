#pragma once

#include <QLineEdit>

namespace app
{

class EntryField : public QLineEdit
{
	Q_OBJECT

	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	EntryField(const QString& placeholder, QWidget* parent);
};

}	// namespace app