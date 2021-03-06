// animator.h - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <QBrush>
#include <QWidget>

class Application;
class Interpreter;

class Animator
{
public:
    Animator(Application &myApp);

public:
    void paint(QPainter *painter, 
        Application &myApp, 
        Interpreter &myChip8);

private:
    QBrush backgroundBrush;
    QBrush pixelBrush;
};

#endif
