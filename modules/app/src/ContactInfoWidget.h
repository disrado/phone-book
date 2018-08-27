#pragma once

#include <pb/PhoneBook.h>

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace app
{

class EntryField;
class PushButton;

//
// Alias declaration.
//
using PhoneBookShPtr = std::shared_ptr<pb::PhoneBook>;

class ContactInfoWidget : public QWidget
{
	Q_OBJECT

	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	ContactInfoWidget(PhoneBookShPtr pb, QWidget* parent = nullptr);

	//
	// Public slots.
	//
public slots:
	//! Handles "Add" button.
	void AddButtonOnClick();
	//! Handles "Remove" button.
	void RemoveButtonOnClick();
	//! Handler "Update" button.
	void UpdateButtonOnClick();
	//! Handles RowSelected event.
	void RowSelected(const pb::PhoneBookEntry);

	//
	// Signals.
	//
signals:
	//! Notifies that "Add" button was pressed.
	void AddButtonPressed(const pb::PhoneBookEntry& entry);
	//! Notifies that "Remove" button was pressed.
	void RemoveButtonPressed(const pb::PhoneBookEntry& entry);
	//! Notifies that "Update" button was pressed.
	void UpdateButtonPressed(const pb::PhoneBookEntry& entry);

	//
	// Private methods.
	//
private:
	//! Creates contact object from entry fields data.
	pb::ContactShPtr ContactFromFields();
	//! Checks if all entry fields filled.
	bool AllFieldsFilled();
	//! Checks if all entry fields filled.
	void CheckEntryFields();
	//! Shows message box with "critical" picture.
	void ShowCriticalMessageBox(const QString& message);
	//! Creates all entry field.
	QVBoxLayout* InitEntryFields();
	//! Creates all buttons.
	QHBoxLayout* InitButtons();
	//! Clears all entry fields.
	void ClearEntryFields();

	//
	// Privat data members.
	//
private:
	//! Name entry field.
	EntryField* NameLineEdit_;
	//! Surname entry field.
	EntryField* SurnameLineEdit_;
	//!Birthday entry field.
	EntryField* BirthdayLineEdit_;
	//! Phone number entry field.
	EntryField* PhoneNumberLineEdit_;
	//! Address entry field.
	EntryField* AddressLineEdit_;
	//! Add push button.
	PushButton* addButton_;
	//! Remove push button.
	PushButton* removeButton_;
	//! Update push button.
	PushButton* updateButton_;
	//! Id of selected contact.
	pb::ContactId currentContactId_;
	//! Shared pointer to phone book.
	PhoneBookShPtr phoneBook_;
};

}	// namespace app