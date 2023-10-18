#include "MulliganMixer.h"


void MulliganMixer::AddMulligan() {
 MulliganAddSelector *m = new MulliganAddSelector;
 m->workspace = this->workspace;
 m->mixer = this;
 this->mwm->Add(m);
 m->Repopulate();
 m->v.Attach(&v, w, h/4);
 v.holder->Arrange2Rows();
 v.holder->SlideTo(&m->v);
 v.holder->MoveStayInFrontToTop();
}