#ifndef GET_ADV_CIPHER_H
#define GET_ADV_CIPHER_H
#include <QObject>
#include <QNetworkReply>
#include <QEventLoop>
#include <qnetworkaccessmanager.h>
#include <QTimer>

#define FETCH_FROM_URL 0

class Get_Adv_Cipher : public QObject
{
    Q_OBJECT
    QString Parse();
public:
    Get_Adv_Cipher();
    QString Request(QString req_body);
    const QString req_start = "http://ext-cipher.mzta.ru/php/cfgkb/testing.php?kod=";
    const QString req_end = "&ch=MCU&xml";
    QUrl url;
    bool is_timed_out;
    QString result;
    QNetworkAccessManager *mgr {new QNetworkAccessManager()};
    QTimer timer;
    QEventLoop loop;
    int modif{0};
};

#endif // GET_ADV_CIPHER_H
