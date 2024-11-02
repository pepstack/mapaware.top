## 目标系统环境

### Windows7, 10, 11 64 位

以下动态库必须随软件分发：

      libdeflate.dll
      libgcc_s_seh-1.dll
      libjbig-0.dll
      libjpeg-8.dll
      liblzma-5.dll
      libproj-22.dll
      libsqlite3-0.dll
      libstdc++-6.dll
      libtiff-6.dll
      libwebp-7.dll
      libwinpthread-1.dll
      libzstd.dll
      zlib1.dll
      SDL2.dll

需要安装：VC_redist.x64.exe

  来自于：
      
      https://download.visualstudio.microsoft.com/download/pr/368cc6bf-087b-49f9-93e6-ab05b70a58e0/814E9DA5EC5E5D6A8FA701999D1FC3BADDF7F3ADC528E202590E9B1CB73E4A11/VC_redist.x64.exe

### Linux 64 位

SDL2


## 编译依赖包

### 编译 SQLite3 Multiple Ciphers

下载: SQLite3MultipleCiphers-1.9.0.zip

    $ wget https://codeload.github.com/utelle/SQLite3MultipleCiphers/zip/refs/tags/v1.9.0

解压后编译:

  - Windows

        MINGW64 $ cd SQLite3MultipleCiphers-1.9.0/
        MINGW64 $ autoreconf
        MINGW64 $ mkdir build
        MINGW64 $ cd build/
        MINGW64 $ ../configure --prefix=/usr/local/sqlite3mc-1.9.0-mingw
        MINGW64 $ make && make install
        MINGW64 $ cp sqlite3shell.exe /usr/local/sqlite3mc-1.9.0-mingw/bin/
        MINGW64 $ cp -r ../test /usr/local/sqlite3mc-1.9.0-mingw/

  - Linux

        CentOS7 $ sudo yum install automake
        CentOS7 $ cd SQLite3MultipleCiphers-1.9.0/
        CentOS7 $ autoreconf
        CentOS7 $ mkdir build
        CentOS7 $ cd build/
        CentOS7 $ ../configure --prefix=/usr/local/sqlite3mc-1.9.0
        CentOS7 $ make && sudo make install
        CentOS7 $ sudo mkdir /usr/local/sqlite3mc-1.9.0/bin
        CentOS7 $ sudo cp sqlite3shell.exe /usr/local/sqlite3mc-1.9.0/bin/
        CentOS7 $ sudo cp -r ../test /usr/local/sqlite3mc-1.9.0/


然后可以将 /usr/local/sqlite3mc-1.9.0 目录复制到 deps 目录下备用。

### tpl (C 语言序列化和反序列化)

    http://troydhanson.github.io/tpl/

    https://codeload.github.com/troydhanson/tpl/zip/refs/heads/master

  TODO:


### 编译 openssl (MingGW64/Linux)

以下依赖默认安装在 /usr/local。

      $ wget https://github.com/openssl/openssl/releases/download/openssl-3.4.0/openssl-3.4.0.tar.gz
      $ tar -zxf openssl-3.4.0.tar.gz
      $ cd openssl-3.4.0
      $ ./config -shared // --openssldir=/usr/local/ssl
      $ make && sudo make install

### 编译 proj.4 (MingGW64/Linux)

  参考

  [1] https://zhang.blog.csdn.net/article/details/119535975
  
  [2] https://stackoverflow.com/questions/56336234/build-fail-sphinx-error-contents-rst-not-found

- sqlite3

      $ wget https://www.sqlite.org/2024/sqlite-autoconf-3470000.tar.gz		
      $ tar -zxf sqlite-autoconf-3470000.tar.gz
      $ cd sqlite-autoconf-3470000
      $ ./configure --prefix=/usr/local
      $ make && sudo make install

- libtiff

      $ wget http://download.osgeo.org/libtiff/tiff-4.6.0.tar.xz
      $ tar -xf tiff-4.6.0.tar.xz
      $ cd tiff-4.6.0/doc
      $ echo "master_doc = 'contents'" >> conf.py
      $ mv index.rst contents.rst
      $ cd ..
      $ ./configure --prefix=/usr/local
      $ make && sudo make install

    or:

      $ wget http://download.osgeo.org/libtiff/tiff-4.7.0.tar.xz
      $ tar -xf tiff-4.7.0.tar.xz
      $ cd tiff-4.7.0/doc
      $ echo "master_doc = 'index'" >> conf.py
      $ cd ..
      $ ./configure --prefix=/usr/local
      $ make && sudo make install

- proj.4

注：proj-9.x.y 只能用 cmake 构建，因此这里使用 proj-8.2.1

      $ wget https://download.osgeo.org/proj/proj-8.2.1.tar.gz
      $ tar -zxf proj-8.2.1.tar.gz
      $ cd proj-8.2.1
      $ ./configure SQLITE3_CFLAGS="-I/usr/local/include" SQLITE3_LIBS="-L/usr/local/lib -lsqlite3" TIFF_CFLAGS="-I/usr/local/include" TIFF_LIBS="-L/usr/local/lib -ltiff" --without-curl --prefix=/usr/local
      $ make -j 8 && sudo make install
