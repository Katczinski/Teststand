QT       += core gui widgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CodeEditor/QCXXHighlighter.cpp \
    CodeEditor/QCodeEditor.cpp \
    CodeEditor/QFramedTextAttribute.cpp \
    CodeEditor/QGLSLCompleter.cpp \
    CodeEditor/QGLSLHighlighter.cpp \
    CodeEditor/QJSONHighlighter.cpp \
    CodeEditor/QLanguage.cpp \
    CodeEditor/QLineNumberArea.cpp \
    CodeEditor/QLuaCompleter.cpp \
    CodeEditor/QLuaHighlighter.cpp \
    CodeEditor/QPythonCompleter.cpp \
    CodeEditor/QPythonHighlighter.cpp \
    CodeEditor/QStyleSyntaxHighlighter.cpp \
    CodeEditor/QSyntaxStyle.cpp \
    CodeEditor/QXMLHighlighter.cpp \
    flash/form_bin_file.cpp \
    flash/get_adv_cipher.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CodeEditor/QCXXHighlighter.hpp \
    CodeEditor/QCodeEditor.hpp \
    CodeEditor/QFramedTextAttribute.hpp \
    CodeEditor/QGLSLCompleter.hpp \
    CodeEditor/QGLSLHighlighter.hpp \
    CodeEditor/QHighlightBlockRule.hpp \
    CodeEditor/QHighlightRule.hpp \
    CodeEditor/QJSONHighlighter.hpp \
    CodeEditor/QLanguage.hpp \
    CodeEditor/QLineNumberArea.hpp \
    CodeEditor/QLuaCompleter.hpp \
    CodeEditor/QLuaHighlighter.hpp \
    CodeEditor/QPythonCompleter.hpp \
    CodeEditor/QPythonHighlighter.hpp \
    CodeEditor/QStyleSyntaxHighlighter.hpp \
    CodeEditor/QSyntaxStyle.hpp \
    CodeEditor/QXMLHighlighter.hpp \
    flash/form_bin_file.h \
    flash/get_adv_cipher.h \
    mainwindow.h

FORMS += \
    mainwindow.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin

!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/CodeEditor $$PWD/flash

RESOURCES += \
    CodeEditor/resources/qcodeeditor_resources.qrc

DISTFILES += \
    CodeEditor/resources/default_style.xml \
    CodeEditor/resources/languages/cpp.xml \
    CodeEditor/resources/languages/glsl.xml \
    CodeEditor/resources/languages/lua.xml \
    CodeEditor/resources/languages/python.xml
