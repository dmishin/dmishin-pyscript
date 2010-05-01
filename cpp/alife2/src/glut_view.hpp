#pragma once
#ifndef _GLUT_VIEW_H_
#define _GLUT_VIEW_H_

#include "point.hpp"

/** This class serves as the GLUT-based observer for the World instance
 */

namespace alife2{
    class World;
    class Mobile;

    class GlutView{
    private:
	World * world;
	vec2 viewPoint;
	float zoom;
	int width, height; //windoww width and height

	Mobile * selectedMob;

	void resize( int w, int h );
	void display();
	void keyPressed( unsigned char key, int x, int y );
	void mousePressed( int button, int state, int x, int y );
	vec2 view2world( const vec2& p );
	vec2 world2view( const vec2& p );
    public:
	GlutView();
	virtual ~GlutView();
	    
	//Set the World reference to objserve
	void setWorld( World * pWorld );
	const World * getWorld()const {return world; };

	//Set this view as active or not. Only one GLUT view can be active at time
	void activate();
	void deactivate();//When already nto active - ewrurns an error
	bool isActive()const;

	void setViewPoint( const vec2& p );
	vec2 getViewPoint()const { return viewPoint; };
	void setZoom( float z );
	float getZoom() const { return zoom; };

	void runLoop();


	//controlling
	void setSelectedMobile( Mobile * mob );
	
    private:
	//GLUT callback functions
	static void glut_resize( int w, int h);
	static void glut_display();
	static void glut_keyboard(unsigned char key, int x, int y);
	static void glut_mouse( int button, int state, int x, int y );

	//Display single mobile
	void drawMobile( Mobile &mob );
	void drawMobileIcon( Mobile &mob );
	void drawSelectionIcon();
    private:
	static GlutView * static_ActiveView;
	
    };
};

#endif /* _GLUT_VIEW_H_ */
