#pragma once
#include "GLWindow.h"
#include "Atlas.h"
#include "FastGUI.h"
#include "AtlasAssembler.h"
#include "AtlasImporter.h"
#include "ImageSlicer.h"
#include "AtlasInspector.h"
#include "AtlasTileEditor.h"
#include "AtlasTilesetsPNGExporter.h"

class AtlasLibraryBrowser : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<Workspace> workspace;
 Zint scrolled;
 Zp<Atlas> atlas;
 Cartesian clickedSp0t;
 Zint col, buttonHeightSm, buttonHeightBig, tfontSize, smidge;
 void OnLoad() {
  Fullscreen();
  background.Int(32,16,24,255);
  col = w < 1900 ? 256 : 384;
  buttonHeightSm = w < 1900 ? 20 : 32;
  buttonHeightBig = w < 1900 ? 24 : 48;
  tfontSize = w < 1900 ? 12 : 16;
  smidge = w < 1900 ? 4 : 8;
 }
 bool ShowAtlasInfo( Atlas *a, int sx, int sy ) {
  if ( !a ) return false;
  int tilesets=0;
  EACH(workspace->workspaceAtlasTilesets.first,AtlasTilesetHandle,atsh)
   if ( atsh->p->atlas == a || !str_cmp(atsh->p->atlasName.c_str(),a->name.c_str()) )
    tilesets++;
  Cartesian area(sx,sy,w/2,col);
  fast.SaveState();
  fast.tfont.Set(tfontSize,tfontSize);
  if ( fast.button(digicaps,"Inspect and Edit",sx+col,sy+ col /2, col,buttonHeightSm) ) {
   SelectAtlas(a);
   return true;
  }
  if ( fast.button(digicaps,"Slice This Atlas",sx+ col,sy+ col /2+ buttonHeightSm+smidge, col, buttonHeightSm) ) {
   SliceAtlas(a);
   return true;
  }
  if ( tilesets > 0 ) {
   if ( fast.button(digicaps,"Export Tilesets",sx+ col,sy+ col /2+(buttonHeightSm + smidge)*2, col, buttonHeightSm) ) {
    ExportTilesets(a);
    return true;
   }
  } else fast.dbutton(digicaps,"Export Tilesets",sx+ col,sy+ col /2+ (buttonHeightSm + smidge) * 2, col, buttonHeightSm);
  if ( fast.button(digicaps,"Create Tileset",sx+ col,sy+ col /2+ (buttonHeightSm + smidge) * 3, col, buttonHeightSm) ) {
   AtlasTileset *at=new AtlasTileset;
   at->name=I2S(atlasTilesets.count);
   at->atlas=a;
   atlasTilesets.Append(at);
   workspace->workspaceAtlasTilesets.Add(at);
   AtlasTileEditor *ate=new AtlasTileEditor;
   ate->back_to=back_to;
   ate->workspace=workspace;
   ate->SetAtlas(a);
   ate->SetEditing(at);
   windows.Add(ate);
   this->destroyMe=true;
   fast.RecallState();
   return true;
  }
  fast.RecallState();
  fast.SaveState();
  fast.tfont.Set(tfontSize,tfontSize);
  fast.ljust(termFont,FORMAT(
    "Atlas: %s\n"
    "Texture Image:\n%s\n"
   "Normal Map:\n%s\n"
   "Heightmap Name:\n%s\n"
   "%dx%d cells, with %d tilesets",
    a->name.c_str(),
    a->textureName.c_str(),
    a->normalName.length>0?a->normalName.c_str():"(none)",
    a->heightName.length>0?a->heightName.c_str():"(none)",
   (int)a->columns, (int)a->rows,
    tilesets
   ).c_str(),
   sx+col, sy+smidge, -5
  );
  if ( !a->texture ) a->texture=library.Load(a->textureName.c_str());
  if ( a->texture ) a->RenderPreview(sx+ buttonHeightSm / 4,sy+ buttonHeightSm / 4, col -buttonHeightSm/2, col -buttonHeightSm/2);
  return false;
 }
 void ScrollToEnd() {
  scrolled=workspace->workspaceAtlasHandles.count-4;
  if ( scrolled < 0 ) scrolled=0;
 }
 void Render() {
  if ( !visible ) return;
  fast.DefaultState();
  fast.ResponsiveRatio(w, h);
  if ( atlas ) Inspector();
  int i=0, drawn=0;
  int sy=buttonHeightBig+smidge*2;
  int sx=0;
  if ( scrolled > 0 && fast.button(fast.upArrow,w/2-buttonHeightBig/2,smidge,buttonHeightBig,buttonHeightBig) ) scrolled-=2;
  if ( scrolled < atlasLibrary.count-3 && atlasLibrary.count > 4 && fast.button(fast.downArrow,w/2-buttonHeightBig/2,buttonHeightBig+smidge+col*2+smidge, buttonHeightBig, buttonHeightBig) ) scrolled+=2;
  EACH(workspace->workspaceAtlasHandles.first,AtlasHandle,ah) {
   if ( i < scrolled ) { i++; continue; }
   Atlas *a=ah->p;
   if ( ShowAtlasInfo(a,sx,sy) ) break;
   drawn++;
   if ( sx == 0 ) sx=w/2;
   else {
    sx=0;
    sy+=col.value;
   }
   i++;
   if ( drawn >= 4 ) break;
  }
  if ( input->wheelUp > 0 ) scrolled-=2;
  if ( input->wheelDown > 0 ) scrolled+=2;
  if ( scrolled < 0 ) scrolled=0;
  if ( scrolled > atlasLibrary.count-3 ) scrolled=atlasLibrary.count-3;
  fast.SaveState();
  fast.tfont.Set(tfontSize,tfontSize);
  if ( fast.button(digicaps,"ASSEMBLE AND IMPORT",smidge,h-(buttonHeightBig+smidge),col,buttonHeightBig) ) {
   AtlasAssembler *aa=new AtlasAssembler;
   aa->back_to=this;
   aa->workspace = workspace;
   aa->atlasFilename += this->atlas ? ( this->atlas->name + string(".png") ) : FMT("atlas%d.png", (int) (workspace ? workspace->workspaceAtlasHandles.count.value+1 : 1) );
   aa->atlasName = this->atlas ? this->atlas->name : FMT("atlas%d", (int) ( workspace ? workspace->workspaceAtlasHandles.count.value+1 : 1 ) );
   this->visible=false;
   windows.Add(aa);
   return;
  }
  if ( fast.button(digicaps,"IMPORT SPRITESHEET",smidge+col+smidge,h-(buttonHeightBig+smidge),col,buttonHeightBig) ) {
   AtlasImporter *ai=new AtlasImporter;
   ai->back_to=this;
   ai->workspace=workspace;
   this->visible=false;
   windows.Add(ai);
   return;
  }
  if ( fast.button(digicaps,"IMAGE SLICER",smidge*3+col*2,h-(buttonHeightBig+smidge),(col*2)/3,buttonHeightBig) ) {
   ImageSlicer *is=new ImageSlicer;
   is->back_to=this;
   this->visible=false;
   windows.Add(is);
   return;
  }
  if ( clickedSp0t.x != input->mxi && clickedSp0t.y != input->myi 
   && fast.button(digicaps,"IMAGE UTILITY FOLDER", smidge * 4 + col * 2 + ((col*2)/3),h-(buttonHeightBig+smidge),col+buttonHeightBig, buttonHeightBig) ) {
   clickedSp0t.Set(input->mxi,input->myi);
   open_explorer((specialPaths.data_folder.value+string("utilities\\README.txt")).c_str());
   open_explorer((specialPaths.data_folder.value+string("utilities")).c_str());
   fast.ignoreNextRelease=1.0;
  }
  if (fast.button(digicaps, "EXPORT", w-(col/2)*2-smidge*2, h - (buttonHeightBig + smidge), (col / 2), buttonHeightBig)) {
   AtlasTilesetsPNGExporter *atsp = new AtlasTilesetsPNGExporter;
   atsp->back_to = this;
   atsp->workspace = workspace;
   this->visible = false;
   windows.Add(atsp);
   return;
  }
  if ( fast.button(digicaps,"GO BACK",w-(col/2)-smidge,h-(buttonHeightBig+smidge),col/2,buttonHeightBig) && back_to ) {
   back_to->visible=true;
   this->destroyMe=true;
   fast.RecallState();
   return;
  }
  fast.RecallState();
 }
 void SliceAtlas( Atlas *a ) {
  if ( !a || !a->texture ) return;
  ImageSlicer *is=new ImageSlicer;
  is->back_to=this;
  this->visible=false;
  windows.Add(is);
  is->loaded=a->texture;
  is->params.w=(double)a->columns;
  is->params.h=(double)a->rows;
  is->params.x=(double)a->texture->width/is->params.w;
  is->params.y=(double)a->texture->height/is->params.h;
 }
 void ExportTilesets( Atlas *a ) {
  AtlasTilesetsPNGExporter *atsp=new AtlasTilesetsPNGExporter;
  atsp->back_to=this;
  atsp->atlasOnly=a;
  atsp->workspace=workspace;
  this->visible=false;
  windows.Add(atsp);
 }
 void SelectAtlas( Atlas *a ) {
  atlas=a;
  AtlasInspector *ai=new AtlasInspector;
  ai->back_to=this;
  ai->atlas=a;
  this->visible=false;
  windows.Add(ai);
 }
 void Inspector() {
 }
};

