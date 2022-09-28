#ifndef TESTSCOUNTER_H
#define TESTSCOUNTER_H
#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class TestsCounter : public QWidget
{


public:
    QWidget w;
    int success = 0, fail = 0;
    TestsCounter();
    void Reset();
    void AddTest(bool is_ok);

    bool Result();
    struct TABLE_ITEMS {
        QTableWidgetItem *total;
        QTableWidgetItem *success;
        QTableWidgetItem *fail;
        QTableWidgetItem *result;
    }table_items;
    QPushButton *button_restart = new QPushButton("Перезапуск", this);
    QPushButton *button_close = new QPushButton("Закрыть", this);
    QLabel *buttons_label = new QLabel("", this);

};

#endif // TESTSCOUNTER_H
