#include "YParamBrowserWidget.h"

#include <QTabWidget>
#include <QPushButton>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>

#include "YParamFactory.hpp"
YParamBrowserWidget::YParamBrowserWidget(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initConnect();
}

void YParamBrowserWidget::setUser(const yparam_user_e &user)
{
    mUser = user;
}

void YParamBrowserWidget::addParams(const std::vector<yparam_s> &param)
{
    using namespace YParamBrowser;

    for (auto item : param) {
        YParamPtr p = YParamFactory::createParameter(
            item.type, item.value,
            item.def_value, item.range,
            item.name,item.tip, item.remark
            );
        auto it = mParamManager.find(item.user);
        YParamBrowser::YParamManager &manager = mParamManager[item.user];
        manager.addParam(p, item.title);
    }

    for (auto &pair : mParamManager) {
        const yparam_user_e& user = pair.first;
        YParamBrowser::YParamManager& manager = pair.second;
        manager.setSavePath(QDir::currentPath() + "/Config/" + mProjectName);
        manager.setConfigName(mProjectName + "_" + mUserName.at(user));
        manager.updateParam();
    }
}

void YParamBrowserWidget::initUI()
{
    auto mainLayout = new QVBoxLayout();
    auto btnLayout = new QHBoxLayout();

    mTableWidget = new QTabWidget();

    mBtnWrite = new QPushButton("保存");
    btnLayout->addStretch();
    btnLayout->addWidget(mBtnWrite);

    mainLayout->addWidget(mTableWidget);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
}

void YParamBrowserWidget::initConnect()
{
    connect(mBtnWrite, &QPushButton::clicked, [this]{
        for (auto &pair : mParamManager) {
            const yparam_user_e& user = pair.first;
            YParamBrowser::YParamManager& manager = pair.second;
            manager.exportParam();
        }
    });
}

void YParamBrowserWidget::slotUpdateTableWidget()
{

    for (auto &pair : mParamManager) {
        const yparam_user_e& user = pair.first;
        YParamBrowser::YParamManager& manager = pair.second;
        QtTreePropertyBrowser *browser;
        manager.getParamWidget(browser);
        if (browser != nullptr) {
            mTableWidget->insertTab(0, (QWidget*)browser, mUserName.at(user));
        } else {
            qDebug() << "错误: 无法获取参数浏览器部件，指针为空。";
        }
        auto isEnable = mUser > user ? false : true;    // 当前参数权限 < 设置用户权限 则不运行操作
        manager.on_update_ui(isEnable);
    }
}
