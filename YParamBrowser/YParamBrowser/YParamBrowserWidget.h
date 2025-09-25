#ifndef YPARAMBROWSERWIDGET_H
#define YPARAMBROWSERWIDGET_H

#include <QWidget>

#include <vector>

#include "YParamManager.h"

class QTabWidget;
class QPushButton;

enum yparam_user_e{
    Administrator,
    Operator,
    user_max,
};

struct yparam_s
{
    yparam_s() = default;
    yparam_s(
        const yparam_user_e& new_user,
        const QString& new_title,
        const YParamBrowser::YParamEnum::ParamType new_type,
        const QVariant& new_value,
        const QString& new_name,
        const QVariant& new_def_value,
        const QVariant new_range = QVariant(),
        const QString& new_tip = "",
        const QString& new_remark = "")
        : user(new_user),
        title(new_title),
        type(new_type),
        value(new_value),
        name(new_name),
        def_value(new_def_value),
        range(new_range),
        tip(new_tip),
        remark(new_remark) {
        if (new_type == YParamBrowser::YParamEnum::ParamType_Enum) {
            for (auto i = 0; i < range.toStringList().size(); ++i) {
                if (value.toString() == range.toStringList().at(i)) {
                    value = i;
                }
                if (def_value.toString() == range.toStringList().at(i)) {
                    def_value = i;
                }
            }
        }
    }

    yparam_user_e   user;   // 用户
    QString         title;  // 参数类别标题
    YParamBrowser::YParamEnum::ParamType type; // 数据类型
    QVariant    value;              // 值
    QString     name;               // 属性名称
    QVariant    def_value;          // 默认值
    QVariant    range;              // 范围
    QString     tip;                // 提示语
    QString     remark;             // 备注(主要体现在文件中)
};

class YParamBrowserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit YParamBrowserWidget(QWidget *parent = nullptr);

    void setUser(const yparam_user_e &user);
    void addParams(const std::vector<yparam_s> &param);
    void setProject(const QString &name);

private:
    void initUI();
    void initConnect();

signals:

public slots:
    void slotUpdateTableWidget();

private:
    QTabWidget  *mTableWidget{nullptr};
    QPushButton *mBtnWrite{nullptr};
    QString mProjectName{"Project"};
    // std::vector<YParamBrowser::YParamManager> mParamManager;
    std::map<yparam_user_e, YParamBrowser::YParamManager> mParamManager;
    QStringList mUserName = QStringList() << "管理员" << "操作员";
    yparam_user_e mUser{Administrator};
};

#endif // YPARAMBROWSERWIDGET_H
