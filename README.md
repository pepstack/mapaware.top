## mapaware.top

本软件仅供学习之用，任何商业上使用必须首先取得软件许可！

Copyright © 2024-2030 mapaware.top All Rights Reserved.

项目主目录 = PREFIX

项目生成的客户端程序软件（支持 Windows 和 Linux）如下：

- shapetool 这是一个处理地图数据的命令行程序。

- mapaware (或 MapAware) 一个基于 SDL 的地图窗口应用程序。

项目同时会生成客户端程序软件运行需要的动态库。

### 构建必备

- Windows 上构建必须首先安装 ming64 编译环境 (略)

- 解压依赖库 deps

  解压 "deps-YYYYMMDD.zip" 到 $(PREFIX)。确保下面文件存在：
  
      $(PREFIX)/deps/LASTDATE.TXT

- 安装依赖包
  
  $(PREFIX)/packages/ 包含了需要的依赖。也可以从网上下载得到。
  
  参考：BUILD.md

### Windows 上编译

#### 使用 mingw64 编译

    $ cd $(PREFIX)/
    $ make

#### VSCode (mingw64) 编译和调试

    $ cd $(PREFIX)/
    $ code .

#### VS2022 编译和调试

  使用 VS2022 打开项目：
    
    $(PREFIX)/projects/msvc/mapaware-x64-vs2022.sln

### Linux 上编译

    $ cd $(PREFIX)/
    $ make

### 发布软件

TODO

