#include "animator.h"
#include "application.h"
#include "config.h"
#include "glwidget.h"
#include "interpreter.h"
#include "util.h"

#include <QApplication>
#include <QPainter>
#include <QTimer>

GLWidget::GLWidget(
    QWidget *parent,
    Animator &animator, 
    Application &myApp, 
    Interpreter &myChip8)
    : 
    QOpenGLWidget(parent), 
    animator(animator), 
    myApp(myApp), 
    myChip8(myChip8)
{
    setFixedSize(myApp.displayWidth, myApp.displayHeight);
}

void GLWidget::animate()
{
    update();
}

void GLWidget::cycle()
{
    for (unsigned i = 0; i < myApp.stepsPerCycle; i++) {
        if (!myApp.debugging) myChip8.emulateCycle(); else {
            if ( !ok("DEBUG : Continue") ) exit(0);
            myChip8.emulateCycle(); 
            myApp.report();
        }
    }
}

void GLWidget::delayTimer()
{
    if(myChip8.delay_timer > 0)
        --myChip8.delay_timer;
}
    
void GLWidget::soundTimer()
{
    if(myChip8.sound_timer > 0) {
        QApplication::beep();
        --myChip8.sound_timer;
    }
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    animator.paint(&painter, myApp, myChip8);
    painter.end();
}
