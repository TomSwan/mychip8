// util.cpp - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTextStream>

#include <iostream> 
#include <stdio.h> 
#include <util.h>

QTextStream cout(stdout);

[[ noreturn ]] void mFatal(const QString &s)
{
    cr(); cout << "ERROR: " << s << Qt::endl << Qt::endl;
    exit(1);
}

void cr()
{
    cout << "\n";
}

void ph(const QString &s)
{
    cr();
    cutline(5);
    cout << s << Qt::endl;
    cutline(5);
}

void ps(const QString &s)
{
    cout << "- " << s << Qt::endl;
}

void pss(const QString &s)
{
    cout << s << Qt::endl;
}

int pfchar(const QString &s)
{
    cout << s << "? ";
    system("stty raw");
    int response = getchar(); 
    system("stty cooked");
    cr();
    return response;
}

QString pfline(const QString &s)
{
    cout << s << "? ";
    QTextStream in(stdin);
    return in.readLine();
}

bool ok(const QString &s)
{
    int response = pfchar(s + " (y/n/q)");
    bool yes = ((response == 'y') || (response == 'Y'));
    bool no = ((response == 'n') || (response == 'N') || (response == '\r') );
    if ((yes) || (no)) return yes; else 
    if ( response == 'q' ) exit(0); else
        return false;
}

void cutline(int count)
{
    while ( count-- > 0 )
        cout << "----";
    cr();
}

bool fileExists(const QString &path)
{
    return QFileInfo::exists(path);
}

int inrange(int n, int low, int high)
{
    if (low < 0) low = 0;
    if (high < 0) high = 0;
    if (n < low) return low;
    if (n > high) return high;
    return n;
}

unsigned int randi(const unsigned int n)
{
  return random() % n;
}

QString editString(const QString &prompt, const QString &value)
{
    QString response = pfline(prompt + "(\"" + value + "\")");
    if ( response.length() > 0 )
        return response;
    return value;
}    

int editInt(const QString &prompt, int value)
{
    QString response = pfline(prompt + "(" + QString::number(value) + ")");
    if ( response.length() > 0 )
        return response.toInt();
    return value;
}    
