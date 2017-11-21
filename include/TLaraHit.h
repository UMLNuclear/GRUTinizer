#ifndef _TLARAHIT_H_
#define _TLARAHIT_H_

#include "TDetectorHit.h"

class TLaraHit : public TDetectorHit {
public:
  TLaraHit();
  TLaraHit(const TDetectorHit&);

  virtual void Print(Option_t* opt = "") const;
  virtual void Clear(Option_t* opt = "");
  virtual void Copy(TObject& obj) const;

  //void SetChannel(int chan)  { fChannel = chan; }
  //int  GetChannel()    const { return fChannel;   }
  
  TVector3 &GetPosition() const;

  int Channel() { return (Address()&0xff); }
  
  double GetBGOTime() const      { return fBGOTime; }
  void   SetBGOTime(double time) { fBGOTime = time; }

private:
  //int   fChannel;

  double fBGOTime;

  ClassDef(TLaraHit,22)
};

#endif /* _TLARAHIT_H_ */
