#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>
#include <QTextCodec>
QFile runtimeLog;

void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("%1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    case QtInfoMsg:
        txt = QString("Info: %1").arg(msg);
        break;
    }
    if (!runtimeLog.isOpen()) {
        try {
            runtimeLog.open(QIODevice::WriteOnly | QIODevice::Append);
            if (runtimeLog.isOpen()) {
                QTextStream ts(&runtimeLog);
                ts << txt << "\n";
                runtimeLog.close();
            }
        } catch (const std::exception &ex) {
             txt = ex.what();
        }
    }
    QTextStream outStream(stdout);
    outStream.setCodec(QTextCodec::codecForName("cp1251"));
    outStream << txt << "\n" << Qt::flush;
//    printf("%s\n", txt.toStdString().c_str());
//    fflush(stdout);
}

void redirectStdout(bool debug)
{
    if (!debug) {
        FILE *fp;
        if ((fp=freopen(runtimeLog.fileName().toStdString().c_str(), "w", stdout))==NULL)
            qDebug() << "Cannot open file";
        else
            qDebug() << "redirecting debug";
    }
}

int main(int argc, char *argv[])
{
    setvbuf(stdin, NULL, _IONBF, 0);
    bool debug = false;
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], "-debug"))
            debug = true;
    }
    QApplication a(argc, argv);
    QString dir = "//A2/kT.teststand.logs/" + QHostInfo::localHostName() + "/kb.EG.Teststand/runtime";
    if (!QDir(dir).exists()) {
        if (!QDir().mkpath(dir))
            QMessageBox::critical(nullptr, "Ошибка", "Не удалось создать директорию. Логи не будут записаны");
        else {
            runtimeLog.setFileName(dir + "/log " + QDateTime::currentDateTime().toString("dd.M.yyyy HH-mm") + ".txt");
            qInstallMessageHandler(myMessageHandler);
            redirectStdout(debug);
        }
    } else {
        runtimeLog.setFileName(dir + "/log " + QDateTime::currentDateTime().toString("dd.M.yyyy HH-mm") + ".txt");
        qInstallMessageHandler(myMessageHandler);
        redirectStdout(debug);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
