#pragma once

#include "GLWindow.h"
#include "FastGUI.h"
#include "Atlas.h"

#include "AppFramework.h"

#include "PixelateShader.h"
#include "BloomShader.h"

#include "Workspace.h"

extern Zstring png_exporter_path;
extern Zint stashed_bigsquare,stashed_grid22,stashed_grid44,stashed_grid88,stashed_grid1616,stashed_gridAtlas,stashed_grid_square64,stashed_grid_square64b,stashed_grid_square64c,stashed_grid_square64d;

class AtlasAssemblerPreviewer : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<Workspace> workspace;
 Zp<GLImage> test_image;
 Zp<Atlas> atlas;
 Zbool nearest,mirrored;
 Cartesian cellSize,gridSize;
 Zstring exportName,atlasName,atlasFilename;
 Zint r,c;
 Zint lastClickedCel, stashed_Cel;
 Cartesiand selectedGridPosition;
 Zbool exported;
 void OnLoad() {
  fast.ignoreNextRelease=1.0;
  Fullscreen();
  background.Pick(black);
  if ( png_exporter_path.length == 0 ) {
   png_exporter_path=specialPaths.saved_documents+string("\\ATE\\");
   if ( !folder_exists(png_exporter_path) ) make_folder(png_exporter_path.c_str());
  }
  exportName=png_exporter_path+atlasName+string(".png");
  atlas=new Atlas(atlasName.c_str());
  atlas->textureName=atlasFilename;
  atlas->Create(test_image,gridSize.y,gridSize.x);
  atlas->mirrored=mirrored;
  atlas->nearest=nearest;
  if ( atlas->uniformGrid ) {
   if ( stashed_gridAtlas != 0 ) art.stash.Replace(stashed_gridAtlas);
   stashed_gridAtlas=art.stash.GenGrid(atlas->columns,atlas->rows,0.003,crayons.Pick(orange));
  } else {
  }
  c=0; r=0;
  ReplaceCelSample(c,r);
  lastClickedCel=-1;
  if ( stashed_bigsquare == 0 ) stashed_bigsquare=art.stash.GenGrid(1,1,0.1);
  if ( stashed_grid22   == 0 ) stashed_grid22  =art.stash.GenGrid( 2, 2,0.0025,crayons.jami(0,128,128,128,255));
  if ( stashed_grid44   == 0 ) stashed_grid44  =art.stash.GenGrid( 4, 4,0.0025,crayons.jami(0, 96, 96,  0,255));
  if ( stashed_grid88   == 0 ) stashed_grid88  =art.stash.GenGrid( 8, 8,0.0025,crayons.jami(0, 64,  0, 64,255));
  if ( stashed_grid1616 == 0 ) stashed_grid1616=art.stash.GenGrid(16,16,0.0025,crayons.jami(0,  0, 32, 32,255));
  if ( stashed_grid_square64 == 0 ) stashed_grid_square64=art.stash.GenGrid(1,1,0.005,crayons.jami(0,64,192,255,255));
  if ( stashed_grid_square64b == 0 ) stashed_grid_square64b=art.stash.GenGrid(1,1,0.01,crayons.jami(0,64,192,255,255));
  if ( stashed_grid_square64c == 0 ) stashed_grid_square64c=art.stash.GenGrid(1,1,0.02,crayons.jami(0,64,192,255,255));
  if ( stashed_grid_square64d == 0 ) stashed_grid_square64d=art.stash.GenGrid(1,1,0.04,crayons.jami(0,64,192,255,255));
 }
 Cartesian area;
 void Render() {
  if ( !visible ) return;
  fast.ResponsiveRatio(w,h);
  double aspectRatio=iratiod(test_image->height,test_image->width);
  double widthRatio=aspectRatio > 1.0 ? iratiod(test_image->width,test_image->height) : 1.0;
  double heightRatio=aspectRatio > 1.0 ? 1.0 : iratiod(test_image->width,test_image->height) * 1.0;
  fast.text("Clean Preview",area.x+area.w/2,area.y-24,true);
  area.SetRect(32,32,(int)(512.0*widthRatio),(int)(512.0*heightRatio));
  if ( WITHIN(input->mxi,input->myi,area.x,area.y,area.x2,area.y2) ) Area(crayons.Pick(darkChestnut),area.x,area.y,area.x2,area.y2);
  Rectangle(crayons.Pick(red255),area.x-1,area.y-1,area.x2+1,area.y2+1);
  StretchiNoAlpha(test_image,crayons.Pick(alabaster),transparency,area.x,area.y,area.w,area.h);
  area.MoveBy(512+64,0);
  fast.text("Clickable Inspector",area.x+area.w/2,area.y-24,true);
  TilePalette();
  fast.SaveState();
  fast.tfont.Set(16,16);
  if ( fast.button(digicaps,"LOOKS GOOD, IMPORT.",16,h-128,384,96) ) {
   ConfirmAndReturn();
  }
  if ( !exported ) {
   fast.tfont.Set(16,24);
   exportName=fast.textbox(termFont,additive,calibri,additive,"\nEXPORT PNG FILENAME",exportName,16+512,h-128,80,false,1,2,4,true,-4,nullptr,nullptr,false,false,true);
   fast.tfont.Set(16,16);
   if ( fast.button(digicaps,"EXPORT PNG...",128+384+32-8,h-96,256,64) ) {
    ExportAs();
   }
  } else {
   fast.tfont.Set(16,24);
   string wasExportName=exportName;
   exportName=fast.textbox(termFont,additive,calibri,additive,"\nEXPORT PNG FILENAME",exportName,16+512,h-128,80,false,1,2,4,true,-4,nullptr,nullptr,false,false,true);
   if ( str_cmp(exportName.c_str(),wasExportName.c_str())) exported=false;
   fast.tfont.Set(16,16);
   fast.dbutton(digicaps,"EXPORTED PNG...",128+384+32-8,h-96,256,64);
  }
  if ( fast.button(digicaps,"GO BACK",w-192-16,h-96-32,192,96) ) {
   CancelAndReturn();
  }
  fast.RecallState();
 }
 void ReplaceCelSample( int i, int j ) {
  if ( stashed_Cel != 0 ) art.stash.Replace(stashed_Cel);
  double tcdx=1.0/(double)atlas->columns;
  double tcdy=1.0/(double)atlas->rows;
  stashed_Cel=art.stash.GenQuad(
   0.0,0.0,
   1.0,0.0,
   1.0,1.0,
   0.0,1.0,
   tcdx*(double)i,tcdy*(double)j,
   tcdx*(double)i+tcdx,tcdy*(double)j+tcdy,
   crayons.Pick(alabaster)
  );
 }
 void TilePalette() {
  art.Rectangle(crayons.Pick(magenta),area.x-1,area.y-1,area.w+2,area.h+2);
  art.Rectangle(crayons.Pick(black),area.x,area.y,area.w,area.h);
  if ( atlas ) {
   if ( atlas->texture ) {
    art.Stretch(atlas->texture,crayons.Pick(alabaster),area.x,area.y,area.w,area.h);
   }
   if ( atlas->uniformGrid ) {
    art.Element(crayons.Pick(red255),additive,stashed_gridAtlas,area.x,area.y,area.w,area.h);
    if ( area.within(input->mxi,input->myi) ) {
     Cartesiand gridPosition;
     for ( int i=0; i<atlas->columns; i++ ) for ( int j=0; j<atlas->rows; j++ ) {
      double dx=(double)area.w/(double)atlas->columns;
      double dy=(double)area.h/(double)atlas->rows;
      double sx=area.x+(double)i*dx;
      double sy=area.y+(double)j*dy;
      bool hovering=WITHIN(input->mxi,input->myi,(int)sx,(int)sy,(int)(sx+dx-1),(int)(sy+dy-1));
      if ( hovering ) {
       gridPosition.SetRect(sx,sy,dx,dy);
       if ( input->leftReleased() ) {
        r=j;
        c=i;
        selectedGridPosition.Set(&gridPosition);
        ReplaceCelSample(i,j);
       }
       if ( input->rightReleased() ) { r=-1; c=-1; }
      }
     }
     art.Element(crayons.Pick(black),transparency,stashed_bigsquare,gridPosition.x,gridPosition.y,gridPosition.w,gridPosition.h);
     art.Element(crayons.Pick(green255),transparency,stashed_bigsquare,gridPosition.x-1,gridPosition.y-1,gridPosition.w+2,gridPosition.h+2);
    }
    if ( c >= 0 && r >= 0 ) {
     Cartesian gridPosition;
     double dx=(double)area.w/(double)atlas->columns;
     double dy=(double)area.h/(double)atlas->rows;
     double sx=area.x+(double)c*dx;
     double sy=area.y+(double)r*dy;
     gridPosition.SetRect(sx,sy,dx,dy);
     art.Element(crayons.Pick(black),transparency,stashed_bigsquare,gridPosition.x,gridPosition.y,gridPosition.w,gridPosition.h);
     art.Element(crayons.Pick(alabaster),transparency,stashed_bigsquare,gridPosition.x-2,gridPosition.y-2,gridPosition.w+4,gridPosition.h+4);
    }
   }
  }
  if ( input->KeyUp(DX_LEFT) ) {
   if ( c >= 0 && r >= 0 ) {
    c-=1; if ( c < 0 ) c =0;
   } else { c=0; r=0; }   
   ReplaceCelSample(c,r);
  }
  if ( input->KeyUp(DX_RIGHT) ) {
   if ( c >= 0 && r >= 0 ) {
    c+=1; if ( c >= atlas->columns ) c =atlas->columns-1;
   } else { c=0; r=0; }
   ReplaceCelSample(c,r);
  }
  if ( input->KeyUp(DX_UP) ) {
   if ( c >= 0 && r >= 0 ) {
    r-=1; if ( r < 0 ) r =0;
   } else { c=0; r=0; }
   ReplaceCelSample(c,r);
  }
  if ( input->KeyUp(DX_DOWN) ) {
   if ( c >= 0 && r >= 0 ) {
    r+=1; if ( r >= atlas->rows ) r =atlas->rows-1;
   } else { c=0; r=0; }
   ReplaceCelSample(c,r);
  }
  if ( atlas && stashed_Cel != 0 ) {
   art.Element(atlas->texture,crayons.Pick(alabaster),transparency,stashed_Cel,area.x2+8,area.y2-64,64,64);
   art.Element(atlas->texture,crayons.Pick(alabaster),transparency,stashed_Cel,area.x2+8+64+8,area.y2-128,128,128);
  }
 }
 void ExportAs() {
  if ( file_exists(exportName.c_str()) ) {
   ShowCursor(TRUE);
   int result=MessageBox( app.game_window, (exportName+string(" already exists, overwrite?")).c_str(), "Overwrite?", MB_YESNO );
   if ( !USE_SYSTEM_CURSOR_CONTROL ) ShowCursor(FALSE);
   fast.ignoreNextRelease=true;
   if ( result == IDNO ) return;
  }
  CPUImage *out=test_image->image->RtoB();
  out->Save(exportName.c_str());
  delete out;
  exported=true;
 }
 void CancelAndReturn() {
  test_image->Unload();
  test_image->DiscardCPUData();
  delete test_image;
  back_to->visible=true;
  this->destroyMe=true;
 }
 void ConfirmAndReturn();
};

class AtlasAssembler : public GLWindow {
public:
 Zp<Workspace> workspace;
 Zbool recursive,largestMode,stretchMode,downscaleMode,nearestWhenSmaller,mirrored,nearest;
 Strings picked;
 Cartesian userCellSize,totalSheetSize,padSize;
 Zint scrolledList;
 Zstring atlasName,atlasFilename;
 Zstring baseFolder;
 Zbool autoname,trimSheet;
 Zbool firstnameSelected,secondnameSelected,nearestNeighbor;
 Zint previewing;
 Zp<GLImage> preview;
 Zstring previewFilename;
 Zp<GLWindow> back_to;
 Zint largestX,largestY;
 Cartesiands imageSizes;
 Cartesiand actualSheetSize,actualCellSize;
 Cartesiand actualRowsColumns;
 Cartesian calculatedSize,calculatedGrid;
 Zdouble ignoreNext;
 Zbool willFit,hq4xPost,bloom,blackToClear,magentaToClear;
 Zint totalSheets;
 Zstring status;
 void OnLoad() {
  Fullscreen();
  background.Pick(black);
  userCellSize.Set(64,64);
  padSize.Set(1,1);
  totalSheetSize.Set(gl.maxTextureSize,gl.maxTextureSize);
  baseFolder=workspace ? workspace->path+string("\\atlases\\") : "";
  if ( atlasFilename.length == 0 ) atlasFilename=baseFolder;
  autoname=true;
  trimSheet=true;
  largestMode=true;
  firstnameSelected=true;
  nearestNeighbor=true;
  nearestWhenSmaller=true;
 }
 void ScrubNonImages() {
  Strings other;
  EACHN(picked.first,String,s,{
   Zstring data=s->s;
   data.toLower();
   bool valid_name = data.ending(".png") || data.ending(".jpg") || data.ending(".jpeg");
   if ( valid_name ) other.Add(s->s);
  })
  picked.Clear();
  picked.Concat(&other);
 }
 void ProcessImageInputs() {
  imageSizes.Clear();
  largestX=0;
  largestY=0;
  EACH(picked.first,String,s) {
   GLImage *i=library.Load(s->s.c_str());
   imageSizes.Append(new Cartesiand((double)i->width,(double)i->height,(double)(i->width*i->height),0.0));
   if ( i->width > largestX ) largestX=i->width;
   if ( i->height > largestY ) largestY=i->height;
  }
  totalSheets=0;
  if ( largestMode ) { // Largest is the cell size
   actualCellSize.Set(largestX,largestY);
  } else { // Cell size by user setting
   actualCellSize.Set(userCellSize.x,userCellSize.y);
  }
  int rows = (int)isqrt(imageSizes.count.value);
  int columns=rows;
  while(imageSizes.count.value > rows*columns) rows++;
  if ( rows < columns && columns-rows==1 ) { columns--; rows++; }
  if ( columns < rows && rows-columns==1 ) { rows--; columns++; }
  while(imageSizes.count.value > rows*columns) rows++;
  calculatedSize.Set(columns * ((int)actualCellSize.x+padSize.x),rows * ((int)actualCellSize.y+padSize.y));
  calculatedGrid.Set(columns,rows);
  if ( !trimSheet ) {
   actualSheetSize.Set(totalSheetSize.x,totalSheetSize.y);
  } else {
   actualSheetSize.Set((double)calculatedSize.x,(double)calculatedSize.y);
  }
  willFit = (calculatedGrid.x*calculatedGrid.y >= imageSizes.count) && (actualSheetSize.x <= totalSheetSize.x && actualSheetSize.y <= totalSheetSize.y);
 }
 void Render() {
  if ( !visible ) return;
  fast.ResponsiveRatio(w,h);
  recursive=fast.toggle("Recursive",recursive,16+512+16,16+4,24,24);
  if ( fast.button("Import All From Folder",16,16,512,32) ) {
   string path=dialog_get_folder("ATLAS TILE EDITOR: Pick a folder full of image files to add to the import list.","Select entire folder to be imported below:",false);
   if ( path.length() > 0 ) {
    Strings *temp=ls(path.c_str());
    picked.Concat(temp);
    delete temp;
    ScrubNonImages();
    ProcessImageInputs();
   }
  }
  if ( ignoreNext <= 0.0 && fast.button("Import Multiple Files",16,16+48,512,32) ) {
   fast.ignoreNextRelease = 4.0;
   ignoreNext = 4.0;
   Strings temp;
   dialog_multiselect(&temp,"ATLAS TILE EDITOR: Pick one or more PNG or JPG image files to add to the import list.");
   if ( temp.count == 1 && ((String *)temp.first)->s.length() < 1 ) {/*none selected*/} else {
    picked.Concat(&temp);
    ScrubNonImages();
    ProcessImageInputs();
   }
  }
  else {
   ignoreNext -= FRAMETIME;
   if (ignoreNext < 0.0) ignoreNext = 0.0;
  }
  bool wasLargestMode=largestMode;
  largestMode = fast.toggle("Cell = Largest", largestMode, 512+16+256+512+16,16,24,24);
  if ( wasLargestMode != largestMode ) ProcessImageInputs();
  bool wasMode=stretchMode;
  stretchMode = fast.toggle("Upscale to Cell", stretchMode, 512+16+256+512+16,16+32,24,24);
  if ( wasMode != stretchMode ) ProcessImageInputs();
  if ( !largestMode ) {   
   bool wasMode=downscaleMode;
   downscaleMode = fast.toggle("Downscale to Cell", downscaleMode, 512+16+256+512+16,16+32+32,24,24);
   if ( wasMode != downscaleMode ) ProcessImageInputs();
  }
  if ( downscaleMode || stretchMode ) {
   nearestWhenSmaller = !fast.toggle("Bilinear Upscale",!nearestWhenSmaller, 512+16+256+512+64+168,16+32,24,24);
   if ( downscaleMode ) {
    nearestNeighbor = !fast.toggle("Bilinear Down", !nearestNeighbor, 512+16+256+512+64+168,16+32+32,24,24);
   }
  }
  fast.GetState(768,0,256,128);
  bool typingSelected=fast.hovering;
  fast.xy(&userCellSize,768,16,1,1024,"CELLS");  fast.xy(&padSize,768+256+16,16,0,32,"PADDING");
  fast.xy(&totalSheetSize,768,16+48,1,hq4xPost?gl.maxTextureSize/4:gl.maxTextureSize,"SHEET MAX");
  trimSheet = fast.toggle("Allow Sheet Trimming", trimSheet, 768+256,16+48+8,24,24);
  if ( fast.buttonarea(16,h-128-24,256,32) ) { /// "Remove Duplicate Files"
   Strings other;
   EACH(picked.first,String,s) other.AddIfUnique(s->s.c_str());
   picked.Clear();
   picked.Concat(&other);
   ProcessImageInputs();
  }
  if ( fast.buttonarea(16+256+16,h-128-24,256,32) ) { /// "Drop Selected Files"
   EACHN(picked.first,String,s,{ if ( s->toggled ) { picked.Remove(s); delete s; } });
   ProcessImageInputs();
  }
  if ( fast.buttonarea(16+256+16+256+16,h-128-24,256,32) ) { /// "Clear File List"
   picked.Clear();
   ProcessImageInputs();
  }
  fast.SaveState();
  fast.tfont.Set(16,24);
  int wasPreviewing=previewing;
  int selected=fast.togglelist(termFont,"",&picked,16,16+96,1280,26,&scrolledList.value,72,true);
  if ( selected >= 0 ) previewing=selected;
  if ( previewing != wasPreviewing ) {
   String *i=(String *)picked.Element(previewing);
   if ( i ) {
    preview=library.Load(i->c_str());
    if ( preview ) {
     previewFilename=basename(preview->filename.c_str());
    }
   }
  }
  if ( preview ) {
   if ( preview->width < 512 && preview->height < 512 ) {
    if ( preview->width < 128 && preview->height < 128 )
    StretchiNoAlpha(preview,crayons.Pick(alabaster),transparency,w-256-32-preview->width,128+256-preview->height,preview->width*2,preview->height*2);
    else
    StretchiNoAlpha(preview,crayons.Pick(alabaster),transparency,w-256-32-preview->width/2,128+256-preview->height/2,preview->width,preview->height);
   } else BestFit(preview,crayons.Pick(alabaster),transparency,(double)w-512.0-32.0,128.0,512.0,512.0,0.0,0.0,0.0,true,false,false);
   fast.text(calibri,FORMAT("%s\n%d WIDE x %d PIXELS",previewFilename.c_str(),preview->height.value,preview->width.value).c_str(),w-256-32,128+512);
  }
  fast.text(calibri,FORMAT("OUTPUT PARAMETERS ___________________________\n\n%d Cells contain Images (%d%% full)\nSHEET SIZE: %d x %d PIXELS\nGRID CELL: %d x %d PADDED",
   (int)imageSizes.count,
   (int)(100.0*iratiod(imageSizes.count,calculatedGrid.x*calculatedGrid.y)),
   (int)calculatedSize.x,(int)calculatedSize.y,
   (int)(actualCellSize.x+padSize.x),(int)(actualCellSize.y+padSize.y)
   ).c_str(),w-256,h-384+64
  );
  if ( !willFit ) fast.text(calibri,"WILL NOT FIT IN SHEET\nREMOVE IMAGES OR\nINCREASE SHEET SIZE",w-128,h-256+64,crayons.Pick(red255));
  int wasGridX=calculatedGrid.x;
  int wasGridY=calculatedGrid.y;
  fast.xy("GRID","COLUMNS","ROWS",&calculatedGrid,w-512,h-256+64,1,10000);
  if ( wasGridX != calculatedGrid.x || wasGridY != calculatedGrid.y ) {
   calculatedSize.Set(calculatedGrid.x * (int)actualCellSize.x,calculatedGrid.y * (int)actualCellSize.y);
   if ( !trimSheet ) {
    actualSheetSize.Set(totalSheetSize.x,totalSheetSize.y);
   } else {
    actualSheetSize.Set((double)calculatedSize.x,(double)calculatedSize.y);
   }
   willFit = (calculatedGrid.x*calculatedGrid.y >= imageSizes.count) && (actualSheetSize.x <= totalSheetSize.x && actualSheetSize.y <= totalSheetSize.y);
  }
  mirrored=fast.toggle("GL_MIRRORED",mirrored,w-512-32,h-256+64+48,24,24);
  nearest=fast.toggle("GL_NEAREST",nearest,w-512+256-64,h-256+64+48,24,24);
  fast.RecallState();
  if ( fast.button("Remove Duplicate Files",16,h-128-24,256,32) ) {}
  if ( fast.button("Drop Selected Files",16+256+16,h-128-24,256,32) ) {}
  if ( fast.button("Clear File List",16+256+16+256+16,h-128-24,256,32) ) {}
  fast.SaveState();
  fast.tfont.Set(16,24);
  bool sync_names = true;
  if ( !autoname ) sync_names=false;
  Zstring wasAtlasName=atlasName;
  atlasName=fast.textbox(termFont,additive,calibri,additive,"\nATLAS NAME",atlasName,16-6,h-72-6,45,false,1,2,4,!typingSelected && firstnameSelected,-4,nullptr,nullptr,false,false,true);
  if ( atlasName == wasAtlasName ) sync_names=false;
  if ( fast.hovering ) {
   firstnameSelected=true;
   secondnameSelected=false;
  }
  if ( sync_names ) atlasFilename=baseFolder+atlasName+string(".png");
  atlasFilename=fast.textbox(termFont,additive,calibri,additive,"\nATLAS IMAGE FILENAME",atlasFilename,16+512+48-6,h-72-6,64,false,1,2,4,!typingSelected && secondnameSelected,-4,nullptr,nullptr,false,false,true);
  if ( fast.hovering ) {
   firstnameSelected=false;
   secondnameSelected=true;
  }
  autoname = fast.toggle("Automatic Filename Generation", autoname, 32+512+96,h-32-6,24,24);
  fast.RecallState();
  fast.SaveState();
  fast.tfont.Set(16,16);
  if ( fast.button(digicaps,"GO BACK",w-16-384-16-128,h-100,128,64) ) {
   back_to->visible=true;
   this->destroyMe=true;
   fast.ignoreNextRelease=1.0;
   return;
  }
  blackToClear = fast.toggle("Black2Clear", blackToClear, w-160,16,24,24);
  if ( blackToClear ) magentaToClear=false;
  magentaToClear = fast.toggle("Purple2Clear", magentaToClear, w-160,16+32,24,24);
  if ( magentaToClear ) blackToClear=false;
//  hq4xPost = fast.toggle("Smooth It", hq4xPost, w-160,16+32,24,24);
//  bloom = fast.toggle("Bloom It", bloom, w-160,16+32+32,24,24);
  fast.text(digicaps,status.c_str(),w-16-384,h-100-fast.tfont.h);
  bool valid_name = atlasFilename.ending(".png") || atlasFilename.ending(".jpg") || atlasFilename.ending(".jpeg");
  if ( atlasName.length < 1 || picked.count == 0 || !valid_name ) fast.dbutton(digicaps,"PREVIEW THEN IMPORT",w-16-384,h-100,384,64);
  else if ( fast.button(digicaps,"PREVIEW THEN IMPORT",w-16-384,h-100,384,64) ) {
   GLImage *result=DoImport();
   if ( !result ) {
    status=FORMAT("Error: Could Not Export %s",atlasFilename.c_str());
   }
   AtlasAssemblerPreviewer *aap=new AtlasAssemblerPreviewer;
   aap->back_to=this;
   aap->exits_to=back_to;
   aap->workspace=workspace;
   aap->cellSize.Set((int)actualCellSize.x,(int)actualCellSize.y);
   aap->gridSize.Set(calculatedGrid.x,calculatedGrid.y);
   aap->test_image=result;
   if ( autoname ) atlasFilename=baseFolder+atlasName+string(".png");
   if ( !atlasFilename.Contains(".png") ) atlasFilename+=".png";
   aap->atlasFilename=baseFolder+atlasFilename;
   aap->atlasName=atlasName;
   aap->nearest=nearest;
   aap->mirrored=mirrored;
   windows.Add(aap);
   fast.RecallState();
   this->visible=false;
   return;
  }
  fast.RecallState();
 }
 void ReturnTo() {
  if ( back_to ) {
   back_to->visible=true;
   this->destroyMe=true;
  }
 }
 GLImage *DoImport() {
  int cellWriteW=(int)actualCellSize.x;
  int cellWriteH=(int)actualCellSize.y;
  int cellW=(int)actualCellSize.x+padSize.x;
  int cellH=(int)actualCellSize.y+padSize.y;
  int cellW2=cellWriteW/2;
  int cellH2=cellWriteH/2;
  int totalW=cellW*calculatedGrid.x;
  int totalH=cellH*calculatedGrid.y;
  Crayon color(clear);
  CPUImage *out=new CPUImage(totalW,totalH);
  if (!out) return null;
  for ( int i=0; i<totalW; i++ ) {
   for ( int j=0; j<totalH; j++ ) {
    out->SetPixel(i,j,&color);
   }
  }
  int sx=0;
  int sy=0;
  EACH(picked.first,String,s) {
   if ( sx+cellW > totalW ) {
    sx=0;
    sy+=cellH;
    if ( sy > totalH ) break;
   }
   //
   CPUImage *original=imageLoader.Load(s->s);
   if ( !original ) continue;
   int oW=original->getWidth();
   int oH=original->getHeight();
   bool isSmallerThanCell = (oW<cellWriteW || oH<cellWriteH);
   bool equalsCellSize = (oW == cellWriteW && oH == cellWriteH);
   if ( equalsCellSize 
     || (isSmallerThanCell && !stretchMode)
     || (largestMode && !stretchMode)
     ) { // copy
    bool oddW=oW%2 == 0;
    bool oddH=oH%2 == 0;
    int halfW=oW/2;
    int halfH=oH/2;
    Cartesian topLeft(
     sx+cellW2-halfW-(oddW?1:0),
     sy+cellH2-halfH-(oddH?1:0)
    );
    if ( topLeft.x < sx ) topLeft.x=sx;
    if ( topLeft.y < sy ) topLeft.y=sy;
    for ( int i=0; i<oW; i++ ) {
     for ( int j=0; j<oH; j++ ) {
      if ( WITHIN(i,j,0,0,oW-1,oH-1) ) {
       original->GetPixel(i,j,&color);
       out->SetPixel(topLeft.x+i,topLeft.y+j,&color);
      }
     }
    }
   } else if ( (downscaleMode && !isSmallerThanCell) || (stretchMode && isSmallerThanCell) ) { // scale
    if ( (isSmallerThanCell && nearestWhenSmaller) || (!isSmallerThanCell && nearestNeighbor) ) { // nearest
     for ( int i=0; i<cellWriteW; i++ ) {
      for ( int j=0; j<cellWriteH; j++ ) {
       int nearestX=(int)(iratiod(oW,cellW)*(double)i);
       int nearestY=(int)(iratiod(oH,cellH)*(double)j);
       original->GetPixel(
        CLAMPTO(nearestX,0,oW-1),
        CLAMPTO(nearestY,0,oH-1),
        &color
       );
       out->SetPixel(sx+i,sy+j,&color);
      }
     }
    } else { // bilinear
     GLImage *originalGL=library.Load(s->s.c_str());
     fboCDS2048.background.Pick(clear);
     fboCDS2048.Bind();
     StretchiNoAlpha(originalGL,crayons.Pick(alabaster),transparency,0,0,cellWriteW*2,cellWriteH);
     fboCDS2048.Unbind();     
     fboCDS2048.BindWithoutClearing();
     CPUImage *c=fboCDS2048.Snapshot();
     fboCDS2048.Unbind();
     CPUImage *d=c->FlipV();
     delete c; c=d->Crop(0,0,cellWriteW*2,cellWriteH*2);
     delete d; d=c->Half();
     delete c; c=d->RtoB();
     delete d; d=null;
     int dW=c->getWidth();
     int dH=c->getHeight();
     for ( int i=0; i<dW; i++ ) {
      for ( int j=0; j<dH; j++ ) {
       c->GetPixel(i,j,&color);
       out->SetPixel(sx+i,sy+j,&color);
      }
     }
     delete c; c=null;
    }
   }
   delete original;
   //
   sx+=cellW;
  }
  GLImage *source=null;
  if ( !blackToClear && !bloom && !hq4xPost ) {
   source=new GLImage((atlasName+string(".preview.source")).c_str(),out);
   source->deleteAfterUpload=false;
   source->Upload();
   return source;
  }
  if ( blackToClear ) { 
   CPUImage *processed = out->BlackToClear();
   delete out;
   out = processed;
  }
  if ( magentaToClear ) { 
   CPUImage *processed = out->MagentaToClear();
   delete out;
   out = processed;
  }
  //if ( bloom ) {
  // CPUImage *processed = out->Gaussian();
  // delete out;
  // out = processed;
  //}
  //if ( hq4xPost ) {
  // CPUImage *processed = out->HQ4X();
  // delete out;
  // out = processed;
  //} 
  source=new GLImage((atlasName+string(".preview.source")).c_str(),out);
  source->deleteAfterUpload=false;
  source->Upload();
  return source;
 }
};

