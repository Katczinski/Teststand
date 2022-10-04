#ifndef RESULTS_LOGGER_H
#define RESULTS_LOGGER_H
#include <QTextEdit>
#include <QFile>

class Results_Logger : public QTextEdit
{
    Q_OBJECT
     QFile file;
public:
    explicit Results_Logger(QWidget *parent = nullptr);
    void Start();
    void Stop(/*QImage image, int type, bool result*/);
    void LogResult(const QString &text);
    void LogString(const QString &text);
    void LogHeader(const QString &text);
    void ScrollToLine(int line);
    void ScrollToBeginning();
    void ScrollToEnd();
private:
    const QString fail_message = "Не удалось записать логи тестирования";
};

#endif // RESULTS_LOGGER_H
