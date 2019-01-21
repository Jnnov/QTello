#include "header/QTelloMainWindow.hpp"

QTelloMainWindow::QTelloMainWindow() {
    this->tello = new QTelloCom();
    this->controller = new QTelloController();
    this->controller->setDelay(20);

    this->setWindowTitle("window");

    this->label_1 = new QLabel("press key", this);
    this->label_1->setGeometry(5, 0, 100, 15);
    this->label_2 = new QLabel("key code", this);
    this->label_2->setGeometry(5, 15, 50, 15);

    QObject::connect(this->controller, SIGNAL(keyPressSignal(QVector<int>)), this, SLOT(keyPressedAction(QVector<int>)) );
    QObject::connect(this->controller, SIGNAL(keyReleaseSignal(int)), this, SLOT(keyReleasedAction(int)) );
    this->installEventFilter(controller);
    this->show();
}

void QTelloMainWindow::keyPressedAction(QVector<int> keyId) {
    QString press_key = "";
    static bool takeoff = false;
    for (int a : keyId) {
        press_key += " " + QString::number(a);
        QEventLoop* waitSignal = new QEventLoop();
        switch (a) {
            case Qt::Key_Space:
                this->tello->sendcmd(!takeoff ? QTelloCom::TAKEOFF : QTelloCom::LAND ,0, 0, 0, 0);
                takeoff = !takeoff;
                QObject::connect(this->controller, SIGNAL(keyReleaseSignal(int)), waitSignal, SLOT(quit()) );
                waitSignal->exec();
                break;

            case Qt::Key_W:
                this->tello->sendcmd(QTelloCom::HIGH, 0, 0, 660, 0);
                break;
            case Qt::Key_S:
                this->tello->sendcmd(QTelloCom::HIGH, 0, 0, -660, 0);
                break;
            case Qt::Key_A:
                this->tello->sendcmd(QTelloCom::HIGH, 660, 0, 0, 0);
                break;
            case Qt::Key_D:
                this->tello->sendcmd(QTelloCom::HIGH, -660, 0, 0, 0);
                break;

            case Qt::Key_Up:
                this->tello->sendcmd(QTelloCom::HIGH, 0, 660, 0, 0);
                break;
            case Qt::Key_Down:
                this->tello->sendcmd(QTelloCom::HIGH, 0, -660, 0, 0);
                break;
            case Qt::Key_Left:
                this->tello->sendcmd(QTelloCom::HIGH, 0, 0, 0, 660);
                break;
            case Qt::Key_Right:
                this->tello->sendcmd(QTelloCom::HIGH, 0, 0, 0, -660);
                break;
            default:
                break;
        }
    }
    this->label_2->setText(press_key);
}

void QTelloMainWindow::keyReleasedAction(int key) {
    if (key != Qt::Key_Space) {
        this->tello->sendcmd(QTelloCom::NORMAL, 0, 0, 0, 0);
    }
}
