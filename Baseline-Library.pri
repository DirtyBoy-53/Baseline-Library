INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# 使用预定义来屏蔽功能
# eg: DEFINES += ENABLE_YLOGGER_FEATURE ENABLE_JSON_FEATURE

contains(DEFINES, ENABLE_JSON_FEATURE) {
    include(NlohmannJson/Json.pri)
}
contains(DEFINES, ENABLE_YLOGGER_FEATURE) {
    include(YLogger/YLogger.pri)
}
contains(DEFINES, ENABLE_YAML_FEATURE) {
    include(yaml-cpp/yaml-cpp.pri)
}
contains(DEFINES, ENABLE_CSERIALPORT_FEATURE) {
    include(CSerialPort/CSerialPort.pri)
}
contains(DEFINES, ENABLE_UTIL_FEATURE) {
    include(Util/Util.pri)
}
contains(DEFINES, ENABLE_YPARAMBROWSER_FEATURE) {
    include(YParamBrowser/YParamBrowser/YParamBrowser.pri)
}
contains(DEFINES, ENABLE_ELALIB_FEATURE) {
    include(ElaLib/ElaLib.pri)
}
