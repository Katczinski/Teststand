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
    results = new Results_Logger();
    results_window = new Results_Window(results);
    results_window->right_layout->addWidget(results);
    results_window->left_layout->addWidget(&tests_counter);

    connect(results_window, &Results_Window::windowClosed, [this]() { fail = true; });
    connect(tests_counter.button_close, &QPushButton::clicked,
            results_window, [this]{
        results_window->close();
        fail = true; //stopping test
//        scan->hasFinished = false; //
    });
    connect(tests_counter.button_restart, &QPushButton::clicked,
            results_window, [this]{
        fail = true; //stopping test
//        scan->hasFinished = false; //
//        QMetaObject::invokeMethod(this, &Interpreter::Start, Qt::QueuedConnection);
    });

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
    connect(shortcut_start, &QShortcut::activated, this, &MainWindow::on_start_clicked);

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
    auto lambda = connect(results_window, &Results_Window::windowClosed, process, &QProcess::kill);
    connect(process, &QProcess::readyRead, [this, &process](){
        while (process->canReadLine()) {
           QString line = QString::fromLocal8Bit(process->readLine());
           line.replace("\r\n", "");
           parseLine(line);
           qDebug() << line;
        }
    });

    connect(process, &QProcess::readyReadStandardError, [this, &process](){
        QByteArray err = process->readAllStandardError();
        results->LogResult(QString(err));
        qDebug() << err;
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
    QObject::disconnect(lambda);
    started = false;
}

void MainWindow::parseLine(QString line)
{
    if (line.left(7) == "result:") {
        if (line.contains("OK"))
            results->LogResult("<span style='color:green'>&nbsp;<b>Успешно</b>&nbsp;</span>");
        else if (line.contains("FAIL"))
            results->LogResult("<span style='color:white; background:red'>&nbsp;<b>Провалено</b>&nbsp;</span>");
    } else if (line.left(7) == "header:") {
        results->LogHeader(line.remove("header:"));
    } else if (line.left(6) == "print:")
        results->LogResult(line.remove("print:"));
}

void MainWindow::flash()
{
    if (started)
        return;
    results->LogHeader("Прошивка");
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

    connect(process, &QProcess::errorOccurred, this, [this, &is_ok, &loop](QProcess::ProcessError error){
        qDebug() << "QProcess error" << error;
        QString err = error == QProcess::ProcessError::FailedToStart ? "Ошибка запуска" : "Неизвесная ошибка";
        is_ok = false;
        results->LogResult(err);
        results->LogResult("<span style='color:white; background:red'>&nbsp;<b>Провалено</b>&nbsp;</span>");
        loop.quit();
    });

    auto lambda = connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, &is_ok, &loop, &process](int exitCode, QProcess::ExitStatus exitStatus){
        QByteArray res = process->readAllStandardError();
        qDebug() << "result:\n" << res;
        QString dir_name = "//A2/kT.teststand.logs/" + QHostInfo::localHostName() + "/oocd/";
        QString filename = "dl_" + QDateTime::currentDateTime().toString("d-MMM-yyyy_hh-mm-ss") + ".txt";

        if (!exitCode)
            results->LogResult("<span style='color:green'>&nbsp;<b>Успешно</b>&nbsp;</span>");
        else {
            results->LogResult(QString(res));
            results->LogResult("<span style='color:white; background:red'>&nbsp;<b>Провалено</b>&nbsp;</span>");
        }
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
    D2xx *d2xx = new D2xx();
    d2xx->SpiSetPowerState(true);
    process->setNativeArguments(args);
    process->setProgram(cmd);
    process->start();
    if (process->state() == QProcess::Running)
        loop.exec();
    d2xx->SpiSetPowerState(false);
    delete d2xx;
    started = false;
}

void MainWindow::on_start_clicked()
{
    QTime start_time = QTime::currentTime();
    QTime elapsed(0,0);
    QTime finish_time;

    results_window->hide();
    results->Start();
    results_window->setFocus();
    results_window->show();
    tests_counter.Reset();

    results->append(start_time.toString("mm:ss") + " Тест начался<br>");

    if (ui->flash->isChecked())
        flash();
    if (ui->test->isChecked())
        start();

    tests_counter.Result();
    finish_time = QTime::currentTime();
    elapsed = elapsed.addSecs(start_time.secsTo(finish_time));
    results->append("\nПродолжительность теста: " + elapsed.toString("mm:ss"));
    results->ScrollToEnd();
    results->setTextInteractionFlags(Qt::TextBrowserInteraction);
}

