#pragma once

#include <pb/PhoneBook.h>

#include <QLineEdit>
#include <QTableWidget>

#include <memory>

namespace app
{

//
// Alias declaration.
//
using PhoneBookShPtr = std::shared_ptr<pb::PhoneBook>;

class ContactsTableWidget : public QTableWidget
{
	Q_OBJECT

	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	ContactsTableWidget(PhoneBookShPtr pb, QWidget* parent = nullptr);

	//
	// Private methods.
	//
private:
	//! Initiantes widget.
	void Init(QWidget* parent);
	//! Fills the content with phone book enties.
	void FillTable();
	//! Inerts phone book entry to row.
	void InsertRow(int rowIdx, const pb::PhoneBookEntry& entry);

	//
	// Signals.
	//
signals:
	//! Notifies which row has been selected.
	void RowSelected(const pb::PhoneBookEntry entry);

	//
	// Public slots.
	//
public slots:
	//! Handles "AddButton pressed" event.
	void AddButtonPressed(const pb::PhoneBookEntry& entry);
	//! Handles "RemoveButton pressed" event.
	void RemoveButtonPressed(const pb::PhoneBookEntry& entry);
	//! Handles "UpdateButton pressed" event.
	void UpdateButtonPressed(const pb::PhoneBookEntry& entry);
	//! Handles "Cell selected" event.
	void CellSelected(int, int);

	//
	// Private data members.
	//
private:
	//! Shared pointer to phone book.
	PhoneBookShPtr phoneBook_;
};

}	// namespace app