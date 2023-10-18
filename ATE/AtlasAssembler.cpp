#include "AtlasAssembler.h"

#include "AtlasLibraryBrowser.h"

void AtlasAssemblerPreviewer::ConfirmAndReturn() {
 if ( file_exists(atlasFilename.c_str()) ) {
  ShowCursor(TRUE);
  int result=MessageBox( app.game_window, (atlasFilename+string(" already exists, overwrite?")).c_str(), "Overwrite?", MB_YESNO );
  if ( !USE_SYSTEM_CURSOR_CONTROL ) ShowCursor(FALSE);
  fast.ignoreNextRelease=true;
  if ( result == IDNO ) return;
 }
 CPUImage *out=test_image->image->RtoB();
 out->Save(atlasFilename.c_str());
 delete out;
 test_image->filename=atlasFilename.value;
 test_image->DiscardCPUData();
 test_image->Unload();
 delete test_image.pointer;
 atlas->textureName = atlasFilename;
 {
  if ( file_exists(atlas->heightName.c_str()) ) {
   if ( atlas->mirrored ) {
    if ( atlas->nearest ) atlas->height=library.Load(atlas->heightName.c_str(),tllto_Mirrored_Nearest);
    else atlas->height=library.Load(atlas->heightName.c_str(),tllto_Mirrored);
   } else {
    if ( atlas->nearest ) atlas->height=library.Load(atlas->heightName.c_str(),tllto_Mirrored_Nearest);
    else atlas->height=library.Load(atlas->heightName.c_str(),tllto_Mirrored);
   }
  }
  if ( file_exists(atlas->normalName.c_str()) ) {
   if ( atlas->mirrored ) {
    if ( atlas->nearest ) atlas->normal=library.Load(atlas->normalName.c_str(),tllto_Mirrored_Nearest);
    else atlas->normal=library.Load(atlas->normalName.c_str(),tllto_Mirrored);
   } else {
    if ( atlas->nearest ) atlas->normal=library.Load(atlas->normalName.c_str(),tllto_Mirrored_Nearest);
    else atlas->normal=library.Load(atlas->normalName.c_str(),tllto_Mirrored);
   }
  }
  if ( file_exists(atlas->textureName.c_str()) ) {
   if ( atlas->mirrored ) {
    if ( atlas->nearest ) atlas->texture=library.Load(atlas->textureName.c_str(),tllto_Mirrored_Nearest);
    else atlas->texture=library.Load(atlas->textureName.c_str(),tllto_Mirrored);
   } else {
    if ( atlas->nearest ) atlas->texture=library.Load(atlas->textureName.c_str(),tllto_Mirrored_Nearest);
    else atlas->texture=library.Load(atlas->textureName.c_str(),tllto_Mirrored);
   }
  }
 }
 atlasLibrary.Append(atlas);
 workspace->workspaceAtlasHandles.Add(atlas);
 back_to->destroyMe=true;
 this->destroyMe=true;
 exits_to->visible=true;
 AtlasLibraryBrowser *alb=(AtlasLibraryBrowser *) exits_to.pointer;
 alb->ScrollToEnd();
 return;
}