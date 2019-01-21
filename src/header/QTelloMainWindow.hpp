#ifndef QT_TELLO_MAINWINDOW_HPP
#define QT_TELLO_MAINWINDOW_HPP

#include "QTelloInclude.hpp"
#include "QTelloCom.hpp"
#include "QTelloController.hpp"

class QTelloMainWindow : public QMainWindow {
    Q_OBJECT
public:
    QTelloMainWindow();
protected:
    QLabel* label_1;
    QLabel* label_2;
    QTelloCom* tello;
    QTelloController* controller;
public slots:
    void keyPressedAction(QVector<int>);
    void keyReleasedAction(int);
};

#endif
