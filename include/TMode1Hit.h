#ifndef TMODE1HIT_H
#define TMODE1HIT_H

#include "TDetector.h"
#include "TDetectorHit.h"

//#include "TRawBanks.h"
class TSmartBuffer;

class TMode1Hit : public TDetectorHit {
public:
  TMode1Hit();
  ~TMode1Hit();

  virtual void Copy(TObject& obj) const;
  //virtual void Compare(TObject &obj) const;
  virtual void Print(Option_t *opt = "") const;
  virtual void Clear(Option_t *opt = "");

  //virtual void          InsertHit(const TDetectorHit& hit) { return;       }
  //virtual TDetectorHit& GetHit(const int &i=0)             { return hit; }
  //virtual int           Size()                             { return 1;     }
  //void     BuildFrom(TSmartBuffer& buf);
  //void     BuildFrom(TRawEvent::TrackGammaRay&);
  //void Draw(Option_t *opt="");

  bool operator<(const TMode1Hit &rhs) const {
    return true; //GetAbsSegId() < rhs.GetAbsSegId();
  }



//private:

  Float_t fESum;       // gamma-ray energy
  Int_t   fNDet;       // number of interaction points
  Float_t fFom;        // figure of merit
  Int_t   fTracked;    // 1== if tracked
  //Long_t  timestamp;   // timestamp of first interaction
  //Float_t x0,y0,z0,e0;
  TVector3 fFirstInt;
  Float_t  fE0;
  //Float_t x1,y1,z1,e1;
  TVector3 fSecondInt;
  Float_t  fE1;
  Int_t    fFHXId;

  ClassDef(TMode1Hit,2);
};





#endif
