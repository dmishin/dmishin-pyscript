#pragma once
#ifndef __GLUT_VIEWPORT_INCLUDED__
#define __GLUT_VIEWPORT_INCLUDED__

#include <vector>
#include "vec2.h"

#include "world.h"

class Located;
class Oriented;
class Mobile;

class GlutGuiViewport
{
private:
    World& world;
    vec2 center;
    ftype zoom; //how much screen pixels in one virtual unit.

    vec2 ptTopLeft, ptBottomRight;//observed area of the grid

    static GlutGuiViewport * spActiveViewport;

//buffer, used for getting information about observed mobiles
    World::MobilesSnapshot displayedMobiles;
    World::FoodSnapshot displayedFood;

    void drawMobile( Mobile &item);
    void drawFood( Located &item);


public:

    GlutGuiViewport(World &w, const vec2& center, ftype zoom);
		
    void draw();//called by the GLUT callback
    void reshape( int w, int h);
    
    void setActive();

    static GlutGuiViewport* getActiveViewport(){ return spActiveViewport; };
    static void init( int argc, char* argv[]);
    static void startLoop();

    ~GlutGuiViewport();
private:
    /**draw one icon, at the center*/
    void drawIcon( Mobile& mob);
    void drawFoodIcon();


    /**called, when viewport become unactive*/
    void deactivate();
};

#endif
