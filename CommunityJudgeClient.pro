#-------------------------------------------------
#
# Project created by QtCreator 2013-08-01T15:19:04
#
#-------------------------------------------------

QT       += core network gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommunityJudgeClient
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x


SOURCES += main.cpp \
    registeration.cpp \
    connection.cpp \
    submission.cpp \
    scrollabletextarea.cpp \
    submitdetails.cpp \
    viewresults.cpp \
    mainwidget.cpp \
    login.cpp \
    loginwidget.cpp \
    addserverdialog.cpp \
    userwidget.cpp \
    textdialog.cpp \
    changepassworddialog.cpp \
    adminpanel.cpp \
    problemmanagement.cpp \
    problemspecsdialog.cpp \
    scorepreview.cpp \
    scoreplanmanager.cpp \
    usermanagement.cpp \
    viewprofile.cpp \
    serverconfig.cpp \
    submissionpurge.cpp \
    scoreboard.cpp \
    problemview.cpp \
    news.cpp \
    newsdialog.cpp \
    contests.cpp \
    contestmanagement.cpp \
    contestspecsdialog.cpp \
    scoreboardconfig.cpp \
    scoreboardspecsdialog.cpp \
    userrecorddialog.cpp

HEADERS  += \
    registeration.h \
    connection.h \
    submission.h \
    scrollabletextarea.h \
    submitdetails.h \
    viewresults.h \
    mainwidget.h \
    login.h \
    loginwidget.h \
    addserverdialog.h \
    userwidget.h \
    textdialog.h \
    changepassworddialog.h \
    adminpanel.h \
    problemmanagement.h \
    problemspecsdialog.h \
    scorepreview.h \
    scoreplanmanager.h \
    usermanagement.h \
    viewprofile.h \
    serverconfig.h \
    submissionpurge.h \
    scoreboard.h \
    problemview.h \
    news.h \
    newsdialog.h \
    contests.h \
    contestmanagement.h \
    contestspecsdialog.h \
    scoreboardconfig.h \
    scoreboardspecsdialog.h \
    userrecorddialog.h \
    general.h

RESOURCES += \
    images.qrc

RC_FILE = Windows.rc
