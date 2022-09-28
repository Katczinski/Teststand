#include "results_logger.h"
#include <QFile>
#include <QHostInfo>
#include <QCoreApplication>
#include <QDir>
#include <QPdfWriter>
#include <QPainter>
#include <QDateTime>
#include <QScrollBar>
#include "komega_type_to_text.h"
//#include "log_to_file.h"
#include "popup.h"
#include <iostream>
Results_Logger::Results_Logger(QTextEdit *parent) : QTextEdit(parent)
{
    setMinimumWidth(600);
    setFont(QFont("sans-serif", 10));
}
void Results_Logger::Start()
{
    clear();
    setTextInteractionFlags(Qt::NoTextInteraction);
}

void Results_Logger::Stop(/*QImage image, int type, bool result*/)
{
    setTextInteractionFlags(Qt::TextBrowserInteraction);

    auto curr_date_time = QDateTime::currentDateTime();

    QString base_file_name = curr_date_time.toString("hh.mm.ss");
//    base_file_name += "_" + Komega_type_to_text(type) + "_";
//    base_file_name += result ? "Passed" : "Failed";

    QString html_file_name =  base_file_name + ".html";
    QString image_file_name =  base_file_name + ".png";

    QString dir_name = "tests";
    dir_name += "/" + QString::number(curr_date_time.date().year());
    dir_name += "/" + QString::number(curr_date_time.date().month());
    dir_name += "/" + QString::number(curr_date_time.date().day());

    QString image_dir_base_name = "Pics";
    QString image_dir_full_name = dir_name + "/" + image_dir_base_name;

    auto html = this->toHtml(); //getting html of QTextEdit field
    int index = html.indexOf("<p style="); //we need to insert image link to html as div

    QString div = "<div style = \"position: fixed;";
    div += "top: 0;";
    div += "right: 0;";
    div += "width: 500px;\">";
    div += "<img src='";
    div += image_dir_base_name + "/" + image_file_name;
    div += "'></div>\n";
    html.insert(index, div);

//    Log_To_File html_logger(dir_name, html_file_name);
//    if(!html_logger.LogString(html)){
//        Popup(fail_message);
//        return;
//    } else
//        std::cerr << "test_result: //A2/kT.teststand.logs/" + QHostInfo::localHostName().toStdString() + "/" + dir_name.toStdString() + "/" + html_file_name.toStdString() << std::endl;

//    Log_To_File image_logger(image_dir_full_name, image_file_name);
//    if(!image_logger.LogImage(image))
//        Popup(fail_message);
}

void Results_Logger::LogResult(const QString &text)
{
    append("<span style='color: darkgrey'>" +
           QDateTime::currentDateTime().toString("mm.ss: ") +
           "</span>" +
           text + "<br>");
}

void Results_Logger::LogString(const QString &text)
{
    append("<span style='color: darkgrey'>" +
           QDateTime::currentDateTime().toString("mm.ss: ") +
           "</span>" +
           text);
}

void Results_Logger::LogHeader(const QString& text)
{
    append("<span style='font-size: 14px; color: darkblue'>" + text + "</span>");
    textCursor().movePosition(QTextCursor::End);
}

void Results_Logger::ScrollToLine(int line)
{
    verticalScrollBar()->setValue(line);
}
void Results_Logger::ScrollToEnd()
{
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}
void Results_Logger::ScrollToBeginning()
{
    verticalScrollBar()->setValue(verticalScrollBar()->minimum());
}
