#include "../include/Console.h"

gfxScreen_t screen;
Console::Console(gfxScreen_t screen_){
    screen = screen_;
}

Console::~Console(){
    clear();
}

string* Console::getExtended(){
    string* ext = new string[4];
    ext[0] = "Classified";
    ext[1] = "TreeElement";
    ext[2] = "Renderable";
    return ext;
}

string Console::pos(int row, int column){
    return "\x1b[" + to_string(row) + ";" + to_string(column) + "H";
}

string Console::fg(Color color){
    return "\x1b["+ to_string( FG_POS+(int)color)+"m";
}

string Console::bg(Color color){
    return "\x1b["+ to_string( BG_POS+(int)color)+"m";
}

string Console::ansi(Ansi_code type){
    return "\x1b["+ to_string(type)+"m";
}

void Console::print(string text){
    if((int)output.size() > 30){
        output.erase(output.begin());
    }
    output.push_back(text+ansi(reset));
}

void Console::clear(){
    output.clear();
}

void Console::setFont(ConsoleFont font){
    consoleSetFont(&printConsole, &font);
}

int Console::getRows(){
    return 30;
}

int Console::getColumns(){
    if(screen == GFX_TOP) return 50;
    if(screen == GFX_BOTTOM) return 40;
    return 0;
}

void Console::render(){
    if(!visible) return;
    
    consoleSelect(&printConsole);
    if(!initialized){
        initialized = true;
        consoleInit(screen, &printConsole); //this has to be called once. Also, do not put it in the constructor or Scene will not render.
    }
    //consoleClear();
    
    for(int i = 0; i<(int)output.size(); i++){
        int topOffset = i+1;
        cout << pos(topOffset) << output[i];
    }
}