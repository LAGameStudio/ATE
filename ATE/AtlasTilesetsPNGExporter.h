#pragma once
#include "GLWindow.h"
#include "Workspace.h"
#include "Atlas.h"
#include "AtlasTileset.h"
#include "FastGUI.h"
#include "Art2d.h"

extern Zbool dontTrim;
extern Zint exporter_size_x,exporter_size_y;
extern Zint exporter_pad_x,exporter_pad_y;

class AtlasTilesetsPNGExporter : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<Workspace> workspace;
 Zp<Atlas> atlasOnly;
 ZIndexed<Zbool> included;
 Zint scrolled;
 Zstring targetPath,filenameExample;
 Zbool usePrefixFromTilesetName, usePrefixFromAtlasName, usePrefixFromTileName, useNoPrefix;
 Zstring status;
 Zbool openFolderAtEnd;
 void OnLoad() {
  Fullscreen();
  background.Int(48,32,8,255);
  included.Size(workspace->workspaceAtlasTilesets.count);
  fast.ignoreNextRelease=1.0;
  openFolderAtEnd=true;
 }
 int IncludedTilesets() { int total=0; for ( int i=0; i<(int)included.length; i++ ) if ( included[i] ) total++; return total; }
 int IncludedTiles() {
  int total=0;
  AtlasTilesetHandle *ath=(AtlasTilesetHandle *) workspace->workspaceAtlasTilesets.first;
  for ( int i=0; i<(int)included.length; i++ ) {
   if ( included[i] ) total += ath->p->tiles.count.value;
   ath=(AtlasTilesetHandle *)ath->next;
  }
  return total;
 }
 int RenderAtlasToggle( int sx, int sy, AtlasTileset *a, int i ) {
   included[i]=fast.toggle(FORMAT("ATLAS: %-20s   TILESET: %-20s   TILES: %d",a->atlasName.c_str(),a->name.c_str(),a->tiles.count.value).c_str(),included[i],sx,sy,24,24);
  int tx=sx+32;
  int ty=sy+32;
  EACH(a->tiles.first,AtlasTile,t) if ( a->atlas ) {
   t->Render(crayons.Pick(alabaster),a->atlas,(double)tx,(double)ty,32.0,32.0);
   tx+=32; if ( tx > w-64 ) { ty+=32; tx=sx+32; }
  }
  return ty+32+16;
 }
 void RenderAtlasToggler() {
  int i=0;
  int sy=16+32+8+64+8;
  bool more=false;
  EACH(workspace->workspaceAtlasTilesets.first,AtlasTilesetHandle,ath) {
   if ( atlasOnly && atlasOnly != ath->p->atlas ) { i++; continue; }
   if ( i < scrolled ) { i++; continue; }
   sy=RenderAtlasToggle(16,sy,ath->p,i);
   if ( sy > h-192 ) { more=true; break; }
   i++;
  }
  if ( input->wheelUp ) {
   scrolled++;
   if ( scrolled > included.length-3 ) scrolled=(int)included.length-3;
   if ( scrolled < 0 ) scrolled=0;
  }
  if ( input->wheelDown ) {
   scrolled--;
   if ( scrolled < 0 ) scrolled=0;
  }
  if ( scrolled > 0 ) {
   if ( fast.button(fast.upArrow,w/2,16+64+8,32,32) ) {
    scrolled--;
    if ( scrolled < 0 ) scrolled=0;
   }
  }
  if ( more ) {
   if ( fast.button(fast.downArrow,w/2,h-128+16,32,32) ) {
    scrolled++;
    if ( scrolled > included.length-3 ) scrolled=(int)included.length-3;
    if ( scrolled < 0 ) scrolled=0;
   }
  }
 }
 void Render() {
  if ( !visible ) return;
  if ( exporting ) { Exporting(); return; }
  fast.ResponsiveRatio(w,h);
  if ( fast.button(digicaps,"Pick Output Folder",16,16,384,48) ) {
   targetPath=dialog_get_folder("Choose a target folder to output all of the exported tiles to.","Select folder directory from below:",true);
   targetPath+="\\";
  }
  if ( targetPath.length > 0 ) fast.ljust(termFont,(string("Outputting files to:\n")+targetPath).c_str(),16+384+16,16,-4);
  Cartesian pen(16,16+64);
  fast.text("Tile Size",(int)pen.x,(int)pen.y);
  pen.x+=192;
  exporter_size_x=fast.integer("X output size",exporter_size_x,1,1024,1,(int)pen.x,(int)pen.y,64,24,5,true);
  pen.x+=192;
  exporter_size_y=fast.integer("Y output size",exporter_size_y,1,1024,1,(int)pen.x,(int)pen.y,64,24,5,true);
  pen.x=w/2+96;
  fast.text("Padding",(int)pen.x,(int)pen.y);
  pen.x+=192;
  exporter_pad_y=fast.integer("X pad size",exporter_pad_x,0,exporter_size_x/2,1,(int)pen.x,(int)pen.y,64,24,5,true);
  pen.x+=192;
  exporter_pad_y=fast.integer("Y pad size",exporter_pad_y,0,exporter_size_y/2,1,(int)pen.x,(int)pen.y,64,24,5,true);
  RenderAtlasToggler();
  fast.boxedtextrjust(FORMAT("EXPORT %d TILESETS WITH %d TILES TOTAL @ %d x %d px\nWILL OVERWRITE EXISTING\nFILENAME FORMAT: %s",
   IncludedTilesets(),
   IncludedTiles(),
   (int)(exporter_size_x),
   (int)(exporter_size_y),
   filenameExample.c_str()
  ).c_str(),w-16,h-32,false,crayons.Pick(green255));
  art.Rectangle(crayons.Pick(black),none,(double)w-192.0,(double)h-192.0-32.0,192.0-8.0,192.0-8.0);
  fast.ljust(digicaps,"Filename Prefix\nOptions",w-192+2,h-192+8-32,12,14,0,3,crayons.Pick(alabaster));
  useNoPrefix=!usePrefixFromAtlasName && !usePrefixFromTilesetName && !usePrefixFromTileName;
  useNoPrefix=fast.toggle("Number Only",useNoPrefix,w-192+8,h-192+24,24,24);
  if ( useNoPrefix ) { usePrefixFromAtlasName=false; usePrefixFromTilesetName=false; usePrefixFromTileName=false; }
  usePrefixFromAtlasName=fast.toggle("Atlas Name",usePrefixFromAtlasName,w-192+8,h-192+32+24,24,24);
  if ( usePrefixFromAtlasName ) useNoPrefix=false; 
  else if ( !usePrefixFromTilesetName && !usePrefixFromTileName ) useNoPrefix=true;
  usePrefixFromTilesetName=fast.toggle("Tileset Name",usePrefixFromTilesetName,w-192+8,h-192+64+24,24,24);
  if ( usePrefixFromTilesetName ) useNoPrefix=false; 
  else if ( !usePrefixFromAtlasName && !usePrefixFromTileName ) useNoPrefix=true;
  usePrefixFromTileName=fast.toggle("Tile Name",usePrefixFromTileName,w-192+8,h-192+64+32+24,24,24);
  if ( usePrefixFromTileName ) useNoPrefix=false; 
  else if ( !usePrefixFromAtlasName && !usePrefixFromTilesetName ) useNoPrefix=true;
  filenameExample="#";
  if ( usePrefixFromAtlasName ) filenameExample+="_atlas";
  if ( usePrefixFromTilesetName ) filenameExample+="_tileset";
  if ( usePrefixFromTileName ) filenameExample+="_tile";
  filenameExample += ".PNG";
  if ( targetPath.length == 0 || !folder_exists(targetPath.c_str()) ) fast.dbutton(digicaps,"EXPORT PNG TILES",16,h-64,256+32,48);
  else if ( fast.button(digicaps,"EXPORT PNG TILES",16,h-64,256+32,48) ) {
   ExportNow();
   return;
  }
  if ( fast.button(digicaps,"Go Back",384,h-48,192,32) ) {
   ReturnTo();
   return;
  }
  fast.ljust(digicaps,status.c_str(),16,h-48-24-2,16,16,0,3,crayons.Pick(gray),additive);
 }
 void ExportNow() {
  exporting=true;
  exportNumber=0;
  exportCounter=0;
  exportTileNumber=0;
  isExporting=null;
 }
 Zstring fn, fn_hm, fn_nm, fn_nt, fn_ht;
 void ExportTile( AtlasTileset *tileset, AtlasTile *tile ) {
  Zstring png_exporter_path=targetPath;
  Atlas *atlas=tileset->atlas;
  if ( !atlas ) { exportCounter++; return; }
  Crayon tint(alabaster);
  Zstring filenameActual=I2S(exportCounter);
  exportCounter++;
  if ( usePrefixFromAtlasName ) filenameActual+=string("_")+atlas->name;
  if ( usePrefixFromTilesetName ) filenameActual+=string("_")+tileset->name;
  if ( usePrefixFromTileName ) filenameActual+=string("_")+tile->name;
  const char *fnp=filenameActual.c_str();
  if ( !folder_exists(png_exporter_path) ) make_folder(png_exporter_path.c_str());
  fn=png_exporter_path+string("\\")+string(fnp)+string(".png");
  fn_hm=png_exporter_path+string("\\")+string(fnp)+string("_hm.png");
  fn_nm=png_exporter_path+string("\\")+string(fnp)+string("_nm.png");
  fn_ht=png_exporter_path+string("\\")+string(fnp)+string("_ah.png");
  fn_nt=png_exporter_path+string("\\")+string(fnp)+string("_an.png");

  Cartesiand outArea;
  outArea.SetRect(0.0,0.0,
   exporter_size_x-(exporter_pad_x*2),
   exporter_size_y-(exporter_pad_y*2)
  );
  // Strange adjustments required.. maybe due to slightly off viewport settings.
  outArea.SetRect(0.0,0.0,
   outArea.w-9.0/128.0*outArea.w,
   (outArea.h+(2.0/128.0*outArea.h))/2.0
  );
  outArea.SetRect(exporter_pad_x*2.0,exporter_pad_y*2.0,outArea.w*2.0,outArea.h*2.0);

  Cartesian xArea;
  xArea.SetRect(
   0,0,(int)exporter_size_x*2,(int)exporter_size_y*2
  );

  CPUImage *c=null,*d=null,*e=null;

  fboCDS2048.background.Pick(clear);
  fboCDS2048.Bind();
  tile->Render(tint,atlas,outArea.x,outArea.y,outArea.w,outArea.h);
  fboCDS2048.Unbind();
  fboCDS2048.BindWithoutClearing();
  c=fboCDS2048.Snapshot();
  fboCDS2048.Unbind();
  d=c->FlipV();
  delete c; c=null;
  e=d->Crop((int)xArea.x,(int)xArea.y,(int)xArea.w,(int)xArea.h);
  delete d; d=null;
  d=e->Half();
  delete e; e=null;
  d->Save(fn.c_str());
  delete d; d=null;

  if ( atlas->normal ) {
   fboCDS2048.background.Pick(clear);
   fboCDS2048.Bind();
   tile->RenderNormal(tint,atlas,outArea.x,outArea.y,outArea.w,outArea.h);
   fboCDS2048.Unbind();
   fboCDS2048.BindWithoutClearing();
   c=fboCDS2048.Snapshot();
   fboCDS2048.Unbind();
   d=c->FlipV();
   delete c; c=null;
   e=d->Crop((int)xArea.x,(int)xArea.y,(int)xArea.w,(int)xArea.h);
   delete d; d=null;
   d=e->Half();
   delete e; e=null;
   d->Save(fn_nt.c_str());
   delete d; d=null;
  }
  
  if ( atlas->height ) {
   fboCDS2048.background.Pick(clear);
   fboCDS2048.Bind();
   tile->RenderHeight(tint,atlas,outArea.x,outArea.y,outArea.w,outArea.h);
   fboCDS2048.Unbind();
   fboCDS2048.BindWithoutClearing();
   c=fboCDS2048.Snapshot();
   fboCDS2048.Unbind();
   d=c->FlipV();
   delete c; c=null;
   e=d->Crop((int)xArea.x,(int)xArea.y,(int)xArea.w,(int)xArea.h);
   delete d; d=null;
   d=e->Half();
   delete e; e=null;
   d->Save(fn_ht.c_str());
   delete d; d=null;
  }

  if ( tile->at && tile->ats ) {
   glFinish();
   fboCDS2048.background.Pick(clear);
   fboCDS2048.Bind();
   tile->at->Render(tint,tile->ats->atlas,outArea.x,outArea.y,outArea.w,outArea.h);
   fboCDS2048.Unbind();
   fboCDS2048.BindWithoutClearing();
   c=fboCDS2048.Snapshot();
   fboCDS2048.Unbind();
   d=c->FlipV();
   delete c; c=null;
   e=d->Crop((int)xArea.x,(int)xArea.y,(int)xArea.w,(int)xArea.h);
   delete d; d=null;
   d=e->Half();
   delete e; e=null;
   d->Save(fn_hm.c_str());
   e=d->HeightToNormalMap();
   delete d; d=null;
   d=e->FlipV();
   delete e; e=null;
   d->Save(fn_nm.c_str());
   delete d; d=null;
  }
 }
 Zint exportNumber,exportTileNumber;
 Zp<AtlasTileset> isExporting;
 Zbool exporting;
 Zint exportCounter;
 void Exporting() {
  openFolderAtEnd=fast.toggle("",openFolderAtEnd,16,h-128,24,24) || input->KeyDown(DX_Y);
  fast.ljust(digicaps,"OPEN FOLDER WHEN COMPLETE (HOLD Y=YES, N=NO)",16+24+8,h-128+8,16,16,0,3,crayons.Pick(orange));
  if ( input->KeyDown(DX_N) ) openFolderAtEnd=false; 
  if ( fast.button(digicaps,"ABORT",16,h-96,128,32) || input->KeyDown(DX_ESCAPE) ) {
   exporting=false;
   isExporting=null;
   status="Last export was aborted.";
   fast.ignoreNextRelease=1.0;
   return;
  }
  fast.ljust(digicaps,"HOLD ESCAPE TO ABORT THE EXPORT PROCESS\nALREADY EXPORTED FILES WILL NOT BE DELETED",16,h-48,16,16,0,3,crayons.Pick(orange));
  if ( !isExporting ) {
  fast.text(digicaps,"FINDING NEXT TILESET",w/2,h/2,24,24);
   if ( exportNumber < included.length && included[(int)exportNumber].value ) {
    AtlasTilesetHandle *ath=(AtlasTilesetHandle *)workspace->workspaceAtlasTilesets.Element(exportNumber);
    if ( ath && ath->p ) {
     isExporting=ath->p;
     exportTileNumber=0;
    }
   } else exportNumber++;
  } else {
   AtlasTile *exportTile=(AtlasTile *)isExporting->tiles.Element(exportTileNumber);
   if ( exportTile ) {
    ExportTile(isExporting,exportTile);
    exportTile->Render(crayons.Pick(alabaster),isExporting->atlas,16.0,16.0,512.0,512.0);
    exportTileNumber++;
    fast.ljust(termFont,FORMAT("Exported %d of %d:\n\n%s\n%s\n%s\n%s\n%s",
      (int)(exportCounter+1),
      IncludedTiles(),
      fn.c_str(), 
      isExporting->atlas->height ? fn_ht.c_str() : "",
      isExporting->atlas->normal ? fn_nt.c_str() : "",
      exportTile->at && exportTile->at->ats ? fn_hm.c_str() : "",
      exportTile->at && exportTile->at->ats ? fn_nm.c_str() : ""
     ).c_str(),16,16+512+16,-4
    );
    fast.ljust(termFont,FORMAT(
      "Atlas:   %s\n"
      "Tileset: %s\n"
      "Tile:    %s\n"
      "Atlas Height? %s\n"
      "Atlas Normal? %s\n"
      "Height Tile? %s\n"
      "Normal Tile? %s\n"
      "Output Padding:    %4d      by %4d border (interior pad)\n"
      "Output Image Size: %4d wide by %4d tall (pixels)\n",
      isExporting->atlasName.c_str(),
      isExporting->name.c_str(),
      exportTile->name.c_str(),
      isExporting->atlas->height ? "YES" : "NO ",
      isExporting->atlas->normal ? "YES" : "NO ",
      exportTile->at && exportTile->at->ats ? "YES" : "NO ",
      exportTile->at && exportTile->at->ats ? "YES" : "NO ",
      (int)exporter_pad_x,
      (int)exporter_pad_y,
      (int)exporter_size_x,
      (int)exporter_size_y
     ).c_str(),
     16+512+16,16,-4
    );
   } else {
    isExporting=null;
    exportNumber+=1;
   }
  }
  if ( exportNumber >= (int) included.length ) {
   exporting=false;
   exportNumber=0;
   exportCounter=0;
   status="Last export was completed.";
   if ( openFolderAtEnd ) open_explorer(targetPath.c_str());
   return;
  }
 }
 void ReturnTo() {
  back_to->visible=true;
  this->destroyMe=true;
 }
};

