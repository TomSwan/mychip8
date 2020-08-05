// window.h - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#ifndef WINDOW_H
#define WINDOW_H

#include "animator.h"

#include <QWidget>

class Application;
class Interpreter;

class Window : public QWidget
{
    Q_OBJECT
    
public:
    Window(Application &myApp, Interpreter &myChip8);
    void help();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Animator animator;
    Application &myApp;
    Interpreter &myChip8;
};

#endif // WINDOW_H
