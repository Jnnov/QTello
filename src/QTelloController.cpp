#include "header/QTelloController.hpp"

QTelloController::QTelloController() {
    this->timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(keyPressSignalSender()) );
}

bool QTelloController::eventFilter(QObject* obj, QEvent* event) {
    QKeyEvent* keyEvent;

    if (event->type() == QEvent::KeyPress) {
        keyEvent = static_cast<QKeyEvent*>(event);
        if (!keyEvent->isAutoRepeat()) {
            if (this->keyId.empty()) {
                this->keyId << keyEvent->key();
                timer->start(delay);
            } else {
                keyId << keyEvent->key();
            }
            return true;
        }
    }

    if (event->type() == QEvent::KeyRelease) {
        keyEvent = static_cast<QKeyEvent*>(event);
        if (!keyEvent->isAutoRepeat()) {
            int remkey = keyEvent->key();
            this->keyId.remove(this->keyId.indexOf(remkey) );
            if (this->keyId.empty()) {
                this->timer->stop();
            }
            emit keyReleaseSignal(remkey);
            return true;
        }
    }
    return false;
}

void QTelloController::setDelay(int time) {
    this->delay = time;
}

void QTelloController::keyPressSignalSender() {
    emit keyPressSignal(this->keyId);
}
