#ifndef YPARAMBASE_HPP
#define YPARAMBASE_HPP

#include <QString>
#include <QVariant>
#include <exception>
#include <string>
#include <QDomDocument>
#include <QObject>
#include <QMetaEnum>
#include <memory>
#include <QDebug>
#include <QtProperty>
// 名称：参数基类
// 描述：所有参数的基类

namespace YParamBrowser{

class YParamBrowserException : public std::exception {
private:
    std::string message;  // 错误消息

public:
    explicit YParamBrowserException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};




class YParamEnum : QObject
{
    Q_OBJECT
public:
    enum ParamType {
        ParamType_Bool,			//	true false
        ParamType_File,			//	/home/user/file.txt
        ParamType_Folder,		//	/home/user/
        ParamType_Int,			//	1234567890
        ParamType_Qreal,		//	1234567890.1234567890
        ParamType_String,		//	"1234567890"
        ParamType_PassWordStr,	//	"**********"
        ParamType_Enum,			//	"0,1,2,3"
        ParamType_Date,			//	2019/01/01
        ParamType_Time,			//	12:00
        ParamType_DateTime,		//	2019/01/01 12:00
        ParamType_KeySeq,		//	"Ctrl+Q"
        ParamType_Locale,		//	"Chinese,China"
        ParamType_Point,		//	"(123,456)"
        ParamType_PointF,		//	"(123.456,456.789)"
        ParamType_Size,			//	"123x456"
        ParamType_SizeF,		//	"123.456x456.789"
        ParamType_Rect,			//	"[(123,456),789x012]"
        ParamType_RectF,		//	"[(123.456,456.789),789.012x012.345]"
        ParamType_Flag,			//	"Flag1|Flag2|Flag3"
        ParamType_SizePolicy,	//	"[Fixed|Minimum, 100,200]"
        ParamType_Font,			//	"Arial,10,-1,5,75,1,1,1,0,0"
        ParamType_Color,		//	"#af6b6867" "Alpha" : "175"
    };

    // friend class YParamBase;
public:
    Q_ENUM(ParamType)
};





class YParamBase {
public:
    YParamBase(
                const YParamEnum::ParamType&    type,
                const QVariant&                 value   = QVariant(),
                const QVariant&                 defvalue= QVariant(),
                const QVariant&                 range   = QVariant(),
                const QString&                  name    = "name",
                const QString&                  tip     = "tips",
                const QString&                  remark  = "Notes"
        )
        : eParamType(type)
        , vParamValue(value)
        , vParamDefault(defvalue)
        , vParamRange(range)
        , sParamName(name)
        , sParamTip(tip)
        , sParamRemark(remark) {}

    virtual ~YParamBase() {};

	YParamBase& operator=(YParamBase& pInfo) {
        this->vParamValue 		= pInfo.vParamValue;
        this->vParamDefault 	= pInfo.vParamDefault;
        this->vParamRange 		= pInfo.vParamRange;

		this->sParamName 		= pInfo.sParamName;
		this->sParamTip 		= pInfo.sParamTip;
        this->eParamType 		= pInfo.eParamType;
		this->sParamRemark 		= pInfo.sParamRemark;
		return (*this);
	}

    const static QString typeToStr(YParamEnum::ParamType& type) {
        QMetaEnum metaEnum = QMetaEnum::fromType<YParamEnum::ParamType>();
        auto ret = metaEnum.valueToKey(type);
        if (QString(ret).isEmpty()) {
            throw YParamBrowserException(QString("Unable to convert the type, please check data:%1").arg(type).toStdString());
        }
        return ret;
    }

    const static YParamEnum::ParamType    strToType(const QString& str) {
        QMetaEnum metaEnum = QMetaEnum::fromType<YParamEnum::ParamType>();
        bool ok{false};
        auto ret = metaEnum.keysToValue(str.toStdString().c_str(), &ok);
        if (!ok) {
            throw YParamBrowserException(QString("Unable to convert the type, please check data:%1").arg(str).toStdString());
        }
        return static_cast<YParamEnum::ParamType>(ret);
    }



    inline  YParamEnum::ParamType    getEParamType()     const { return eParamType; }
    inline  QString                  getSParamName()     const { return sParamName; }
    inline  QVariant                 getVParamValue()    const { return vParamValue; }
    inline  QVariant                 getVParamRange()    const { return vParamRange; }
    inline  QString                  getSParamTip()      const { return sParamTip; }

    virtual QString                  getValueByRange()   const { return QString();}
    virtual void                     setVParamValue(const QVariant &newVParamValue, const QtProperty *property=nullptr);
    virtual void                     setStringValue(const QString &newStringValue) { vParamValue = QVariant(newStringValue);}

    virtual QDomElement     toDom(QDomDocument& doc);
    virtual bool            fromDom(QDomElement& dom);
    virtual QString toString();



protected:
    QVariant        vParamValue;        // 值
    QVariant        vParamDefault;      // 默认值
    QVariant        vParamRange;        // 参数范围

    QString    		sParamName;         // 参数名字
    QString    		sParamTip;          // 参数在界面的提示的说明文
    QString    		sParamRemark;       // 参数备注说明

    YParamEnum::ParamType		eParamType;         // 参数类型

};

using YParamPtr = std::shared_ptr<YParamBase>;

inline void YParamBase::setVParamValue(const QVariant &newVParamValue, const QtProperty *property)
{
    Q_UNUSED(property)
    vParamValue = newVParamValue;
}

inline QDomElement YParamBase::toDom(QDomDocument &doc)
{
    // 创建参数节点，及其子元素

    // 类型作为节点名称
    QDomElement param = doc.createElement(typeToStr(eParamType));
    param.setAttribute("Name", sParamName);             //方法1，创建属性，键值对可以是各种类型

    // 添加值
    QDomAttr attrValue = doc.createAttribute("Value");      //方法2，创建属性，值必须是字符串
    attrValue.setValue(vParamValue.toString());
    param.setAttributeNode(attrValue);

    // 添加备注
    QDomElement eleRemark = doc.createElement("Remark");  //创建子元素
    QDomText txtRemark = doc.createTextNode(sParamRemark);//设置括号标签中间的值
    param.appendChild(eleRemark);
    eleRemark.appendChild(txtRemark);

    // 添加默认值
    QDomElement eleDefault = doc.createElement("Default");
    QDomText txtDefault = doc.createTextNode(vParamDefault.toString());
    param.appendChild(eleDefault);
    eleDefault.appendChild(txtDefault);

    // 添加范围值（无范围）
    QDomElement eleRange = doc.createElement("Range");
    auto rangePair = vParamRange.toString();
    QDomText txtRange = doc.createTextNode(rangePair);
    param.appendChild(eleRange);
    eleRange.appendChild(txtRange);

    // 添加提示
    QDomElement eleTip = doc.createElement("Tip");
    QDomText txtTip = doc.createTextNode(sParamTip);
    param.appendChild(eleTip);
    eleTip.appendChild(txtTip);

    return param;
}

inline bool YParamBase::fromDom(QDomElement &dom)
{
    // 获取名称
    sParamName = dom.attribute("Name");

    // 获取值
    QDomAttr attrValue = dom.attributeNode("Value");
    if (!attrValue.isNull()) {
        vParamValue = attrValue.value();
    } else {
        throw YParamBrowserException(QString("Failed to read 'Value' information").toStdString());
    }

    // 获取备注
    QDomElement eleRemark = dom.firstChildElement("Remark");
    if (!eleRemark.isNull()) {
        sParamRemark = eleRemark.text();
    } else {
        throw YParamBrowserException(QString("Failed to read 'Remark' information").toStdString());
    }

    // 获取默认值
    QDomElement eleDefault = dom.firstChildElement("Default");
    if (!eleDefault.isNull()) {
        vParamDefault = eleDefault.text();
    } else {
        throw YParamBrowserException(QString("Failed to read 'Default' information").toStdString());
    }

    // 获取范围值 *需要子类自行处理*

    // 获取提示
    QDomElement eleTip = dom.firstChildElement("Tip");
    if (!eleTip.isNull()) {
        sParamTip = eleTip.text();
    } else {
        throw YParamBrowserException(QString("Failed to read 'Tip' information").toStdString());
    }

    return true;
}

inline QString YParamBase::toString()
{
    QString msg = QString("sParamName:%1 vParamValue:%2 sParamRemark:%3 vParamDefault:%4 sParamTip:%5 eParamType:%6 eParamRange:%7")
    .arg(sParamName).arg(vParamValue.toString())
        .arg(sParamRemark).arg(vParamDefault.toString())
        .arg(sParamTip).arg(typeToStr(eParamType)).arg(vParamRange.toString());
    return msg;
}

} // namespace YParamBrowser

#endif  // YPARAMBASE_HPP
