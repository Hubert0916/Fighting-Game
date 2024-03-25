#pragma once
#include "Enemy.h"
#include "Image.h"
#include "Mixer.h"
class Kecleon :
    public Enemy
{
    public:
        Kecleon();
        Kecleon(const char*, int, SDL_Renderer*, int, int, int, int);
        void draw();
        void damaged(bool);
        void setHealth(int);
        int getWidth();
        int getHeight();
        void getLinkPos(int, int);
        int getSpeed();
        void starttrace(Uint32);
        void startmotion(Uint32);
        void startinvisible(Uint32);
        void stoptrace();
        void stopmotion();
        void stopinvisible();
        int getAlpha();
        static Uint32 RoutineMotion(Uint32, void*);
        static Uint32 traceLink(Uint32, void*);
        static Uint32 invisible(Uint32, void*);
        void close();

    private:
        int num;
        int frame;
        int link_x;
        int link_y;
        bool touchLink;
        Uint32 time_motion;
        Uint32 time_trace;
        Uint32 time_invisible;
        Image* image;
        Mixer invision;
        Mixer cry;
};

