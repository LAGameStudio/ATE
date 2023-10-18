/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "LinkedList.h"
#include "Strings.h"
#include "DJ.h"
#include "Film.h"
#include "AtlasTileset.h"
#include "SpecialPaths.h"
#include "AudioSampleLookup.h"
#include "Mulligan.h"
#include "MulliganMix.h"
#include "Moxel.h"
#include "ATEVoxel.h"

// this file could be converted into a template for #define QUERIABLE()

// To contain map assets, levels, dialogs, blueprints, weapon templates, item templates, quests, npc- and player-state change scripties
class Workspace;

class WorkspaceQuery {
public:
 void Query( Workspace *c );
};

ONE(Workspace,{})
 Zp<GLImage> image;
 Zstring filename,path;
 Zstring content,description;
 Zstring init_script;
 Zbool proceed; // Whether or not we should proceed with loading items (must encounter cprefix first)
 DJTracks tracks;
 WorkspaceQuery query;
 AtlasTilesetHandles workspaceAtlasTilesets;
 AtlasHandles workspaceAtlasHandles;
 Mulligans mulligans;
 MulliganMixes mulliganMixes;
 ATEVoxels voxels;
 void Load( const char *fn, const char *path_prefix="" ) {
  filename=fn;
  path=path_prefix;
  image=library.Load(FORMAT("%sworkspace.png",path_prefix).c_str());
  content=file_as_string(fn);
  fromString(content.c_str());
  ValidateFolderStructure();
 }
 void ValidateFolderStructure() {  
  string myPath;
  myPath=(path)+string("atlases");     if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("textures");    if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("films");       if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("sounds");      if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("music");       if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("particles");   if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("mulligans");   if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("bumpmaps");    if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("tiles");       if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("scripts");     if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
  myPath=(path)+string("voxels");      if ( !folder_exists( myPath.c_str() ) ) make_folder(myPath.c_str());
 }
 KEYWORDS_POST({
  TXTWORD("name",name)
  else TXTWORD("description",description)
  else TXTWORD("init_script",init_script)
  else BADKEY("Workspace");
  NEXTWORD;
 },{
   // Load secondary files saved alongside workspace.txt
   vault.Load((path+string("films\\vault.txt")).c_str());
   atlasLibrary.Load(&workspaceAtlasHandles,(path+string("atlases.txt")).c_str());
   atlasTilesets.Load(&workspaceAtlasTilesets,(path+string("atlastilesets.txt")).c_str());
   OUTPUT("Loading music:\n");
   tracks.fromString((path+string("music\\tracks.txt")).c_str());
   dJ.Merge(&tracks);
   OUTPUT("Loading samples:\n");
   sampleLookup.Load((path + string("samples.txt")).c_str());
   // Set all of the unset workspace references to the last one loaded.
   // Also, transmogrify pointers in the places where strings were loaded to bind a reference.
   atlasTilesets.PostLoad();
   query.Query(this);
   OUTPUT("Loading mulligans:\n");
   mulligans.Load((path + string("mulligans.txt")).c_str());
   OUTPUT("Loading mulligan mixes:\n");
   mulliganMixes.fromString(file_as_string((path + string("mulligan.mixes.txt")).c_str()).c_str());
   OUTPUT("Loading voxels:\n");
   voxels.Load((path+string("voxels/")).c_str());
  }
 );
 KEYWORDSCustom(fromStringIterative,{
  TXTWORD("name",name)
  else TXTWORD("description",description)
  else TXTWORD("init_script",init_script)
  else BADKEY("Workspace");
 });
 WORDKEYS({
  query.Query(this);
  TXTKEY("name",name)
  TXTKEY("description",description)
  TXTKEY("init_script",init_script)
 })
 void Parse() {
  fromString(content.c_str());
 }
 void Encode() {
  content=toString();
 }
 void Save() {
  Encode();
  string_as_file(content.c_str(),filename.c_str());
  atlasLibrary.Save(&workspaceAtlasHandles,(path+string("atlases.txt")).c_str());
  atlasTilesets.Save(&workspaceAtlasTilesets,(path+string("atlastilesets.txt")).c_str());
  mulligans.Save((path + string("mulligans.txt")).c_str());
  string_as_file(mulliganMixes.toString().c_str(), (path + string("mulligan.mixes.txt")).c_str());
 }
 bool SearchCompare( Zstring *a, const char *b, bool prefix=false ) {
  if ( a->value.length() == 0 ) return false;
  if ( !str_cmp(a->value.c_str(),b) ) return true;
  if ( prefix && a->prefix(b) ) return true;
  return false;
 }
#define CAMPAIGNFINDFUNC(A,B,C) A *B(const char *search,bool exact=true) { if ( strlen(search) == 0 ) return null; \
  if ( exact ) { EACH(C.first,A,c) if ( c->workspace == this && SearchCompare(c->name.self(),search) ) return c; } \
  else EACH(C.first,A,c) { if ( c->workspace == this && SearchCompare(c->name.self(),search,true) ) return c; } \
 return null; }
#define CAMPAIGNFINDOPTIMIZED(A,B,C,D) A *B(const char *search, bool exact=true) { if ( strlen(search) == 0 ) return null; \
  if ( exact ) { EACH(query.C.first,D,c) if ( SearchCompare(c->p->name.self(),search) ) return c->p; } \
  else { EACH(query.C.first,D,c) if ( SearchCompare(c->p->name.self(),search,true) ) return c->p; } \
 return null; }

#define CAMPAIGNALLOPTIMIZED(A,B,C) void AllOptimized(const char *search, A*out, bool exact=true) { \
  if ( exact ) { EACH(query.B.first,C,c) if ( SearchCompare(c->p->name.self(),search) ) out->Add(c->p); } \
  else { EACH(query.B.first,C,c) if ( SearchCompare(c->p->name.self(),search,true) ) out->Add(c->p); } }
MANY(Workspace,WorkspaceHandle,WorkspaceHandles,"workspace",Workspaces,{})
 void Load( const char *workspace_folder );
 bool IterativeLoad( const char *workspace_folder );
 Workspace *findByFilename( const char * s ) {
  FOREACH(Workspace,c) if ( !str_cmp(s,c->filename.c_str() ) ) return c;
  return null;
 }
#define APPEND_REFERENCE_LIST(plural,single,NAME_,getter) { plural *ts=getter(); output+="\n\n<A NAME='"; output+=ts->identity(); output+="'></A><TABLE>"; output+=ts->identity(); output+=" References:\n\n"; EACH(ts->first,single,t) { dom_id++; output+="<TR><TD id='left'><A NAME='#"; output+=ts->identity(); output+="_"; output+=I2S(dom_id); output+="'></A>"; output+=t->name; output+="</TD><TD id='right'>"; output+=t->NAME_; output+="</TD></TR>\n"; } output+="</TABLE>"; }
 void SaveReferenceLists() {
  Zstring output="<!DOCTYPE html>\n<HTML><HEAD><TITLE>References lists</TITLE><STYLE>";
  output+=file_as_string("data/references.css");
  output+="</STYLE><SCRIPT>";
  output+=file_as_string("data/references.js");
  output+="</SCRIPT></HEAD><BODY><DIV id='toc'>"
   "<a href='#Things'>Things</a> <BR> "
   "<a href='#Commodities'>Commodities</a> <BR> "
   "<a href='#PlanetProfiles'>PlanetProfiles</a> <BR> "
   "<a href='#Evolution'>Evolution</a> <BR> "
   "<a href='#Lifeforms'>Lifeforms</a> <BR> "
   "<a href='#Interiors'>Interiors</a> <BR> "
   "<a href='#GameLocations'>Locations</a> <BR> "
   "<a href='#SpaceCars'>SpaceCars</a> <BR> "
   "<a href='#VehicleBlueprints'>VehicleBlueprints</a> <BR> "
   "<a href='#VehicleModuleBlueprints'>VehicleModuleBlueprints</a> <BR> "
   "<a href='#VehicleWeaponBlueprints'>VehicleWeaponBlueprints</a> <BR> "
   "<a href='#Munitions'>Munitions</a> <BR> "
   "</DIV>";
  int dom_id=0;
  output+="</BODY></HTML>";
  string_as_file(output.c_str(),specialPaths.historic("references.html").c_str());
 }
DONE(Workspace);
extern Workspaces workspaces;
extern Zp<Workspace> activeWorkspace;