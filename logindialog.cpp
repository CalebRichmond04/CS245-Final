#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>

/*Login Password is: CS245
 *
 *Simple login window that is required before the mainwindow will be pop up
 */




logindialog::logindialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logindialog)
{
    ui->setupUi(this);

    // Connect OK and Cancel buttons manually to handle login logic
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        this->done(QDialog::Accepted);
    });

    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [this]() {
        this->done(QDialog::Rejected);
    });
}

logindialog::~logindialog()
{
    delete ui;
}

// Override QDialog::done to implement custom password validation
void logindialog::done(int r)
{
    QString key = "CS245";  // plaintext password for validation

    if (r == QDialog::Accepted)
    {
        QString input = ui->passwordEdit->text();

        // Check if user input matches the password
        if (input == key)
        {
            QDialog::done(r); // Accept the dialog if password is correct
            return;
        }
        else
        {
            // Show warning if password is incorrect
            QMessageBox::warning(
                this,
                tr("Alert"),
                tr("Password Incorrect")
                );

            ui->passwordEdit->clear(); // Clear the input for another attempt
            return;
        }
    }
    else
    {
        // If the dialog was rejected (Cancel)
        QDialog::done(r);
        return;
    }
}
