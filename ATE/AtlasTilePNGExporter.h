#pragma once
#include "GLWindow.h"
#include "AtlasTileset.h"
#include "FastGUI.h"
#include "ConvolverShader.h"

extern Zstring png_exporter_path;
extern Zbool dontTrim;
extern Zint exporter_size_x,exporter_size_y;
extern Zint exporter_pad_x,exporter_pad_y;

class AtlasTilePNGExporter : public GLWindow {
public:
 Zp<GLWindow> back_to;
 Zp<AtlasTile> tile;
 Zp<AtlasTileset> tileset;
 Zp<Atlas> atlas;
 Crayon tint;
 Zstring prefix,fn,fn_hm,fn_nm;
 void OnLoad() {
  Fullscreen();
  background.Int(64,64,64,255);
  tint.Pick(alabaster);
  prefix="tile";
  if ( png_exporter_path.length == 0 ) {
   png_exporter_path=specialPaths.saved_documents+string("\\ATE\\");
   if ( !folder_exists(png_exporter_path) ) make_folder(png_exporter_path.c_str());
  }
 }
 void ReturnToPrevious() {
  back_to->visible=true;
  this->destroyMe=true;
 }
 void Between() {
 }
 void Render() {
  if ( tileset ) RenderTileset();
  else if ( tile ) RenderTile();
  else ReturnToPrevious();
#if defined(DEBUG)
  if ( input->KeyDown(DX_F8) ) FBODebug();
#endif
 }
 void RenderTileset() {
 }
 void RenderTile() {
  Cartesiand previewArea;
  previewArea.SetRect(16+96,48+96,256,256);
  Rectangle(crayons.Pick(green255),&previewArea);
  previewArea.SetRect(
   previewArea.x+exporter_pad_x,
   previewArea.y+exporter_pad_y,
   256-(int)(iratiod(exporter_pad_x,exporter_size_x)*256.0)*2,
   256-(int)(iratiod(exporter_pad_y,exporter_size_y)*256.0)*2
  );
  Rectangle(crayons.jami(0,0,96,0,255),&previewArea);
  tile->Render(tint,atlas,previewArea.x,previewArea.y,previewArea.w,previewArea.h);
  Cartesiand pen;
  pen.Set(previewArea.x+64,previewArea.y2+64);
  png_exporter_path=fast.textbox(termFont,additive,calibri,additive,"OUTPUT PATH",png_exporter_path,(int)pen.x,(int)pen.y,80);
  pen.y+=48;
  prefix=fast.textbox(termFont,additive,calibri,additive,"FILENAME PREFIX",prefix,(int)pen.x,(int)pen.y,80);
  pen.y+=48;
  fn=png_exporter_path+prefix+string(".png");
  fn_hm=png_exporter_path+prefix+string("_hm.png");
  fn_nm=png_exporter_path+prefix+string("_nm.png"); 
  fast.text(FORMAT("Will export to:\n%s\n%s\n%s",fn.c_str(),fn_hm.c_str(),fn_nm.c_str()).c_str(),(int)pen.x,(int)pen.y);
  pen.x=previewArea.x2+32;
  pen.y=previewArea.y;
  fast.text("Output Resolution",(int)pen.x,(int)pen.y);
  pen.y+=48;
  exporter_size_x=fast.integer("X output size",exporter_size_x,1,1024,1,(int)pen.x,(int)pen.y,64,24,5,true);
  pen.y+=48;
  exporter_size_y=fast.integer("Y output size",exporter_size_y,1,1024,1,(int)pen.x,(int)pen.y,64,24,5,true);
  pen.y+=48;
  exporter_pad_y=fast.integer("X pad size",exporter_pad_x,0,exporter_size_x/2,1,(int)pen.x,(int)pen.y,64,24,5,true);
  pen.y+=48;
  exporter_pad_y=fast.integer("Y pad size",exporter_pad_y,0,exporter_size_y/2,1,(int)pen.x,(int)pen.y,64,24,5,true);
  pen.y+=48;
//  dontTrim=fast.toggle("Don't crop the edges",dontTrim,(int)pen.x,(int)pen.y,24,24);
  pen.y+=64;
  if( fast.button("Export Now",(int)pen.x+128,(int)pen.y+256,128,32) ) {
   Output( prefix.c_str(), tile );
  }
  pen.y+=48;
  if( fast.button("<- Go back",(int)pen.x+128,(int)pen.y+256,128,32) ) {
   this->back_to->visible=true;
   this->destroyMe=true;
  }
 }
 void Output( const char *fnp, AtlasTile *tile ) {
  if ( !folder_exists(png_exporter_path) ) make_folder(png_exporter_path.c_str());
  fn=png_exporter_path+string("\\")+string(fnp)+string(".png");
  fn_hm=png_exporter_path+string("\\")+string(fnp)+string("_hm.png");
  fn_nm=png_exporter_path+string("\\")+string(fnp)+string("_nm.png");

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
};

