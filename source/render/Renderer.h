#pragma once

#include "Engine.h"

class Renderer
{
public:
    Renderer(Engine *engine);
    ~Renderer();

    void NewFrame();
    void EndFrame();

    // TODO: add functions to add stuff to rendering queue

private:
    Engine *p_Engine;
};