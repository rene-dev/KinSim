//
//  main.cpp
//  parse
//
//  Created by Rene on 12/11/13.
//  Copyright (c) 2013 Rene. All rights reserved.
//

//TODO leerzeilen
//idee: gcode in path, zeile in vec, vorschau der aktuellen zeile

#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>
#include <cstdlib>
#include "gcode.h"

using namespace std;

namespace g{
    enum{any,code,pos}next;
    enum{x,y,z}axis;
}

path* gcode(const char *filename){
    FILE *f = fopen(filename, "r");
    char c;
    string word;
    path* result = 0;
    vec newvec;
    float x = 0,y = 0,z = 0;
    bool linedone = true;

    word.clear();
    g::next = g::any;

    while (!feof(f)) {
        c = fgetc(f);
        switch (g::next) {
            case g::code:
                if(isdigit(c)){
                    word.append(&c,1);
                }else{
                    g::next = g::any;
                }
                break;
            case g::pos:
                if(isdigit(c) || c == '.' || c == '-'){
                    word.append(&c,1);
                }else{
                    switch (g::axis) {
                        case g::x:
                            x = strtof(word.c_str(), NULL);
                            break;
                        case g::y:
                            y = strtof(word.c_str(), NULL);
                            break;
                        case g::z:
                            z = strtof(word.c_str(), NULL);
                            break;
                        default:
                            break;
                    }
                    g::next = g::any;
                }
                break;
            default:
                break;
        }

        switch (c) {
            case 'g':
                g::next = g::code;
                word.clear();
                linedone = false;
                break;
            case 'x' :
                g::next = g::pos;
                g::axis = g::x;
                word.clear();
                linedone = false;
                break;
            case 'y' :
                g::next = g::pos;
                g::axis = g::y;
                word.clear();
                linedone = false;
                break;
            case 'z' :
                g::next = g::pos;
                g::axis = g::z;
                word.clear();
                linedone = false;
                break;
            case '\n' :
                if(!linedone){
                    newvec.type = vec::axis;
                    newvec.axis_pos[0] = x;
                    newvec.axis_pos[1] = y;
                    newvec.axis_pos[2] = z;
                    append(&result, newvec);
                    word.clear();
                    linedone = true;
                    cout << "goto:" << x << " " << y << " " << z << endl;
                }
                break;
            default:
                break;
        }
    }

    if(word.length() > 0){
        newvec.type = vec::axis;
        newvec.axis_pos[0] = x;
        newvec.axis_pos[1] = y;
        newvec.axis_pos[2] = z;
        append(&result, newvec);
        word.clear();
        cout << "goto:" << x << " " << y << " " << z << endl;
    }
    return result;
}
