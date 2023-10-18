#include "AtlasImporter.h"

#include "AtlasLibraryBrowser.h"

void AtlasImporter::ScrollLibrary() {
 AtlasLibraryBrowser *alb=(AtlasLibraryBrowser *)back_to.pointer;
 alb->ScrollToEnd();
}