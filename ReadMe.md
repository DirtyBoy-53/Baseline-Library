**ToolTip**

描述：Qt 桌面弹窗，具有动画效果，可从屏幕右下角弹出，亦可从屏幕中央弹出，Windows/Linux适用。

**Queue**

描述：C++ 模板队列，便于存储和读取数据，Windows/Linux适用

**OpenCV**

描述：OpenCV450，Windows适用

**spdlog**
描述：C++日志记录，Windows/Linux适用
调用方法，搭配log.cpp和log.h，调用initLog后，使用Log::Logger()->info("hello {}!","world");

**QSS**
描述：Qt美化功能，将QSS中的图片和QSS文件添加到资源文件中，最好都在根目录（图片调用都是用的相对路径，所以最好在根目录，避免修改），Windows/Linux适用
在main.cpp中使用

#include <QFile>

void addQssFile(QApplication &a,QString &&file)
{
    QFile qss(file);
    if( qss.open(QFile::ReadOnly)){
        qDebug("open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else{
        qDebug("Open failed");
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    addQssFile(a,":/QSS/MacOS.qss");
    MainWindow w;
    w.show();
    return a.exec();
}

**Demp**

描述：Qt程序崩溃调试，可生成Dump文件后使用Vistual Studio打开，可直接定位到错误所在。 Windows适用
#include "CrashDump.h"
int main(int argc,char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    CrashDump::init("Crash");
    QApplication app(argc,argv);
    Window w;
    w.show();
    return app.exec();
}
