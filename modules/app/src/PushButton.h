#pragma once

#include <QPushButton>

namespace app
{

class PushButton : public QPushButton
{
	Q_OBJECT

	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	PushButton(const QString& label, QWidget* parent);
};

}	// namespace app