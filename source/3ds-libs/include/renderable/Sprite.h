#ifndef SPRITE_H
#define SPRITE_H

#pragma once
#include "Renderable.h"
#include "../TreeElement.h"
#include <citro2d.h>

class Sprite : public Renderable, public TreeElement {
    public:
        Sprite();
        virtual ~Sprite();

        C2D_Sprite c2dSprite;

        float rotationRad = 0;

        int posX = 0;
        int posY = 0;

        float scaleX = 1;
        float scaleY = 1;

        /** Returns the type name of the TreeElement. */
        virtual string getType(){return "Sprite";}

        virtual string* getExtended();

        /**
         * Converts the specified degrees into radians then sets them.
         * @param degrees Rotation degrees.
        */
        void setRotationDegrees(float degrees);
        /** Returns the converted Sprite's rotation in radians into degrees. */
        float getRotationDegrees();
        /** Returns the width in pixels. */
        unsigned int getWidth();
        /** Returns the height in pixels. */
        unsigned int getHeight();
        /**
         * Calculates the X scale to match the specified pixel width
         * @param width Width in pixels.
        */
        void setWidth(unsigned int width);
        /**
         * Calculates the Y scale to match the specified pixel height and sets it.
         * @param height Height in pixels.
        */
        void setHeight(unsigned int height);
        /**
         * Calculates the X and Y scale from the specified pixel with and height and sets it.
         * @param width Width in pixels.
         * @param height Height in pixels.
        */
        void setSize(unsigned int width, unsigned int height);
        /**
         * Set X and Y scale of the sprite at the same time.
         * @param x Width scale percent.
         * @param y Height scale percent.
        */
        void setScale(float x, float y);
        /**
         * Sets the center of the sprite.
         * @param x Center scale width.
         * @param y Center scale height.
         * 
        */
        void setCenter(float x=0.5, float y=0.5);
        /**
         * Loads a sprite from a sprite sheet.
         * @param sheet The sprite sheet.
         * @param index Index of the sprite in sheet.
        */
        bool loadFromSheet(C2D_SpriteSheet sheet, unsigned int index=0);
        /**
         * Loads a sprite from a sprite sheet at the specified file path.
         * @param path File path to the sprite sheet, example 'someSheet.t3x'.
         * @param index Index of the sprite in sheet.
        */
        bool loadFromSheetFile(char* path, unsigned int index=0);
        /**
         * Loads a sprite from a C2D_Image.
         * @param image The image you want to load.
        */
        void loadFromImage(C2D_Image image);
        /** Renders the sprite*/
        virtual void render();
        /** Checks if the sprite texture is loaded */
        bool isLoaded();

    private:
        unsigned int width = 0;
        unsigned int height = 0;
        bool loaded;
        C2D_SpriteSheet tempSheet;

};

#endif