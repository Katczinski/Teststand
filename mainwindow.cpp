#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->codeEditor->setSyntaxStyle(QSyntaxStyle::defaultStyle());
    ui->codeEditor->setCompleter(new QPythonCompleter(this));
    ui->codeEditor->setHighlighter(new QPythonHighlighter());
    dir = QDir::current();
    dir.cd("scripts");
    filedialog.setDirectory(dir);
    createActions();

}

void MainWindow::createActions()
{
    QToolBar *fileToolBar = new QToolBar();
    ui->codeEditorMenu->addWidget(fileToolBar);
    fileToolBar->setMovable(false);//we don't allow to move the bar
    fileToolBar->setContextMenuPolicy(Qt::PreventContextMenu);//or to disable it

    QAction *newAct = new QAction(QIcon("images/new.png"), tr("&Новый"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Создать новый файл"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileToolBar->addAction(newAct);

    QAction *openAct = new QAction(QIcon("images/open.png"), tr("&Открыть..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Открыть файл"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileToolBar->addAction(openAct);

    QAction *openMainAct = new QAction(QIcon("images/open_main.png"), tr("&Открыть main.py"), this);
    openMainAct->setStatusTip(tr("Открыть main.py"));
    connect(openMainAct, &QAction::triggered, [this]{
       openFile(dir.filePath("main.py"));
    });
    fileToolBar->addAction(openMainAct);

    QAction *saveAct = new QAction(QIcon("images/save.png"), tr("&Сохранить"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранить файл"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileToolBar->addAction(saveAct);

    fileToolBar->addSeparator();
    QShortcut *shortcut_start = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(shortcut_start, &QShortcut::activated, this, &MainWindow::start);

    connect(ui->codeEditor, &QCodeEditor::includeFileClicked, this, [this](QString text){
       openFile(dir.filePath(text + ".py"));
    });

    openFile(QDir::current().path() + "\\scripts\\main.py");
}

bool MainWindow::saveAs()
{
    QString full_path = filedialog.directory().currentPath() +"/scripts/" + curFile;
    QString fileName = filedialog.getSaveFileName(this,
                                                  tr("Сохранить как"),
                                                  full_path,
                                                  "python(*.py)");
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr(""),
                             tr("Не удалось записать в файл %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->codeEditor->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл сохранен"), 2000);
    return true;
}

bool MainWindow::save()
{
    return isUntitled ? saveAs() : saveFile(curFile);
}

void MainWindow::openFile(const QString &fileName)
{
    QFile(curFile).close();
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr(""),
                             tr("Невозможно прочитать файл %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->codeEditor->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл загружен"), 5000);
}

void MainWindow::open()
{
    AskToSave();
    const QString fileName = filedialog.getOpenFileName(this, tr("WOOT"), dir.absolutePath() + "/scripts", "*.py");
    if (!fileName.isEmpty())
        openFile(fileName);
}

void MainWindow::AskToSave()
{
    if (!ui->codeEditor->document()->isModified())
        return;

    QMessageBox messageBox {QMessageBox::Warning,
                tr("Файл изменён"),
                tr("Сохранить изменения?"),
                QMessageBox::Save | QMessageBox::Cancel};
    messageBox.setButtonText(QMessageBox::Save, tr("Сохранить"));
    messageBox.setButtonText(QMessageBox::Cancel, tr("Не сохранять"));
    int ret = messageBox.exec();
    if(ret == QMessageBox::Save)
        save();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    isUntitled = fileName.isEmpty();
    if (isUntitled) {
        curFile = "new_algorithm.py";
    } else {
        curFile = QFileInfo(fileName).canonicalFilePath();
    }

    ui->codeEditor->document()->setModified(false);
}

void MainWindow::newFile()
{
    AskToSave();
    isUntitled = true;
    setCurrentFile("");
    ui->codeEditor->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::start()
{
    if (started)
        return;
    started = true;
    QEventLoop loop;
    QProcess *process = new QProcess();
    QString cmd = "python";
    QString args = "-u " + curFile + " " + ui->module->currentText()[0] + " " + ui->modification->currentText(); // -u for disabling stdout buffering
    connect(process, &QProcess::readyRead, [&process](){
        while (process->canReadLine()) {
           QString line = QString::fromLocal8Bit(process->readLine());
           line.replace("\r\n", "");
           qDebug() << line;
        }
    });
    connect(process, &QProcess::readyReadStandardError, [&process](){
        qDebug() << process->readAllStandardError();
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&loop, &process](){
        process->deleteLater();
        qDebug() << "process finished";
        loop.quit();
    });
    process->setNativeArguments(args);
    process->setProgram(cmd);
    qDebug() << cmd << args;
    process->start();
    if (process->state() == QProcess::Running)
        loop.exec();
    started = false;
}

void MainWindow::flash()
{
    if (started)
        return;
    started = true;
    QStringList info = { "4111150302111120",
                         ui->serialnum->text(),
                         ui->modification->currentText(),
                         ui->module->currentText().split(" - ")[0],
                         ui->mac->text(),
                         ui->ip->text(),
                         ui->mask->text(),
                         ui->gateway->text()
                       };
    if (!Form_bin_file(info)) {
        qDebug() << "Could not generate cipher.bin";
        started = false;
        return;
    }
    QEventLoop loop;
    QProcess *process = new QProcess();
    QString oocd_dir = "flash\\oocd";
    process->setWorkingDirectory(oocd_dir);
    QString cmd = oocd_dir + "\\openocd.exe";
    QString args = QString("-f target\\komega_basic\\init.cfg \
                    -c \"program ../firmware/") + ui->modification->currentText() + QString("b.hex verify\" \
                    -c \"program ../firmware/cipher.bin verify 0x100FE000\" \
                    -c \"program ../firmware/") + ui->modification->currentText() + QString(".hex verify\" \
                    -f target\\komega_basic\\finish.cfg");
    bool is_ok = true;
    connect(process, &QProcess::readyReadStandardError, this, [&process](){
        qDebug() << process->readAllStandardError();
    });
    connect(process, &QProcess::errorOccurred, this, [&is_ok, &loop](QProcess::ProcessError error){
        qDebug() << "QProcess error" << error;
        QString err = error == QProcess::ProcessError::FailedToStart ? "Ошибка запуска" : "Неизвесная ошибка";
        is_ok = false;
        loop.quit();
    });

    auto lambda = connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [&is_ok, &loop, &process](int exitCode, QProcess::ExitStatus exitStatus){
        QByteArray res = process->readAllStandardError();
        QString dir_name = "//A2/kT.teststand.logs/" + QHostInfo::localHostName() + "/oocd/";
        QString filename = "dl_" + QDateTime::currentDateTime().toString("d-MMM-yyyy_hh-mm-ss") + ".txt";
//        QDir().mkpath(dir_name);
//        QFile log(dir_name + filename);
//        if (log.open(QIODevice::WriteOnly)) {
//            log.write(res);
//            log.close();
//        }
        is_ok = exitCode == 0;
        qDebug() << "process finished with exitCode" << exitCode << "exitStatus" << exitStatus;
        loop.quit();
    });
    process->setNativeArguments(args);
    process->setProgram(cmd);
    process->start();
    if (process->state() == QProcess::Running)
        loop.exec();
    started = false;
}

void MainWindow::on_start_clicked()
{
    if (ui->flash->isChecked()) {
        flash();
    }
    if (ui->test->isChecked())
        start();
}

