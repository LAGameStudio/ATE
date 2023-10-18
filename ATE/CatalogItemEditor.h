/************************************************************** Author: H. Elwood Gilliland III
 *  _____  _                 _____       _                    *
 * |  _  ||_| ___  ___  ___ |     | ___ | |_  ___  ___        * (c) 2014 PieceMaker Technologies
 * |   __|| || -_||  _|| -_|| | | || .'|| '_|| -_||  _|       * ---------------------------------
 * |__|__ |_||___||___||___||_|_|_||__,||_,_||____|_|         * Replacement for CarouselItemEditor
 * |_   _| ___  ___ | |_  ___  ___ | | ___  ___ |_| ___  ___  * 
 *   | |  | -_||  _||   ||   || . || || . || . || || -_||_ -| * 
 *   |_|  |___||___||_|_||_|_||___||_||___||_  ||_||___||___| * 
 *                                         |___|              *
 **************************************************************/
#pragma once

#include "GLWindow.h"
#include "Delay.h"
#include "FastGUI.h"
#include "CarouselItem.h"
#include "CarouselItemEditorWebAPI.h"
#include "LookAtPerspective.h"
#include "FastGUI.h"
#include "Stopwatch.h"
#include "FBO.h"

#include "KioskFontSelector.h"
#include "KioskVignette.h"

#define PM_PRICE_GROUPS 200
#define PM_VIGNETTE_TYPES "colorsize ring pendant litho " \
                          "lego-plate lego-man 3dlegoifier " \
                          "castlemaker pendantofier citymaker lithofier"
#define PM_DELTA_AMOUNTS  "0.05 0.1 0.2 0.4 0.5 1.0 2.0 4.0 5.0 8.0 10.0 12.0 15.0 16.0 20.0 25.0 30.0"

#define INPUT_DELAY_SHORT  0.2f
#define INPUT_DELAY_MEDIUM 0.5f
#define INPUT_DELAY_LONG   1.0f

enum CatalogItemEditorModes {
 cie_Pick,
 cie_LayoutOverview,
 cie_PlacementEdit
};

class CatalogItemEditor : public GLWindow {
public:
 ScreenPosition vdisplay;
 Zp<FBOColorDepthStencil> fbo;
 Stopwatch stopwatch;
 Zstring help1,help2,help3,status;
 Delay inputDelay;
 Zp<CarouselItem> item;
 Zp<GLWindow> back;
 CatalogItemEditorModes mode;
 CatalogItemEditor() : GLWindow() {
  mode=cie_Pick;
 }
 CatalogItemEditor( CarouselItem *specific ) : GLWindow() {
  item=specific;
  mode=cie_LayoutOverview;
 }
 CatalogItemEditor( GLWindow *back_to ) : GLWindow() {
  back=back_to;
  mode=cie_Pick;
 }
 CatalogItemEditor( GLWindow *back_to, CarouselItem *specific ) : GLWindow() {
  back=back_to;
  item=specific;
  mode=cie_LayoutOverview;
 }
 void SetFBO( FBOColorDepthStencil *f ) {
  fbo=f; //&fboCDS2048;
  vdisplay.Size(fbo->w,fbo->h);
 }
 void OnLoad() {
  Name("Catalog Item Editor");
  SetFBO(&fboCDSScreen);
  background.Pick(black);
  Fullscreen();
  ReloadCatalogList();
  InitPick();
  InitLayoutOverview();
  InitPlacementEdit();
  inputDelay.Set(INPUT_DELAY_SHORT);
  help1="Key help for this editor:\n"
        "\nItem Selection Commands\n"
        "-----------------------\n"
        "Up, down: scroll catalog (loops)\n"
        "Mouse wheel: scroll catalog\n"
        "PgUp, PgDn: page by page scroll\n"
        "Click to select\n"
        ;
  help2="Key help for this editor:\n"
        "Item Layout Overview\n"
        "--------------------\n\n"
        "Click a layout name to edit\n"
        "Click [+] to add new layout\n\n"
        "Camera Controls:\n"
        "Mouse wheel: Change unit delta\n"
        "W,S - Dolly  A,D - Left/Right\n"
        "Q,E - Raise/Lower\n"
        "1,2 - Alternate Rotation Mode\n"
        "Key 'Backspace': Capture camera\n"
        "Hit escape to exit without saving\n"
        "(Will discard any work done)\n"
        "Hold O: see box bounds / wireframe\n"
        "Hold I: Show inside out (normals)\n"
        "Hold U:Flat  Hold P:Wireframe\n\n"
        ;
  help3="Key help for this editor:\n"
        "Layout Detail View\n"
        "------------------\n\n"
        "Click to edit placements\n"
        "Click [+] to add new of type\n"
        "Camera Controls:\n"
        "W,S - Dolly  A,D - Left/right\n"
        "Q,E - Raise / Lower\n"
        "Q,E - Raise/Lower\n"
        "1,2 - Alternate Rotation Mode\n"
        "Key 'Backspace': Capture camera\n\n"
        "Hold F+Change Text Button=Fit Text\n"
        "Hold O: see box bounds / wireframe\n"
        "Hold I: Show inside out (normals)\n"
        "Hold U:Flat  Hold P:Wireframe\n\n"
        "Using the Numpad:\n"
        "  Numpad+ALT: Scale\n"
        " Numpad+CTRL: Slow Rotate\n"
        "Numpad+SHIFT: Quick Rotate\n"
        "Numpad:  Move Selected Placement\n"
        "Z-axis: PgUp/PgDn Y: 8/2 X: 4/6\n"
        "Mouse wheel: Change unit delta\n"
        "Numpad + and -: Fine-tune\n"
        "[X] button - click to deselect/abort\n"
        "Hit escape to exit without saving\n"
        "(Will not save but keeps any work\n"
        "until you exit previous screen)\n"
        ;
 }
 void ReloadCatalogList() {
  carouselItemHTTP.catalogList.Clear();
  pick_first=null;
  InitPick();
  if ( !carouselItemHTTP.RequestItemList() )
  Status( "Warning: could not get the latest item list!  You may overwrite someone's work if you proceed, or make work that is on an older version of the catalog.");
  else
  Status( FORMAT("Catalog loaded via http contained %s items", carouselItemHTTP.catalogList.count.toString().c_str() ).c_str() );
 }
 void Status( const char *s ) {
  Strings lines;
  lines.SplitPush(status.c_str(),'\n');
  lines.SplitPush(s,'\n');
  while ( lines.count > 6 ) {
   String *f=(String *) lines.first;
   lines.Remove(f);
   delete f;
  }
  status = lines.Concatenate('\n');
 }
 void Between() {
  if ( visible ) {
   fast.ResponsiveRatio(fbo->w,fbo->h);
   inputDelay.Between();
   switch ( mode ) {
              case cie_Pick: BetweenPick(); break;
    case cie_LayoutOverview: BetweenLayoutOverview(); break;
     case cie_PlacementEdit: BetweenPlacementEdit(); break;
   }
//   if ( !inputDelay && input->KeyDown(DX_X) ) {
//    TTFExTestcaseGeneration();
//    inputDelay.Reset();
//   }
   fast.ResetResponsiveRatio();
  }
 }
 void Render() {
  if ( visible ) {
   fast.ResponsiveRatio(fbo->w,fbo->h);
   fbo->Bind();
   switch ( mode ) {
              case cie_Pick: RenderPick(); break;
    case cie_LayoutOverview: RenderLayoutOverview(); break;
     case cie_PlacementEdit: RenderPlacementEdit(); break;
   }
   // Show status message
   Blending(none);
   Area( crayons.jami(0,0,0,64,196), 8.0, display->hd-128.0-8.0, display->wd-8.0, display->hd-8.0 );
   crayons.Pick(cyan).gl();
   MultilineText( status.c_str(), 16.0, display->hd-128.0+10.0, 9.0, 12.0, 4.0, false );
   if ( input->KeyDown(DX_F1) ) {
    Area( crayons.jami(0,196,196,215,255), display->w-460-16, 0, display->w, display->h );
    crayons.Pick(black).gl();
    glLineWidth(2);
    switch ( mode ) {
               case cie_Pick: MultilineText( help1.c_str(), display->wd-460, 24.0, 12.0, 16.0, 4.0, false ); break;
     case cie_LayoutOverview: MultilineText( help2.c_str(), display->wd-460, 24.0, 12.0, 16.0, 4.0, false ); break;
      case cie_PlacementEdit: MultilineText( help3.c_str(), display->wd-460, 24.0, 12.0, 16.0, 4.0, false ); break;
    }
    glLineWidth(1);
   }
   else {
    glLineWidth(2);
    Text( FORMAT( "%s       Hold F1:Help   ESC:Back", 
      mode == cie_Pick ? "Item Selection Screen"
      : mode == cie_LayoutOverview ? "Carousel Item View"
      : mode == cie_PlacementEdit ? "Item Layout Editor"
      : "Carousel Item Editor"
     ).c_str(),
     display->w2d, 24.0, 12.0, 16.0, false 
    );
    glLineWidth(1);
   }
   fbo->Unbind();
   fbo->RenderUpsideDown(0,0,display->w,display->h);
//   FBODebug();
   fast.ResetResponsiveRatio();
  }
 }
 /*****************************************************************************************************/
 /* Pick Item Mode */
 /*****************************************************************************************************/
 Zp<CarouselItemCatalogEntry> pick_first;
 Zint pick_listLength;
 void InitPick() {
  pick_listLength = 20;
  pick_first=(CarouselItemCatalogEntry *) carouselItemHTTP.catalogList.first;
 }
 void PickListAdvance() {
  if ( !pick_first ) pick_first=(CarouselItemCatalogEntry *) carouselItemHTTP.catalogList.first;
  else {
   pick_first=(CarouselItemCatalogEntry *) pick_first->next;
  }
 }
 void PickListRetreat() {
  if ( !pick_first ) pick_first=(CarouselItemCatalogEntry *) carouselItemHTTP.catalogList.last;
  else {
   pick_first=(CarouselItemCatalogEntry *) pick_first->prev;
  }
 }
 void RenderPick() {
  // Display item list
  CarouselItemCatalogEntry *ci=(CarouselItemCatalogEntry *) pick_first;
  int sx=16;
  int sy=64;
  for ( int i=0; i<pick_listLength; i++ ) {
   if ( !ci ) {
    ci=(CarouselItemCatalogEntry *) carouselItemHTTP.catalogList.first;
   }
   if ( !ci ) break; // No catalog list
   if ( ci && ci->item ) {
    glLineWidth(2);
    if ( fast.button( FORMAT("%s: %s", FORMAT("%5d", ci->id.value).c_str(),
      ci->item->title.c_str() ).c_str(), sx,sy,512,24, false, 16 ) ) {
     if ( !inputDelay ) {
      stopwatch.Go();
      inputDelay.Reset();
      if ( SetItem(ci->item) ) {
       mode=cie_LayoutOverview;
      }
      stopwatch.Stop();
      if ( mode == cie_LayoutOverview )
      Status(FORMAT("Item Loaded in %1.4f seconds", (double) stopwatch.seconds ).c_str() );
      else Status("Item request failed.");
     }
    }
    if ( ci->item->stl 
     && (ci->item->stl->facets.count <= 0
     || !file_exists(ci->item->filename.c_str()) ) ) {
     crayons.Pick(red255).gl();
     Text("EMPTY/NO STL",(double)(sx+512+64),(double)(sy+14), 12,14,false);
    } else if ( ci->item->stl
             && ( ci->item->stl->facets.extents.x > 150.0 
               || ci->item->stl->facets.extents.y > 150.0
               || ci->item->stl->facets.extents.z > 50.0 ) ) {
     crayons.Pick(red255).gl();
     Text("STL TOO BIG",(double)(sx+512+64),(double)(sy+14), 12,14,false);
    } else {
     crayons.Pick(darkCyan).gl();
     Text(
      FORMAT("Facets: %d  Vertices: %d",
       (int)ci->item->stl->facets.count,
       (int)ci->item->stl->facets.count*3
      ).c_str(),
      (double)(sx+512+64),(double)(sy+14), 12,14,false
     );
    }
    if ( i==0 ) Stretchi( fast.leftArrow.pointer, crayons.Pick(hotPink), 1.0f, additive, sx+512+16, sy, 24, 24 );
    Text(
     FORMAT("Items in remote catalog: %d",
      (int)carouselItemHTTP.catalogList.count
     ).c_str(),
     512+16+64, 32, 12, 16, false
    );
    glLineWidth(1);
   }
   ci=(CarouselItemCatalogEntry *) ci->next;
   if ( !ci ) ci=(CarouselItemCatalogEntry *) carouselItemHTTP.catalogList.first;
   sy+=32;
  }
 }
 void BetweenPick() {
  if ( !inputDelay ) {
   if ( input->KeyDown(ESC) )  {
    inputDelay.Reset();
    if ( !back ) { exit(1); }
    else {
     this->destroyMe=true;
     back->visible=true;
     return;
    }
   } else if ( input->KeyDown(DX_DOWN) || input->KeyDown(DX_NUMPAD2) || input->wheelDown ) {
    inputDelay.Reset();
    PickListAdvance();
   } else if ( input->KeyDown(DX_UP) || input->KeyDown(DX_NUMPAD8) || input->wheelUp ) {
    inputDelay.Reset();
    PickListRetreat();
   } else if ( input->KeyDown(DX_PGUP) || input->KeyDown(DX_NUMPAD9) ) {
    inputDelay.Reset();
    for ( int i=0; i<pick_listLength; i++ ) PickListRetreat();
   } else if ( input->KeyDown(DX_PGDN) || input->KeyDown(DX_NUMPAD3) ) {
    inputDelay.Reset();
    for ( int i=0; i<pick_listLength; i++ ) PickListAdvance();
   }
  }
 }
 /*****************************************************************************************************/
 /* Layout Overview Mode */
 /*****************************************************************************************************/
 Zp<PlacementLayout> layout;
 Zbool typing;
 Strings priceGroups,vignetteTypes;
 Zstring iconFilename;
 Zbool removeEntryConfirm;
 Zbool removePlacementConfirm;
 Zbool indicatedNext,indicatedPrev;
 LookAtPerspective lap;
 Cartesiand position,rotation;
 Zfloat amount;
 Strings amounts;
 Zint amountCount;
 void InitLayoutOverview() {
  // Generate price group names
  for ( int i=0; i<PM_PRICE_GROUPS; i++ ) {
   priceGroups.Add(FORMAT("$%d.99 USD",i).c_str());
  }
  vignetteTypes.SplitPush(PM_VIGNETTE_TYPES,' ');
  // Setup OpenGL Camera
  lap.center.Set(0.0,0.0,0.0);
  lap.eye.Set(0.0,0.0,200.0);
  lap.Near=0.1;
  lap.Far=1000.0;
  amount=1.0;
  amounts.SplitPush(PM_DELTA_AMOUNTS,' ');
  amountCount=5;
//    rotation.x=360.0f;
//    rotation.y=360.0f;
 }
 void BetweenLayoutOverview() {
  if ( !item ) {
   mode=cie_Pick;
  } else if ( !inputDelay ) {
   if ( input->KeyDown(ESC) ) { //Switch to 
    inputDelay.Set();
    mode=cie_Pick;
    ReloadCatalogList();
    return;
   } else if ( input->KeyDown(DX_BACKSPACE) ) { // Capture camera to Layout
    inputDelay.Reset();
    item->camera.Set(&lap,&position,&rotation);
   }
   CameraControls();
  }
  if ( rotationMode ) {
   rotation.x=360.0f*(input->myi-display->h2)/display->hf;
   rotation.z=360.0f*(input->mxi-display->w2)/display->wf;
  } else {
   rotation.y=360.0f*(input->myi-display->h2)/display->hf;
   rotation.x=360.0f*(input->mxi-display->w2)/display->wf;
  }
 }
 bool SetItem( CarouselItem *i ) {
  this->item=i;  
  rotation.Set(&item->camera.rotation);
  position.Set(&item->camera.position);
  lap.Set(&item->camera.lap);
  if ( !carouselItemHTTP.RequestItemData(i,i->id) ) {
   Status("Warning: could not request latest item!  You may be overwriting someone's work.");
   return true;//return false;
  }
  if ( i->icon ) iconFilename=i->icon->filename;
  else iconFilename="";
  // Make sure STL is loaded and ready for viewing.
  if ( !file_exists(item->filename.c_str()) ) {
   Status(FORMAT("File not found >> %s",item->filename.c_str()).c_str());
  } else {
   OUTPUT("Reloading...\n");
   item->stl=stls.Load(item->filename.c_str());
   if ( item->stl && item->stl->facets.count > 0 ) Status(FORMAT("Loaded: %s",item->filename.c_str()).c_str());
   else Status(FORMAT("Issue with STL %s",item->filename.c_str()).c_str());
  }
  RecomputeVBOs();
  return true;
 }
 void RecomputeVBOs() {
  if ( (item && item->stl) 
    && (item->stl->vboGroup == null
     || item->vbos.count == 0) ) {
   item->stl->vboGroup = &item->vbos;
   item->stl->toVBOGroup(&item->vbos);
  } else item->stl->vboGroup=&item->vbos;
  if ( item && item->stl ) {
   double biggest=item->stl->facets.extents.z;
   if ( item->stl->facets.extents.y > biggest ) biggest=item->stl->facets.extents.y;
   if ( item->stl->facets.extents.x > biggest ) biggest=item->stl->facets.extents.x;
   lap.eye.Set(0.0,0.0,biggest*2);
  }
 }
 void RenderLayoutOverview() {
  typing=false;
  if ( item ) {
   if ( item->stl ) {
    item->RenderPreview(&lap,&position,&rotation,0,0,816,600.0,false);
      // Item Layout Camera
    crayons.Pick(gray).gl();
    fast.box(16,16+600+16-64-8,300+16,300+16);
    item->RenderPreview(&item->camera.lap,&item->camera.position,&item->camera.rotation,16,16+600+16-64,300,300,&item->pq,false,false);
    crayons.Pick(alabaster).gl();
    MultilineText("Default Camera Angle\nBackspace: Capture",8,16+600+16-64+300+24,8,12,4,false);
    if ( fast.button("Save Angle",300-64,16+600+16-64+300+24,116,24) ) {
     if ( !inputDelay ) {
      inputDelay.Reset();
      if ( carouselItemHTTP.RequestUpdateCamera(item) ) {
       Status("The item's default camera angle saved successfully.");
      } else Status("Camera angle did not save properly.");
     }
    }
   }
   crayons.Pick(green255).gl();
   glLineWidth(2);
   MultilineText(
    FORMAT( "Item Title: %s\nSTL FiLe: %s\nID#%d", 
     item->title.c_str(),
     item->stl ? item->stl->filename.c_str() : "None set!",
     (int)item->id.value
    ).c_str(),
    32.0, 32.0, 12.0, 16.0, 4.0, false
   );
   if ( fast.button("Reload STL", 32, 128+32, 128, 24) ) {
    if ( !inputDelay ) {
     inputDelay.Reset();
     if ( !file_exists(item->filename.c_str()) ) {
      Status(FORMAT("File not found >> %s",item->filename.c_str()).c_str());
     } else {
      OUTPUT("Reloading...\n");
      item->stl=stls.Load(item->filename.c_str());
      removeEntryConfirm=false;
      removePlacementConfirm=false;
      RecomputeVBOs();
      if ( item->stl && item->stl->facets.count > 0 ) Status(FORMAT("Loaded: %s",item->filename.c_str()).c_str());
      else Status(FORMAT("Issue with STL %s",item->filename.c_str()).c_str());
     }
    }
   }
   crayons.Pick(skyBlue).gl();
   glLineWidth(2);
   String *e=(String *) vignetteTypes.Element(item->vignette);
   string vt=e ? e->s : string("");
   MultilineText(FORMAT(
    "Item extents (mm): %s\n"
    "Facets in STL:    %8d   Price Group:   %d\n"
    "Vignette Type: %d (%s)\n"
    "Layouts %d\n"
    "Total Placements: Text    %d\n"
    "Total Placements: Stencil %d\n"
    "Item Position: %s\n"
    "Item Rotation: %s\n"
    "Camera: ----\n%s\n-------\n",
    item->stl ? item->stl->facets.extents.toString().c_str(): "Invalid STL",
    item->stl ? (int) item->stl->facets.count : -1,
    (int) item->priceGroup,
    (int) item->vignette, vt.c_str(),
    (int) item->layouts.count,
    (int) item->texts.count,
    (int) item->stencils.count,
    item->position.toString().c_str(),
    item->rotation.toString().c_str(),
    item->camera.toString().c_str()
    ).c_str(),16.0+400+16.0,512.0+64.0,14.0,16.0,4.0,false);
   if ( item->icon ) {
    Area( crayons.Pick(peru), 800-1,32+8-1,800+256+2,8+256+2 );
    Rectangle( crayons.Pick(hotPink), 800-1,32+8-1,800+256+2,8+256+2 );
    Stretchi( item->icon, crayons.Pick(alabaster), 1.0f, transparency, 800, 32+8, item->icon->width/4, item->icon->height/4 );
    Rectangle( crayons.Pick(gray), 800, 32+8, 800+item->icon->width/4, 32+8+item->icon->height/4 );
   }
   Text( "Item Layouts:", 512.0+256.0, 384.0-32.0, 14.0,16.0,false);
   if ( fast.button("+", 1024, 384-32-20, 20,20 ) ) {
    if ( !inputDelay ) {
     inputDelay.Reset();
     layout=item->layouts.New();
     item->pq.Layout(item,layout);
     item->pq.ExampleUserData(fs.selected);
     Status("New layout created.");
     mode=cie_PlacementEdit;
    }
   }
   glLineWidth(1);
   int sx=512+256;
   int sy=384;
   EACH(item->layouts.first,PlacementLayout,L) {
    if ( fast.button( FORMAT("%s: %s", FORMAT("%5d", L->id.value).c_str(), L->name.c_str()).c_str(), sx,sy,512,24, false, 16 ) ) {
     if ( !inputDelay ) {
      inputDelay.Reset();
      layout=L;
      item->pq.Layout(item,L);
      item->pq.ExampleUserData(fs.selected);
      mode=cie_PlacementEdit;
     }
    }
    sy+=32;
   }
   glLineWidth(1);
  }
  amountCount=fast.enumerated(&amounts,"Unit Delta (mm)",amountCount,0,amounts.count-1,display->w2-256,16,64,32);
  String *e=(String *) amounts.Element(amountCount);
  amount=atof(e->s.c_str());
  if ( rotationMode ) {
   crayons.Pick(alabaster);
   Text( "XZ Mouse Rotation", display->w2-256-128, 16, 12, 16, true);
  } else {
   crayons.Pick(alabaster);
   Text( "YX Mouse Rotation", display->w2-256-128, 16, 12, 16, true);
  }
 }
 /*****************************************************************************************************/
 /* Placement Edit Mode */
 /*****************************************************************************************************/
 KioskFontSelector fs;
 Zp<PlacementText> placeText;
 Zp<PlacementStencil> placeStencil;
 Zbool ctrl,shift,alt;
 Zbool kLeft, kRight, kUp, kDown, kPgUp, kPgDn, kHome, kEnd, kIns, kDel, kStar;
 Zbool rotationMode;
 void InitPlacementEdit() {
  fs.MoveBy( 800+64, -display->h2/4 );
 }
 void BetweenPlacementEdit() {
  if ( placeText ) fs.Between();
  rotation.x=360.0f*(input->myi-display->h2)/display->hf;
  rotation.y=360.0f*(input->mxi-display->w2)/display->wf;
  if ( !layout ) {
   if ( !item ) {
    mode=cie_Pick;
   } else {
    mode=cie_LayoutOverview;
   }
  }
  if ( !inputDelay && !typing ) {
   // Key Modifiers
   alt=input->KeyDown(LAlt)     || input->KeyDown(RAlt);
   shift=input->KeyDown(LShift) || input->KeyDown(RShift);
   ctrl=input->KeyDown(LCtrl)   || input->KeyDown(RCtrl);
   kLeft =input->KeyDown(Left)  || input->KeyDown(DX_NUMPAD4);
   kRight=input->KeyDown(Right) || input->KeyDown(DX_NUMPAD6);
   kUp   =input->KeyDown(Up)    || input->KeyDown(DX_NUMPAD8); 
   kDown =input->KeyDown(Down)  || input->KeyDown(DX_NUMPAD2);
   kPgUp =input->KeyDown(PgUp)  || input->KeyDown(DX_NUMPAD9);
   kPgDn =input->KeyDown(PgDn)  || input->KeyDown(DX_NUMPAD3);
   kHome =input->KeyDown(Home)  || input->KeyDown(DX_NUMPAD7);
   kEnd  =input->KeyDown(End)   || input->KeyDown(DX_NUMPAD1);
   kIns  =input->KeyDown(Insert)|| input->KeyDown(DX_NUMPAD0);
   kDel  =input->KeyDown(Delete)|| input->KeyDown(DX_NUMPADPERIOD);
   kStar =input->KeyDown(DX_NUMPADSTAR)>0;
   // Back
   if ( input->KeyDown(ESC) ) {
    inputDelay.Reset();
    placeText=null;
    placeStencil=null;
    mode=cie_LayoutOverview;
    return;
   } else if ( layout && input->KeyDown(DX_BACKSPACE) ) { // Capture camera to Layout
    inputDelay.Reset();
    layout->camera.Set(&lap,&position,&rotation);
   } else if ( layout && input->KeyDown(DX_BACKSLASH) ) {
    inputDelay.Reset();
    if ( placeText ) {
     placeText->camera.Set(&lap,&position,&rotation);
    } else if ( placeStencil ) {
     placeStencil->camera.Set(&lap,&position,&rotation);
    }
   }
   CameraControls();
   PlacementControls();   // Arrow keys (adjust placement)
   // Fine-tune
   if ( input->KeyDown(NumpadPlus) || input->wheelUp )  { amountCount+=1; if ( amountCount >= amounts.count ) amountCount=0; inputDelay.Reset(); }
   else if ( input->KeyDown(NumpadMinus) || input->wheelDown ) { amountCount-=1; if ( amountCount <= 0 ) amountCount=amounts.count; inputDelay.Reset(); }
  }
  if ( rotationMode ) {
   rotation.x=360.0f*(input->myi-display->h2)/display->hf;
   rotation.z=360.0f*(input->mxi-display->w2)/display->wf;
  } else {
   rotation.y=360.0f*(input->myi-display->h2)/display->hf;
   rotation.x=360.0f*(input->mxi-display->w2)/display->wf;
  }
 }
 void PlacementControls() {
  if ( shift ) { // Quick Rotate (Shift)
   if ( kUp ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.x=norm_deg(placeText->rotation.x+45.0/2.0);
    if ( placeStencil ) placeStencil->rotation.x=norm_deg(placeStencil->rotation.x+45.0/2.0);
   } else if ( kDown ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.x=norm_deg(placeText->rotation.x-45.0/2.0);
    if ( placeStencil ) placeStencil->rotation.x=norm_deg(placeStencil->rotation.x-45.0/2.0);
   }
   if ( kPgUp ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.y=norm_deg(placeText->rotation.y+45.0/2.0);
    if ( placeStencil ) placeStencil->rotation.y=norm_deg(placeStencil->rotation.y+45.0/2.0);
   } else if ( kPgDn ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.y=norm_deg(placeText->rotation.y-45.0/2.0);
    if ( placeStencil ) placeStencil->rotation.y=norm_deg(placeStencil->rotation.y-45.0/2.0);
   } 
   if ( kLeft ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.z=norm_deg(placeText->rotation.z+45.0/2.0);
    if ( placeStencil ) placeStencil->rotation.z=norm_deg(placeStencil->rotation.z+45.0/2.0);
   } else if ( kRight ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.z=norm_deg(placeText->rotation.z-45.0/2.0);
    if ( placeStencil ) placeStencil->rotation.z=norm_deg(placeStencil->rotation.z-45.0/2.0);
   }
  } else if ( ctrl ) { // Regular rotation
   if ( kUp ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.x=norm_deg(placeText->rotation.x+amount);
    if ( placeStencil ) placeStencil->rotation.x=norm_deg(placeStencil->rotation.x+amount);
   } else if ( kDown ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.x=norm_deg(placeText->rotation.x-amount);
    if ( placeStencil ) placeStencil->rotation.x=norm_deg(placeStencil->rotation.x-amount);
   }
   if ( kPgUp ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.y=norm_deg(placeText->rotation.y+amount);
    if ( placeStencil ) placeStencil->rotation.y=norm_deg(placeStencil->rotation.y+amount);
   } else if ( kPgDn ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.y=norm_deg(placeText->rotation.y-amount);
    if ( placeStencil ) placeStencil->rotation.y=norm_deg(placeStencil->rotation.y-amount);
   } 
   if ( kLeft ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.z=norm_deg(placeText->rotation.z+amount);
    if ( placeStencil ) placeStencil->rotation.z=norm_deg(placeStencil->rotation.z+amount);
   } else if ( kRight ) {
    inputDelay.Reset();
    if ( placeText )    placeText->rotation.z=norm_deg(placeText->rotation.z-amount);
    if ( placeStencil ) placeStencil->rotation.z=norm_deg(placeStencil->rotation.z-amount);
   }
  } else if ( alt ) { // Scale box
   if ( kLeft ) {
    inputDelay.Reset();
    if ( placeText )    { placeText->scale.x-=amount;    if ( placeText->scale.x < 0.1 ) placeText->scale.x=0.1; }
    if ( placeStencil ) { placeStencil->scale.x-=amount; if ( placeStencil->scale.x < 0.1 ) placeStencil->scale.x=0.1; }
   } else if ( kRight ) {
    inputDelay.Reset();
    if ( placeText )    placeText->scale.x+=amount;
    if ( placeStencil ) placeStencil->scale.x+=amount;
   }
   if ( kUp ) {
    inputDelay.Reset();
    if ( placeText )    placeText->scale.y+=amount;
    if ( placeStencil ) placeStencil->scale.y+=amount;
   } else if ( kDown ) {
    inputDelay.Reset();
    if ( placeText )    { placeText->scale.y-=amount;     if ( placeText->scale.y < 0.1 ) placeText->scale.y=0.1; }
    if ( placeStencil ) { placeStencil->scale.y-=amount;  if ( placeStencil->scale.y < 0.1 ) placeStencil->scale.y=0.1; }
   } 
   if ( kPgUp ) {
    inputDelay.Reset();
    if ( placeText )    placeText->scale.z+=amount;
    if ( placeStencil ) placeStencil->scale.z+=amount;
   } else if ( kPgDn ) {
    inputDelay.Reset();
    if ( placeText )    { placeText->scale.z-=amount;     if ( placeText->scale.z < 0.1 ) placeText->scale.z=0.1; }
    if ( placeStencil ) { placeStencil->scale.z-=amount;  if ( placeStencil->scale.z < 0.1 ) placeStencil->scale.z=0.1; }
   }
  } else { // Shift box position
   if ( kRight ) {
    inputDelay.Reset();
    if ( placeText )    placeText->offset.x+=amount;
    if ( placeStencil ) placeStencil->offset.x+=amount;
   } else if ( kLeft ) {
    inputDelay.Reset();
    if ( placeText )    { placeText->offset.x-=amount;     if ( placeText->scale.x < 0.1 ) placeText->scale.x=0.1; }
    if ( placeStencil ) { placeStencil->offset.x-=amount;  if ( placeStencil->scale.x < 0.1 ) placeStencil->scale.x=0.1; }
   }
   if ( kUp ) {
    inputDelay.Reset();
    if ( placeText )    placeText->offset.y+=amount;
    if ( placeStencil ) placeStencil->offset.y+=amount;
   } else if ( kDown ) {
    inputDelay.Reset();
    if ( placeText )    { placeText->offset.y-=amount;    if ( placeText->scale.y < 0.1 ) placeText->scale.y=0.1; }
    if ( placeStencil ) { placeStencil->offset.y-=amount; if ( placeStencil->scale.y < 0.1 ) placeStencil->scale.y=0.1; }
   } 
   if ( kPgUp ) {
    inputDelay.Reset();
    if ( placeText )    placeText->offset.z+=amount;
    if ( placeStencil ) placeStencil->offset.z+=amount;
   } else if ( kPgDn ) {
    inputDelay.Reset();
    if ( placeText )    { placeText->offset.z-=amount;    if ( placeText->scale.z < 0.1 ) placeText->scale.z=0.1; }
    if ( placeStencil ) { placeStencil->offset.z-=amount; if ( placeStencil->scale.z < 0.1 ) placeStencil->scale.z=0.1; }
   }
  }
 }
 void CameraControls() {
  // Camera Controls
  // Positional adjustments
  if ( input->KeyDown(LAlt) ) {
   if ( input->KeyDown(DX_W) ) { lap.eye.z+=amount/2.0; inputDelay.Reset(); }
   if ( input->KeyDown(DX_S) ) { lap.eye.z-=amount/2.0; inputDelay.Reset(); }
   if ( input->KeyDown(DX_A) ) { lap.eye.x-=amount/2.0; inputDelay.Reset(); }
   if ( input->KeyDown(DX_D) ) { lap.eye.x+=amount/2.0; inputDelay.Reset(); }
   if ( input->KeyDown(DX_Q) ) { lap.eye.y+=amount/2.0; inputDelay.Reset(); }
   if ( input->KeyDown(DX_E) ) { lap.eye.y-=amount/2.0; inputDelay.Reset(); }
  } else {
   if ( input->KeyDown(DX_W) ) { position.z+=amount; inputDelay.Reset(); }
   if ( input->KeyDown(DX_S) ) { position.z-=amount; inputDelay.Reset(); }
   if ( input->KeyDown(DX_A) ) { position.x-=amount; inputDelay.Reset(); }
   if ( input->KeyDown(DX_D) ) { position.x+=amount; inputDelay.Reset(); }
   if ( input->KeyDown(DX_Q) ) { position.y+=amount; inputDelay.Reset(); }
   if ( input->KeyDown(DX_E) ) { position.y-=amount; inputDelay.Reset(); }
  }
  if ( !typing ) {
   if ( !inputDelay ) {
    if ( input->KeyDown(DX_1) ) {
     rotationMode=false;
     inputDelay.Reset();
    } else if ( input->KeyDown(DX_2) ) {
     rotationMode=true;
     inputDelay.Reset();
    }
   }
  }
 }
 Zdouble updatePlacementsDelay;
 void RenderPlacementEdit() {
  if ( layout && placeText && fs.changed ) {
   fs.changed=false;
   item->pq.Layout(item,layout);
   item->pq.ExampleUserData(fs.selected,placeText);
  }
  typing=false;
  // Inspection Camera
  item->RenderPreview(&lap,&position,&rotation,16,16,800,600,&item->pq,true,true,false,false,(input->KeyDown(DX_O)>0));
  item->RenderPreview(&lap,&position,&rotation,16,16,800,600,&item->pq,true,true,false,false,(input->KeyDown(DX_O)>0),true);
  crayons.jami(0,96,96,96,255).gl();
  // Item Layout Camera
  fast.box((16+800-8+256)/2,16+600+16-64-8,400+16,300+16);
  item->RenderPreview(&item->pq.layout->camera.lap,&item->pq.layout->camera.position,
   &item->pq.layout->camera.rotation,
   (16+800+256)/2,16+600+16-64,400,300,&item->pq,false,false,true);
  crayons.Pick(alabaster).gl();
  fast.text(&nexaBold,this->layout->name.c_str(),
   (16+800+256)/2+200,
   16+600+16-64+ui->fontMedSm.h+ui->marginMedSm,
   ui->fontMedSm.x,ui->fontMedSm.y,ui->fontMedSm.w,ui->fontMedSm.h,
   ui->color_b_text
  );
  MultilineText("Layout Camera View\nBackspace: Capture Angle",16+800+8-256,16+600+16-64+300+24,8,12,4,false);
  // Editable Layout Information
  layout->name = fast.textbox( "Layout Name", layout->name, 128, 32+600, 40 );
  if ( fast.hovering ) typing=true;
  layout->description = fast.textbox( "Description", layout->description, 128, 32+600+64, 40 );
  if ( fast.hovering ) typing=true;
  int px=816;
  int py=32+128+64;
  int c=0;
  py+=24;
  crayons.Pick(tangelo).gl();
  Text( "Texts", px, py, 8.0, 12.0, false );
  py+=24;
  EACH(item->pq.texts.first,PlacementTextHandle,pth) {
   PlacementText *pt=(PlacementText *) pth->p;
   c+=1;
   if ( fast.button(FORMAT("%c%5d: %s",' ',c,pt->name.c_str()).c_str(),px,py,256-48,24,(placeText == pt),8) ) {
    placeText=pt;
    placeStencil=null;
    removePlacementConfirm=false;
   }
   py+=24;
  }
  if ( fast.button("+",px,py,24,24) ) {
   PlacementText *created=new PlacementText;
   created->name=FORMAT("Text %d",(int)item->pq.texts.count+1);
   created->description="Enter Text To Personalize";
   created->placementLayout.value=item->pq.layout->id.value;
   created->scale.Set(5.0,6.0,2.0);
   item->texts.Append(created);
   item->pq.Layout(item,layout);
   item->pq.ExampleUserData(fs.selected);
   placeText=created;
   placeStencil=null;
   removePlacementConfirm=false;
  }
  px=816;
  py+=24+12;
  py+=24;
  crayons.Pick(tangelo).gl();
  Text( "Stencils", px, py, 8.0, 12.0, false );
  py+=24;
  c=0;
  EACH(item->pq.stencils.first,PlacementStencilHandle,psh) {
   PlacementStencil *ps=(PlacementStencil *)psh->p;
   c+=1;
   if ( fast.button(FORMAT("%c%4d: %s",' ',c,ps->name.c_str()).c_str(), px,py,256-48,24,(placeStencil == ps),8) ) {
    placeStencil=ps;
    placeText=null;
    removePlacementConfirm=false;
   }
   py+=24;
  }
  if ( fast.button("+",px,py,24,24) ) {
   PlacementStencil *created=new PlacementStencil;
   created->name=FORMAT("Image %d",(int)item->pq.stencils.count+1);
   created->description="Choose an Image to Add To Your Piece";
   created->placementLayout.value=item->pq.layout->id.value;
   item->stencils.Append(created);
   item->pq.Layout(item,layout);
   item->pq.ExampleUserData();
   placeStencil=created;
   placeText=null;
   removePlacementConfirm=false;
  }
  py+=48;
  if ( fast.button("X",px,py,24,24) ) {
   placeText=null;
   placeStencil=null;
   removePlacementConfirm=false;
   item->pq.Layout(item,layout);
  }
 
  if ( placeText ) {
   fs.Render();
   int px=816+32+64+128;
   int py=32+128+64;
   placeText->name = fast.textbox( "Label", placeText->name, px, py, 40 );
   if ( fast.hovering ) typing=true;
   py+=48;
   placeText->description = fast.textbox( "Description", placeText->description, px, py, 40 );
   if ( fast.hovering ) typing=true;
   py+=48;
   int old=placeText->characters;
   placeText->characters = (unsigned int) fast.numeric( "Character Limit", (int) placeText->characters, 0, MAX_3D_TEXT_CHARACTERS, px, py, 48, 24 );
   if ( old != placeText->characters.value ) {
    updatePlacementsDelay=1.0f;
   }
   if ( updatePlacementsDelay > 0.0 ) {
    updatePlacementsDelay-=FRAMETIME;
    if ( updatePlacementsDelay <= 0.0 ) fs.changed=true;
   }
   if ( fast.hovering ) typing=true;
   py+=48;
   placeText->canEtch = fast.toggle( "Subtractive", placeText->canEtch.value, px, py, 24, 24 );
   py+=48;
   glLineWidth(2);
   crayons.Pick(tangelo).gl();
   MultilineText(
    FORMAT("Text Placement Box\nPos:  %s (mm)\nRot:  %s (deg)\nSize: %s(mm)\nAvg Glyph Size: %1.2f x %1.2f y (mm)",
     placeText->offset.toString().c_str(),
     placeText->rotation.toString().c_str(),
     placeText->scale.toString().c_str(),
     (float) ( (double)placeText->scale.x/(double)(int)placeText->characters ),
     (float) placeText->scale.y
    ),
    px, py, 10.0, 14.0, 4.0, false
   );
   if ( placeText->scale.x/(double)(int)placeText->characters < 5.0
     || placeText->scale.y < 6.0
     || placeText->scale.z < 1.0 ) {
    crayons.Pick(red255).gl();
    Text("Average glyph size may be too small to print!",display->w2d/2,display->h2d-48.0,10.0,14.0,true);
   }   
   glLineWidth(1);
   // Placement Camera
   crayons.Pick(skyBlue).gl();
   fast.box((16+800-8+256),16+600+16-64-8,400+16,300+16);
   item->RenderPreview(&placeText->camera.lap,&placeText->camera.position,
    &placeText->camera.rotation,
    (16+800+256),16+600+16-64,400,300,&item->pq,false,false,true);
   crayons.Pick(alabaster).gl();
   fast.text(&nexaBold,placeText->name.c_str(),
    (16+800+256)+200,
    16+600+16-64+ui->fontMedSm.h+ui->marginMedSm,
    ui->fontMedSm.x,ui->fontMedSm.y,ui->fontMedSm.w,ui->fontMedSm.h,
    ui->color_b_text
   );
   MultilineText("Detail Camera View\nBackslash: Capture Angle",16+800+8+256,16+600+16-64+300+24,8,12,4,false);
   //
   if ( fast.button("Remove text placement", px, py+96, 512, 24 ) ) {
    removePlacementConfirm=true;
   }
   if ( removePlacementConfirm && fast.button("Remove now?", px, py+128, 128, 24 ) ) {
    item->texts.Remove(placeText);
    item->pq.Layout(item,layout);
    item->pq.ExampleUserData(fs.selected);
    placeText=null;
    removePlacementConfirm=false;
   }
  }
  if ( placeStencil ) {
   int px=816+32+64+128;
   int py=32+128+64;
   placeStencil->name = fast.textbox( "Label", placeStencil->name, px, py, 40 );
   if ( fast.hovering ) typing=true;
   py+=48;
   placeStencil->description = fast.textbox( "Description", placeStencil->description, px, py, 80, false, 2, 4, 8 );
   if ( fast.hovering ) typing=true;
   py+=96;
   placeStencil->canEtch = fast.toggle( "Subtractive", placeStencil->canEtch.value, px, py, 24, 24 );
   py+=48;
   glLineWidth(2);
   crayons.Pick(tangelo).gl();
   MultilineText(FORMAT("Stencil Placement Box\nPos:  %s (mm)\nRot:  %s (deg)\nSize: %s (mm)",
     placeStencil->offset.toString().c_str(),
     placeStencil->rotation.toString().c_str(),
     placeStencil->scale.toString().c_str()
    ), px, py, 10.0, 14.0, 4.0, false
   );
   if ( placeStencil->scale.x < 5.0
     || placeStencil->scale.y < 5.0
     || placeStencil->scale.z < 1.0 ) {
    crayons.Pick(red255).gl();
    Text("Stencil may be too small to print!",display->w2d/2,display->h2d-48.0,10.0,14.0,true);
   }  
   glLineWidth(1);
   // Placement Camera
   crayons.Pick(skyBlue).gl();
   fast.box((16+800-8+256),16+600+16-64-8,400+16,300+16);
   item->RenderPreview(&placeStencil->camera.lap,&placeStencil->camera.position,
    &placeStencil->camera.rotation,
    (16+800+256),16+600+16-64,400,300,&item->pq,false,false,true);
   crayons.Pick(alabaster).gl();
   fast.text(&nexaBold,placeStencil->name.c_str(),
    (16+800+256)+200,
    16+600+16-64+ui->fontMedSm.h+ui->marginMedSm,
    ui->fontMedSm.x,ui->fontMedSm.y,ui->fontMedSm.w,ui->fontMedSm.h,
    ui->color_b_text
   );
   MultilineText("Detail Camera View\nBackslash: Capture Angle",16+800+8+256,16+600+16-64+300+24,8,12,4,false);
   if ( fast.button("Remove stencil placement", px, py+96, 512, 24 ) ) {
    removePlacementConfirm=true;
   }
   if ( removePlacementConfirm && fast.button("Remove now?", px, py+128, 128, 24 ) ) {
    item->stencils.Remove(placeStencil);
    item->pq.Layout(item,layout);
    item->pq.ExampleUserData(fs.selected);
    placeStencil=null;
    removePlacementConfirm=false;
   }
  }
  amountCount=fast.enumerated(&amounts,"Delta (mm)",amountCount,0,amounts.count-1,display->w2-256,16,64,32);
  String *e=(String *) amounts.Element(amountCount);
  amount=atof(e->s.c_str());
  if ( rotationMode ) {
   crayons.Pick(alabaster);
   Text( "XZ Mouse Rotation", display->w2-256-128, 16, 12, 16, true);
  } else {
   crayons.Pick(alabaster);
   Text( "YX Mouse Rotation", display->w2-256-128, 16, 12, 16, true);
  }
  if ( fast.button("Save to Web",display->w-128-16,display->h-256-8-48-56,128,32) ) {
   inputDelay.Reset();
   if ( !carouselItemHTTP.RequestUpdateLayout( item, layout ) ) {
    Status("Warning!  Could not save to web!  Work is still unsaved.");
   } else Status("Layout was saved successfully to the web.");
  }
  if ( fast.button("Try Me Live",display->w-128-16,display->h-256-8-48,128,32) ) {
   inputDelay.Reset();
   windows.Add(new KioskVignette(this->item,this));
   this->visible=false;
  }
  if ( fast.button("Make Me One",display->w-128-16,display->h-256-8-48-56-56,128,32) ) {
   inputDelay.Reset();
   Manufacture();
  }
 }
 // Temporary hack, move parts to to CarouselItem
 void Manufacture() {
  SYSTEMTIME systemTime;
  GetSystemTime(&systemTime);
  CDateTime now(systemTime);
  Zstring hash(
   FORMAT("%d%d%d%d%d%d",
   (int)now.GetDay(),
   (int)now.GetMonth(),
   (int)now.GetYear(),
   (int)now.GetHour(),
   (int)now.GetMinute(),
   (int)now.GetSecond()
   )
  );
  Zstring orderNumber=hash;
  string_as_file(
   FORMAT("TEST\nTEST@TEST.COM\n%d %d %d\n%s\n%s\n",
    (int) item->printTint.r,
    (int) item->printTint.g,
    (int) item->printTint.b,
    item->config.c_str(),
    item->toString().c_str()
   ).c_str(),
   FORMAT("%s%s.txt", KR3_OUT_PATH, hash.c_str() ).c_str()
  );
 
  STLFiles unions;
  unions.Copy(item->stl);
  EACH(item->pq.texts.first,PlacementTextHandle,pth) {
   ImmediateModel translated;
   translated.CopyInto(&pth->p->text->text);
   if ( &pth->p->text->fit ) {
    translated.BoxBounds();
    translated.Centered();
    translated.ScaleRotateMove(&pth->p->scale,&pth->p->rotation,&pth->p->offset);
   } else {
    translated.BoxBounds();
    translated.ZeroCentered(true,true,true);
    translated.ScaleRotateMove(&pth->p->scale,&pth->p->rotation,&pth->p->offset);
   }
   if ( translated.polygons.count > 0 ) {
     translated.BoxBounds();
     unions.Add(&translated);
   } 
  }
  EACH(item->pq.stencils.first,PlacementStencilHandle,psh) {
   ImmediateModel translated;
   translated.CopyAsUnitScaleRotateMove(
    psh->p->placingStencil->stl.pointer,
    &psh->p->scale,
    &psh->p->rotation,
    &psh->p->offset
   );
   translated.BoxBounds();
   translated.moveBy(0.0f,0.0f,-translated.boxMagnitude.z*0.5f);
   unions.Add(&translated);
  }  
  
 // item->stl->SaveAs(FORMAT("%s/%s-base.stl",KR3_OUT_PATH,this->orderNumber.c_str()).c_str());
  unions.SaveAsPrefix(KR3_OUT_PATH,hash.c_str(),true);
  Status(FORMAT("Created: %s/%s",KR3_OUT_PATH,hash.c_str()).c_str());
 }
};