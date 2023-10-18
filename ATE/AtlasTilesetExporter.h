#pragma once

#include "Atlas.h"
#include "AtlasTileset.h"
#include "GLWindow.h"
#include "FastGUI.h"

extern Zstring png_exporter_path;

class AtlasTilesetExporter : public GLWindow {
public:
 Zp<Atlas> atlas;
 Zp<AtlasTileset> tileset;
 Zp<GLWindow> back_to;
 Zbool outputAtlasToo,outputIndividualTiles,zipIt;
 Zstring prefix;
 Zstring fn_png,fn_png_hm,fn_png_nm,fn_txt,fn_json,fn_atlas_json,zipFilename;
 Zdouble ignoreNext;
 void OnLoad() {
  Fullscreen();
  background.Int(0,48,0,255);
  prefix=atlas->name;
  if ( png_exporter_path.length == 0 ) {
   png_exporter_path=specialPaths.saved_documents+string("\\ATE\\");
   if ( !folder_exists(png_exporter_path) ) make_folder(png_exporter_path.c_str());
  }
 }
 void Output() {
  if ( !folder_exists(png_exporter_path) ) make_folder(png_exporter_path.c_str());
  if ( outputAtlasToo ) {
   string_as_file((string("{\n")+atlas->toJSON()+string("}\n")).c_str(),fn_atlas_json.c_str());
   CopyFile(atlas->texture->filename.c_str(),fn_png.c_str(),FALSE);
  }
  string_as_file((string("tileset {\n")+tileset->toString()+string("}\n")).c_str(),fn_txt.c_str());
  string_as_file((string("{\n")+tileset->toJSON()+string("}\n")).c_str(),fn_json.c_str());
 }
 void Render() {
  if ( atlas && atlas->texture ) art.Stretch(atlas->texture,crayons.Pick(alabaster),256,32,512,512);
  Cartesiand tiles(256+512+32,32,512,512);
  Cartesiand tile(64,64);
  int i=0;
  double sx=tiles.x;
  double sy=tiles.y;
  if ( atlas ) EACH(tileset->tiles.first,AtlasTile,t) {
   t->Render(crayons.Pick(alabaster),atlas,(double)sx,(double)sy,(double)tile.x,(double)tile.y);
   sx+=tile.x+2.0;
   if ( sx > tiles.x2 ) {
    sx=tiles.x;
    sy+=tile.y;
   }
  }
  png_exporter_path=fast.textbox(termFont,additive,calibri,additive,"\nOUTPUT FOLDER\n\n(must end in \\)",png_exporter_path,32,32+512+32,80);
  prefix=fast.textbox(termFont,additive,calibri,additive,"\nFILENAME PREFIX",prefix,(int)32,32+512+128,80);
  zipFilename=png_exporter_path+prefix+string(".ATE.zip");
  fn_png       =(zipIt?string(""):png_exporter_path)+prefix+string(".atlas.png");
  fn_png_hm    =(zipIt?string(""):png_exporter_path)+prefix+string(".atlas_hm.png");
  fn_png_nm    =(zipIt?string(""):png_exporter_path)+prefix+string(".atlas_nm.png");
  fn_txt       =(zipIt?string(""):png_exporter_path)+prefix+string(".tileset.txt");
  fn_json      =(zipIt?string(""):png_exporter_path)+prefix+string(".tiles.json");
  fn_atlas_json=(zipIt?string(""):png_exporter_path)+prefix+string(".atlas.json"); 
  outputAtlasToo=fast.toggle("ATLAS TOO",outputAtlasToo,32,h-384+64,32,32);
  outputIndividualTiles=fast.toggle("ALL TILES FROM THIS TILESET",outputIndividualTiles,32,h-384+48+64,32,32);
  zipIt=fast.toggle("ZIP IT",zipIt,32,h-384+48+48+64,32,32);
  bool
    valid_png   =file_exists(atlas->textureName.c_str()),
    valid_png_hm=file_exists(atlas->heightName.c_str()) ,
    valid_png_nm=file_exists(atlas->normalName.c_str()) ;
  fast.SaveState();
  fast.tfont.Set(24,24);
  if ( zipIt ) fast.ljust(calibri,FORMAT("INSIDE ARCHIVE:\n%s",zipFilename.c_str()).c_str(),512,h-384+80);
  if ( outputAtlasToo ) {
   fast.ljust(calibri,FORMAT("EXPORTING:\n%s\n%s\n%s\n%s\n%s",
    valid_png    ? fn_png.c_str()    : "",
    valid_png_hm ? fn_png_hm.c_str() : "", 
    valid_png_nm ? fn_png_nm.c_str() : "",
    fn_txt.c_str(),fn_json.c_str(),fn_atlas_json.c_str()).c_str(),
    512,h-192
   );
  } else {
   fast.ljust(calibri,FORMAT("EXPORTING:\n%s\n%s",fn_txt.c_str(),fn_json.c_str()).c_str(),512,h-192);
  }
  if ( outputIndividualTiles ) {
   fast.ljust(calibri,
    FORMAT("and %s tiles with filenames like %s.tile-0.png %s.tile_hm-0.png and %s.tile_nm-0.png ",
     tileset->tiles.count.toString().c_str(),
     prefix.c_str(),
     prefix.c_str(),
     prefix.c_str()
    ).c_str(),512,h-32
   );
  }
  fast.RecallState();
  if ( fast.button(digicaps,"Export Now",32,h-128,192,48) ) {
   if ( file_exists(fn_png.c_str()) ) {
   }
   if ( outputAtlasToo ) {
    if ( valid_png    ) CopyFile(atlas->textureName.c_str(),fn_png.c_str(),   FALSE);
    if ( valid_png_hm ) CopyFile(atlas->heightName.c_str(), fn_png_hm.c_str(),FALSE);
    if ( valid_png_nm ) CopyFile(atlas->normalName.c_str(), fn_png_nm.c_str(),FALSE);
   }
   Output();
   ReturnTo();
  }
  if ( fast.button(digicaps,"Cancel",32,h-64,128,32) ) {
   ReturnTo();
  }
 }
 void ReturnTo() {
  back_to->visible=true;
  this->destroyMe=true;
 }
};