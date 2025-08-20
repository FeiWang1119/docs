# system runtime 

| command       | description                    |
|---------------|--------------------------------|
| ipcs          | 进程间通讯设施状态             |
| uptime        | Linux 系统运行时长             |
| iostat        | CPU 平均负载和磁盘活动         |
| sar           | 监控，收集和汇报系统活动        |
| mpstat        | 监控多处理器使用情况           |
| pmap          | 监控进程的内存使用情况         |
| nmon          | 系统管理员调优和基准测量工具   |
| glances       | 密切关注linux系统              |
| strace        | 查看系统调用                   |
| ftptop        | ftp 服务器基本信息             |
| powertop      | 电源消耗和电源管理             |
| mytop         | 监控mysql的线程和性能          |
| htop/top/atop | 系统运行参数分析               |
| netstat       | linux网络统计监控工具          |
| ethtool       | 显示和修改网络接口控制器       |
| tcpdump       | 网络数据包分析利刃             |
| telnet        | 远程登录服务的标准协议         |
| iptraf        | 获取实时网络统计信息           |
| iftop         | 显示主机上网络接口带宽使用情况 |

# systemd

| command                                     | description                   |
|---------------------------------------------|-------------------------------|
| systemctl start unit                        | 开启某个服务                  |
| systemctl stop unit                         | 停止某个服务                  |
| systemctl restart unit                      | 重启某个服务                  |
| systemctl reload unit                       | 重新加载某个服务              |
| systemctl status unit                       | 查看某个服务的状态            |
| systemctl enable unit                       | 使某个服务开机自启动          |
| systemctl disable unit                      | 使某个服务开机不自启动        |
| systemctl mask unit                         | 使某个服务不能使用和激活      |
| systemctl unmask unit                       | 使某个服务能使用和激活        |
| systemctl daemon-reload                     | 重新加载 systemd manager 配置 |
| systemctl edit unit                         | 编辑某个服务的配置            |
| systemctl list-unit-files                   | 列举所有unit文件              |
| systemctl list-units --all --state=inactive | 列举所有inactive单元          |
| journalctl -b                               | 最近启动日志信息              |
| journalctl -k                               | 内核日志信息                  |
| journalctl -b -u foo.service                | 某个服务的日志信息            |

# debian

| command                       | description           |
|-------------------------------|-----------------------|
| apt build-dep packagename     | 构建依赖              |
| dpkg-buildpackage -us -uc -nc | 构建deb包             |
| apt depends packagename       | packagename依赖的包   |
| apt rdepends packagename      | 哪些包依赖packagename |
| apt-file search file          | 搜索文件相关的包      |
| apt download packagename      | 下载包                |
| apt source packagename        | 下载源码包            |

# gsetting

| command                             | description                              |
|-------------------------------------|------------------------------------------|
| glib-compile-schemas                | /usr/share/glib-2.0/schemas              |
| gsettings list-schemas              | 显示系统已安装的不可重定位的schema       |
| gsettings list-relocatable-schemas  | 显示已安装的可重定位的schema             |
| gsettings list-children SCHEMA      | 显示指定schema的children，其中SCHEMA指xml |
| gsettings list-keys SCHEMA          | 显示指定schema的所有项(key)              |
| gsettings range SCHEMA KEY          | 查询指定schema的指定项KEY的有效取值范围  |
| gsettings get SCHEMA KEY            | 显示指定schema的指定项KEY的值            |
| gsettings set SCHEMA KEY VALUE      | 设置指定schema的指定项KEY的值为VALUE     |
| gsettings reset SCHEMA KEY          | 恢复指定schema的指定项KEY的值为默认值    |
| gsettings reset-recursively SCHEMA  | 恢复指定schema的所有key的值为默认值      |
| gsettings list-recursively [SCHEMA] | 递归显示schema的所有项(key)和值(value)   |

# coredump

| command                      | description                 |
|------------------------------|-----------------------------|
| apt install systemd-coredump | 安装 coredump               |
| apt install dde-dock-dbgsym  | 安装应用符号调试信息        |
| ulimit -c                    | 查看是否开启,默认是不开启的 |
| ulimit -c unlimited          | 开启core dump功能           |
| coredumpctl list             | 查看崩溃列表 获取崩溃的pid  |
| coredumpctl dump             | 查看堆栈信息                |
| coredumpctl info pid         | 查看堆栈信息                |
| apt install lz4; lz4 -d FILE | 来解压coredump文件          |

# ssh

| command                                  | description                                           |
|------------------------------------------|-------------------------------------------------------|
| ssh-keygen                               | default key type : RSA                                |
| ssh-keygen -t ed25519                    | specify key type: ed25519                             |
| sshd -T                                  | 查看出错原因                                          |
| ssh-copy-id -i key_file [user@]server_name | install a public key automatically on a remote server |

# process 

| command                 | description        |
|-------------------------|--------------------|
| cat /proc/12345/environ | 查看进程的环境变量 |
| cat /proc/12345/maps    | 查看进程依赖的so   |
| pldd 12345              | 查看进程依赖的.so  |
| strings *.so            | 查看so的字符       |

# dbus

| command                                                        | description                      |
|----------------------------------------------------------------|----------------------------------|
| qdbus --session                                                | 查看当前session所有的service信息 |
| qdbus --system                                                 | 查看当前system所有的service信息  |
| dbus-monitor --session interface=org.freedesktop.Notifications | 监听dbus服务接口                 |
| GLib.Variant('b', False)                                       | d-feet Variant 变量格式          |

# tmux
| command                          | description |
|----------------------------------|-------------|
| tmux new -s name                 | 创建一个新的session |
| tmux attach -t name              | 连接到session name  |
| tmux ls                          | 查看session         |
| tmux kill-session -t name        | 关闭session         |
| tmux kill-server                 | 关闭server          |
| tmux new-session -s name -n name | 创建一个新的session |
| tmux rename-session -t old new   | 重命名session       |
| tmux new-window -t name -n name  | 创建一个新的window  |
| tmux rename-window -t old new    | 重命名window        |
| tmux list-windows -t name        | 查看window          |
| tmux kill-window -t name         | 关闭window          |
| tmux select-pane -t name         | 选择pane            |
| tmux split-window -t name -h     | 分割window          |

# DTK

| command                       | description  |
|-------------------------------|--------------|
| D_DXCB_FORCE_NO_TITLEBAR      | 强制无标题栏 |
| D_DXCB_DISABLE_OVERRIDE_HIDPI | 使能放大倍数 |
| deepin-gui-settings           | 设置gui属性  |

# x11

| command  | description  |
|----------|--------------|
| xprop    | 查看窗口属性 |
| xwininfo | 查看窗口属性 |

# changelog

| command                                                    | description                                      |
|------------------------------------------------------------|--------------------------------------------------|
| dch -m -D unstable -bv "5.6.16+test" "Test 5.6.16+test"    | package: devscripts                              |
| gbp deepin-changelog -N <version> --deepin-branch=<branch> | package: git-buildpackage deepin-gbp-dch-plugins |

# others

|command|description|
|--|--|
sudo apt install manpages-dev | 安装手册
uos-activator-cmd -s --kms kms.uniontech.com:8900:Vlc1cGIyNTBaV05v | 激活UOS
sudo pkill -ef /usr/lib/deepin-daemon/dde-system-daemon; sudo DDE_DEBUG_LEVEL=debug DDE_DEBUG_MATCH=account /usr/lib/deepin-daemon/dde-system-daemon | 定位 dde-daemon
DEEPIN_APPLICATION_MANAGER_APP_LAUNCH_HELPER_BIN=./build/apps/app-launch-helper/src/app-launch-helper ./build/apps/dde-application-manager/src/dde-application-manager

