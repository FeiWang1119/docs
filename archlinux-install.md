1.测试网络联通性

```sh
ping www.baidu.com
```

2.更新系统时钟

```sh
timedatectl set-ntp true # 将系统时间与网络时间进行同步
timedatectl status       # 检查服务状态
```

3.更换国内软件仓库镜像源

```sh
vim /etc/pacman.d/mirrorlist

Server = https://mirrors.ustc.edu.cn/archlinux/$repo/os/$arch          # 中国科学技术大学开源镜像站
Server = https://mirrors.tuna.tsinghua.edu.cn/archlinux/$repo/os/$arch # 清华大学开源软件镜像站
Server = https://repo.huaweicloud.com/archlinux/$repo/os/$arch         # 华为开源镜像站
Server = http://mirror.lzu.edu.cn/archlinux/$repo/os/$arch             # 兰州大学开源镜像站
```

4.分区和格式化

```sh
lsblk           # 显示当前分区情况
cfdisk /dev/sda # 对安装 archlinux 的磁盘分区
                # Swap 分区建议为电脑内存大小的 60%, 引导分区boot（1G），剩余给默认类型 linux filesystem
fdisk -l        # 复查磁盘情况

mkfs.fat -F 32 /dev/sda1 # 格式化启动分区
mkswap /dev/sda2         # 格式化交换分区
mkfs.ext4 /dev/sda3      # 使用 ext4 格式化根分区

mount /dev/sda1 /mnt/boot --mkdir # 挂载启动分区
swapon /dev/sda2                  # 启动交换分区
mount /dev/sda3 /mnt              # 挂载根分区
mkdir -p /mnt/boot/EFI
mount /dev/sda1 /mnt/boot/EFI     # 挂载 EFI 分区
```

5.安装基础软件包

```sh
pacstrap /mnt base base-devel linux linux-firmware vim sudo
```

6.生成 fstab 文件

```sh
genfstab -U /mnt >> /mnt/etc/fstab  # 生成新的自动挂载文件，并写入到新安装的系统
```

7.change root

```sh
arch-chroot /mnt
```

8.配置 archlinux

- 配置网络

```sh
# 安装网络 DHCP 动态 IP 软件包和网络管理软件包：
pacman -S dhcpcd networkmanager
# 启动相关服务
systemctl enable dhcpcd
systemctl enable NetworkManager
```

- 配置时区

```sh
ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
```

- 配置本地字符编码

编辑 /etc/locale.gen 文件，取消 en_US.UTF-8 UTF-8 和 zh_CN.UTF-8 UTF-8 这两行前的注释信息。
执行 locale-gen 命令生成本地字符集信息。
创建 /etc/locale.conf 文件，内容为：LANG=en_US.UTF-8

- 配置用户

```sh
passwd                               # 设置 root 账号密码
useradd -m -G wheel -s /bin/bash fei # 添加普通用户，并加入到 wheel 用户组，以方便使用 sudo 命令来执行一些需要超级用户权限的操作
vim /etc/sudoers                     # 移除 # %wheel ALL=(ALL) ALL 这一行前的井号，使 wheel 用户组的用户都可以正常执行 sudo 命令
passwd fei                           # 给新用户设置密码
```

- 配置主机名

```sh
vim /etc/hostname # feiarch
vim /etc/hosts

127.0.0.1   localhost
::1         localhost
127.0.1.1   feiarch.localdomain feiarch
```

- 配置硬件时间

```sh
hwclock --systohc
```

- 配置引导程序

```sh
pacman -S grub efibootmgr
grub-install --recheck /dev/sda      # 安装 GRUB 引导信息至指定的硬盘
grub-mkconfig -o /boot/grub/grub.cfg # 生成并写入 GRUB 配置信息
```

9. 进入新系统

```sh
exit           # 退出 chroot环境
umount -R /mnt # 卸载之前挂载的分区
```
