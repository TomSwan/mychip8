#include "application.h"
#include "config.h"
#include "glwidget.h"
#include "interpreter.h"
#include "util.h"
#include "window.h"

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>

#include <iostream>

using namespace std;

Window::Window(Application &myApp, Interpreter &myChip8) :
    animator(myApp),
    myApp(myApp), 
    myChip8(myChip8)
{
    setWindowTitle(TITLE);

    GLWidget *openGL = new GLWidget(this, animator, myApp, myChip8);
    QLabel *openGLLabel = new QLabel(CAPTION);
    openGLLabel->setAlignment(Qt::AlignHCenter);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(openGL, 0, 1);
    layout->addWidget(openGLLabel, 1, 1);
    setLayout(layout);

    QTimer *timerA = new QTimer(this);
    connect(timerA, &QTimer::timeout, openGL, &GLWidget::animate);
    QTimer *timerB = new QTimer(this);
    connect(timerB, &QTimer::timeout, openGL, &GLWidget::cycle);
    if (!myApp.silentRunning) {
    QTimer *timerC = new QTimer(this);
    connect(timerC, &QTimer::timeout, openGL, &GLWidget::soundTimer);
    timerC->start(myApp.soundTimerRateMS);
    }
    QTimer *timerD = new QTimer(this);
    connect(timerD, &QTimer::timeout, openGL, &GLWidget::delayTimer);
    timerD->start(myApp.delayTimerRateMS);
    timerA->start(myApp.refreshRateMS);
    timerB->start(myApp.cycleRateMS);
}

void Window::help()
{
    cout << endl 
         << "1 2 3 4  ==  1 2 3 C" << endl;
    cout << "Q W E R  ==  4 5 6 D" << endl;
    cout << "A S D F  ==  7 8 9 E" << endl;
    cout << "Z X C V  ==  A 0 B F" << endl;
    cout << endl << TITLE << endl;
}

void Window::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        exit(0);

         if(event->key() == Qt::Key_1)    myChip8.key[0x1] = 1;
    else if(event->key() == Qt::Key_2)    myChip8.key[0x2] = 1;
    else if(event->key() == Qt::Key_3)    myChip8.key[0x3] = 1;
    else if(event->key() == Qt::Key_4)    myChip8.key[0xC] = 1;

    else if(event->key() == Qt::Key_Q)    myChip8.key[0x4] = 1;
    else if(event->key() == Qt::Key_W)    myChip8.key[0x5] = 1;
    else if(event->key() == Qt::Key_E)    myChip8.key[0x6] = 1; 
    else if(event->key() == Qt::Key_R)    myChip8.key[0xD] = 1;

    else if(event->key() == Qt::Key_A)    myChip8.key[0x7] = 1;
    else if(event->key() == Qt::Key_S)    myChip8.key[0x8] = 1;
    else if(event->key() == Qt::Key_D)    myChip8.key[0x9] = 1;
    else if(event->key() == Qt::Key_F)    myChip8.key[0xE] = 1;

    else if(event->key() == Qt::Key_Z)    myChip8.key[0xA] = 1;
    else if(event->key() == Qt::Key_X)    myChip8.key[0x0] = 1;
    else if(event->key() == Qt::Key_C)    myChip8.key[0xB] = 1;
    else if(event->key() == Qt::Key_V)    myChip8.key[0xF] = 1;

    else if(event->key() == Qt::Key_F1)   help();
    else if (myApp.altKeys) {
         if(event->key() == Qt::Key_Up)    myChip8.key[0x5] = 1;
    else if(event->key() == Qt::Key_Down)  myChip8.key[0x8] = 1;
    else if(event->key() == Qt::Key_Left)  myChip8.key[0x7] = 1;
    else if(event->key() == Qt::Key_Right) myChip8.key[0x9] = 1;
    }
        
}

void Window::keyReleaseEvent(QKeyEvent *event)
{
         if(event->key() == Qt::Key_1)    myChip8.key[0x1] = 0;
    else if(event->key() == Qt::Key_2)    myChip8.key[0x2] = 0;
    else if(event->key() == Qt::Key_3)    myChip8.key[0x3] = 0;
    else if(event->key() == Qt::Key_4)    myChip8.key[0xC] = 0;
    else if(event->key() == Qt::Key_Q)    myChip8.key[0x4] = 0;
    else if(event->key() == Qt::Key_W)    myChip8.key[0x5] = 0;
    else if(event->key() == Qt::Key_E)    myChip8.key[0x6] = 0;
    else if(event->key() == Qt::Key_R)    myChip8.key[0xD] = 0;
    else if(event->key() == Qt::Key_A)    myChip8.key[0x7] = 0;
    else if(event->key() == Qt::Key_S)    myChip8.key[0x8] = 0;
    else if(event->key() == Qt::Key_D)    myChip8.key[0x9] = 0;
    else if(event->key() == Qt::Key_F)    myChip8.key[0xE] = 0;
    else if(event->key() == Qt::Key_Z)    myChip8.key[0xA] = 0;
    else if(event->key() == Qt::Key_X)    myChip8.key[0x0] = 0;
    else if(event->key() == Qt::Key_C)    myChip8.key[0xB] = 0;
    else if(event->key() == Qt::Key_V)    myChip8.key[0xF] = 0;
    else if (myApp.altKeys) {
         if(event->key() == Qt::Key_Up)    myChip8.key[0x5] = 0;
    else if(event->key() == Qt::Key_Down)  myChip8.key[0x8] = 0;
    else if(event->key() == Qt::Key_Left)  myChip8.key[0x7] = 0;
    else if(event->key() == Qt::Key_Right) myChip8.key[0x9] = 0;
    }
}
