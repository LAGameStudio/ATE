/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once
#include "GLSetup.h"
#include "Strings.h"

// CCommandLine.h
// Original version gifted by Johan, then majorly fixed and enhanced by H. Elwood

class CCommandLine
{
public:
 Zstring working,execution,options;
	char *m_CommandLine;
 Strings entries;
	
	CCommandLine();
	~CCommandLine();

 int GetInt(char *Entry, int defaultValue=0);
 double GetDecimal(char *Entry, double defaultValue=0.0);
 string OptionPair(char *Entry);             // Returns value from a pairing (-option value), null if no value supplied or not present
	String *Option(char *Entry);                // Returns null if no option (comparative match string)
 Strings *Options(char *Prefix);             // Return all prefix matching options in a Strings
 void Options(char *Prefix, Strings *out);   // Writes out all prefix matches to a &Strings provided
// String *NextOption(char *Entry);
};

extern CCommandLine CmdLine;
