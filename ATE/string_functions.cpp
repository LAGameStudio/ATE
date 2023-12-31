﻿#include "string_functions.h"
#include "moremath.h"
#include "macros.h"
#include "Strings.h"
#include <Windows.h> // for OutputDebugString
#include "Indexed.h"
#include "ZeroTypes.h"

static const unsigned char ascii[256] = {
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
100,101,102,103,104,105,106,107,108,109,
110,111,112,113,114,115,116,117,118,119,
120,121,122,123,124,125,126,127,128,129,
130,131,132,133,134,135,136,137,138,139,
140,141,142,143,144,145,146,147,148,149,
150,151,152,153,154,155,156,157,158,159,
160,161,162,163,164,165,166,167,168,169,
170,171,172,173,174,175,176,177,178,179,
180,181,182,183,184,185,186,187,188,189,
190,191,192,193,194,195,196,197,198,199,
200,201,202,203,204,205,206,207,208,209,
210,211,212,213,214,215,216,217,218,219,
220,221,222,223,224,225,226,227,228,229,
230,231,232,233,234,235,236,237,238,239,
240,241,242,243,244,245,246,247,248,249,
250,251,252,253,254,255
};

// Work Strings
char buf[STRING_SIZE];
char buf2[STRING_SIZE];
char buf3[STRING_SIZE];

void InitWorkStrings() {
 for ( int i=0; i<STRING_SIZE; i++ ) {
  buf[i]=buf2[i]=buf3[i]='\0';
 }
}

const char string_error='\0';

void backup_text_file( const char *fn_to_back_up ) {
 if ( !file_exists(fn_to_back_up) ) return;
 Zstring in=file_as_string(fn_to_back_up);
 int i=1;
 string bfn=string("");
 while ( file_exists((bfn=FORMAT("%s.%d",fn_to_back_up,i)).c_str()) && i < 10 ) i++;
 if ( i == 10 ) { // Reset all backups.
  for ( int j=1; j<5; j++ ) if ( file_exists(FORMAT("%s.%d",fn_to_back_up,i).c_str()) ) DeleteFile(FORMAT("%s.%d",fn_to_back_up,i).c_str());
  i=1;
  bfn=FORMAT("%s.%d",fn_to_back_up,i);
 }
 string_as_file(in.c_str(),bfn.c_str());
}


bool make_folder( const char *targetPath ) {
#if defined(WIN32) 
 // Prepare string for use with FindFile functions.  First, copy the
 // string to a buffer, then append '\*' to the directory name.
 char path[MAX_PATH+1];
 FORMAT(path,MAX_PATH+1,"%s",targetPath);
 TCHAR szDir[MAX_PATH];
 for ( int i=0; i<MAX_PATH; i++ ) szDir[i]=(TCHAR) path[i];
 CreateDirectory(szDir,NULL);
 DWORD dw=GetLastError();
 if ( dw == ERROR_ALREADY_EXISTS ) return true;
 else if ( dw == ERROR_PATH_NOT_FOUND ) return false;
 return true;
#endif
}

// given away on msdn:
// usage
//     CHAR msgText[256];
//     getLastErrorText(msgText,sizeof(msgText));
// converts "Lasr Error" code into text
string getLastErrorText() {
 static char msg[256];
 DWORD retSize;
 LPSTR pTemp=NULL;
 msg[0]='\0';
 retSize=FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ARGUMENT_ARRAY,
                       NULL,GetLastError(),LANG_NEUTRAL,(LPSTR)&pTemp,0,NULL );
 if (!retSize || pTemp == NULL) return string("");
 return string(pTemp);
}

string getLastErrorText( DWORD lastError ) {
 static char msg[256];
 DWORD retSize;
 LPSTR pTemp=NULL;
 msg[0]='\0';
 retSize=FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ARGUMENT_ARRAY,
                       NULL,lastError,LANG_NEUTRAL,(LPSTR)&pTemp,0,NULL );
 if (!retSize || pTemp == NULL) return string("");
 return string(pTemp);
}

#include <WinInet.h>
string getLastInternetErrorText() {
 DWORD error = GetLastError();
 DWORD dwInetError;
 DWORD dwExtLength = 1000;
 TCHAR *szExtErrMsg = NULL;
 TCHAR errmsg[1000];
 szExtErrMsg = errmsg;
 int returned = InternetGetLastResponseInfo( &dwInetError, szExtErrMsg, &dwExtLength );
 return FORMAT("Last Error: %d dwInetError: %d Returned: %d Buffer: %s", error, dwInetError, returned, szExtErrMsg );
}
string getLastInternetErrorText( DWORD lastError ) {
 DWORD dwInetError;
 DWORD dwExtLength = 1000;
 TCHAR *szExtErrMsg = NULL;
 TCHAR errmsg[1000];
 szExtErrMsg = errmsg;
 int returned = InternetGetLastResponseInfo( &dwInetError, szExtErrMsg, &dwExtLength );
 return FORMAT("Last Error: %d dwInetError: %d Returned: %d Buffer: %s", lastError, dwInetError, returned, szExtErrMsg );
}

// Source: 
// http://social.msdn.microsoft.com/Forums/en/Vsexpressvc/thread/0f749fd8-8a43-4580-b54b-fbf964d68375
// and http://codereview.stackexchange.com/questions/419/converting-between-stdwstring-and-stdstring
std::wstring s2ws(const std::string& s)
{
 int slength = (int)s.length() + 1;
 int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
 Indexed<wchar_t> buf;
 buf.Size(len);
 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf.list, len);
 std::wstring r(buf.list);
 return r;
}

// Source:
// http://stackoverflow.com/questions/4339960/how-do-i-convert-wchar-t-to-stdstring
std::string ToNarrow( const wchar_t *s, char dfault, const std::locale& loc ) {
  std::ostringstream stm;
  while( *s != L'\0' ) {
    stm << std::use_facet< std::ctype<wchar_t> >( loc ).narrow( *s++, dfault );
  }
  return stm.str();
}

// Source: ArkM, Daniweb
// http://www.daniweb.com/software-development/cpp/threads/155420/lpwstr-to-stdstring-help
bool cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage) {
 bool res = false;
 char* p = 0;
 int bsz = WideCharToMultiByte(codepage, 0, pw, -1, 0, 0, 0, 0);
 if (bsz > 0) {
  p = new char[bsz];
  int rc = WideCharToMultiByte(codepage, 0, pw, -1, p, bsz, 0, 0);
  if (rc != 0) {
   p[bsz-1] = 0;
   s = p;
   res = true;
  }
 }
 delete[] p;
 return res;
}

WCHAR *ConstCharToWCHAR( const char *in ) {
 static WCHAR out[STRING_SIZE];
 ZeroMemory(out,STRING_SIZE*sizeof(WCHAR));
 string orary=string(in);
 wstring temp=s2ws(orary);
 const wchar_t *p=temp.c_str();
 WCHAR *w=out;
 while ( *p != '\0' ) {
  *w=(WCHAR) *p;
  p++;
  w++;
 }
 *w='\0';
 return out;
}


LPCWSTR chartoLPCW( const char * in ) {
 LPCWSTR converted=ConstCharToWCHAR(in);
 return converted;
}

LPCSTR chartoLPC( const char * in ) {
 LPCSTR converted=TEXT(in);
 return converted;
}

void trim( string *toworkon ) {
 std::remove(toworkon->begin(), toworkon->end(), ' ');
}

bool SKEYDEBUG=false;

#include "SpecialPaths.h"

#include "pThread.h"



void OutputLogString( const char *output, size_t length ) {
#if defined(USE_PTHREADS)
 pthreads.Lock();
#endif
 string fn=specialPaths.historic("applog.txt");
 if ( file_exists(fn.c_str()) ) {
  std::fstream filestr;
  filestr.open(fn.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
  filestr<<output<<std::endl;
  filestr.close();
 } else string_as_file(output,fn.c_str());
#if defined(USE_PTHREADS)
 pthreads.Unlock();
#endif
}

void OUTPUTSTRING( string buf ) {
 OutputDebugString(TEXT(buf.c_str()));
}

void OUTPUTSTRING(const char * buf) {
 OutputDebugString(TEXT(buf));
}

void NADA() {}

bool log_OUTPUT_to_file = false;
bool stop_log_OUTPUT_to_file = true;

#include <fcntl.h>
#include <io.h>

static void OpenConsole()
{
    int outHandle, errHandle, inHandle;
    FILE *outFile, *errFile, *inFile;
    AllocConsole();
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 9999;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
 
    outHandle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    errHandle = _open_osfhandle((long)GetStdHandle(STD_ERROR_HANDLE),_O_TEXT);
    inHandle = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT );
 
    outFile = _fdopen(outHandle, "w" );
    errFile = _fdopen(errHandle, "w");
    inFile =  _fdopen(inHandle, "r");
 
    *stdout = *outFile;
    *stderr = *errFile;
    *stdin = *inFile;
 
    setvbuf( stdout, NULL, _IONBF, 0 );
    setvbuf( stderr, NULL, _IONBF, 0 );
    setvbuf( stdin, NULL, _IONBF, 0 );
 
    std::ios::sync_with_stdio();
}

#if defined(DEBUG) || defined(DEBUG_OUTPUT) || defined(OPTIONAL_LOGGING_TO_FILE)
void OUTPUT(const char * fmt, ...) {
#if defined(USE_PTHREADS)
 pthreads.Lock();
#endif
#if defined(DEBUG) || defined(DEBUG_OUTPUT)
 static char buf[32768];
 buf[32767]='\0';
 va_list args;
 va_start(args, fmt);
 vsprintf_s(buf, fmt, args);
 va_end(args);
 OutputDebugString(TEXT(buf));
 DWORD dwSize = 0;
 WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buf, lstrlen(buf), &dwSize, NULL);
#else
#if defined(OUTPUT_LOG)
 static char buf[32768];
 va_list args;
 va_start(args, fmt);
 vsprintf_s(buf, fmt, args);
 va_end(args);
 OutputLogString(buf, 32768);
#endif
#endif
#if defined(OPTIONAL_LOGGING_TO_FILE)
 if (log_OUTPUT_to_file) {
  static char buf[32768];
  va_list args;
  va_start(args, fmt);
  vsprintf_s(buf, fmt, args);
  va_end(args);
  file_append_string(OUTPUT_LOG_FILE, buf);
 }
#endif
#if defined(USE_PTHREADS)
 pthreads.Unlock();
#endif
}
#endif

void OUTPUTLong( string fmt, string value, char bookends ) {
 pthreads.Lock();
#if defined(DEBUG) || defined(DEBUG_OUTPUT)
 char buf[2];
 buf[0]=bookends;
 buf[1]='\0';
 Strings words;
 words.SplitPush( (char *) fmt.c_str() );
 string out=string("");
 EACH(words.first,String,s) {
  if ( !str_cmp(s->s.c_str(),"###") ) out+=string(buf)+value+string(buf);
  else out+=s->s;
 }
 pthreads.Unlock();
 OutputDebugString(TEXT((char *)out.c_str()));
#endif
}

#define FORMAT_STRING_SIZE 65536
string FORMAT(char *b, int len, const char *fmt, ...)
{
 pthreads.Lock();
 string s;
 static char buf [FORMAT_STRING_SIZE];
 if ( strlen(fmt) > FORMAT_STRING_SIZE/4 ) {
  pthreads.Unlock();
  OUTPUT("FORMAT(): Warning, format string longer than maximum length.\n");
  pthreads.Lock();
 }
 else if ( len > FORMAT_STRING_SIZE ) {
  pthreads.Unlock();
  OUTPUT("FORMAT(): Warning, provided buffer length exceeds maximum.\n" );
  pthreads.Lock();
 }
 va_list args;
 va_start (args, fmt);
 vsprintf_s (buf, fmt, args);
 va_end (args);
 s=string(buf);
 const char *cstr=s.c_str();
 for ( int i=0; i<len; i++ ) {
  b[i]=cstr[i];
  if ( cstr[i]=='\0' ) break;
 }
 b[len-1]='\0';
 pthreads.Unlock();
 return s;
}

string FORMAT(const char*fmt, ...) {
 pthreads.Lock();
 string s;
 static char buf[FORMAT_STRING_SIZE];
 va_list args;
 va_start( args, fmt );
 vsprintf_s(buf, fmt, args);
 va_end(args);
 s=string(buf);
 pthreads.Unlock();
 return s;
}

// avoids ambiguity in the compiler, is exactly like FORMAT(fmt,...)
string FMT(const char*fmt, ...) {
 pthreads.Lock();
 string s;
 static char buf[FORMAT_STRING_SIZE];
 va_list args;
 va_start( args, fmt );
 vsprintf_s(buf, fmt, args);
 va_end(args);
 s=string(buf);
 pthreads.Unlock();
 return s;
}

// avoids ambiguity in the compiler, is exactly like FORMAT(fmt,...)
string FORMATv(const char*fmt, ...) {
 pthreads.Lock();
 string s;
 static char buf[FORMAT_STRING_SIZE];
 va_list args;
 va_start( args, fmt );
 vsprintf_s(buf, fmt, args);
 va_end(args);
 s=string(buf);
 pthreads.Unlock();
 return s;
}

#if defined(MSVC2013)
string FMT(const string &fmt, ...) {
 pthreads.Lock();
 string result;
 va_list args;
 va_start(args, fmt.c_str());
 result=FMTchar(fmt.c_str(),args);
 va_end(args);
 pthreads.Unlock();
 return result;
}
#endif

// to be converted into a new format
string FMTchar(const char *fmt, ...) {
 pthreads.Lock();
 string result;
 va_list args;
 va_start(args, fmt);
 result=FORMAT(fmt,args);
 va_end(args);
 pthreads.Unlock();
 return result;
}

/*
 * Compare strings, case insensitive.
 */
bool contains( const char *astr, const char *bstr )
{
 if ( astr == null ) {
  OUTPUT( "Str_cmp: null astr.\n" );
  return false;
 }
 if ( bstr == null ) {
  OUTPUT( "Str_cmp: null bstr.\n" );
  return false;
 }
 for ( ; *astr || *bstr; astr++, bstr++ ) if ( LOWER(*astr) != LOWER(*bstr) ) return false;
 return true;
}

bool endsWith( const char *searching, const char *find, bool removePadding ) {
 if ( searching == null ) {
  OUTPUT( "endsWith: null searching.\n" );
  return false;
 }
 if ( find == null ) {
  OUTPUT( "endsWith: null find.\n" );
  return false;
 }
 int L1=(int)strlen(searching);
 if ( L1 == 0 ) return false;
 int L2=(int)strlen(find);
 if ( L1<L2 ) return false;
 if ( L2 == 0 ) {
  OUTPUT("endsWith() warns find string was empty, returning false\n");
  return false;
 }
 string unpadded;
 if ( !removePadding ) unpadded=string(searching);
 else {
  unpadded=string(searching);
  trim(&unpadded);
  L1=(int)unpadded.length();
  if ( L1<L2 ) return false;
 }
 const char *tosearch=unpadded.c_str();
 int x=L1-1;
 int y=L2-1;
 while ( y>=0 ) {
  if ( LOWER(tosearch[x]) != LOWER(find[y]) ) {
   return false;
  }
  x--;
  y--;
 }
 return true;
}

bool hasEnding(std::string const &fullString, std::string const &ending) {
 if (fullString.length() >= ending.length()) {
  return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
 } else {
  return false;
 }
}

bool hasEnding( const char *searching, const char *find, bool caseSensitive, bool unpad ) {
 string fullString;
 string ending;
 if ( !caseSensitive ) {
  fullString=lower(string(searching));
  ending=lower(string(find));
 } else {
  fullString=string(searching);
  ending=string(find);
 }
 if ( unpad ) {
  trim(&fullString);
 }
 if (fullString.length() >= ending.length()) {
  return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
 } else {
  return false;
 }
}

bool hasEnding( string const &searching, const char *find, bool caseSensitive, bool unpad ) {
 string fullString=searching;
 string ending=string(find);
 if ( !caseSensitive ) {
  fullString=lower(fullString);
  ending=lower(ending);
 }
 if ( unpad ) {
  trim(&fullString);
 }
 if (fullString.length() >= ending.length()) {
  return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
 } else {
  return false;
 }
}

bool endsWith(string const &fullString, string const &ending) {
 string a=fullString;
 trim(&a);
 if (a.length() >= ending.length()) {
  return (0 == a.compare(a.length() - ending.length(), ending.length(), ending));
 } else {
  return false;
 }
}

bool hasEnding( string const &searching, string const &find, bool caseSensitive, bool unpad ) {
 string fullString=searching;
 string ending=find;
 if ( !caseSensitive ) {
  fullString=lower(fullString);
  ending=lower(ending);
 }
 if ( unpad ) {
  trim(&fullString);
 }
 if (fullString.length() >= ending.length()) {
  return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
 } else {
  return false;
 }
}

void rtrim(string& s, const string& delimiters = " \f\n\r\t\v" ) {
   s.erase( s.find_last_not_of( delimiters ) + 1 );
}

void ltrim(string& s,  const string& delimiters = " \f\n\r\t\v" )
{
   s.erase( 0, s.find_first_not_of( delimiters ) );
}
 
void trim(string& s, const string& delimiters = " \f\n\r\t\v" )
{
    s.erase( s.find_last_not_of( delimiters ) + 1 ).erase( 0, s.erase( s.find_last_not_of( delimiters ) + 1 ).find_first_not_of( delimiters ) );
}

void replaceOnce(string& in, string const &search, string const &substitute) {
 int found=0;
 string::size_type pos=0;
 while((pos=in.find(search, pos))!=string::npos&&found<1)
 {
  in.erase(pos, search.length());
  in.insert(pos, substitute);
  pos+=substitute.length();
  found++;
 }
}

void replaceAll(string& in, string const &search, string const &substitute) {
 string::size_type pos=0;
 while((pos=in.find(search, pos))!=string::npos)
 {
  in.erase(pos, search.length());
  in.insert(pos, substitute);
  pos+=substitute.length();
 }
}

string replace_all(const string& in, const string& search, const string& substitute) {
 string::size_type pos=0;
 string out=in;
 while((pos=out.find(search, pos))!=string::npos)
 {
  out.erase(pos, search.length());
  out.insert(pos, substitute);
  pos+=substitute.length();
 }
 return out;
}

string UnStringSpecialCodes( string k ) {
 return replace_all(replace_all(k,string("#*$#-1-#$*#"),string("{")),string("#*$#-2-#$*#"),string("}"));
}

string StringSpecialCodes( string k ) {
 return replace_all(replace_all(k,string("{"),string("#*$#-1-#$*#")),string("}"),string("#*$#-2-#$*#"));
}

// Used in textbox entry, this replaces the last space with a newline.
std::string word_wrap_for_textbox( const char * p ) {
 string lines=string(p);
 size_t position;
 if ( (position=lines.find_last_of(' ')) == string::npos ) {
  return lines+string("\n");
 }
 return lines.replace(position,1,"\n");
}

int last_line_length( const char * p ) {
 const char *w=p;
 int length=0;
 while ( *w != '\0' ) {
  w++;
  length++;
  if ( *w == '\n' ) {
   length=0;
  }
 }
 return length;
}

int longest_line_length( const char * p ) {
 const char *w=p;
 int longest=0;
 int length=0;
 while ( *w != '\0' ) {
  w++;
  length++;
  if ( *w == '\0' || *w == '\n' ) {
   if ( longest < length ) longest=length;
   length=0;
  }
 }
 return longest;
}

int char_is_of( char c, const char *list ) {
 int o,O;
 O=(int)strlen(list);
 for ( o=0; o<O; o++ ) if ( list[o] == c ) return o;
 return -1;
}

char randomChar( const char *list ) {
 int c=(int)upto((int)strlen(list));
 return list[c];
}

bool char_in(char c, const char *list ) {
 int o,O;
 O=(int)strlen(list);
 for ( o=0; o<O; o++ ) if ( list[o] == c ) return true;
 return false;
}


char shift_support( char c ) {
static char *key_no_shift = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./";
static char *key_shifted  = "~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
int code=char_is_of(c,key_no_shift);
if ( code == -1 ) return c;
return key_shifted[code];
}

/*
 * Smash all \r's and tildes for file writes.
 */
char *fix_string( const char *str )
{
 static char strfix[STRING_SIZE];
 int i,o;
 if ( str == null ) return '\0';
 for ( o = i = 0; str[i+o] != '\0'; i++ ) {
  if (str[i+o] == '\r' || str[i+o] == '~') o++;
  strfix[i] = str[i+o];
 }
 strfix[i] = '\0';
 return (char *) strfix;
}

/*
 * Sees if last char is 's' and returns 'is' or 'are' 
 * if the word is not like 'dress' 
 */
char * is_are( const char *text )
{
 while ( *text != '\0' ) text++;
 /* Special cases. */
 if ( !str_infix( text, "kris" ) ) return "is";
 text--;
 if ( LOWER(*text) == 's' && LOWER(*(--text)) != 's' ) return "are";
 else return "is";
}

/*
 * Smashes all occurances of a string from a larger string.
 * Arguments must be seperated by a space.
 */
char *smash_arg( const char *text, const char *name )
{
 const char *arg;
 static char buf[STRING_SIZE];
 static char buf2[STRING_SIZE];
 buf2[0] = '\0';
 arg = one_argument( text, buf );
 while ( buf[0] != '\0' ) {
  if ( str_cmp( buf, name ) ) {
   strcat_s( buf2, buf );
   strcat_s( buf2, " " );
  }
  arg = one_argument( arg, buf );
 }
 return (char *) buf2;
}

/*
 * Grab 'a' 'an's and 'the's and smash the living hell out of them.
 * Good for use with manipulation of prop names.  Only destroys first
 * occurance.
 */
char *smash_article( const char *text )
{
 const char *arg;
 static char buf[STRING_SIZE];
 static char buf2[STRING_SIZE];
 one_argument( text, buf );
 if ( !str_cmp( buf, "the" ) || !str_cmp( buf, "an" ) || !str_cmp( buf, "a" ) ) {
  arg = one_argument( text, buf );
  FORMAT( buf2,STRING_SIZE, "%s", arg );
 }
 else strcpy_s( buf2, text );
 return (char *) buf2;
}

/*
 * Pluralize English objective statements properly.
 */
char *pluralize(const char *argument )
{
    static char buf[STRING_SIZE];
    char *v;

    FORMAT( buf,STRING_SIZE, "%s", smash_article(argument) );

    if ( strstr( buf, "some" ) ) return (char *) buf;
    v = strstr( buf, " of " );
    if ( v==null ) v = strstr( buf, " to " );

    if ( v == null )
    {
        if ( LOWER(buf[strlen(buf)-1]) == 'y' )
        {
            if ( LOWER(buf[strlen(buf)-2]) == 'a' ) 
            {
              strcat_s( buf, "s" );
            }
            else {
              buf[strlen(buf)-1] = 'i';
              strcat_s( buf, "es" );
            }
        }
        else
        if ( LOWER(buf[strlen(buf)-1]) == 'f' 
          && LOWER(buf[strlen(buf)-2]) != 'f'
          && (LOWER(buf[strlen(buf)-2]) != 'i'
            && LOWER(buf[strlen(buf)-3]) == 'o') )
        {
            buf[strlen(buf)-1] = 'v';
            strcat_s( buf, "es" );
        }
        else
        if ( LOWER(buf[strlen(buf)-1]) == 'h' )
        strcat_s( buf, "es" );
        else
        if ( LOWER(buf[strlen(buf)-1]) == 'x' )
        strcat_s( buf, "es" );
        else            
        if ( LOWER(buf[strlen(buf)-1]) == 's' )
        {
            if ( LOWER(buf[strlen(buf)-2]) == 'u'
              && !IS_VOWEL(LOWER(buf[strlen(buf)-3])) )
            {
                buf[strlen(buf)-2] = 'i';
                buf[strlen(buf)-1] = '\0';
            }
            else
            strcat_s( buf, "es" );
        }
        else
        strcat_s( buf, "s" );
    }
    else
    {
        char xbuf[STRING_SIZE];

        FORMAT( xbuf,STRING_SIZE, "%s", v );
        buf[strlen(buf)-strlen(v)] = '\0';

        if ( LOWER(buf[strlen(buf)-1]) == 'y' )
        {
            if ( LOWER(buf[strlen(buf)-2]) == 'a' ) 
            {
              strcat_s( buf, "s" );
            }
            else {
              buf[strlen(buf)-1] = 'i';
              strcat_s( buf, "es" );
            }
        }
        else
        if ( LOWER(buf[strlen(buf)-1]) == 'f' 
          && LOWER(buf[strlen(buf)-2]) != 'f'
          && (LOWER(buf[strlen(buf)-2]) != 'i'
            && LOWER(buf[strlen(buf)-3]) == 'o') )
        {
            buf[strlen(buf)-1] = 'v';
            strcat_s( buf, "es" );
        }
        else
        if ( LOWER(buf[strlen(buf)-1]) == 'h' )
        strcat_s( buf, "es" );
        else
        if ( LOWER(buf[strlen(buf)-1]) == 'x' )
        strcat_s( buf, "es" );
        else            
        if ( LOWER(buf[strlen(buf)-1]) == 's' )
        {
            if ( LOWER(buf[strlen(buf)-2]) == 'u'
              && !IS_VOWEL(LOWER(buf[strlen(buf)-3])) )
            {
                buf[strlen(buf)-2] = 'i';
                buf[strlen(buf)-1] = '\0';
            }
            else
            strcat_s( buf, "es" );
        }
        else
        strcat_s( buf, "s" );

        strcat_s( buf, xbuf );
    }

    return (char *) buf;
}


/*
 * Word wrap a string.
 */
string wordwrap( const char *oldstring, int charWidth ) {
 Zstring content(oldstring);
 content("\n"," ");
 content("\r","");
 Strings words;
 words.SplitPush(content.c_str(),' ');
 Zstring out;
 int current_line_length=0;
 EACH(words.first,String,s) if ( s->s.length() > 0 ) {
  if ( current_line_length+(int)s->s.length() > charWidth ) {
   if ( (int)s->s.length() > charWidth ) { // Too long for a line...
    out+=string("\n")+s->s+string(" ")+string("\n");
    current_line_length=0;
   } else {
    out+=string("\n")+s->s+string(" ");
    current_line_length=s->s.length()+1;
   }
  } else out+=s->s+string(" ");
  current_line_length+=s->s.length()+1;
 }
 out.rTrim(" ");
 return out.value;
}

/*
 * Formats as though the text was a single paragraph.
 */
string paragraph(const char *oldstring, const char *prefix, int width, bool fEnd )
{
 static char xbuf[STRING_SIZE];
 static char xbuf2[STRING_SIZE];
 char *rdesc;
 int i=0;
 bool cap=true;
 xbuf[0]=xbuf2[0]=0;
 i = 0;
 for ( rdesc = (char *) oldstring; *rdesc; rdesc++ ) {
  if ( *rdesc == '\n' ) {
   if ( xbuf[i-1] != ' ' ) {
    xbuf[i] = ' ';
    i++;
   }
  }
  else if ( *rdesc == '\r' ) ;
  else if ( *rdesc == ' ' ) {
   if ( xbuf[i-1] != ' ' ) {
    xbuf[i]=' ';
    i++;
   }
  }
  else if ( *rdesc == ')' ) {
   if ( xbuf[i-1] == ' ' && xbuf[i-2] == ' '
     && ( xbuf[i-3] == '.' || xbuf[i-3] == '?' || xbuf[i-3] == '!' ) ) {
    xbuf[i-2]=*rdesc;
    xbuf[i-1]=' ';
    xbuf[i]=' ';
    i++;
   }
   else { xbuf[i]=*rdesc; i++; }
  }
  else if ( *rdesc == '.' || *rdesc == '?' || *rdesc == '!' ) {
   cap = true;
   if ( xbuf[i-1] == ' ' && xbuf[i-2] == ' '
     && ( xbuf[i-3] == '.' || xbuf[i-3] == '?' || xbuf[i-3] == '!' ) ) {
    xbuf[i-2]=*rdesc;
    if ( *(rdesc+1) != '\"' ) {
     xbuf[i-1]=' ';
     xbuf[i]=' ';
     i++;
    } else {
     xbuf[i-1]='\"';
     xbuf[i]=' ';
     xbuf[i+1]=' ';
     i+=2;
     rdesc++;
    }
   } else {
    xbuf[i]=*rdesc;
    if (*(rdesc+1) != '\"') {
     if ( *(rdesc+1) == ' ' ) {
      xbuf[i+1]=' ';
      xbuf[i+2]=' ';
      i += 3;
     } else {
      cap = false;
      xbuf[i] = *rdesc;
      i++;
     }
    } else {
     xbuf[i+1]='\"';
     xbuf[i+2]=' ';
     xbuf[i+3]=' ';
     i += 4;
     rdesc++;
    }
   }
  } else {
   xbuf[i]=*rdesc;
   if ( cap ) {
    cap = false;
    xbuf[i] = UPPER( xbuf[i] );
   }
   i++;
  }
 }
 xbuf[i]=0;
 strcpy_s(xbuf2,xbuf);
 rdesc=xbuf2;
 xbuf[0]=0;
 for ( ; ; ) {
  for ( i = 0; i < width+1; i++ ) if ( !*(rdesc+i) ) break;
  if ( i < width+1 ) break;
  for ( i = xbuf[0] ? width : width-3; i; i-- ) if ( *(rdesc+i) == ' ' ) break;
  if ( i ) {
   *(rdesc+i)=0;
   strcat_s(xbuf,rdesc);
   strcat_s(xbuf,"\n\r");
   strcat_s(xbuf,prefix);
   rdesc += i+1;
   while (*rdesc == ' ') rdesc++;
  } else {
   printf ("No spaces");
   *(rdesc+width-1)=0;
   strcat_s(xbuf,rdesc);
   strcat_s(xbuf,"-\n\r");
   rdesc += width;
  }
 }
 while (*(rdesc+i) && (*(rdesc+i)==' '|| *(rdesc+i)=='\n'|| *(rdesc+i)=='\r')) i--;
 *(rdesc+i+1)=0;
 strcat_s(xbuf,rdesc);
 if ( fEnd && xbuf[strlen(xbuf)-2] != '\n') strcat_s(xbuf,"\n\r");
 if ( xbuf[strlen(xbuf)-1] == ' ' ) xbuf[strlen(xbuf)-1] = '\0';
 if ( xbuf[strlen(xbuf)-1] == ' ' ) xbuf[strlen(xbuf)-1] = '\0';
 delete oldstring;
 string final=xbuf;
 return(final);
}

/*
 * Replace a substring with a new substring, return the new version.
 * (make sure to CopyString() when needed)
 */
char *replace_string(const char *orig, const char *old, const char *n )
{
 static char xbuf[STRING_SIZE];
 int i;
 xbuf[0] = '\0';
 strcpy_s(xbuf, orig);
 if ( strstr(orig,old) != null ) {
  i = (int) ( strlen( orig ) - strlen( strstr(orig, old) ) );
  xbuf[i]  = '\0';
  strcat_s( xbuf, n);
  strcat_s( xbuf, &orig[i+strlen( old )] );
 }
 return (char *) ( xbuf );
}

void replace_char( char *orig, char old, char n)
{
 while (*orig) if (*orig == old) *orig++ = n;
  else orig++;
}

/*
 * Counts the number of "words" (strings of non-spaces separated by spaces)
 * in a string.     Not the best way (should use one_arg)?
 */
int words(const char *argument )
{
 int total;
 char *s = (char *) argument;
 total = 0;
 while ( *s != '\0' ) 
  if ( *s != ' ' ) {
   total++;
   while ( *s != ' ' && *s != '\0' ) s++;
  } else s++;
 return total;
}

/*
 * Returns a string with one space between each argument. 
 */
const char * string_unpad( const char * argument )
{
 static Zstring a(argument);
 a.Trim();
 return a.value.c_str();
}

/*
 * First char is capital, the rest is lowercase or something
 * to that effect.  Similar to ucfirst() from PHP
 */
const char * string_proper( const char * argument ) {
 static Zstring a(argument);
 static Zstring out;
 out="";
 const char *p=a.c_str();
 while ( *p!='\0' ) {
  char c=(*p);
  if ( *p==' ' ) { out+=string(" "); while ( *p==' ' ) p++; }
  if ( *p!='\0' ) {
   out+=FORMAT("%c",UPPER(*p));
   p++;
   while ( IS_ALPHA(*p) ) { out+=(*p); p++; }
  }
 }
 return out.c_str();
}

inline bool is_digit( char c ) {
 return (c =='0' || c == '1' || c == '2' || c == '3' || c == '4' || c =='5' || c =='6' || c=='7' || c == '8' || c == '9' );
}
             
/*
 * Return true if an argument is completely numeric.
 */
bool is_integer( const char *arg ) {
 const char *o=arg;
 bool foundspace=false;
 if ( *arg == '\0' ) return false;
 if ( *arg == '-' ) arg++;
 for ( ; *arg != '\0'; arg++ ) {
  if ( !is_digit(*arg) ) {
   if ( _FILLER(*arg) ) foundspace=true;
   else return false;
  }
 }
 if ( foundspace ) {
  if ( words(o) == 1 ) return true;
  return false;
 }
 return true;
}

/*
 * Return true if an argument is completely numeric.
 */
bool is_decimal( const char *arg ) {
 const char *o=arg;
 int periods=0;
 bool foundspace=false;
 if ( *arg == '\0' ) return false;
 while ( _FILLER(*arg) ) arg++;
 if ( *arg == '-' ) arg++;
 for ( ; *arg != '\0'; arg++ ) {
  if ( !is_digit(*arg) ) {
   if ( *arg == '.' ) {
    if ( periods >=1 ) return false;
    periods++;
    arg++;
   } else
   if ( _FILLER(*arg) ) foundspace=true;
   else return false;
  }
 }
 if ( foundspace ) {
  if ( words(o) == 1 ) return true;
  return false;
 }
 return true;
}

/*
 * Same as above but uses atoi error handling, decimal only (base-10, no floats).
 * Does not return the value if valid (tiny inefficiency).
 * Source: http://www.gidforums.com/t-17778.html
 */
bool is_int( string k ) {
 const char *in=k.c_str();
 char * last = 0;

 int x1 = atoi(in);
 int x2 = strtol(in, &last, 10);
 if ( *last == '\0' ) // was valid or empty
  if ( k.length() > 0 ) return false;
 return true;
}

/*
 * Same as above but uses atoi error handling.  Only base 10 checked.
 * Does not return the value if valid (tiny inefficiency).
 * Source: http://www.gidforums.com/t-17778.html
 */
bool is_hex( string k ) {
 const char *in=k.c_str();
 char *last = null;
 int x1 = atoi(in);
 long x2 = strtol(in, &last, 16);
 if ( last && *last == '\0' ) // was valid or empty
  if ( k.length() > 0 ) return false;
 return true;
}

/*
 * Same as above but uses atoi error handling, decimal only (base-10, no floats).
 * Does not return the value if valid (tiny inefficiency).
 * Source: http://www.gidforums.com/t-17778.html
 */
bool is_float( string k ) {
 char * last = 0;
 double x1 = atof(k.c_str());
 long x2 = strtol(k.c_str(), &last, 10);
 if ( *last == '\0' ) // was valid or empty returns false positive (indicating no error, but does not specify why)
  if ( k.length() > 0 ) return false;
 return true;
}

int from_hex( string hex ) {
 int result;
 sscanf_s(hex.c_str(), "%x", &result);
 return result;
}

string to_hex( int i ) {
 char buf[80];
 FORMAT(buf,80,"%x", (int) i);
 string out=string(buf);
 if ( out.length() == 1 ) out=string("0")+out; // A==0A
 return out;
}

/* Pick off one argument from a string and return the rest.
 * Understands quotes and {}=[], treats interim commas and equal signs as space.
 * Uses the stack so not recommended for large strings (use string_argument);
 */
const char *one_argument( const char *args, char *arg_first )
{
 const char *argument=args;
 char cEnd = ' ';
 if ( argument == null ) return &string_error;
 // Advance past spaces and interim commas, equal signs, newlines, skip //comments and #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '/' && *(argument+1) == '/' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 if ( _SEP(*argument) ) {
  cEnd = *argument;
  argument++;
 }
 if ( _NESTERS(*argument) ) { // Handle nested {} or []
  argument++;
  int nests=1;
  while ( *argument != '\0' && _NESTERE(*argument) && nests > 0 ) {
   if ( _NESTERE(*argument) ) {
    nests--;
    argument++;
    if ( nests == 0 ) break;
   } else if ( *argument == '{' || *argument == '[' ) nests++;
   *arg_first=*argument;
   argument++;
   arg_first++;
  }
 } else { // Quotes are the delimiters
  while ( *argument != '\0' ) {
   if ( *argument == '\n' ) {
    argument++; 
    if ( cEnd == ' ' ) break;
   }
   if ( *argument == '\r' ) {
    argument++;
    if ( cEnd == ' ' ) break;
   }
   if ( *argument == ',' && cEnd == ' ' ) break; // end on a comma unless quotes are cEnd
   if ( *argument == '=' && cEnd == ' ' ) break; // end on equal sign unless quotes are cEnd
   if ( *argument == cEnd ) {
    argument++;
    break;
   }
   *arg_first = LOWER(*argument);
   arg_first++;
   argument++;
  }
 }
 *arg_first = '\0';
 // Advance past spaces and interim commas, equal signs, newlines, skip #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 return argument;
}

/* Pick off one argument from a string and return the rest.
 * Understands quotes and {}=[], treats interim commas and equal signs as space.
 * Uses the stack so not recommended for large strings (use string_argument);
 */
const char *one_argument_case( const char *args, char *arg_first )
{
 const char *argument=args;
 char cEnd = ' ';
 if ( argument == null ) return &string_error;
 // Advance past spaces and interim commas, equal signs, newlines, skip //comments and #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '/' && *(argument+1) == '/' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 if ( _SEP(*argument) ) {
  cEnd = *argument;
  argument++;
 }
 if ( _NESTERS(*argument) ) { // Handle nested {} or []
  argument++;
  int nests=1;
  while ( *argument != '\0' && _NESTERE(*argument) && nests > 0 ) {
   if ( _NESTERE(*argument) ) {
    nests--;
    argument++;
    if ( nests == 0 ) break;
   } else if ( *argument == '{' || *argument == '[' ) nests++;
   *arg_first=*argument;
   argument++;
   arg_first++;
  }
 } else { // Quotes are the delimiters
  while ( *argument != '\0' ) {
   if ( *argument == '\n' ) {
    argument++; 
    if ( cEnd == ' ' ) break;
   }
   if ( *argument == '\r' ) {
    argument++;
    if ( cEnd == ' ' ) break;
   }
   if ( *argument == ',' && cEnd == ' ' ) break; // end on a comma unless quotes are cEnd
   if ( *argument == '=' && cEnd == ' ' ) break; // end on equal sign unless quotes are cEnd
   if ( *argument == cEnd ) {
    argument++;
    break;
   }
   *arg_first = (*argument);
   arg_first++;
   argument++;
  }
 }
 *arg_first = '\0';
 // Advance past spaces and interim commas, equal signs, newlines, skip #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 return argument;
}

/* Pick off one argument from a string and return the rest.
 * Uses std::string thus dynamic and doesn't use the stack.
 * Understands quotes and {}=[](), treats interim commas and equal signs as space.
 */
const char *FSstring_argument( const char *argument, bool *hit_spacer_eol, string *arg_first, bool slash_comments, bool pound_comments )
{
 (*hit_spacer_eol)=false;
 char cEnd = ' ';
// char buf[2];
 (*arg_first)=string("");
 if ( argument == null ) return &string_error; // undef behavior? maybe "" is constant
 // Advance past spaces and interim commas, equal signs, newlines, skip //comments and #comments
 while ( _FILLER(*argument) || (*argument == '#' && pound_comments) ) {
  if ( slash_comments && *argument == '/' && *(argument+1) == '/' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else if ( pound_comments && *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 // Handle nested {} [] (), or quotes "" '' ``
 if ( _NESTERS(*argument) || _SEP(*argument) ) { // Delimiters
  int nests=1;
  char cStart=*argument;
  argument++;
  switch ( cStart ) {
   case '{': cEnd = '}'; break;
   case '[': cEnd = ']'; break;
   case '(': cEnd = ')'; break;
  case '\'': cEnd = '\''; break;
   case '"': cEnd = '"'; break;
   case '`': cEnd = '`'; break;
  }
  while ( *argument != '\0' && nests > 0 ) {
   if ( *argument == cEnd ) {
    nests--;
    if ( nests == 0 ) break;
   } else if ( *argument == cStart ) nests++;
   (*arg_first)+=(*argument);
   argument++;
  }
  argument++;
 } /*else if ( _EXPRESSIVE(*argument) ) {
  (*arg_first)+=*argument;
 }*/ else { // No delimiters, keep case, stop when you hit = , [ { ( + / - * % ! <space>, support hit_spacer_eol
  while ( *argument != '\0' ) {
   if ( *argument == '\n' ) {
    (*hit_spacer_eol)=true;
    argument++;
    break;
   } else if ( char_in( *argument, "\r[{(,= " ) ) break;
   (*arg_first)+=(*argument);//string(buf);
   argument++;
  }
 }
 // Advance past spaces and interim commas, equal signs, newlines, skip #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '\n' ) {
   (*hit_spacer_eol)=true;
   break;
  }
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 return argument;
}


/* Pick off one argument from a string and return the rest.
 * Uses std::string thus dynamic and doesn't use the stack.
 * Understands quotes and {}=[](), treats interim commas and equal signs as space.
 */
const char *string_argument( const char *argument, string *arg_first, bool slash_comments, bool pound_comments )
{
 char cEnd = ' ';
// char buf[2];
 (*arg_first)=string("");
 if ( argument == null ) return &string_error; // undef behavior? maybe "" is constant
 // Advance past spaces and interim commas, equal signs, newlines, skip //comments and #comments
 while ( _FILLER(*argument) || (*argument == '#' && pound_comments) ) {
  if ( slash_comments && *argument == '/' && *(argument+1) == '/' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else if ( pound_comments && *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 // Handle nested {} [] (), or quotes "" '' ``
 if ( _NESTERS(*argument) || _SEP(*argument) ) { // Delimiters
  int nests=1;
  char cStart=*argument;
  argument++;
  switch ( cStart ) {
   case '{': cEnd = '}'; break;
   case '[': cEnd = ']'; break;
   case '(': cEnd = ')'; break;
  case '\'': cEnd = '\''; break;
   case '"': cEnd = '"'; break;
   case '`': cEnd = '`'; break;
  }
  while ( *argument != '\0' && nests > 0 ) {
   if ( *argument == cEnd ) {
    nests--;
    if ( nests == 0 ) break;
   } else if ( *argument == cStart ) nests++;
   (*arg_first)+=(*argument);
   argument++;
  }
  argument++;
 } /*else if ( _EXPRESSIVE(*argument) ) {
  (*arg_first)+=*argument;
 }*/ else { // No delimiters, lower case, stop when you hit = , [ { ( + / - * % ! <space>
  while ( *argument != '\0' ) {
   if ( char_in( *argument, "\n\r[{(,= " ) ) break;
   (*arg_first)+=LOWER(*argument);//string(buf);
   argument++;
  }
 }
 // Advance past spaces and interim commas, equal signs, newlines, skip #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 return argument;
}

/* Pick off one argument from a string and return the rest.
 * Uses std::string thus dynamic and doesn't use the stack.
 * Understands quotes and {}=[](), treats interim commas and equal signs as space.
 */
const char *string_argument( const char *argument, Zstring *arg_firstz )
{
 string *arg_first=&arg_firstz->value;
 char cEnd = ' ';
// char buf[2];
 (*arg_first)=string("");
 if ( argument == null ) return &string_error; // undef behavior? maybe "" is constant
 // Advance past spaces and interim commas, equal signs, newlines, skip //comments and #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '/' && *(argument+1) == '/' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 // Handle nested {} [] (), or quotes "" '' ``
 if ( _NESTERS(*argument) || _SEP(*argument) ) { // Delimiters
  int nests=1;
  char cStart=*argument;
  argument++;
  switch ( cStart ) {
   case '{': cEnd = '}'; break;
   case '[': cEnd = ']'; break;
   case '(': cEnd = ')'; break;
  case '\'': cEnd = '\''; break;
   case '"': cEnd = '"'; break;
   case '`': cEnd = '`'; break;
  }
  while ( *argument != '\0' && nests > 0 ) {
   if ( *argument == cEnd ) {
    nests--;
    if ( nests == 0 ) break;
   } else if ( *argument == cStart ) nests++;
   (*arg_first)+=(*argument);
   argument++;
  }
  argument++;
 } else { // No delimiters, lower case, stop when you hit = , [ { (
  while ( *argument != '\0' ) {
   if ( char_in( *argument, "\n\r[{(,= " ) ) break;
   (*arg_first)+=LOWER(*argument);//string(buf);
   argument++;
  }
 }
 // Advance past spaces and interim commas, equal signs, newlines, skip #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 arg_firstz->length=(int)arg_firstz->value.length();
 return argument;
}

/* Pick off one argument from a string and return the rest without lowering case.
 * Uses std::string thus dynamic and doesn't use the stack.
 * Understands quotes and {}=[](), treats interim commas and equal signs as space.
 */
const char *string_argument_case( const char *argument, string *arg_first )
{
 char cEnd = ' ';
// char buf[2];
 (*arg_first)=string("");
 if ( argument == null ) return &string_error; // undef behavior? maybe "" is constant
 // Advance past spaces and interim commas, equal signs, newlines, skip //comments and #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '/' && *(argument+1) == '/' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 // Handle nested {} [] (), or quotes "" '' ``
 if ( _NESTERS(*argument) || _SEP(*argument) ) { // Delimiters
  int nests=1;
  char cStart=*argument;
  argument++;
  switch ( cStart ) {
   case '{': cEnd = '}'; break;
   case '[': cEnd = ']'; break;
   case '(': cEnd = ')'; break;
  case '\'': cEnd = '\''; break;
   case '"': cEnd = '"'; break;
   case '`': cEnd = '`'; break;
  }
  while ( *argument != '\0' && nests > 0 ) {
   if ( *argument == cEnd ) {
    nests--;
    if ( nests == 0 ) break;
   } else if ( *argument == cStart ) nests++;
   (*arg_first)+=(*argument);
   argument++;
  }
  argument++;
 } else { // No delimiters, lower case, stop when you hit = , [ { (
  while ( *argument != '\0' ) {
   if ( char_in( *argument, "\n\r[{(,= " ) ) break;
   (*arg_first)+=(*argument);//string(buf);
   argument++;
  }
 }
 // Advance past spaces and interim commas, equal signs, newlines, skip #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 return argument;
}


/* Pick off one argument from a string and return the rest without lowering case.
 * Uses std::string thus dynamic and doesn't use the stack.
 * Understands quotes and {}[]
 */
const char *string_argument_case_ignorequotes( const char *argument, string *arg_first )
{
 char cEnd = ' ';
// char buf[2];
 (*arg_first)=string("");
 if ( argument == null ) return &string_error; // undef behavior? maybe "" is constant
 // Advance past spaces and interim commas, equal signs, newlines, skip //comments and #comments
 while ( _FILLERnocommaequals(*argument) || *argument == '#' ) {
  if ( *argument == '/' && *(argument+1) == '/' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 // Handle nested {} []
 if ( _NESTERSnoparen(*argument) ) { // Delimiters
  int nests=1;
  char cStart=*argument;
  argument++;
  switch ( cStart ) {
   case '{': cEnd = '}'; break;
   case '[': cEnd = ']'; break;
  }
  while ( *argument != '\0' && nests > 0 ) {
   if ( *argument == cEnd ) {
    nests--;
    if ( nests == 0 ) break;
   } else if ( *argument == cStart ) nests++;
   (*arg_first)+=(*argument);
   argument++;
  }
  argument++;
 } else { // No delimiters, lower case, stop when you hit  [ { (
  while ( *argument != '\0' ) {
   if ( char_in( *argument, " \n\r[{" ) ) break;
   (*arg_first)+=(*argument);//string(buf);
   argument++;
  }
 }
 // Advance past spaces and interim commas, equal signs, newlines, skip #comments
 while ( _FILLERnocommaequals(*argument) || *argument == '#' ) {
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 return argument;
}



/* Pick off one argument from a string and return the rest without lowering case.
 * Uses std::string thus dynamic and doesn't use the stack.
 * Understands quotes and {}=[](), treats interim commas and equal signs as space.
 * Does not filter any characters except fillers (comma, space, tab, newline)
 */
const char *string_argument_case( bool distinguishers, const char *argument, string *arg_first )
{
 char cEnd = ' ';
// char buf[2];
 (*arg_first)=string("");
 if ( argument == null ) return &string_error; // undef behavior? maybe "" is constant
 // Advance past spaces and interim commas, equal signs, newlines, skip //comments and #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '/' && *(argument+1) == '/' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 // Handle nested {} [] (), or quotes "" '' ``
 if ( _NESTERS(*argument) || _SEP(*argument) ) { // Delimiters
  int nests=1;
  char cStart=*argument;
  (*arg_first) += cStart;
  argument++;
  switch ( cStart ) {
   case '{': cEnd = '}'; break;
   case '[': cEnd = ']'; break;
   case '(': cEnd = ')'; break;
  case '\'': cEnd = '\''; break;
   case '"': cEnd = '"'; break;
   case '`': cEnd = '`'; break;
  }
  while ( *argument != '\0' && nests > 0 ) {
   if ( *argument == cEnd ) {
    (*arg_first) += cEnd;
    nests--;
    if ( nests == 0 ) break;
   } else if ( *argument == cStart ) nests++;
   (*arg_first)+=(*argument);
   argument++;
  }
  argument++;
 } else { // No delimiters, lower case, stop when you hit = , [ { (
  while ( *argument != '\0' ) {
   if ( char_in( *argument, "\n\r[{(,= " ) ) break;
   (*arg_first)+=(*argument);//string(buf);
   argument++;
  }
 }
 // Advance past spaces and interim commas, equal signs, newlines, skip #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 return argument;
}

/* Pick off one argument from a string and return the rest.
 * Uses std::string thus dynamic and doesn't use the stack.
 * Understands quotes and {}=[](), treats interim commas and equal signs as space.
 */
const char *string_argument_end( const char *argument, string *arg_first, char *sep )
{
 char cEnd = ' ';
// char buf[2];
 (*arg_first)=string("");
 if ( argument == null ) return &string_error; // undef behavior? maybe "" is constant
 // Advance past spaces and interim commas, equal signs, newlines, skip //comments and #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '/' && *(argument+1) == '/' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 // Handle nested {} [] (), or quotes "" '' ``
 if ( char_in(*argument, "{([`'\"") ) { // Delimiters
  int nests=1;
  char cStart=*argument;
  switch ( cStart ) {
   case '{': cEnd = '}'; break;
   case '[': cEnd = ']'; break;
   case '(': cEnd = ')'; break;
  case '\'': cEnd = '\''; break;
   case '"': cEnd = '"'; break;
   case '`': cEnd = '`'; break;
  }
  argument++;
  while ( *argument != '\0' && nests > 0 ) {
   if ( *argument == cEnd ) {
    nests--;
    if ( nests == 0 ) break;
   } else if ( *argument == cStart ) nests++;
   /*buf[0]=*argument;
   buf[1]='\0';*/
   (*arg_first)+=(*argument);//string(buf);
   argument++;
  }
  argument++;
 } else { // No delimiters, lower case, stop when you hit = , [ { (
  while ( *argument != '\0' ) {
   if ( char_in( *argument, "\n\r[{(,= " ) ) { *sep=*argument; } break;
   /*buf[0]=(*argument);
   buf[1]='\0';*/
   (*arg_first)+=LOWER(*argument);//string(buf);
   argument++;
  }
 }
 // Advance past spaces and interim commas, equal signs, newlines, skip #comments
 while ( _FILLER(*argument) || *argument == '#' ) {
  if ( *argument == '#' ) while ( *argument != '\n' && *argument != '\r' && *argument != '\0' ) argument++;
  else argument++;
 }
 return argument;
}

string remove_outer_padding( const char *s ) {
 const char *p=s;
 int len=strlen(s);
 int i=len-1;
 if ( strlen(s) <= 0 ) return string("");
 const char *e=(p+(i));
 while ( *e != '\0' && *e != '\n' && _FILLERnocommaequals(*e) && i > 0 ) { e--; i--; }
 if ( i==0 ) return string("");
 if ( *e != '\0' && (_NESTERE(*e) || _SEP(*e)) ) { e--; i--; } // trim ending signal, if available.
 while ( *p != '\0' && _FILLERnocommaequals(*p) ) p++;
 if ( *p != '\0' && (_NESTERS(*p) || _SEP(*p) ) ) p++; // trim starting signal, if available
 Zstring out;
 while ( p != e && *p != '\0' ) { out+=(*p); p++; }
 if ( p == e ) out+=(*p);
 return out.value;
}

// Gather until next substring match or end of string (returns the point right after the substring)
const char *gather_to( const char *argument, const char *substr, string *buf ) {
 *buf=string("");
 int substrlen=(int)strlen(substr);
 const char *p=argument;
 bool found=false;
 while ( *p != '\0' && !found ) {
  if ( *p==*substr ) {
   const char *oldp=p;
   const char *q=substr;
   bool match=true;
   int i=0;
   while ( match && i<substrlen ) {
    if ( *p!=*q ) match=false;
    p++;
    q++;
    i++;
   }
   if ( match ) {
    found=true;
    return p;
   }
   else {
    p=oldp;
    *buf+=(*p);
    p++;
   }
  } else {
   *buf+=(*p);
   p++;
  }
 }
 return p;
}

/* Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
const char *first_split( const char *argument, char *arg_first, char sep )
{
 char cEnd;
 if ( argument == null ) return &string_error;
 while ( (*argument == ' ') || *argument == '\n' || *argument == '\r' ) argument++;
 cEnd = sep;
 if ( *argument == '\'' || *argument == '"' || *argument == '\'' ) cEnd = *argument++;
 else if ( *argument == '{' ) { cEnd='}'; argument++; }
 while ( *argument != '\0' ) {
  if ( *argument == '\n' ) { argument++; if ( cEnd == sep ) break; }
  if ( *argument == '\r' ) { argument++; if ( cEnd == sep ) break; }
  if ( *argument == cEnd ) { argument++; break; }
  *arg_first = LOWER(*argument);
  arg_first++;
  argument++;
 }
 *arg_first = '\0';
 while ( (*argument == sep) || *argument == '\n' || *argument == '\r' ) argument++;
 return argument;
}

/* Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
const char *one_argcase( const char *argument, char *arg_first )
{
 char cEnd;
 if ( argument == null ) return &string_error;
 while ( (*argument == ' ') || *argument == '\n' || *argument == '\r' )	argument++;
 cEnd = ' ';
 if ( *argument == '\'' || *argument == '"' || *argument == '\'' ) cEnd = *argument++;
 else if ( *argument == '{' ) { cEnd='}'; argument++; }
 while ( *argument != '\0' ) {
  if ( *argument == '\n' ) { argument++; if ( cEnd == ' ' ) break; }
  if ( *argument == '\r' ) { argument++; if ( cEnd == ' ' ) break; }
  if ( *argument == cEnd ) { argument++; break;	}
  *arg_first = (*argument);
  arg_first++;
  argument++;
 }
 *arg_first = '\0';
 while ( (*argument == ' ') || *argument == '\n' || *argument == '\r' ) argument++;
 return argument;
}

char * strupper( char * s ) {
 char *u;
 for( u = s; *u != '\0'; u++ ) *u = UPPER(*u);
 return s;
}

char * strlower( char * s ) {
 char *u;
 for( u = s; *u != '\0'; u++ ) *u = LOWER(*u);
 return s;
}

string lower( string a ) {
 Zstring lowered;
 const char *p=a.c_str();
 while ( *p != '\0' ) { char b=LOWER(*p); lowered+=b; p++; }
 return lowered.value;
}

string upper( string a ) {
 Zstring lowered;
 const char *p=a.c_str();
 while ( *p != '\0' ) { char b=UPPER(*p); lowered+=b; p++; }
 return lowered.value;
}

/*
 * See if a string is one of the words in the namelist
 */
bool is_name( const char *str, const char *namelist )
{
 static char name[STRING_SIZE];
 const char *p;
 if ( str == null || namelist == null ) return false;
 if ( strlen(str) <= 3 ) {
  for ( p = (char *) namelist; ; ) {
  	p = one_argument( p, name );
  	if ( name[0] == '\0' ) return false;
   if ( !str_cmp( str, name ) ) return true; /* str_cmp for full matching */
  }
  for ( p = (char *) namelist; ; ) {
  	p = one_argument( p, name );
  	if ( name[0] == '\0' ) return false;
   if ( !str_cmp( str, name ) ) return true; /* str_cmp for full matching */
  }
 } else {
  for ( p = (char *) namelist; ; ) {
   p = one_argument( p, name );
   if ( name[0] == '\0' ) return false;
   if ( !str_infix( str, name ) ) return true;
  } 
  for ( p = (char *) namelist; ; ) {
   p = one_argument( p, name );
  	if ( name[0] == '\0' ) return false;
   if ( !str_infix( str, name ) ) return true;
  }
 }
}

/*
 * See if a string is one of the keywords in a string.
 * Checks as prefixes.
 */
bool is_prename( const char *str, const char *namelist )
{
 static char name[1024];
 if ( str == null || namelist == null ) return false;
 for ( ; ; ) {
  namelist = one_argument( namelist, name );
  if ( name[0] == '\0' ) return false;
  if ( !str_prefix( str, name ) ) return true;
 }
}

bool valid_characters( const char *fn ) {
 while ( *fn != '\0' ) 
  switch ( *fn++ ) {
   case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': 
   case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
   case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
   case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z': 
   case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
   case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
   case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': 
   case '%': case '_': case '.': case '-': 
   case '\n': case '\0': break;
   default: return false;                
  }   
 return true;
}

// number to string floats
char *ntos( float n, const char *fmt ) {
 static char buf[STRING_SIZE];
 FORMAT(buf,STRING_SIZE,fmt,n);
 return (char *) buf;
}

/*
 * Removes a specified character from a string.
 * Cleans user-entered strings that go into disk files.
 */
void smash_char( char *str, char c )
{
 for ( ; *str != '\0'; str++ )
 if ( *str == c ) *str = '?';
 return;
}

/*
 * Compare strings, case insensitive.
 * Return true if different (compatibility with historical functions).
 */
bool str_cmp( string astr, string bstr ) { return str_cmp( astr.c_str(), bstr.c_str() ); }
bool str_cmp( const char *astr, const char *bstr )
{
 if ( astr == null ) { OUTPUT( "Str_cmp: null astr.\n" ); return true; }
 if ( *astr == '\0' ) return true;
 if ( bstr == null ) { OUTPUT( "Str_cmp: null bstr.\n" ); return true; }
 if ( *bstr == '\0' ) return true;
 const char *p=astr, *q=bstr;
 while ( *p != '\0' && *q != '\0' ) {
  if ( LOWER(*p) != LOWER(*q) ) return true;
  p++; q++;
 }
 if ( *p == *q && *p == '\0' ) return false;
 return true;
}

/*
 * Compare strings, case sensitive.
 * Return true if different
 *   (compatibility with historical functions).
 */
bool str_cmp_case( const char *astr, const char *bstr )
{
 if ( astr == null ) { OUTPUT( "Str_cmp: null astr.\n" ); return true; }
 if ( bstr == null ) { OUTPUT( "Str_cmp: null bstr.\n" ); return true; }
 const char *p=astr, *q=bstr;
 while ( *p != '\0' && *q != '\0' ) {
  if ( *p != *q ) return true;
  p++; q++;
 }
 if ( *p == *q && *p == '\0' ) return false;
 return true;
}

/*
 * Compare strings, case insensitive, for prefix matching.
 * Return true if hay not a prefix of haystack
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
 if ( astr == null || *astr == '\0' ) {
  OUTPUT( "Str_prefix: null astr.\n" );
  return true;
 }
 if ( bstr == null || *bstr == '\0'  ) {
  OUTPUT( "Str_prefix: null bstr.\n" );
  return true;
 }
 while ( *astr != '\0' && *bstr != '\0' ) {
  if ( LOWER(*astr) != LOWER(*bstr) ) return true;
  astr++;
  bstr++;
 }
 if ( *astr == '\0' ) return false;
 return true;
}

/*
 * Compare strings, case sensitive, for prefix matching.
 * Return true if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix_case( const char *astr, const char *bstr )
{
 if ( astr == null || *astr == '\0' ) {
  OUTPUT( "Str_prefix: null astr.\n" );
  return true;
 }
 if ( bstr == null || *bstr == '\0'  ) {
  OUTPUT( "Str_prefix: null bstr.\n" );
  return true;
 }
 while ( *astr != '\0' && *bstr != '\0' ) {
  if ( (*astr) != (*bstr) ) return true;
  astr++;
  bstr++;
 }
 return false;
}

string strtolower( const char *input ) {
 return lower(string(input));
}

/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns true if shorter string not part of longer string.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr ) {
 if ( astr == null || *astr == '\0' ) {
  OUTPUT( "Str_infix: null astr (compared against '%s').\n", bstr );
  return true;
 }
 if ( bstr == null || *bstr == '\0' ) {
  OUTPUT( "Str_infix: null bstr (compared against '%s').\n", astr );
  return true;
 }
 const char *key,*haystack,*q;
 // Figure out which string is shorter, and use that as a key (q)
 if ( strlen(astr) > strlen(bstr) ) {
  haystack= (char *) astr;
  key=q= (char *) bstr;
 } else {
  haystack= (char *) bstr;
  key=q= (char *) astr;
 }
 // Make the keyword lowercase;
 string _key=lower(string(key));
 q=_key.c_str();
 char start=(*q);
 bool found=false;
 while ( *haystack !='\0' && !found ) {
  if ( LOWER(*haystack) != start ) { haystack++; continue; }
  if ( strlen(haystack) < _key.length() ) break;
  q=_key.c_str();
  while ( *q != '\0' && *haystack!='\0' && *q==LOWER(*haystack) ) { q++; haystack++; }
  if ( *q == '\0' ) { found=true; break; }
 }
 return !found;
}


/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns true if shorter string not part of longer string.
 *   (compatibility with historical functions).
 */
bool str_infix_case( const char *astr, const char *bstr ) {
 if ( astr == null || *astr == '\0' ) {
  OUTPUT( "Str_infix_case: null astr (compared against '%s').\n", bstr );
  return true;
 }
 if ( bstr == null || *bstr == '\0' ) {
  OUTPUT( "Str_infix_case: null bstr (compared against '%s').\n", astr );
  return true;
 }
 const char *key,*p,*q;
 // Figure out which string is shorter, and use that as a key (q)
 if ( strlen(astr) > strlen(bstr) ) {
  p= (char *) astr;
  key=q= (char *) bstr;
 }
 else {
  p= (char *) bstr;
  key=q= (char *) astr;
 }
 // Make the keyword lowercase;
 char start=(*q);
 int keyLength=strlen(key);
 bool found=false;
 while ( *p!='\0' && !found ) {
  if ( LOWER(*p) != start ) { p++; continue; }
  if ( (int)strlen(p) < keyLength ) break;
  q=key;
  while ( *q != '\0' && *p!='\0' && *q==LOWER(*p) ) { q++; p++; }
  if ( *q == '\0' ) { found=true; break; }
 }
 return !found;
}
bool string_infix( string astr, string bstr ) {
 string a=string(astr); std::transform(a.begin(), a.end(), a.begin(),(int(*)(int)) std::tolower);
 string b=string(bstr); std::transform(b.begin(), b.end(), b.begin(),(int(*)(int)) std::tolower);
 if ( b.find(a) != string::npos ) return false;
 return true;
}

/*
 * Compare strings, case insensitive, for suffix matching.
 * Return true if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
 size_t sstr1;
 size_t sstr2;
 if ( !astr || !bstr ) return true;
 sstr1 = strlen(astr);
 sstr2 = strlen(bstr);
 if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) ) return false;
 else return true;
}

/*
 * Returns an initial-capped string.
 */
char *capitalize( const char *str )
{
 static char strcap[STRING_SIZE];
 int i;
 for ( i = 0; str[i] != '\0'; i++ )
 strcap[i] = str[i];
 strcap[i] = '\0';
 strcap[0] = UPPER(strcap[0]);
 return (char *) strcap;
}

void capitalize( string *in ) {
 *in=string(capitalize(in->c_str()));
}


// Normalizes a string, then limits the total words to col-3, backing up to last ' ' and adding ..
char *LimitTo( const char *s, int col ) {
 static char buf[1024]; // Since we are limiting this, why use STRING_SIZE?
 int i=col-3,j=0;
 if ( strlen(s) <= (unsigned int) (col-3) ) {
  FORMAT(buf,1024,"%s",s);
  return buf;
 }
 while ( s[i] != ' ' ) i--;
 for ( j=0; j<i; j++ ) { buf[j]=s[j]; }
 buf[i]=' ';
 buf[i+1]='.';
 buf[i+2]='.';
 buf[i+3]='\0';
 return buf;
}

// count the number of newline characters
int linefeeds( const char *s ) {
 int total=0,walked=0;
 while ( s && *s != '\0' ) {
  if ( *s=='\n' ) total++;
  s++;
  walked++;
 }
 if ( walked==0 || *(s-1)!='\n' ) total++;
 return total;
}

const char *ParseFloat( const char *line, float *target ) {
 static char x[1024];
 const char *p=one_argument(line,x);
 *target = (float) atof(x);
 return p;
}

const char *ParseInt( const char *line, int *target ) {
 static char x[1024];
 const char *p=one_argument(line,x);
 *target = atoi(x);
 return p;
}

#include <iostream>
#include <fstream>

unsigned int filesize( const char *filename ) {
 ifstream file( filename, ios::binary | ios::ate);
 return (unsigned int) file.tellg();
}

#include "BinaryFile.h"

string file_as_string( const char *filename ) {
 Zstring fn(filename); fn("\\","/"); fn("//","/");
 unsigned int size=filesize(fn.c_str());
 BinaryFile file;
 if ( !file.Open( fn.c_str(), false ) ) return string("");
 char *text=new char[size+1];
 file.read(text,(size_t)size);
 text[size]='\0';
 string out=string(text);
 delete[] text;
 return out;
}

string file_as_string( const char *filename, unsigned int limit ) {
 unsigned int size=filesize(filename);
 size = size > limit ? limit : size;
 BinaryFile file;
 if ( !file.Open( filename, false ) ) return string("");
 char *text=new char[size+1];
 file.read(text,(size_t)size);
 text[size]='\0';
 string out=string(text);
 delete[] text;
 return out;
}

#include <streambuf>
string file_as_string_streams( const char *filename ) {
 std::ifstream t(filename);
 std::string str;
 t.seekg(0, std::ios::end);   
 str.reserve((unsigned int) t.tellg());
 t.seekg(0, std::ios::beg);
 str.assign((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
 return str;
}

string file_as_string_robust( const char *filename ) {
  Zstring fn(filename); fn("\\","/"); fn("//","/");
  string line,lines=string("");
  ifstream myfile (fn.c_str());
  if (myfile.is_open())
  {
    while ( myfile.good() )
    {
      getline (myfile,line);
      lines+=line;//endl;
      lines+=string("\n");
      //cout << line << endl;
#if defined(DEBUG)
//      OUTPUT(".");
#endif
    }
    myfile.close();
  }
  else {
   OUTPUT("Error opening file `%s`\n",fn.c_str());
  }
  return lines;
}

string file_as_string_robust( const char *filename, unsigned int limit ) {
  string line,lines=string("");
  ifstream myfile (filename);
  if (myfile.is_open())
  {
    while ( myfile.good() && limit > 0 )
    {
      getline (myfile,line);
      lines+=line;//endl;
      lines+=string("\n");
      //cout << line << endl;
#if defined(DEBUG)
//      OUTPUT(".");
#endif
      limit--;
    }
    myfile.close();
  }
  else {
   OUTPUT("Error opening file `%s`\n",filename);
  }
  return lines;
}

 // had some odd problems with this
/*string file_as_string( char *filename )
{
 string r=string("");
 FILE *f;
 fopen_s(&f,filename, "r");
 if(f)
 {
  char temp[STRING_SIZE];
  while(!feof(f))
  {
   char ch='\0';
   int i=0;
   while ( (ch=fgetc(f)) != EOF && !feof(f) && i < STRING_SIZE-2 ) {
    temp[i] =ch;
    i++;
   }
   temp[i]='\0';
   r+=string(temp);
  }
  fclose(f);
 }  
 else OUTPUT("File not found: %s\n", filename);
 return r;
}*/

void string_as_file( const char *content, const char *filename ) {
 Zstring fn(filename); fn("\\","/"); fn("//","/");
 Zstring filtered(content);
 filtered("\r","");
 FILE *pFile=NULL;
 errno_t error=fopen_s(&pFile,fn.c_str(),"w");
 if ( error == 0 && pFile ) {
  fwrite (filtered.c_str(),sizeof(char),strlen(filtered.c_str()),pFile);
  fflush(pFile);
  fclose(pFile);
 } else {
  if ( fn.length != 0 ) {
   OUTPUT( "string_as_file: Error!  Cannot write '%s'\n", fn.c_str() );
   dialog_last_error((string("string_as_file could not write the file `")+fn+string("`")).c_str());
  }
#if defined(DIV_BY_ZERO_REPORTING)
  else OUTPUT("string_as_file: Error!  No filename provided (empty string)\n");
#endif
 }
}

#include <Windows.h>

string slash_to_backslash( const char * s ) {
 Zstring in(s);
 in("/","\\");
 return in.c_str();
}

string backslash_to_slash( const char * s ) {
 Zstring in(s);
 in("\\","/");
 return in.c_str();
}

string trailing_slash( const char *s ) {
 Zstring in(s);
 in.Trim();
 if ( in.last() != '/' ) in+=string("/");
 return in.value;
}

string basename( const char *path, char sep ) {
 Strings parts;
 parts.SplitPush(path,sep);
 if ( !parts.last ) return string("");
 return ((String *)parts.last)->s;
}

string trailing_backslash( const char *s ) {
 Zstring in(s);
 in.Trim();
 if ( in.last() != '\\' ) in+=string("\\");
 return in.value;
}

bool folder_exists(const char *path) {
 DWORD ftyp = GetFileAttributesA(path);
 if (ftyp == INVALID_FILE_ATTRIBUTES) return false;  //something is wrong with your path!
 if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;   // this is a directory!
 return false;    // this is not a directory!
}

bool create_path( const char *path ) {
 bool result= ( SHCreateDirectoryEx( NULL, path, NULL ) == ERROR_SUCCESS );
 if ( !result ) OUTPUT("create_path(`%s`) could not create path\n", path );
 return result;
}

bool file_exists( const char *fn ) {
 Zstring filename(fn); filename("\n",""); filename("\r","");
 DWORD fileAttr = GetFileAttributes(filename.c_str());
 if (0xFFFFFFFF == fileAttr) return false;
 return true;
#if defined(LINUX)
 ifstream myfile(filename);
 if (myfile.is_open()) {
  int size;
  myfile.seekg (0, ios::end);
  size = (int) myfile.tellg();
  myfile.close();
  return (size != 0);
 }
 return false;
#endif
}

bool file_older_than( const char *filename, int day, int month, int year ) {
 if ( !file_exists(filename) ) return false;
 HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 FILETIME created,accessed,written;
 BOOL res=GetFileTime(hFile,&created,&accessed,&written);
 SYSTEMTIME fileTime;
 res=FileTimeToSystemTime(&created, &fileTime);
 res=CloseHandle(hFile);
 if ( (int) fileTime.wYear < year ) return true;
 if ( year == (int) fileTime.wYear
  && (int) fileTime.wMonth < month ) return true;
 if ( year == (int) fileTime.wYear
  && month == (int) fileTime.wMonth
  && (int) fileTime.wDay < day ) return true;
 return false;
}

// Used when writing to a file.
/* in·den·tion (noun) \in-ˈden(t)-shən\ archaic : indentation;
 a: the action of indenting : the condition of being indented
 b: the blank space produced by indenting
 */
string indention=string("");

void incdent() {
 indention+=string(" ");
}

void decdent() {
 size_t spaces=indention.length();
 spaces--;
 nodent();
 for ( int i=0; i<(int)spaces; i++ ) incdent();
}

void nodent() {
 indention=string("");
}

// Low-brow alphanumeric hash.
string Hash( int len ) {
 if ( len<1 ) len=1;
 char *buf;
 buf = new char[len+1];
 for ( int i=0; i<len; i++ ) buf[i]=randomChar("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
 buf[len]='\0';
 string s=string(buf);
 delete[] buf;
 return s;
}


bool HashCompare5( string *a, string *b ) {
 const char *A=a->c_str();
 const char *B=b->c_str();
 return ( A[0]==B[0] && A[1]==B[1] && A[2]==B[2] && A[3]==B[3] && A[4]==B[4] );
}

// Pseudo-
string RomanNumerals( int v ) {
 switch ( v ) {
 case 1: return string("I");
 case 2: return string("II");
 case 3: return string("III");
 case 4: return string("IV");
 case 5: return string("V");
 case 6: return string("VI");
 case 7: return string("VII");
 case 8: return string("VIII");
 case 9: return string("IX");
 case 10: return string("X");
 case 11: return string("X1");
 case 12: return string("X2");
 case 13: return string("X3");
 case 14: return string("X4");
 case 15: return string("X5");
 case 16: return string("X6");
 case 17: return string("X7");
 case 18: return string("X8");
 case 19: return string("X9");
 case 20: return string("2X");
 case 21: return string("2Xi");
 case 22: return string("2Xii");
 case 23: return string("2Xiii");
 case 24: return string("2Xiv");
 case 25: return string("2Xv");
 case 26: return string("2Xvi");
 case 27: return string("2Xvii");
 case 28: return string("2Xviii");
 case 29: return string("2X9");
 case 30: return string("3X");
 default:
  {
   char buf[128];
   if ( v%10 == 0 ) FORMAT(buf,128,"%d", (int) v);
   else {
    string numbers=RomanNumerals(v%10);
    numbers=lower(numbers);
    FORMAT(buf,128,"%dX%s", (int)(v/10), numbers.c_str() );
   }
   return string(buf);
  }
 }
}

/*
static const char B64[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
#define B64_LEN(len) ((len + 2) / 3 * 4) + 1)
#define B64_PAD '='

int b64_lookup( char c ) {
 for ( int i=0; i<65; i++ ) if ( B64[i] == c ) return i;
 return -1;
}

string base64( const char *in, bool encode ) {
 Zstring b; 
 size_t len = strlen(in);
 if ( encode ) {
  char bin[4];
  for (size_t i=0; i<len; i+=3) {
   bin[0] = (char)(in[i+0]);
   if ( i+1 < len ) bin[1] = (char)(in[i+1]);
   if ( i+2 < len ) bin[2] = (char)(in[i+2]);
   b += B64[(bin[0]>>2)];
   b += B64[((bin[0]&3)<<4)|(bin[1]>>4)];
   if (i+1>=len) b += B64_PAD;
   else b += B64[((bin[1]&15)<<2)|(bin[2]>>6)];
   if (i+2>=len) b += B64_PAD;
   else b += B64[(bin[2]&63)];
  }
 } else {
  int bin[4];
  Zstring input(in);
  while ( (input.length % 4) > 0 ) input += B64_PAD;
  for(size_t i=0; i<len; i+=4) {
   bin[0] = b64_lookup(input.At(i+0));
   if ( bin[0] == -1 ) {
    OUTPUT("base64: Decode Error: Invalid character `%c`\n", input.At(i+0) );
    continue;
   }
   bin[1] = b64_lookup(input.At(i+1));
   if ( bin[1] == -1 ) {
    OUTPUT("base64: Decode Error: Invalid character `%c`\n", input.At(i+1) );
    continue;
   }
   bin[2] = b64_lookup(input.At(i+2));
   if ( bin[2] == -1 ) {
    OUTPUT("base64: Decode Error: Invalid character `%c`\n", input.At(i+2) );
    continue;
   }
   bin[3] = b64_lookup(input.At(i+3));
   if ( bin[3] == -1 ) {
    OUTPUT("base64: Decode Error: Invalid character `%c`\n", input.At(i+3) );
    continue;
   }
   b += (char)(255&(bin[0]<<2)|(bin[1]>>4));
   if (bin[2] >= 0) b += (char)(255&(bin[1]<<4)|(bin[2]>>2));
   if (bin[3] >= 0) b += (char)(255&(bin[2]<<6)|(bin[3]));
  }
 }
 return b.value;
}
*/

/* 
   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

#include <iostream>

// B64K functions: Swaps out the two B64 characters that don't work in URLs.
string b64k_encode(const char *in) {
 Zstring piped(in);
 piped = base64_encode(piped.value);
 piped("/", "_");
 piped("+", "-");
 piped("#", "!");
 return piped.value;
}

string b64k_decode(const char *in) {
 Zstring piped(in);
 piped("-", "+");
 piped("_", "/");
 piped("!", "#");
 return base64_decode(piped.value);
}


static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64_encode(string const& s) {
  unsigned char const *bytes_to_encode=reinterpret_cast<const unsigned char*>(s.c_str());
  std::string ret;
  size_t in_len=s.length();
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

string base64_decode(string const& encoded_string) {
  size_t in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  char_array_4[0]='\0';
  char_array_4[1]='\0';
  char_array_4[2]='\0';
  char_array_4[3]='\0';
  char_array_3[0]='\0';
  char_array_3[1]='\0';
  char_array_3[2]='\0';
  Zstring ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = (unsigned char) base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = (unsigned char) base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

/* -- END base64 */

string vigenere( const char *in, const char *key, bool encypher, int lo, int hi ) {
 unsigned char const *in_str=reinterpret_cast<const unsigned char*>(in);
 unsigned char const *key_str=reinterpret_cast<const unsigned char*>(key);
 string out=string("");
 size_t inLen = strlen(in);
 size_t keyLen = strlen(key);
 int span = (hi - lo) + 1;
 for ( int pos=0; pos<(int)inLen; pos++ ) {
  unsigned char const *inChar = &in_str[pos];
  unsigned char const *keyChar = &key_str[(pos % keyLen)];
  int inVal = UMIN(UMAX(lo,(int)(*inChar)),hi)-lo;
  int keyVal = UMIN(UMAX(lo,(int)(*keyChar)),hi)-lo;
  int outVal=
   encypher ? (((span + inVal - keyVal) % span) + lo)
            : (((inVal + keyVal) % span) + lo);
  out += (unsigned char) outVal;
 }
 return out;
}

#include <fstream>

using namespace std;

void file_append_string(const char *fn, const char *s) {
 ofstream ofile(fn, ios::app);
 if (ofile) {
  ofile << s;
  ofile.close();
 }
}

string friendly_number( double d ) {
 Zstring num;
 Zstring unit;
 if ( d > 1000.0 ) { d/=1000.0; unit="k"; }
 if ( d > 1000.0 ) { d/=1000.0; unit="M"; }
 if ( d > 1000.0 ) { d/=1000.0; unit="B"; }
 if ( d > 1000.0 ) { d/=1000.0; unit="T"; }
 num=FORMAT("%1.2f",d);
 num+=unit;
 num(".00",".0");
 return num.value;
}

string friendly_number( float d ) {
 Zstring num;
 Zstring unit;
 if ( d > 1000.0 ) { d/=1000.0; unit="k"; }
 if ( d > 1000.0 ) { d/=1000.0; unit="M"; }
 if ( d > 1000.0 ) { d/=1000.0; unit="B"; }
 if ( d > 1000.0 ) { d/=1000.0; unit="T"; }
 num=FORMAT("%1.2f",d);
 num+=unit;
 num(".00",".0");
 return num.value;
}

string friendly_number( int d ) {
 Zstring num;
 Zstring unit;
 if ( d > 1000 ) { d/=1000; unit="k"; }
 if ( d > 1000 ) { d/=1000; unit="M"; }
 if ( d > 1000 ) { d/=1000; unit="B"; }
 if ( d > 1000 ) { d/=1000; unit="T"; }
 num=FORMAT("%1.2f",d);
 num+=unit;
 num(".00",".0");
 return num.value;
}

// Requires '.' in the incoming string (decimal only!)
string remove_trailing_zeroes( const char *s ) {
 if ( !char_is_of('.',s) ) return string(s);
 bool foundnumordot=false;
 static Zstring result;
 result="";
 int len=strlen(s);
 if ( len == 0 ) return string("");
 if ( len == 1 ) return string(s);
// bool founddot=false;
// for ( int i=0; i<len; i++ ) if ( s[i] == '.' ) founddot=true;
// if ( !founddot ) return string(s);
 for ( int i=len-1; i>=0; i-- ) {
  char c=s[i];
  if ( s[i-1] == '.' ) foundnumordot=true;
  if ( !foundnumordot && 
      (c == '.'
    || c == '9'
    || c == '8'
    || c == '7'
    || c == '6'
    || c == '5'
    || c == '4'
    || c == '3'
    || c == '2'
    || c == '1')
   ) foundnumordot=true;
  if ( foundnumordot || c != '0' ) {
   static char buf[2];
   buf[0]=c;
   buf[1]='\0';
   result=string(buf)+result.value;
  }
 }
 return result.value;
}

// Removes slash-star and double-slash comments, and strips /r
#define DOUBLE_SLASH(p) ( *p == '/' && *(p+1) == '/' )
#define SLASH_STAR(p)   ( *p == '/' && *(p+1) == '*' )
#define STAR_SLASH(p)   ( *p == '*' && *(p+1) == '/' )
string remove_slash_comments( const char *in ) {
 if ( strlen(in) < 2 ) return string(in);
 Zstring out;
 const char *p=in;
 while ( *p != '\0' ) {
  if ( *p == '\r' ) { p++; continue; }
  if ( DOUBLE_SLASH(p) ) {
   while ( *p != '\0' && *p != '\n' ) p++;
  }
  if ( SLASH_STAR(p) ) {
   int ins=1;
   while ( ins > 0 && *(p+1) != '\0' ) {
     while ( STAR_SLASH(p) || *(p+1) == '\0' ) { if ( SLASH_STAR(p) ) { ins++; p++; p++; } p++;
     if ( STAR_SLASH(p) ) { p++; p++; }
    }
   }
  }
  if ( *p != '\0' ) {
   out+=(*p);
   p++;
  } else break;
 }
 return out.value;
}