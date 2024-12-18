QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11

TRANSLATIONS += translations/zh.ts\
                translations/en.ts
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        commentdialog.cpp \
        commentlist_item.cpp \
        control.cpp \
        controlvertical.cpp \
        customslider.cpp \
        customwindow.cpp \
        cvolumebutton.cpp \
        cvolumeslider.cpp \
        cvolumewidget.cpp \
        datamanager.cpp \
        listpanel.cpp \
        playlist_item.cpp \
        the_player.cpp \
        tomeo.cpp

HEADERS += \
    commentdialog.h \
    commentlist_item.h \
    control.h \
    controlvertical.h \
    customslider.h \
    customwindow.h \
    cvolumebutton.h \
    cvolumeslider.h \
    cvolumewidget.h \
    datamanager.h \
    listpanel.h \
    playlist_item.h \
    the_player.h \
    the_videowidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    commentdialog.ui \
    commentlist_item.ui \
    control.ui \
    controlvertical.ui \
    listpanel.ui \
    playlist_item.ui

RESOURCES += \
    images.qrc \
    translations.qrc

