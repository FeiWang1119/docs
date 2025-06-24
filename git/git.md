
@[TOC](目录)

# Git 常用命令

## 仓库

| command                 | description                         |
| ----------------------- | ----------------------------------- |
| git init                | 在当前目录新建一个Git代码库         |
| git init [project-name] | 新建一个目录，将其初始化为Git代码库 |
| git clone [url]         | 下载一个项目和它的整个代码历史      |

## 配置

| command                                                 | description              |
| ------------------------------------------------------- | ------------------------ |
| git config --list                                       | 显示当前的Git配置        |
| git config [--global] core.editor vim                   | 编辑Git配置文件          |
| git config [--global] user.name "[name]"                | 设置提交代码时的用户名   |
| git config [--global] user.email "[email address]"      | 设置提交代码时的邮箱地址 |
| git config [--global] commit.template ~/.gitmessage.txt | 设置提交模板             |
| git config [--global] help.autocorrect  1               | 设置自动纠正时间         |

## 增加/删除文件

| command                               | description                                                                |
| ------------------------------------- | -------------------------------------------------------------------------- |
| git add [file1] [file2] ...           | 添加指定文件到暂存区                                                       |
| git add [dir]                         | 添加指定目录到暂存区，包括子目录                                           |
| git add .                             | 添加当前目录的所有文件到暂存区                                             |
| git add -p                            | 添加每个变化前，都会要求确认。对于同一个文件的多处变化，可以实现分次提交。 |
| git rm [file1] [file2] ...            | 删除工作区文件，并且将这次删除放入暂存区                                   |
| git rm --cached [file]                | 停止追踪指定文件，但该文件会保留在工作区                                   |
| git mv [file-original] [file-renamed] | 改名文件，并且将这个改名放入暂存区                                         |

## 代码提交

| command                                     | description                                    |
| ------------------------------------------- | ---------------------------------------------- |
| git commit -m [message]                     | 提交暂存区到仓库区                             |
| git commit [file1] [file2] ... -m [message] | 提交暂存区的指定文件到仓库区                   |
| git commit -a                               | 提交工作区自上次commit之后的变化，直接到仓库区 |
| git commit -v                               | 提交时显示所有diff信息                         |
| git commit --amend -m [message]             | 补充上一次的提交,并改写提交信息                |
| git commit --amend [file1] [file2] ...      | 补交上一次提交，并包括指定文件的新变化         |

## 分支

| command                                            | description                                  |
| -------------------------------------------------- | -------------------------------------------- |
| git branch                                         | 列出所有本地分支                             |
| git branch -r                                      | 列出所有远程分支                             |
| git branch -a                                      | 列出所有本地分支和远程分支                   |
| git branch [branch-name]                           | 新建一个分支，但依然停留在当前分支           |
| git checkout -b [branch]                           | 新建一个分支，并切换到该分支                 |
| git checkout -b [branch] [remote-branch]           | 从远程某分支创建新分支                       |
| git checkout commit_hash                           | 在当前分支切换到指定提交commit_hash          |
| git checkout -b branch commit_hash                 | 新建分支，并指定提交commit_hash              |
| git branch [branch] [commit]                       | 新建一个分支，指向指定commit                 |
| git branch --track [branch] [remote-branch]        | 新建一个分支，与指定的远程分支建立追踪关系   |
| git checkout [branch-name]                         | 切换到指定分支，并更新工作区                 |
| git checkout -                                     | 切换到上一个分支                             |
| git branch --set-upstream-to [remote-branch] [branch] | 建立追踪关系，在现有分支与指定的远程分支之间 |
| git merge [branch]                                 | 合并指定分支到当前分支                       |
| git cherry-pick [commit]                           | 选择一个commit，合并进当前分支               |
| git branch -d [branch-name]                        | 删除分支                                     |
| git push origin --delete [branch-name]             | 删除远程分支                                 |
| git branch -dr [remote/branch]                     | 删除远程分支                                 |

## 标签

| command                              | description               |
| ------------------------------------ | ------------------------- |
| git tag                              | 列出所有tag               |
| git tag [tag]                        | 新建一个tag在当前commit   |
| git tag [tag] [commit]               | 新建一个tag在指定commit   |
| git tag -d [tag]                     | 删除本地tag               |
| git push origin :refs/tags/[tagName] | 删除远程tag               |
| git show [tag]                       | 查看tag信息               |
| git push [remote] [tag]              | 提交指定tag               |
| git push [remote] --tags             | 提交所有tag               |
| git checkout -b [branch] [tag]       | 新建一个分支，指向某个tag |

## 查看信息

| command                                          | description                                                |
| ------------------------------------------------ | ---------------------------------------------------------- |
| git status                                       | 显示有变更的文件                                           |
| git log                                          | 显示当前分支的版本历史                                     |
| git log --stat                                   | 显示commit历史，以及每次commit发生变更的文件               |
| git log -S [keyword]                             | 搜索提交历史，根据关键词                                   |
| git log [tag] HEAD --pretty=format:%s            | 显示某个commit之后的所有变动，每个commit占据一行           |
| git log [tag] HEAD --grep feature                | 显示某个commit之后的所有变动，其"提交说明"必须符合搜索条件 |
| git log --follow [file] / git whatchanged [file] | 显示某个文件的版本历史，包括文件改名                       |
| git log -p [file]                                | 显示指定文件相关的每一次diff                               |
| git log -5 --pretty --oneline                    | 显示过去5次提交                                            |
| git shortlog -sn                                 | 显示所有提交过的用户，按提交次数排序                       |
| git blame [file]                                 | 显示指定文件是什么人在什么时间修改过                       |
| git diff                                         | 显示暂存区和工作区的差异                                   |
| git diff --cached [file]                         | 显示暂存区和上一个commit的差异                             |
| git diff HEAD                                    | 显示工作区与当前分支最新commit之间的差异                   |
| git diff HEAD^ HEAD                              | 显示这次提交和上次之间的差异                               |
| git diff [first-branch]...[second-branch]        | 显示两次提交之间的差异                                     |
| git diff --shortstat "@{0 day ago}"              | 显示今天你写了多少行代码                                   |
| git show [commit]                                | 显示某次提交的元数据和内容变化                             |
| git show --name-only [commit]                    | 显示某次提交发生变化的文件                                 |
| git show [commit]:[filename]                     | 显示某次提交时，某个文件的内容                             |
| git reflog                                       | 显示当前分支的最近几次提交                                 |

## 远程同步

| command                                 | description                                                              |
| --------------------------------------- | ------------------------------------------------------------------------ |
| git review -r origin yourbranchname     | 提交到远程仓库某分支                                                     |
| git fetch [remote]                      | 下载远程仓库的所有变动                                                   |
| git fetch origin develop/snipe:snipe    | 从远程分支到本地分支                                                     |
| git fetch origin pull/2/head:branch     | 拉去github的pr到分支branch                                               |
| git remote -v                           | 显示所有远程仓库                                                         |
| git remote show [remote]                | 显示某个远程仓库的信息                                                   |
| git remote add [shortname] [url]        | 增加一个新的远程仓库，并命名                                             |
| git pull [remote] [branch]              | 取回远程仓库的变化，并与本地分支合并                                     |
| git submodule update --init --recursive | 已经clone了之后，下载整个子模块                                          |
| git pull linuxdeepin pull/92/head:92    | 取回github的某个pr                                                       |
| git push [remote] [branch]              | 上传本地指定分支到远程仓库                                               |
| git push [remote] --delete [branch]     | 删除远程分支develop                                                      |
| git push [remote] --force               | 强行推送当前分支到远程仓库，即使有冲突                                   |
| git push [remote] --all                 | 推送所有分支到远程仓库                                                   |
| git checkout [file]                     | 撤销# 恢复暂存区的指定文件到工作区                                       |
| git checkout [commit] [file]            | 恢复某个commit的指定文件到暂存区和工作区                                 |
| git checkout .                          | 恢复暂存区的所有文件到工作区                                             |
| git checkout hashValue -- file          | 将文件file恢复到hashValue的提交                                          |
| git checkout hashValue~1 -- file        | 将文件file恢复到hashValue的前一个提交                                    |
| git reset [file]                        | 重置暂存区的指定文件，与上一次commit保持一致，但工作区不变               |
| git reset --hard                        | 重置暂存区与工作区，与上一次commit保持一致                               |
| git reset --hard origin/master          | 切到master最新的點                                                       |
| git reset [commit]                      | 重置当前分支的指针为指定commit，同时重置暂存区，但工作区不变             |
| git reset --hard [commit]               | 重置当前分支的HEAD为指定commit，同时重置暂存区和工作区，与指定commit一致 |
| git reset --keep [commit]               | 重置当前HEAD为指定commit，但保持暂存区和工作区不变                       |
| git revert [commit]                     | 新建一个commit，用来撤销指定commit。                                     |
| git stash & git stash pop               | 暂时将未提交的变化移除，稍后再移入                                       |

## git stash

| command                                    | description                                                                        |
| ------------------------------------------ | ---------------------------------------------------------------------------------- |
| git stash save "save message"              | 执行存储时，添加备注，方便查找。                                                   |
| git stash list                             | 查看保存列表                                                                       |
| git stash show                             | 显示做了哪些改动，默认show第一个存储,如果要显示某个存储，后面加stash@{$num}        |
| git stash show -p : show  stash@{$num}  -p | 显示第一个存储的改动，如果想显示某个存储 git stash                                 |
| git stash apply                            | 应用某个存储,但不会把存储从存储列表中删除，默认使用第一个存储,即stash@{0}          |
| git stash pop                              | 恢复之前缓存的工作目录，将缓存堆栈中的对应stash删除，默认为第一个stash,即stash@{0} |
| git stash pop stash@{$num}                 | 如果要应用并删除其他stash，命令                                                    |
| git stash drop stash@{$num}                | 丢弃stash@{$num}存储，从列表中删除这个存储                                         |
| git stash clear                            | 删除所有缓存的stash                                                                |
| git stash push -m savefile file            | 保存特定文件file                                                                   |

## 其他

| command       | description                                 |
| ------------- | ------------------------------------------- |
| git archive   | 生成一个可供发布的压缩包                    |
| git clean -df | 删除当前目录下没有被 track 过的文件和文件夹 |

# 案例

## 更新远程fork分支

1、添加远程仓库到本地remote分支

    $ git remote add upstream git@github.com:apache/flink.git # 远程仓库地址
    
 2、查看当前仓库的远程分支

    $ git remote -v

3、fetch 远程分支

    $ git fetch upstream

4、合并 fetch 的分支到本地master

    $ git merge upstream/master
    $ git log

5、推送本地master 到远程仓库（自己fork的仓库）

    $ git push origin master

## 将修改内容切换到别的分支

1、保存修改

    $ git stash 

2、切换分支

    $ git checkout newBranch

3、恢复修改

    $ git stash pop


## 配置第三方比较工具

1. 编辑脚本extMerge： vim /usr/local/bin/extMerge

``` sh
#!/bin/sh
/Your/Path/To/p4merge $*
```

2.  编辑脚本extDiff：vim /usr/local/bin/extDiff

``` sh
#!/bin/sh
[ $# -eq 7 ] && /usr/local/bin/extMerge "$2" "$5"
```

3. 修改权限  

  ``` sh 
sudo chmod +x /usr/local/bin/extMerge
sudo chmod +x /usr/local/bin/extDiff
```
4. 配置git

``` sh
git config --global merge.tool extMerge
git config --global mergetool.extMerge.cmd \
  'extMerge "$BASE" "$LOCAL" "$REMOTE" "$MERGED"' 
git config --global mergetool.extMerge.trustExitCode false
git config --global diff.external extDiff
```
