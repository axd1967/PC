#-------------------------------------------------
#
# Project created by QtCreator 2013-01-04T11:39:52
#
#-------------------------------------------------

TARGET = NeobotWidgetTools
TEMPLATE = lib
CONFIG += staticlib

QT += widgets
QT += printsupport

include( ../../BuildConfiguration.pri)

INCLUDEPATH += ../ToolsLib
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/QCustomPlot
INCLUDEPATH += $${EXT_LIBS_HEADER_DIR}/qtpropertybrowser/src

win32:INCLUDEPATH += .

HEADERS += TextEditLogger.h \
    SlidingStackedWidget.h \
    PointEdit.h \
    FullScreenWidget.h \
    StrategyScene.h \
    StrategyView.h \
    NPlotWidget.h \
    NSettingsPropertyBrowser.h \
    Ax12MovementEditor.h \
    SpinBoxDelegate.h \
    TapWidget.h \
    ColorDisplayWidget.h \
    LineHighlighting.h \
    NCodeEdit.h \
    SyntaxRuleHighlighting.h \
    TextHighlighter.h \
    TextHighlighterStrategyInterface.h \
    NSHelpDialog.h

SOURCES += TextEditLogger.cpp \
    SlidingStackedWidget.cpp \
    PointEdit.cpp \
    FullScreenWidget.cpp \
    StrategyScene.cpp \
    StrategyView.cpp \
    NPlotWidget.cpp \
    NSettingsPropertyBrowser.cpp \
    Ax12MovementEditor.cpp \
    SpinBoxDelegate.cpp \
    TapWidget.cpp \
    ColorDisplayWidget.cpp \
    LineHighlighting.cpp \
    NCodeEdit.cpp \
    SyntaxRuleHighlighting.cpp \
    TextHighlighter.cpp \
    TextHighlighterStrategyInterface.cpp \
    NSHelpDialog.cpp

RESOURCES += WidgetToolsLib.qrc

FORMS += \
    Ax12MovementEditor.ui \
    NSHelpDialog.ui
