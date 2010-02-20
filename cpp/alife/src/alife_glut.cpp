//alife code, relaed to the GLUT gui
#include <GL/glut.h>
#include "alife_glut.h"
#include "mobile.h"
#include "item-acceptor.h"
#include "grid.h"

class GlutGuiViewport: public ItemAcceptor
{
private:
    Grid& grid;
    vec2 center;
    ftype zoom; //how much screen pixels in one virtual unit.

    vec2 ptTopLeft, ptBottomRight;//observed area of the grid
	
public:
    virtual void put( Located& item);
    GlutGuiViewport(Grid &g, const vec2& center, ftype zoom);
		
    void draw();//called by the GLUT callback
    void reshape( int w, int h);
private:
    /**draw one icon, at the center*/
    void drawIcon();
};

GlutGuiViewport *currentGlutViewport = 0;


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
//GLUT callback
void reshape( int w ,int h)
{
    if (currentGlutViewport)
	currentGlutViewport->reshape(w,h);
}

//GLUT callback
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (currentGlutViewport)
	currentGlutViewport->draw();;
    glFlush();  /* Single buffered, so needs a flush. */
}


GlutGuiViewport::GlutGuiViewport(Grid &g, const vec2& c, ftype z):
    grid(g),
    center(c),
    zoom(z)
{
}

//called to draw one item
void GlutGuiViewport::put( Located& item)
{
    //drawing a triangle
    const vec2 &pos = item.getPos();
    Mobile & mob( static_cast<Mobile&>(item));

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
	
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(mob.getAngle()*(180/M_PI), 0,0,1);
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
    //grid.itemsInRect( ptTopLeft.x, ptTopLeft.y, ptBottomRight.x, ptBottomRight.y, *this);
    //using generators
    /*
      Grid::rectangle_generator gen(grid, ptTopLeft, ptBottomRight);
      do{
      Located* l = gen();
      if (! gen.stopped() )
      put( *l );
		
      }while (! gen.stopped() );
    */
/*
    Grid::rectangle_iterator end, itr(grid, ptTopLeft, ptBottomRight);
    while (itr != end){
	put ( **itr );
	++itr;
    };
*/

  Grid::rectangle_generator gen1( grid, ptTopLeft, ptBottomRight);
  Grid::rectangle_generator gen;
  gen = Grid::rectangle_generator( grid, ptTopLeft, ptBottomRight);
  for( Located* mob; gen( mob ); ){
      put( static_cast<Mobile&>(*mob) );
  }


    /*
      Grid::rectangle_iterator e, i=grid.genItemsInRect(ptTopLeft, ptBottomRight);
      for (; i!=e; ++i){
      std::cout<<"printed"<<(int)*i<<std::endl;
      //put( **i );
      }
    */
}


void initialize( int argc, char* argv[], Grid &grid)
{
    currentGlutViewport = new GlutGuiViewport( grid, grid.center(), 30 );//30 pixels in one unit
    glutInit(&argc, argv);
    glutCreateWindow("UI with glut");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
}

void start_glut_loop()
{
    glutMainLoop();
}
