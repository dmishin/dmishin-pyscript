#include <GL/glut.h>
#include "glut_view.hpp"
#include "item_enumerator.hpp"
#include <assert.h>
#include "world.hpp"
#include "mobile.hpp"
#include <math.h>
#include <iostream>

using namespace alife2;

alife2::GlutView * alife2::GlutView::static_ActiveView = NULL;

GlutView::GlutView()
{
    world = NULL;
    zoom = 8;
    viewPoint = vec2( 0, 0);
    width = 0;
    height = 0;
}
GlutView:: ~GlutView()
{
    if ( isActive() ){
	deactivate();
    };
}
	    
//Set the World reference to objserve
void GlutView::setWorld( World * pWorld )
{
    world = pWorld;
    if ( world )
	setViewPoint( world->getCenter() );
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
    char * argv[] = {};
    int argc = 0;
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE );
    glutCreateWindow( "UI with glut" );
    glutDisplayFunc( glut_display );
    // here is the setting of the idle function
    //glutIdleFunc( glut_display );
    glutReshapeFunc( glut_resize );

    glutMainLoop();
}

//interaction with GLUT
void GlutView::resize( int w, int h )
{
    std::cout<<"resize:"<<w<<h<<std::endl;
    std::cout.flush();

    width = w;
    height = h;

    //calculate viewed area size
    vec2 ptTopLeft = view2world( vec2(0,0) );
    vec2 ptBottomRight = view2world( vec2( width, height ) );

    //set up viewport
    glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */

    glMatrixMode(GL_PROJECTION);  /* Start modifying the projection matrix. */
    glLoadIdentity();             /* Reset project matrix. */

    glOrtho(ptTopLeft.x, ptBottomRight.x, ptTopLeft.y, ptBottomRight.y, -1, 1);   /* Map abstract grid coords to window coords. */

}
void GlutView::display()
{
    assert( isActive() );
    if ( ! world )
	return;// world not set
    //request the list of the bots in the viewable area
    rectangle viewRect( view2world( vec2( 0, 0 ) ),
			view2world( vec2( width, height ) ) );
    
    //TODO: avoid creation of the buffers
    ItemCollector::Items mobiles;
    ItemCollector collector( mobiles );

    world->gridMobiles.enumerateInRectangle( viewRect, collector );
    std::cout<<"Draw "<<mobiles.size()<<" mobiles"<<std::endl;
    std::cout.flush();

    for (ItemCollector::Items::iterator i = mobiles.begin(); i != mobiles.end(); ++i){
	drawMobile( * static_cast<Mobile *>(*i) );
    }
}

//called to draw one item
void GlutView::drawMobile( Mobile& item)
{
    const vec2 &pos = item.getLocation();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0);
    glRotatef( item.getAngle().angle()*(180/M_PI), 0, 0, 1 );//TODO: not really effective
    drawMobileIcon( item );
    glPopMatrix();
}
void GlutView::drawMobileIcon( Mobile &mob )
{
    float  iconSize = 1.0;

    glBegin(GL_POLYGON);
    glColor3f( 1, 1, 1);
    
    glVertex2f( 0, -iconSize);
    glVertex2f( 0.7*iconSize, -0.7*iconSize);
    glVertex2f( iconSize, 0);
    glVertex2f( 0.7*iconSize, 0.7*iconSize);
    glVertex2f( 0, iconSize);
    glVertex2f( -0.7*iconSize, 0.7*iconSize);
    glVertex2f( -iconSize, 0);
    glVertex2f( -0.7*iconSize, -0.7*iconSize);

    glEnd();
 
    glBegin( GL_LINES );
    glColor3f( 1, 0, 0);
    glVertex2f( 0,0);
    glVertex2f( 0, iconSize*2);
    glEnd();

    //draw motors
    /*
    glBegin( GL_LINES );
    
    for( int i = 0; i < Mobile::NUM_MOTORS; ++i){
	const Motor& m = mob.getMotor( i );
	const vec2& pos = m.getPos();
	vec2 posEnd = pos + m.getForce();
	glColor3f( 1, 1, 1);
	glVertex2f( pos.x, pos.y);
	glColor3f( 0, 1, 0);
	glVertex2f( posEnd.x, posEnd.y );
    }
    glEnd();
    */
}
void GlutView::keyPressed( unsigned char key, int x, int y )
{
}


/////////////////// coord transforms ////////////////
vec2 GlutView::view2world( const vec2& p )
{
    return ( p - vec2( width/2, height/2 ) ) / zoom + viewPoint;
}
vec2 GlutView::world2view( const vec2& p )
{
    return ( p - viewPoint ) * zoom + vec2( width/2, height/2 );
}



///////////////// GLUT callbacks ///////////////////
//GLUT callback functions
void GlutView::glut_resize( int w, int h)
{
//    std::cout<<"g-resize\n";
//    std::cout.flush();
    if ( static_ActiveView ){
	static_ActiveView->resize( w, h );
    };
}
void GlutView::glut_display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (static_ActiveView ){
	static_ActiveView->display();
    }
    glutSwapBuffers();/*in double-buffered mode, swapping*/
}
void GlutView::glut_keyboard(unsigned char key, int x, int y)
{
    if ( static_ActiveView ){
	static_ActiveView->keyPressed( key, x, y );
    };
}
