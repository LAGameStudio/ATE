#pragma once

#include "ZeroTypes.h"
#include "LinkedList.h"
#include "ZIndexed.h"

#include "Trigger.h"
#include "MagicaVoxLoader.h"
#include "Model.h"

#include "Atlas.h"

#include "VBO_VNTCTan.h"

class SimpleTriangle {
public:
 Crayon color;
 Vertexd a,b,c;
 Vertexd atc,btc,ctc;
 Vertexd T,B,N;
 void Scale ( Cartesiand &scale ) {
  a.Multiply(scale.x,scale.y,scale.z);
  b.Multiply(scale.x,scale.y,scale.z);
  c.Multiply(scale.x,scale.y,scale.z);
 }
 void MoveBy( double dx, double dy, double dz ) {
  a.Add(dx,dy,dz);
  b.Add(dx,dy,dz);
  c.Add(dx,dy,dz);
 }
 void PushTo(mPolygon &pA) {
  Vertex *v1=new Vertex;
  v1->Set(a.x,a.y,a.z);
  v1->TexCoord(atc.x,atc.y);
  v1->Color(color);
  Vertex *v2=new Vertex;
  v2->Set(b.x,b.y,b.z);
  v2->TexCoord(btc.x,btc.y);
  v2->Color(color);
  Vertex *v3=new Vertex;
  v3->Set(c.x,c.y,c.z);
  v3->TexCoord(ctc.x,ctc.y);
  v3->Color(color);
  v1->SetTriNormal(v1,v2,v3);
  v2->SetTriNormal(v1,v2,v3);
  v3->SetTriNormal(v1,v2,v3);
  v1->TangentBasis(v2,v3);
  v2->TangentBasis(v1,v3);
  v3->TangentBasis(v1,v2);
  v1->BiTangent(v2,v3);
  v2->BiTangent(v1,v3);
  v3->BiTangent(v1,v2);
  pA.addVertex(v1);
  pA.addVertex(v2);
  pA.addVertex(v3);
  pA.draw_method=GL_TRIANGLES;
 }
 void PushTo( VBOStaticVNTCTanBi &out ) {
  static mPolygon pA;
  PushTo(pA);
  out.PushTriangle(&pA);
 }
};

class BoxFaceBool {
public:
 enum Face { none=-1, up = 0, down = 1, left = 2, right = 3, front = 4, back = 5 };
 ZIndexed<Zbool> faces;
 BoxFaceBool() { faces.Size(6); }
 bool Up() { return faces[BoxFaceBool::Face::up]; }
 bool Down() { return faces[BoxFaceBool::Face::down]; }
 bool Left() { return faces[BoxFaceBool::Face::left]; }
 bool Right() { return faces[BoxFaceBool::Face::right]; }
 bool Front() { return faces[BoxFaceBool::Face::front]; }
 bool Back() { return faces[BoxFaceBool::Face::back]; }
 void Up( bool value ) { faces[BoxFaceBool::Face::up]=value; }
 void Down( bool value ) { faces[BoxFaceBool::Face::down]=value; }
 void Left( bool value ) { faces[BoxFaceBool::Face::left]=value; }
 void Right( bool value ) { faces[BoxFaceBool::Face::right]=value; }
 void Front( bool value ) { faces[BoxFaceBool::Face::front]=value; }
 void Back( bool value ) { faces[BoxFaceBool::Face::back]=value; }
 bool All() { return Up() && Down() && Left() && Right() && Front() && Back(); }
 void All( bool value ) { for ( unsigned int i=0; i<faces.length; i++ ) faces[i]=value; }
 void Set( BoxFaceBool &in ) { for ( unsigned int i=0; i<faces.length; i++ ) { faces[i]=in.faces[i]; } }
 void Set( Face face, bool value ) { faces[face]=value; }
};

class HorizVertFlip {
public:
 Zbool h,v;
};

class TexCoordModifiers {
public:
 enum Rotation {
  zero,
  ninety,
  oneeighty,
  twoseventy
 };
 HorizVertFlip flip;
 Rotation rotation;
 TexCoordModifiers() {
  rotation=zero;
 }
 bool Zero() {
  return (!flip.h && !flip.v && rotation == TexCoordModifiers::Rotation::zero);
 }
 KEYWORDS({
  ENUMWORD("r",rotation,TexCoordModifiers::Rotation)
  TAGWORD("h",flip.h)
  TAGWORD("v",flip.v)
 })
 WORDKEYS({
  TAGKEY("h",flip.h)
  TAGKEY("v",flip.v)
  if ( rotation != TexCoordModifiers::Rotation::zero ) ENUMKEY("r",rotation)
 })
};

class BoxFaceTexCoordModifiers {
public:
 ZIndexed<TexCoordModifiers> modifiers;
 BoxFaceTexCoordModifiers() {
  modifiers.Size(6);
 }
 TexCoordModifiers *Get( BoxFaceBool::Face face ) {
  if ( face == BoxFaceBool::Face::none ) return nullptr;
  return modifiers.Element((int)face);
 }
 KEYWORDS({
  SSUBWORD("0",modifiers[0])
  SSUBWORD("1",modifiers[1])
  SSUBWORD("2",modifiers[2])
  SSUBWORD("3",modifiers[3])
  SSUBWORD("4",modifiers[4])
  SSUBWORD("5",modifiers[5])
 })
 WORDKEYS({
  if ( !modifiers[0].Zero() ) SSUBKEY("0",modifiers[0])
  if ( !modifiers[1].Zero() ) SSUBKEY("1",modifiers[1])
  if ( !modifiers[2].Zero() ) SSUBKEY("2",modifiers[2])
  if ( !modifiers[3].Zero() ) SSUBKEY("3",modifiers[3])
  if ( !modifiers[4].Zero() ) SSUBKEY("4",modifiers[4])
  if ( !modifiers[5].Zero() ) SSUBKEY("5",modifiers[5])
 })
};

#if defined(NEVER)
class BoxCornerBool {
public:
 enum Corner {
  frontupleft = 0, frontdownleft = 1, frontupright = 2, frontdownright = 3,
  backupleft = 4, backdownleft = 5, backupright = 6, backdownright = 7
 };
 void FaceCorners( BoxFaceBool::Face face, BoxCornerBool::Corner &a, BoxCornerBool::Corner &b, BoxCornerBool::Corner &c, BoxCornerBool::Corner &d ) {
  switch ( face ) {
   case BoxFaceBool::Face::up:
     a=Corner::backupleft;
     b=Corner::backupright;
     c=Corner::frontupright;
     d=Corner::frontupleft;
    break;
   case BoxFaceBool::Face::down:
     a=Corner::frontdownright;
     b=Corner::frontdownleft;
     c=Corner::backdownleft;
     d=Corner::backdownright;
    break;
   case BoxFaceBool::Face::left:
     a=Corner::backupleft;
     b=Corner::frontupleft;
     c=Corner::frontdownleft;
     d=Corner::backdownleft;
    break;
   case BoxFaceBool::Face::right:
     a=Corner::frontupright;
     b=Corner::backupright;
     c=Corner::backdownright;
     d=Corner::frontdownright;
    break;
   case BoxFaceBool::Face::front:
     a=Corner::frontupleft;
     b=Corner::frontupright;
     c=Corner::frontdownright;
     d=Corner::frontdownleft;
    break;
   case BoxFaceBool::Face::back:
     a=Corner::backupright;
     b=Corner::backupleft;
     c=Corner::backdownleft;
     d=Corner::backdownright;
    break;
  }
 }
 ZIndexed<Zbool> corners;
 BoxCornerBool() { corners.Size(8); }
 bool FrontUpLeft( ) { return corners[BoxCornerBool::Corner::frontupleft]; }
 bool FrontDownLeft( ) { return corners[BoxCornerBool::Corner::frontupleft]; }
 bool FrontUpRight( ) { return corners[BoxCornerBool::Corner::frontupleft]; }
 bool FrontDownRight( ) { return corners[BoxCornerBool::Corner::frontupleft]; }
 bool BackUpLeft( ) { return corners[BoxCornerBool::Corner::frontupleft]; }
 bool BackDownLeft( ) { return corners[BoxCornerBool::Corner::frontupleft]; }
 bool BackUpRight( ) { return corners[BoxCornerBool::Corner::frontupleft]; }
 bool BackDownRight( ) { return corners[BoxCornerBool::Corner::frontupleft]; }
 void FrontUpLeft( bool value ) { corners[BoxCornerBool::Corner::frontupleft]=value; }
 void FrontDownLeft( bool value ) { corners[BoxCornerBool::Corner::frontupleft]=value; }
 void FrontUpRight( bool value ) { corners[BoxCornerBool::Corner::frontupleft]=value; }
 void FrontDownRight( bool value ) { corners[BoxCornerBool::Corner::frontupleft]=value; }
 void BackUpLeft( bool value ) { corners[BoxCornerBool::Corner::frontupleft]=value; }
 void BackDownLeft( bool value ) { corners[BoxCornerBool::Corner::frontupleft]=value; }
 void BackUpRight( bool value ) { corners[BoxCornerBool::Corner::frontupleft]=value; }
 void BackDownRight( bool value ) { corners[BoxCornerBool::Corner::frontupleft]=value; }
 bool All() { return FrontUpLeft() && FrontDownLeft() && FrontUpRight() && FrontDownRight() && BackUpLeft() && BackDownLeft() && BackUpRight() && BackDownRight(); }
 void All( bool value ) { for ( unsigned int i=0; i<corners.length; i++ ) corners[i]=value; }
 void Set( BoxCornerBool &in ) { for ( unsigned int i=0; i<corners.length; i++ ) { corners[i]=in.corners[i]; } }
};
#endif

class BoxCornerPoints {
public:
 BoxCornerPoints() {
  points.Size(8);
 }
 ZIndexed<Cartesiand> points;
 enum Corner {
  frontupleft = 0, frontdownleft = 1, frontupright = 2, frontdownright = 3,
  backupleft = 4, backdownleft = 5, backupright = 6, backdownright = 7
 };
 void FaceCorners( BoxFaceBool::Face face, BoxCornerPoints::Corner &a, BoxCornerPoints::Corner &b, BoxCornerPoints::Corner &c, BoxCornerPoints::Corner &d ) {
  switch ( face ) {
  default: return;
  case BoxFaceBool::Face::up:
   a=Corner::backupleft;
   b=Corner::backupright;
   c=Corner::frontupright;
   d=Corner::frontupleft;
   break;
  case BoxFaceBool::Face::down:
   a=Corner::frontdownright;
   b=Corner::frontdownleft;
   c=Corner::backdownleft;
   d=Corner::backdownright;
   break;
  case BoxFaceBool::Face::left:
   a=Corner::backupleft;
   b=Corner::frontupleft;
   c=Corner::frontdownleft;
   d=Corner::backdownleft;
   break;
  case BoxFaceBool::Face::right:
   a=Corner::frontupright;
   b=Corner::backupright;
   c=Corner::backdownright;
   d=Corner::frontdownright;
   break;
  case BoxFaceBool::Face::front:
   a=Corner::frontupleft;
   b=Corner::frontupright;
   c=Corner::frontdownright;
   d=Corner::frontdownleft;
   break;
  case BoxFaceBool::Face::back:
   a=Corner::backupright;
   b=Corner::backupleft;
   c=Corner::backdownleft;
   d=Corner::backdownright;
   break;
  }
 }
 Cartesiand &_Get( BoxCornerPoints::Corner corner ) { return points[corner]; }
 void Set( Cartesiand &in ) {
  Set(BoxCornerPoints::Corner::frontupleft,in);
  Set(BoxCornerPoints::Corner::frontdownleft,in);
  Set(BoxCornerPoints::Corner::frontupright,in);
  Set(BoxCornerPoints::Corner::frontdownright,in);
  Set(BoxCornerPoints::Corner::backupleft,in);
  Set(BoxCornerPoints::Corner::backdownleft,in);
  Set(BoxCornerPoints::Corner::backupright,in);
  Set(BoxCornerPoints::Corner::backdownright,in);
 }
 void Set( BoxCornerPoints::Corner corner, Cartesiand &in ) {
  points[corner].Set(&in);
 }
 Cartesiand *Get( BoxCornerPoints::Corner corner ) {
  return &points[corner];
 }
 bool NoOffset( BoxCornerPoints::Corner corner ) {
  Cartesiand *p=Get(corner);
  return ( p->x == 0.0 && p->y == 0.0 && p->z == 0.0 );
 }
 bool NoOffset() {
  for ( int i=0; i<8; i++ ) if ( !NoOffset( (BoxCornerPoints::Corner) i ) ) return false;
  return true;
 }
 KEYWORDS({
  SUBWORD("0",points[0])
  else SUBWORD("1",points[1])
  else SUBWORD("2",points[2])
  else SUBWORD("3",points[3])
  else SUBWORD("4",points[4])
  else SUBWORD("5",points[5])
  else SUBWORD("6",points[6])
  else SUBWORD("7",points[7])
  })
 WORDKEYS({ 
  if (!NoOffset(BoxCornerPoints::Corner::frontupleft)   ) SSUBKEY("0",points[0])
  if (!NoOffset(BoxCornerPoints::Corner::frontdownleft) ) SSUBKEY("1",points[1])
  if (!NoOffset(BoxCornerPoints::Corner::frontupright)  ) SSUBKEY("2",points[2])
  if (!NoOffset(BoxCornerPoints::Corner::frontdownright)) SSUBKEY("3",points[3])
  if (!NoOffset(BoxCornerPoints::Corner::backupleft)    ) SSUBKEY("4",points[4])
  if (!NoOffset(BoxCornerPoints::Corner::backdownleft)  ) SSUBKEY("5",points[5])
  if (!NoOffset(BoxCornerPoints::Corner::backupright)   ) SSUBKEY("6",points[6])
  if (!NoOffset(BoxCornerPoints::Corner::backdownright) ) SSUBKEY("7",points[7])
 })
};

class ATEVoxelLattice;

ONE(ATEVoxelNode, {
 shape=VoxelShape::Cube;
 nearbyVoxels.self=this;
 faceTriangles.Size(6);
})
 class FaceTriangles {
 public:
  SimpleTriangle a,b;
  Zbool valid;
  void PushTo( VBOStaticVNTCTanBi &out ) { a.PushTo(out); b.PushTo(out); }
};
 ZIndexed<FaceTriangles> faceTriangles;
 class NearbyVoxelsQuery {
 public:
  Zint simpleOcclusions;
  Zp<ATEVoxelNode> up,down,left,right,front,back, self;
  void Get( ATEVoxelLattice &voxels, int x, int y, int z );
  ATEVoxelNode *Face( BoxFaceBool::Face face ) {
   switch ( face ) {
    case BoxFaceBool::Face::up: return up;
    case BoxFaceBool::Face::down: return down;
    case BoxFaceBool::Face::left: return left;
    case BoxFaceBool::Face::right: return right;
    case BoxFaceBool::Face::front: return front;
    case BoxFaceBool::Face::back: return back;
    default: return nullptr;
   }
  }
  void GetSimpleOcclusions( BoxFaceBool &occluded ) {
   simpleOcclusions=0;
   ATEVoxelNode *n=nullptr;
   occluded.All(false);
   if ( self->shape == VoxelShape::Cube && self->filled ) {
    for (int face = 0; face <= (int)BoxFaceBool::Face::back; face++) {
     n = Face((BoxFaceBool::Face) face);
     if (!n) continue;
     if ( n->shape == VoxelShape::Cube && n->filled ) {
      simpleOcclusions++;
      occluded.Set((BoxFaceBool::Face)face, true);
     }
    }
   }
  }
  enum IsOutsideQueryResult {
   CouldNotContinue=0,
   FoundOutside=1,
   AlreadyWalked=2
  };
  IsOutsideQueryResult WalkOutsideQuery(ATEVoxelLattice &voxels, int x, int y, int z, BoxFaceBool::Face from, bool walking = false);
  // Attempts to find out if this voxel is on the outside of the model by traversing the voxel tree.
  bool IsOutside( ATEVoxelLattice &voxels, int x, int y, int z );
  bool FaceOccluded( BoxFaceBool::Face face );
 };
 enum VoxelShape {
  Cube=0,
  Sphere=1,
  Cone=2,
  Cylinder=3,
  Hemisphere=4,
  TriPrism=5,
  Pyramid=6,
  Quadramid=7,
  Pentamid=8,
  Toroid=9,
  Tube=10,
  Menger=11
 };
 VoxelShape shape;
 Cartesian position;
 Vertexd offset,rotation;
 Crayon color;
 Zbool filled;
 Zint i,j;                        // Atlas cel
 BoxCornerPoints deformation;     // Ratio of 0-1 of placement of corners, where 0=not deformed.
 BoxFaceTexCoordModifiers tcMods; // Facial tex-coord modifiers
 // Query information:
 BoxFaceBool simplyOccluded;      // Occluded faces are pruned from output. These are simple tests of filled next to filled.
 Zbool hasWalkedOutsideQuery;     // Temporary marker for CanReachOutside query.
 Zbool isOutside;                 // Marked by FindOutside occlusion query.
 Zbool pristine;                  // Not deformed.
 NearbyVoxelsQuery nearbyVoxels;
 KEYWORDS({
  SUBWORD("c",color)
  else SUBWORD("o",offset)
  else SUBWORD("r",rotation)
  else SUBWORD("d",deformation)
  else ENUMWORD("s",shape,VoxelShape)
 })
 WORDKEYS({ 
  SSUBKEY("c",color)
  SSUBKEY("o",offset)
  SSUBKEY("r",rotation)
  if ( !pristine ) SSUBKEY("d",deformation)
  ENUMKEY("s",shape)
 })
 void Position( int x, int y, int z ) {
  position.Set(x,y);
  position.z=z;
 }
 void Paint( Crayon &ink, ATEVoxelNode::VoxelShape brush=ATEVoxelNode::VoxelShape::Cube ) {
 }
 void Erase() {
 }
 void Unerase() {
 }
 bool isFullyOccluded() {
  return simplyOccluded.All();
 }
 bool isTransparent() {
  return !filled || color.ad != 1.0;
 }
 void CalculatePolygons( Cartesiand &scale ) {
  switch ( shape ) {
  case VoxelShape::Cube:
   CalculateCubicVoxel( scale );
   break;
  }
 }
 void CalculateCubicVoxel( Cartesiand &scale ) {
  for ( int i=0; i<6; i++ ) faceTriangles[i].valid=GetFace(faceTriangles[i].a,faceTriangles[i].b,(BoxFaceBool::Face)i,scale);
 }
 bool GetFaceTriangles( BoxFaceBool::Face face, SimpleTriangle &a, SimpleTriangle &b, Cartesiand &scale ) {
  if ( face == BoxFaceBool::Face::none ) return false;
  Vertexd A,B,C,D;
  switch ( face ) {
   case BoxFaceBool::Face::up:
     A.Set(0.0,0.0,0.0);
     B.Set(1.0,0.0,0.0);
     C.Set(1.0,0.0,1.0);
     D.Set(0.0,0.0,1.0);
    break;
   case BoxFaceBool::Face::down:
     D.Set(0.0,1.0,0.0);
     C.Set(1.0,1.0,0.0);
     B.Set(1.0,1.0,1.0);
     A.Set(0.0,1.0,1.0);
    break;
   case BoxFaceBool::Face::left:
     A.Set(0.0,0.0,0.0);
     B.Set(0.0,0.0,1.0);
     C.Set(0.0,1.0,1.0);
     D.Set(0.0,1.0,0.0);
    break;
   case BoxFaceBool::Face::right:
     D.Set(1.0,0.0,0.0);
     C.Set(1.0,0.0,1.0);
     B.Set(1.0,1.0,1.0);
     A.Set(1.0,1.0,0.0);
    break;
   case BoxFaceBool::Face::front:
     A.Set(0.0,0.0,1.0);
     B.Set(1.0,0.0,1.0);
     C.Set(1.0,1.0,1.0);
     D.Set(0.0,1.0,1.0);
    break;
   case BoxFaceBool::Face::back:
     D.Set(0.0,0.0,0.0);
     C.Set(1.0,0.0,0.0);
     B.Set(1.0,1.0,0.0);
     A.Set(0.0,1.0,0.0);
    break;
  }
  BoxCornerPoints::Corner cA,cB,cC,cD;
  deformation.FaceCorners(face,cA,cB,cC,cD);
  Cartesiand *cpA=deformation.Get(cA);
  if ( !cpA ) return false;
  if ( A.x == 0.0 ) A.x=cpA->x; else A.x = A.x * cpA->x;
  if ( A.y == 0.0 ) A.y=cpA->y; else A.y = A.y * cpA->y;
  if ( A.z == 0.0 ) A.z=cpA->z; else A.z = A.z * cpA->z;
  Cartesiand *cpB=deformation.Get(cB);
  if ( !cpB ) return false;
  if ( B.x == 0.0 ) B.x=cpB->x; else B.x = B.x * cpB->x;
  if ( B.y == 0.0 ) B.y=cpB->y; else B.y = B.y * cpB->y;
  if ( B.z == 0.0 ) B.z=cpB->z; else B.z = B.z * cpB->z;
  Cartesiand *cpC=deformation.Get(cC);
  if ( !cpC ) return false;
  if ( C.x == 0.0 ) C.x=cpC->x; else C.x = C.x * cpC->x;
  if ( C.y == 0.0 ) C.y=cpC->y; else C.y = C.y * cpC->y;
  if ( C.z == 0.0 ) C.z=cpC->z; else C.z = C.z * cpC->z;
  Cartesiand *cpD=deformation.Get(cD);
  if ( !cpD ) return false;
  if ( D.x == 0.0 ) D.x=cpD->x; else D.x = D.x * cpD->x;
  if ( D.y == 0.0 ) D.y=cpD->y; else D.y = D.y * cpD->y;
  if ( D.z == 0.0 ) D.z=cpD->z; else D.z = D.z * cpD->z;
  a.a.Set(&A); a.b.Set(&B); a.c.Set(&D);
  b.a.Set(&D); b.b.Set(&B); b.c.Set(&C);
  TexCoordModifiers *tcMod=tcMods.Get(face);
  if ( tcMod && !tcMod->Zero() ) {
   switch ( tcMod->rotation ) {
   case TexCoordModifiers::Rotation::zero:
     a.atc.Set(0.0,0.0);
     a.btc.Set(1.0,0.0);
     a.ctc.Set(1.0,1.0);
     b.atc.Set(0.0,1.0);
     b.btc.Set(1.0,0.0);
     b.ctc.Set(0.0,1.0);
    break;
   case TexCoordModifiers::Rotation::ninety:
     a.atc.Set(0.0,1.0);
     a.btc.Set(0.0,0.0);
     a.ctc.Set(1.0,0.0);
     b.atc.Set(1.0,1.0);
     b.btc.Set(0.0,1.0);
     b.ctc.Set(0.0,0.0);
    break;
   case TexCoordModifiers::Rotation::oneeighty:
    a.atc.Set(1.0,0.0);
    a.btc.Set(0.0,1.0);
    a.ctc.Set(0.0,0.0);
    b.atc.Set(1.0,0.0);
    b.btc.Set(1.0,1.0);
    b.ctc.Set(0.0,1.0);
    break;
   case TexCoordModifiers::Rotation::twoseventy:
     a.atc.Set(0.0,1.0);
     a.btc.Set(1.0,0.0);
     a.ctc.Set(0.0,1.0);
     b.atc.Set(0.0,0.0);
     b.btc.Set(1.0,0.0);
     b.ctc.Set(1.0,1.0);
    break;
   }
   if ( tcMod->flip.h ) {
    a.atc.x=a.atc.x == 1.0 ? 0.0 : 1.0;
    a.btc.x=a.btc.x == 1.0 ? 0.0 : 1.0;
    a.ctc.x=a.ctc.x == 1.0 ? 0.0 : 1.0;
    b.atc.x=b.atc.x == 1.0 ? 0.0 : 1.0;
    b.btc.x=b.btc.x == 1.0 ? 0.0 : 1.0;
    b.ctc.x=b.ctc.x == 1.0 ? 0.0 : 1.0;
   }
   if ( tcMod->flip.v ) {
    a.atc.y=a.atc.y == 1.0 ? 0.0 : 1.0;
    a.btc.y=a.btc.y == 1.0 ? 0.0 : 1.0;
    a.ctc.y=a.ctc.y == 1.0 ? 0.0 : 1.0;
    b.atc.y=b.atc.y == 1.0 ? 0.0 : 1.0;
    b.btc.y=b.btc.y == 1.0 ? 0.0 : 1.0;
    b.ctc.y=b.ctc.y == 1.0 ? 0.0 : 1.0;
   }
  } else {
   a.atc.Set(0.0,0.0);
   a.btc.Set(1.0,0.0);
   a.ctc.Set(1.0,1.0);
   b.atc.Set(0.0,1.0);
   b.btc.Set(1.0,0.0);
   b.ctc.Set(0.0,1.0);
  }
  a.color.fromCrayon(color);
  b.color.fromCrayon(color);
  a.Scale(scale);
  b.Scale(scale);
  a.MoveBy(position.x * scale.x,position.y*scale.y,position.z*scale.z);
  b.MoveBy(position.x * scale.x,position.y*scale.y,position.z*scale.z);
  return true;
 }
 bool GetFace( SimpleTriangle &a, SimpleTriangle &b, BoxFaceBool::Face face, Cartesiand &scale ) {
  bool hasFace=!nearbyVoxels.FaceOccluded(face);
  if ( !hasFace ) return false;
  hasFace=GetFaceTriangles(face,a,b,scale);
  if ( !hasFace ) return false;
  return true;
 }
MANY_(ATEVoxelNode, {})
DONE(ATEVoxelNode);

class ATEVoxelLattice : public ZIndexed<ZIndexed<ATEVoxelNode>> {
public:
 Cartesian extents;
 void Init( int w, int h, int d );
 ATEVoxelNode *Get(unsigned int x, unsigned int y, unsigned int z);
 ATEVoxelNode *Get(unsigned int i, unsigned int j);
 ATEVoxelNode *GetLeft(int x, int y, int z );
 ATEVoxelNode *GetRight(int x, int y, int z );
 ATEVoxelNode *GetUp(int x, int y, int z );
 ATEVoxelNode *GetDown(int x, int y, int z );
 ATEVoxelNode *GetFront(int x, int y, int z );
 ATEVoxelNode *GetBack(int x, int y, int z );
 ATEVoxelNode *Get(BoxFaceBool::Face direction, int x, int y, int z );
 void SetVoxel( ATEVoxelNode &in );
 void fromString( const char *s );
 string toString();
 void MakeModel( Cartesiand &scale );
};

ONE(ATEVoxel, {
  scale.Set(1.0,1.0,1.0);
  lightPower1=10.0f;
  lightColor1.Pick(alabaster);
  lightPosition1.Set(0.0f,0.0f,3.0f);
  lightPower2=10.0f;
  lightColor2.Pick(alabaster);
  lightPosition2.Set(-3.0f,-3.0f,3.0f);
  lightPower3=10.0f;
  lightColor3.Pick(alabaster);
  lightPosition3.Set(3.0f,3.0f,3.0f);
 })
 Zstring filename;
 Zp<GLImage> preview;
 Cartesiand size,scale;
 ATEVoxelLattice voxels;
 Vertexd lightPosition1,lightPosition2,lightPosition3;
 Zfloat lightPower1,lightPower2,lightPower3;
 Crayon lightColor1,lightColor2,lightColor3;
 Zp<Atlas> atlas;
 KEYWORDS_POST({
  TXTWORD("name",name)
  else KEYWORD("size",{ size.fromString(w); Init((int)size.x,(int)size.y,(int)size.z); })
  else SUBWORD("scale",scale)
  else KEYWORD("voxels",{ voxels.fromString(w); })
  else SUBWORD("lpos1",lightPosition1)
  else SUBWORD("lpos2",lightPosition2)
  else SUBWORD("lpos3",lightPosition3)
  else TINTWORD("lcol1",lightColor1)
  else TINTWORD("lcol2",lightColor2)
  else TINTWORD("lcol3",lightColor3)
  else NUMWORD("lpow1",lightPower1)
  else NUMWORD("lpow2",lightPower2)
  else NUMWORD("lpow3",lightPower3)
 }, { PostLoad(); })
 WORDKEYS({ 
  TXTKEY("name",name)
  SSUBKEY("size",size)
  SSUBKEY("scale",scale)
  WORDKEY("voxels",voxels.toString())
  SSUBKEY("lpos1",lightPosition1)
  SSUBKEY("lpos2",lightPosition2)
  SSUBKEY("lpos3",lightPosition3)
  TINTKEY("lcol1",lightColor1)
  TINTKEY("lcol2",lightColor2)
  TINTKEY("lcol3",lightColor3)
  NUMBKEY("lpow1",lightPower1)
  NUMBKEY("lpow2",lightPower2)
  NUMBKEY("lpow3",lightPower3)
 })
 void PostLoad() {
  FindOutside();
  voxels.MakeModel(scale);
 }
 void Init( int w, int h, int d ) {
  size.Set(w,h,d);
  voxels.Init(w,h,d);
 }
 void Save( const char *path ) {
  string_as_file(toString().c_str(),(string(path)+filename).c_str());
 }
 void FindOutside() {
  for ( unsigned int x=0; x<voxels.w; x++ ) for ( unsigned int y=0; y<voxels.h; y++ ) for ( unsigned int z=0; z<voxels(x,y).length; z++ ) {
   ATEVoxelNode *v=voxels.Get(x,y,z);
   if ( v ) v->isOutside=v->nearbyVoxels.IsOutside(voxels,x,y,z);
  }
 }
 void Paint( int x, int y, int z, Crayon &ink, ATEVoxelNode::VoxelShape brush=ATEVoxelNode::VoxelShape::Cube ) {
  ATEVoxelNode *v=voxels.Get(x,y,z);
  if ( v ) v->Paint(ink,brush);
 }
 void Erase( int x, int y, int z ) {
  ATEVoxelNode *v=voxels.Get(x,y,z);
  if ( v ) v->Erase();
 }
 void Unerase( int x, int y, int z ) {
  ATEVoxelNode *v=voxels.Get(x,y,z);
  if ( v ) v->Unerase();
 }
 void CalculatePolygons() { voxels.MakeModel(scale); }
 void To( VBOStaticVNTCTanBi &out ) {
  for ( int i=0; i< (int) voxels.length; i++ ) for ( int j=0; j< (int) voxels[i].length; j++ ) {
   ATEVoxelNode *v=voxels.Get(i,j);
   for ( int i=0; i<6; i++ ) if ( v->faceTriangles[i].valid ) v->faceTriangles[i].PushTo(out);
  }
 }
MANY_(ATEVoxel, {})
 void Load( const char *path ) {
  Strings filters;
  filters.Add(".txt");
  Strings *filelist=ls(path,true,true,&filters);
  EACH(filelist->first,String,f) {
   ATEVoxel *v=new ATEVoxel;
   v->fromString(file_as_string(f->s.c_str()).c_str());
   Append(v);
  }
  delete filelist;
 }
 void Save(const char *path) { FOREACH(ATEVoxel,v) v->Save(path); }
DONE(ATEVoxel);

