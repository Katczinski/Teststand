#ifndef RESULTS_H
#define RESULTS_H

#include <QQuickView>
#include <QWidget>
#include <QVBoxLayout>
#include "testscounter.h"
#include "results_logger.h"

class Results_Window : public QWidget
{
    Q_OBJECT
public:
    Results_Window(Results_Logger* pLogger);
    ~Results_Window();
    QQuickView  *view;
    QVBoxLayout *right_layout;
    QVBoxLayout *left_layout;
    QWidget     *container;
    TestsCounter tests_counter;
    QImage GrabScreenshot();
private:
    Results_Logger  *results_logger;
    QHBoxLayout     *main_layout;
    void closeEvent(QCloseEvent *event) override;
public slots:
    void scrollLoggerToLine(int line);

signals:
    void sendToQml(QString type, int subtype, int num, bool is_good, int line_num);
    void windowClosed();
};

#endif // RESULTS_H
