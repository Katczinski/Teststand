#include "results_window.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFrame>
#include <QTextBlock>
#include <QScrollBar>

Results_Window::Results_Window(Results_Logger* pLogger)
    : results_logger(pLogger)
{
    this->setWindowTitle("Результат тестирования");

    view = new QQuickView();
    view->engine()->rootContext()->setContextProperty("results_window", this);
    container = QWidget::createWindowContainer(view, this);

    container = QWidget::createWindowContainer(view);
    container->setMinimumSize(500, 400);

    auto line = new QFrame();
    line->setGeometry(QRect());
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    left_layout = new QVBoxLayout();
    left_layout->addWidget(container);

    left_layout->addWidget(line);
    right_layout = new QVBoxLayout();

    main_layout = new QHBoxLayout(this);
    main_layout->addLayout(left_layout);
    main_layout->addLayout(right_layout);
}

Results_Window::~Results_Window()
{
    delete view;
    delete left_layout;
    delete right_layout;
}

QImage Results_Window::GrabScreenshot()
{
   for (int i = 0; i< 10; ++i)
       QApplication::processEvents();

   return view->grabWindow();
}
void Results_Window::scrollLoggerToLine(int line)
{
    if(line >= 0)// line == -1 means report was not performed
        results_logger->ScrollToLine(line);
}

void Results_Window::closeEvent(QCloseEvent *event)
{
    event->accept();
    qDebug() << "Results window has been closed";
    emit windowClosed();
}
