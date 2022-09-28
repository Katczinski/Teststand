#include "get_adv_cipher.h"
#include <QFile>

Get_Adv_Cipher::Get_Adv_Cipher()
{
    timer.setSingleShot(true);
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );
    connect(mgr, &QNetworkAccessManager::finished, this, [this](QNetworkReply *rep){
        is_timed_out = false;
        result = QString(rep->readAll());
        loop.quit();
    });
}
QString Get_Adv_Cipher::Request(QString req_body)
{
    if (FETCH_FROM_URL){
        is_timed_out = true;
        url = QUrl(req_start + req_body + req_end);
        qDebug() << "Trying URL" << url;
        mgr->get(QNetworkRequest(url));
        timer.start(2000);
        loop.exec();
    } else {
        QFile xml("xmls/" + req_body + ".txt");
        if (xml.open(QIODevice::ReadOnly)) {
            qDebug() << "file" <<  req_body << "found";
            result = xml.readAll();
            is_timed_out = false;
            xml.close();
        } else
            return "Не удалось найти/открыть файл с шифром";
    }
    if(is_timed_out){
        qDebug("timeout");
        return "ошибка : сайт не ответил вовремя";
    } else
        return Parse();
}
QString Get_Adv_Cipher::Parse()
{
    qDebug() << "GOT from site" << result;
    if (result == "") //TODO remove this stub for not working site
            /*result = "<root>                         "
                     "<modif>kB.AIO-1</modif>"
                     "<isp>kB.AIO - 5180-0001</isp>"
                     "<shifr>##1111-1111-1111-1111#</shifr>"
                     "<rev/>                          "
                     "<channel/>                      "
                     "<elements> </elements>          "
                     "</root>                         "
                      ;*/
            result = "<root>                         "
                    "<modif>kB.DIOPDO-1</modif>      "
                    "<isp>kB.DIOPDO - 6180-0004</isp>"
                    "<shifr>#22-22##1111-1111</shifr>"
                    "<rev/>                          "
                    "<channel/>                      "
                    "<elements> </elements>          "
                    "</root>                         "
                     ;
    QString to_search = "shifr>";

    auto split = result.split(to_search);
    if (split.size() < 3)
        return "ошибка : с сайта " +
                url.toString() +
                " не удалось получить ожидаемое содержимое";
    QString ext = "";
    if (split.at(1) != "") {
        ext = split.at(1).chopped(2);
        ext.replace("-", "");
    }
    to_search = "modif>";
    if(!result.contains(to_search)) {
        qDebug() << "no modif found";
        return "";
    }
    split = result.split(to_search);
    if (split.size() < 3) {
        qDebug() << "splitted by '" << to_search << "' returned size < 3";
        return "";
    }
    auto modif_string = split.at(1).chopped(2);
    qDebug() << "modif_string" << modif_string;
    split = modif_string.split("-");
    if (split.size() < 2) {
        qDebug() << modif_string << "splitted by '-' returned size < 2";
        return "";
    }
    qDebug() << "modif_string" << split[1];
    bool is_ok;
    modif = split[1].toInt(&is_ok);
    // TODO: "ошибка" - модификация 0 при отправке исполнение было >0
    if (!is_ok) {
        qDebug() << "modif is not a number";
        return "";
    }
    return ext;
}
