QT       += core gui
QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
requires(qtConfig(undoview))

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filesystem/extendedqfilesystemmodel.cpp \
    filesystem/projectloader.cpp \
    filesystem/sequencedata.cpp \
    filesystem/sequenceregister.cpp \
    filesystem/tbesounddata.cpp \
    parameters_interfaces/customqslider.cpp \
    parameters_interfaces/customshotparameterinterface.cpp \
    parameters_interfaces/shotparametersinterface.cpp \
    sequence_elements/soundtrack.cpp \
    sequence_elements\shot.cpp \
    main.cpp \
    mainwindow.cpp \
    sequence_elements\ruler.cpp \
    sequence_elements\timelinescene.cpp \
    undo_framework/commands.cpp

HEADERS += \
    filesystem/extendedqfilesystemmodel.h \
    filesystem/projectloader.h \
    filesystem/sequencedata.h \
    filesystem/sequenceregister.h \
    filesystem/tbesounddata.h \
    parameters_interfaces/customqslider.h \
    parameters_interfaces/customshotparameterinterface.h \
    parameters_interfaces/shotparametersinterface.h \
    sequence_elements/soundtrack.h \
    sequence_elements\shot.h \
    mainwindow.h \
    sequence_elements\ruler.h \
    sequence_elements\timelinescene.h \
    undo_framework/commands.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES = \
    ressources.qrc
