#pragma once
#include "FastGUI.h"
#include "Atlas.h"
#include "Workspace.h"

class AtlasImporter : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<GLImage> texture,heightMap,normalMap;
 Zp<Workspace> workspace;
 Zstring baseName,prefix;
 Zstring imageFilename;
 Cartesiand params;  // x,y=col,row w,h=cellSize
 Zbool nearest,mirrored,saveAtlasInWorkspace;
 void OnLoad() {
  Fullscreen();
  background.Int(32,16,24,255);
  saveAtlasInWorkspace=true;
  nearest=true;
  mirrored=true;
 }
 void Render() {
  if ( !visible ) return;
  Cartesiand area(128.0,128.0,800.0,800.0);
  fast.DefaultState();
  fast.ResponsiveRatio(w,h);
  fast.SaveState();
  fast.tfont.Set(16,20);
  bool isEditingNumbers = fast.hoverarea(128+800+32,128,384,128);
  prefix=fast.textbox(termFont,additive,calibri,additive,"\nATLAS NAME",prefix,16+384+32,64,80,false,1,2,4,!isEditingNumbers);
  if ( fast.button(digicaps,"Pick Source Image",16,64,384,48) ) {
   imageFilename=dialog_open("Choose an image to import");
   if ( file_exists(imageFilename.c_str()) ) {
    baseName=basename(imageFilename.c_str());
    Strings pieces;
    pieces.SplitPush(baseName.c_str(),'.');
    if ( pieces.count < 1 ) prefix="";
    else prefix=pieces.Get(0);
    texture=library.Load(imageFilename.c_str());
    if ( texture ) params.SetRect(64.0,64.0,(double)texture->width/64.0,(double)texture->height/64.0);
   }
  }
  if ( texture ) {
   if ( fast.button(digicaps,"Exact Pixels", (int)area.x2+32+384,(int)area.y,232,32) ) {
    params.x=(int)params.x;
    params.y=(int)params.y;
    if ( params.x < 1.0 ) params.x=1.0;
    if ( params.y < 1.0 ) params.y=1.0;
    params.w=(double)texture->width/params.x;
    params.h=(double)texture->height/params.y;
    if ( params.w < 1.0 ) params.w=1.0;
    if ( params.h < 1.0 ) params.h=1.0;
   }
   Cartesiand wasParams;
   wasParams.Set(&params);
   fast.xy(&params,(int)area.x2+32,(int)area.y,1.0,(double)texture->width,"Cells","WIDE","HIGH",12,true);
   if ( !fast.editing(params.x) && !fast.editing(params.y) 
    && ( params.x != wasParams.x || params.y != wasParams.y ) ) {
    if ( params.x < 1.0 ) params.x=1.0;
    if ( params.y < 1.0 ) params.y=1.0;
    params.w=(double)texture->width/params.x;
    params.h=(double)texture->height/params.y;
    if ( params.w < 1.0 ) params.w=1.0;
    if ( params.h < 1.0 ) params.h=1.0;
   }
   wasParams.Set(&params);
   fast.wh(&params,(int)area.x2+32,(int)area.y+64,1.0,10000.0,"Grid","COLUMNS","ROWS",5,true);
   if ( !fast.editing(params.w) && !fast.editing(params.h) 
    && ( params.w != wasParams.w || params.h != wasParams.h ) ) {
    params.w = (double)(int)params.w;
    params.h = (double)(int)params.h;
    if ( params.w < 1.0 ) params.w=1.0;
    if ( params.h < 1.0 ) params.h=1.0;
    params.x=(double)texture->width/params.w;
    params.y=(double)texture->height/params.h;
    if ( params.x < 1.0 ) params.x=1.0;
    if ( params.y < 1.0 ) params.y=1.0;
   }
   art.Stretch(texture,crayons.Pick(alabaster),transparency,area.x,area.y,area.w,area.h);
   DrawGrid(area.x,area.y,area.w,area.h,crayons.Pick(red255),(int)params.h,(int)params.w);
   fast.ljust(calibri, FORMAT("Image is %d by %d pixels tall",(int)texture->width,(int)texture->height).c_str(),(int)area.x,(int)area.y2+32);
   if ( (double)(int)params.x != params.x || (double)(int)params.y != params.y
     || (double)(int)params.w != params.w || (double)(int)params.h != params.h )
   fast.ljust( calibri,
    "Note that if grid columns # and rows # are not perfect whole numbers,\n"
    "they will be rounded down to the nearest integer.\n"
    "If the resulting cell size is also a non-perfect whole number,\n"
    "thereby meaning that the cell size shows not as an exact pixel value,\n"
    "then that may mean partial pixels are present at the edge of a grid cell.\n"
    "This is why padding could be used (of at least 1 pixel) between each sprite cell\n"
    "when sheet image dimensions will be sliced into grids that cannot be evenly divided.\n"
    "The unsliced sheet's image dimensions should precisely reflect the intended\n"
    "number of rows and columns for best results.\n", 
    (int)area.x2+32, (int)area.y+128
   );
   mirrored=fast.toggle("MIRRORED (4 way)",mirrored,(int)area.x2+32,(int)(area.y+area.w/2.0),24,24);
   nearest=fast.toggle("NEAREST NEIGHBOR (Retain Sharp Pixel Edges When Scaled)",nearest,(int)area.x2+32,(int)(area.y+area.w/2.0)+32,24,24);
   saveAtlasInWorkspace=fast.toggle("Copy Atlas Texture to Workspace Folder",saveAtlasInWorkspace,(int)area.x2+32,(int)(area.y+area.w/2.0)+64,24,24);
  }
  heightMap=fast.imagePicker("HEIGHT",heightMap,(int)area.x2+32,(int)(area.y+area.w/2.0)+128,256,256,24,true);
  normalMap=fast.imagePicker("NORMAL",normalMap,(int)area.x2+32+256+16,(int)(area.y+area.w/2.0)+128,256,256,24,true);
  if ( !texture ) fast.dbutton(digicaps,"IMPORT ATLAS",w-256-16,h-64-128,256,96);
  else if ( fast.button(digicaps,"IMPORT ATLAS",w-256-16,h-64-128,256,96) ) {
   ImportNow();
   return;
  }
  if ( fast.button(digicaps,"Go Back",w-192-16,h-64,192,48) ) {
   ReturnTo();
   return;
  }
//  if ( targetPath.length > 0 ) fast.ljust(termFont,(string("Outputting files to:\n")+targetPath).c_str(),16+384+16,128,-4);
  fast.RecallState();
 }
 void ImportNow() {
  Zstring wasLoadedTexture=texture->filename;
  Zstring textureName=workspace->path+string("atlases\\")+basename(wasLoadedTexture.c_str());
  {
   int tries=0;
   while ( CopyFile(texture->filename.c_str(),textureName.c_str(),TRUE) == 0 && tries < 100 ) {
    tries++;
    dialog_last_error((string("Copying `")+texture->filename+string("` to `")+textureName+string("`")).c_str());
    textureName=workspace->path+string("atlases\\")+I2S(tries)+basename(wasLoadedTexture.c_str());
   }
   if ( tries >= 100 ) return;
  }
  // Reload the texture using the provided information
  if ( file_exists(textureName.c_str()) ) {
   if ( mirrored ) {
    if ( nearest ) texture=library.Load(textureName.c_str(),tllto_Mirrored_Nearest);
    else texture=library.Load(textureName.c_str(),tllto_Mirrored);
   } else {
    if ( nearest ) texture=library.Load(textureName.c_str(),tllto_Mirrored_Nearest);
    else texture=library.Load(textureName.c_str(),tllto_Mirrored);
   }
  }
  if ( heightMap ) {
   Zstring wasLoadedHeight=heightMap->filename;
   Zstring heightName=workspace->path+string("atlases\\")+basename(wasLoadedHeight.c_str());
   {
    int tries=0;
    while ( CopyFile(heightMap->filename.c_str(),heightName.c_str(),TRUE) == 0 && tries < 100 ) {
     tries++;
     dialog_last_error((string("Copying `")+heightMap->filename+string("` to `")+heightName+string("`")).c_str());
     heightName=workspace->path+string("atlases\\")+I2S(tries)+basename(wasLoadedHeight.c_str());
    }
    if ( tries >= 100 ) return;
   }
   // Reload the texture using the provided information
   if ( file_exists(heightName.c_str()) ) {
    if ( mirrored ) {
     if ( nearest ) heightMap=library.Load(heightName.c_str(),tllto_Mirrored_Nearest);
     else heightMap=library.Load(heightName.c_str(),tllto_Mirrored);
    } else {
     if ( nearest ) heightMap=library.Load(heightName.c_str(),tllto_Mirrored_Nearest);
     else heightMap=library.Load(heightName.c_str(),tllto_Mirrored);
    }
   }
  }
  if ( normalMap ) {
   Zstring wasLoadedNormal=normalMap->filename;
   Zstring normalName=workspace->path+string("atlases\\")+basename(wasLoadedNormal.c_str());
   {
    int tries=0;
    while ( CopyFile(normalMap->filename.c_str(),normalName.c_str(),TRUE) == FALSE && tries < 100 ) {
     tries++;
     dialog_last_error((string("Copying `")+normalMap->filename+string("` to `")+normalName+string("`")).c_str());
     normalName=workspace->path+string("atlases\\")+I2S(tries)+basename(wasLoadedNormal.c_str());
    }
    if ( tries >= 100 ) return;
   }
   // Reload the texture using the provided information
   if ( file_exists(normalName.c_str()) ) {
    if ( mirrored ) {
     if ( nearest ) normalMap=library.Load(normalName.c_str(),tllto_Mirrored_Nearest);
     else normalMap=library.Load(normalName.c_str(),tllto_Mirrored);
    } else {
     if ( nearest ) normalMap=library.Load(normalName.c_str(),tllto_Mirrored_Nearest);
     else normalMap=library.Load(normalName.c_str(),tllto_Mirrored);
    }
   }
  }
  // Create and import
  Atlas *a=new Atlas;
  a->mirrored=mirrored;
  a->nearest=nearest;
  a->dontGenElements=true;
  a->height=heightMap;
  a->normal=normalMap;
  a->Create(texture,(int)params.h,(int)params.w);
  a->name=prefix;
  atlasLibrary.Append(a);
  workspace->workspaceAtlasHandles.Add(a);
  workspace->Save();
  atlasLibrary.Save(&workspace->workspaceAtlasHandles,(workspace->path+string("atlases.txt")).c_str());
  ScrollLibrary();
  ReturnTo();
 }
 void ReturnTo() {
  back_to->visible=true;
  this->destroyMe=true;
 }
 void ScrollLibrary();
};

