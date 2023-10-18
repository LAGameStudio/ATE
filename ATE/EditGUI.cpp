#include "EditGUI.h"

MoveableEditorWindow::MoveableEditorWindow( MoveableWindowManager *mwm ) : Proce55or() {
 Defaults();
 this->mwm=mwm;
 mwm->Add(this);
}

#include "FilmPickEditor.h"
#include "Mulligan.h"
#include "MulliganEditor.h"
#include "SoundEffect.h"
#include "SoundEffectEditor.h"
#include "ImageCartesiansEditor.h"

void MoveableEditorWindow::ButtonFilmPick(const char *named, Zp<Film> *f, int bx, int by, int bw, int bh) {
 TempFilmReferencer *tfr=tfrs.Find(f);
 fast.text(named,bx+bw/2,by+bh+fast.fontH,true);
 if ( fast.button(fast.filmIcon,bx,by,bw,bh) ) {
  VirtualWindowHandle *existing=null;
  if ( tfr ) {
   existing = v.holder->Editing(tfr);
  }
  if ( existing ) {
   v.holder->SlideTo(existing->p);
  } else {
   TempFilmReferencer *tfr=new TempFilmReferencer;
   tfr->bound=f;
   tfrs.Append(tfr);
   FilmPickEditor *fpe=new FilmPickEditor(f);
   v.holder->Add(&fpe->v);
   fpe->window.Set( x,y+h+8 );
   fpe->v.Editing(tfr);
   fpe->Setup();
   fpe->Name((this->v.title+string(" > Pick a Film ")+string(named)).c_str());
   fpe->v.Attach(&v,true,bx-x,by-y);
   v.holder->SlideTo(&fpe->v);
   return;
  }
 }
}


void MoveableEditorWindow::ButtonFilmPick(const char *label, const char *named, Zp<Film> *f, int bx, int by, int bw, int bh) {
 TempFilmReferencer *tfr=tfrs.Find(f);
 fast.text(label,bx+bw/2,by+bh+fast.fontH,true);
 if ( fast.button(fast.filmIcon,bx,by,bw,bh) ) {
  VirtualWindowHandle *existing=null;
  if ( tfr ) {
   existing = v.holder->Editing(tfr);
  }
  if ( existing ) {
   v.holder->SlideTo(existing->p);
  } else {
   TempFilmReferencer *tfr=new TempFilmReferencer;
   tfr->bound=f;
   tfrs.Append(tfr);
   FilmPickEditor *fpe=new FilmPickEditor(f);
   v.holder->Add(&fpe->v);
   fpe->window.Set( x,y+h+8 );
   fpe->v.Editing(tfr);
   fpe->Setup();
   fpe->Name((this->v.title+string(" > Pick a Film ")+string(named)).c_str());
   fpe->v.Attach(&v,true,bx-x,by-y);
   v.holder->SlideTo(&fpe->v);
   return;
  }
 }
}

void MoveableEditorWindow::ButtonFilmPick( Zp<Film> *f, int bx, int by, int bw, int bh) {
 TempFilmReferencer *tfr=tfrs.Find(f);
 if ( fast.button(fast.filmIcon,bx,by,bw,bh) ) {
  VirtualWindowHandle *existing=null;
  if ( tfr ) {
   existing = v.holder->Editing(tfr);
  }
  if ( existing ) {
   v.holder->SlideTo(existing->p);
  } else {
   TempFilmReferencer *tfr=new TempFilmReferencer;
   tfr->bound=f;
   tfrs.Append(tfr);
   FilmPickEditor *fpe=new FilmPickEditor(f);
   v.holder->Add(&fpe->v);
   fpe->window.Set( x,y+h+8 );
   fpe->v.Editing(tfr);
   fpe->Setup();
   fpe->Name((this->v.title+string(" > Pick a Film ")).c_str());
   fpe->v.Attach(&v,true,bx-x,by-y);
   v.holder->SlideTo(&fpe->v);
   return;
  }
 }
}

void MoveableEditorWindow::ButtonImageCartesians(const char *named, Cartesians * c, Zp<GLImage> * i, int bx, int by, int bw, int bh) {
 if ( fast.button(named,bx,by,bw,bh) ) {
  VirtualWindowHandle *existing=v.holder->Editing(c);
  if ( existing ) {
   v.holder->SlideTo(existing->p);
  } else {
   ImageCartesiansEditor *ice=new ImageCartesiansEditor(i,c);
   v.holder->Add(&ice->v);
   ice->window.Set( x,y+h+8 );
   ice->v.Editing(c);
   ice->Setup();
   ice->Name((this->v.title+string(" > Collide Boxes")+string(named)).c_str());
   ice->v.Attach(&v,true,bx-x,by-y);
   v.holder->SlideTo(&ice->v);
   return;
  }
 }
}

void MoveableEditorWindow::ButtonMulligan(const char *named, Mulligan * in, int bx, int by, int bw, int bh) {
 fast.text(named,bx+bw/2,by+bh+fast.fontH,true);
 if ( fast.button(fast.particlesIcon,bx,by,bw,bh) ) {
  VirtualWindowHandle *existing=v.holder->Editing(in);
  if ( existing ) {
   v.holder->SlideTo(existing->p);
  } else {
   MulliganEditor *me=new MulliganEditor(in);
   v.holder->Add(&me->v);
   me->window.Set( x,y+h+8 );
   me->v.Editing(in);
   me->Setup();
   me->Name((this->v.title+string(" > Mulligan")+string(named)).c_str());
   me->v.Attach(&v,true,bx-x,by-y);
   v.holder->SlideTo(&me->v);
   return;
  }
 }
}

void MoveableEditorWindow::ButtonSound(const char *named, SoundEffect * in, Workspace *c, int bx, int by, int bw, int bh) {
 fast.text(named,bx+bw/2,by+bh+fast.fontH,true);
 if ( fast.button(fast.soundIcon,bx,by,bw,bh) ) {
  VirtualWindowHandle *existing=v.holder->Editing(in);
  if ( existing ) {
   v.holder->SlideTo(existing->p);
  } else {
   SoundEffectEditor *sfe=new SoundEffectEditor(in);
   v.holder->Add(&sfe->v);
   sfe->workspace=c;
   sfe->window.Set( x,y+h+8 );
   sfe->v.Editing(in);
   sfe->Setup();
   sfe->Name((this->v.title+string(" > Sound Effect")+string(named)).c_str());
   sfe->v.Attach(&v,true,bx-x,by-y);
   v.holder->SlideTo(&sfe->v);
   return;
  }
 }
}
