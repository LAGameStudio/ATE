#include "Box2D.h"

#include "Display.h"
#include "GLWindow.h"


 void DestructionListener::SayGoodbye(b2Joint* joint) {
 	if (world->m_mouseJoint == joint)	world->m_mouseJoint = NULL;
 	else
		world->JointDestroyed(joint);
 }

 