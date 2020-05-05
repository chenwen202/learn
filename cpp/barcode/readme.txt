条码识别Demo，其中用到了barcode，opencv等算法库
编译环境：
编译工具：x86_64-w64-mingw32 : 64-bit Windows
         barcode, opencv  win64 libraries
配置说明：
1. 代码库和头文件配置， makefile里面设置library, include
    c_cpp_properties.json设置include (barcode and opencv)


如何编译barcode
1. 代码下载
    https://github.com/mchehab/zbar.git
2. 编译barcode, x64 lib
Building on Windows Subsystem for Linux (WSL)
-------------------------

You need to install the following packages:

    sudo apt-get install -y \
	autoconf automake autotools-dev libdbus-1-dev \
	pkg-config binutils-mingw-w64 gcc-mingw-w64 \
	mingw-w64-dev mingw-w64-common win-iconv-mingw-w64-dev \
	xmlto

Then, build Zbar with:

	export PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/lib/pkgconfig

    autoreconf -vfi

    CC=x86_64-w64-mingw32-gcc LDFLAGS=-L/usr/local/mingw64/lib  LIBS+=-liconv ./configure \
    --host=x86_64-w64-mingw32 --prefix=/usr/local/win64 --with-directshow --without-gtk \
    --without-python --without-qt --without-java --without-imagemagick --enable-pthread

    make    

3. 其它
在编译zbar时候，需要链接iconv.dll库，如果从网上下载的libiconv库，导出函数前面都是带有 lib的可能会不一致。
例如：我下载的libiconv-1.15，就是存在这样的问题。
我的解决方法是通过，字符串替换命令将zbar里面用到的iconv函数都添加上lib前缀
命令：
grep -rlz 'iconv_open' |xargs -0 sed -i 's|iconv_open|libiconv_open|g'


补充资料：
how to build win32 lib
Building on Ubuntu Bionic
-------------------------

You need to install the following packages:

    sudo apt-get install -y \
	autoconf automake autotools-dev libdbus-1-dev \
	pkg-config binutils-mingw-w64-i686 gcc-mingw-w64 \
	mingw-w64-i686-dev mingw-w64-common win-iconv-mingw-w64-dev \
	xmlto

Then, build Zbar with:

    export PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/lib/pkgconfig

    autoreconf -vfi

    CC=i686-w64-mingw32-gcc  ./configure --host=i686-w64-mingw32 \
	--prefix=/usr/local/win32 --with-directshow \
	--without-gtk --without-python --without-qt --without-java \
	--without-imagemagick --enable-pthread

    make



