#pragma once
#include "FastGUI.h"
#include "Strings.h"

class ImageSlicer : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<GLImage> loaded;
 Zstring baseName,prefix;
 Zstring targetPath;
 Zstring imageFilename;
 Cartesiand params;  // x,y=col,row w,h=cellSize
 Zbool exportMirroredLR, exportMirroredUD, exportMirroredLRUD;
 Zint slicing;
 void OnLoad() {
  Fullscreen();
  background.Int(32,16,24,255);
 }
 void Render() {
  if ( !visible ) return;
  if ( slicing > 0 ) {
   fast.ljust(termFont,(string("Slicing images now!  PLEASE WAIT\n\nOutputting files to:\n")+targetPath).c_str(),16+384+16,128,-4);
   slicing++;
   if ( slicing == 3 ) SliceImage();
   return;
  }
  Cartesiand area(128.0,128.0+64.0,800.0,800.0);
  fast.DefaultState();
  fast.ResponsiveRatio(w,h);
  fast.SaveState();
  fast.tfont.Set(16,20);
  prefix=fast.textbox(termFont,additive,calibri,additive,"\nFILENAME PREFIX",prefix,16+384+32,64,80,false,1,2,4,true);
  if ( fast.button(digicaps,"Pick Target Folder",16,128,384,48) ) {
   targetPath=dialog_get_folder("Choose a target folder to output all of the image slices to.","Select folder directory from below:",true);
   targetPath+="\\";
  }
  if ( targetPath.length > 0 ) fast.ljust(termFont,(string("Outputting files to:\n")+targetPath).c_str(),16+384+16,128,-4);
  if ( fast.button(digicaps,"Pick Source Image",16,64,384,48) ) {
   imageFilename=dialog_open("Choose an image to import");
   if ( file_exists(imageFilename.c_str()) ) {
    baseName=basename(imageFilename.c_str());
    Strings pieces;
    pieces.SplitPush(baseName.c_str(),'.');
    if ( pieces.count < 1 ) prefix="";
    else prefix=pieces.Get(0);
    loaded=library.Load(imageFilename.c_str());
    if ( loaded ) params.SetRect(64.0,64.0,(double)loaded->width/64.0,(double)loaded->height/64.0);
   }
  }
  if ( loaded ) {
   if ( fast.button(digicaps,"Exact Pixels", (int)area.x2+32+384,(int)area.y,232,32) ) {
    params.x=(int)params.x;
    params.y=(int)params.y;
    if ( params.x < 1.0 ) params.x=1.0;
    if ( params.y < 1.0 ) params.y=1.0;
    params.w=(double)loaded->width/params.x;
    params.h=(double)loaded->height/params.y;
    if ( params.w < 1.0 ) params.w=1.0;
    if ( params.h < 1.0 ) params.h=1.0;
   }
   Cartesiand wasParams;
   wasParams.Set(&params);
   fast.xy(&params,(int)area.x2+32,(int)area.y,1.0,(double)loaded->width,"Cells","WIDE","HIGH",12,true);
   if ( !fast.editing(params.x) && !fast.editing(params.y) 
    && ( params.x != wasParams.x || params.y != wasParams.y ) ) {
    if ( params.x < 1.0 ) params.x=1.0;
    if ( params.y < 1.0 ) params.y=1.0;
    params.w=(double)loaded->width/params.x;
    params.h=(double)loaded->height/params.y;
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
    params.x=(double)loaded->width/params.w;
    params.y=(double)loaded->height/params.h;
    if ( params.x < 1.0 ) params.x=1.0;
    if ( params.y < 1.0 ) params.y=1.0;
   }
   art.Stretch(loaded,crayons.Pick(alabaster),transparency,area.x,area.y,area.w,area.h);
   DrawGrid(area.x,area.y,area.w,area.h,crayons.Pick(red255),(int)params.h,(int)params.w);
   fast.ljust(calibri, FORMAT("Image is %d by %d pixels tall",(int)loaded->width,(int)loaded->height).c_str(),(int)area.x,(int)area.y2+32);
   if ( (double)(int)params.x != params.x || (double)(int)params.y != params.y
     || (double)(int)params.w != params.w || (double)(int)params.h != params.h )
   fast.ljust( calibri,
    "Note that if grid columns # and rows # are not perfect whole numbers,\n"
    "this indicates partial coverage of cell contents in the last column or row,\n"
    "therefore any partial cells will be filled with 'clear' pixels RGBA 0,0,0,0", 
    (int)area.x2+32, (int)area.y+128
   );
   exportMirroredLR=fast.toggle("MIRROR HORIZONTAL",exportMirroredLR,(int)area.x2+32, (int)area.y+128+256,24,24);
   exportMirroredUD=fast.toggle("MIRROR VERTICAL",exportMirroredUD,(int)area.x2+32, (int)area.y+128+256+32,24,24);
   exportMirroredLRUD=fast.toggle("MIRROR ALL WAYS",exportMirroredLRUD,(int)area.x2+32, (int)area.y+128+256+64,24,24);
  }
  if ( targetPath.length > 0 && loaded && fast.button(digicaps,"Slice now",w-256-16,h-192,256,96) ) {
   slicing=1;
   return;
  }
  if ( fast.button(digicaps,"Go Back",w-192-16,h-64,192,48) ) {
   ReturnTo();
   return;
  }
  fast.RecallState();
 }
 void SliceImage() {
  slicing=0;
  if ( !loaded ) return;
  if ( targetPath.length == 0 || !folder_exists(targetPath.c_str()) ) return;
  if ( params.x == 0.0 || params.y == 0.0 || params.w == 0.0 || params.h == 0.0 ) return;
  CPUImage *source=imageLoader.Load(string(loaded->filename));
  if ( !source ) return;
  int sourceWidth=source->getWidth();
  int sourceHeight=source->getHeight();
  if ( sourceWidth > 0 && sourceHeight > 0 ) {
   int colWidth=(int)params.x;
   int rowHeight=(int)params.y;
   int columns=(int)params.w;
   int rows=(int)params.h;
   Crayon pix;
   for ( int i=0; i<columns; i++ ) {
    for ( int j=0; j<rows; j++ ) {
     {
      CPUImage *out=new CPUImage(colWidth,rowHeight);
      for ( x=0; x<colWidth; x++ ) {
       for ( y=0; y<rowHeight; y++ ) {
        int ix=x+i*colWidth;
        int iy=y+j*rowHeight;
        if ( WITHIN(ix,iy,0,0,sourceWidth,sourceHeight) ) { 
         source->GetPixel(ix,iy,&pix);
        } else {
         pix.Pick(clear);
        }
        out->SetPixel(x,y,&pix);
       }
      }
      CPUImage *rtob=out->RtoB();
      delete out; out=rtob;
      rtob=out->FlipH(); delete out; out=rtob;
      out->Save((targetPath+prefix+I2S(i)+string("_")+I2S(j)+string(".png")).c_str());
      delete out;
     }
     if ( exportMirroredLR ) {
      CPUImage *out=new CPUImage(colWidth,rowHeight);
      for ( x=0; x<colWidth; x++ ) {
       for ( y=0; y<rowHeight; y++ ) {
        int ix=x+i*colWidth;
        int iy=y+j*rowHeight;
        if ( WITHIN(ix,iy,0,0,sourceWidth,sourceHeight) ) { 
         source->GetPixel(ix,iy,&pix);
        } else {
         pix.Pick(clear);
        }
        out->SetPixel(colWidth-x-1,y,&pix);
       }
      }
      CPUImage *rtob=out->RtoB();
      delete out; out=rtob;
      rtob=out->FlipH(); delete out; out=rtob;
      out->Save((targetPath+prefix+I2S(i)+string("_")+I2S(j)+string("_H.png")).c_str());
      delete out;
     }
     if ( exportMirroredUD ) {
      CPUImage *out=new CPUImage(colWidth,rowHeight);
      for ( x=0; x<colWidth; x++ ) {
       for ( y=0; y<rowHeight; y++ ) {
        int ix=x+i*colWidth;
        int iy=y+j*rowHeight;
        if ( WITHIN(ix,iy,0,0,sourceWidth,sourceHeight) ) { 
         source->GetPixel(ix,iy,&pix);
        } else {
         pix.Pick(clear);
        }
        out->SetPixel(x,rowHeight-y-1,&pix);
       }
      }
      CPUImage *rtob=out->RtoB();
      delete out; out=rtob;
      rtob=out->FlipH(); delete out; out=rtob;
      out->Save((targetPath+prefix+I2S(i)+string("_")+I2S(j)+string("_V.png")).c_str());
      delete out;
     }
     if ( exportMirroredLRUD ) {
      CPUImage *out=new CPUImage(colWidth,rowHeight);
      for ( x=0; x<colWidth; x++ ) {
       for ( y=0; y<rowHeight; y++ ) {
        int ix=x+i*colWidth;
        int iy=y+j*rowHeight;
        if ( WITHIN(ix,iy,0,0,sourceWidth,sourceHeight) ) { 
         source->GetPixel(ix,iy,&pix);
        } else {
         pix.Pick(clear);
        }
        out->SetPixel(colWidth-x-1,rowHeight-y-1,&pix);
       }
      }
      CPUImage *rtob=out->RtoB();
      delete out; out=rtob;
      rtob=out->FlipH(); delete out; out=rtob;
      out->Save((targetPath+prefix+I2S(i)+string("_")+I2S(j)+string("_HV.png")).c_str());
      delete out;
     }
    }
   }
  }
  delete source;
  open_explorer(targetPath.c_str());
 }
 void ReturnTo() {
  back_to->visible=true;
  this->destroyMe=true;
 }
};

