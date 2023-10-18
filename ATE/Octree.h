#pragma once
#include "ZeroTypes.h"
#include "ZIndexed.h"

// TODO: Before using this, templatize.


// Structure of a point 
struct Point { 
 int x,y,z; 
 Point() : x(-1), y(-1), z(-1) {} 
 Point(int a, int b, int c) : x(a), y(b), z(c) {} 
};

// Octree class 
class Octree { 
 enum OctreeDirections {
  None=-1,
  TopLeftFront=0,
  TopRightFront=1,
  BottomRightFront=2,
  BottomLeftFront=3,
  TopLeftBottom=4,
  TopRightBottom=5,
  BottomRightBack=6,
  BottomLeftBack=7 
 };

 // if point == NULL, node is internal node. 
 // if point == (-1, -1, -1), node is empty. 
 Zdis<Point> point; 

 // Represent the boundary of the cube 
 Zdis<Point> topLeftFront, bottomRightBack; 
 ZIndexed<Zdis<Octree>> children; 

public: 
 // Constructor to declare empty node  
 Octree() { point.Instantiate(); } 

 // Constructor with three arguments to declare point node 
 Octree(int x, int y, int z) { point=new Point(x, y, z); } 

 // Constructor with six arguments, use to construct Octree with boundaries defined 
 Octree(int x1, int y1, int z1, int x2, int y2, int z2) { 
  if (x2 < x1 || y2 < y1 || z2 < z1) return; 
  topLeftFront = new Point(x1, y1, z1); 
  bottomRightBack = new Point(x2, y2, z2); 
  children.Size(8);
  for (int i = TopLeftFront; i <= BottomLeftBack; ++i) children[i] = new Octree(); 
 } 

 // Function to insert a point in the octree 
 void Insert(int x, int y, int z) {
  // If the point already exists in the octree 
  if ( Search(x, y, z) ) return;

  // If the point is out of bounds 
  if (x < topLeftFront->x || x > bottomRightBack->x 
   || y < topLeftFront->y || y > bottomRightBack->y 
   || z < topLeftFront->z || z > bottomRightBack->z) 
   return; 

  // Binary search to insert the point 
  int midx = (topLeftFront->x + bottomRightBack->x) / 2; 
  int midy = (topLeftFront->y + bottomRightBack->y) / 2; 
  int midz = (topLeftFront->z + bottomRightBack->z) / 2; 
  OctreeDirections d = OctreeDirections::None; 

  // Checking the octant of the point 
  if (x <= midx) { 
   if (y <= midy) { 
    if (z <= midz) d = TopLeftFront; 
    else d = TopLeftBottom; 
   } else { 
    if (z <= midz) d = BottomLeftFront; 
    else d = BottomLeftBack; 
   } 
  } else { 
   if (y <= midy) { 
    if (z <= midz) d = TopRightFront; 
    else d = TopRightBottom; 
   } else { 
    if (z <= midz) d = BottomRightFront; 
    else d = BottomRightBack; 
   }
  } 

  // If an internal node is encountered 
  if (!children[d]->point) { 
   children[d]->Insert(x, y, z);
   return; 
  } 
  else if (children[d]->point->x == -1) { // If an empty node is encountered 
   children[d].Recycle(new Octree(x, y, z));
   return;
  } else { 
   Point old(children[d]->point->x,children[d]->point->y,children[d]->point->z);
   switch (d) {
    case None: children[d].Deinstantiate(); break;
    case TopLeftFront:
     children[d].Recycle(new Octree(
      topLeftFront->x,
      topLeftFront->y,
      topLeftFront->z,
      midx,
      midy,
      midz
     ));
    case TopRightFront:
     children[d].Recycle(new Octree(
      midx + 1,
      topLeftFront->y,
      topLeftFront->z,
      bottomRightBack->x,
      midy,
      midz
     ));
    case BottomRightFront:
     children[d].Recycle(new Octree(
      midx + 1,
      midy + 1,
      topLeftFront->z,
      bottomRightBack->x,
      bottomRightBack->y,
      midz
     ));
    case BottomLeftFront:
     children[d].Recycle(new Octree(
      topLeftFront->x,
      midy + 1,
      topLeftFront->z,
      midx,
      bottomRightBack->y,
      midz
     ));
    case TopLeftBottom:
     children[d].Recycle(new Octree(
      topLeftFront->x,
      topLeftFront->y,
      midz + 1,
      midx,
      midy,
      bottomRightBack->z
     ));
    case TopRightBottom:
     children[d] = new Octree(
      midx + 1,
      topLeftFront->y,
      midz + 1,
      bottomRightBack->x,
      midy,
      bottomRightBack->z
     );
    case BottomRightBack:
     children[d] = new Octree(
      midx + 1,
      midy + 1,
      midz + 1,
      bottomRightBack->x,
      bottomRightBack->y,
      bottomRightBack->z
     );
    case BottomLeftBack:
     children[d] = new Octree(
      topLeftFront->x,
      midy + 1,
      midz + 1,
      midx,
      bottomRightBack->y,
      bottomRightBack->z
     );
    break;
   }
   children[d]->Insert(old.x, old.y, old.z); 
   children[d]->Insert(x, y, z); 
  } 
 } 

 // Function that returns true if the point (x, y, z) exists in the octree 
 bool Search(int x, int y, int z) { 
  // If point is out of bound 
  if (x < topLeftFront->x || x > bottomRightBack->x 
   || y < topLeftFront->y || y > bottomRightBack->y 
   || z < topLeftFront->z || z > bottomRightBack->z) 
   return 0; 
  // Otherwise perform binary search for each ordinate 
  int midx = (topLeftFront->x + bottomRightBack->x) / 2; 
  int midy = (topLeftFront->y + bottomRightBack->y) / 2; 
  int midz = (topLeftFront->z + bottomRightBack->z) / 2; 
  int pos = -1; 

  // Deciding the position where to move 
  if (x <= midx) { 
   if (y <= midy) { 
    if (z <= midz) pos = TopLeftFront; 
    else pos = TopLeftBottom; 
   } else { 
    if (z <= midz) pos = BottomLeftFront; 
    else pos = BottomLeftBack; 
   } 
  } 
  else { 
   if (y <= midy) { 
    if (z <= midz) pos = TopRightFront; 
    else pos = TopRightBottom; 
   } else { 
    if (z <= midz) pos = BottomRightFront; 
    else pos = BottomRightBack; 
   } 
  } 

  // If an internal node is encountered 
  if ( !children[pos]->point) return children[pos]->Search(x, y, z); 
  else if (children[pos]->point->x == -1) return 0;  // If an empty node is encountered 
  else if (x == children[pos]->point->x && y == children[pos]->point->y && z == children[pos]->point->z) return 1; // If node is found with the given value 
  return 0; 
 } 
}; 