#include "header/QTelloCom.hpp"

QTelloCom::QTelloCom() {
    if ((this->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->TELLO_PORT_CMD);
    this->addr.sin_addr.s_addr = inet_addr(this->TELLO_IP.c_str());

    this->sendcmd(REQ_CONN, 0, 0, 0, 0);
}

QTelloCom::~QTelloCom() {
    close(this->sockfd);
}

void QTelloCom::sendcmd(QTelloCom::CMD cmd_id, int yaw, int throttle, int pitch, int roll) {
    QVector<unsigned char> sendCommand;
    this->commandSignalGen(sendCommand, cmd_id, yaw, throttle, pitch, roll);
    sendto(this->sockfd, sendCommand.data(), sendCommand.size(), 0, (struct sockaddr*) &(this->addr), sizeof(this->addr));
    for (auto&& n : sendCommand) {
        printf("%x ", n);
    }
    printf("\n");
}

void QTelloCom::commandSignalGen(QVector<unsigned char>& cmd_binary, int ex_mode, int yaw, int throttle, int pitch, int roll) {
    int mode;
    if ((mode = ex_mode - 4) < 0) { // not stick control
        cmd_binary << this->command[ex_mode];
        return;
    }
    // stick control
    yaw += 1024;
    throttle += 1024;
    pitch += 1024;
    roll += 1024;
    cmd_binary += {0xcc, 0xb0, 0x00, 0x7f, 0x60, 0x50, 0x00, 0x00, 0x00};
    cmd_binary += {
        static_cast<unsigned char>(roll),
        static_cast<unsigned char>((pitch << 3) ^ (roll >> 8)),
        static_cast<unsigned char>((throttle << 6) ^ (pitch >> 5)),
        static_cast<unsigned char>(throttle >> 2),
        static_cast<unsigned char>((yaw << 1) ^ (throttle >> 10)),
        static_cast<unsigned char>((mode << 4) ^ (yaw >> 7))
    };

    QDateTime now = QDateTime::currentDateTime();
    cmd_binary += {
        static_cast<unsigned char>(now.time().hour()),
        static_cast<unsigned char>(now.time().minute()),
        static_cast<unsigned char>(now.time().second()),
        static_cast<unsigned char>(now.time().msec() >> 8),
        static_cast<unsigned char>(now.time().msec())
    };

    int seed = 0x3692;
    for (auto&& i : cmd_binary) {
        seed = this->table_crc16[(seed ^ i) & 0xff ] ^ (seed >> 8);
    }

    cmd_binary += {
        static_cast<unsigned char>(seed),
        static_cast<unsigned char>(seed >> 8)
    };
}
