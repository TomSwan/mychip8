// glwidget.h - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QPaintEvent>

class Animator;
class Application;
class Interpreter;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent,
        Animator &animator, 
        Application &myApp, 
        Interpreter &myChip8);

public slots:
    void animate();
    void cycle();
    void delayTimer();
    void soundTimer();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Animator &animator;
    Application &myApp;
    Interpreter &myChip8;
};

#endif
