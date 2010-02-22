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

//called to draw one item
void GlutGuiViewport::drawMobile( Oriented& item)
{
    //drawing a triangle
    const vec2 &pos = item.getPos();
    

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
	
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(item.getAngle()*(180/M_PI), 0,0,1);
    drawIcon();
	
    glPopMatrix();
}

void GlutGuiViewport::drawIcon()
{
    ftype iconSize = 0.5;

    glBegin(GL_TRIANGLES);

    glColor3f(0.0, 0.0, 1.0);  /* blue */
    glVertex3f( -iconSize, -iconSize, 0);

    glColor3f(0.0, 1.0, 0.0);  /* green */
    glVertex3f( +iconSize, -iconSize, 0);

    glColor3f(1.0, 0.0, 0.0);  /* red */
    glVertex3f( 0  , +iconSize*2, 0);
    glEnd();
}

void GlutGuiViewport:: draw()
{
//get the complete information about all idsplayed mobiles first
    world.getMobilesSnapshot( ptTopLeft, ptBottomRight, displayedMobiles);
//then draw them
//Such approach is used to make parallel execution more effective (drawing is ecpected to be much slower than getting snapshot)
    
    World::MobilesSnapshot::iterator i, e=displayedMobiles.end();
    for( i = displayedMobiles.begin(); i!=e; ++i){
	drawMobile( *i );
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
    glutCreateWindow("UI with glut");
    glutDisplayFunc(GLUT_display);
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

    glFlush();  /* Single buffered, so needs a flush. */
}
