#include "glut_controller.h"
#include <GL/glut.h>
#include <cassert>
#include <stdlib.h>
#include "world.h"
#include "abstract_simulator.h"

GLUTController *GLUTController::pActiveController = NULL;


GLUTController::GLUTController()
{
    pWorld = NULL;
    pSimulator = NULL;
}
void GLUTController::init()
{
    glutKeyboardFunc( &keyboardCallback );
    glutSpecialFunc( &keyboardSpecialCallback );
}

void GLUTController::setActive()
{
    if (pActiveController){
	pActiveController->deactivated();
    }else{
	init();
    }
    pActiveController = this;
}

void GLUTController::deactivated()
{
    assert( pActiveController == this );
    
}

void GLUTController::keyboardCallback(unsigned char key, int x, int y)
{
    if (pActiveController){
	pActiveController->onKeyboard( key, x, y);
    }
}
void GLUTController::keyboardSpecialCallback(int key, int x, int y)
{
    if (pActiveController){
	pActiveController->onKeyboardSpecial( key, x, y);
    }
}
void GLUTController::onKeyboardSpecial(int key, int x, int y)
{
    assert( pWorld );
    assert( pSimulator );
    switch( key ){
	
    };
}

void GLUTController::onKeyboard(unsigned char key, int x, int y)
{
    assert( pWorld );
    assert( pSimulator );

    switch( key ){
	case '1':
	    pSimulator->setDelay( 1 );
	    break;
	case '2':
	    pSimulator->setDelay( 2 );
	    break;
	case '3':
	    pSimulator->setDelay( 5 );
	    break;
	case '4':
	    pSimulator->setDelay( 10 );
	    break;
	case '5':
	    pSimulator->setDelay( 100 );
	    break;
	case '0':
	    pSimulator->setDelay( 0 );
	    break;
    };
}



void GLUTController::setWorld( World &w, AbstractSimulator& s)
{
    pWorld = &w;
    pSimulator = &s;
}
