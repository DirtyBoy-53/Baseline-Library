# YParamBrowser 项目说明文档

## 概述
YParamBrowser 是一个基于 Qt 框架的参数浏览器，旨在提供一个简单易用的界面来管理和查看各种类型的参数。该项目支持多种数据类型，包括整数、字符串、浮点数、布尔值、文件路径、文件夹路径、日期、时间、日期时间、快捷键序列、区域设置、点、浮点点、大小、浮点大小、矩形、浮点矩形、标志、字体和颜色。

## 前置条件
- 确保您的开发环境中已安装 Qt 框架。
- 项目使用 C++ 编写，因此需要 C++ 编译器。

## 运行方法
1. 克隆或下载项目到本地。
2. 打开终端或命令提示符，导航到项目目录。
3. 运行构建命令（例如，对于 Qt 项目，可以使用 `qmake` 和 `make`）。
4. 执行生成的可执行文件。

## 构建方法
1. 确保已安装 Qt 开发环境。
2. 在项目根目录下，运行 `qmake` 生成 Makefile。
3. 使用 `make` 命令编译项目。
4. 编译成功后，在 `build` 目录中找到生成的可执行文件。

## 许可证
本项目遵循 MIT 许可证。请查看项目中的 LICENSE 文件了解更多信息。

## 注意事项
- 请确保在使用本项目时遵守所有相关的法律法规。
- 本项目中的代码示例仅供参考，实际使用时请根据具体需求进行调整。

## 贡献
如果您有任何改进意见或想要贡献代码，请随时提交 Pull Request 或创建 Issue。

## 联系方式
如有任何问题或建议，请通过项目的 Issue 跟踪系统或联系项目的维护者。

---

请注意，本 README 文件是基于提供的项目代码和假设的项目结构生成的。在实际项目中，可能需要根据项目的具体情况进行调整。


## 使用案例
### 使用带用户权限、按项目保存以及完整界面的接口
```C++
    auto paramWidget = new YParamBrowserWidget();   // 创建窗口
    
    // 构建默认参数
    std::vector<yparam_s> param;
    
    // 管理员参数
    param.emplace_back(yparam_s(Administrator, "系统参数", YParamBrowser::YParamEnum::ParamType_Int, 10, "开机时间", 20));
    param.emplace_back(yparam_s(Administrator, "系统参数", YParamBrowser::YParamEnum::ParamType_File, "", "脚本路径", ""));
    param.emplace_back(yparam_s(Administrator, "系统参数", YParamBrowser::YParamEnum::ParamType_Folder, "", "生成路径", ""));
    param.emplace_back(yparam_s(Administrator, "用户参数", YParamBrowser::YParamEnum::ParamType_Enum, "COM1", "串口号", "COM2", QStringList() << "COM1" << "COM2"));
    
    // 操作员参数
    param.emplace_back(yparam_s(Operator, "基础参数", YParamBrowser::YParamEnum::ParamType_Int, 10, "开机时间", 20));
    param.emplace_back(yparam_s(Operator, "基础参数", YParamBrowser::YParamEnum::ParamType_PointF, QPointF(1.1,1.2), "坐标", QPointF(1.1,1.2)));
    param.emplace_back(yparam_s(Operator, "基础参数", YParamBrowser::YParamEnum::ParamType_Date, QDate::currentDate(), "北京时间", QDate::currentDate()));
    param.emplace_back(yparam_s(Operator, "基础参数", YParamBrowser::YParamEnum::ParamType_Color, QColor(100,100,100,50), "颜色", QColor(100,100,100,50)));
    
    // 添加参数
    paramWidget->addParams(param);
    
    // 更新参数到界面
    paramWidget->slotUpdateTableWidget();

```
### 使用最基础的接口
```C++
    using namespace YParamBrowser;
    YParamManager *manager = new YParamManager();
    QVBoxLayout *layout = new QVBoxLayout(centralWidget());
    
    YParamPtr param1 = YParamFactory::createParameter(
            YParamEnum::ParamType_Int,
            QVariant(11),
            QVariant(10),
            QVariant::fromValue(QPair<int,int>(0,100)),
            "Test1",
            "Test1_Tips"
        );
        
    YParamPtr param2 = YParamFactory::createParameter(
            YParamEnum::ParamType_Int,
            QVariant(12),
            QVariant(10),
            QVariant::fromValue(QPair<int,int>(0,100)),
            "Test2",
            "Test2_Tips"
        );
        
    YParamPtr param3 = YParamFactory::createParameter(
        YParamEnum::ParamType_String,
        QVariant("Hello YParam"),
        QVariant("string"),
        QVariant(),
        "Test3",
        "Test3_Tips"
        );
        
    YParamPtr param4 = YParamFactory::createParameter(
        YParamEnum::ParamType_Double,
        QVariant(3.1415926),
        QVariant(0.0),
        QVariant(),
        "Test4",
        "Test4_Tips"
        );
        
    YParamPtr param5 = YParamFactory::createParameter(
        YParamEnum::ParamType_PassWordStr,
        QVariant(12345678),
        QVariant(1),
        QVariant(),
        "Test5",
        "Test5_Tips"
        );
        
    YParamPtr param6 = YParamFactory::createParameter(
        YParamEnum::ParamType_Enum,
        QVariant(2),
        QVariant(2),
        QVariant(QStringList() << "banana" << "orange" << "apple" << "grape"),
        "Test6",
        "Test6_Tips"
        );
        
    YParamPtr param7 = YParamFactory::createParameter(
        YParamEnum::ParamType_Bool,
        QVariant(false),
        QVariant(false),
        QVariant(),
        "Test7",
        "Test7_Tips"
        );
        
    YParamPtr param8 = YParamFactory::createParameter(
        YParamEnum::ParamType_File,
        QVariant("D:/file.txt"),
        QVariant("D:/file.txt"),
        QVariant(),
        "Test8",
        "Test8_Tips"
        );
        
    YParamPtr param9 = YParamFactory::createParameter(
        YParamEnum::ParamType_Folder,
        QVariant("D:/"),
        QVariant("D:/"),
        QVariant(),
        "Test9",
        "Test9_Tips"
        );
        
    YParamPtr param10 = YParamFactory::createParameter(
        YParamEnum::ParamType_Date,
        QVariant("2025-01-11"),
        QVariant(QDate::currentDate()),
        QVariant(),
        "Test10",
        "Test10_Tips"
        );
        
    YParamPtr param11 = YParamFactory::createParameter(
        YParamEnum::ParamType_Time,
        QVariant("15:35:11"),
        QVariant(QTime::currentTime()),
        QVariant(),
        "Test11",
        "Test11_Tips"
        );
        
    YParamPtr param12 = YParamFactory::createParameter(
        YParamEnum::ParamType_DateTime,
        QVariant("2025-01-11 15:39:52.768"),
        QVariant(QDateTime::currentDateTime()),
        QVariant(),
        "Test12",
        "Test12_Tips"
        );
        
    YParamPtr param13 = YParamFactory::createParameter(
        YParamEnum::ParamType_KeySeq,
        QVariant("Ctrl+Q"),
        QVariant(QVariant::fromValue(QKeySequence(Qt::ControlModifier | Qt::Key_Q))),
        QVariant(),
        "Test13",
        "Test13_Tips"
        );
        
    YParamPtr param14 = YParamFactory::createParameter(
        YParamEnum::ParamType_Locale,
        QVariant("zh_CN"),
        QVariant(QLocale(QLocale::Polish, QLocale::Poland)),
        QVariant(),
        "Test14",
        "Test14_Tips"
        );
        
    YParamPtr param15 = YParamFactory::createParameter(
        YParamEnum::ParamType_Point,
        QVariant(QPoint(1,1)),
        QVariant(QPoint(2,2)),
        QVariant(),
        "Test15",
        "Test15_Tips"
        );
        
    YParamPtr param16 = YParamFactory::createParameter(
        YParamEnum::ParamType_PointF,
        QVariant(QPointF(1.234,1.3333)),
        QVariant(QPointF(2.23456,2.34534)),
        QVariant(),
        "Test16",
        "Test16_Tips"
        );
        
    YParamPtr param17 = YParamFactory::createParameter(
                YParamEnum::ParamType_Size,
                QVariant(QSize(1,1)),
                QVariant(QSize(2,2)),
                QVariant(),
                "Test17",
                "Test17_Tips"
                );
                
    YParamPtr param18 = YParamFactory::createParameter(
                YParamEnum::ParamType_SizeF,
                QVariant(QSizeF(1.11,2.22)),
                QVariant(QSizeF(3.33,4.44)),
                QVariant(),
                "Test18",
                "Test18_Tips"
                );
                
    YParamPtr param19 = YParamFactory::createParameter(
                YParamEnum::ParamType_Rect,
                QVariant(QRect(1,2,3,4)),
                QVariant(QRect(1,2,3,4)),
                QVariant(),
                "Test19",
                "Test19_Tips"
                );
                
    YParamPtr param20 = YParamFactory::createParameter(
                YParamEnum::ParamType_RectF,
                QVariant(QRectF(1.1,2.2,3.3,4.4)),
                QVariant(QRectF(1.2,2.3,3.4,4.5)),
                QVariant(),
                "Test20",
                "Test20_Tips"
                );
                
    YParamPtr param21 = YParamFactory::createParameter(
                YParamEnum::ParamType_Flag,
                QVariant(1),
                QVariant(2),
                QVariant(QStringList() << "t1" << "t2" << "t3"),
                "Test21",
                "Test21_Tips"
                );
                
    YParamPtr param22 = YParamFactory::createParameter(
                YParamEnum::ParamType_Font,
                QVariant(QFont()),
                QVariant(QFont()),
                QVariant(),
                "Test22",
                "Test22_Tips"
                );
                
    YParamPtr param23 = YParamFactory::createParameter(
                YParamEnum::ParamType_Color,
                QVariant(QColor(100,100,100,50)),
                QVariant(QColor()),
                QVariant(),
                "Test23",
                "Test23_Tips"
                );
                
    manager->addParam(param1, "IntParam");
    manager->addParam(param2, "IntParam");
    manager->addParam(param3, "StringParam");
    manager->addParam(param4, "DoubleParam");
    manager->addParam(param5, "PassWordStr");
    manager->addParam(param6, "Enum");
    manager->addParam(param7, "Bool");
    manager->addParam(param8, "File");
    manager->addParam(param9, "Folder");
    manager->addParam(param10, "Date");
    manager->addParam(param11, "Time");
    manager->addParam(param12, "DateTime");
    manager->addParam(param13, "KeySeq");
    manager->addParam(param14, "Locale");
    manager->addParam(param15, "Point");
    manager->addParam(param16, "PointF");
    manager->addParam(param17, "Size");
    manager->addParam(param18, "SizeF");
    manager->addParam(param19, "Rect");
    manager->addParam(param20, "RectF");
    manager->addParam(param21, "Flag");
    manager->addParam(param22, "Font");
    manager->addParam(param23, "Color");
    manager->layoutAddParamWidget(layout);
    manager->on_update_ui();
```
