QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
include/BoardType.h \
include/CalibrationOptions.h \
include/DistortionLevel.h \
    include/CalculateCorner.h \
    include/CameraCalibrate.h \
    include/ImageUndistort.h \
    include/PointMapper.h \
    include/Tools.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += include
#LIBS += -L$$(PWD)/../Computer-Vision-QT/lib/win32/Debug/  -lopencv_world4100d



CONFIG += debug_and_release

# Check for debug mode
CONFIG(debug, debug|release) {
    contains(DEFINES, WIN64): message("Debugging on Win64")
    else: {
        message("Debugging on Win32")
        # LIBS += -L$$(PWD)/../Computer-Vision-QT/lib/win32/Debug/  -lCV_DLL
        LIBS += -LD:\Desktop\Computer-Vision-QT\Computer-Vision-QT\lib\win32\Debug   -lCV_DLL
    }
} else {
    # Not in debug mode (i.e., release mode)
    contains(DEFINES, WIN64): message("Releasing on Win64")
    else: {
        message("Releasing on Win32")
        LIBS += -L$$(PWD)/../Computer-Vision-QT/lib/win32/Release/  -lCV_DLL
    }
}
