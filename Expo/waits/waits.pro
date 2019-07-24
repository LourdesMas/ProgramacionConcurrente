QT = core
CONFIG -= moc app_bundle
CONFIG += console

SOURCES += waits.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/threads/waitconditions
INSTALLS += target