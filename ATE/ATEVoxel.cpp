#include "ATEVoxel.h"

void ATEVoxelLattice::Init( int width, int height, int depth ) {
 extents.Set(width,height,depth);
 this->Size((size_t)width, (size_t)height);
 for ( int w=0; w<width; w++ ) {
  for ( int h=0; h< height; h++ ) {
   (*this)(w,h).Size((size_t)depth);
   for ( int d=0; d< (int) depth; d++ ) {
    ATEVoxelNode *v=Get(w,h,d);
    if ( v ) {
     v->Position(w,h,d);
     v->nearbyVoxels.Get(*this,w,h,d);
    }
   }
  }
 }
}

ATEVoxelNode *ATEVoxelLattice::Get(unsigned int x, unsigned int y, unsigned int z) {
 ZIndexed<ATEVoxelNode> *vn=this->Element(x,y);
 if (vn && vn != &this->oob) {
  ATEVoxelNode *v=vn->Element(z);
  if ( v && v != &vn->oob ) return v;
 }
 return nullptr;
}

ATEVoxelNode *ATEVoxelLattice::Get(unsigned int i, unsigned int j) {
 return this->Element(i)->Element(j);
}

ATEVoxelNode *ATEVoxelLattice::GetLeft(int x, int y, int z ) { return Get(x-1,y,z); }
ATEVoxelNode *ATEVoxelLattice::GetRight(int x, int y, int z ) { return Get(x+1,y,z); }
ATEVoxelNode *ATEVoxelLattice::GetUp(int x, int y, int z ) { return Get(x-1,y,z);}
ATEVoxelNode *ATEVoxelLattice::GetDown(int x, int y, int z ) { return Get(x,y+1,z); }
ATEVoxelNode *ATEVoxelLattice::GetFront(int x, int y, int z ) { return Get(x,y,z+1); }
ATEVoxelNode *ATEVoxelLattice::GetBack(int x, int y, int z ) { return Get(x,y,z-1); }
ATEVoxelNode *ATEVoxelLattice::Get(BoxFaceBool::Face direction, int x, int y, int z ) {
 switch ( direction ) {
 case BoxFaceBool::Face::up: return GetUp(x,y,z);
 case BoxFaceBool::Face::down: return GetDown(x,y,z);
 case BoxFaceBool::Face::left: return GetLeft(x,y,z);
 case BoxFaceBool::Face::right: return GetRight(x,y,z);
 case BoxFaceBool::Face::back: return GetBack(x,y,z);
 case BoxFaceBool::Face::front: return GetFront(x,y,z);
 default: return nullptr;
 }
}

void ATEVoxelLattice::SetVoxel( ATEVoxelNode &in ) {
 int w=(int)in.position.x;
 int h=(int)in.position.y;
 int d=(int)in.position.z;
 ATEVoxelNode *v=Get(w,h,d);
 if ( v ) {
  v->color.fromCrayon(in.color);
  v->position.Set(w,h);
  v->position.z=d;
  v->offset.Set(in.offset);
  v->rotation.Set(in.rotation);
  v->color.fromCrayon(in.color);
 }
}

void ATEVoxelLattice::fromString( const char *s ) {
 Zstring _in(s);
 const char *w=_in.Next();
 while (*w != '\0') {
  ATEVoxelNode v;
  v.fromString(w);
  SetVoxel(v);
  NEXTWORD;
 }
}
string ATEVoxelLattice::toString() {
 Zstring out;
 for ( int i=0; i< (int) length; i++ ) for ( int j=0; j< (int) (*this)[i].length; j++ ) {
  ATEVoxelNode *v=Get(i,j);
  if ( v->filled ) {
   out+="{";
   out+= v->toString();
   out+="} ";
  }
 }
 return out;
}

void ATEVoxelNode::NearbyVoxelsQuery::Get( ATEVoxelLattice &voxels, int x, int y, int z ) {
 self=voxels.Get(x,y,z);
 up=voxels.GetUp(x,y,z);
 down=voxels.GetDown(x,y,z);
 left=voxels.GetLeft(x,y,z);
 right=voxels.GetRight(x,y,z);
 back=voxels.GetBack(x,y,z);
 front=voxels.GetFront(x,y,z);
}

ATEVoxelNode::NearbyVoxelsQuery::IsOutsideQueryResult ATEVoxelNode::NearbyVoxelsQuery::WalkOutsideQuery(ATEVoxelLattice &voxels, int x, int y, int z, BoxFaceBool::Face from, bool walking) {
 if (x <= 0 || y <= 0 || z <= 0 || x >= (int)voxels.w-1 || y >= (int)voxels.h-1 || z >= (int)voxels(x,y).length-1 ) return IsOutsideQueryResult::FoundOutside;
 ATEVoxelNode *it=voxels.Get(x,y,z);
 if ( from != BoxFaceBool::Face::none && it->hasWalkedOutsideQuery ) return IsOutsideQueryResult::AlreadyWalked;
 if ( it->filled && !it->isTransparent() ) return IsOutsideQueryResult::CouldNotContinue;
 if (from != BoxFaceBool::Face::up) {
  IsOutsideQueryResult walk_up = WalkOutsideQuery(voxels, x, y + 1, z, BoxFaceBool::Face::down, true );
  if (walk_up == IsOutsideQueryResult::FoundOutside) return IsOutsideQueryResult::FoundOutside;
 }
 if (from != BoxFaceBool::Face::down) {
  IsOutsideQueryResult walk_down = WalkOutsideQuery(voxels, x, y - 1, z, BoxFaceBool::Face::up, true);
  if (walk_down == IsOutsideQueryResult::FoundOutside) return IsOutsideQueryResult::FoundOutside;
 }
 if (from != BoxFaceBool::Face::left) {
  IsOutsideQueryResult walk_left = WalkOutsideQuery(voxels, x - 1, y, z, BoxFaceBool::Face::right, true);
  if (walk_left == IsOutsideQueryResult::FoundOutside) return IsOutsideQueryResult::FoundOutside;
 }
 if (from != BoxFaceBool::Face::right) {
  IsOutsideQueryResult walk_right = WalkOutsideQuery(voxels, x + 1, y, z, BoxFaceBool::Face::left, true);
  if (walk_right == IsOutsideQueryResult::FoundOutside) return IsOutsideQueryResult::FoundOutside;
 }
 if (from != BoxFaceBool::Face::front) {
  IsOutsideQueryResult walk_front = WalkOutsideQuery(voxels, x, y, z - 1, BoxFaceBool::Face::back, true); // + or - ?
  if (walk_front == IsOutsideQueryResult::FoundOutside) return IsOutsideQueryResult::FoundOutside;
 }
 if (from != BoxFaceBool::Face::back) {
  IsOutsideQueryResult walk_back = WalkOutsideQuery(voxels, x, y, z + 1, BoxFaceBool::Face::front, true);
  if (walk_back == IsOutsideQueryResult::FoundOutside) return IsOutsideQueryResult::FoundOutside;
 }
 return IsOutsideQueryResult::CouldNotContinue;
}
// Attempts to find out if this voxel is on the outside of the model by traversing the voxel tree.
bool ATEVoxelNode::NearbyVoxelsQuery::IsOutside( ATEVoxelLattice &voxels, int x, int y, int z ) {
 ATEVoxelNode *it=voxels.Get(x,y,z);
 if ( !it ) return true;
 it->hasWalkedOutsideQuery=true;
 // Reset our temporary markers.
 for ( unsigned int i=0; i<voxels.length; i++ ) for ( unsigned int j=0; j<voxels[i].length; j++ ) voxels[i][j].hasWalkedOutsideQuery=false;
 // Run recursive query.
 bool result=(WalkOutsideQuery(voxels,x,y,z,BoxFaceBool::Face::none) == IsOutsideQueryResult::FoundOutside);
 it->hasWalkedOutsideQuery=false;
 return result;
}

bool ATEVoxelNode::NearbyVoxelsQuery::FaceOccluded( BoxFaceBool::Face face ) {
 if ( !self || !self->filled ) return true;
 ATEVoxelNode *v=Face(face);
 if ( !v ) return false;
 if ( v->shape != ATEVoxelNode::VoxelShape::Cube ) return true;
 if ( v->filled ) return true;
 return false;
}


void ATEVoxelLattice::MakeModel( Cartesiand &scale ) {
 for ( int i=0; i< (int) length; i++ ) {
  for ( int j=0; j< (int) (*this)[i].length; j++ ) {
   ATEVoxelNode *v=Get(i,j);
   if ( v ) v->CalculatePolygons( scale );
  }
 }
}