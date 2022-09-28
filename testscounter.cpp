#include "testscounter.h"
#include <QHeaderView>
#include <QDebug>
TestsCounter::TestsCounter()
{
//    table_items.total   = new QTableWidgetItem("");
//    table_items.success = new QTableWidgetItem("");
//    table_items.success->setForeground(QBrush(QColor("green")));
//    table_items.fail    = new QTableWidgetItem("");
//    table_items.fail->setForeground(QBrush(QColor("red")));
//    table_items.result  = new QTableWidgetItem("");
    Reset();
//    auto table  = new QTableWidget(2, 2, this);
//    table->setItem(0, 0, new QTableWidgetItem("Тестов всего"));
//    table->setItem(1, 0, new QTableWidgetItem("Тестов успешно"));
//    table->setItem(2, 0, new QTableWidgetItem("Тестов провалено"));
//    table->setItem(0, 0, new QTableWidgetItem("Итоговый результат"));
//    table->setItem(0, 1, table_items.total);
//    table->setItem(1, 1, table_items.success);
//    table->setItem(2, 1, table_items.fail);
//    table->setItem(0, 1, table_items.result);

//    table->setColumnWidth(0, 180);
//    table->setColumnWidth(1, 150);
    auto font = QFont("Arial", 13, 0);
//    table->setFont(font);
    font.setWeight(75);
    Reset();
//    for (auto x : {table_items.total/*, table_items.fail, table_items.success*/})
//        x->setFont(font);
    buttons_label->setStyleSheet("QLabel {color : darkblue; }");
    buttons_label->setFont(QFont("Arial", 13, 0));
    buttons_label->setAlignment(Qt::AlignCenter);
    auto buttons_layout = new QHBoxLayout();
//    buttons_layout->addWidget(buttons_label);
    buttons_layout->addWidget(button_restart);
    buttons_layout->addWidget(button_close);

    auto main_layout = new QVBoxLayout(this);
//    main_layout->addWidget(table);
    main_layout->addWidget(buttons_label, Qt::AlignHCenter);
    main_layout->addLayout(buttons_layout);

//    table->verticalHeader()->setVisible(false);
//    table->horizontalHeader()->setVisible(false);
    this->setMaximumHeight(141);

//    table->setMinimumWidth(180 + 150 + 2);
//    table->setMaximumWidth(table->minimumWidth());
}

void TestsCounter::Reset()
{
//    success = fail = 0;
//    table_items.total->setText("0");
//    table_items.success->setText("0");
//    table_items.fail->setText("0");
//    table_items.result->setText("");
    buttons_label->setText("Тест исполняется");
}
void TestsCounter::AddTest(bool is_ok)
{
//    is_ok ? ++success : ++fail;
//    table_items.total->setText(QString::number(success + fail));
//    table_items.success->setText(QString::number(success));
//    table_items.fail->setText(QString::number(fail));
//    table_items.result->setText(fail == 0 ? "Тест исполняется" : "Тест не пройден");
//    auto color = fail == 0 ? QColor("blue") :  QColor("red");
//    table_items.result->setForeground(QBrush(color));
}
bool TestsCounter::Result()
{
//    if (fail == 0){
//        table_items.result->setText("Тест успешен");
//        table_items.result->setForeground(QBrush(QColor("green")));
//    } else {
//        table_items.result->setText("Тест не пройден");
//        table_items.result->setForeground(QBrush(QColor("red")));
//    }
    buttons_label->setText("Тест завершен");
    return (fail == 0);
}
