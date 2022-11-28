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
        if (info.size() < 6)
            return false;
        kb_eg to_bin;
        to_bin.serialnum = serial_number;
        to_bin.rev = 1;
        to_bin.since_epoch = (int32_t)QDateTime::currentDateTime().toSecsSinceEpoch();
        to_bin.modif = modification;
        for (int i = 0; i < 16; ++i){
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
//        info[4].remove("-");
        QStringList split = info[4].split(':');
        if (split.size() < 6)
            return false;
        for (int i = 0; i < 6; ++i)
            to_bin.mac[i] = split[i].toInt(nullptr, 16);
        to_bin.unique_id = info[5].toUInt();
        QFile cipher_f("flash/firmware/cipher.bin");
        cipher_f.open(QIODevice::WriteOnly);
        cipher_f.write(reinterpret_cast<char*>(&to_bin), sizeof(to_bin));
        cipher_f.close();
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
