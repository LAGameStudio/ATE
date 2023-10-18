#include "Workspace.h"
#include "PresentationEditor.h"

#include "DJ.h"

Zp<Workspace> activeWorkspace;
Workspaces workspaces;

 void Workspaces::Load( const char *workspace_folder ) {
 Zstring path=slash_to_backslash(trailing_slash(workspace_folder).c_str());
 Zdisposable<Strings> folders; folders.Recycle(ls(workspace_folder,false,false,true));
 if ( !folders ) return;
 EACH(folders->first,String,subfolder) {
  string workspacepath=path+subfolder->s+string("\\");
  string filepath=workspacepath+string("workspace.txt");
  if ( file_exists(filepath.c_str()) ) {
   Workspace *loading=new Workspace;
   loading->Load(filepath.c_str(),workspacepath.c_str());
   Append(loading);
  }
  presentationProfiles.Load( (workspacepath+string("presentations\\")).c_str() );
 }
}

Zint globalLoadedWorkspaceCounter,lastLoadedWorkspaceCounter;
Zint globalLoadedTypeCounter,lastLoadedTypeCounter;
Zint globalLoadedObjectCounter,lastLoadedObjectCounter;

Zbool globalLoadedFolders;

Zbool loadingVehicleWeaponBlueprints,
 loadingVehicleModuleBlueprints,
 loadingVehicleBlueprints,
 loadingCommodity,loadingMunition,loadingSpecies,
 loadingQuest,loadingDialog,loadingSpaceCars,
 loadingStarmapProfile,loadingGameLocation,
 loadingPlanetProfile,loadingThing,loadingInterior,
 loadingOvertalk,loadingQmap,loadingLifeform;
Zbool doneLoadingVehicleWeaponBlueprints,
 doneLoadingVehicleModuleBlueprints,
 doneLoadingVehicleBlueprints,doneLoadingSpecies,
 doneLoadingCommodity,doneLoadingMunition,
 doneLoadingQuest,doneLoadingDialog,doneLoadingSpaceCars,
 doneLoadingStarmapProfile,doneLoadingGameLocation,
 doneLoadingPlanetProfile,doneLoadingThing,doneLoadingInterior,
 doneLoadingOvertalk,doneLoadingQmap,doneLoadingLifeform,
 doneLoadingCommodities;

Zdis<Strings> loadedFolders;
Zp<String> loadedSubfolder;
Zp<Workspace> loadedWorkspace;

Zstring loadedWorkspacePath;
Zstring loadedString;
Zstring loadingType;
Zdis<BinaryFile> loadedFile;
int loadedRemaining=0;

#define LOADING_OBJECT_TYPE(classtype,classkey,loading,loaded,filenam3,collection,done) if ( !done ) { \
  if ( !loading ) { loading=true; globalLoadedTypeCounter++; \
   string fpath=loadedWorkspacePath+string(filenam3); \
   loadedString=file_as_string(fpath.c_str()); \
   loadingType=classkey; \
   OUTPUT("<-- LOADING FILE: %s -->\n",fpath.c_str()); } \
  const char *w=loadedString.Next(); \
  if ( *w == '\0' ) { done=true; loadingType="Type"; } else if ( !str_cmp(w,classkey) ){ \
   w=loadedString.Next(); loaded=new classtype; loaded->fromString(w); collection.Append(loaded); \
   globalLoadedObjectCounter++; } else { BADKEY(classkey); w=loadedString.Next(); } \
  return false;}

#define LOADING_OBJECT_TYPE_BIN(classtype,loading,loaded,filenam3,collection,done) if ( !done ) { \
  if ( !loading ) { loading=true; globalLoadedTypeCounter++; \
   string fpath=loadedWorkspacePath+string(filenam3); \
   loadedFile.Recycle(); loadedFile->Open(fpath.c_str(),false); loadedFile->read(&loadedRemaining); \
   OUTPUT("<-- LOADING %d FROM BINARY FILE: %s -->\n",loadedRemaining,fpath.c_str()); } \
   if ( loadedFile->err != 0 || loadedFile->end_of_file || loadedRemaining == 0 ) { \
   if ( loadedFile->err != 0 || loadedFile->end_of_file ) OUTPUT("ERROR READING FROM FILE!!\n"); done=true; } else { \
   collection.BinaryReadIterative(loadedFile.pointer,&loadedRemaining); \
   globalLoadedObjectCounter++; \
  } return false;}

#define LOADING_OBJECTS_TYPE_BINARY(classtype,loading,loaded,filenam3,collection,done) if ( !done ) { \
  int previous=collection.count; B_IN_INCLUDE((loadedWorkspacePath+string(filenam3)).c_str(),collection); \
  globalLoadedTypeCounter++; globalLoadedObjectCounter+=collection.count-previous; \
  done=true; return false;}

#define KEYWORDS_PATH_INCLUDE_bool(pathnam3,staticclassvar) { OUTPUT("<--Loading all files from path:`%s`---\n",pathnam3); Strings *list=ls(pathnam3,true,true,true); if ( !list ) return false; EACH(list->first,String,s) KEYWORDS_INCLUDE(s->s.c_str(),staticclassvar); delete list; }

#define LOADING_OBJECTS_TYPE_PATH(classtype,loading,loaded,pathnam32,collection,done) if ( !done ) { \
  int previous=collection.count; KEYWORDS_PATH_INCLUDE_bool((loadedWorkspacePath+string(pathnam32)+string("\\")).c_str(),collection); \
  globalLoadedTypeCounter++; globalLoadedObjectCounter+=collection.count-previous; \
  done=true; return false;}

//void Workspace::IterativeLoad()
bool Workspaces::IterativeLoad( const char *workspace_folder ) {
 if ( !globalLoadedFolders ) {
  loadedFolders.Recycle(ls(workspace_folder,false,false,true));
  if ( !loadedFolders ) return true; /// STOPS ITERATIVE.  NOTHING TO DO.
  loadedSubfolder=(String *)loadedFolders->first;
  globalLoadedFolders=true;
  return false;
 }
 if ( !loadedWorkspace ) {
  if ( !loadedSubfolder ) return true; /// STOPS ITERATIVE.  NO MORE TO PROCESS.
  Zstring path=slash_to_backslash(trailing_slash(workspace_folder).c_str());
  loadedWorkspacePath=path+loadedSubfolder->s+string("\\");
  string filepath=loadedWorkspacePath+string("workspace.txt");
  if ( file_exists(filepath.c_str()) ) {
   Zstring workspaceFile=file_as_string(filepath.c_str()).c_str();
   loadedWorkspace=new Workspace;
   loadedWorkspace->path=loadedWorkspacePath;
   loadedWorkspace->filename=filepath;
   loadedWorkspace->content=file_as_string(filepath.c_str());
   loadedWorkspace->image=library.Load(FORMAT("%sworkspace.png",loadedWorkspacePath.c_str()).c_str());
   loadedWorkspace->fromStringIterative(workspaceFile.c_str(),loadedWorkspacePath.c_str());  
   loadedWorkspace->ValidateFolderStructure();
   Append(loadedWorkspace);
   loadedWorkspace->mulligans.fromString(file_as_string((loadedWorkspacePath + string("mulligans.txt")).c_str()).c_str());
   loadedWorkspace->mulliganMixes.fromString(file_as_string((loadedWorkspacePath + string("mulligan.mixes.txt")).c_str()).c_str());
   // Load secondary files saved alongside workspace.txt
   vault.Load((loadedWorkspacePath +string("films\\vault.txt")).c_str());
   atlasLibrary.Load(&loadedWorkspace->workspaceAtlasHandles,(loadedWorkspacePath+string("atlases.txt")).c_str());
   atlasTilesets.Load(&loadedWorkspace->workspaceAtlasTilesets,(loadedWorkspacePath+string("atlastilesets.txt")).c_str());
   loadedWorkspace->tracks.fromString((loadedWorkspacePath +string("music\\tracks.txt")).c_str());
   dJ.Merge(&loadedWorkspace->tracks);
   sampleLookup.Load((loadedWorkspacePath+string("samples.txt")).c_str());
   presentationProfiles.Load( (loadedWorkspacePath+string("presentations\\")).c_str() );
  }
  loadedSubfolder=(String *)loadedSubfolder->next;
  globalLoadedWorkspaceCounter++;
  return false;
 }
 loadedFile.Deinstantiate();
   // Set all of the unset workspace references to the last one loaded.
   // Also, transmogrify pointers in the places where strings were loaded to bind a reference.
 atlasTilesets.PostLoad();
 loadedWorkspace->query.Query(loadedWorkspace);
 loadedWorkspace=null;
 return false;
}

void WorkspaceQuery::Query( Workspace *workspace ) {
}