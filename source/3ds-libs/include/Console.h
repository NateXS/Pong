#ifndef CONSOLE_H
#define CONSOLE_H

#pragma once
#include <3ds.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include "renderable/Renderable.h"
#include "TreeElement.h"

using std::string;
using std::cout;

enum Color {
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white,
};

enum Ansi_code {
    reset = 0,
    bold = 1,
    italic = 3,
    underline = 4,
    strikethrough = 9,
};

enum Wrap_mode {
    trim_characters,
    trim_words,
    wrap_characters,
    wrap_words,
};

class Console : public Renderable, public TreeElement {
private:
    vector<string> output;
    PrintConsole printConsole;
    bool initialized = false;
public:
    gfxScreen_t screen;

    static const int FG_POS = 30;
    static const int BG_POS = 40;
    

    Console(gfxScreen_t screen_);
    ~Console();

    virtual string* getExtended();
    virtual string getType(){ return "Console"; }

    /** Prints line of text.
    * @param text The text you want to print. ANSI codes are supported.
    */
    void print(string text);
    /** Clears the output */
    void clear();
    /** Renders the console. Must be called in a loop. */
    void render();
    /** 
    * Sets the console font 
    * @param font The font you want to use.
    */
    void setFont(ConsoleFont font);
    /** Returns the total character rows. */
    int getRows();
    /** Returns the total character columns. */
    int getColumns();
    /** 
     * Returns an ANSI code that moves text to the specified row and column. 
     * @param row Row from 0 to 30.
     * @param columns Column from 0 to 50 on the top screen and 40 on the bottom.
     */
    
    static string pos(int row, int column = 0);
    /** 
    * Returns an ANSI code that sets the foreground color of text.
    * @param color The color you want to use.
    */
    static string fg(Color color);
    /** 
    * Returns an ANSI code that sets the background color of text.
    * @param color The color you want to use.
    */
    static string bg(Color color);
    /** 
    * Returns an ANSI code that can format text in many ways.
    * @param type The formatting option you want to use.
    */
    static string ansi(Ansi_code type);
};

#endif