********************************************************************************************
*  __    __________________   ________________________________   __________  ________      *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm)*
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/     *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\     *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/     *
*    --------------------------------------------------------------------------------      *
********************************************************************************************

LOST ASTRONAUT STUDIOS - ATE Complementary Utilities

Hello and welcome to this folder in the ATE (Atlas Tile Editor) package.

I lay no claim to the software included in this folder, it is not mine, I did not
make it.

However, it's useful, free, open-source tools that can help you manipulate your
textures.  I've included them here in their current version, if you need a
newer version you can find their respective project folders.

-------------------------------------------------------------------------------------
GlueIT.exe  (2006) - v1.06
GlueIT lets you glue together a group of image files into a single sheet. It hasn't 
been updated since 2006, so this is the "final version?"
GlueIT 1.04 by Yves Plouffe, July 2006 (Freeware, GarageGames)
https://github.com/Kavex/GlueIT
Also check out Kavex's Game Dev Resources list:
https://github.com/Kavex/GameDev-Resources

-------------------------------------------------------------------------------------
ImageResizer-r129.exe    R129
2D Image Resizer is a free tool that lets you resize images using many different
algorithms.  You can grab the source code, too.  It came off Google Code Archive,
so you may not see it active again, but you should check out their project page:
https://code.google.com/archive/p/2dimagefilter/
Some others have modified it since:
https://github.com/icebreaker/2dimagefilter
One possible use for this tool is to use HQnX filter (or about 50 other scale methods)
to smooth pixelated legacy artwork.

-------------------------------------------------------------------------------------
PngOptimizer.exe  v2.5.1
PNGOptimizer is "Donationware", please donate to Psydk.org if you find it useful.
It makes PNG files smaller by optimizing them!
https://psydk.org/pngoptimizer

-------------------------------------------------------------------------------------
Folder: ImageMagick-7.0.7-11-x86/
Folder: ImageMagick-7.0.7-11-x64/
ImageMagick v7.0.7-11-portable (x64 and x86 versions)
The famous, scriptable, command-line image processing toolset
Examples using ImageMagick to do tiling (GlueIT-style) imagestrips and sprite-sheets: 
 montage -background none -tile 6x6 -geometry 128x128+0+0 *.png walkcycle.png
 montage -background transparent -tile 6x6 -geometry 32x32+0+0 *.png balloons.png
Combine a series of images numbered 0 to n into a single horizontal imagestrip
 convert +append myimage_frame_*.png myimage.png  
Split a large image into a series of tiles:
 convert -crop [tileSizeX]x[tileSizeY] bigImage.png bigImage_tile%d.png  
Source: https://www.imagemagick.org/
Usage Tips: http://www.imagemagick.org/Usage/ and http://www.imagemagick.org/Usage/windows/

-------------------------------------------------------------------------------------
Archived File: gfie3.6.zip  - Greenfish Icon Editor Pro 3.6 
A GPL3 Malware-Free Icon Editor (Donationware) .. support this project if you find it
useful.  You can visit http://greenfishsoftware.org/gfie.php to get the latest version.
"Greenfish Icon Editor Pro (GFIE Pro) is a powerful open source image editor,
especially suitable for creating icons, cursors, animations and icon libraries
...Professional features: layers and high-quality filters
Supports editing animated cursors and managing icon libraries
Not just for icons: GFIE is also a general purpose image editor"
