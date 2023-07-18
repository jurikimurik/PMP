QT       += core gui
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Backend/sources/main.cpp \
    Backend/sources/player/pmplayermodel.cpp \
    Backend/sources/playlist/element/playlistmediaelement.cpp \
    Backend/sources/playlist/playlistmodel.cpp \
    GUI/sources/playlist/playlistview.cpp \
    GUI/sources/player/pmplayerview.cpp \
    GUI/sources/video/pmpvideowidget.cpp


HEADERS += \
    Backend/headers/player/pmplayermodel.h \
    Backend/headers/playlist/element/playlistmediaelement.h \
    Backend/headers/playlist/playlistmodel.h \
    GUI/headers/player/pmplayerview.h \
    GUI/headers/playlist/playlistview.h \
    GUI/headers/video/pmpvideowidget.h

FORMS += \
    GUI/ui/player/pmplayerview.ui \
    GUI/ui/video/pmpvideowidget.ui

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
