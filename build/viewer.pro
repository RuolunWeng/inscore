win32 { TEMPLATE = vcapp }
else  { TEMPLATE = app }

unix:!macx:!ios:!android:TARGET = inscoreviewer
else { TARGET = INScoreViewer }
OBJECTS_DIR = tmp
MOC_DIR		= tmp
RCC_DIR		= tmp

VERSION = 1.23

QT += core gui widgets svg printsupport multimedia multimediawidgets websockets
QT += quick qml quickwidgets sensors

############################## 
# locations
############################## 
ROOT = $$PWD/..
SRC  = $$ROOT/src
LOCALLIB 	= $$ROOT/lib
APPL = $$SRC/inscoreviewer
LIB  = $$SRC/inscore
OSC  = $$LOCALLIB/oscpack
win32 { OSCIP = $$OSC/ip/win32 }
else  { OSCIP = $$OSC/ip/posix }

CONFIG += c++11

############################## 
# source and headers
############################## 
NOVIEW { SOURCES += $$APPL/INScoreNoView.cpp } 
else   { SOURCES += $$APPL/INScoreAppl.cpp }
HEADERS 	+= $$APPL/INScoreAppl.h
INCLUDEPATH += $$APPL $$LIB/interface
INCLUDEPATH += $$LIB/model $$LIB/controller $$LIB/lib $$LIB/mapping $$LIB/events $$LIB/view
INCLUDEPATH += $$LIB/signal $$LIB/scripting $$LIB/expression
INCLUDEPATH += $$files($$OSC)
INCLUDEPATH += $$files($$SRC/libmapping/src/[^.]*)

############### Resources
RESOURCES += $$ROOT/rsrc/inscore.qrc
ICON = $$ROOT/rsrc/INScoreViewer.icns

############################## 
# macos x support
############################## 
macx {
	QMAKE_LFLAGS += -FRelease
	LIBS += -framework INScore
	QMAKE_INFO_PLIST = $$PWD/Info.plist
	RSRC.files 	= $$ROOT/rsrc/INScoreViewer.icns
	RSRC.path 	= Contents/Resources
	PLUG.files 	= $$ROOT/lib/libMusicXML/macosx/libmusicxml2.dylib
	PLUG.files += $$ROOT/lib/GestureFollower/macosx/libGFLib.dylib
	PLUG.files += $$ROOT/lib/faust/macosx/libfaust.dylib
	PLUG.path 	= Contents/PlugIns
	QMAKE_BUNDLE_DATA += PLUG RSRC
	ICON 		= $$ROOT/rsrc/INScoreViewer.icns
}

##############################
# ios support
##############################
ios {
    QMAKE_IOS_DEPLOYMENT_TARGET = 8.0
	QMAKE_LFLAGS += -L Release-iphoneos
	LIBS += -lINScore $$ROOT/lib/GuidoEngine/ios/libGUIDOEngine.a $$ROOT/lib/GuidoAR/ios/libguidoar.a
	QMAKE_INFO_PLIST = $$PWD/Info-ios.plist
	ios_icon.files = $$files($$ROOT/rsrc/ios-icons/*.png)
	QMAKE_BUNDLE_DATA += ios_icon
	OBJECTIVE_SOURCES += $$SRC/mobile/ios/QtAppDelegate.mm
	INCLUDEPATH += $$SRC/mobile/ios
	HEADERS += $$files($$SRC/mobile/ios/*.h)
	RESOURCES += $$PWD/tmp-rsc/inscoremobile.qrc $$ROOT/rsrc/inscorescriptmobileios.qrc
	DEFINES += IOS __MOBILE__
    CONFIG+= arm64 armv7 armv7s
}

############################## 
# windows support
# assumes environment is MSVC
############################## 
win32 {
	VERSION = ""
	CONFIG(debug,debug|release) { LIBS   += debug/INScore.lib }
	else { LIBS   += release/INScore.lib }
	RC_FILE = $$ROOT/win32/Viewer/INScoreViewer.rc
}

############################## 
# linux support
############################## 
unix:!android:!macx:!ios:LIBS += -L. -lINScore -lGUIDOEngine -lguidoar -lmicrohttpd

############################## 
# android support
############################## 
android {
	LIBS += -L. -lINScore
	ANDROID_EXTRA_LIBS = $$ROOT/lib/GuidoEngine/android/libGUIDOEngine.so $$ROOT/lib/GuidoAR/android/libguidoar.so libINScore.so
	DISTFILES +=  $$ROOT/rsrc/android/AndroidManifest.xml
	ANDROID_PACKAGE_SOURCE_DIR = $$ROOT/rsrc/android
	QT += androidextras
	DEFINES += ANDROID __MOBILE__
	RESOURCES += $$PWD/tmp-rsc/inscoremobile.qrc $$ROOT/rsrc/inscorescriptmobile.qrc
}

mobile_ui{
    DEFINES += __MOBILE__
    RESOURCES += $$PWD/tmp-rsc/inscoremobile.qrc $$ROOT/rsrc/inscorescriptmobile.qrc
}

