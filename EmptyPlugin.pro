!include("../Common/retroshare_plugin.pri"): error("Could not include file ../Common/retroshare_plugin.pri")

CONFIG += qt uic qrc resources

SOURCES = EmptyPluginPlugin.cpp \
    topjcdialog.cpp
HEADERS = EmptyPluginPlugin.h \
    topjcdialog.h
FORMS   = \
    topjcdialog.ui

TARGET = EmptyPlugin

RESOURCES = EmptyPlugin_images.qrc lang/EmptyPlugin_lang.qrc

TRANSLATIONS +=  \
                        lang/EmptyPlugin_cs.ts \
                        lang/EmptyPlugin_da.ts \
                        lang/EmptyPlugin_de.ts \
                        lang/EmptyPlugin_el.ts \
                        lang/EmptyPlugin_en.ts \
                        lang/EmptyPlugin_es.ts \
                        lang/EmptyPlugin_fi.ts \
                        lang/EmptyPlugin_fr.ts \
                        lang/EmptyPlugin_hu.ts \
                        lang/EmptyPlugin_it.ts \
                        lang/EmptyPlugin_ja_JP.ts \
                        lang/EmptyPlugin_ko.ts \
                        lang/EmptyPlugin_nl.ts \
                        lang/EmptyPlugin_pl.ts \
                        lang/EmptyPlugin_ru.ts \
                        lang/EmptyPlugin_sv.ts \
                        lang/EmptyPlugin_tr.ts \
                        lang/EmptyPlugin_zh_CN.ts

OTHER_FILES += \
    testit.sh
