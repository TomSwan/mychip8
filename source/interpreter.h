// interpreter.h - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
    public:
        Interpreter();
        ~Interpreter();
        
        unsigned char  delay_timer;
        unsigned char  sound_timer;
        
        unsigned char  gfx[64 * 32];
        unsigned char  key[16];

        void emulateCycle();
        bool loadApplication(const char * filename);

        unsigned short getpc() const;
        unsigned short getopcode() const;
        unsigned short getI() const;
        unsigned short getsp() const;
        unsigned char getV(int i) const;
        unsigned short getstack(int i) const;
        unsigned char getmemory(int i) const;

    private:    
        unsigned short pc;
        unsigned short opcode;
        unsigned short I;
        unsigned short sp;
        
        unsigned char  V[16];
        unsigned short stack[16];
        unsigned char  memory[4096];    
                
        void init();
};

#endif // INTERPRETER_H

// Original file header
///////////////////////////////////////////////////////////////////////////////
// Project description
// ???????????????????
// Name: myChip8
//
// Author: Laurence Muller
// Contact: laurence.muller@gmail.com
//
// License: GNU General Public License (GPL) v2 
// ( http://www.gnu.org/licenses/old-licenses/gpl-2.0.html )
//
// Copyright (C) 2011 Laurence Muller / www.multigesture.net
///////////////////////////////////////////////////////////////////////////////
