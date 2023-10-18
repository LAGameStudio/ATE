#pragma once

#include "FastGUI.h"
#include "UglyFont.h"
#include "Workspace.h"
#include "Player.h"
#include "GameMenu.h"

extern Zint globalLoadedWorkspaceCounter,lastLoadedWorkspaceCounter;
extern Zint globalLoadedTypeCounter,    lastLoadedTypeCounter;
extern Zint globalLoadedObjectCounter,  lastLoadedObjectCounter;

extern Zbool globalLoadedFolders;
extern Zdis<Strings> loadedFolders;
extern Zp<String> loadedSubfolder;

ONE(LoadingWindowCounter,{})
 WORDKEYS({ NUMBKEY("last",globalLoadedWorkspaceCounter); NUMBKEY("lastType",globalLoadedTypeCounter); NUMBKEY("lastObject",globalLoadedObjectCounter) })
 KEYWORDS({ NUMWORD("last",lastLoadedWorkspaceCounter); NUMWORD("lastType",lastLoadedTypeCounter); NUMWORD("lastObject",lastLoadedObjectCounter) })
MANY(LoadingWindowCounter,LoadingWindowCounterHandle,LoadingWindowCounterHandles,"LoadingWindowCounter",LoadingWindowCounters,{})
DONE(LoadingWindowCounter);

class GameWorkspaceLoadingWindow : public GLWindow {
public:
 Zp<GLImage> loading_bg;
 Zdouble percent,percent2,percent3;
 Zbool completed1,completed2;
 Zint framePassed;
 Zstring workspaceFolder,workspaceFolder2;
 void OnLoad() {
  Fullscreen();
  background.Pick(black);
  workspaceFolder=specialPaths.working_directory+string("data\\workspaces\\");
  workspaceFolder2=specialPaths.saved_documents+string("\\ATE\\workspaces\\");
  if ( !folder_exists(specialPaths.saved_documents) ) make_folder(specialPaths.saved_documents.c_str());
  if ( !folder_exists(workspaceFolder2.c_str()) ) make_folder(workspaceFolder2.c_str());
  loading_bg=library.Load("data/images/splash.png");  
  if ( file_exists("data/loadingcounter.txt") ) {
   LoadingWindowCounter windowCounterFile;  windowCounterFile.fromString(file_as_string("data/loadingcounter.txt").c_str());
  } 
  UpdatePercentages();
 }
 void UpdatePercentages() {
  percent= 100.0 * (lastLoadedWorkspaceCounter==0?0.0:iratiod((int)globalLoadedWorkspaceCounter,(int)lastLoadedWorkspaceCounter));
  percent2=100.0 * (lastLoadedTypeCounter    ==0?0.0:iratiod((int)globalLoadedTypeCounter,    (int)lastLoadedTypeCounter));
  percent3=100.0 * (lastLoadedObjectCounter  ==0?0.0:iratiod((int)globalLoadedObjectCounter,  (int)lastLoadedObjectCounter));
 }
 void Between() {
  UpdatePercentages();
  if (completed1) {
   completed2 = workspaces.IterativeLoad(workspaceFolder2.c_str());
   if (completed2) EndLoadingSequence();
  } else {
   bool wasCompleted1=completed1;
   completed1=workspaces.IterativeLoad(workspaceFolder.c_str());
   if ( !wasCompleted1 && completed1 ) {
    globalLoadedFolders=false;
    loadedSubfolder=null;
    loadedFolders.Recycle();
   }
  }
 }
 void Render() {
  art.PushViewport(w,h);
  art.Stretch(loading_bg,crayons.Pick(alabaster),0,0,w,h);
  fast.bargraph(crayons.Pick(clear),crayons.jami(0,0,64,0,255),percent/100.0,16,(h-48*3-6)-16,w-32,42);
  if ( framePassed == 0 ) {
   Text(
    "Loading Textures and Building Atlases",
    crayons.Pick(green255),transparency,
    (double)(w/2),(double)(h-48*3),24.0,32.0,1.0,1.0,true
   );
   framePassed++;
  } else {
   /*
   Text(
    FORMAT("Loading Workspace %10d / %10d (%5.1f%%)",(int)globalLoadedWorkspaceCounter,(int)lastLoadedWorkspaceCounter,(double)percent).c_str(),
    crayons.Pick(green255),transparency,
    (double)(w/2),(double)(h-48*3),24.0,32.0,1.0,1.0,true);
   fast.bargraph(crayons.Pick(clear),crayons.jami(0,0,64,0,255),percent2/100.0,16,(h-48*2-6)-16,w-32,42);
   Text(
    FORMAT("        Type     %10d / %10d (%5.1f%%)",(int)globalLoadedTypeCounter,(int)lastLoadedTypeCounter,(double)percent2).c_str(),
    crayons.Pick(green255),transparency,
    (double)(w/2),(double)(h-96),24.0,32.0,1.0,1.0,true);
   fast.bargraph(crayons.Pick(clear),crayons.jami(0,0,64,0,255),percent3/100.0,16,(h-48-6)-16,w-32,42);
   Text(
    FORMAT("        Object   %10d / %10d (%5.1f%%)",(int)globalLoadedObjectCounter,(int)lastLoadedObjectCounter,(double)percent3).c_str(),
    crayons.Pick(green255),transparency,
    (double)(w/2),(double)(h-48),24.0,32.0,1.0,1.0,true);
    */
  }
  art.PopViewport();
 }
 void EndLoadingSequence() {  
  this->destroyMe=true;
  LoadingWindowCounter windowCounterFile;
  string_as_file(windowCounterFile.toString().c_str(),"data/loadingcounter.txt");
  players.Load();
  options.Load();
  windows.Add(new GameMenu);
#if defined(DEBUG)
  workspaces.SaveReferenceLists();
#else
  if ( CmdLine.Option("-editor") ) workspaces.SaveReferenceLists();
#endif
 }
};

