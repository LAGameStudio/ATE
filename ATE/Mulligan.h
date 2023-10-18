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
#include "Tweens.h"
#include "fx_FireAndForget.h"
#include "Trigger.h"
#include "SoundEffect.h"

class Workspace;

//////////////////////////////////////////////////////////////
// Implements a particle effect and a mulligan (set of effects)
// which are an explosion or other temporary visual particle system.
//////////////////////////////////////////////////////////////

// pfx_ExpandContractFaderFilm=?,  was not used because it relies on a Film
enum PFXTypes {
 pfx_SpinningExpandingFading=0,
 pfx_ExpandFader=1,
 pfx_WanderingExpandFader=2,
 pfx_ExpandContractFader=3,
 pfx_LegacyExpandContract=4,
 pfx_ExpandContractBurn=5,
 pfx_ExpandContract2Images=6,
 pfx_SpinningExpandContractFader=7,
 pfx_Flicker=8,
 pfx_Flicker2Images=9,
 pfx_IsoTrap=10
};

ONE(PFX,{
 type=pfx_LegacyExpandContract;
 tint.Pick(alabaster);
 color.Pick(alabaster);
 burn.Pick(alabaster);
 xin=tweens.random();
 xout=tweens.random();
 effectDuration=1.0;
 emitDuration=0.1;
 width=32.0;
 height=32.0;
 widthTo=64.0;
 heightTo=64.0;
 blend=additive;
 in=0.25;
 out=0.75;
 quantity=1;
 emitSpreadAngleRange=90.0;
 circularSpread=true;
 nearby=true;
 deltaWidth=0.5;
 mina=1.0/255.0;
 interval=0.1;
 trailDuration=2.0;
 splitAngle=45.0;
})
 PFXTypes type;
 Zp<GLImage> image,imageA,imageB;
 Zp<Film> film;
 Zp<Tween> xin,xout;
 Blends blend;
 Crayon tint,color,burn;
 Cartesiand velocity;
 Zint quantity,burnSize;
 Zdouble emitDuration,effectDuration,emitSpreadAngleRange,in,out,endIn,startOut,
  width,height,widthTo,heightTo,spin,effectAngle,alpha,deltaWidth,mina,
  burnDelay,splitAngle,waiting,interval,trailDuration;
 Zbool shot,coalesce,calculateDirectionFromShotAngle,ejects;
 Zbool fizzle,shoot,trails,splits,nearby,circularSpread;
 Zdouble nearbyRange,ejectionSpeed;
 Zdis<PFX> trailsEffect; // self-referential, do not instantiate in the constructor
 CLONE(PFX,{
  DUPE(name)
  DUPE(type);
  DUPE(film);
  DUPE(image);
  DUPE(imageA);
  DUPE(imageB);
  DUPE(xin);
  DUPE(xout);
  DUPE(blend);
  DUPE(tint);
  DUPE(color);
  DUPE(burn);
  a->velocity.Set(&velocity);
  DUPE(quantity);
  DUPE(burnSize);
  DUPE(emitDuration);
  DUPE(effectDuration);
  DUPE(emitSpreadAngleRange);
  DUPE(in);
  DUPE(out);
  DUPE(endIn);
  DUPE(startOut);
  DUPE(width);
  DUPE(height);
  DUPE(widthTo);
  DUPE(heightTo);
  DUPE(spin);
  DUPE(effectAngle);
  DUPE(alpha);
  DUPE(deltaWidth);
  DUPE(mina);
  DUPE(burnDelay);
  DUPE(splitAngle);
  DUPE(waiting);
  DUPE(interval);
  DUPE(trailDuration);
  DUPE(shot);
  DUPE(coalesce);
  DUPE(calculateDirectionFromShotAngle);
  DUPE(ejects);
  DUPE(fizzle);
  DUPE(shoot);
  DUPE(trails);
  DUPE(splits);
  DUPE(nearby);
  DUPE(circularSpread);
  DUPE(nearbyRange);
  DUPE(ejectionSpeed);
  if ( trailsEffect ) a->trailsEffect=trailsEffect->Duplicate();
 })
 void LoadTrailEffect( const char *w );
 KEYWORDS({
  KEYWORD("name",{name = w; })
  else KEYWORD("type",{type=(PFXTypes)atoi(w);})
  else KEYWORD("film",{Zstring vaultname=w; NEXTWORD; film=vault.find(vaultname.c_str(), w); })
  else KEYWORD("image",{image=library.Load(w);})
  else KEYWORD("imageA",{image=library.Load(w);})
  else KEYWORD("imageB",{image=library.Load(w);})
  else KEYWORD("xin",{xin=tweens.find(w);})
  else KEYWORD("xout",{xout=tweens.find(w);})
  else KEYWORD("blend",{blend.fromString(w);})
  else KEYWORD("tint",{tint.CSS(w);})
  else KEYWORD("color",{color.CSS(w);})
  else KEYWORD("burn",{burn.CSS(w);})
  else KEYWORD("velocity",{velocity.fromString(w);})
  else KEYWORD("quantity",{quantity=w;})
  else KEYWORD("burnSize",{burnSize=w;})
  else KEYWORD("emitDuration",{emitDuration=w;})
  else KEYWORD("effectDuration",{effectDuration=w;})
  else KEYWORD("emitSpreadAngleRange",{emitSpreadAngleRange=w;})
  else KEYWORD("in",{in=w;})
  else KEYWORD("out",{out=w;})
  else KEYWORD("endIn",{endIn=w;})
  else KEYWORD("startOut",{startOut=w;})
  else KEYWORD("width",{width=w;})
  else KEYWORD("height",{height=w;})
  else KEYWORD("widthTo",{widthTo=w;})
  else KEYWORD("heightTo",{heightTo=w;})
  else KEYWORD("spin",{spin=w;})
  else KEYWORD("effectAngle",{effectAngle=w;})
  else KEYWORD("alpha",{alpha=w;})
  else KEYWORD("deltaWidth",{deltaWidth=w;})
  else KEYWORD("mina",{mina=w;})
  else KEYWORD("burnDelay",{burnDelay=w;})
  else KEYWORD("splitAngle",{splitAngle=w;})
  else KEYWORD("waiting",{waiting=w;})
  else KEYWORD("interval",{interval=w;})
  else KEYWORD("trailDuration",{trailDuration=w;})
  else KEYTAG("shot",{shot=true;})
  else KEYTAG("coalesce",{coalesce=true;})
  else KEYTAG("calculateDirectionFromShotAngle",{calculateDirectionFromShotAngle=true;})
  else KEYTAG("ejects",{ejects=true;})
  else KEYTAG("fizzle",{fizzle=true;})
  else KEYTAG("shoot",{shoot=true;})
  else KEYTAG("trails",{trails=true;})
  else KEYTAG("splits",{splits=true;})
  else KEYTAG("nearby",{nearby=true;})
  else KEYTAG("circularSpread",{circularSpread=true;})
  else KEYWORD("nearbyRange",{nearbyRange=w;})
  else KEYWORD("ejectionSpeed",{ejectionSpeed=w;})
  else KEYWORD("trailsEffect",{LoadTrailEffect(w);})
 })
 WORDKEYS({
  WORDKEY("name",name)
  ENUMKEY("type",type)
  if ( film )  {
   FilmLibrary *library=film->GetLibrary();
   if ( library ) {
    _out+=string("film {")+library->title+string("} {")+film->title("}\n");
   }
  }
  if ( image ) WORDKEY("image",image->filename)
  if ( imageA) WORDKEY("imageA",imageA->filename)
  if ( imageB) WORDKEY("imageB",imageB->filename)
  if ( xin   ) WORDKEY("xin",xin->name)
  if ( xout  ) WORDKEY("xout",xout->name);
  SSUBKEY("blend",blend)
  SSUBKEYCustom("tint",tint,toCSS)
  SSUBKEYCustom("color",color,toCSS)
  SSUBKEYCustom("burn",burn,toCSS)
  SSUBKEY("velocity",velocity)
  WORDKEY("quantity",quantity.toString())
  WORDKEY("burnSize",burnSize.toString())
  WORDKEY("emitDuration",emitDuration)
  WORDKEY("effectDuration",effectDuration)
  WORDKEY("emitSpreadAngleRange",emitSpreadAngleRange)
  WORDKEY("in",in)
  WORDKEY("out",out)
  WORDKEY("endIn",endIn)
  WORDKEY("startOut",startOut)
  WORDKEY("width",width)
  WORDKEY("height",height)
  WORDKEY("widthTo",widthTo)
  WORDKEY("heightTo",heightTo)
  WORDKEY("spin",spin)
  WORDKEY("effectAngle",effectAngle)
  WORDKEY("alpha",alpha)
  WORDKEY("deltaWidth",deltaWidth)
  WORDKEY("mina",mina)
  WORDKEY("burnDelay",burnDelay)
  WORDKEY("splitAngle",splitAngle)
  WORDKEY("waiting",waiting)
  WORDKEY("interval",interval)
  WORDKEY("trailDuration",trailDuration)
  TAGKEY("shot",shot)
  TAGKEY("coalesce",coalesce)
  TAGKEY("calculateDirectionFromShotAngle",calculateDirectionFromShotAngle)
  TAGKEY("ejects",ejects)
  TAGKEY("fizzle",fizzle)
  TAGKEY("shoot",shoot)
  TAGKEY("trails",trails)
  TAGKEY("splits",splits)
  TAGKEY("nearby",nearby)
  TAGKEY("circularSpread",circularSpread)
  TAGKEY("shot",shot)
  WORDKEY("nearbyRange",nearbyRange)
  WORDKEY("ejectionSpeed",ejectionSpeed)
  if ( trailsEffect ) SUBKEY("trailsEffect",trailsEffect)
 })
 Proce55or *GetInner(int x, int y, double vx, double vy, double scx, double scy ) {
  Proce55or *inner=null;
  switch ( type ) { 
   case pfx_SpinningExpandingFading:
     if ( image && xin && xout ) {
      fx_SpinningExpandingFader *sfe=new fx_SpinningExpandingFader;
      sfe->image=image;
      sfe->tint=tint;
      sfe->blend=blend;
      sfe->xin=xin;
      sfe->xout=xout;
      sfe->w=(int)(width*scx);
      sfe->h=(int)(height*scy);
      sfe->angle=uniform()*emitSpreadAngleRange*2.0-emitSpreadAngleRange;
      sfe->spin=spin;
      sfe->duration=effectDuration;
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        sfe->MoveTo(x + (int)(delta.x*scx), y + (int)(delta.y*scy));
       } else {
        int dx = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scx);
        int dy = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scy);
        sfe->MoveTo(x+dx,y+dy);
       }
      } else sfe->MoveTo(x,y);
      inner=(Proce55or *) sfe;
     }
    break;
   case pfx_ExpandFader:
     if ( image && xin ) {
      fx_ExpandFader *xf=new fx_ExpandFader(image,blend,tint,xin,(float)((double)deltaWidth*scx),(float)in,(float)out);
      xf->duration=effectDuration;
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        xf->MoveTo(x+(int)(delta.x*scx),y+(int)(delta.y*scy));
       } else {
        int dx=(int)((uniform()*nearbyRange*2.0-nearbyRange)*scx);
        int dy=(int)((uniform()*nearbyRange*2.0-nearbyRange)*scy);
        xf->MoveTo(x+dx,y+dy);
       }
      } else xf->MoveTo(x,y);
      inner=(Proce55or *) xf;
     }
    break;
   case pfx_WanderingExpandFader:
     if ( image && xin ) {
      fx_WanderingExpandFader *wxf=new fx_WanderingExpandFader(image,blend,tint,xin,(float)((double)deltaWidth*scx),(float)in,(float)out);
      wxf->duration=effectDuration;
      wxf->w=width*scx;
      wxf->h=height*scy;
      if ( calculateDirectionFromShotAngle ) {
       wxf->velocity.x = vx * scx;
       wxf->velocity.y = vy * scy;
      }
      if ( ejects ) {
       Circle c(0.0,0.0,ejectionSpeed*0.75+0.25*ejectionSpeed);
       Vertexd ejection;
       c.Point(uniform(),&ejection);
       wxf->velocity.x+=ejection.x*scx;
       wxf->velocity.x+=ejection.y*scy;
      }
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        wxf->MoveTo(x + (int)(delta.x*scx), y + (int)(delta.y*scy));
        if ( coalesce ) {
         Cartesian dir((int)delta.x,(int)delta.y);
         Circle c(0.0,0.0,(dir.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(dir.LineAngle()/TWO_PI,&ejection);
         wxf->velocity.x+=ejection.x*scx;
         wxf->velocity.y+=ejection.y*scy;
        }
       } else {
        int dx = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scx);
        int dy = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scy);
        wxf->MoveTo(x+dx,y+dy);
        if ( coalesce ) {
         Cartesian delta(dx,dy);
         Circle c(0.0,0.0,(delta.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(delta.LineAngle()/TWO_PI,&ejection);
         wxf->velocity.x+=ejection.x*scx;
         wxf->velocity.y+=ejection.y*scy;
        }
       }
      } else wxf->MoveTo(x,y);
      inner=(Proce55or *) wxf;
     }
    break;
   case pfx_ExpandContractFader:
     if ( image && xin && xout ) {
      fx_ExpandContractFader *xcf=new fx_ExpandContractFader(image,blend,tint,xin,xout,(float)in,(float)out);
      xcf->duration=effectDuration;
      xcf->mina=mina;
      xcf->w=width*scx;
      xcf->h=height*scy;
      if ( calculateDirectionFromShotAngle ) {
       xcf->direction.x=vx*scx;
       xcf->direction.y=vy*scy;
      }
      if ( ejects ) {
       Circle c(0.0,0.0,ejectionSpeed*0.75+0.25*ejectionSpeed);
       Vertexd ejection;
       c.Point(uniform(),&ejection);
       xcf->direction.x+=ejection.x*scx;
       xcf->direction.y+=ejection.y*scy;
      }
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        xcf->MoveTo(x + (int)(delta.x*scx), y + (int)(delta.y*scy));
        if ( coalesce ) {
         Cartesian dir((int)delta.x,(int)delta.y);
         Circle c(0.0,0.0,(dir.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(dir.LineAngle()/TWO_PI,&ejection);
         xcf->direction.x+=ejection.x*scx;
         xcf->direction.y+=ejection.y*scy;
        }
       } else {
        int dx = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scx);
        int dy = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scy);
        xcf->MoveTo(x+dx,y+dy);
        if ( coalesce ) {
         Cartesian delta(dx,dy);
         Circle c(0.0,0.0,(delta.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(delta.LineAngle()/TWO_PI,&ejection);
         xcf->direction.x+=ejection.x*scx;
         xcf->direction.y+=ejection.y*scy;
        }
       }
      } else xcf->MoveTo(x,y);
      inner=(Proce55or *) xcf;
     }
    break;
   case pfx_LegacyExpandContract:
     if ( image && xin && xout ) {
      fx_LegacyExpandContractFader *xcf=new fx_LegacyExpandContractFader(image,blend,tint,xin,xout,(float)in,(float)out);
      xcf->duration=effectDuration;
      xcf->w=width*scx;
      xcf->h=height*scy;
      if ( calculateDirectionFromShotAngle ) {
       xcf->direction.x = vx * scx;
       xcf->direction.y = vy * scy;
      }
      if ( ejects ) {
       Circle c(0.0,0.0,ejectionSpeed*0.75+0.25*ejectionSpeed);
       Vertexd ejection;
       c.Point(uniform(),&ejection);
       xcf->direction.x+=ejection.x*scx;
       xcf->direction.y+=ejection.y*scy;
      }
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        xcf->MoveTo(x + (int)(delta.x*scx), y + (int)(delta.y*scy));
        if ( coalesce ) {
         Cartesian dir((int)delta.x,(int)delta.y);
         Circle c(0.0,0.0,(dir.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(dir.LineAngle()/TWO_PI,&ejection);
         xcf->direction.x+=ejection.x* scx;
         xcf->direction.y+=ejection.y* scy;
        }
       } else {
        int dx = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scx);
        int dy = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scy);
        xcf->MoveTo(x+dx,y+dy);
        if ( coalesce ) {
         Cartesian delta(dx,dy);
         Circle c(0.0,0.0,(delta.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(delta.LineAngle()/TWO_PI,&ejection);
         xcf->direction.x += ejection.x*scx;
         xcf->direction.y += ejection.y*scy;
        }
       }
      } else xcf->MoveTo(x,y);
      inner=(Proce55or *) xcf;
     }
    break;
   case pfx_ExpandContractBurn:
     if ( image && xin && xout ) {
      fx_ExpandContractBurnFader *xcf=new fx_ExpandContractBurnFader(image,blend,tint,xin,xout,(float)in,(float)out);
      xcf->duration=effectDuration;
      xcf->w = width * scx;
      xcf->h = height * scy;
      if ( calculateDirectionFromShotAngle ) {
       xcf->direction.x = vx * scx;
       xcf->direction.y = vy * scy;
      }
      if ( ejects ) {
       Circle c(0.0,0.0,ejectionSpeed*0.75+0.25*ejectionSpeed);
       Vertexd ejection;
       c.Point(uniform(),&ejection);
       xcf->direction.x += ejection.x*scx;
       xcf->direction.y += ejection.y*scy;
      }
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        xcf->MoveTo(x + (int)(delta.x*scx), y + (int)(delta.y*scy));
        if ( coalesce ) {
         Cartesian dir((int)delta.x,(int)delta.y);
         Circle c(0.0,0.0,(dir.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(dir.LineAngle()/TWO_PI,&ejection);
         xcf->direction.x += ejection.x* scx;
         xcf->direction.y += ejection.y* scy;
        }
       } else {
        int dx = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scx);
        int dy = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scy);
        xcf->MoveTo(x+dx,y+dy);
        if ( coalesce ) {
         Cartesian delta(dx,dy);
         Circle c(0.0,0.0,(delta.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(delta.LineAngle()/TWO_PI,&ejection);
         xcf->direction.x += ejection.x*scx;
         xcf->direction.y += ejection.y*scy;
        }
       }
      } else xcf->MoveTo(x,y);
      inner=(Proce55or *) xcf;
     }
    break;
   case pfx_ExpandContract2Images:
     if ( image && imageA && imageB && xin && xout ) {
      fx_ExpandContract2Images *xcf=new fx_ExpandContract2Images(imageA,imageB,blend,tint,xin,xout,(float)in);
      xcf->image=image;
      xcf->duration=effectDuration;
      xcf->w = width * scx;
      xcf->h = height * scy;
      if ( calculateDirectionFromShotAngle ) {
       xcf->velocity.x = vx * scx;
       xcf->velocity.y = vy * scy;
      }
      if ( ejects ) {
       Circle c(0.0,0.0,ejectionSpeed*0.75+0.25*ejectionSpeed);
       Vertexd ejection;
       c.Point(uniform(),&ejection);
       xcf->velocity.x += ejection.x*scx;
       xcf->velocity.y += ejection.y*scy;
      }
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        xcf->MoveTo(x + (int)(delta.x*scx), y + (int)(delta.y*scy));
        if ( coalesce ) {
         Cartesian dir((int)delta.x,(int)delta.y);
         Circle c(0.0,0.0,(dir.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(dir.LineAngle()/TWO_PI,&ejection);
         xcf->velocity.x += ejection.x*scx;
         xcf->velocity.y += ejection.y*scy;
        }
       } else {
        int dx = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scx);
        int dy = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scy);
        xcf->MoveTo(x+dx,y+dy);
        if ( coalesce ) {
         Cartesian delta(dx,dy);
         Circle c(0.0,0.0,(delta.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(delta.LineAngle()/TWO_PI,&ejection);
         xcf->velocity.x += ejection.x*scx;
         xcf->velocity.y += ejection.y*scy;
        }
       }
      } else xcf->MoveTo(x,y);
      inner=(Proce55or *) xcf;
     }
    break;
   case pfx_SpinningExpandContractFader:
     if ( image && xin && xout ) {
      fx_SpinningExpandContractFader *xcf=new fx_SpinningExpandContractFader(image,blend,tint,xin,xout,(float)in,(float)out);
      xcf->duration=effectDuration;
      xcf->w = width * scx;
      xcf->h = height * scy;
      if ( calculateDirectionFromShotAngle ) {
       xcf->velocity.x = vx * scx;
       xcf->velocity.y = vy * scy;
      }
      if ( ejects ) {
       Circle c(0.0,0.0,ejectionSpeed*0.75+0.25*ejectionSpeed);
       Vertexd ejection;
       c.Point(uniform(),&ejection);
       xcf->velocity.x += ejection.x*scx;
       xcf->velocity.y += ejection.y*scy;
      }
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        xcf->MoveTo(x + (int)(delta.x*scx), y + (int)(delta.y*scy));
        if ( coalesce ) {
         Cartesian dir((int)delta.x,(int)delta.y);
         Circle c(0.0,0.0,(dir.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(dir.LineAngle()/TWO_PI,&ejection);
         xcf->velocity.x += ejection.x*scx;
         xcf->velocity.y += ejection.y*scy;
        }
       } else {
        int dx = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scx);
        int dy = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scy);
        xcf->MoveTo(x+dx,y+dy);
        if ( coalesce ) {
         Cartesian delta(dx,dy);
         Circle c(0.0,0.0,(delta.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(delta.LineAngle()/TWO_PI,&ejection);
         xcf->velocity.x += ejection.x*scx;
         xcf->velocity.y += ejection.y*scy;
        }
       }
      } else xcf->MoveTo(x,y);
      inner=(Proce55or *) xcf;
     }
    break;
   case pfx_Flicker:
     if ( image && xin && xout ) {
      fx_Flicker *xcf=new fx_Flicker(image,blend,tint,xin,xout,(float)in);
      xcf->duration=effectDuration;
      xcf->w = width * scx;
      xcf->h = height * scy;
      if ( calculateDirectionFromShotAngle ) {
       xcf->velocity.x = vx * scx;
       xcf->velocity.y = vy * scy;
      }
      if ( ejects ) {
       Circle c(0.0,0.0,ejectionSpeed*0.75+0.25*ejectionSpeed);
       Vertexd ejection;
       c.Point(uniform(),&ejection);
       xcf->velocity.x += ejection.x*scx;
       xcf->velocity.y += ejection.y*scy;
      }
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        xcf->MoveTo(x + (int)(delta.x*scx), y + (int)(delta.y*scy));
        if ( coalesce ) {
         Cartesian dir((int)delta.x,(int)delta.y);
         Circle c(0.0,0.0,(dir.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(dir.LineAngle()/TWO_PI,&ejection);
         xcf->velocity.x += ejection.x*scx;
         xcf->velocity.y += ejection.y*scy;
        }
       } else {
        int dx = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scx);
        int dy = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scy);
        xcf->MoveTo(x+dx,y+dy);
        if ( coalesce ) {
         Cartesian delta(dx,dy);
         Circle c(0.0,0.0,(delta.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(delta.LineAngle()/TWO_PI,&ejection);
         xcf->velocity.x += ejection.x*scx;
         xcf->velocity.y += ejection.y*scy;
        }
       }
      } else xcf->MoveTo(x,y);
      inner=(Proce55or *) xcf;
     }
    break;
   case pfx_Flicker2Images:
     if ( image && imageA && imageB && xin && xout ) {
      fx_Flicker2Images *xcf=new fx_Flicker2Images(imageA,imageB,blend,tint,xin,xout,(float)in);
      xcf->image=image;
      xcf->duration=effectDuration;
      xcf->w = width * scx;
      xcf->h = height * scy;
      if ( calculateDirectionFromShotAngle ) {
       xcf->velocity.x = vx * scx;
       xcf->velocity.y = vy * scy;
      }
      if ( ejects ) {
       Circle c(0.0,0.0,ejectionSpeed*0.75+0.25*ejectionSpeed);
       Vertexd ejection;
       c.Point(uniform(),&ejection);
       xcf->velocity.x += ejection.x*scx;
       xcf->velocity.y += ejection.y*scy;
      }
      if ( nearby ) {
       if ( circularSpread ) {
        Circle c(0,0,uniform()*nearbyRange,36);
        Vertexd delta;
        c.Point(uniform(),&delta);
        xcf->MoveTo(x + (int)(delta.x*scx), y + (int)(delta.y*scy));
        if ( coalesce ) {
         Cartesian dir((int)delta.x,(int)delta.y);
         Circle c(0.0,0.0,(dir.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(dir.LineAngle()/TWO_PI,&ejection);
         xcf->velocity.x += ejection.x*scx;
         xcf->velocity.y += ejection.y*scy;
        }
       } else {
        int dx = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scx);
        int dy = (int)((uniform()*nearbyRange*2.0 - nearbyRange)*scy);
        xcf->MoveTo(x+dx,y+dy);
        if ( coalesce ) {
         Cartesian delta(dx,dy);
         Circle c(0.0,0.0,(delta.Distance(0,0)/nearbyRange)*ejectionSpeed);
         Vertexd ejection;
         c.Point(delta.LineAngle()/TWO_PI,&ejection);
         xcf->velocity.x += ejection.x*scx;
         xcf->velocity.y += ejection.y*scy;
        }
       }
      } else xcf->MoveTo(x,y);
      inner=(Proce55or *) xcf;
     }
    break;
   case pfx_IsoTrap:
     {
     }
    break;
   default: break; // unknown type
  }
  return inner;
 }
 void Implement( Proce55ors *p5, int x, int y, double vx, double vy, double scx=1.0, double scy=1.0, Proce55or *placeBehind=null ) {
  for ( int i=0; i<quantity; i++ ) {
   Proce55or *inner=null,*outer=null;
   inner=GetInner(x,y,vx,vy,scx,scy);
   if ( !inner ) continue;
   if ( fizzle ) {
    outer=(Proce55or *) new fx_Fizzle(inner,(int)(widthTo*scx),(int)(heightTo*scy));
   } else if ( shoot ) {
    outer=(Proce55or *) new fx_Shoot(inner,(float)(vx*scx),(float)(vy*scy));
   }
   if ( !outer ) {
    p5->Add(inner,placeBehind);
    inner->Init();
    inner->Setup();
   } else {
    p5->Add(outer,placeBehind);
    outer->Init();
    outer->Setup();
   }
  }
 }
MANY(PFX,PFXHandle,PFXHandles,"PFX",PFXs,{})
 CLONES(PFX,PFXs)
 void Implement( Proce55ors *out, int x, int y, double vx=0.0, double vy=0.0, double scx=1.0, double scy=1.0, Proce55or *placeBehind=null ) {
  FOREACH(PFX,p) p->Implement(out,x,y,vx,vy,scx,scy,placeBehind);
 }
 KEYWORDSGroup("PFX",PFX);
 WORDKEYSGroup("PFX",PFX);
DONE(PFX);

ONE(Mulligan,{})
 Zp<Workspace> workspace; // Source workspace for keeping items relative
 SoundEffect sound;
 PFXs particleProgram;
 void Set( Mulligan *in ) {
  sound.Set(&in->sound);
  particleProgram.Duplicate(&in->particleProgram);
  workspace=in->workspace;
 }
 CLONE(Mulligan,{
  DUPE(name);
  DUPE(workspace);
  a->sound.Set(&sound);
  a->particleProgram.Duplicate(&particleProgram);
 })
 KEYWORDS({
  KEYWORD("name",{name = w; })
  KEYWORD("particleProgram",{particleProgram.fromString(w);})
  KEYWORD("sound",{sound.fromString(w);})
 })
 WORDKEYS({
  WORDKEY("name",name)
  SSUBKEY("sound",sound);
  SSUBKEY("particleProgram",particleProgram)
 })
 void Implement( Proce55ors *out, int x, int y, double vx=0.0, double vy=0.0, double scx = 1.0, double scy = 1.0, float soundDistanceAttenuation=0.5f, Proce55or *placeBehind=null ) {
  sound.Impulse(soundDistanceAttenuation);
  particleProgram.Implement(out,x,y,vx,vy,scx,scy,placeBehind);
 }
MANY(Mulligan,MulliganHandle,MulliganHandles,"Mulligan",Mulligans,{})
 CLONES(Mulligan,Mulligans)
 KEYWORDSGroup("Mulligan",Mulligan);
 WORDKEYSGroup("Mulligan",Mulligan);
 void Load(const char *s) {
  fromString(file_as_string(s).c_str());
 }
 void Save(const char *s) {
  string_as_file(toString().c_str(), s);
 }
DONE(Mulligan);