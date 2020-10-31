#include "mainwindow.h"
#include "note.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Note note;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
