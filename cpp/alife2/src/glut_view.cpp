#include "glut_view.hpp"

using namespace alife2;

GlutView::GlutView()
{
    world = NULL;
    zoom = 1;
    viewPoint = vec2( 0, 0);
}
GlutView:: ~GLutView()
{
    if ( isActive() ){
	deactivate();
    };
}
	    
	//Set the World reference to objserve
void Glutview::setWorld( World * pWorld )
{
    world = pWorld;
}
//Set this view as active or not. Only one GLUT view can be active at time
void  GlutView::activate()
{
    if ( static_ActiveView ){
	if ( static_ActiveView == this )
	    return;//nothing to activate
	else{
	    static_ActiveView->deactivate();
	}
    }
    static_ActiveView = this;
}
void GlutView::deactivate()
{
    if ( static_ActiveView ){
	if (static_ActiveView == this ){
	    //OK, active
	    static_ActiveView = NULL;
	}
    }
    assert( false );//not active
}
void GlutView::setViewPoint( const vec2& p )
{
    viewPoint = p;
}

void GlutView::setZoom( float z )
{
    zoom = z;
}

bool GlutView::isActive()const
{
    return (static_ActiveView == this );
}

void GlutView::runLoop()
{
    //TODO
}
