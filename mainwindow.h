#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QCodeEditor.hpp"
#include "QSyntaxStyle.hpp"
#include "QPythonCompleter.hpp"
#include "QPythonHighlighter.hpp"
#include <QToolBar>
#include <QMessageBox>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <QToolButton>
#include <QPixmap>
#include <QProcess>
#include <QDebug>
#include <QShortcut>
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
    void parseLine(QString line);
    void createActions();
    bool save();
    void AskToSave();
    void newFile();
    void setCurrentFile(const QString &fileName);
    void openFile(const QString &fileName);
    bool saveAs();
    bool saveFile(const QString &fileName);
    void open();
    void start();
    void flash();
private:
    Ui::MainWindow  *ui;
    bool            isUntitled;
    QDir            dir;
    QString         curFile;
    QFileDialog     filedialog;
    bool            started = false;
    Results_Logger  *results;
    Results_Window  *results_window;
    TestsCounter    tests_counter;
    bool            fail = false;
    QStringList     cipher = {
                    "",
                    "4111150000111110",
                    "4111150302111120",
                    "4111150302111220"
    };


};
#endif // MAINWINDOW_H
