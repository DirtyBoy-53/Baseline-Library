INCLUDEPATH += $$PWD/include




CONFIG(debug,debug|release){
    LIBS += -L$$PWD/lib -lElaWidgetToolsd
}
CONFIG(release,debug|release){
    LIBS += -L$$PWD/lib -lElaWidgetTools
}


RESOURCES += $$PWD/include/ElaWidgetTools.qrc

