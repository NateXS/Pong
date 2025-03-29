#include "../include/Classified.h"

string* Classified::getExtended(){
    string* ext = new string[0];
    return ext;
}

string Classified::getType(){
    return "Classified";
}

bool instanceof(Classified *classified, string className){
    if(classified->getType() == className) return true;
    string* ext = classified->getExtended();
    for(size_t i=0; i<ext->size(); i++){
        if(ext[i] == className) return true;
    }
    return false;
}