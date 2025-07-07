# 上海交通大学-cs-(2024-2025-2)-CS2303-中文班-操作系统课程设计-OpenEuler版

中文班大作业做的课本的八个project，感觉比英文班的简单多了（但是最后均分比英文班低！？）

## 评分标准

代码+报告参与评分，需要线下cr，本届有2'bonus，若有四个以上实验使用伟大的华为的伟大的自研国产操作系统OpenEuler，获得1'bonus，八个实验完全国产，获得2'bonus

据说下一届不再能使用Ubuntu，而是强制使用OpenEuler，我先用Ubuntu写了一遍，再用OpenEuler重新跑了一遍，区别不大，但是OpenEuler在配环境等方面较为恶心

## 注意事项

为了不改动过多示例代码，最好使用内核版本4开头的Linux内核，否则在写proc相关内容的时候函数的接口会和示例代码差别巨大，本人使用了ubuntu-16.04.6-desktop以及openEuler 20.03 LTS SP3

## 内容说明

两个文件夹分别为Ubuntu版本的代码+报告，以及OpenEuler版本的代码+报告

我懒得改报告的配图了，所以把所有的配图放在了文件```操作系统openEuler重写版.pdf```当中，你还可以在这里面找到OpenEuler环境配置的全流程（下面也会附上），实在是太贴心了

各个文件夹中的sh文件为批处理文件，阅读或者运行这些文件可以对对应任务进行验证，避免反复输入对应指令

## OpenEuler环境配置

### OpenEuler虚拟机安装

由于大作业需要比较古老的内核（4开头？），我使用了最古老的[openEuler 20.03 LTS SP3](https://www.openeuler.org/zh/download/archive/detail/?version=openEuler%2020.03%20LTS%20SP3)

使用VMware安装虚拟机，硬盘大小给了32G，内存大小需要2G，否则安装时会卡死

下面最好在root用户中进行操作

### 网络问题

ping一下看看能不能上网，大概率是不行

[这个链接](https://blog.csdn.net/SOBE_rrr/article/details/137300009)指示修改ifcfg-ens33 文件，使其开机时启动网卡，再reboot，终于通网了

### 图形化界面安装
再用yum安装dde图形化界面，可以加上-y自动确认

```bash
sudo yum update
sudo yum install dde
sudo systemctl set-default graphical.target
sudo reboot
```

可能要安两个小时

### 安装kernel-devel

这一步是为了获得make所需的build文件夹内容，openEuler**不自带**这个文件夹

```bash
yum install kernel-devel-$(uname -r)
```

### 安装JDK

[这个链接](https://blog.csdn.net/loveLifeLoveCoding/article/details/111313930)教了怎么安装JDK和配置环境

编译为字节码的指令和书上有所区别，需要“javac 完整文件名”

到此，我们就可以在openEuler上面完成操作系统的八个大作业了