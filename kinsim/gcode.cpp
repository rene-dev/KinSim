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

using namespace std;

namespace g{
    enum{G,code,axis,posx,posy,posz,posa,posb,posc}next;
    float xstack,ystack;
}

bool parse(string w){
    cout << "p ";
    if (g::next == g::code) {
       cout << "G:" << stoi(w) << endl;
    }
    if (g::next == g::posx) {
        cout << "X:" << stof(w) << endl;
    }
    if (g::next == g::posy) {
        cout << "Y:" << stof(w) << endl;
    }
    if (g::next == g::posz) {
        cout << "Z:" << stof(w) << endl;
    }
    if (w == "\n") {
        cout << "go" << endl;
    }
    return true;
}

int gcode()
{
    FILE *f = fopen("file", "r");
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

