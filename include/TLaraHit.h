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

  bool IsGood() const { if(Time()<4000 && Charge()>10) return true; return false; }
  bool IsBGOGood() const { if(GetBGOTime()>1 && GetBGOTime()<4000) return true; return false; }
  bool HasBGO()    const { return fBGOCharge>150; }

  void SetBGOCharge(int chg) { fBGOCharge=chg;    }
  int  GetBGOCharge() const  { return fBGOCharge; }


private:
  //int   fChannel;

  //double fBGOTime;
  int  fBGOTime;
  int  fBGOCharge;


  ClassDef(TLaraHit,24)
};

#endif /* _TLARAHIT_H_ */
