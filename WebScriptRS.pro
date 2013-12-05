!include("../Common/retroshare_plugin.pri"): error("Could not include file ../Common/retroshare_plugin.pri")

CONFIG += qt uic qrc resources
QT += webkit
QT += network
SOURCES = WebScriptRSPlugin.cpp \
    topjcdialog.cpp \
    RSJsonItems.cpp \
    p3JsonRS.cpp \
    webviewrs.cpp \
    msgQue.cpp \
    webbridgers.cpp \
    webtabcontents.cpp \
    filedownloader.cpp \
    pytabcontents.cpp \
    testservice.cpp
HEADERS = WebScriptRSPlugin.h \
    topjcdialog.h \
    RSJsonItems.h \
    p3JsonRS.h \
    msgQue.h \
    webviewrs.h \
    webbridgers.h \
    webtabcontents.h \
    filedownloader.h \
    pytabcontents.h \
    testservice.h
FORMS   = topjcdialog.ui \
    webtabcontents.ui

#embedded pyqt
INCLUDEPATH += -I./embpyqt

SOURCES += \
    embpyqt/embeddedpyqt.cpp \
    embpyqt/pythonize.cpp
HEADERS += \
    embpyqt/embeddedpyqt.h \
    embpyqt/pythonize.h
# Python
LIBS        += -lpython2.7 -L/usr/lib/
INCLUDEPATH += /usr/include/python2.7/

# RPC
PRE_TARGETDEPS *= ./qjsonrpc/src/libqjsonrpc.a

LIBS += ./qjsonrpc/src/libqjsonrpc.a
INCLUDEPATH += ./qjsonrpc/src/
LIBS += -L./qjsonrpc/src/ #-lqjsonrpc

TARGET = WebScriptRS

RESOURCES = WebScriptRS_images.qrc lang/WebScriptRS_lang.qrc

TRANSLATIONS +=  \
                        lang/WebScriptRS_cs.ts \
                        lang/WebScriptRS_da.ts \
                        lang/WebScriptRS_de.ts \
                        lang/WebScriptRS_el.ts \
                        lang/WebScriptRS_en.ts \
                        lang/WebScriptRS_es.ts \
                        lang/WebScriptRS_fi.ts \
                        lang/WebScriptRS_fr.ts \
                        lang/WebScriptRS_hu.ts \
                        lang/WebScriptRS_it.ts \
                        lang/WebScriptRS_ja_JP.ts \
                        lang/WebScriptRS_ko.ts \
                        lang/WebScriptRS_nl.ts \
                        lang/WebScriptRS_pl.ts \
                        lang/WebScriptRS_ru.ts \
                        lang/WebScriptRS_sv.ts \
                        lang/WebScriptRS_tr.ts \
                        lang/WebScriptRS_zh_CN.ts

OTHER_FILES += \
    testit.sh \
    html/interactiveparticles.html \
    html/index.html \
    html/broadcastchat.html \
    README.md \
    html/phystest.html \
    lang/WebScriptRS_zh_CN.qm \
    lang/WebScriptRS_tr.qm \
    lang/WebScriptRS_sv.qm \
    lang/WebScriptRS_ru.qm \
    lang/WebScriptRS_pl.qm \
    lang/WebScriptRS_nl.qm \
    lang/WebScriptRS_ko.qm \
    lang/WebScriptRS_ja_JP.qm \
    lang/WebScriptRS_it.qm \
    lang/WebScriptRS_hu.qm \
    lang/WebScriptRS_fr.qm \
    lang/WebScriptRS_fi.qm \
    lang/WebScriptRS_es.qm \
    lang/WebScriptRS_en.qm \
    lang/WebScriptRS_el.qm \
    lang/WebScriptRS_de.qm \
    lang/WebScriptRS_da.qm \
    lang/WebScriptRS_cs.qm \
    lang/WebScriptRS_zh_CN.ts \
    lang/WebScriptRS_tr.ts \
    lang/WebScriptRS_sv.ts \
    lang/WebScriptRS_ru.ts \
    lang/WebScriptRS_pl.ts \
    lang/WebScriptRS_nl.ts \
    lang/WebScriptRS_ko.ts \
    lang/WebScriptRS_ja_JP.ts \
    lang/WebScriptRS_it.ts \
    lang/WebScriptRS_hu.ts \
    lang/WebScriptRS_fr.ts \
    lang/WebScriptRS_fi.ts \
    lang/WebScriptRS_es.ts \
    lang/WebScriptRS_en.ts \
    lang/WebScriptRS_el.ts \
    lang/WebScriptRS_de.ts \
    lang/WebScriptRS_da.ts \
    lang/WebScriptRS_cs.ts \
    html/phystest2.html \
    html/directchat.html \
    html/ivank/car.png \
    html/ivank/asphalt.jpg \
    html/ivank/ivank.js \
    html/ivank/kevents.html
