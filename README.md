# QMooTool Qt 编写的开发者桌面小工具

[TOC]

## 简述
前些日子在逛开源中国的时候，看到一个开源的小工具 [MooTool](https://www.oschina.net/p/mootool) ，想起来自己之前也想做一个方便的小工具，以便提升日常工作的效率。

现在准备动手写这个工具了，既然用Qt编写，那么就叫 QMooTool 好了。

## 功能列表
这里列出工具的所有功能列表，没有实现的会慢慢做。因为这个主要是个人使用，所以会根据在工作中的需求，慢慢添加。

### 1、时间格式转换
实现获取当前时间戳，并支持多种时间字符串格式之间进行转换。

### 2、JSON 相关处理
实现 JSON 的格式化、压缩、存为普通文件或 [CBOR 简明的二进制对象表示](http://cbor.io/) 文件以及可视化查看等功能。

后期待实现功能：
- JSON 转 XML
- JSON 生成 C++ 解析和生成它的代码
- JSON 生成 Go 代码

### 3、XML 相关处理

### 4、文本编解码
实现 Base32/64 、URL 、Unicode 编解码，已经字符串添加或移除字符转义等。

### 5、二维码
实现二维码生成，已经屏幕上二维码扫描。

### 6、文件转代码
实现将文件转换为 C/C++ 代码，以便嵌入到程序中。

### 7、随机密码生成

