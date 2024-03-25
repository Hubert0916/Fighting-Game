#pragma once
#include "Enemy.h"
#include "Image.h"
#include "Mixer.h"

class Green :
    public Enemy
{
    public:
        Green();
        Green(const char*, int ,SDL_Renderer*, int, int ,int, int);
        void draw();
        void damaged(bool);
        void setHealth(int);
        int getWidth();
        int getHeight();
        void getLinkPos(int, int);
        int getSpeed();
        void starttrace(Uint32);
        void startmotion(Uint32);
        void stoptrace();
        void stopmotion();
        static Uint32 RoutineMotion(Uint32, void*);
        static Uint32 traceLink(Uint32 interval, void* param);
        void close();

    private:
        int num;
        int frame;
        int link_x;
        int link_y;
        bool touchLink;
        Uint32 time_motion;
        Uint32 time_trace;
        Image *image;
        Mixer zombie;
        Mixer cry;
};

