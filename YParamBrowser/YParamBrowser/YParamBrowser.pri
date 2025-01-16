INCLUDEPATH += $$PWD
QT += xml
include (../qtpropertybrowser/qtpropertybrowser.pri)
HEADERS += \
    $$PWD/YParamBrowserWidget.h \
    $$PWD/YParamFactory.hpp \
    $$PWD/YParamBase.hpp    \
    $$PWD/YParamManager.h \
    $$PWD/YParamXmlHandle.h \
    $$PWD/YParameters.h

SOURCES += \
    $$PWD/YParamBrowserWidget.cpp \
    $$PWD/YParamManager.cpp \
    $$PWD/YParamXmlHandle.cpp \
    $$PWD/YParameters.cpp

DISTFILES += \
    $$PWD/ReadMe.md
