#include "glut_viewport.h"

#include <GL/glut.h>
//for cerr
#include <iostream>

#include "alife_glut.h"
#include "mobile.h"
#include "grid.h"

GlutGuiViewport * GlutGuiViewport::spActiveViewport = NULL;

//callbacks
void GLUT_reshape( int w ,int h);
void GLUT_display();

void GlutGuiViewport::reshape( int w, int h)
{
    //calculate viewed area size
    ptTopLeft = center - vec2(w/zoom, h/zoom)*0.5;
    ptBottomRight = center + vec2(w/zoom, h/zoom)*0.5;

    //set up viewport
    glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */

    glMatrixMode(GL_PROJECTION);  /* Start modifying the projection matrix. */
    glLoadIdentity();             /* Reset project matrix. */

    glOrtho(ptTopLeft.x, ptBottomRight.x, ptTopLeft.y, ptBottomRight.y, -1, 1);   /* Map abstract grid coords to window coords. */

}

GlutGuiViewport::GlutGuiViewport(World &w, const vec2& c, ftype z):
    world(w),
    center(c),
    zoom(z)
{
}

void GlutGuiViewport::drawFood( Located& item )
{
    //drawing a triangle
    const vec2 &pos = item.getPos();
    

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	
    glTranslatef(pos.x, pos.y, 0);
//    glRotatef(item.getAngle()*(180/M_PI), 0,0,1);
    drawFoodIcon();
	
    glPopMatrix();
}
//called to draw one item
void GlutGuiViewport::drawMobile( Mobile& item)
{
    //drawing a triangle
    const vec2 &pos = item.getPos();
    

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
	
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(item.getAngle()*(180/M_PI), 0,0,1);
    drawIcon( item );
	
    glPopMatrix();
}
void GlutGuiViewport::drawFoodIcon()
{
   ftype iconSize = 0.1;

    glBegin(GL_TRIANGLES);

    glColor3f(0.0, 1.0, 0.0);  /* green */

    glVertex3f( 0, -iconSize, 0);
    glVertex3f( iconSize, 0, 0);
    glVertex3f( 0, iconSize, 0);

    glVertex3f( 0, -iconSize, 0);
    glVertex3f( -iconSize, 0, 0);
    glVertex3f( 0, iconSize, 0);

    glEnd();
}
void GlutGuiViewport::drawIcon(Mobile& mob)
{
    ftype iconSize = 1.0;

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
    glBegin( GL_LINES );
    
    for( int i = 0; i < Mobile::NUM_MOTORS; ++i){
	const Motor& m = mob.getMotor( i );
	const vec2& pos = m.getPos();
	vec2 posEnd = pos-m.getForce();
	glColor3f( 1, 1, 1);
	glVertex2f( pos.x, pos.y);
	glColor3f( 0, 1, 0);
	glVertex2f( posEnd.x, posEnd.y );
    }
    glEnd();
}

void GlutGuiViewport:: draw()
{
//get the complete information about all idsplayed mobiles first
    world.getMobilesSnapshot( ptTopLeft, ptBottomRight, displayedMobiles);
//then draw them
//Such approach is used to make parallel execution more effective (drawing is ecpected to be much slower than getting snapshot)
    if(true){
	World::MobilesSnapshot::iterator i, e=displayedMobiles.end();
	for( i = displayedMobiles.begin(); i!=e; ++i){
	    drawMobile( **i );
	}
    }
    if(true){
	world.getFoodSnapshot( ptTopLeft, ptBottomRight, displayedFood);
	World::FoodSnapshot::iterator i, e = displayedFood.end();
	for( i = displayedFood.begin(); i!=e; ++i){
	    drawFood( *i );
	}
    }
}

/**Called to set this viewport active*/
void GlutGuiViewport::setActive()
{
    if (spActiveViewport){
	spActiveViewport->deactivate();
    }
    spActiveViewport = this;
}
GlutGuiViewport::~GlutGuiViewport()
{
    if( spActiveViewport == this)
	deactivate();
}

void GlutGuiViewport::deactivate()
{
//Doing nothing special
    if (spActiveViewport==this)
	spActiveViewport = NULL;
    else{
	std::cerr<<"Warning: tried to deactivate non-active viewport\n";
    }
}

void GlutGuiViewport::init( int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE );
    glutCreateWindow("UI with glut");
    glutDisplayFunc(GLUT_display);
    // here is the setting of the idle function
    glutIdleFunc(GLUT_display);
	
    glutReshapeFunc(GLUT_reshape);
}


void GlutGuiViewport::startLoop()
{
    if (! getActiveViewport() ){
	std::cerr<<"Warning: started GLUUT loop without active viewport set. Windows will be empty.\n";
    }
    glutMainLoop();
}


/////////////////////// GLUT callbacks /////////////////
//GLUT callback
void GLUT_reshape( int w ,int h)
{
    if (GlutGuiViewport::getActiveViewport())
	GlutGuiViewport::getActiveViewport()->reshape(w,h);
}

//GLUT callback
void GLUT_display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (GlutGuiViewport::getActiveViewport())
	GlutGuiViewport::getActiveViewport()->draw();

//    glFlush();  /* Single buffered, so needs a flush. */
    glutSwapBuffers();/*in double-buffered mode, swapping*/
}
