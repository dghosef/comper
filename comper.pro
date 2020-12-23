QT       += core

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
     src/chord.cpp \
     src/main.cpp \
     src/note.cpp \
     src/probcfg.cpp \
     src/midiwriter.cpp \
     src/midifile/Binasc.cpp \
     src/midifile/MidiEvent.cpp \
     src/midifile/MidiEventList.cpp \
     src/midifile/MidiFile.cpp \
     src/midifile/MidiMessage.cpp

HEADERS += \
     src/chord.h \
     src/comp.h \
     src/note.h \
     src/note_numbers.h \
     src/probcfg.h \
     src/weighted_vector.h \
     src/simpleBassline.h \
     src/midiwriter.h \
     src/midifile/Binasc.h \
     src/midifile/MidiEvent.h \
     src/midifile/MidiEventList.h \
     src/midifile/MidiFile.h \
     src/midifile/MidiMessage.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
