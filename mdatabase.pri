QT *= sql

INCLUDEPATH += $$PWD

SOURCES += \
        $$PWD/connectionproviders/mconnectionproviderbase.cpp \
        $$PWD/connectionproviders/mconnectionprovidersqlite.cpp \
        $$PWD/connectionproviders/mconnectionprovidersqlitecipher.cpp \
        $$PWD/connectionproviders/mconnectionprovidersqlitesee.cpp \
        $$PWD/mdbhelpers.cpp \
        $$PWD/mmigration.cpp \
        $$PWD/mabstractmigrationmanager.cpp

HEADERS += \
        $$PWD/connectionproviders/mconnectionproviderbase.h \
        $$PWD/connectionproviders/mconnectionprovidersqlite.h \
        $$PWD/connectionproviders/mconnectionprovidersqlitecipher.h \
        $$PWD/connectionproviders/mconnectionprovidersqlitesee.h \
        $$PWD/mdbhelpers.h \
        $$PWD/mmigration.h \
        $$PWD/mmigrationmanager.h \
        $$PWD/mabstractmigrationmanager.h \
        $$PWD/mmigrationsdata.h
