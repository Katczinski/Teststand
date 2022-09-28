#include "popup.h"
#include <QWidget>
#include "QString"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

void Popup(QString text)
{
    QWidget *popup = new QWidget(nullptr, Qt::Dialog |
                                 Qt::CustomizeWindowHint |
                                 Qt::WindowCloseButtonHint |
                                 Qt::WindowTitleHint );
    popup->setWindowModality( Qt::WindowModal);
    popup->setWindowTitle(" ");
    popup->setAttribute(Qt::WA_DeleteOnClose, true);
    popup->resize(150, 90);

    QLabel* text_label = new QLabel(popup);

    QVBoxLayout* layout = new QVBoxLayout(popup);
    layout->addWidget(text_label);
    text_label->setFont(QFont("Arial", 14, QFont::Bold));
    text_label->setText(text);
    popup->show();
}
