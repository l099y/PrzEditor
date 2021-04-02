QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filesystem/extendedqfilesystemmodel.cpp \
    filesystem/sequencedata.cpp \
    filesystem/sequenceregister.cpp \
    sequence_elements/storage/graphicsequence.cpp \
    sequence_elements/storage/sequenceroom.cpp \
    sequence_elements\extendedqgri.cpp \
    main.cpp \
    mainwindow.cpp \
    emitter.cpp \
    sequence_elements\ruler.cpp \
    sequence_elements\timelinescene.cpp

HEADERS += \
    filesystem/extendedqfilesystemmodel.h \
    filesystem/sequencedata.h \
    filesystem/sequenceregister.h \
    sequence_elements/storage/graphicsequence.h \
    sequence_elements/storage/sequenceroom.h \
    sequence_elements\extendedqgri.h \
    mainwindow.h \
    emitter.h \
    sequence_elements\ruler.h \
    sequence_elements\timelinescene.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
