#ifndef QT_TELLO_KEY_CONTROLLER_HPP
#define QT_TELLO_KEY_CONTROLLER_HPP

#include "QTelloInclude.hpp"

class QTelloController : public QWidget {
    Q_OBJECT
public:
    QTelloController();
    bool eventFilter(QObject*, QEvent*);
    void setDelay(int);
private:
    int delay = 20;
    QTimer* timer;
    QVector<int> keyId;
signals:
    void keyPressSignal(QVector<int>);
    void keyReleaseSignal(int);
public slots:
    void keyPressSignalSender();
};

#endif
