#include "ContactsTableWidget.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QScrollBar>
#include <QStringList>

#include <map>

namespace app
{
namespace details
{

const static QString tableHeaderStyleSheet{ QStringLiteral(
	"QHeaderView::section{"
		"spacing: 10px;"
		"background-color:lightblue;"
		"border: 1px solid black;"
		"margin: 1px;"
		"text-align: center;"
		"font-size: 13px;"
		"font-family: arial;"
	"}"
)};

}	// namespace details

ContactsTableWidget::ContactsTableWidget(PhoneBookShPtr pb, QWidget *parent)
	: QTableWidget{ parent }
	, phoneBook_{ pb }
{
	Init(parent);
}

void ContactsTableWidget::Init(QWidget *parent)
{
	QStringList headerLabeles{ 
		tr("id"), tr("name"), tr("surname"), tr("birthday"), tr("phone number"), tr("address")
	};
	this->setColumnCount(headerLabeles.size());
	this->setHorizontalHeaderLabels(headerLabeles);
	this->setStyleSheet(details::tableHeaderStyleSheet);

	this->resize((parent->width() * 775 / 1000),  parent->height());
	this->move(parent->width() * 25 / 100, 0);

	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	this->verticalHeader()->setVisible(false);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->FillTable();

	connect(this, &ContactsTableWidget::cellClicked, this, &ContactsTableWidget::CellSelected);
}

void ContactsTableWidget::FillTable()
{
	int row{ 0 };
	for (const auto& entry : phoneBook_->GetContactsInfo()) {
		InsertRow(row++, entry);
	}
	
	this->sortItems(1);
}

void ContactsTableWidget::InsertRow(int rowIdx, const pb::PhoneBookEntry& entry)
{
	if (rowIdx != this->currentRow()) {
		this->setRowCount(this->rowCount() + 1);
	}
	
	std::vector<QTableWidgetItem*> items{
		new QTableWidgetItem(entry.first.c_str()),
		new QTableWidgetItem(entry.second->name.c_str()),
		new QTableWidgetItem(entry.second->surname.c_str()),
		new QTableWidgetItem(entry.second->birthday.c_str()),
		new QTableWidgetItem(entry.second->phoneNumber.c_str()),
		new QTableWidgetItem(entry.second->address.c_str())
	};

	for (int col{ 0 }; col < items.size(); ++col)
	{
		items[col]->setFlags(items[col]->flags() ^ Qt::ItemIsEditable);
		this->setItem(rowIdx, col, items[col]);
		this->setColumnWidth(this->column(items[col]), this->width() * 162 / 1000);
	}
}

void ContactsTableWidget::AddButtonPressed(const pb::PhoneBookEntry& entry)
{
	this->InsertRow(this->rowCount(), entry);
	this->sortItems(1);
}

void ContactsTableWidget::RemoveButtonPressed(const pb::PhoneBookEntry& entry)
{
	this->removeRow(this->currentRow());
}

void ContactsTableWidget::UpdateButtonPressed(const pb::PhoneBookEntry& entry)
{
	this->InsertRow(this->currentRow(), entry);
	this->sortItems(1);
}

void ContactsTableWidget::CellSelected(int row, int col)
{
	try
	{
		pb::ContactId id{ this->item(row, 0)->text().toStdString() };
		pb::PhoneBookEntry entry{ phoneBook_->GetEntryById(id) };
		emit RowSelected(entry);
	}
	catch (const std::logic_error& ex)
	{
		QMessageBox error;
		error.setText(ex.what());
		error.setIcon(QMessageBox::Critical);
		error.exec();
	}
}

}	// namespace app