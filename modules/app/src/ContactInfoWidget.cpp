#include "ContactInfoWidget.h"

#include "EntryField.h"
#include "PushButton.h"

#include <pb/Contact.h>
#include <pb/PhoneBook.h>

#include <QMessageBox>

namespace app
{

ContactInfoWidget::ContactInfoWidget(PhoneBookShPtr pb, QWidget* parent)
	: QWidget{ parent }
	, phoneBook_{ pb }
{
	this->resize(parent->size().width() * 25 / 100,  parent->size().height());

	auto commonLayout{ new QVBoxLayout{ this } };
	commonLayout->addLayout(this->InitEntryFields());
	commonLayout->addLayout(this->InitButtons());
	this->setLayout(commonLayout);
}

QVBoxLayout* ContactInfoWidget::InitEntryFields()
{
	NameLineEdit_ = new EntryField{ tr("Name"), this };
	SurnameLineEdit_ = new EntryField{ tr("Surname"), this };
	BirthdayLineEdit_ = new EntryField{ tr("Birthday"), this };
	PhoneNumberLineEdit_ = new EntryField{ tr("Phone number"), this };
	AddressLineEdit_ = new EntryField{ tr("Address"), this };

	auto entryFieldsLayout{ new QVBoxLayout{} };
	entryFieldsLayout->addWidget(NameLineEdit_);
	entryFieldsLayout->addWidget(SurnameLineEdit_);
	entryFieldsLayout->addWidget(BirthdayLineEdit_);
	entryFieldsLayout->addWidget(PhoneNumberLineEdit_);
	entryFieldsLayout->addWidget(AddressLineEdit_);

	return entryFieldsLayout;
}

QHBoxLayout* ContactInfoWidget::InitButtons()
{
	addButton_ = new PushButton{ tr("Add"), this };
	removeButton_ = new PushButton{ tr("Remove"), this };
	updateButton_ = new PushButton{ tr("Update"), this };

	removeButton_->setEnabled(false);
	updateButton_->setEnabled(false);

	connect(addButton_, &QPushButton::pressed, this, &ContactInfoWidget::AddButtonOnClick);
	connect(removeButton_, &QPushButton::pressed, this, &ContactInfoWidget::RemoveButtonOnClick);
	connect(updateButton_, &QPushButton::pressed, this, &ContactInfoWidget::UpdateButtonOnClick);

	auto buttonsLayout{ new QHBoxLayout{} };
	buttonsLayout->addWidget(addButton_);
	buttonsLayout->addWidget(updateButton_);
	buttonsLayout->addWidget(removeButton_);

	return buttonsLayout;
}

void ContactInfoWidget::AddButtonOnClick()
{
	this->CheckEntryFields();

	try
	{
		const auto entry{ phoneBook_->AddContact(this->ContactFromFields()) };
		currentContactId_ = entry.first;
		emit AddButtonPressed(entry);
	}
	catch (const std::logic_error& ex)
	{
		this->ShowCriticalMessageBox(ex.what());
	}
}

void ContactInfoWidget::RemoveButtonOnClick()
{
	try
	{
		const auto entry{ phoneBook_->RemoveContact(currentContactId_) };
		emit RemoveButtonPressed(entry);

		currentContactId_ = "";
		ClearEntryFields();

		removeButton_->setEnabled(false);
		updateButton_->setEnabled(false);
	}
	catch (const std::logic_error& ex)
	{
		this->ShowCriticalMessageBox(ex.what());
	}
}

void ContactInfoWidget::UpdateButtonOnClick()
{
	this->CheckEntryFields();

	try
	{
		const auto entry{ phoneBook_->UpdateContact(currentContactId_, this->ContactFromFields()) };
		currentContactId_ = entry.first;
		emit UpdateButtonPressed(entry);
	}
	catch (const std::logic_error& ex)
	{
		this->ShowCriticalMessageBox(ex.what());
	}
}

void ContactInfoWidget::RowSelected(const pb::PhoneBookEntry entry)
{
	removeButton_->setEnabled(true);
	updateButton_->setEnabled(true);

	currentContactId_ = entry.first;

	NameLineEdit_->setText(entry.second->name.c_str());
	SurnameLineEdit_->setText(entry.second->surname.c_str());
	BirthdayLineEdit_->setText(entry.second->birthday.c_str());
	PhoneNumberLineEdit_->setText(entry.second->phoneNumber.c_str());
	AddressLineEdit_->setText(entry.second->address.c_str());
}

pb::ContactShPtr ContactInfoWidget::ContactFromFields()
{
	return std::make_shared<pb::Contact>(
		NameLineEdit_->text().toStdString(),
		SurnameLineEdit_->text().toStdString(),
		BirthdayLineEdit_->text().toStdString(),
		PhoneNumberLineEdit_->text().toStdString(),
		AddressLineEdit_->text().toStdString()
	);
}

bool ContactInfoWidget::AllFieldsFilled()
{
	return NameLineEdit_->text().size() != 0
		&& SurnameLineEdit_->text().size() != 0
		&& BirthdayLineEdit_->text().size() != 0
		&& PhoneNumberLineEdit_->text().size() != 0
		&& AddressLineEdit_->text().size() != 0;
}

void ContactInfoWidget::CheckEntryFields()
{
	if (!this->AllFieldsFilled()) {
		this->ShowCriticalMessageBox(tr("Please fill all fields."));
	}
}

void ContactInfoWidget::ShowCriticalMessageBox(const QString& message)
{
	QMessageBox error;
	error.setText(message);
	error.setIcon(QMessageBox::Critical);
	error.exec();
}

void ContactInfoWidget::ClearEntryFields()
{
	NameLineEdit_->setText("");
	SurnameLineEdit_->setText("");
	BirthdayLineEdit_->setText("");
	PhoneNumberLineEdit_->setText("");
	AddressLineEdit_->setText("");
}

}	// namespace app