

<div align="center">
  <a href="https://github.com/ROV-Master/rovmaster-embedded"><img src="https://zengwangfa.oss-cn-shanghai.aliyuncs.com/rov/rovmaster(vector)1.png" alt=""></a>
  <a href="https://github.com/ROV-Master/rovmaster-embedded"><h1>ROV-Master</h2></a>
</div>


<div align="center">
  <a href="http://wiki.friendlyarm.com/wiki/index.php/NanoPi_NEO_Core/zh"><img src="https://img.shields.io/badge/Device-Nanopi NEO Core-brigreen.svg?style=flat-square" alt="Device"></a>
  <a href="https://img.shields.io"><img src="https://img.shields.io/github/repo-size/ROV-Master/rovmaster-embedded?style=flat-square" alt="Size"></a>
</div>

# 1. 介绍

- 本项目用到了以下软件包：
  - [WiringNP](https://github.com/friendlyarm/WiringNP) This is a GPIO access library for NanoPi. It is based on the WiringOP for Orange PI which is based on original WiringPi for Raspberry Pi.
  - [Mjpg-streamer](https://github.com/jacksonliam/mjpg-streamer) 是一个命令行应用程序，可将JPEG帧从一个或多个输入插件复制到多个输出插件，从一个UVC内核摄像头读取内容，然后将它推送到本地的端口上。
  - [EasyLogger](https://github.com/armink/EasyLogger) 是一款超轻量级(ROM<1.6K, RAM<0.3K)、高性能的 C/C++ 日志库。

# 2. 使用
- 运行环境：NanoPi NEO
```shell
$ git clone https://github.com/ROV-Master/rovmaster-embedded
$ cd rovmaster-embedded
```

## 2.1 安装相关依赖
在第一次运行程序之前，务必执行(`build.sh` 自动化安装依赖库)):

```shell
$ chmod +x build.sh
$ ./build.sh

```
build.sh 中安装相关依赖库

## 2.2 编译与执行
由于使用 WiringNP 库，需要 root 权限：

```shell
$ make 
$ sudo ./rovmaster
```

## 2.3 注意事项

### 2.3.1 设备未打开
若执行时提示无法打开某设备，输入 `sudo npi-config` > `Advanced Options` 中使能相关设备：

```shell
$ sudo npi-config
```
若系统时间不对，为时区未进行更改：
在 `npi-config` > `Localisation Options` > `Change Timezone` 中选择修改。


### 2.3.2 lib库编译
当修改了lib库中文件，需要进行重新编译时，进行编译选择：
- 1.WiringNP
- 2.easylogger
- 3.mjpg-streamer

启动 `compile.sh` 脚本，输入对应库的编号，进行自动化编译，并自动拷贝相关库
```shell
$ ./compile.sh
```

### 2.3.3 视频推流
视频推流命令在 `video.sh` 脚本中，指定相关视频参数，在其中修改**摄像头视频推流参数**即可。

# 3.协议
| 编号| 1 | 2 |3  | 4 |  5| 6 | 7 | 8 | 9 | 10 |  11|  12| 13 | 14 | 15 |16|17|18|19|20|
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |--- |--- |--- |--- |--- |--- |
|功能|包头|包头 | 数据长度位 |深度锁定位  |方向锁定位  |X轴动力模拟量  |Y轴动力模拟量  |垂直运动方向位  | 旋转运动方向位 | 油门量 | 灯亮度控制位 |变焦摄像头动作位  |云台控制位  | 机械臂控制位 |树莓派开机位  |Reserve |Reserve |Reserve |启动停止位|校验位|
| 取值| AA |55  | 10 | 0/1 | 0/1 | 0~255 |  0~255 |  0~255 |  0~255 | 0~3 |0~3  | 1/2/11/12 |  |  | |x  | x| x| |SUM|
|数组编号|0| 1 | 2 |3  | 4 |  5| 6 | 7 | 8 | 9 | 10 |  11|  12| 13 | 14 | 15 |16|17|18|19|
* **数据皆为16进制**
* **以X正方向为机器人正面。**

# 4. 进度
- 驱动模块
	- [x] ADS1118 (ADC)
		- [x] 电压检测 (待实际测试)
		- [x] 电流检测 (待实际测试)
	- [x] PCA9685 (PWM)
	- [x] 深度传感器
		- [x] SPL1301
		- [x] MS5837
	- [x] 系统设备状态
		- [x] CPU使用率、温度
		- [x] 内存状态
		- [x] 硬盘状态
		- [x] 实时网速
	- [x] IO设备
		- [x] RGB
		- [x] 按键
		- [x] 蜂鸣器
	- [ ] PWM设备 (待实际测试)
		- [x] 云台
		- [x] 探照灯
		- [x] 机械臂
	- [x] OLED
	- [x] 九轴
	- [ ] 音频MIC

- 功能模块
	- [x] OLED状态显示
	- [x] 视频推流
	- [x] 数据通信
		- [x] 下行-控制数据
		- [x] 上行-ROV状态数据
	- [ ] 系统自检
	- [x] 日志记录
	- [ ] 传感器融合

- 控制模块
	- [x] 手柄控制 (待实际测试)
	- [x] 定深控制 (待实际测试)
	- [ ] 定向控制
	- [ ] 变焦摄像头控制


