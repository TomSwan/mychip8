// main.cpp - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#include "application.h"
#include "config.h"
#include "interpreter.h"
#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{		
	QApplication app(argc, argv);
    QApplication::setApplicationName(TITLE);
    QApplication::setApplicationVersion(VERSION);

    Interpreter myChip8;
    Application myApp(myChip8);

    if (!myApp.init(app)) return 1;

    Window window(myApp, myChip8);
    window.show();
    return app.exec();
}
