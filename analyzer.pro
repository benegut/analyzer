######################################################################
# Automatically generated by qmake (3.1) Thu Jun 16 21:30:21 2022
######################################################################

QT = gui core widgets
TEMPLATE = app
TARGET = analyzer
INCLUDEPATH += ./

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Tell the qcustomplot header that it will be used as library:
DEFINES += QCUSTOMPLOT_USE_LIBRARY

# Link with debug version of qcustomplot if compiling in debug mode, else with release library:
CONFIG(debug, release|debug) {
  win32:QCPLIB = qcustomplotd1
  else: QCPLIB = qcustomplotd
} else {
  win32:QCPLIB = qcustomplot1
  else: QCPLIB = qcustomplot
}
LIBS += -L./ -l$$QCPLIB -lkfr_io -lkfr_dft

# Input
HEADERS += window.hpp
SOURCES += main.cpp window.cpp video.cpp parser.cpp plotcontextmenu.cpp filtercontextmenu.cpp dialog.cpp
