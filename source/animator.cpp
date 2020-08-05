// animator.cpp - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#include "animator.h"
#include "application.h"
#include "config.h"
#include "interpreter.h"

#include <QPainter>
#include <QPaintEvent>

Animator::Animator(Application &myApp)
{
    backgroundBrush = QBrush(QColor(myApp.backgroundColor));
    pixelBrush = QBrush(QColor(myApp.pixelColor));
}

void Animator::paint(QPainter *painter, 
    Application &myApp, 
    Interpreter &myChip8)
{
    QBrush brush;
    
    unsigned display_x;
    unsigned display_y;
    
    for (unsigned y = 0; y < myApp.screenHeight; y++) {
        display_y = (y + 1) * myApp.blockSize;
        for (unsigned x = 0; x < myApp.screenWidth; x++) {
            display_x = (x + 1) * myApp.blockSize;
            if(myChip8.gfx[(y * 64) + x] == 0)
                brush = backgroundBrush;
            else
                brush = pixelBrush;
            painter->fillRect(display_x, display_y, 
                myApp.blockSize, myApp.blockSize, brush);
        }
    }
}
