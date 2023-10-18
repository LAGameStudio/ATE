#include "LoadingSequence.h"

#include "AppFramework.h"

void LoadingSequenceThread::Main() {
  while (!done) {
   wglMakeCurrent(display.hdc, app.upload_rc);
   done |= Unlocked();
   wglMakeCurrent(display.hdc, app.game_rc);
   pthreads.Lock();
   done |= Locked();
   pthreads.Unlock();
   iteration++;
  }
  done = true;
  pthreads.Lock();
  OnComplete();
  pthreads.Unlock();
 }