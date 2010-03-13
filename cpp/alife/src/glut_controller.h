#ifndef _GLUT_CONTROLLER_H_
#define _GLUT_CONTROLLER_H_

class World;
class AbstractSimulator;

class GLUTController
{
private:
    World * pWorld;
    AbstractSimulator * pSimulator;
public:
    GLUTController();
    
    void init();
    void setActive();

    /**set world to control*/
    void setWorld( World &w, AbstractSimulator& s);
    
private:
    static GLUTController * pActiveController;
    static void keyboardCallback(unsigned char key, int x, int y);
    static void keyboardSpecialCallback(int key, int x, int y);


protected:
    void onKeyboard(unsigned char key, int x, int y);
    void onKeyboardSpecial(int key, int x, int y);

    void deactivated();
};

#endif /* _GLUT_CONTROLLER_H_ */
