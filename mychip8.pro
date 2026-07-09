QT          += widgets

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-result
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

INCLUDEPATH += source

HEADERS     = \
              config.h \
              $${INCLUDEPATH}/animator.h \
              $${INCLUDEPATH}/application.h \
              $${INCLUDEPATH}/glwidget.h \
              $${INCLUDEPATH}/interpreter.h \
              $${INCLUDEPATH}/util.h \
              $${INCLUDEPATH}/window.h
SOURCES     = \
              main.cpp \
              $${INCLUDEPATH}/animator.cpp \
              $${INCLUDEPATH}/application.cpp \
              $${INCLUDEPATH}/glwidget.cpp \
              $${INCLUDEPATH}/interpreter.cpp \
              $${INCLUDEPATH}/util.cpp \
              $${INCLUDEPATH}/window.cpp
