#include <QApplication>
#include "widget.h"

//#include <QApplication>


#include <QString>
#include <QInputDialog>
QTextStream cout(stdout);
QTextStream cin(stdin);

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    bool bOk;
    int N = QInputDialog::getInt( 0, "enter the number of population",
                                    "number:", 1000, 0, 10000, 1, &bOk
                                        );
    if (!bOk) {
        N = 1000;
    }
    Widget w(N);
   // w.show();
    return a.exec();
}
