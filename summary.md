# 1. 拼接字符时，注意前后字符是否为空

```
    QString(“wangfei") + ":" + QString("003691")
```    
 当前后字符是外界传进来的时候，判断是否为空，为空时，中间“：”怎么处理？

# 2. 自定义pkexec提权弹窗，需要配置policy文件

com.deepin.signclient.deepin-certplat-checker.policy
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE policyconfig PUBLIC
 "-//freedesktop//DTD PolicyKit Policy Configuration 1.0//EN"
 "http://www.freedesktop.org/standards/PolicyKit/1/policyconfig.dtd">
<policyconfig>
  <vendor>The PolicyKit Project</vendor>
  <vendor_url>http://hal.freedesktop.org/docs/PolicyKit/</vendor_url>

  <action id="com.deepin.signclient.deepin-certplat-checker">
    <description>Run programs as another user</description>
    <description xml:lang="zh_CN">Kør et program som en anden bruger</description>
    <message>Authentication is required to check virus</message>
    <message xml:lang="zh_CN">安全检测需要管理员认证</message>
    <defaults>
      <allow_any>auth_admin_keep</allow_any>
      <allow_inactive>auth_admin_keep</allow_inactive>
      <allow_active>auth_admin_keep</allow_active>
    </defaults>
    <annotate key="org.freedesktop.policykit.exec.path">/bin/bash</annotate>
  </action>
</policyconfig>
```
安装路径：/usr/share/polkit-1/actions  
cmake 配置: 
```

# polkit policy file
install(FILES com.deepin.signclient.deepin-certplat-checker.policy DESTINATION share/polkit-1/actions)
if (OS_DEEPIN_PROFESSIONAL)
    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        install(FILES com.deepin.signclient.deepin-certplat-checker.policy DESTINATION share/polkit-1/actions)
    endif()
endif()
```
deepin-signtool-client.install ： usr/share   
 
 # 3. debian 打包报错 Cannot find (any matched for) "usr/bin/deepin-signtool-client" (tried in ., debian/tmp)

Build the source using "debian/rules build" into $(DESTDIR)  
  - DESTDIR=debian/binarypackage/(single binary package)
  - DESTDIR=debian/tmp/（multi binary package）
  
单个二进制包解决方法：debian/rules
```
override_dh_auto_install:
	dh_auto_install --destdir=debian/tmp
```

# 4. 应用版本号跟随打包版本号

debian/rules:
```
override_dh_auto_configure:
	dh_auto_configure -- -DCVERSION=$(DEB_VERSION_UPSTREAM)
```
cmake:
```
add_compile_definitions(CVERSION="${CVERSION}")
```
main.cpp:
```
#ifdef CVERSION
    QString verstr(CVERSION);
    if (verstr.isEmpty())
        verstr="1.0.0";
    app->setApplicationVersion(verstr);
#else
    app->setApplicationVersion("1.0.0");
#endif
```

# 5. 启动器快捷图标 deepin-signtool-client.desktop

deepin-signtool-client.desktop
```
[Desktop Entry]
Categories=Application;System;Settings
Comment=certificate-sign-platform
Exec=deepin-signtool-client
GenericName=certificate sign platform
Icon=DeepinSigntoolClient
Name=certificate sign platform
StartupNotify=false
Terminal=false
Type=Application
X-Deepin-Vendor=deepin
X-MultipleArgs=false
# Translations:
# Do not manually modify!
Comment[zh_CN]=证书签名平台
GenericName[zh_CN]=证书签名平台
Name[zh_CN]=证书签名平台
X-Deepin-AppID=deepin-signtool-client
```
cmake: /usr/share/applications
```
# desktop
install(FILES deepin-signtool-client.desktop DESTINATION share/applications)
```

# 5. 日志过滤规则使用 Q_LOGGING_CATEGORY

头文件声明：
```
Q_DECLARE_LOGGING_CATEGORY(signclient)
```
cpp文件定义（一次）：
```
Q_LOGGING_CATEGORY(signclient, "sign.client")

```
相关配置：
```
    // log
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();
    Logger *gLogger = Logger::globalInstance();
    FileAppender *fileAppener = new FileAppender(DLogManager::getlogFilePath());
    gLogger->registerCategoryAppender("sign.client", fileAppener);
    ConsoleAppender *consoleAppender = new ConsoleAppender;
    gLogger->registerCategoryAppender("sign.client", consoleAppender);
```
使用：
```
qCDebug(signclient) << result.debFile
```
启动应用：
```
QT_LOGGING_RULES="sign.client=true;sign.client.info=false" && deepin-signtool-client
```

# 6. 单例进程

```
    app->setSingleInstance(app->applicationName());
    app->setAutoActivateWindows(true);  // 自动激活上一个进程窗口
```

# 7. 保存主题切换的设置

```
    DApplicationSettings as;
    Q_UNUSED(as)
```

# 8. 应用图标和标题框图标

```
    app->setProductIcon(QIcon::fromTheme(APP_ICON));
    app->setWindowIcon(QIcon::fromTheme(APP_ICON));
```

# 9. UI上考虑最大化和最小化布局，主题切换颜色

最大化：哪些地方需要拉伸，尽量不要写死长宽。  
主题切换：自定义调色板颜色需要自己适配主题切换
```
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &DragDropWidget::onThemeTypeChanged);
    ...
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        successBackgroundColor = QColor(247, 247, 247);
        failBackgroundColor = QColor(254, 240, 239);
    } else {
        successBackgroundColor = QColor(37, 37, 37);
        failBackgroundColor = QColor(42, 28, 27);
    }
```

# 10. 兼容性打包

1. 找不到DPaletterHelper, 替换成DApplicationHelper
2. 找不到libdtkcommon-dev, 编译依赖去掉libdtkcommon-dev,  libdtkcore-dev,
 libdtkgui-dev,，只要libdtkwidget-dev，它会依赖core，gui。
3. 去掉运行依赖libdframeworkdbus2。

# 11. 对对话框的处理，需要考虑右上角关闭按钮

修改前：
```
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!m_pLoginedSignWidget->isBusy()) {
        return;
    }
    connect(m_pTipDlg->getButton(0), &QPushButton::clicked, this, [event]{
        event->ignore();
    });
    connect(m_pTipDlg->getButton(1), &QPushButton::clicked, this, [event]{
        event->accept();
    });
    m_pTipDlg->exec();
}
```
修改后：（逻辑优化，只要点确认就接受关闭事件，其他情况都忽略）
```
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!m_pLoginedSignWidget->isBusy()) {
        return;
    }
    if (m_pTipDlg->exec() == 1) {
        event->accept();
    } else {
        event->ignore();
    }
}
```

# 12. 交互的时候提示消息不要太频繁，也不要不提示