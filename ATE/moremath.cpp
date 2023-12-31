
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include <math.h>
#include <limits>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

#include <limits.h>
#include "macros.h"
#include "moremath.h"
#include "ZeroTypes.h"

double norm_deg( double angle ) {
 while ( angle < 0.0 ) angle+=360.0;
 while ( angle > 360.0 ) angle-=360.0;
 return angle;
}
float norm_deg( float angle ) {
 while ( angle < 0.0f ) angle+=360.0f;
 while ( angle > 360.0f ) angle-=360.0f;
 return angle;
}
double norm_rad( double angle ) {
 while ( angle < 0.0 ) angle+=TWO_PI;
 while ( angle > TWO_PI ) angle-=TWO_PI;
 return angle;
}
float norm_rad( float angle ) {
 while ( angle < 0.0 ) angle+=TWO_PIf;
 while ( angle > TWO_PIf ) angle-=TWO_PIf;
 return angle;
}
float norm_deg( Zfloat angle ) { return norm_deg(angle.value); }
float norm_rad( Zfloat angle ) { return norm_rad(angle.value); }
double norm_deg( Zdouble angle ) { return norm_deg(angle.value); }
double norm_rad( Zdouble angle ) { return norm_rad(angle.value); }

unsigned long getTime(void)
{
    time_t ltime;

    // Set time zone from TZ environment variable. If TZ is not set,
    // the operating system is queried to obtain the default value 
    // for the variable. 
    //
    _tzset();
    // Get UNIX-style time and display as number and string. 
    time( &ltime );
    return (unsigned long) ltime;
}

unsigned long getMillis(void) {
 struct _timeb tstruct;
 // Print additional time information. 
 _ftime_s( &tstruct ); // C4996
 // Note: _ftime is deprecated; consider using _ftime_s instead
 return (unsigned long) tstruct.millitm;
}

struct _timeb GetTime(void) {
 struct _timeb tstruct;
 // Print additional time information. 
 _ftime_s( &tstruct ); // C4996
 // Note: _ftime is deprecated; consider using _ftime_s instead
 return tstruct;
}

// Constants needed for random generation

const long m = 2147483647L;
const long a = 48271L;
const long q = 44488L;
const long r = 3399L;

// A variable used to initialize the generator (should never be 0).
long r_seed = 12345678L;

// Integer SQRT
#define MAX_SRT 10000
double SRT[MAX_SRT];
int SRT_initialized=0;
void init_isqrt( ) { 
 int i; 
 for ( i =0; i< MAX_SRT; i++ ) SRT[i]=sqrt((double)i); 
 SRT_initialized=1; 
}
float isqrt ( int i ) {
 i=abs(i); 
 if ( i > MAX_SRT-1 ) return (float) sqrt((float) i); 
 return (float) SRT[i]; 
}

double uniform ()
{
  long t, lo, hi;
  double u;

  hi = r_seed / q;
  lo = r_seed - q * hi;
  t = a * lo - r * hi;
  if (t > 0)
    r_seed = t;
  else
    r_seed = t + m;
  u = (double) r_seed / (double) m ;

  return u;
}

double uniform ( bool reseed )
{
  long t, lo, hi;
  double u;

  hi = r_seed / q;
  lo = r_seed - q * hi;
  if ( reseed ) {
   t = a * lo - r * hi;
   if (t > 0)
     r_seed = t;
   else
     r_seed = t + m;
  }
  u = (double) r_seed / (double) m ;

  return u;
}

// Works better with small numbers 0-255, used in Randoms.h,
// Adheres to RAND_MAX (0x7fff) but is intended to be distinct and non-OS specific
int pseudorandom( unsigned int s, unsigned int x ) {
 s++;
 x++;
 long S=s;
 long X=x;
 long result=((56789*(X+S*X+S))/2)%PSEUDORAND_MAX;
 return abs((int)result);
}

// Reseeding
void init_seeder() {
 r_seed = (long) ( getTime() * 1000 + getMillis() );
 r_seed = (long) (LONG_MIN+uniform()*ULONG_MAX);
 if ( r_seed == 0 ) r_seed++;
}

void reseed() {
 r_seed = (long) ( getTime() * 1000 + getMillis() );
 r_seed = (long) (LONG_MIN+uniform()*ULONG_MAX);
 if ( r_seed == 0 ) r_seed++;
}

double random(double x) {
	return x*uniform();
}

// Properly rounds ints
int iround ( float f ) {
 return (int)(f+0.5f);
}

// Properly rounds ints
int iround ( double d ) {
 return (int)(d+0.5);
}

// Do a floating point lerp from "start" to "finish"
int ilerp( int start, int end, float percent ) {
 int d;
 if ( start > end ) {
  d=start-end;
  return start-iround( (float) d * percent );
 } else {
  d=end-start;
  return start+iround( (float) d * percent );
 }
}

// Do a floating point lerp from "start" to "finish"
int ilerpw( int start, int end, float percent ) {
 float d=((float)ADIFF(start,end));
 if ( start > end ) d=-d;
 return start+iround( d * percent );
}


// Do a floating point lerp from "start" to "finish"
int ilerp( int start, int end, double percent ) {
 int d;
 if ( start > end ) {
  d=start-end;
  return start-iround( (double) d * percent );
 } else {
  d=end-start;
  return start+iround( (double) d * percent );
 }
}

// Do a floating point lerp from "start" to "finish"
int ilerpw( int start, int end, double percent ) {
 double d=((double)ADIFF(start,end));
 if ( start > end ) d=-d;
 return start+iround( d * percent );
}

double double_range( double L, double H ) {
 double u=uniform();
 double t=(H-L);
 u*= t;
 return (u) + L;
}

float float_range( float L, float H ) {
 double u=uniform();
 double t= (double) (H-L);
 u*= t;
 return ((float) u) + L;
}

// Randomize an integer in range L-H
int number_range( int L, int H ) {
   double u=uniform();
   int t=H-L;
   u*=(double) t;
   u+=(double) L;
   return iround((float) (u));
}

int upto( int M ) {
 if ( M==0 ) return 0;
 return number_range(0,M*2)%M;
//  return (int) (uniform()/*0.999999999999999*/*(double) max);
}

#include "Randoms.h"
int upto( int M, int seed ) {
 if ( M==0 ) return 0;
 int result=(int)((float)M*randoms.Unique(seed/M+M,seed));
 return result != M ? result : 0;
}

// Generating gaussian random number with mean 0 and standard deviation 1.
float Gauss()
{
	double u1 = uniform();
	double u2 = uniform();
	if (u1 < 1e-6f)
		u1 = 1e-6f;
	return sqrtf(-2 * logf((float) u1)) * cosf((float)(2*PI*u2));
}

/* Math */

double deg2rad( double deg ) {
  return (deg * (PI/180));  
}

double rad2deg( double rad ) {
  return (rad * (180/PI));
}

float deg2rad( float deg ) {
  return (float) (deg * (PI/180));  
}

float rad2deg( float rad ) {
  return (float) (rad * (180/PI));
}

float iratiof( int i, int y ) {
 return ((float)i)/((float)y);
}

double iratiod( int i, int y ) {
 return ((double)i)/((double)y);
}

double uiratiod( unsigned int i, unsigned int y ) {
 return ((double)i)/((double)y);
}

int fiaddi( int a, float f ) {
 return (int)((float) a + f );
}

int faddi( float a, float f ) {
 return (int)(a + f );
}

float wrap( float c, float a, float b ) {
  float d=b-a;
  if ( c >= a && c <= b ) return c;
  while ( c > b ) c-=d;
  while ( c < a ) c+=d;
  return c;
}

double wrap( double c, double a, double b ) {
  double d=b-a;
  if ( c >= a && c <= b ) return c;
  while ( c > b ) c-=d;
  while ( c < a ) c+=d;
  return c;
}

int iwrap( int c, int a, int b ) {
  int d=b-a;
  if ( c >= a && c <= b ) return c;
  if ( c == b+1 ) return a;
  if ( c == a-1 ) return b;
  while ( c > b ) c-=d;
  while ( c < a ) c+=d;
  return c;
}

double ddistance( double x, double y, double xx, double yy ) {
 if ( x==xx && y==yy ) return 0.0;
 return sqrt( ((x - xx) * (x - xx)) + ((y - yy) * (y - yy)) ) ;
}

double ddistance( double x, double y, double z, double xx, double yy, double zz ) {
 double dist2d = ddistance(x,y,xx,yy);
 return sqrt(((dist2d*dist2d)+((zz-z)*(zz-z))));
}

float fdistance( float x, float y, float xx, float yy ) {
 if ( x==xx && y==yy ) return 0.0f;
 return sqrtf( ((x - xx) * (x - xx)) + ((y - yy) * (y - yy)) ) ;
}

float fdistance( float x, float y, float z, float xx, float yy, float zz ) {
 float dist2d = fdistance(x,y,xx,yy);
 return sqrtf(((dist2d*dist2d)+((zz-z)*(zz-z))));
}

float idistance( int x, int y, int xx, int yy ) {
 if ( x==xx && y==yy ) return 0.0f;
 return isqrt( ((x - xx) * (x - xx)) + ((y - yy) * (y - yy)) ) ;
}
float idistance( float x, float y, float xx, float yy ) {
 return isqrt( (int) ( ((x - xx) * (x - xx)) + ((y - yy) * (y - yy)) ) ) ;
}
float idistance( double x, double y, double xx, double yy ) {
 return isqrt( (int) ( ((x - xx) * (x - xx)) + ((y - yy) * (y - yy)) ) ) ;
}

float idistance( int x, int y, int z, int xx, int yy, int zz ) {
 float dist2d = idistance(x,y,xx,yy);
 return sqrtf(((dist2d*dist2d)+((zz-z)*(zz-z))));
}
float idistance( float x, float y, float z, float xx, float yy, float zz ) {
 float dist2d = idistance(x,y,xx,yy);
 return sqrtf(((dist2d*dist2d)+((zz-z)*(zz-z))));
}
float idistance( double x, double y, double z, double xx, double yy, double zz ) {
 float dist2d = idistance(x,y,xx,yy);
 return sqrtf(((dist2d*dist2d)+(float)((zz-z)*(zz-z))));
}

float idistance( Zint x, Zint y, int xx, int yy ) { return idistance( (int) x, (int) y, xx, yy ); }
float idistance( int x, int y, Zint xx, Zint yy ) { return idistance( x, y, (int) xx, (int) yy ); }
float idistance( Zint x, Zint y, Zint xx, Zint yy ) { return idistance( (int) x, (int) y, (int) xx, (int) yy ); }
float idistance( Zfloat x, Zfloat y, Zfloat xx, Zfloat yy ) { return idistance( (float) x, (float) y, (float) xx, (float) yy ); }
float idistance( Zdouble x, Zdouble y, Zdouble xx, Zdouble yy ) { return idistance( (double) x, (double) y, (double) xx, (double) yy ); }
float idistance( Zint x, Zint y, Zint z, Zint xx, Zint yy, Zint zz ) { return idistance( (int) x, (int) y, (int) z, (int) xx, (int) yy, (int) zz ); }
float idistance( Zfloat x, Zfloat y, Zfloat z, Zfloat xx, Zfloat yy, Zfloat zz ) { return idistance( (float) x, (float) y, (float) z, (float) xx, (float) yy, (float) zz ); }
float idistance( Zdouble x, Zdouble y, Zdouble z, Zdouble xx, Zdouble yy, Zdouble zz ) { return idistance( (double) x, (double) y, (double) z, (double) xx, (double) yy, (double) zz ); }


float rsqrtf(const float v) {
	union {	float vh;	int i0;	};
	union {	float vr;	int i1;	};
	vh = v * 0.5f;
	i1 = 0x5f3759df - (i0 >> 1);
	return vr * (1.5f - vh * vr * vr);
}

float sqrf(const float x){	return x * x; }
float sincf(const float x){	return (x == 0)? 1 : sinf(x) / x; }

float intAdjustf(const float x, const float diff = 0.01f){
	float f = roundf(x);
	return (fabsf(f - x) < diff)? f : x;
}

float lerpf(const float a, const float b, const float x){	return a + x * (b - a); }
double lerpd( double a, double b, double x ) {	return a + x * (b - a); }

float cerpf(const float a, const float b, const float c, const float d, const float x){
	float p = (d - c) - (a - b);
	float q = (a - b) - p;
	float r = c - a;
	return x * (x * (x * p + q) + r) + b;
}

unsigned int getClosestPowerOfTwo(const unsigned int x){
	int i=-1,k = x;
	while (k != 0){	k >>= 1;	i++;	}
	return 1 << (i + ((x >> (i-1)) & 1));
}

//"Morton"
int even_dilate(const int val){
	int u = ((val & 0x0000ff00) << 8) | (val & 0x000000ff);
	int v = ((  u & 0x00f000f0) << 4) | (  u & 0x000f000f);
	int w = ((  v & 0x0c0c0c0c) << 2) | (  v & 0x03030303);
	int r = ((  w & 0x22222222) << 1) | (  w & 0x11111111);
	return r;
}

int floatToIntBits(float f) {
 int i;
 memcpy(&i, &f, 4);
 return i;
}

/* NVIDIA wrote: */
int NVfloatToIntBits(float f)
{
 int i;
 memcpy(&i, &f, 4);
  union {
    float f;  // assuming 32-bit IEEE 754 single-precision
    int i;    // assuming 32-bit 2's complement int
  } u;

  if ((f!=f)) return 0x7fc00000;
  else {
    u.f = f;
    return u.i;
  }
}

float mapRangef(float inputValue, float inputMin, float inputMax, float outputMin, float outputMax) {
 float perc=(inputValue-inputMin)/(inputMax-inputMin);
 float outMax=outputMax-outputMin;
 return outputMin+(perc*outMax);
}

double mapRanged(double inputValue, double inputMin, double inputMax, double outputMin, double outputMax) {
 double perc=(inputValue-inputMin)/(inputMax-inputMin);
 double outMax=outputMax-outputMin;
 return outputMin+(perc*outMax);
}

float percf(float inputValue, float inputMin, float inputMax) {
 return (inputValue-inputMin)/(inputMax-inputMin);
}

/*
void test_is_same() { 
 OUTPUT( "is_same<base, base> == %d\n",    std::is_same<base, base>::value    ? 1 : 0 );
 OUTPUT( "is_same<base, derived> == %d\n", std::is_same<base, derived>::value ? 1 : 0 );
 OUTPUT( "is_same<derived, base> == %d\n", std::is_same<derived, base>::value ? 1 : 0 );
 OUTPUT( "is_same<int, int> == %d\n",      std::is_same<int, int>::value      ? 1 : 0 );
 OUTPUT( "is_same<int, const int> == %d\n",std::is_same<int, const int>::value? 1 : 0 );
} 
*/

float RandomSign() {
 if ( number_range(0,50000) % 2 == 1 ) return -1.0f;
 else return 1.0f;
}

// Simple Perlin Noise (freespace.virgin.net/hugo.elias/models/m_perlin.htm)

double Noise(int x) {
 x = (x<<13) ^ x;
 return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7FFFFFFF) / 1073741824.0);    
}

double Noise(double x) {
 return Noise((int)x)/2  +  Noise((int)x-1)/4  +  Noise((int)x+1)/4;
};

double Noise(int x, int y) {
 int n = x + y * 57;
 n = (n<<13) ^ n;
 return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7FFFFFFF) / 1073741824.0);    
}

double Noise(double x, double y) {
 int xi=(int) x;
 int yi=(int) y;
 double corners = ( Noise(xi-1, yi-1)+Noise(xi+1, yi-1)+Noise(xi-1, yi+1)+Noise(xi+1, yi+1) ) / 16;
 double sides   = ( Noise(xi-1, yi)  +Noise(xi+1, yi)  +Noise(xi, yi-1)  +Noise(xi, yi+1) ) /  8;
 double center  =  Noise(xi, yi) / 4;
 return corners + sides + center;
}

double Interpolate( double a, double b, double x ) {
 return  a*(1-x) + b*x;
}

double CosInterpolate( double a, double b, double x ) {
 double ft = x * PI;// 3.1415927;
 double f = (1 - cos(ft)) * .5;
 return  a*(1-f) + b*f;
}

double CubicInterpolate(double v0, double v1, double v2, double v3, double x) {
 double P = (v3 - v2) - (v0 - v1);
 double Q = (v0 - v1) - P;
 double R = v2 - v0;
 double S = v1;
 return (P*x)*(P*x)*(P*x) + SQ(Q*x) + R*x + S;
}

double InterpolatedNoise(double x) {
 int integer_X    = int(x);
 double fractional_X = x - integer_X;
 double v1 = Noise(integer_X);
 double v2 = Noise(integer_X + 1);
 return Interpolate(v1 , v2 , fractional_X);
}

double InterpolatedNoise(double x, double y) {
 int integer_X    = (int)(x);
 double fractional_X = x - (double) integer_X;
 int integer_Y    = (int)(y);
 double fractional_Y = y - (double) integer_Y;

 double v1 = Noise(integer_X,     integer_Y);
 double v2 = Noise(integer_X + 1, integer_Y);
 double v3 = Noise(integer_X,     integer_Y + 1);
 double v4 = Noise(integer_X + 1, integer_Y + 1);

 double i1 = Interpolate(v1 , v2 , fractional_X);
 double i2 = Interpolate(v3 , v4 , fractional_X);
 return Interpolate(i1 , i2 , fractional_Y);
}

double PerlinNoise_1D(double x, int octaves, int persistence ) {
 double total=0.0;
// p = persistence
// n = Number_Of_Octaves - 1
 for ( int i=0; i<octaves; i++ ) {
  double frequency = 2.0*i;
  double amplitude = PI;
  total += InterpolatedNoise(x * frequency) * amplitude;
 }
 return total;
}

double PerlinNoise_2D(double x, double y, int octaves, int persistence ) {
 double total=0.0;
//      p = persistence
//      n = Number_Of_Octaves - 1
 for ( int i=0; i<octaves; i++ ) {
  double frequency=2*(double)i;
  double amplitude=PI*2.0;
  total += InterpolatedNoise(x * frequency, y * frequency) * amplitude;
 }
 return total;
}

#define randomint upto(INT_MAX)
#define B 0x100
#define BM 0xff
#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

#define s_curve(t) ( t * t * (3. - 2. * t) )
#define setup(i,b0,b1,r0,r1)\
        t = vec[i] + N;\
        b0 = ((int)t) & BM;\
        b1 = (b0+1) & BM;\
        r0 = t - (int)t;\
        r1 = r0 - 1.;
#define at2(rx,ry) ( rx * q[0] + ry * q[1] )
#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

static int p[B + B + 2];
static double g3[B + B + 2][3];
static double g2[B + B + 2][2];
static double g1[B + B + 2];
static int start = 1;

/* Locals */
void initperlin(void);
double noise1(double);
double noise2(double *);
double noise3(double *);
void normalize3(double *);
void normalize2(double *);

double noise1(double arg)
{
   int bx0, bx1;
   double rx0, rx1, sx, t, u, v, vec[1];

   vec[0] = arg;
   if (start) {
      start = 0;
      initperlin();
   }

   setup(0,bx0,bx1,rx0,rx1);

   sx = s_curve(rx0);
   u = rx0 * g1[ p[ bx0 ] ];
   v = rx1 * g1[ p[ bx1 ] ];

   return(p_lerp(sx, u, v));
}

double noise2(double vec[2])
{
   int bx0, bx1, by0, by1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
   int i, j;

   if (start) {
      start = 0;
      initperlin();
   }

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   sx = s_curve(rx0);
   sy = s_curve(ry0);

   q = g2[ b00 ] ; u = at2(rx0,ry0);
   q = g2[ b10 ] ; v = at2(rx1,ry0);
   a = p_lerp(sx, u, v);

   q = g2[ b01 ] ; u = at2(rx0,ry1);
   q = g2[ b11 ] ; v = at2(rx1,ry1);
   b = p_lerp(sx, u, v);

   return p_lerp(sy, a, b);
}

double noise3(double vec[3])
{
   int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
   double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
   int i, j;

   if (start) {
      start = 0;
      initperlin();
   }

   setup(0, bx0,bx1, rx0,rx1);
   setup(1, by0,by1, ry0,ry1);
   setup(2, bz0,bz1, rz0,rz1);

   i = p[ bx0 ];
   j = p[ bx1 ];

   b00 = p[ i + by0 ];
   b10 = p[ j + by0 ];
   b01 = p[ i + by1 ];
   b11 = p[ j + by1 ];

   t  = s_curve(rx0);
   sy = s_curve(ry0);
   sz = s_curve(rz0);

   q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
   q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
   a = p_lerp(t, u, v);

   q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
   q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
   b = p_lerp(t, u, v);

   c = p_lerp(sy, a, b);

   q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
   q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
   a = p_lerp(t, u, v);

   q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
   q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
   b = p_lerp(t, u, v);

   d = p_lerp(sy, a, b);

   return p_lerp(sz, c, d);
}

void normalize2(double v[2])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
}

void normalize3(double v[3])
{
   double s;

   s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] = v[0] / s;
   v[1] = v[1] / s;
   v[2] = v[2] / s;
}

void initperlin(void)
{
   int i, j, k;

   for (i = 0 ; i < B ; i++) {
      p[i] = i;
      g1[i] = (double)((randomint % (B + B)) - B) / B;

      for (j = 0 ; j < 2 ; j++)
         g2[i][j] = (double)((randomint % (B + B)) - B) / B;
      normalize2(g2[i]);

      for (j = 0 ; j < 3 ; j++)
         g3[i][j] = (double)((randomint % (B + B)) - B) / B;
      normalize3(g3[i]);
   }

   while (--i) {
      k = p[i];
      p[i] = p[j = randomint % B];
      p[j] = k;
   }

   for (i = 0 ; i < B + 2 ; i++) {
      p[B + i] = p[i];
      g1[B + i] = g1[i];
      for (j = 0 ; j < 2 ; j++)
         g2[B + i][j] = g2[i][j];
      for (j = 0 ; j < 3 ; j++)
         g3[B + i][j] = g3[i][j];
   }
}

/* --- My harmonic summing functions - PDB --------------------------*/

/*
   In what follows "alpha" is the weight when the sum is formed.
   Typically it is 2, As this approaches 1 the function is noisier.
   "beta" is the harmonic scaling/spacing, typically 2.
*/

double PerlinNoise1D(double x,double alpha,double beta,int n)
{
   int i;
   double val,sum = 0;
   double p,scale = 1;

   p = x;
   for (i=0;i<n;i++) {
      val = noise1(p);
      sum += val / scale;
      scale *= alpha;
      p *= beta;
   }
   return(sum);
}

double PerlinNoise2D(double x,double y,double alpha,double beta,int n)
{
   int i;
   double val,sum = 0;
   double p[2],scale = 1;

   p[0] = x;
   p[1] = y;
   for (i=0;i<n;i++) {
      val = noise2(p);
      sum += val / scale;
      scale *= alpha;
      p[0] *= beta;
      p[1] *= beta;
   }
   return(sum);
}

double PerlinNoise3D(double x,double y,double z,double alpha,double beta,int n)
{
   int i;
   double val,sum = 0.0;
   double p[3],scale = 1.0;

   p[0] = x;
   p[1] = y;
   p[2] = z;
   for (i=0;i<n;i++) {
      val = noise3(p);
      sum += val / scale;
      scale *= alpha;
      p[0] *= beta;
      p[1] *= beta;
      p[2] *= beta;
   }
   return(sum);
}

float frequency(float v, float one) {
 while ( v > one ) v-=one;
 return v/one;
}

float absf( float f ) {
 return abs(f);
}

int absi( int i ) {
 return abs(i);
}

double absd( double d ) {
 return abs(d);
}

/*  Lifted, with permission, from Humus..	Credits:
	Based on the great work done by Ken Perlin.
	http://mrl.nyu.edu/~perlin/doc/oscar.html
*/


#include <stdlib.h>
#include <math.h>


double bias(const double a, const double b){
 return pow(a, log(b) / log(0.5));
}

double gain(const double a, const double b){
 if (a < 0.001)	return 0;
 else if (a > 0.999) return 1.0;
 double p = log(1.0 - b) / log(0.5);
 if (a < 0.5) return (pow(2.0 * a, p) / 2.0);
 else return 1.0 - (pow(2 * (1.0 - a), p) / 2.0);
}

/* noise functions over 1, 2, and 3 dimensions */

#define nB 0x1000
#define nBM 0xff

#define nN 0x1000
#define nNP 12   /* 2^N */
#define nNM 0xfff

static int np[nB + nB + 2];
static double ng3[nB + nB + 2][3];
static double ng2[nB + nB + 2][2];
static double ng1[nB + nB + 2];

#define ns_curve(t) (t * t * (3 - 2 * t))
#define nlerp(t, a, b) (a + t * (b - a))
#define nsetup(i,b0,b1,r0,r1)\
	t = i + nN;\
	b0 = ((int) t) & nBM;\
	b1 = (b0 + 1) & nBM;\
	r0 = t - (int) t;\
	r1 = r0 - 1;

double Noise1(const float x){
 int bx0, bx1;
 double rx0, rx1, sx, t, u, v;
 nsetup(x, bx0,bx1, rx0,rx1);
 sx = ns_curve(rx0);
 u = rx0 * ng1[p[bx0]];
 v = rx1 * ng1[p[bx1]];
 return nlerp(sx, u, v);
}

#define nat2(rx,ry) (rx * q[0] + ry * q[1])

double Noise2(const double x, const double y){
 int bx0, bx1, by0, by1, b00, b10, b01, b11;
 double rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
 int i, j; 
 nsetup(x, bx0,bx1, rx0,rx1);
 nsetup(y, by0,by1, ry0,ry1); 
 i = np[bx0];
 j = np[bx1]; 
 b00 = np[i + by0];
 b10 = np[j + by0];
 b01 = np[i + by1];
 b11 = np[j + by1]; 
 sx = ns_curve(rx0);
 sy = ns_curve(ry0); 
 q = ng2[b00]; u = nat2(rx0,ry0);
 q = ng2[b10]; v = nat2(rx1,ry0);
 a = nlerp(sx, u, v); 
 q = ng2[b01]; u = nat2(rx0,ry1);
 q = ng2[b11]; v = nat2(rx1,ry1);
 b = nlerp(sx, u, v); 
 return nlerp(sy, a, b);
}

#define nat3(rx,ry,rz) (rx * q[0] + ry * q[1] + rz * q[2])

double Noise3(const double x, const double y, const double z){
 int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
 double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
 int i, j;
 nsetup(x, bx0,bx1, rx0,rx1);
 nsetup(y, by0,by1, ry0,ry1);
 nsetup(z, bz0,bz1, rz0,rz1);
 i = np[bx0];
 j = np[bx1];
 b00 = np[i + by0];
 b10 = np[j + by0];
 b01 = np[i + by1];
 b11 = np[j + by1];
 t  = ns_curve(rx0);
 sy = ns_curve(ry0);
 sz = ns_curve(rz0);
 q = ng3[b00 + bz0]; u = nat3(rx0,ry0,rz0);
 q = ng3[b10 + bz0]; v = nat3(rx1,ry0,rz0);
 a = nlerp(t, u, v);
 q = ng3[b01 + bz0]; u = nat3(rx0,ry1,rz0);
 q = ng3[b11 + bz0]; v = nat3(rx1,ry1,rz0);
 b = nlerp(t, u, v);
 c = nlerp(sy, a, b);
 q = ng3[b00 + bz1]; u = nat3(rx0,ry0,rz1);
 q = ng3[b10 + bz1]; v = nat3(rx1,ry0,rz1);
 a = nlerp(t, u, v);
 q = ng3[b01 + bz1]; u = nat3(rx0,ry1,rz1);
 q = ng3[b11 + bz1]; v = nat3(rx1,ry1,rz1);
 b = nlerp(t, u, v);
 d = nlerp(sy, a, b);
 return nlerp(sz, c, d);
}

static void Normalize2(double v[2]){
 double s = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1]);
 v[0] *= s;
 v[1] *= s;
}

static void Normalize3(double v[3]){
 double s = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
 v[0] *= s;
 v[1] *= s;
 v[2] *= s;
}

double Turbulence2(const double x, const double y, double freq){
 double t = 0.0;
 do {
 	t += Noise2(freq * x, freq * y) / freq;
 	freq *= 0.5;
 } while (freq >= 1.0);	
 return t;
}

double Turbulence3(const double x, const double y, const double z, double freq){
 double t = 0.0; 
 do {
 	t += Noise3(freq * x, freq * y, freq * z) / freq;
 	freq *= 0.5;
 } while (freq >= 1.0);
 return t;
}

double TileableNoise1(const double x, const double w){
 return (noise1(x) * (w - x) + noise1(x - w) * x) / w;
}

double TileableNoise2(const double x, const double y, const double w, const double h){
 return
 (Noise2(x,     y)     * (w - x) * (h - y) +
  Noise2(x - w, y)     *      x  * (h - y) +
  Noise2(x,     y - h) * (w - x) *      y  +
  Noise2(x - w, y - h) *      x  *      y) / (w * h);
}

double TileableNoise3(const double x, const double y, const double z, const double w, const double h, const double d){
 return
 (Noise3(x,     y,     z)     * (w - x) * (h - y) * (d - z) +
  Noise3(x - w, y,     z)     *      x  * (h - y) * (d - z) +
  Noise3(x,     y - h, z)     * (w - x) *      y  * (d - z) +
  Noise3(x - w, y - h, z)     *      x  *      y  * (d - z) + 
  Noise3(x,     y,     z - d) * (w - x) * (h - y) *      z  +
  Noise3(x - w, y,     z - d) *      x  * (h - y) *      z  +
  Noise3(x,     y - h, z - d) * (w - x) *      y  *      z  +
  Noise3(x - w, y - h, z - d) *      x  *      y  *      z) / (w * h * d);
}

double TileableTurbulence2(const double x, const double y, const double w, const double h, double freq){
 double t = 0.0;
 do {
 	t += TileableNoise2(freq * x, freq * y, w * freq, h * freq) / freq;
 	freq *= 0.5;
 } while (freq >= 1.0);
 return t;
}

double TileableTurbulence3(const double x, const double y, const double z, const double w, const double h, const double d, double freq){
 double t = 0.0;
 do {
  t += TileableNoise3(freq * x, freq * y, freq * z, w * freq, h * freq, d * freq) / freq;
  freq *= 0.5;
 } while (freq >= 1.0);
 return t;
}

void InitPerlin(){
 int i, j, k; 
 for (i = 0; i < nB; i++) {
  np[i] = i;
  ng1[i] = (double) ((rand() % (nB + nB)) - nB) / nB;
  for (j = 0; j < 2; j++) ng2[i][j] = (double) ((rand() % (nB + nB)) - nB) / nB;
  normalize2(ng2[i]);
  for (j = 0; j < 3; j++) ng3[i][j] = (double) ((rand() % (nB + nB)) - nB) / nB;
  normalize3(ng3[i]);
 }
 while (--i) {
  k = np[i];
  np[i] = np[j = rand() % nB];
  np[j] = k;
 } 
 for (i = 0; i < nB + 2; i++) {
  np[nB + i] = np[i];
  ng1[nB + i] = ng1[i];
  for (j = 0; j < 2; j++) ng2[nB + i][j] = ng2[i][j];
  for (j = 0; j < 3; j++) ng3[nB + i][j] = ng3[i][j];
 }
}


// Liang-Barsky function by Daniel White @ http://www.skytopia.com/project/articles/compsci/clipping.html
// This function inputs 8 numbers, and outputs 4 new numbers (plus a boolean value to say whether the clipped line is drawn at all).
//
bool LiangBarsky (double edgeLeft, double edgeRight, double edgeBottom, double edgeTop,   // Define the x/y clipping values for the border.
                  double x0src, double y0src, double x1src, double y1src,                 // Define the start and end points of the line.
                  double *x0clip, double *y0clip, double *x1clip, double *y1clip)         // The output values, so declare these outside.
{
    
    double t0 = 0.0;    double t1 = 1.0;
    double xdelta = x1src-x0src;
    double ydelta = y1src-y0src;
    double p,q,r;

    for(int edge=0; edge<4; edge++) {   // Traverse through left, right, bottom, top edges.
        if (edge==0) {  p = -xdelta;    q = -(edgeLeft-x0src);  }
        if (edge==1) {  p = xdelta;     q =  (edgeRight-x0src); }
        if (edge==2) {  p = -ydelta;    q = -(edgeBottom-y0src);}
        if (edge==3) {  p = ydelta;     q =  (edgeTop-y0src);   }   
        r = q/p;
        if(p==0 && q<0) return false;   // Don't draw line at all. (parallel line outside)

        if(p<0) {
            if(r>t1) return false;         // Don't draw line at all.
            else if(r>t0) t0=r;            // Line is clipped!
        } else if(p>0) {
            if(r<t0) return false;      // Don't draw line at all.
            else if(r<t1) t1=r;         // Line is clipped!
        }
    }

    *x0clip = x0src + t0*xdelta;
    *y0clip = y0src + t0*ydelta;
    *x1clip = x0src + t1*xdelta;
    *y1clip = y0src + t1*ydelta;

    return true;        // (clipped) line is drawn
}

double cos90, sin90,
       cos180, sin180,
       cos270, sin270;
void InitMath() { 
  cos90 =cos(HALF_PI);
  sin90 =sin(HALF_PI);
  cos180=cos(PI);
  sin180=sin(PI);
  cos270=cos(PI+HALF_PI);
  sin270=sin(PI+HALF_PI);
  InitPerlin();
}

// Source: Paeth, Graphics Gems V
int PntOnLine(int px, int py, int qx, int qy, int tx, int ty) {
/*
 * given a line through P:(px,py) Q:(qx,qy) and T:(tx,ty)
 * return 0 if T is not on the line through      <--P--Q-->
 *        1 if T is on the open ray ending at P: <--P
 *        2 if T is on the closed interior along:   P--Q
 *        3 if T is on the open ray beginning at Q:    Q-->
 *
 * Example: consider the line P = (3,2), Q = (17,7). A plot
 * of the test points T(x,y) (with 0 mapped onto '.') yields:
 *
 *     8| . . . . . . . . . . . . . . . . . 3 3
 *  Y  7| . . . . . . . . . . . . . . 2 2 Q 3 3    Q = 2
 *     6| . . . . . . . . . . . 2 2 2 2 2 . . .
 *  a  5| . . . . . . . . 2 2 2 2 2 2 . . . . .
 *  x  4| . . . . . 2 2 2 2 2 2 . . . . . . . .
 *  i  3| . . . 2 2 2 2 2 . . . . . . . . . . .
 *  s  2| 1 1 P 2 2 . . . . . . . . . . . . . .    P = 2
 *     1| 1 1 . . . . . . . . . . . . . . . . .
 *      +--------------------------------------
 *        1 2 3 4 5 X-axis 10        15      19
 *
 * Point-Line distance is normalized with the Infinity Norm
 * avoiding square-root code and tightening the test vs the
 * Manhattan Norm. All math is done on the field of integers.
 * The latter replaces the initial ">= MAX(...)" test with
 * "> (ABS(qx-px) + ABS(qy-py))" loosening both inequality
 * and norm, yielding a broader target line for selection.
 * The tightest test is employed here for best discrimination
 * in merging collinear (to grid coordinates) vertex chains
 * into a larger, spanning vectors within the Lemming editor.
 */
	/* addenda: this first set of tests has been added to detect
	 * the case where the line is of zero length. Remove this if
	 * such a case is impossible.
	 */
	if ((px == qx) && (py == qy))	if ((tx == px) && (ty == py)) return 2;
	else return 0;
 if ( abs((qy-py)*(tx-px)-(ty-py)*(qx-px)) >= (UMAX(abs(qx-px), abs(qy-py)))) return(0);
 if (((qx<px)&&(px<tx)) || ((qy<py)&&(py<ty))) return(1);
 if (((tx<px)&&(px<qx)) || ((ty<py)&&(py<qy))) return(1);
 if (((px<qx)&&(qx<tx)) || ((py<qy)&&(qy<ty))) return(3);
 if (((tx<qx)&&(qx<px)) || ((ty<qy)&&(qy<py))) return(3);
 return(2);
}

int isPowerOfTwo (unsigned int x) {
 return ((x != 0) && ((x & (~x + 1)) == x));
}

/* glibc/cmath/c++11/portable Roundings */
double drint(double x) { return (x >= 0.0) ? floor(x + 0.5) : ceil(x - 0.5); }
int	irint( double x ) {	return (int) drint( x ); }

uint64 LAST_TICK=0,TICK=0,TICKSECS=0,FRAME;
uint64 PerfCounterToMillisecondsDivisor=0,FirstQueryPerformanceCounterValue=0;

void InitTICK() {
	// Setup timing data
	LARGE_INTEGER l;
	::QueryPerformanceFrequency( &l );
	PerfCounterToMillisecondsDivisor = l.QuadPart/1000;

	::QueryPerformanceCounter( &l );
	FirstQueryPerformanceCounterValue = l.QuadPart;
}

void GameTICK() {
	LARGE_INTEGER l;
	::QueryPerformanceCounter( &l );

	LAST_TICK = TICK;
	TICK = (l.QuadPart - FirstQueryPerformanceCounterValue) / PerfCounterToMillisecondsDivisor;
 TICKSECS = TICK / 1000;
 FRAME++;
}
