#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    logindialog login;

    if (login.exec() == QDialog::Accepted) {
        w.show();
    }
    else {
        exit(0);
    }

    return a.exec();
}

