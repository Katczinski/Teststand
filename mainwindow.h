#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QCodeEditor.hpp"
#include "QSyntaxStyle.hpp"
#include "QPythonCompleter.hpp"
#include "QPythonHighlighter.hpp"
#include <QToolBar>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDir>
#include <QTimer>
#include <QNetworkReply>
#include <QTextStream>
#include <QFileDialog>
#include <QToolButton>
#include <QPixmap>
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QScrollBar>
#include <QShortcut>
#include <QSettings>
#include <QKeySequence>
#include <QTextDocumentFragment>
#include <QDateTime>
#include <QHostInfo>
#include <QNetworkInterface>
#include "form_bin_file.h"
#include "results_logger.h"
#include "results_window.h"
#include "testscounter.h"
#include "d2xx.h"
#include "crc.h"

#define LIBS_PROTECT_ID_SIZE        4
#define SALT_SIZE_WORD              4 // Размер соли в словах (1 слово 4 байта)
#define CODESYS_LICENSE_ADDRESS     0x100FE064

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_start_clicked();

    void on_modification_currentIndexChanged(int index);

private:
    void rollToHistoryPoint();
    void parseIncludedFiles();
    void indexIncludedFiles();
    bool validateSettings();
    void parseLine(QString line);
    void createActions();
    bool save();
    void AskToSave();
    void newFile();
    uint32_t getCodesysLicense();
    bool codesysFirmware();
    bool checkCodesys();
    void setCurrentFile(const QString &fileName);
    void openFile(const QString &fileName);
    bool saveAs();
    bool saveFile(const QString &fileName);
    void open();
    bool start();
    bool flash();
private:
    Ui::MainWindow  *ui;
    QSettings       settings {QSettings::IniFormat, QSettings::SystemScope, "org", "Teststand"};
    QDir            dir;
    QString         curFile;
    QFileDialog     filedialog;
    bool            started = false;
    bool            fail = false;
    bool            isUntitled;
    Results_Logger  *results;
    Results_Window  *results_window;
    QNetworkAccessManager *mgr;
    QStringList                                 includedFiles;
    QMap<QString, QVector<QPair<QString, int>>> index;
    QTimer                                      indexTimer;
    QStringList     cipher = {
                    "41100000001101100",
                    "41111503021111100",
                    "41111503021111200",
                    "41111503021112200"

    };


};
#endif // MAINWINDOW_H
