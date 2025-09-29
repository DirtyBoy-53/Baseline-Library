INCLUDEPATH += "$$PWD/include"
INCLUDEPATH += "$$PWD/include/CSerialPort"

HEADERS +=  $$PWD/include/CSerialPort/ibuffer.hpp\
#            $$PWD/include/CSerialPort/import.bat\
            $$PWD/YSerialPort.h \
            $$PWD/include/CSerialPort/ithread.hpp\
            $$PWD/include/CSerialPort/itimer.hpp\
            $$PWD/include/CSerialPort/iutils.hpp\
            $$PWD/include/CSerialPort/SerialPort.h\
            $$PWD/include/CSerialPort/SerialPortBase.h\
            $$PWD/include/CSerialPort/SerialPortInfo.h\
            $$PWD/include/CSerialPort/SerialPortInfoBase.h\
            $$PWD/include/CSerialPort/SerialPortListener.h\
            $$PWD/include/CSerialPort/SerialPort_global.h\
            $$PWD/include/CSerialPort/SerialPort_version.h
            # $$PWD/include/CSerialPort/YSerialPort.h

        win32{
            HEADERS +=$$PWD/include/CSerialPort/SerialPortInfoWinBase.h
            HEADERS +=$$PWD/include/CSerialPort/SerialPortWinBase.h
        }
        unix{
            HEADERS +=$$PWD/include/CSerialPort/SerialPortInfoUnixBase.h
            HEADERS +=$$PWD/include/CSerialPort/SerialPortUnixBase.h
        }
            
            
SOURCES +=  $$PWD/src/SerialPort.cpp\
            $$PWD/YSerialPort.cpp \
            $$PWD/src/SerialPortBase.cpp\
            $$PWD/src/SerialPortInfo.cpp\
            $$PWD/src/SerialPortInfoBase.cpp
            # $$PWD/src/YSerialPort.cpp

        win32{
            SOURCES += $$PWD/src/SerialPortInfoWinBase.cpp
            SOURCES += $$PWD/src/SerialPortWinBase.cpp
        }
        unix{
            SOURCES += $$PWD/src/SerialPortInfoUnixBase.cpp
            SOURCES += $$PWD/src/SerialPortUnixBase.cpp
        }
#add system libs
win32-msvc*:LIBS += advapi32.lib
win32-msvc*:LIBS += setupapi.lib
win32-g++:LIBS += libsetupapi
