#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>

logindialog::logindialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logindialog)
{
    ui->setupUi(this);

    // Connect OK and Cancel buttons manually
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

void logindialog::done(int r)
{
    QString key = "CS245";  // plaintext password

    if (r == QDialog::Accepted)
    {
        QString input = ui->passwordEdit->text();

        if (input == key)
        {
            QDialog::done(r);
            return;
        }
        else
        {
            QMessageBox::warning(
                this,
                tr("Alert"),
                tr("Password Incorrect")
                );

            ui->passwordEdit->clear();
            return;
        }
    }
    else
    {
        QDialog::done(r);
        return;
    }
}
