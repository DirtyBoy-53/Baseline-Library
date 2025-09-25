#ifndef YPARAMMANAGER_H
#define YPARAMMANAGER_H

#include "YParamBase.hpp"
#include <QMap>
#include <QObject>

class QtTreePropertyBrowser;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QBoxLayout;
class QPushButton;
namespace YParamBrowser {


class YParamManager : public QObject
{
    Q_OBJECT
public:
    using YParamMap = QMap<QString, YParamPtr>;
    using YGroupParamMap = QMap<QString, YParamMap>;

    explicit YParamManager(QObject* parent = nullptr);
    ~YParamManager();

    // 增加参数
    void addParam(YParamPtr param, const QString& group = "Param1");
    void updateParam(); // 更新本地参数


    // 读取参数
    template <typename T>
    T getParamValue(const QString &name);

    void getParamWidget(QtTreePropertyBrowser *&browser);
    // std::unique_ptr<QtTreePropertyBrowser> creatParamWidget(QWidget *parent);


    // 导入xml参数文件
    bool importParam( YGroupParamMap &param);
    // 导出参数到xml文件
    bool exportParam();

    void setSavePath(const QString &newSavePath);
    void setConfigName(const QString &newName);

    YGroupParamMap groupParamMap() const;
    void setGroupParamMap(const YGroupParamMap &groupParamMap);

    void clearGroupParamMap();
private:
    void initConnect();

    bool parseXML(const QString& filePath, YGroupParamMap &group);

public slots:
    void on_update_ui();

private:

    YGroupParamMap                              mGroupParamMap;
    std::unique_ptr<QtTreePropertyBrowser>      mEditor;
    std::unique_ptr<QtVariantPropertyManager>   mVariantManager;
    std::unique_ptr<QtVariantEditorFactory>     mVariantFactory;

    QString     mSavePath{"./Config"};
    QString     mFileName{"config"};

    bool        mValueChangeSigEnable{false};
};

}


#endif // YPARAMMANAGER_H
