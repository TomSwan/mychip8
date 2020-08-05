// util.h - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QTextStream>
#include <iostream> 

[[ noreturn ]] void mFatal(const QString &s);
void cr();
void ph(const QString &s);
void ps(const QString &s);
void pss(const QString &s);
int pfchar(const QString &s);
QString pfline(const QString &s);
bool ok(const QString &s);
void cutline(int count);
bool makeDir(const QString &folder);
bool fileExists(const QString &path);
void deleteFile(const QString &path);
void deleteFolder(const QString &folder);
bool copyFile(const QString &fromPath, const QString &toPath);
int inrange(int n, int low, int high);
unsigned int randi(const unsigned int n);
QString editString(const QString &prompt, const QString &value);
int editInt(const QString &prompt, int value);

struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

struct HexShortStruct
{
  unsigned short s;
  HexShortStruct(unsigned short _s) : s(_s) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline std::ostream& operator<<(std::ostream& o, const HexShortStruct& hs)
{
  return (o << std::hex << (unsigned short)hs.s);
}

inline HexCharStruct hexc(unsigned char _c)
{
  return HexCharStruct(_c);
}

inline HexShortStruct hexs(unsigned short _s)
{
  return HexShortStruct(_s);
}

#endif // UTIL_H
