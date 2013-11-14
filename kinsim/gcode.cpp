//
//  main.cpp
//  parse
//
//  Created by Rene on 12/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

//TODO no newline at end, leerzeilen

#include <iostream>
#include <string>
#include <ctype.h>
#include "path.h"
#include "gcode.h"

using namespace std;

namespace g{
    enum{G,code,posx,posy,posz,posa,posb,posc}next;
}

bool parse(string w){
    static float xstack = 0,ystack = 0,zstack = 0;
    if (g::next == g::code) {
    }
    if (g::next == g::posx) {
        xstack = stof(w);
    }
    if (g::next == g::posy) {
        ystack = stof(w);
    }
    if (g::next == g::posz) {
        zstack = stof(w);
    }
    if (w == "\n") {
        cout << "goto:" << xstack << " " << ystack << " " << zstack << endl;
    }
    return true;
}

int gcode(const char *filename){
    FILE *f = fopen(filename, "r");
    char c;
    string word = "";
    
    g::next = g::G;
    
    while (!feof(f)) {
        c = fgetc(f);

        switch (g::next) {
            case g::code:
                if(isdigit(c)){
                    word.append(&c,1);
                }else{
                    parse(word);
                    g::next = g::G;
                }
                break;
            case g::posx:
                if(isdigit(c) || c == '.' || c == '-'){
                    word.append(&c,1);
                }else{
                    parse(word);
                    g::next = g::G;
                }
                break;
            case g::posy:
                if(isdigit(c) || c == '.' || c == '-'){
                    word.append(&c,1);
                }else{
                    parse(word);
                    g::next = g::G;
                }
                break;
            case g::posz:
                if(isdigit(c) || c == '.' || c == '-'){
                    word.append(&c,1);
                }else{
                    parse(word);
                    g::next = g::G;
                }
                break;
            default:
                break;
        }
        
        switch (c) {
            case 'g':
                g::next = g::code;
                word = "";
                break;
            case 'x' :
                g::next = g::posx;
                word = "";
                break;
            case 'y' :
                g::next = g::posy;
                word = "";
                break;
            case 'z' :
                g::next = g::posz;
                word = "";
                break;
            case '\n' :
                parse("\n");
                break;
            default:
                break;
        }
    }
    return 0;
}

