#ifndef APPLICATION_H
#define APPLICATION_H

#include <QColor>
#include <QString>

class QApplication;
class Interpreter;

class Application
{
public:
    Application(Interpreter &myChip8);

public:
    bool altKeys;
    bool debugging;
    bool silentRunning;
    
    QColor backgroundColor;    
    QColor pixelColor;   
     
    unsigned blockSize;
    unsigned cycleRateMS;
    unsigned delayTimerRateMS;
    unsigned displayHeight;
    unsigned displayWidth;      
    unsigned refreshRateMS;
    unsigned screenHeight;
    unsigned screenWidth;
    unsigned soundTimerRateMS;
    unsigned stepsPerCycle;    
    
    QString sourceFileName;
    QString outputFileName;

    bool init(QApplication &app);
    void report();

private:
    Interpreter &myChip8;
};

#endif
