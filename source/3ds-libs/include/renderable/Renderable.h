#ifndef RENDERABLE_H
#define RENDERABLE_H

#pragma once
#include <3ds.h>

class Renderable {
    public:
        Renderable() = default;
        virtual ~Renderable() = default;

        bool visible = true;

        /** Renders the renderable object. */
        virtual void render() = 0;
};

#endif