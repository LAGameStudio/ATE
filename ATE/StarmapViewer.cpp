#include "StarmapViewer.h"
#include "LongRangePanel.h"
#include "SpaceGameView.h"

Zint stashed_circle64tenth,stashed_circle64,stashed_starmapgrid;

void StarmapViewer::LockCourseAndReturn() {
 LongRangePanel *lrp=(LongRangePanel *) back_to.pointer;
 SpaceGameView *sgv=(SpaceGameView *) lrp->back_to.pointer;
 this->destroyMe=true;
 lrp->destroyMe=true;
 sgv->visible=true;
 sgv->hyperspaceDestination=selectedDestination;
}