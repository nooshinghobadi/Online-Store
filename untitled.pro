#-------------------------------------------------
#
# Project created by QtCreator 2024-12-13T07:32:01
#
#-------------------------------------------------

QT       += core gui widgets sql # Add SQL support for SQLite

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = online_store_simulation
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           product.cpp \
           cart.cpp \
           order.cpp \
           database.cpp

HEADERS  += mainwindow.h \
            product.h \
            cart.h \
            order.h \
            database.h \
    product.h \
    cart.h \
    order.h \
    database.h

FORMS    += mainwindow.ui
CONFIG   += c++11
QT       += sql

