# proxy

## ssh config(~/.ssh/config)

``` sh
Host github.com
    Hostname github.com
    ProxyCommand /usr/bin/corkscrew 10.20.52.42 7890 %h %p
```

## git config(~/.gitconfig)

git config --global http.proxy 'http://127.0.0.1:7890'
git config --global https.proxy 'https://127.0.0.1:7890'

## http config(~/.zshrc)

export http_proxy=http://127.0.0.1:7890/
export https_proxy=http://127.0.0.1:7890/

## npm config (~/.npmrc)

npm config set https-proxy http://127.0.0.1:7897

# plantuml (vscode plugin)

install java & Graphviz
``` sh

sudo apt install default-jre graphviz
```

# gammaray

/proc/sys/kernel/yama/ptrace_scope to 0

``` sh
sudo vim /etc/sysctl.d/10-ptrace.conf
```
type : ` kernel.yama.ptrace_scope = 0`

take effect immediately：

``` sh
sudo sysctl --system -a -p | grep yama
```

# Qt http passwd

iQbFdwc8jWo+w7FRzqM4SKRJu7+kg4JzmHmA78SM3g


# transifex

## ~/.transifexrc

```c
[https://www.transifex.com]
rest_hostname = https://rest.api.transifex.com
api_hostname = https://api.transifex.com
hostname = https://www.transifex.com
token = ???
```

## .tx/config(project)

``` c
[main]
host = https://www.transifex.com
minimum_perc = 80
mode = developer

[o:linuxdeepin:p:deepin-desktop-environment:r:dde-calendar]
file_filter = translations/dde-calendar_<lang>.ts
source_file = translations/dde-calendar_en_US.ts
source_lang = en_US
type = QT

[o:linuxdeepin:p:deepin-desktop-environment:r:dde-calendar_desktop]
file_filter = translations/desktop/desktop_<lang>.ts
source_file = translations/desktop/desktop.ts
source_lang = en
type = QT

[o:linuxdeepin:p:deepin-desktop-environment:r:dde-calendar-service]
file_filter = translations/dde-calendar-service_<lang>.ts
source_file = translations/dde-calendar-service_en_US.ts
source_lang = en_US
type = QT
```

## tx

| command                      | description              |
|------------------------------|--------------------------|
| apt install transifex-client | 安装tx                   |
| tx pull -t -a -s -branch m23   | 拉取翻译（-a -f 全部拉取） |
| tx push -s -branch m23         | 推送翻译                 |
| tx push --branch m23 --languages zh_CN --t dde-control-center | 推送某个语言翻译|

# printer

b3-9 ip: 10.20.51.241

# github action

| command                      | description              |
|------------------------------|--------------------------|
| integrate                    | 预集成/重新集成          | 
| topic name                   | 创建/加入主题name        |
| integr-topic name            | 主题集成（多个项目集成） |
| retest(check obs)            | check failed 重新 build  |
| ok-to-test                   | 同意执行 retest          |

# doxygen 


- generate template Doxygen:

```sh
doxygen -g Doxygen  
```

- generate document:

```sh
doxygen Doxygen
```

## configure UML

| command                      | description              |
|------------------------------|--------------------------|
| EXTRACT_ALL                  | YES                      |
| HAVE_DOT                     | YES                      |
| UML_LOOK                     | YES                      |
| RECURSIVE                    | YES                      |

## deb source uos-professional

deb http://pools.uniontech.com/ppa/dde-eagle eagle/1070 main contrib non-free
deb http://pools.uniontech.com/desktop-professional eagle contrib non-free


# python virtual environment

1. create a virtual environment in a local folder named .venv:

```sh
python3 -m venv .venv
```

2. activate a virtual environment:

```sh
source .venv/bin/activate
```

3. check the location of your Python interpreter:

```sh
which python
```

4. switch projects or leave your virtual environment, deactivate the environment:

```sh
deactivate
```

# dpkg-shlibdeps: error: no dependency information found folder

```sh
override_dh_shlibdeps:
    dh_shlibdeps --dpkg-shlibdeps-params=--ignore-missing-info
```
