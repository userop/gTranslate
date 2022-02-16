# QtQuick 鼠标选中即可翻译

做这个翻译工具的初衷是因为我想看英文资料，但是使用网页版的在线翻译又比较费事，
所以便自己写了这个工具。

该工具最大的特点就是鼠标选中/双击某一段文本/单词，就能实时翻译并显示在鼠标点击的位置。
并且在QHotKey项目的加持下，支持全局热键功能，也就是说可以用快捷键控制开启或者关闭实时翻译功能。
我的快捷键是写死在代码中的 <Ctrl+Shift+Q>


## 编译方法

### 编译全局热键<QHotkey>

    git clone git@github.com:userop/gTranslate.git
    cd gTranslate
    git submodule 
    git submodule init
    git submodule update
    git submodule foreach git checkout master

    cd QHotkey/QHotkey
    qmake
    make

### 编译gTranslate二进制工具

    #回到gTranslate目录
    qmake
    make
    