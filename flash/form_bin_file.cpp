#include "form_bin_file.h"
#include <QDateEdit>
#include <QDebug>
#include <QFile>

bool Form_bin_file(QStringList info)
{
    if (info.size() < 4)
        return false;
    QString cipher = info[0];
    int serial_number = info[1].toInt();
    int modification = info[2].toInt();
    int type = info[3].toInt();
    if (type == 4) {
        if (info.size() < 8)
            return false;
        kb_eg to_bin;
        to_bin.serialnum = serial_number;
        to_bin.rev = 1;
        to_bin.since_epoch = (int32_t)QDateTime::currentDateTime().toSecsSinceEpoch();
        to_bin.modif = modification;
        for (int i = 0; i < cipher.size(); ++i){
            int num = cipher[i].toLatin1();
            if(num >= '0' && num <= '9')
                to_bin.cipher[i] = num - '0';
            else if(num >= 'A' && num <='Z')
                to_bin.cipher[i] = num - 'A' + 10;
            else if(num == '#')
                to_bin.cipher[i] = (char)200;
            else {
                qDebug() << num;
                return false;
            }
        }
        for (int i = 0; i < 12; ++i)
            to_bin.ext_cipher[i] = 0;
        if (info[4].size() < 12)
            return false;
        to_bin.mac[0] = info[4].left(8).toUInt(nullptr, 16);        //0x00FF3834;
        to_bin.mac[1] = info[4].remove(0, 8).toUInt(nullptr, 16);   //0x96A50000;
        QStringList split = info[5].split('.');
        if (split.size() < 4)
            return false;
        to_bin.ip = (split[0].toInt() << 0) + (split[1].toInt() << 8) + (split[2].toInt() << 16) + (split[3].toInt() << 24);
        split.clear();
        split = info[6].split('.');
        if (split.size() < 4)
            return false;
        to_bin.mask = (split[0].toInt() << 0) + (split[1].toInt() << 8) + (split[2].toInt() << 16) + (split[3].toInt() << 24);
        split.clear();
        split = info[7].split('.');
        if (split.size() < 4)
            return false;
        to_bin.gateway = (split[0].toInt() << 0) + (split[1].toInt() << 8) + (split[2].toInt() << 16) + (split[3].toInt() << 24);
        to_bin.info[0] = 0;
        to_bin.info[1] = 0;
        QFile f("flash/firmware/cipher.bin");
        f.open(QIODevice::WriteOnly);
        f.write(reinterpret_cast<char*>(&to_bin), sizeof(to_bin));
        f.close();
    } else {
        QVector<int32_t> to_bin {};
        for (auto x : cipher){
            int num = x.toLatin1();
            if(num >= '0' && num <= '9')
                to_bin.append(num - '0');
            else if(num >= 'A' && num <='Z')
                to_bin.append(num - 'A' + 10);
            else if(num == '#')
                to_bin.append(200);
            else
                return false;
        }
        to_bin.resize(60 + 64);
        to_bin.append(serial_number);
        to_bin.append(0);
        auto since_epoch = QDateTime::currentDateTime().toSecsSinceEpoch();
        to_bin.append(int32_t(since_epoch));
        to_bin.append(int32_t(since_epoch >> 32));
        to_bin.append(modification);
        qDebug() << "CIPHER" << to_bin;
        QFile f("flash/firmware/cipher.bin");
        f.open(QIODevice::WriteOnly);
        f.write(reinterpret_cast<char*>(to_bin.data()), to_bin.size() * 4);
        f.close();
    }
    return true;
}
