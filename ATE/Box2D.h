/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BOX2D_H
#define BOX2D_H

/**
\mainpage Box2D API Documentation

\section intro_sec Getting Started

For documentation please see http://box2d.org/documentation.html

For discussion please visit http://box2d.org/forum
*/

// These include files constitute the main Box2D API

#include "b2Settings.h"
#include "b2CircleShape.h"
#include "b2PolygonShape.h"

#include "b2BroadPhase.h"
#include "b2Distance.h"
#include "b2DynamicTree.h"
#include "b2TimeOfImpact.h"

#include "b2Body.h"
#include "b2Fixture.h"
#include "b2WorldCallbacks.h"
#include "b2TimeStep.h"
#include "b2World.h"

#include "b2Contact.h"
         
#include "b2DistanceJoint.h"
#include "b2FrictionJoint.h"
#include "b2GearJoint.h"
#include "b2LineJoint.h"
#include "b2MouseJoint.h"
#include "b2PrismaticJoint.h"
#include "b2PulleyJoint.h"
#include "b2RevoluteJoint.h"
#include "b2WeldJoint.h"

#include <stdio.h>
#include "moremath.h"
#include "macros.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Art.h"
#include "TexFont.h"
#include "Vertex.h"

#define k_maxContactPoints 2048

extern Interface input;
extern Display display;
extern Zp<TexFont> microFont;

class Box2dExtents {
public:
 Cartesian camera,moved;
	int width;
	int height;
	int framePeriod;
	int mainWindow;
	float settingsHz;
 GLWindow *attached;
	float zoom;
	Cartesian center,middle;
	b2Vec2 lastp;
 float h, w;
 float u, v, aspect;
 Cartesian lower,upper,extents;
 Cartesian mouse;
 float worldToScreen; 

 Box2dExtents() {
  attached=null;
  zoom=1.0f;
  center.Set(0.0f, 20.0f);
  width=0;
  height=0;
  framePeriod=0;
  mainWindow=0;
  settingsHz=0;
  attached=null;
  zoom=1.0f;
  h=w=0.0f;
  u=v=aspect=0.0f;
  worldToScreen=10.0f;
 }

 void PrintDebugInfo(GLWindow *surface) {
  int mx,my;
  mx=(int) (camera.fx+(mouse.fx)-middle.fx);
  my=(int) (camera.fy-(mouse.fy));
  FORMAT(buf,1024,
   "Camera: (%d,%d):(%f,%f) width/height: %d/%d\nMiddle: (%d,%d):(%f,%f)\nMoved: (%d,%d):(%f,%f)\nZoom: %f\n",
   (float) camera.x, (float) camera.y, (float) camera.fx, (float) camera.fy,
   (int) width, (int) height,
   (int) middle.x, (int) middle.y, (float) middle.fx, (float) middle.fy,
   (int) moved.x, (int) moved.y, (float) moved.fx, (float) moved.fy,
   (float) zoom );
  WriteTexFont(surface,microFont,crayons.Pick(green),additive,8,(int) h-148,7,16,2,buf);
  DrawLine(surface,crayons.Pick(green),64+mx,0,64+mx,(int)h);
  DrawLine(surface,crayons.Pick(green),0,128-(height-my),(int)w,128-(height-my));
 }

 void MoveCameraBy( float x, float y ) {
  int xi=(int)x,yi=(int)y;
  camera.fx+=x;
  camera.fy+=y;
  camera.x +=xi;
  camera.y +=yi;
  moved.fx+=x;
  moved.fy+=y;
  moved.x +=xi;
  moved.y +=yi;
 }

 void MoveCameraBy( int x, int y ) {
  float xf=(float)x,yf=(float)y;
  camera.fx+=xf;
  camera.fy+=yf;
  camera.x +=x;
  camera.y +=y;
  moved.fx+=xf;
  moved.fy+=yf;
  moved.x +=x;
  moved.y +=y;
 }

 void SetCamera( int x, int y ) {
  float fx=(float) x, fy=(float) y;
  camera.fx=fx+middle.fx;
  camera.fy=fy+middle.fy;
  camera.x = x+middle.x;
  camera.y = y+middle.y;
  moved.fx=fx;
  moved.fy=fy;
  moved.x =x;
  moved.y =y;
 }

 void SetCamera( float x, float y ) {
  int xi=(int)x,yi=(int)y;
  camera.fx= x+middle.fx;
  camera.fy= y+middle.fy;
  camera.x =xi+middle.x;
  camera.y =yi+middle.y;
  moved.fx=x;
  moved.fy=y;
  moved.x =xi;
  moved.y =yi;
 }
 
 void ConvertScreenToWorld(int x, int y) {
  mouse.fx=(x+(camera.x)-width)/10.0f*zoom;
  mouse.fy=(-y+(camera.y))/10.0f*zoom;
 }

 void ZoomIn( )  { zoom = min(1.1f * zoom, 20.0f); }
 void ZoomOut( ) {	zoom = max(0.9f * zoom, 0.02f); }
 void Resized(GLWindow *w) {
  Attach(w);
 }
 void Attach(GLWindow *win) {
  attached=win;
  width=win->w;
  height=win->h;
  w=(float) width;
  h=(float) height;
 	aspect = iratiof( attached->w, attached->h );
 	extents.fx=aspect * 25.0f * zoom;
  extents.fy=25.0f * zoom;
 	lower.Set(center.fx - extents.fx, center.fy - extents.fy);
  upper.Set(center.fx + extents.fx, center.fy + extents.fy);
  middle.Set(w/2.0f,h);
  SetCamera(0.0f,0.0f);

	// L/R/B/T
	//gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
 }
};

struct ContactPoint {
public:
	b2Fixture* fixtureA;
	b2Fixture* fixtureB;
	b2Vec2 normal;
	b2Vec2 position;
	b2PointState state;
};

class QueryCallback : public b2QueryCallback {
public:
	b2Vec2 m_point;
	b2Fixture* m_fixture;

 QueryCallback(const b2Vec2& point)	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture)	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)	{
			bool inside = fixture->TestPoint(m_point);
			if (inside)	{
				m_fixture = fixture;
				return false;
			}
		}
		return true;
	}
};

class TouchableBox2dWorld;

class DestructionListener : public b2DestructionListener
{
public:
	void SayGoodbye(b2Fixture* fixture) { B2_NOT_USED(fixture); }
	void SayGoodbye(b2Joint* joint);

	TouchableBox2dWorld* world;
};

enum b2BodyTypes {
 b2Line, b2Triangle, b2Box, b2Circle, b2Poly
};

class b2BodyHandle : public ListItem {
public:
 int index;
 string name;
 b2Body *body;
 b2Fixture *fixture;
 b2BodyHandle( b2Body *b, string n ) : ListItem() {
  body=b;
  index=0;
  name=n;
  fixture=null;
 }
 b2BodyHandle( b2Body *b, string n, int i ) : ListItem() {
  body=b;
  index=i;
  name=n;
  fixture=null;
 }
 b2BodyHandle( b2Body *b, b2Fixture *f, string n ) : ListItem() {
  body=b;
  index=0;
  name=n;
  fixture=f;
 }
 b2BodyHandle( b2Body *b, b2Fixture *f, string n, int i ) : ListItem() {
  body=b;
  index=i;
  name=n;
  fixture=f;
 }
};

class b2BodyList : public LinkedList {
public:
 string name;
 b2BodyHandle *Add( b2Body *b, string n ) {
  b2BodyHandle *bh=new b2BodyHandle(b,n);
  Append(bh);
  return bh;
 }
 b2BodyHandle *Add( b2Body *b, b2Fixture *f, string n ) {
  b2BodyHandle *bh=new b2BodyHandle(b,f,n);
  Append(bh);
  return bh;
 }
 CLEARLISTRESET(b2BodyHandle);
 ~b2BodyList() { Clear(); }
};

class b2BodiesGroup : public ListItem {
public:
 string name;
 matrix1d<b2BodyList> lists;
 unsigned int size;
 b2BodiesGroup() : ListItem() { lists.resize(size=0); name=""; }
 b2BodiesGroup( string n ) : ListItem() { lists.resize(size=0); name=n; }
 b2BodyList *New( string name ) {
  size++;
  lists.resize(size);
  return &(lists(size-1));
 }
};

class b2Chain : public b2BodiesGroup {
public:
	b2Body* head;
 b2Body* tail;
 b2Body* ring;
};

class b2Lever : public b2BodiesGroup {
public:
	b2Body* base;
 b2Body* head;
 b2Joint* armature;
};

class b2GearSet : public b2BodiesGroup {
public:
};

class b2Pulleys : public b2BodiesGroup {
public:
};

class b2Pile : public b2BodiesGroup {
public:
};

class b2Tightrope : public b2BodiesGroup {
public:
	b2Body* start;
 b2Body* end;
 b2Body* middle;
};

class b2Lines : public b2BodiesGroup {
public:
};

class b2Cantilever : public b2BodiesGroup {
public:
};

class b2BodiesGroups : public LinkedList {
public:
 string name;
 CLEARLISTRESET(b2BodiesGroup);
 ~b2BodiesGroups() { Clear(); }
};

class TouchableBox2dWorld : public b2ContactListener {
public:
 Box2dExtents view;
	b2Body* m_groundBody;
	b2AABB m_worldAABB;
	ContactPoint m_points[k_maxContactPoints];
	int m_pointCount;
	DestructionListener m_destructionListener;
	int m_textLine;
	b2World* m_world;
	b2Body* m_bomb;
	b2MouseJoint* m_mouseJoint;
	b2Vec2 m_bombSpawnPoint;
	bool m_bombSpawning;
	b2Vec2 m_mouseWorld;
	int m_stepCount;
	float hz;
	int velocityIterations;
	int positionIterations;
	bool enableWarmStarting;
	bool enableContinuous;
	bool singleStep;
 bool paused;
	b2Vec2 gravity;
 b2BodyList bodies;
 b2BodiesGroups groups;

	virtual ~TouchableBox2dWorld() {
	 // By deleting the world, we delete the bomb, mouse joint, etc.
	 if ( m_world ) delete m_world;
	 m_world = NULL;
 }

 TouchableBox2dWorld() {
  velocityIterations=8;
  positionIterations=3;
  enableWarmStarting=true;
  enableContinuous=true;
  singleStep=false;
  paused=false;
  hz=FPS;
 	gravity.Set(0.0f, -10.0f);
 	m_bomb = NULL;
 	m_textLine = 30;
 	m_mouseJoint = NULL;
 	m_pointCount = 0;
 	m_bombSpawning = false;
 	m_stepCount = 0;
 	m_destructionListener.world = this;
  m_world=null;
 }

 virtual void Singularity() {}
	virtual void BigBang() {
  Singularity();
 	m_world = new b2World(gravity, true);
 	m_world->SetDestructionListener(&m_destructionListener);
 	m_world->SetContactListener(this);
 	b2BodyDef bodyDef;
 	m_groundBody = m_world->CreateBody(&bodyDef);
  Inflation();
 }
 virtual void Inflation() {}

 b2BodyHandle *AddPoly(
  Vertices *vertices,
  Cartesian *dimensions,
  float density,
  float friction,
  bool wakes, bool fixedRotation, bool calcTOC, bool active,
  void *data ) {
			b2BodyDef bd;
  	b2PolygonShape shape;
   bd.linearDamping = 0.0f;   // recommended
   bd.angularDamping = 0.01f; // between 0.0f - 0.1f recommended
   bd.position.Set(dimensions->fx,dimensions->fy);
   bd.allowSleep = true;
   bd.bullet=calcTOC;
   bd.fixedRotation = fixedRotation;
   bd.awake = wakes;
   bd.active = active;
   bd.userData = data;
 		bd.angle = dimensions->a; // radians
 		bd.type = b2_dynamicBody;
			b2Body* b = m_world->CreateBody(&bd);

   b2Vec2 *vertex;
   vertex = new b2Vec2[(unsigned int) vertices->count];
   int i=0;
   EACH(vertices->first,Vertex,v) {
    vertex[i].Set(v->x*dimensions->fx,v->y*dimensions->fy);
    i++;
   }
   b2PolygonShape polygon;
   polygon.Set(vertex,vertices->count);

   b2FixtureDef fd;
			fd.shape = &polygon;
			fd.friction = friction;
			fd.density = density;
			b->CreateFixture(&fd);
   return bodies.Add(b,"");
 }

 b2BodyHandle *AddLines(
  Vertices *vertices,
  Cartesian *dimensions,
  float density,
  float friction,
  bool wakes, bool fixedRotation, bool calcTOC, bool active,
  void *data ) {
   if ( vertices->count < 2 ) {
    OUTPUT("TouchableBox2dWorld:AddLines() requires input of more than one vertex.\n" );
    return null;
   }
			b2BodyDef bd;
  	b2PolygonShape shape;
   bd.linearDamping = 0.0f;   // recommended
   bd.angularDamping = 0.01f; // between 0.0f - 0.1f recommended
   bd.position.Set(dimensions->fx,dimensions->fy);
   bd.allowSleep = true;
   bd.bullet=calcTOC;
   bd.fixedRotation = fixedRotation;
   bd.awake = wakes;
   bd.active = active;
   bd.userData = data;
 		bd.angle = dimensions->a; // radians
 		bd.type = b2_dynamicBody;
			b2Body* b = m_world->CreateBody(&bd);

   b2FixtureDef fd;
			fd.friction = friction;
			fd.density = density;

   Vertex *prev=(Vertex *) vertices->first;
   EACH(vertices->first->next,Vertex,v) {    
				b2PolygonShape shape;
				shape.SetAsEdge(b2Vec2(prev->x, prev->y), b2Vec2(v->x, v->y));
    fd.shape=&shape;
				b->CreateFixture(&fd);
    prev=v;
   }
   return bodies.Add(b,"");
 }

 b2BodyHandle *AddBody(
  b2BodyTypes bodyType,
  Cartesian *dimensions,
  float density,
  float friction,
  float elasticity,
  bool wakes, bool fixedRotation, bool calcTOC, bool active, 
  void *data ) {
   if ( bodyType != b2Circle
     && bodyType != b2Line
     && bodyType != b2Box ) {
    OUTPUT("TouchableBox2dWorld:AddBody() warns it only supports bodyTypes of b2Line, b2Box, b2Circle.\n");
    return null;
   }
   // Place
			b2BodyDef bd;
  	b2PolygonShape shape;
   bd.linearDamping = 0.0f;   // recommended
   bd.angularDamping = 0.01f; // between 0.0f - 0.1f recommended
   bd.position.Set(dimensions->fx,dimensions->fy);
   bd.allowSleep = true;
   bd.bullet=calcTOC;
   bd.fixedRotation = fixedRotation;
   bd.awake = wakes;
   bd.active = active;
   bd.userData = data;
 		bd.angle = dimensions->a; // radians
 		bd.type = b2_dynamicBody;
			b2Body* b = m_world->CreateBody(&bd);
   b2FixtureDef fd;
 		fd.friction = friction;
 		fd.density = density;
   fd.restitution = elasticity;
   switch ( bodyType ) {
    case b2Line:
     {
      float w2=dimensions->fw/2.0f;
	 	 	 shape.SetAsEdge( b2Vec2(-w2, 0.0f), b2Vec2(w2, 0.0f) );
   			fd.shape = &shape;
   			b->CreateFixture(&fd);
     }
     break;
    case b2Box:
     {
      shape.SetAsBox(dimensions->fw,dimensions->fh);
   			fd.shape = &shape;
   			b->CreateFixture(&fd);
     }
     break;
    case b2Circle:
     {
      b2CircleShape circle;
      circle.m_radius = dimensions->fw;
   			fd.shape = &circle;
   			b->CreateFixture(&fd);
     }
     break;
   }
   return bodies.Add(b,"");
 }
 
 Cartesian worldCoords;
 b2Body *AddBodyScreen(
  b2BodyTypes type,
  Cartesian *dimensions,
  Cartesian *camera,
  float density,
  float friction,
  float elasticity,
  bool wakes, bool fixedRotation, bool calcTOC, bool active, 
  void *data ) {
   // Convert Cartesian to world coordinates from screen coordinates
   worldCoords.fx=(camera->fx+dimensions->fx)/view.worldToScreen*view.zoom;
   worldCoords.fy=(camera->fy+dimensions->fy)/view.worldToScreen*view.zoom;
   worldCoords.fh=dimensions->fh/view.worldToScreen*view.zoom;
   worldCoords.fw=dimensions->fw/view.worldToScreen*view.zoom;
   worldCoords.a=dimensions->a; // radians
   // Place
   AddBody( type, &worldCoords, density, friction, elasticity, wakes, fixedRotation, calcTOC, active, data );
 }

 b2Body *FlatLand( float halfWidth ) {
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsEdge(b2Vec2(-halfWidth, 0.0f), b2Vec2(halfWidth, 0.0f));
			ground->CreateFixture(&shape, 0.0f);
   bodies.Add(ground,"flatland");
   return ground;
	}

 // Drop a pile
 b2Pile *DropPile( int e_count, b2Vec2 *x, Numbers *densities, float size, b2Vec2 *spacing, b2Vec2 *hwRatio, b2Vec2 *inclination ) {
  if ( densities->count < 1 ) {
   OUTPUT( "TouchableBox2dWorld:DropPile() densities must be at least 1 number long.\n" );
   return null;
  }
  b2Pile *pile=new b2Pile;
  b2BodyList *list=pile->New("pile");
		b2PolygonShape shape;
		shape.SetAsBox(size*hwRatio->x, size*hwRatio->y);

		b2Vec2 y;
		b2Vec2 deltaX=*spacing;
		b2Vec2 deltaY=*inclination;

		for (int i = 0; i < e_count; ++i)
		{
			y = *x;

			for (int j = i; j < e_count; ++j)
			{
    Number *n=(Number *) densities->Element(i+j%densities->count);
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position = y;
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&shape, n->f);
    list->Add(body,"piled");

				y += deltaY;
			}

			*x += deltaX;
		}
  groups.Append(pile);
	}

 // density = 40, friction = 0.5f  angleLimits = 90
 // w>h when horiz, but really depends on initial position, h>w when vertical
 b2Chain *Hang( int links, float x, float y, float w, float h,
   float spacing, float density,
   float pendulumDensity, 
   bool ringLastLink, // true (recommended) makes the last ring as dense as the pendulum
   float friction,
   float angleLimits ) {
  if ( links < 2 ) {
   OUTPUT( "TouchableBox2dWorld:Hang() needs more than 1 length.\n" );
   return null;
  }
  b2Chain *chain = new b2Chain;

  chain->name=string("chain");

		b2PolygonShape shape;
		shape.SetAsBox(w, h);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = density;
		fd.friction = friction;

		b2RevoluteJointDef jd;
  jd.collideConnected=false;
  jd.enableLimit=true;
  jd.lowerAngle=-angleLimits;
  jd.upperAngle=angleLimits;

  b2BodyList *Links=chain->New("links");
		b2Body* prevBody = null;
		for (int i = 0; i < links; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(x + (float) i * spacing, y);
			b2Body* body = m_world->CreateBody(&bd);
   if ( i == links-2 && ringLastLink ) {
  		jd.collideConnected = true;
    fd.density=pendulumDensity;
    chain->ring=body;
    Links->Add(body,"ring");
   } else
   if ( i == links-1 ) {
  		jd.collideConnected = true;
    jd.enableLimit=false;
    fd.density=pendulumDensity;
    chain->tail=body;
    Links->Add(body,"pendulum");
   } else
    Links->Add(body,"link");

			body->CreateFixture(&fd);

			b2Vec2 anchor((float)(i), y);
			if ( prevBody != null ) jd.Initialize(prevBody, body, anchor);
   else chain->head=prevBody;
			m_world->CreateJoint(&jd);
   
			prevBody = body;
		}
		groups.Append(chain);
 }

 // when spacing=1.0f, segW=0.5,segH=0.125, bounderOffset=1.5 when y=0.5
 b2Tightrope *Suspend( int segments, float x, float y, float spacing, float density, float friction, float segW, float segH ) {
  float spacing2=spacing/2.0f;
  b2Tightrope *rope=new b2Tightrope;
  b2BodyList *list=rope->New("rope");
		b2PolygonShape shape;
		shape.SetAsBox(segW, segH);

  rope->name=string("rope");

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = density;
		fd.friction = friction;

		b2RevoluteJointDef jd;

		b2Body* prevBody = null;
		for (int i = 0; i < segments; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(x + spacing * i, y);
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&fd);

			b2Vec2 anchor(x + spacing * i, y);

   if ( prevBody != null ) {
 			jd.Initialize(prevBody, body, anchor);
 			m_world->CreateJoint(&jd);
   } else {
    rope->start=body;
   }

			if (i == (segments >> 1))
			{
				rope->middle = body;
			}
			prevBody = body;
		}
  rope->end=prevBody;

  // Place rebounder guides? (insert here)

		b2Vec2 anchor(-x + spacing * segments, y);
		jd.Initialize(prevBody, rope->start, anchor);
		m_world->CreateJoint(&jd);

  groups.Append(rope);
 }


 // Box2D is tuned for meters-kilograms-seconds (MKS).
 // Your moving objects should be between 0.1 - 10 meters.
 // Do not use pixels as units! You will get a jittery simulation. 
 //
 // How do I convert pixels to meters?
 // Suppose you have a sprite for a character that is 100x100 pixels.
 // You decide to use a scaling factor that is 0.01. This will make the
 // character physics box 1m x 1m. So go make a physics box that is 1x1.
 // Now suppose the character starts out at pixel coordinate (345,679).
 // So position the physics box at (3.45,6.79). Now simulate the physics
 // world. Suppose the character physics box moves to (2.31,4.98), so move
 // your character sprite to pixel coordinates (231,498). Now the only tricky
 // part is choosing a scaling factor. This really depends on your game. You
 // should try to get your moving objects in the range 0.1 - 10 meters, with
 // 1 meter being the sweet spot. 
 inline int toScreenX( float x ) { return view.attached->x + (int) ((x/view.worldToScreen)*( view.w * view.zoom )); } 
 inline int toScreenY( float y ) { return view.attached->y + (int) ((y/view.worldToScreen)*( view.h * view.zoom )); }
 inline int fromScreenX( float x ) { return (int) ((x*view.worldToScreen)/( view.w * view.zoom ))-view.attached->x; } 
 inline int fromScreenY( float y ) { return (int) ((y*view.worldToScreen)/( view.h * view.zoom ))-view.attached->y; }

	void SetTextLine(int line) { m_textLine = line; }

	virtual void Keyboard(unsigned char key) { B2_NOT_USED(key); }

 void TranslateMousePosition() {
 	m_mouseWorld = b2Vec2( view.mouse.fx+fromScreenX(view.moved.fx), view.mouse.fy+fromScreenY(view.moved.fy) );	
 }
 
	void MouseMove() {
  TranslateMousePosition();
 	if (m_mouseJoint)	m_mouseJoint->SetTarget(m_mouseWorld);
 }
 
 virtual void ShiftMouseDown() {
  TranslateMousePosition();
 	if (m_mouseJoint != NULL)	return;
 	SpawnBomb(m_mouseWorld);
 }

	virtual void MouseDown()  {
  TranslateMousePosition();
 	if (m_mouseJoint != NULL)	return;
  // Make a small box.
 	b2AABB aabb;
 	b2Vec2 d;
 	d.Set(1.0f, 1.0f);
 	aabb.lowerBound = m_mouseWorld - d;
 	aabb.upperBound = m_mouseWorld + d;
	 // Query the world for overlapping shapes.
	 QueryCallback callback(m_mouseWorld);
	 m_world->QueryAABB(&callback, aabb);
	 if (callback.m_fixture) {
 		b2Body* body = callback.m_fixture->GetBody();
 		b2MouseJointDef md;
 		md.bodyA = m_groundBody;
 		md.bodyB = body;
 		md.target = m_mouseWorld;
 		md.maxForce = 1000.0f * body->GetMass();
 		m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
 		body->SetAwake(true);
	 }
 }

	virtual void MouseUp() {
  TranslateMousePosition();
 	if (m_mouseJoint)	{
 		m_world->DestroyJoint(m_mouseJoint);
 		m_mouseJoint = NULL;
 	}	
 	if (m_bombSpawning)	CompleteBombSpawn(m_mouseWorld);
 }

 /* Bombin' */
 void LaunchBomb() {
 	b2Vec2 p(float_range(0.0f, 30.0f)-15.0f, 30.0f);
 	b2Vec2 v = -5.0f * p;
 	LaunchBomb(p, v);
 }

 void LaunchBomb(const b2Vec2& position, const b2Vec2& velocity) {
 	if (m_bomb)	{
 		m_world->DestroyBody(m_bomb);
 		m_bomb = NULL;
 	} 
 	b2BodyDef bd;
 	bd.type = b2_dynamicBody;
 	bd.position = position;
 	bd.bullet = true;
 	m_bomb = m_world->CreateBody(&bd);
 	m_bomb->SetLinearVelocity(velocity); 	
 	b2CircleShape circle;
 	circle.m_radius = 0.3f; 
 	b2FixtureDef fd;
 	fd.shape = &circle;
 	fd.density = 20.0f;
 	fd.restitution = 0.0f; 	
 	b2Vec2 minV = position - b2Vec2(0.3f,0.3f);
 	b2Vec2 maxV = position + b2Vec2(0.3f,0.3f); 	
 	b2AABB aabb;
 	aabb.lowerBound = minV;
 	aabb.upperBound = maxV; 
 	m_bomb->CreateFixture(&fd);
 }

	void SpawnBomb(const b2Vec2& worldPt) {
 	m_bombSpawnPoint = worldPt;
 	m_bombSpawning = true;
 }

	void CompleteBombSpawn(const b2Vec2& p) {
 	if (m_bombSpawning == false)	return;
 	const float multiplier = 30.0f;
 	b2Vec2 vel = m_bombSpawnPoint - p;
 	vel *= multiplier;
 	LaunchBomb(m_bombSpawnPoint,vel);
 	m_bombSpawning = false;
 }

	// Let derived tests know that a joint was destroyed.
	virtual void JointDestroyed(b2Joint* joint)   { B2_NOT_USED(joint);   }

	// Callbacks for derived classes.
	virtual void BeginContact(b2Contact* contact) { B2_NOT_USED(contact); }

	virtual void EndContact(b2Contact* contact)   { B2_NOT_USED(contact); }

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
 	const b2Manifold* manifold = contact->GetManifold(); 
 	if (manifold->pointCount == 0)	return;
 	b2Fixture* fixtureA = contact->GetFixtureA();
 	b2Fixture* fixtureB = contact->GetFixtureB();
 	b2PointState state1[b2_maxManifoldPoints], state2[b2_maxManifoldPoints];
 	b2GetPointStates(state1, state2, oldManifold, manifold);
 	b2WorldManifold worldManifold;
 	contact->GetWorldManifold(&worldManifold);
 	for (int i = 0; i < manifold->pointCount && m_pointCount < k_maxContactPoints; ++i)
 	{
 		ContactPoint* cp = m_points + m_pointCount;
 		cp->fixtureA = fixtureA;
 		cp->fixtureB = fixtureB;
 		cp->position = worldManifold.points[i];
 		cp->normal = worldManifold.normal;
 		cp->state = state2[i];
 		++m_pointCount;
 	}
 }

	virtual void PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse)
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(impulse);
	}

 /*
  * Why is the restitution/friction mixing inaccurate?
  * A physically correct restitution value must be measured in experiments.
  * But as soon as you change the geometry from the experiment then the value
  * is wrong. Next, adding simultaneous collision makes the answer worse. We've
  * been down this road before.
  *
  * So the question of accuracy has been answered: failure.
  *
  * The only remaining question is how do we make it convenient. On this opinions may vary.
  * b2Settings is just that. Settings you can adjust if you know what you are doing. 
  */
 virtual void Step() {
 	float timeStep = hz > 0.0f ? 1.0f / hz : (0.0f); // maybe move this out of the loop?
 	if (paused)	{
 		if (singleStep)	singleStep = false;
 		else	timeStep = 0.0f;
 	} 
 	m_world->SetWarmStarting(enableWarmStarting);
 	m_world->SetContinuousPhysics(enableContinuous); 
 	m_pointCount = 0; 
 	m_world->Step(timeStep, velocityIterations, positionIterations); 
 	if (timeStep > 0.0f)	++m_stepCount;
 }

 void MoveBy( float dx, float dy ) {
		view.center.fx += dx;
		view.center.fy += dx;
  view.Resized(view.attached);
 }

 // Call Attach or Resize after this
 void ResetView() {
		view.zoom = 1.0f;
		view.center.Set(0.0f, 20.0f);
  Resize(view.attached);
 }

 void Attach(GLWindow *w) { view.Attach(w); }
 void Resize(GLWindow *w) { view.Resized(w); }

protected:
	friend class DestructionListener;
	friend class BoundaryListener;
	friend class ContactListener;
};

class PhysicsToy : public GLWindow {
public:
 Zp<TouchableBox2dWorld> simulated;
 Zp<GLImage> box;
 Crayon tint;

 void CameraArrows() {
  int movex=0,movey=0;
  if ( input->KeyDown(DX_LEFT) )  movex--;
  else
  if ( input->KeyDown(DX_RIGHT) ) movex++;
  else
  if ( input->KeyDown(DX_UP) )    movey--;
  else
  if ( input->KeyDown(DX_DOWN) )  movey++;
  simulated->view.MoveCameraBy(movex,movey);
  MouseMove();
 }

 virtual void PreProcessing() { }
 virtual void PostProcessing() { }

 virtual void Each( b2Body *body, b2Vec2 position, float x, float y ) {
  
   tint.Any();
   
   RotatedRectf(this,box,crayons.Pick(alabaster),additive,
    x,y,
    10.0f,5.0f, 
    -float_range(0.0f,rad2deg(body->GetAngle()))
   );
   RotatedRectf(this,box,tint,additive,
    x,y,
    10.0f,5.0f, 
    rad2deg(body->GetAngle())
   );
   RotatedRectf(this,box,tint,additive,
    x,y,
    10.0f,5.0f, 
    float_range(0.0f,rad2deg(body->GetAngle()))
   );
 }

 Cartesian position;
 void TranslatePosition ( b2Body *b ) {
  b2Vec2 bodyPosition=b->GetPosition();
  position.fx=(float) this->x+simulated->view.camera.fx+(bodyPosition.x*simulated->view.worldToScreen);
  position.fx*=simulated->view.zoom;
  position.fy=(float) this->y+simulated->view.camera.fy-(bodyPosition.y*simulated->view.worldToScreen);
  position.fy*=simulated->view.zoom;
 }
 
 virtual void Render() {
  PreProcessing();
  b2Body *bodies=simulated->m_world->GetBodyList();
  for ( b2Body *body = bodies; body; body=body->GetNext() ) {
   b2Vec2 bodyPosition=body->GetPosition();
   position.fx=(float) this->x+simulated->view.camera.fx+(bodyPosition.x*simulated->view.worldToScreen);
   position.fx*=simulated->view.zoom;
   position.fy=(float) this->y+simulated->view.camera.fy-(bodyPosition.y*simulated->view.worldToScreen);
   position.fy*=simulated->view.zoom;
   Each( body, bodyPosition, position.fx, position.fy );
  }
  PostProcessing();
 }

 void Debug() {
  if ( simulated->m_mouseJoint ) {
   b2Vec2 aPosition=simulated->m_mouseJoint->GetAnchorA();
   b2Vec2 bPosition=simulated->m_mouseJoint->GetAnchorB();
   Cartesian a,b;
   a.Set((simulated->view.camera.fx)+(aPosition.x*simulated->view.worldToScreen*simulated->view.zoom),
         (simulated->view.camera.fy)-(aPosition.y*simulated->view.worldToScreen*simulated->view.zoom));
   b.Set((simulated->view.camera.fx)+(bPosition.x*simulated->view.worldToScreen*simulated->view.zoom),
         (simulated->view.camera.fy)-(bPosition.y*simulated->view.worldToScreen*simulated->view.zoom));
   DrawLine(this,crayons.Pick(red255),a.x,a.y,b.x,b.y);
  }
  b2Body *bodies=simulated->m_world->GetBodyList();
  for ( b2Body *body = bodies; body; body=body->GetNext() ) {
   b2Vec2 bodyPosition=body->GetPosition();
   DrawPixelf(this,crayons.Pick(red255),64+bodyPosition.x,128-bodyPosition.y);
  }
 }
};

#endif
