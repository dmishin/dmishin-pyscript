#pragma once
#ifndef _GLUT_VIEW_H_
#define _GLUT_VIEW_H_

/** This class serves as the GLUT-based observer for the World instance
 */

namespace alife2{
    class World;


    class GlutView{
    private:
	World * world;
	vec2 viewPoint;
	float zoom;

    public:
	GlutView();
	virtual ~GLutView();
	    
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
	
    private:
	
    private:
	static GlutView * static_ActiveView;
	
    };
};

#endif /* _GLUT_VIEW_H_ */
