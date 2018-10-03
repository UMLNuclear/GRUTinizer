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

    //void Draw(Option_t *opt="");

    bool operator<(const TMode1Hit &rhs) const {
      return true; //GetAbsSegId() < rhs.GetAbsSegId();
    }

    virtual double GetEnergy() const { return fESum; } 
    virtual double GetESum() const { return fESum; } 
    Float_t GetFirstIntEnergy() const { return fE0; }
    Float_t GetSecondIntEnergy() const { return fE1; }

    Int_t   NumberOfInteractions() const { return fNDet; }
    Float_t GetFOM()               const { return fFom; }
    Int_t   IsTracked()            const { return fTracked; }
    Int_t   GetCrystalId()         const { return fFHXId;   }


    TVector3 GetPostion()          const { return GetFirstIntPosition(); }
    TVector3 GetFirstIntPosition() const { return fFirstInt; }
    TVector3 GetSecondIntPosition() const { return fSecondInt; }


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
