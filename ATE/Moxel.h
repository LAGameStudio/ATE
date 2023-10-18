#pragma once

#include "ZeroTypes.h"
#include "LinkedList.h"
#include "ZIndexed.h"

#include "MagicaVoxLoader.h"
#include "Model.h"

#define MAX_STRIP_WIDTH 4096

ONE(MoxelNode, {})
 Vertexd position;
 Crayon color;
 Zbool filled;
MANY_(MoxelNode, {})
DONE(MoxelNode);

ONE(Moxel, {})
 Vertexd size;
 ZIndexed<ZIndexed<MoxelNode>> nodes;
 MoxelNodeHandles tops;
 Zdis<MV_Model> magicaVoxel;
 void Import(const char *filename) {
  magicaVoxel.Recycle(new MV_Model(filename));
  Import(magicaVoxel.pointer);
 }
 void Import(MV_Model *in) {
  nodes.Size((size_t)in->sizez);
  size.Set(in->sizex, in->sizey, in->sizez);
  for (int i = 0; i < (int)nodes.length; i++) nodes[i].Size((size_t)in->sizex, (size_t)in->sizey);
  for (int i = 0; i < in->numVoxels; i++) {
   MV_Voxel *v = &in->voxels[i];
   if (v) {
    if (nodes.inBounds((int)v->z)) {
     ZIndexed<MoxelNode> *plane = &nodes((int)v->z);
     if ( plane && (*plane).inBounds((int)v->x, (int)v->y) ) {
      MoxelNode *node = &(*plane)(v->x, v->y);
      if (node) {
       node->position.Set(v->x, v->y, v->z);
       node->filled = true;
       node->color.Int(
        in->palette[v->colorIndex].r,
        in->palette[v->colorIndex].g,
        in->palette[v->colorIndex].b,
        in->palette[v->colorIndex].a
       );
      }
     }
    }
   }
  }
  OUTPUT("Moxel:Import(%s) Magical Voxel converted to a Moxel (WxHxD) ~ %d x %d x %d\n", in->filename.c_str(), (int) size.x, (int) size.y, (int) size.z);
  FindTops();
 }
 void FindTops() {
  for (int i = 0; i < (int)size.x; i++) {
   for (int j = 0; j < (int)size.y; j++) {
    for (int k = (int)size.z - 1; k >= 0; k--) {
     if (nodes.inBounds(k)) {
      ZIndexed<MoxelNode> *plane = &nodes(k);
      if (plane && (*plane).inBounds(i, j)) {
       MoxelNode *node = &(*plane)(i, j);
       if (node && node->filled) { tops.Add(node); break; }
      }
     }
    }
   }
  }
 }
 CPUImage *TopDownHeightMap() {
  CPUImage *i = new CPUImage((int)size.x, (int)size.y);
  Crayon c(clear);
  for (int x = 0; x < (int)i->getWidth(); x++) for (int y = 0; y < (int)i->getHeight(); i++) {
   i->SetPixel(x, y, &c);
  }
  EACH(tops.first, MoxelNodeHandle, mnh) {
   double intensity = mnh->p->position.z / size.z;
   c.Double(intensity,intensity,intensity,1.0);
   i->SetPixel((int)mnh->p->position.x, (int)mnh->p->position.y,&c);
  }
  return i;
 }
 CPUImage *TopDownColorMap() {
  CPUImage *i = new CPUImage((int)size.x, (int)size.y);
  Crayon c(clear);
  for (int x = 0; x < (int)i->getWidth(); x++) for (int y = 0; y < (int)i->getHeight(); i++) {
   i->SetPixel(x, y, &c);
  }
  EACH(tops.first, MoxelNodeHandle, mnh) {
   i->SetPixel((int)mnh->p->position.x, (int)mnh->p->position.y,&mnh->p->color);
  }
  return i;
 }
 CPUImage *LayerColorMap( int z ) {
  CPUImage *i = new CPUImage((int)size.x, (int)size.y);
  Crayon c(clear);
  for (int x = 0; x < (int)i->getWidth(); x++) for (int y = 0; y < (int)i->getHeight(); y++) {
   i->SetPixel(x, y, &c);
  }
  if (nodes.inBounds(z)) {
   ZIndexed<MoxelNode> *plane = &nodes(z);
   MoxelNode *v = nullptr;
   for (int x = 0; x < (int)i->getWidth(); x++) for (int y = 0; y < (int)i->getHeight(); y++) {
    v = &(*plane)(x, y);
    if (v && v->filled) {
     i->SetPixel(x, y, &v->color);
    }
   }
  }
  return i;
 }
 CPUImage *PackedLayerStrip( int start, int end ) {
  CPUImage *combined = new CPUImage((int)size.x*(end-start), (int)size.y);
  int sx = 0;
  for (int z = start; z < end; z++) {
   CPUImage *tile = LayerColorMap(z);
   combined->Blit(tile, sx, 0);
   delete tile;
   sx += (int)size.x;
  }
  return combined;
 }

 CPUImage *PackedLayers() {
  int totalWidth = (int)(size.x*size.z);
  int totalRows = 1;
  if (totalWidth > MAX_STRIP_WIDTH) {
   totalRows = totalWidth / MAX_STRIP_WIDTH + 1;
   totalWidth = MAX_STRIP_WIDTH;
  }
  if (totalRows == 1) return PackedLayerStrip(0, (int)size.z);
  CPUImage *combined = new CPUImage(totalWidth, (int)size.y*totalRows);
  int stride = MAX_STRIP_WIDTH / (int)size.x;
  int si = 0;
  int sy = 0;
  for (int i = 0; i < totalRows; i++) {
   CPUImage *layer = PackedLayerStrip(si, si + stride);
   combined->Blit(layer, 0, sy);
   delete layer;
   si += stride;
   sy += (int)size.y;
  }
  return combined;
 }
MANY_(Moxel, {})
DONE(Moxel);
