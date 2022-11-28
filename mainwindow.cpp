#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("kB.EG Teststand");
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
    results_window->left_layout->addWidget(&results->tests_counter);
    mgr = new QNetworkAccessManager();
    QString s_login = settings.value("login").toString();
    if (!s_login.isEmpty())
        ui->login->setText(s_login);

    connect(ui->login, &QLineEdit::textChanged, this, [this](){
       settings.setValue("login", ui->login->text());
    });
    connect(results_window, &Results_Window::windowClosed, [this]() { fail = true; });
    connect(results->tests_counter.button_close, &QPushButton::clicked,
            results_window, [this]{
        results_window->close();
        fail = true; //stopping test
    });
    connect(results->tests_counter.button_restart, &QPushButton::clicked,
            results_window, [this]{
        fail = true; //stopping test
        started = false;
        on_start_clicked();
    });

    auto port = new QSerialPort(this);
    QPair<int, int> id = {0x23d0, 0x0c4a}; // Honeywell HF500

    foreach (auto& serialPort, QSerialPortInfo::availablePorts()) {
        if (id.first == serialPort.vendorIdentifier() && id.second == serialPort.productIdentifier()) {
            port->setPortName(serialPort.portName());
            port->open(QIODevice::ReadOnly);
            qDebug() << "Scanner found in com port" << serialPort.portName();
            break;
        }
    }
    connect(port, &QSerialPort::readyRead, [port, this]{
        auto result = QString(port->readAll());
        if(result.right(1) == '\r')// scanner ends the string with '\r', site may fail, so we chop the symbol
            result.chop(1);
        qDebug("ACCEPTED");
        QStringList split = result.split("-");
        QString l_cipher = split[1];
        QString serial = split[2];
        ui->cipher->setText(l_cipher);
        ui->serialnum->setText(serial);
        bool is_matched = false;
        for (int i = 0; i < cipher.size(); ++i) {
            if (l_cipher == cipher[i]) {
                ui->modification->setCurrentIndex(i);
                qDebug() << "modification" << i + 1 << "matched";
                is_matched = true;
            }
        }
        if (!is_matched) {
            QMessageBox box(QMessageBox::Question, "Информация", "Нестандартный шифр исполнения:\n" + l_cipher, QMessageBox::Yes | QMessageBox::No);
            box.setButtonText(QMessageBox::Yes, "Применить");
            box.setButtonText(QMessageBox::No, "Пропустить");
            if (box.exec() == QMessageBox::Yes)
                ui->cipher->setText(l_cipher);
        }
        QString url = "https://komega.mzta.ru/get-mac?kod=*";
        url += result;
        url += "&login=" + ui->login->text();
        QTimer timer;
        QEventLoop loop;
        QString mac;
        bool is_timed_out = true;
        timer.setSingleShot(true);
        connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );
        auto lambda = connect(mgr, &QNetworkAccessManager::finished, this, [&mac, &is_timed_out, &loop](QNetworkReply *rep){
            is_timed_out = false;
            mac = QString(rep->readAll());
            loop.quit();
        });
        QNetworkRequest req = QNetworkRequest(url);
        QSslConfiguration conf = QSslConfiguration::defaultConfiguration();
        auto cert = QSslCertificate::fromPath("\\\\SERVER_1C_TE\\instr\\Сертификат (корневой)\\new\\ca.crt");
        conf.addCaCertificate(cert.first());
        req.setSslConfiguration(conf);
        mgr->get(req);
        qDebug() << url;
        timer.start(2000);
        loop.exec();
        qDebug() << mac;
        if(is_timed_out){
            qDebug("timeout");
            QMessageBox::critical(this, "Ошибка", "Сайт не ответил вовремя. MAC-адрес не получен");
        } else if (mac.split("<div>").size() == 1) {
            if (mac.split(":").size() == 6)
                ui->mac->setText(mac);
            else
                QMessageBox::critical(this, "Ошибка", mac);
        } else {
            QString error = mac.split("<div>")[1].remove("</div>");
            ui->mac->setText(mac.split("<div>")[0]);
            QMessageBox::information(this, "Информация", mac);
        }
        QObject::disconnect(lambda);
    });
    connect(ui->codeEditor, &QCodeEditor::includeFileClicked, this, [this](QString text){
       openFile(dir.filePath(text + ".py"));
    });
    connect(ui->codeEditor, &QCodeEditor::functionClicked, this, [this](QString text){
        for (auto it = index.begin(); it != index.end(); it++) {
            QVector<QPair<QString, int>> funcs = it.value();
            for (QPair<QString, int> func : funcs) {
                if (func.first == text) {
                    openFile(it.key());
                    QTextCursor cursor(ui->codeEditor->document()->findBlockByLineNumber(func.second));
                    ui->codeEditor->verticalScrollBar()->setValue(ui->codeEditor->verticalScrollBar()->maximum());
                    ui->codeEditor->setTextCursor(cursor);
                    return;
                }
            }
        }
    });
    indexTimer.setSingleShot(true);
    connect(&indexTimer, &QTimer::timeout, this, &MainWindow::parseIncludedFiles);
    connect(ui->codeEditor, &QCodeEditor::textChanged, this, [this](){
        indexTimer.start(1000);
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
    indexTimer.start(1000);
}

void MainWindow::indexIncludedFiles()
{
    for (QString fileName : includedFiles) {
        QFile file(QDir::currentPath() + "/scripts/" + fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QString text = stream.readAll();
            QStringList lines = text.split('\n');
            int count = 0;
            for (QString line : lines) {
                if (line.contains("def")) {
                    QStringList parts = line.split(' ', Qt::SkipEmptyParts);
                    if (parts.size() > 1) {
                        QPair<QString, int> pair = { parts[1].mid(0, parts[1].indexOf('(')), count };
                        index[QDir::currentPath() + "/scripts/" + fileName].append(pair);
                    }
                }
                count++;
            }
            file.close();
        }

    }
}

void MainWindow::parseIncludedFiles()
{
    includedFiles.clear();
    index.clear();

    QString text = ui->codeEditor->toPlainText();
    QStringList lines = text.split('\n');
    for (QString line : lines) {
        if (line.contains("import")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.length() == 4 && parts[0] == "from" && parts[2] == "import")
                includedFiles.append(parts[1] + ".py");
        }
    }
    includedFiles.append(curFile.mid(curFile.lastIndexOf('/') + 1));
    indexIncludedFiles();
}

void MainWindow::openFile(const QString &fileName)
{
    if (curFile == fileName) {
        save();
        return;
    }
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
    ui->codeEditor->setPlainText(in.readAll());

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл загружен"), 5000);
    parseIncludedFiles();
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
        curFile = "new_script.py";
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
    delete results;
    delete results_window;
}

bool MainWindow::start()
{
    if (started || results->isHidden())
        return false;
    started = true;
    QEventLoop loop;
    QProcess *process = new QProcess();
    QString cmd = "scripts/Python37/python";
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
        results->LogString(QString(err));
        qDebug() << err;
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&loop, &process](int exitCode, QProcess::ExitStatus exitStatus){
        process->deleteLater();
        qDebug() << "process finished with exitCode" << exitCode << "exitStatus" << exitStatus;
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
    return true;
}

void MainWindow::parseLine(QString line)
{
    if (line.size() >= 7 && line.left(7) == "result:") {
        if (line.contains("OK"))
            results->LogResult(SPAN_OK);
        else if (line.contains("FAIL"))
            results->LogResult(SPAN_FAIL);
    } else if (line.size() >= 7 && line.left(7) == "header:") {
        results->LogHeader(line.remove("header:"));
    } else if (line.size() >= 6 && line.left(6) == "print:")
        results->LogString(line.remove("print:"));
}

int mk_protect_get_unique_id(uint32_t * buff, int size)
{
    /*
     * Для данного микроконтроллера размер уникального идентификатора навен 4 слова (16 байт)
     */

    if (size != 4 || buff == NULL) {
        return 1;
    }

    buff[0] = *(volatile uint32_t*)(0x40048054); // 0x14ffff
    buff[1] = *(volatile uint32_t*)(0x40048058); // 0xffffffff
    buff[2] = *(volatile uint32_t*)(0x4004805C); // 0x4e453791
    buff[3] = *(volatile uint32_t*)(0x40048060); // 0x400a000f

    return 0;
}

uint32_t MainWindow::getCodesysLicense()
{
    bool is_ok = true;
    QEventLoop loop;
    QProcess *process = new QProcess();
    QString oocd_dir = "flash\\oocd";
    uint32_t unique_id[LIBS_PROTECT_ID_SIZE + SALT_SIZE_WORD];
    process->setWorkingDirectory(oocd_dir);
    QString cmd = oocd_dir + "\\openocd.exe";
//     2>&1 | findstr /c:\"0x40048054:\" > log.txt
    QString args = "-f target/komega_basic/init.cfg -f read_memory.cfg -f target/komega_basic/finish.cfg";
    connect(process, &QProcess::readyReadStandardError, [&process, &unique_id](){
        QByteArray err = process->readAllStandardError();
        if (err.startsWith("0x40048054:")) {
            QString str = QString(err).split(":")[1].replace("\n\n", "");
            QStringList splitted = str.split(" ", Qt::SkipEmptyParts);
            for (int i = 0; i < 4; ++i) {
                unique_id[i] = splitted[i].toUInt(nullptr, 16);
                qDebug() << "number[" << i << "]" << QString::number(unique_id[i], 16);
            }
        }
    });
    connect(process, &QProcess::errorOccurred, this, [this, &is_ok, &loop](QProcess::ProcessError error){
        qDebug() << "QProcess error" << error;
        QString err = error == QProcess::ProcessError::FailedToStart ? "Ошибка запуска" : "Неизвесная ошибка";
        is_ok = false;
        results->LogString(err);
        results->LogResult(SPAN_FAIL);
        loop.quit();
    });
    auto lambda = connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [&loop, &is_ok](int exitCode, QProcess::ExitStatus exitStatus){
        qDebug() << "GetCodesysLicense";
        qDebug() << "process finished with exitCode" << exitCode << "exitStatus" << exitStatus;
        is_ok = exitCode == 0;
        loop.quit();
    });
    process->setNativeArguments(args);
    process->setProgram(cmd);
    qDebug() << "process started";
    process->start();
    if (process->state() == QProcess::Running)
        loop.exec();
    QObject::disconnect(lambda);
    if (!is_ok)
        return 0xFFFFFFFF;
    //===================================================Формируем CRC===================================================
    static const uint32_t SALT[SALT_SIZE_WORD] = {
            0x10859533,
            0x77154820,
            0x00553498,
            0x66131156,
    };

    for (int i=0; i<SALT_SIZE_WORD; i++) {
        unique_id[LIBS_PROTECT_ID_SIZE + i] = SALT[i];
    }
    uint32_t crc = crc32_calc((uint8_t*)unique_id, LIBS_PROTECT_ID_SIZE*4 + SALT_SIZE_WORD*4);
    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    stream << crc;
    qDebug() << "crc" << arr.toHex(' ').toUpper();
    return crc;
}

bool MainWindow::codesysFirmware()
{
    if (started || results->isHidden())
        return false;
    results->LogHeader("Прошивка Codesys");
    started = true;
    bool is_ok = true;
    QEventLoop loop;
    QProcess *process = new QProcess();
    QString oocd_dir = "flash\\oocd";
    QString cmd = oocd_dir + "\\openocd.exe";
    process->setWorkingDirectory(oocd_dir);
    auto lambda = connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                [this, &is_ok, &loop, &process](int exitCode, QProcess::ExitStatus exitStatus){
            QByteArray res = process->readAllStandardError();
            qDebug() << "result:\n" << res;

            if (!exitCode)
                results->LogResult(SPAN_OK);
            else {
                results->LogString(QString(res));
                results->LogResult(SPAN_FAIL);
            }
            is_ok = exitCode == 0;
            qDebug() << "process finished with exitCode" << exitCode << "exitStatus" << exitStatus;
            loop.quit();
        });
    int type = ui->module->currentText().split(" - ")[0].toInt();
    QString args = QString("-f target\\komega_basic\\init.cfg \
                   -c \"program ../firmware/") + QString::number(type) + QString("c.hex verify\" \
                   -f target\\komega_basic\\finish.cfg");

    D2xx *d2xx = new D2xx();
    d2xx->SpiSetPowerState(true);
    QThread::sleep(1);
    process->setNativeArguments(args);
    process->setProgram(cmd);
    process->start();
    if (process->state() == QProcess::Running) {
        qDebug() << "process started";
        loop.exec();
    }
    d2xx->SpiSetPowerState(false);
    QThread::sleep(1);
    delete d2xx;
    started = false;
    QObject::disconnect(lambda);
    return is_ok;
}

bool MainWindow::checkCodesys() {
    if (started || results->isHidden())
        return false;
    started = true;
    D2xx *d2xx = new D2xx();
    d2xx->SpiSetPowerState(true);
    QThread::sleep(1);

    results->LogHeader("Проверка Codesys");
    QSerialPort port;
    QPair<int, int> id = { 0x0403, 0x6015 };
    bool is_ok = false;
    for (auto &serialPort : QSerialPortInfo::availablePorts()) {
        if (id.first == serialPort.vendorIdentifier() && id.second == serialPort.productIdentifier()) {
            port.setPortName(serialPort.portName());
            port.setBaudRate(QSerialPort::Baud115200);
            port.setStopBits(QSerialPort::OneStop);
            port.setDataBits(QSerialPort::Data8);
            port.setParity(QSerialPort::NoParity);
            port.open(QIODevice::ReadWrite);
            qDebug() << "kb.EG found on port" << serialPort.portName();
            is_ok = true;
            break;
        }
    }
    if (!is_ok) {
        results->LogString("Com port не найден");
        results->LogResult(SPAN_FAIL);
        d2xx->SpiSetPowerState(false);
        delete d2xx;
        started = false;
        return false;
    } else if (!port.isOpen()) {
        results->LogString(port.errorString());
        results->LogResult(SPAN_FAIL);
        d2xx->SpiSetPowerState(false);
        delete d2xx;
        started = false;
        return false;
    }

    QEventLoop  loop;
    QTimer      timer;
    bool is_signal_received = false;
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(&port, &QSerialPort::readyRead, [&is_signal_received, &loop, &port, this]{
        QByteArray result = port.readAll();
        qDebug() << result.toHex(' ').toUpper();
        qDebug() << (int)result[4] << (int)result[6] << (int)result[8];
        if (result.size() < 8)
            results->LogResult(SPAN_FAIL);
        else if ((int)result[4] == 0x11 && (int)result[6] == 0x22 && (int)result[8] == 0x33)
            results->LogResult(SPAN_OK);
        is_signal_received = true;
        loop.quit();
    });
    QByteArray to_send = QByteArray::fromHex("F00406A40003E441");
    qDebug() << "sent:" << to_send.toHex(' ').toUpper() << "size:" << to_send.size();
    port.write(to_send);
    if (!is_signal_received) {
        timer.start(10000);
        loop.exec();
        if (!timer.isActive()) {
            results->LogString("timeout");
            results->LogResult(SPAN_FAIL);
            d2xx->SpiSetPowerState(false);
            delete d2xx;
            started = false;
            return false;
        }
    }
    d2xx->SpiSetPowerState(false);
    QThread::sleep(1);
    delete d2xx;
    started = false;
    port.close();
    return true;
}

bool MainWindow::flash()
{
    if (started || results->isHidden())
        return false;
    results->LogHeader("Прошивка");
    started = true;
    D2xx *d2xx = new D2xx();
    d2xx->SpiSetPowerState(true);
    QThread::sleep(4);
    uint32_t crc;

    if (ui->codesys->isChecked()) {
        qDebug() << "reading codesys registers";
        crc = getCodesysLicense();
        if (crc == 0xFFFFFFFF) {
            results->LogString("Не удалось получить код лицензии");
            results->LogResult(SPAN_FAIL);
            d2xx->SpiSetPowerState(false);
            delete d2xx;
            started = false;
            return false;
        } else
            results->LogString("Код лицензии: " + QString::number(crc, 16).toUtf8());
    } else {
        qDebug() << "codesys checkbox is not checked";
        crc = 0xFFFFFFFF;
    }

    qDebug() << "crc" << QByteArray::fromHex(QString::number(crc, 16).toUtf8());

    QStringList info = { ui->cipher->text(),
                         ui->serialnum->text(),
                         ui->modification->currentText(),
                         ui->module->currentText().split(" - ")[0],
                         ui->mac->text(),
                         QString::number(crc),
                       };
    if (!Form_bin_file(info)) {
        qDebug() << "Could not generate cipher.bin";
        d2xx->SpiSetPowerState(false);
        delete d2xx;
        started = false;
        return false;
    }
//                    -c \"program ../firmware/ip.bin verify 0x100FC060\"
    QEventLoop loop;
    QProcess *process = new QProcess();
    QString oocd_dir = "flash\\oocd";
    process->setWorkingDirectory(oocd_dir);
    QString cmd = oocd_dir + "\\openocd.exe";
    int type = ui->module->currentText().split(" - ")[0].toInt();
    QString args = QString("-f target\\komega_basic\\init_erase.cfg \
                    -c \"program ../firmware/") + QString::number(type) + QString("b.hex verify\" \
                    -c \"program ../firmware/cipher.bin verify 0x100FE000\" \
                    -c \"program ../firmware/") + QString::number(type) + QString(".hex verify\" \
                    -f target\\komega_basic\\finish.cfg");
    bool is_ok = true;

    connect(process, &QProcess::errorOccurred, this, [this, &is_ok, &loop](QProcess::ProcessError error){
        qDebug() << "QProcess error" << error;
        QString err = error == QProcess::ProcessError::FailedToStart ? "Ошибка запуска" : "Неизвесная ошибка";
        is_ok = false;
        results->LogString(err);
        results->LogResult(SPAN_FAIL);
        loop.quit();
    });

    auto lambda = connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, &is_ok, &loop, &process](int exitCode, QProcess::ExitStatus exitStatus){
        QByteArray res = process->readAllStandardError();
        qDebug() << "result:\n" << res;
        QString dir_name = "//A2/kT.teststand.logs/" + QHostInfo::localHostName() + "/oocd/";
        QString filename = "dl_" + QDateTime::currentDateTime().toString("d-MMM-yyyy_hh-mm-ss") + ".txt";

        if (!exitCode)
            results->LogResult(SPAN_OK);
        else {
            results->LogString(QString(res));
            results->LogResult(SPAN_FAIL);
        }
        is_ok = exitCode == 0;
        qDebug() << "process finished with exitCode" << exitCode << "exitStatus" << exitStatus;
        loop.quit();
    });

    process->setNativeArguments(args);
    process->setProgram(cmd);
    process->start();
    if (process->state() == QProcess::Running)
        loop.exec();
    d2xx->SpiSetPowerState(false);
    QThread::sleep(1);
    delete d2xx;
    started = false;
    return is_ok;
}

bool MainWindow::validateSettings()
{
    if (ui->flash->isChecked()) {
        if (ui->flash->isChecked()) {
            if (ui->mac->text().split(":").size() != 6) {
                QMessageBox::critical(this, "Ошибка", "Неправильный mac-адрес");
                return false;
            }
//            else if (ui->cipher->text().size() != 17) {
//                QMessageBox::critical(this, "Ошибка", "Шифр должен состоять из 17 символов");
//                return false;
//            }
        }
    }
    return true;
}

void MainWindow::on_start_clicked()
{
    QTime start_time = QTime::currentTime();
    QTime elapsed(0,0);
    QTime finish_time;
    QApplication::restoreOverrideCursor();
    if (!validateSettings())
        return;

    results_window->hide();
    results->Start();
    results_window->setFocus();
    results_window->show();
    results->LogString("Тест начался");
    bool is_ok = true;
    if (is_ok && ui->flash->isChecked())
        is_ok = flash();
    if (is_ok && ui->test->isChecked())
        is_ok = start();
    if (is_ok && ui->codesys->isChecked() && ui->flash->isChecked())
        is_ok = codesysFirmware();
    if (is_ok && ui->codesys->isChecked())
        is_ok = checkCodesys();
    results->Result();
    finish_time = QTime::currentTime();
    elapsed = elapsed.addSecs(start_time.secsTo(finish_time));
    results->append("\nПродолжительность теста: " + elapsed.toString("mm:ss"));
    results->ScrollToEnd();
    results->setTextInteractionFlags(Qt::TextBrowserInteraction);

}


void MainWindow::on_modification_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    int modif = ui->modification->currentText().toInt();
    if (modif == 2)
        ui->codesys->setEnabled(true);
    else {
        ui->codesys->setCheckState(Qt::Unchecked);
        ui->codesys->setEnabled(false);
    }
    ui->cipher->setText(cipher[modif - 1]);

}

