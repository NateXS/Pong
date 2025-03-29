#include "../../include/renderable/Sprite.h"

Sprite::Sprite(){
    name = "Sprite";
}
Sprite::~Sprite(){
    if(tempSheet != NULL){
        C2D_SpriteSheetFree(tempSheet);
    }
}

string* Sprite::getExtended(){
    string* ext = new string[4];
    ext[0] = "Classified";
    ext[1] = "TreeElement";
    ext[2] = "Renderable";
    return ext;
}

bool Sprite::loadFromSheet(C2D_SpriteSheet sheet, unsigned int index){
    if(index > C2D_SpriteSheetCount(sheet)) return false;
    C2D_SpriteFromSheet(&c2dSprite,sheet,index);
    loaded = true;
    return true;
}

bool Sprite::loadFromSheetFile(char* path, unsigned int index){
    tempSheet = C2D_SpriteSheetLoad(path);
    if(tempSheet == NULL) return false;
    bool res = loadFromSheet(tempSheet, index);
    return res;
}

void Sprite::loadFromImage(C2D_Image image){
    C2D_SpriteFromImage(&c2dSprite,image);
    loaded = true;
}

unsigned int Sprite::getHeight(){
    return c2dSprite.image.tex->height;
}

unsigned int Sprite::getWidth(){
    return c2dSprite.image.tex->width;
}

float Sprite::getRotationDegrees(){
    return rotationRad*360;
}

void Sprite::setRotationDegrees(float degrees){
    if(degrees == 0){ //to prevent zero-division error
        rotationRad = 0;
        return;
    }
    rotationRad = degrees/360;
}

void Sprite::setScale(float x, float y){
    scaleX = x;
    scaleY = y;
}

void Sprite::setCenter(float x, float y){
    C2D_SpriteSetCenter(&c2dSprite,x,y);
}

void Sprite::render(){
    if(!visible) return;
    C2D_SpriteSetPos(&c2dSprite,posX,posY);
    C2D_SpriteSetScale(&c2dSprite,scaleX,scaleY);
    C2D_SpriteSetRotation(&c2dSprite,rotationRad);
    C2D_DrawSprite(&c2dSprite);
}

bool Sprite::isLoaded(){
    return loaded;
}