# QScrollArea 的view widget不能resize，自动填充可用空间

解决方法：

``` c++
    scrollArea->setWidgetResizable(true);
```
# QScrollArea::setWidget(QWidget *widget) 

The widget's autoFillBackground property will be set to true.

如果想要QScrollArea的view widget无背景颜色，只能在setWidget之后调用widget->setAutoFillBackground(false);

``` c++
    scrollArea->setWidget(licenseWidget);
    licenseWidget->setAutoFillBackground(false);
```

#  翻译文案部分高亮显示

``` c++
    QString ack = QObject::tr("Sincerely appreciate all the open-source software used.");
    QString tmp = QObject::tr("open-source software");
    ack = ack.replace(tmp, websiteLinkTemplate.arg(websiteLink).arg(tmp));
```
# 代码逻辑简化
``` c++
bool ok = d->licenseDialog != nullptr ? d->licenseDialog->loadLicense() : false;
// 简化后
bool ok = d->licenseDialog && d->licenseDialog->loadLicense(); 
```
# 设置view的奇偶颜色不一样

```c++
    listView->setAlternatingRowColors(true);
```

# 添加新格式的文件需要修改dep5

# StandardItem 可以设置数据，获取数据

通过这种方式获取特定项的数据，不用从数据容器里面遍历每项匹配之后获取。

``` c++
 auto pItem = new DStandardItem(componentInfo->name());
 pItem->setData(...);
 pItem->data(...);
 ```

 # 避免频繁的操作里面出现重复的工作

 ```c++
 void show() 
 {
    d->listModel->clear();
    for (auto component : d->licenseInfo.componentInfos()) {
        d->addComponentItem(component);
    } 
    ...
 }
 ```

 # 接口的命名准确有意义，可读性好

 # 清空存储指针容器时，注意释放指针指向的资源
 
 ``` c++
 void DLicenseInfoPrivate::clear()
{
    qDeleteAll(componentInfos);
    componentInfos.clear();
}
```
