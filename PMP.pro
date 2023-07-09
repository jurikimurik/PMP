QT       += core gui
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    sources/main.cpp \
    sources/player/pmplayermodel.cpp \
    sources/player/pmplayerview.cpp

HEADERS += \
    headers/player/pmplayermodel.h \
    headers/player/pmplayerview.h

FORMS += \
    GUI/player/pmplayerview.ui

TRANSLATIONS += \
    translations/PMP_pl_PL.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/icons/icons.qrc
