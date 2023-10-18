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
#pragma warning (disable:4100)

#include "main.h"
#include "AppFramework.h"
#include <Xinput.h>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#if defined(DEBUG)
#if defined(USE_STEAMWORKS)
#include "Steamworks.h"
#endif
#else
#if defined(USE_STEAMWORKS)
#include "Steamworks.h"
#endif
#endif

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(_In_ HINSTANCE hinstance,
                   _In_opt_ HINSTANCE hprevinstance,
                   _In_ LPSTR lpCmdLine,
                   _In_ int ncmdshow)
{

 AllocConsole();
 freopen_s(NULL,"CONIN$", "r", stdin);
 freopen_s(NULL,"CONOUT$", "w", stdout);
 freopen_s(NULL,"CONOUT$", "w", stderr);
 HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
 int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
 FILE* hf_out = _fdopen(hCrt, "w");
 setvbuf(hf_out, NULL, _IONBF, 1);
 *stdout = *hf_out;
 HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
 hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
 FILE* hf_in = _fdopen(hCrt, "r");
 setvbuf(hf_in, NULL, _IONBF, 128);
 *stdin = *hf_in;


 XInputEnable(TRUE);
#if defined(USE_STEAMWORKS)
 if ( !CmdLine.Option("-nosteam") ) if ( !steamworks.Init() ) return 0;
#endif
 // Change application to your Application child here if you want.
 //application=your_application;
 app.hinstance = hinstance;
 app.hprevinstance = hprevinstance;
 app.Main();
 return (int) (app.msg.wParam); // return to Windows
} // end WinMain