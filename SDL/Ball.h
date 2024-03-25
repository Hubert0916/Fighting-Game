#pragma once
#include "Enemy.h"
#include "Red.h"
#include "Mixer.h"
class Ball :
    public Enemy
{
    public:
        Ball();
        Ball(SDL_Renderer *, int, int, int, int, int, int, char);
        void setSpeedXY(int, int);
        void Colide(Ball &);
        void Colide(Red &);
        void draw();
        void setColor(int, int, int);
        void setMass(int);
        void setRadius(int);
        void damaged(bool);
        int getHealth();
        void close();
        int xv;
        int yv;

    protected:
        int mass;
        int r;
        int g;
        int b;
        Mixer effect;
        Mixer bonk;
};

