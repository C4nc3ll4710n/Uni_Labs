QT += widgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
                callgraph.h \
                collapser.h \
                highlighter.h \
                xmlwindow.h
SOURCES       = main.cpp \
                callgraph.cpp \
                collapser.cpp \
                highlighter.cpp \
                mainwindow.cpp \
                xmlwindow.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target
