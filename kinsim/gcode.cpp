//
//  main.cpp
//  parse
//
//  Created by Rene on 12/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

//TODO no newline at end, leerzeilen
//idee: gcode in path, zeile in vec, vorschau der aktuellen zeile

#include <iostream>
#include <string>
#include <ctype.h>
#include "gcode.h"

using namespace std;

namespace g{
    enum{G,code,posx,posy,posz,posa,posb,posc}next;
}

struct path* gcode(const char *filename){
    FILE *f = fopen(filename, "r");
    char c;
    string word = "";
    struct path* result = 0;
    struct vec newvec;
    float x = 0,y = 0,z = 0;
    
    g::next = g::G;
    
    while (!feof(f)) {
        c = fgetc(f);

        switch (g::next) {
            case g::code:
                if(isdigit(c)){
                    word.append(&c,1);
                }else{
                    g::next = g::G;
                }
                break;
            case g::posx:
                if(isdigit(c) || c == '.' || c == '-'){
                    word.append(&c,1);
                }else{
                    x = stof(word);
                    g::next = g::G;
                }
                break;
            case g::posy:
                if(isdigit(c) || c == '.' || c == '-'){
                    word.append(&c,1);
                }else{
                    y = stof(word);
                    g::next = g::G;
                }
                break;
            case g::posz:
                if(isdigit(c) || c == '.' || c == '-'){
                    word.append(&c,1);
                }else{
                    z = stof(word);
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
                newvec.type = vec::axis;
                newvec.axis_pos[0] = x;
                newvec.axis_pos[1] = y;
                newvec.axis_pos[2] = z;
                append(&result, newvec);
                cout << "goto:" << x << " " << y << " " << z << endl;
                break;
            default:
                break;
        }
    }
    return result;
}

