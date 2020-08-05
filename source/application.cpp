// application.cpp - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#include "application.h"
#include "config.h"
#include "interpreter.h"
#include "util.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QString>
#include <QTextStream>

#include <iostream>
#include <iomanip>

using namespace std;

Application::Application(Interpreter &myChip8) : 
    altKeys(ALT_KEYS),
    debugging(DEBUGGING),
    silentRunning(SILENT_RUNNING),
    backgroundColor(BACKGROUND_COLOR),
    pixelColor(PIXEL_COLOR),
    blockSize(BLOCK_SIZE),
    cycleRateMS(CYCLE_RATE_MS),
    delayTimerRateMS(DELAY_TIMER_RATE_MS),
    refreshRateMS(REFRESH_RATE_MS),
    screenHeight(SCREEN_HEIGHT),
    screenWidth(SCREEN_WIDTH),
    soundTimerRateMS(SOUND_TIMER_RATE_MS),
    stepsPerCycle(STEPS_PER_CYCLE),
    sourceFileName(""),
    outputFileName("output.gif"),
    myChip8(myChip8)
{
}

bool Application::init(QApplication &app)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(DESCRIPTION);
    parser.addHelpOption();
    parser.addVersionOption();    
    parser.addPositionalArgument("source", "Chip-8 source file.");
    parser.addPositionalArgument("output", "GIF output file (optional).");

    QCommandLineOption blockSizeOption(
        QStringList() << "b" << "blocksize",
            "Sets block height and width to <size>.", "size");
    parser.addOption(blockSizeOption);

    QCommandLineOption pixelColorOption(
        QStringList() << "p" << "pixelcolor",
            "Sets pixel color to <color>.", "color");
    parser.addOption(pixelColorOption);

    QCommandLineOption cycleRateOption(
        QStringList() << "cycle",
            "Sets cycle rate to <ms>.", "ms");
    parser.addOption(cycleRateOption);

    QCommandLineOption refreshRateOption(
        QStringList() << "refresh",
            "Sets refresh rate to <ms>.", "ms");
    parser.addOption(refreshRateOption);

    QCommandLineOption stepsPerCycleOption(
        QStringList() << "steps",
            "Sets steps per cycle to <steps>.", "steps");
    parser.addOption(stepsPerCycleOption);

    QCommandLineOption debuggingOption(
        QStringList() << "d" << "debugging",
            "Toggles debugging (single-step) mode.");
    parser.addOption(debuggingOption);

    QCommandLineOption forceOption(
        QStringList() << "f" << "force",
        "Overwrites existing files.");
    parser.addOption(forceOption);

    QCommandLineOption altKeysOption(
        QStringList() << "k" << "altkeys",
        "Selects alternate navigation keys.");
    parser.addOption(altKeysOption);

    QCommandLineOption recordOption(
        QStringList() << "r" << "record",
        "Records frames for GIF movie creation.");
    parser.addOption(recordOption);

    QCommandLineOption silenceOption(
        QStringList() << "s" << "silence",
            "Toggles buzzer (silent-running).");
    parser.addOption(silenceOption);

    parser.process(app);
    
    if (parser.isSet(altKeysOption)) {
      altKeys = !altKeys;
      cout << "altKeys == " << altKeys << endl;
    }

    if (parser.isSet(debuggingOption)) {
      debugging = !debugging;
      cout << "debugging == " << debugging << endl;
    }

    if (parser.isSet(silenceOption)) {
      silentRunning = !silentRunning;
      cout << "silentRunning == " << silentRunning << endl;
    }

    if (parser.isSet(blockSizeOption)) {
      int t = parser.value(blockSizeOption).toInt();
      blockSize = t;
      cout << "blockSize == " << blockSize << endl;
    }

    if (parser.isSet(pixelColorOption)) {
      pixelColor = QColor(parser.value(pixelColorOption));
    }

    if (parser.isSet(cycleRateOption)) {
      int t = parser.value(cycleRateOption).toInt();
      cycleRateMS = t;
      cout << "cycleRateMS == " << cycleRateMS << endl;
    }

    if (parser.isSet(refreshRateOption)) {
      int t = parser.value(refreshRateOption).toInt();
      refreshRateMS = t;
      cout << "refreshRateMS == " << refreshRateMS << endl;
    }

    if (parser.isSet(stepsPerCycleOption)) {
      int t = parser.value(stepsPerCycleOption).toInt();
      stepsPerCycle = t;
      cout << "stepsPerCycle == " << stepsPerCycle << endl;
    }

    const QStringList args = parser.positionalArguments();
    if (args.length() >= 1) sourceFileName = args.at(0);
    if (args.length() >= 2) outputFileName = args.at(1);

    cout << endl << TITLE << " by Tom Swan" << endl;

    displayWidth  = ((screenWidth +2)*blockSize);
    displayHeight = ((screenHeight+2)*blockSize);
    
    if (debugging) stepsPerCycle = 1;

    if(!myChip8.loadApplication(sourceFileName.toLatin1()))
        return false;

    return true;
}

void Application::report()
{
    cout << endl << endl;
    cutline(10);
    cout << "DEBUGGING REPORT" << endl;
    cutline(10); cout << endl;
    cout << "pc     : " << setw(4) << setfill('0') << hexs(myChip8.getpc()) << endl;
    cout << "opcode : " << setw(4) << setfill('0') << hexs(myChip8.getopcode()) << endl;
    cout << "I      : " << setw(4) << setfill('0') << hexs(myChip8.getI()) << endl;
    cout << "sp     : " << hexc(myChip8.getsp()) << endl << endl;
    
    for (int i = 0; i < 16; i++) {
        cout << "V" << hexc(i) << ":" << setw(2) << hexc(myChip8.getV(i)) << " ";
        if ( ((i+1)%4) == 0 ) cout << endl;
    }
    cout << endl;

    for (int i = 0; i < 16; i++) {
        cout << "s[" << hexs(i) << "]:" << setw(4) << hexs(myChip8.getstack(i)) << " ";
        if ( ((i+1)%4) == 0 ) cout << endl;
    }
    cout << endl;
}
