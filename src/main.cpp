#include <QObject>
#include <QtGui>
#include <QThread>
#include <QTimer>
#include <QtCore>
#include <QtWidgets>

#include "header/QTelloMainWindow.hpp"
#include "header/QTelloController.hpp"

int main(int argc, char *argv[]) {
    QApplication* app = new QApplication(argc, argv);
    QTelloMainWindow* mainwin = new QTelloMainWindow();
    return app->exec();
}
