#include "VBO_VNTCTanBi.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVNTCTanBi::fromModel( ImmediateModel *m ) {
 if ( m->polygons.count > 1 ) {
  EACH(m->polygons.first,mPolygon,p) PushTriangle(p);
 } else {
  mPolygon *p=(mPolygon *) m->polygons.first;
  Vertex *v=(Vertex *) p->points.first;
  if ( v && !v->tangent || !v->bitangent ) p->CalcTangentBasis();
  int i=0;
  Vertex *a=null;
  Vertex *b=null;
  EACH(p->points.first,Vertex,v) {
   i++;
   if ( i % 3 == 1 ) a=v;
   else if ( i % 3 == 2 ) b=v;
   else PushTriangle( a, b, v );
  }
 }
}
void VBOStaticVNTCTanBi::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVNTCTanBi::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }

#include "OldVBO.h"
void VBOStaticVNTCTanBi::fromVBO( VBO16 *v ) {
 Vertex a,b,c;
 a.normal.Instantiate();
 b.normal.Instantiate();
 c.normal.Instantiate();
 for ( int i=0; i<v->sizeVertices; i++ ) {
  VBO_Vertex16 *p=&v->vertices[i];
  switch ( i%3 ) {
  case 0:
   a.Set(p->pos[0],p->pos[1],p->pos[2]);
   a.normal->x=p->normal[0];
   a.normal->y=p->normal[1];
   a.normal->z=p->normal[2];
   a.q=p->texCoord[0];
   a.r=p->texCoord[1];
   a.rgba[0]=p->color[0];
   a.rgba[1]=p->color[1];
   a.rgba[2]=p->color[2];
   a.rgba[3]=p->color[3];
   break;
  case 1:
   b.Set(p->pos[0],p->pos[1],p->pos[2]);
   b.normal->x=p->normal[0];
   b.normal->y=p->normal[1];
   b.normal->z=p->normal[2];
   b.q=p->texCoord[0];
   b.r=p->texCoord[1];
   b.rgba[0]=p->color[0];
   b.rgba[1]=p->color[1];
   b.rgba[2]=p->color[2];
   b.rgba[3]=p->color[3];
   break;
  case 2:
   c.Set(p->pos[0],p->pos[1],p->pos[2]);
   c.normal->x=p->normal[0];
   c.normal->y=p->normal[1];
   c.normal->z=p->normal[2];
   c.q=p->texCoord[0];
   c.r=p->texCoord[1];
   c.rgba[0]=p->color[0];
   c.rgba[1]=p->color[1];
   c.rgba[2]=p->color[2];
   c.rgba[3]=p->color[3];
   a.TangentBasis(&b,&c);
   b.TangentBasis(&a,&c);
   c.TangentBasis(&a,&b);
   PushTriangle(&a,&b,&c);
   break;
  }
 }
}

void VBOStaticVNTCTanBi::BindToStreams() {
 glEnableVertexAttribArray(0);	glVertexAttribPointer(	*position,	3,	GL_FLOAT, GL_FALSE,	sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,pos))	);
 glEnableVertexAttribArray(2);	glVertexAttribPointer( *normal,   3, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,normal)) );
 glEnableVertexAttribArray(1); glVertexAttribPointer( *texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,texCoord)) );
 glEnableVertexAttribArray(3); glVertexAttribPointer( *tangent,  3, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,tangent)) );
 glEnableVertexAttribArray(4); glVertexAttribPointer( *bitangent,3, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,bitangent)) );
 glEnableVertexAttribArray(5); glVertexAttribPointer( *color,    4, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,color)) );
}
void VBOStaticVNTCTanBi::UnbindFromStreams() {
 glDisableVertexAttribArray(0);
 glDisableVertexAttribArray(2);
 glDisableVertexAttribArray(1);
 glDisableVertexAttribArray(3);
 glDisableVertexAttribArray(4);
 glDisableVertexAttribArray(5);
}

void VBOStaticVNTCTanBi::BindToStreams2() {
 glEnableVertexAttribArray(0);	glVertexAttribPointer(	*position,	3,	GL_FLOAT, GL_FALSE,	sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,pos))	);
 glEnableVertexAttribArray(1);	glVertexAttribPointer( *normal,   3, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,normal)) );
 glEnableVertexAttribArray(2); glVertexAttribPointer( *texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,texCoord)) );
 glEnableVertexAttribArray(3); glVertexAttribPointer( *tangent,  3, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,tangent)) );
}
void VBOStaticVNTCTanBi::UnbindFromStreams2() {
 glDisableVertexAttribArray(0);
 glDisableVertexAttribArray(1);
 glDisableVertexAttribArray(2);
 glDisableVertexAttribArray(3);
}

void VBOStaticVNTCTanBi::BindToStreams3() {
 glEnableVertexAttribArray(0);	glVertexAttribPointer(	*position,	3,	GL_FLOAT, GL_FALSE,	sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,pos))	);
 glEnableVertexAttribArray(1);	glVertexAttribPointer( *normal,   3, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,normal)) );
 glEnableVertexAttribArray(2); glVertexAttribPointer( *tangent,  3, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,tangent)) );
 glEnableVertexAttribArray(3); glVertexAttribPointer( *bitangent,3, GL_FLOAT, GL_FALSE, sizeof(VBO_VNTCTanBi), BUFFER_OFFSET(offsetof(VBO_VNTCTanBi,bitangent)) );
}
void VBOStaticVNTCTanBi::UnbindFromStreams3() {
 glDisableVertexAttribArray(0);
 glDisableVertexAttribArray(1);
 glDisableVertexAttribArray(2);
 glDisableVertexAttribArray(3);
}

#include "GLMCamera.h"
void VBOStaticVNTCTanBi::DebugRender() {
 glBegin(GL_TRIANGLES);
 for ( unsigned int i=0; i<indices.size(); i++ ) {
  VBO_VNTCTanBi *v=&data[indices[i]];
  glm::vec3 p(v->pos[0],v->pos[1],v->pos[2]);
  glColor3f(v->texCoord[0],v->texCoord[1],0.0f);
  glVertex3fv(&p.x);
 }
 glEnd();
}

void VBOStaticVNTCTanBi::DebugRender( glm::vec3 lightPos, GLMCamera *camera ) {

 if ( input.KeyDown(DX_LSHIFT) ) {
  // normals
  glColor3f(0,0,1);
  glBegin(GL_LINES);
  for (unsigned int i=0; i<indices.size(); i++){
   VBO_VNTCTanBi *v=&data[indices[i]];
   glm::vec3 p(v->pos[0],v->pos[1],v->pos[2]);
   glVertex3fv(&p.x);
   glm::vec3 o = glm::vec3(v->normal[0],v->normal[1],v->normal[2]);
   o=glm::normalize(o);
   p+=o*0.1f;
   glVertex3fv(&p.x);
  }
  glEnd();
  // tangents
  glColor3f(1,0,0);
  glBegin(GL_LINES);
  for (unsigned int i=0; i<indices.size(); i++){
   VBO_VNTCTanBi *v=&data[indices[i]];
   glm::vec3 p(v->pos[0],v->pos[1],v->pos[2]);
   glVertex3fv(&p.x);
   glm::vec3 o = glm::vec3(v->tangent[0],v->tangent[1],v->tangent[2]);
   o=glm::normalize(o);
   p+=o*0.1f;
   glVertex3fv(&p.x);
  }
  glEnd();
  // bitangents
  glColor3f(0,1,0);
  glBegin(GL_LINES);
  for (unsigned int i=0; i<indices.size(); i++){
   VBO_VNTCTanBi *v=&data[indices[i]];
   glm::vec3 p(v->pos[0],v->pos[1],v->pos[2]);
   glVertex3fv(&p.x);
   glm::vec3 o = glm::vec3(v->bitangent[0],v->bitangent[1],v->bitangent[2]);
   o=glm::normalize(o);
   p+=o*0.1f;
   glVertex3fv(&p.x);
  }
  glEnd();
 }

 // light pos
 glColor3f(1,1,1);
 glBegin(GL_LINES);
 glVertex3fv(&lightPos.x);
 lightPos+=glm::vec3(1,0,0)*0.1f;
 glVertex3fv(&lightPos.x);
 lightPos-=glm::vec3(1,0,0)*0.1f;
 glVertex3fv(&lightPos.x);
 lightPos+=glm::vec3(0,1,0)*0.1f;
 glVertex3fv(&lightPos.x);
 glEnd();
}
