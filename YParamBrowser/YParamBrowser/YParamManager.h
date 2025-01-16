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


class YParamManager : public QObject{
    Q_OBJECT

public:
    using YParamMap = QMap<QString, YParamPtr>;
    using YGroupParamMap = QMap<QString, YParamMap>;

    explicit YParamManager(QObject* parent = nullptr);
    ~YParamManager();

    // 增加参数
    void addParam(YParamPtr param, const QString& group = "Param1");

    // 读取参数
    template <typename T>
    T getParamValue(const QString &name);


    void layoutAddParamWidget(QBoxLayout *layout, bool isEnableBtn = false);

    // std::unique_ptr<QtTreePropertyBrowser> creatParamWidget(QWidget *parent);


    // 导入xml参数文件
    bool importParam(const QString &path);
    // 导出参数到xml文件
    bool exportParam(const QString &path);

    void setSavePath(const QString &newSavePath);

    YGroupParamMap groupParamMap() const;
    void setGroupParamMap(const YGroupParamMap &groupParamMap);

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

    QPushButton *mBtnSaveParam;
    QPushButton *mBtnCancel;
    QString     mSavePath{"./Config"};

    bool        mValueChangeSigEnable{false};
};

}


#endif // YPARAMMANAGER_H
