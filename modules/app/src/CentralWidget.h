#pragma once

#include <pb/PhoneBook.h>

#include <QHBoxLayout>
#include <QWidget>

namespace app
{

class ContactInfoWidget;
class ContactsTableWidget;

//
// Alias declaration.
//
using PhoneBookShPtr = std::shared_ptr<pb::PhoneBook>;

class CentralWidget : public QWidget
{
	Q_OBJECT

	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	CentralWidget(QWidget* parent = nullptr);

	//
	//Privarte data members.
	//
private:
	//! Contacts table widget.
	ContactsTableWidget* contactsTableWidget_;
	//! Contact information entry fields and buttons.
	ContactInfoWidget* contactInfoWidget_;
	//! Layout for grouping widgets.
	QHBoxLayout* layout_;
	//! Shared pointer to phone book.
	PhoneBookShPtr phobeBook_;
};

}	// namespace app