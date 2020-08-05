// util.cpp - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTextStream>

#include <iostream> 
#include <stdio.h> 
#include <util.h>

using namespace std;  

void cr()
{
    QTextStream(stdout) << "\n";
}

[[ noreturn ]] void mFatal(const QString &s)
{
    cr(); QTextStream(stdout) << "ERROR: " << s << endl << endl;
    exit(1);
}

void ph(const QString &s)
{
    cr();
    cutline(5);
    QTextStream(stdout) << s << endl;
    cutline(5);
}

void ps(const QString &s)
{
    QTextStream(stdout) << "- " << s << endl;
}

void pss(const QString &s)
{
    QTextStream(stdout) << s << endl;
}

int pfchar(const QString &s)
{
    QTextStream(stdout) << s << "? ";
    system("stty raw");
    int response = getchar(); 
    system("stty cooked");
    cr();
    return response;
}

QString pfline(const QString &s)
{
    QTextStream(stdout) << s << "? ";
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
        QTextStream(stdout) << "----";
    cr();
}

bool makeDir(const QString &folder)
{
    if (QFileInfo::exists(folder))
        return true;
    else
        return (QProcess::execute("mkdir " + folder) == 0);
}

bool fileExists(const QString &path)
{
    return QFileInfo::exists(path);
}

void deleteFile(const QString &path)
{
    QFile f(path);
    if (f.exists()) f.remove();
}

void deleteFolder(const QString &folder)
{
    if (fileExists(folder)) {
        QFileInfo info(folder);
        QDir dir(info.absoluteFilePath());
        if ( ok("REMOVE " + dir.path()) )
            dir.removeRecursively();
    }
}

bool copyFile(const QString &fromPath, const QString &toPath)
{
    pss(fromPath + " --> " + toPath);
    return QFile::copy(fromPath, toPath);
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
