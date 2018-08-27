#include "CentralWidget.h"

#include "ContactInfoWidget.h"
#include "ContactsTableWidget.h"

#include <QColor>
#include <QPalette>

namespace app
{

CentralWidget::CentralWidget(QWidget* parent)
	: QWidget{ parent }
	, phobeBook_{ std::make_shared<pb::PhoneBook>() }
{
	this->resize(parent->size().width(), parent->size().height());

	QPalette palette{ this->palette() };
	palette.setColor(QPalette::Background, QColor(200, 200, 235));
	this->setAutoFillBackground(true);
	this->setPalette(palette);

	contactInfoWidget_ = new ContactInfoWidget{ phobeBook_, this };
	contactsTableWidget_ = new ContactsTableWidget{ phobeBook_, this };

	connect(
		contactInfoWidget_,
		&ContactInfoWidget::AddButtonPressed,
		contactsTableWidget_,
		&ContactsTableWidget::AddButtonPressed);

	connect(
		contactInfoWidget_,
		&ContactInfoWidget::RemoveButtonPressed,
		contactsTableWidget_,
		&ContactsTableWidget::RemoveButtonPressed);

	connect(
		contactInfoWidget_,
		&ContactInfoWidget::UpdateButtonPressed,
		contactsTableWidget_,
		&ContactsTableWidget::UpdateButtonPressed);

	connect(
		contactsTableWidget_,
		&ContactsTableWidget::RowSelected,
		contactInfoWidget_,
		&ContactInfoWidget::RowSelected);
}

}	// namespace app