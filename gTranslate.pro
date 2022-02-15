QT += quick network gui

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        lib/textfromclip.cpp \
        lib/gtranslate.cpp \
        main.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    gTranslate_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += \
    /home/hgc/Project/QHotkey/QHotkey

LIBS += \
    -L/home/hgc/Project/QHotkey/QHotkey/ -lQHotkey

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = devUI

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = devUI

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/gtranslate.h \
    include/textfromclip.h
