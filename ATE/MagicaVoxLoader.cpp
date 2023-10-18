#include "MagicaVoxLoader.h"

// This function has been derived from the official Magica Voxel import
// code: https://voxel.codeplex.com/wikipage?title=Sample%20Codes
int MV_ID(int a, int b, int c, int d) { return (a) | (b << 8) | (c << 16) | (d << 24); }// magic number
bool isMagicaVoxel(const string& filename) {
 FILE *fp = NULL;
 fopen_s(&fp, filename.c_str(), "rb");
 if (!fp) return false;
 const int MV_VERSION = 150;
 const int ID_VOX  = MV_ID('V', 'O', 'X', ' ');
 const int ID_MAIN = MV_ID('M', 'A', 'I', 'N');
 const int ID_SIZE = MV_ID('S', 'I', 'Z', 'E');
 const int ID_XYZI = MV_ID('X', 'Y', 'Z', 'I');
 const int ID_RGBA = MV_ID('R', 'G', 'B', 'A');
 // magic number
 int magic;
 fread(&magic, 4, 1, fp);
 if (magic != ID_VOX)
 {
  // Magic number does not match
  return false;
 }
 // version
 int version;
 fread(&version, 4, 1, fp);
 if (version != MV_VERSION) {
  // Version does not match
  return false;
 }
 fclose(fp);
 return true;
}