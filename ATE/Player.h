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
#include "Workspace.h"
#include "ScriptSection.h"
#include "PlayerPortrait.h"

ONE(BuiltBaseLookup,{})
 Zstring locale;
 Zint base_id,base_id_group;
 KEYWORDS({
  TXTWORD("locale",locale)
  NUMWORD("base_id",base_id)
  NUMWORD("bidg",base_id_group)
 })
 WORDKEYS({
  TXTKEY("locale",locale)
  NUMBKEY("base_id",base_id)
  NUMBKEY("bidg",base_id_group)
 })
MANY(BuiltBaseLookup,BuiltBaseLookupHandle,BuiltBaseLookupHandles,"BuiltBaseLookup",BuiltBaseLookups,{})
 KEYSWORDSGroups("BuiltBaseLookup",BuiltBaseLookup)
 int GetNextID();
 void Add( const char * locale, int base_id_group );
 BuiltBaseLookupHandles matches;
 void Matching( const char *locale, int base_id_group ) {
  matches.Clear();
  FOREACH(BuiltBaseLookup,b) if ( b->locale == locale && (base_id_group==0 || base_id_group == b->base_id_group ) ) {
   matches.Add(b);
  }
 }
DONE(BuiltBaseLookup);

enum ScriptResultEnum {
 sre_ConditionFailed=0,
 sre_ConditionSuccess=1,
 sre_NoEffect=2,
 sre_ErrorNotFound=3,
 sre_EngineChanged=4
};

#define CLEAR_SAVES_BEFORE_MONTH 12
#define CLEAR_SAVES_BEFORE_DAY 15
#define CLEAR_SAVES_BEFORE_YEAR 2016


class Player;
extern Zp<Player> player; // currently loaded / active player.

class SpaceGameView;
class InteriorGameView;
ONE(Player,{
})
 Zbool male;
 Zint seed,local_id;
 Zstring firstName,lastName;
 Zstring rank,reloadHere;
 Zstring portraitImage;
 Crayon suitColor;
 Zint level,exp,skillPoints;
 Cartesiand worldpos;
 ///
 Zbool killed;
 double WorldX() {
  return worldpos.x;
 }
 double WorldY() {
  return worldpos.y;
 }
 void Save();
 KEYWORDS_POST({
 },{
 })
 WORDKEYS_PRE({
  },
  {
  TXTKEY("name",name)
  NUMBKEY("seed",seed)
  TXTKEY("first",firstName)
  TXTKEY("last",lastName)
  TXTKEY("rank",rank)
  TAGKEY("male",male)
  SSUBKEY("worldpos",worldpos)
//  SUBKEY("car",car)
  TXTKEY("reloadHere",reloadHere)
 })
 void Dereferences();
MANY(Player,PlayerHandle,PlayerHandles,"player",Players,{})
 void ParseReferences();
 void Load() {
#if defined(DEBUG)
  string filename=specialPaths.historic("ate.users.debug.txt");
  if ( !file_older_than(filename.c_str(),CLEAR_SAVES_BEFORE_DAY,CLEAR_SAVES_BEFORE_MONTH,CLEAR_SAVES_BEFORE_YEAR) ) {
   fromString(file_as_string(filename.c_str()).c_str());
  } else {
   OUTPUT("SAVE GAMES TOO OLD!\n");
   fromString(file_as_string(filename.c_str()).c_str());
  }
  ParseReferences();
#else
  string filename=specialPaths.historic("ate.users.txt");
  if ( !file_older_than(filename.c_str(),CLEAR_SAVES_BEFORE_DAY,CLEAR_SAVES_BEFORE_MONTH,CLEAR_SAVES_BEFORE_YEAR) ) {
   fromString(b64k_decode(file_as_string(specialPaths.historic("ate.users.txt").c_str()).c_str()).c_str());
  }
  ParseReferences();
#endif
 }
 void Save() {
#if defined(DEBUG)
  string_as_file(toString().c_str(),specialPaths.historic("ate.users.debug.txt").c_str());
#else
  string_as_file(b64k_encode(toString().c_str()).c_str(),specialPaths.historic("fringes.of.the.empire.saved.games.txt").c_str());
#endif
 }
 KEYSWORDSGroups("player",Player)
 int GetNextID() {
  int last_id=0;
  FOREACH(Player,p) if ( p->local_id > last_id ) last_id=p->local_id;
  return last_id+1;
 }
DONE(Player);

extern Players players;