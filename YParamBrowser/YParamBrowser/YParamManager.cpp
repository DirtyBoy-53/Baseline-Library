#include "YParamManager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QDir>

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <QDebug>
#include <exception>

#include "qttreepropertybrowser.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "YParamFactory.hpp"

using namespace YParamBrowser;
YParamManager::YParamManager(QObject *parent)
    : QObject(parent)
    , mEditor(std::make_unique<QtTreePropertyBrowser>())
    , mVariantManager(std::make_unique<QtVariantPropertyManager>())
    , mVariantFactory(std::make_unique<QtVariantEditorFactory>())
{
    qRegisterMetaType<YParamEnum::ParamType>("YParamEnum::ParamType");
    initConnect();

    mSavePath = QDir::currentPath() + "/Config";
    QDir dir(mSavePath);
    if (!dir.exists()) {
        dir.mkpath(mSavePath);
    }
}

YParamManager::~YParamManager()
{

}

void YParamManager::addParam(YParamPtr param, const QString &group)
{
    const auto name = param.get()->getSParamName();
    if(name.isEmpty() || group.isEmpty()) {
        throw YParamBrowserException(QString("Unable to add parameter").toStdString());
    } else {
        mGroupParamMap[group][param.get()->getSParamName()] = param;
    }
}

void YParamManager::updateParam()
{
    // 1. 读取本地文件
    // 1.1. 如果有内容
    // 1.1.1 暂存读取的值，看软件缓存中的参数是否包含每一项
    // 1.1.2 包含则将缓存中的值更新为读取值
    // 1.2. 无内容
    // 1.2.1 直接使用缓存中的参数即可（相当于初次创建）

    // 2. 导出一次参数到本地(刷新一次所有参数)


    // 读取本地文件
    YGroupParamMap groupParam;
    importParam(groupParam);

    // 根据是否由内容判断是否需要更新缓存数据
    if (groupParam.size()) {
        for (auto titleKey : groupParam.keys()) {
            for (auto key : groupParam[titleKey].keys()) {
                if (mGroupParamMap.contains(titleKey) &&
                    mGroupParamMap[titleKey].contains(key)) {
                    mGroupParamMap[titleKey][key] = groupParam[titleKey][key];
                }
            }
        }
    }

    // 刷新参数到本地
    exportParam();
}


void YParamManager::getParamWidget(QtTreePropertyBrowser *&browser)
{
    browser = mEditor.get();
}

template<typename T>
T YParamManager::getParamValue(const QString &name)
{
    QVariant value = QVariant(QVariant::UserType);

    for (auto outerIt = mGroupParamMap.constBegin(); outerIt != mGroupParamMap.constEnd(); ++outerIt) {
        const YParamMap& innerMap = outerIt.value();
        if (innerMap.contains(name)) {
            value = innerMap.value(name).get()->getVParamValue();
            break;
        }
    }

    if (std::is_same<T,         int>::value         &&
                    value.type() == QVariant::Int) {
        return value.toInt();
    } else if (std::is_same<T,  double>::value      &&
                    value.type() == QVariant::Double) {
        return value.toDouble();
    } else if (std::is_same<T,  QString>::value     &&
                    value.type() == QVariant::String) {
        return value.toString();
    } else if (std::is_same<T,  bool>::value        &&
                    value.type() == QVariant::Bool) {
        return value.toBool();
    }
    else {
        throw YParamBrowserException("Incompatible type or conversion failed.");
    }
    return T();
}

void traverseAllElements(const QDomNode& node) {
    // 如果当前节点是元素节点，则输出该元素的信息
    if (node.isElement() ) {
        QDomElement element = node.toElement();

        if (element.tagName().contains("ParamType")) {
            try {
                auto paramPtr = YParamFactory::createParameter(element.tagName());
                paramPtr->fromDom(element);
//                qDebug() << paramPtr.get()->toString();
            } catch (const YParamBrowserException& e) {
                qDebug() << QString(e.what());
            }

        }


        // // 打印该元素的属性
        // QDomNamedNodeMap attributes = element.attributes();
        // for (int i = 0; i < attributes.count(); ++i) {
        //     QDomNode attr = attributes.item(i);
        //     qDebug() << "  Attribute:" << attr.nodeName() << "=" << attr.nodeValue();
        // }

        // // 打印该元素的文本内容
        // if (!element.text().isEmpty()) {
        //     qDebug() << "  Text:" << element.text();
        // }
    }

    // 遍历当前节点的子节点
    QDomNode child = node.firstChild();
    while (!child.isNull()) {
        traverseAllElements(child); // 递归遍历每个子节点
        child = child.nextSibling(); // 获取下一个兄弟节点
    }
}
bool loadXmlFromFile(const QString& filePath, QDomDocument& doc) {
    QFile file(filePath);

    if (!file.exists()) {
        qDebug() << "File does not exist!";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file!";
        return false;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    if (!doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "Failed to load XML content!";
        qDebug() << "Error Message:" << errorMsg;
        qDebug() << "Error Line:" << errorLine;
        qDebug() << "Error Column:" << errorColumn;
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool YParamManager::parseXML(const QString &filePath, YGroupParamMap &group)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open the file! "  << filePath;
        return false;
    }

    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorColumn;
    if (!doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "Failed to load XML content!";
        qDebug() << "Error Message:" << errorMsg;
        qDebug() << "Error Line:" << errorLine;
        qDebug() << "Error Column:" << errorColumn;
        file.close();
        return false;
    }

    QDomElement root = doc.documentElement();  // 获取根节点（<root>）
    QDomNodeList childNodes = root.childNodes();  // 获取所有子节点

    for (int i = 0; i < childNodes.count(); ++i) {
        QDomNode node = childNodes.at(i);
        if (node.isElement()) {  // 确保是元素节点
            QDomElement element = node.toElement();

            // 遍历每个元素节点的子元素
            QDomNodeList paramChildren = element.childNodes();
            for (int j = 0; j < paramChildren.count(); ++j) {
                QDomNode paramNode = paramChildren.at(j);
                YParamMap param;
                // 处理每个子元素
                if (paramNode.isElement()) {
                    QDomElement paramEle = paramNode.toElement();
                    if (paramEle.tagName().contains("ParamType")) {
                        try {
                            auto paramPtr = YParamFactory::createParameter(paramEle.tagName());
                            paramPtr->fromDom(paramEle);
                            group[element.tagName()][paramPtr.get()->getSParamName()] = paramPtr;
                            qDebug() << paramPtr.get()->toString();
                        } catch (const YParamBrowserException& e) {
                            qDebug() << QString(e.what());
                        }

                    }
                }
                if (!param.isEmpty()) {

                }
            }
        }
    }
    return true;
}

bool YParamManager::importParam(YGroupParamMap &param)
{
    YGroupParamMap groupParam;
    parseXML(mSavePath + "/" + mFileName + ".xml", groupParam);
    if(!groupParam.isEmpty()) {
        param = groupParam;
    }else{
        return false;
    }
    return true;
}

bool YParamManager::exportParam()
{
    QFile file(mSavePath + "/" + mFileName + ".xml");//打开或新建xml文件
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)) {//Truncate表示清空原来的内容
        QMessageBox::warning(nullptr,"错误","文件打开失败");
        return false;
    }

    QDomDocument doc;
    //写入xml头部
    QDomProcessingInstruction instruction;//添加处理指令
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);

    for (auto outerIt = mGroupParamMap.constBegin(); outerIt != mGroupParamMap.constEnd(); ++outerIt) {
        const YParamMap& innerMap = outerIt.value();
        QDomElement paramNode = doc.createElement(outerIt.key());
        root.appendChild(paramNode);

        for (auto paramIt = innerMap.constBegin(); paramIt != innerMap.constEnd(); ++paramIt) {
            const YParamPtr paramPtr = paramIt.value();
            QDomElement paramEle = paramPtr.get()->toDom(doc);
            if (paramEle.isNull()) {
                qDebug() << "XML-Key:" << paramIt.key();
            } else {
                qDebug() << "XML-Value:" << paramEle.tagName();
            }
            paramNode.appendChild(paramEle);
        }
    }
//    qDebug() <<file.fileName() << doc.toString();
    //输出文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4);//缩进4格
    file.close();

    return true;
}

void YParamManager::initConnect()
{

    connect(mVariantManager.get(), &QtVariantPropertyManager::valueChanged, [=](QtProperty *property, const QVariant &val){
        if (!mValueChangeSigEnable) return; // 在未初始化完成之前,值发生变化不响应
        qDebug() << __func__  << "  ValueChange "<< property->propertyName() << ":" << val.toString();

    for (auto outerIt = mGroupParamMap.constBegin(); outerIt != mGroupParamMap.constEnd(); ++outerIt) {
        // const QString& groupKey = outerIt.key();      // 获取组的键
        const YParamMap& innerMap = outerIt.value();  // 获取组内的键值对

        // 遍历每个组内的参数
        for (auto innerIt = innerMap.constBegin(); innerIt != innerMap.constEnd(); ++innerIt) {
            const QString& paramKey = innerIt.key();  // 获取参数的键
            const YParamPtr& paramValue = innerIt.value();  // 获取参数的值
            if(paramKey == property->propertyName()){
                paramValue.get()->setVParamValue(val.toString(), property);
            }
        }
    }

    });
}

int getTypeByParamType(YParamEnum::ParamType type){
    switch(type) {
    case YParamEnum::ParamType_Bool         : return QVariant::Bool;
    case YParamEnum::ParamType_File         : return QtVariantPropertyManager::fileBoxTypeId();
    case YParamEnum::ParamType_Folder       : return QtVariantPropertyManager::fileBoxTypeId();
    case YParamEnum::ParamType_Int          : return QVariant::Int;
    case YParamEnum::ParamType_Qreal       : return QVariant::Double;
    case YParamEnum::ParamType_String       : return QVariant::String;
    case YParamEnum::ParamType_PassWordStr  : return QVariant::String;
    case YParamEnum::ParamType_Enum         : return QtVariantPropertyManager::enumTypeId();
    case YParamEnum::ParamType_Date         : return QVariant::Date;
    case YParamEnum::ParamType_Time         : return QVariant::Time;
    case YParamEnum::ParamType_DateTime     : return QVariant::DateTime;
    case YParamEnum::ParamType_KeySeq       : return QVariant::KeySequence;
    case YParamEnum::ParamType_Locale       : return QVariant::Locale;
    case YParamEnum::ParamType_Point        : return QVariant::Point;
    case YParamEnum::ParamType_PointF       : return QVariant::PointF;
    case YParamEnum::ParamType_Size         : return QVariant::Size;
    case YParamEnum::ParamType_SizeF        : return QVariant::SizeF;
    case YParamEnum::ParamType_Rect         : return QVariant::Rect;
    case YParamEnum::ParamType_RectF        : return QVariant::RectF;
    case YParamEnum::ParamType_Flag         : return QtVariantPropertyManager::flagTypeId();
    case YParamEnum::ParamType_SizePolicy   : return QVariant::SizePolicy;
    case YParamEnum::ParamType_Font         : return QVariant::Font;
    case YParamEnum::ParamType_Color        : return QVariant::Color;
    }
    return 0;
}

QtVariantProperty * getProperty(QtVariantPropertyManager *variantManager, YParamPtr param, const bool &isEnable = true)
{
    QtVariantProperty *item = variantManager->addProperty(getTypeByParamType(param.get()->getEParamType()), param.get()->getSParamName());
    item->setValue(param.get()->getVParamValue());
    item->setToolTip(param.get()->getSParamTip());
    item->setEnabled(isEnable);
    // 属性设置
    if (param.get()->getEParamType() == YParamEnum::ParamType_File) {
        item->setAttribute(QLatin1String("setFilePathType"), true);
    } else if (param.get()->getEParamType() == YParamEnum::ParamType_Folder) {
        item->setAttribute(QLatin1String("setFilePathType"), false);
    } else if (param.get()->getEParamType() == YParamEnum::ParamType_Int) {
        auto rangePair = param.get()->getVParamRange().value<QPair<int,int>>();
        if (rangePair.first || rangePair.second) {
            item->setAttribute(QLatin1String("minimum"), rangePair.first);
            item->setAttribute(QLatin1String("maximum"), rangePair.second);
        }
        int step = std::abs(rangePair.first-rangePair.second) > 1000 ? 10 : 1;
        item->setAttribute(QLatin1String("singleStep"), step);
    } else if (param.get()->getEParamType() == YParamEnum::ParamType_Qreal) {
        auto rangePair = param.get()->getVParamRange().value<QPair<qreal,qreal>>();
        if (rangePair.first || rangePair.second) {
            item->setAttribute(QLatin1String("minimum"), rangePair.first);
            item->setAttribute(QLatin1String("maximum"), rangePair.second);
        }
        int step = std::abs(rangePair.first-rangePair.second) > 1000 ? 10 : 1;
        item->setAttribute(QLatin1String("singleStep"), step);
    } else if (param.get()->getEParamType() == YParamEnum::ParamType_PassWordStr) {
        item->setAttribute(QLatin1String("echoMode"), QLineEdit::PasswordEchoOnEdit);
    } else if (param.get()->getEParamType() == YParamEnum::ParamType_Enum) {
        item->setAttribute(QLatin1String("enumNames"), param.get()->getVParamRange());
        item->setValue(param.get()->getVParamValue());
    } else if (param.get()->getEParamType() == YParamEnum::ParamType_Flag) {
        item->setAttribute(QLatin1String("flagNames"), param.get()->getVParamRange());
        item->setValue(param.get()->getVParamValue());
    }

    return item;
}

void YParamManager::on_update_ui(const bool &isEnable)
{
    // 遍历mParamMap 更新到界面
    mValueChangeSigEnable = false;
    mVariantManager.get()->clear();
    mEditor.get()->clear();

    for (auto outerIt = mGroupParamMap.constBegin(); outerIt != mGroupParamMap.constEnd(); ++outerIt) {
        const QString& groupKey = outerIt.key();      // 获取组的键
        const YParamMap& innerMap = outerIt.value();  // 获取组内的键值对
        QtProperty *topItem = mVariantManager.get()->addProperty(QtVariantPropertyManager::groupTypeId(), groupKey);

        qDebug() << __func__ << ":" << innerMap.size();
        // 遍历每个组内的参数
        for (auto innerIt = innerMap.constBegin(); innerIt != innerMap.constEnd(); ++innerIt) {
            const QString& paramKey = innerIt.key();  // 获取参数的键
            const YParamPtr& paramValue = innerIt.value();  // 获取参数的值
            qDebug() << __func__ << ":" << paramKey << paramValue->toString();
            topItem->addSubProperty(getProperty(mVariantManager.get(), paramValue, isEnable));
        }

        mEditor->setFactoryForManager(mVariantManager.get(), mVariantFactory.get());
        mEditor->addProperty(topItem);
        mEditor->setPropertiesWithoutValueMarked(true);
        mEditor->setRootIsDecorated(false);
    }

    mValueChangeSigEnable = true; 

}

void YParamManager::setGroupParamMap(const YGroupParamMap &groupParamMap)
{
    clearGroupParamMap();
    mGroupParamMap = groupParamMap;
}

void YParamManager::clearGroupParamMap()
{
    mGroupParamMap.clear();
}

YParamManager::YGroupParamMap YParamManager::groupParamMap() const
{
    return mGroupParamMap;
}

void YParamManager::setSavePath(const QString &newSavePath)
{
    mSavePath = newSavePath;
    QDir dir(mSavePath);
    if (!dir.exists()) {
        dir.mkpath(mSavePath);
    }
}

void YParamManager::setConfigName(const QString &newName)
{
    mFileName = newName;
}

