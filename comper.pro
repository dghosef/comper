QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chord.cpp \
    main.cpp \
    mainwindow.cpp \
    note.cpp \
    probcfg.cpp

HEADERS += \
    chord.h \
    mainwindow.h \
    note.h \
    note_numbers.h \
    probcfg.h \
    weighted_vector.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
LIBS += /usr/lib/libfluidsynth.so -lfluidsynth
