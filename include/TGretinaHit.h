#ifndef TGRETINAHIT_H
#define TGRETINAHIT_H

#include <TObject.h>
#include <Rtypes.h>
#include <TVector3.h>
#include <TMath.h>
#include <TChain.h>


#include <cmath>

#include "TDetectorHit.h"

#define MAXHPGESEGMENTS 36

class TSmartBuffer;
class TInteractionPoint {

  public:
    TInteractionPoint() { } 
    TInteractionPoint(int seg,float eng,float frac,TVector3 lpos) :
                       fSegNum(seg),fEng(eng),fDecompEng(frac),fLPosition(lpos) { }
    virtual ~TInteractionPoint() { }

    int   GetSegNum()  const { return fSegNum;    }
    float GetPreampE() const { return fEng;       }
    float GetDecompE() const { return fDecompEng; }
    float GetAssignE() const { return fAssignedEng; }
    int   GetOrder()   const { return fOrder; } 
    TVector3 GetPosition(int xtal) const; // { return TGretina::CrystalToGlobal(xtal,
                                          //                                  fLPosition.X(),
                                          //                                  fLPosition.Y(),
                                          //                                  fLPosition.Z()); }
    TVector3 GetLocalPosition() const { return fLPosition; }
  
  private:
    int   fSegNum;
    float fEng;          // energy as recorded by preamp.  energy in mode2 format
    float fDecompEng;    // energy as assigned by decomp.  fraction in mode2 format
    float fAssignedEng;  // percent eng assigned by decomp scaled to the core. not in mode2 format
    int   fOrder;        // interaction order
    //TVector3 fPosition;  // in global coordinates
    TVector3 fLPosition; // in local coordinates.
  ClassDef(TInteractionPoint,1)

};



class TGretinaHit : public TDetectorHit {

public:
  TGretinaHit();
  TGretinaHit(const TGretinaHit& hit){ hit.Copy(*this); }
  ~TGretinaHit();

  void Copy(TObject& obj) const;

  void BuildFrom(TSmartBuffer& raw);

  Long_t   GetTimestamp()       const { return fTimeStamp; }
  Double_t GetTime()            const { return (double)fTimeStamp - (double)fT0; }
  Double_t GetT0()              const { return fT0; }
  Int_t    GetAddress()         const { return fAddress;        }
  Int_t    GetCrystalId()       const { return fCrystalId;      }
  Int_t    GetHoleNumber()      const { return fCrystalId/4-1;  }
  Int_t    GetCrystalNumber()   const { return fCrystalId%4;    }
  Float_t  GetCoreEnergy()      const { return fCoreEnergy;     }
  Int_t    GetCoreCharge(int i) const { return fCoreCharge[i];  }
  Float_t  GetCoreEnergy(int i) const; // { return fCoreCharge[i];  }
  virtual Int_t Charge()        const { return fCoreCharge[3];  }
  virtual Double_t GetEnergy()  const { return fCoreEnergy;     } 

  const char *GetName() const;

  Int_t GetPad() const { return fPad; }

  void  Print(Option_t *opt="") const;
  void  Clear(Option_t *opt="");
  Int_t Compare(const TObject *obj) const; 
  
  Int_t Size() const { return fNumberOfInteractions; }//fSegmentNumber.size(); }

  double GetX() const { return GetPosition().X(); }
  double GetY() const { return GetPosition().Y(); }
  double GetZ() const { return GetPosition().Z(); }

  double GetPhi() {
    double phi = GetPosition().Phi();
    if(phi<0) {
      return TMath::TwoPi()+phi;
    } else {
      return phi;
    }
  }
  double GetTheta() { return GetPosition().Theta(); }
  double GetPhiDeg() { return GetPhi()*TMath::RadToDeg(); }
  double GetThetaDeg() { return GetTheta()*TMath::RadToDeg(); }

  bool HasInteractions() { return fNumberOfInteractions; }
  bool operator<(const TGretinaHit &rhs) const { return fCoreEnergy > rhs.fCoreEnergy; }

  double GetDoppler(double beta,const TVector3 *vec=0);
  double GetDoppler_dB(double beta,const TVector3 *vec=0, double Dta=0);

  TVector3 GetPosition() const; //                  const { return GetIntPosition(0); }

  TInteractionPoint GetInteractionPoint(int i) const { return fInteractions.at(i); }

  Int_t    NumberOfInteractions()     const { return fNumberOfInteractions; }
  Int_t    GetSegmentId(int i)        const { return GetInteractionPoint(i).GetSegNum(); }
  Float_t  GetIntPreampEng(int i)     const { return GetInteractionPoint(i).GetPreampE(); }
  Float_t  GetIntDecomEng(int i)      const { return GetInteractionPoint(i).GetDecompE(); }
  Float_t  GetIntAssignEng(int i)     const { return GetInteractionPoint(i).GetAssignE(); }
  TVector3 GetIntPosition(int i)      const { return GetInteractionPoint(i).GetPosition(GetCrystalId()); }
  TVector3 GetLocalIntPosition(int i) const { return GetInteractionPoint(i).GetLocalPosition(); }
  
  void   SetCoreEnergy(float temp) const { fCoreEnergy = temp; }


//private:
  //void SortHits();
  void SortInts();

  Long_t  fTimeStamp;
  Float_t fT0; //WalkCorrection;

  Int_t   fAddress;
  Int_t   fCrystalId;
  mutable Float_t fCoreEnergy;
  Int_t   fCoreCharge[4];

  Int_t   fPad;

  //Int_t   fFirstInteraction;
  //Int_t   fSecondInteraction;

  Int_t   fNumberOfInteractions;

  /// The number of the segment containing the interaction.
  /**
     Note: This is not equal to the segment number as read from the datastream.
     This is equal to 36*raw.crystal_id + raw.segnum.  ///not anymore pcb.
   */
  //std::vector<Int_t> fSegmentNumber; //[fNumberOfInteractions]

  /// The position of the interaction point in lab coordinates
  /**
     Note: this is not equal to the position as read from the datastream.
     This has been transformed to lab coordinates.
     To get the crystal coordinate, use TGretinaHit::GetCrystalPosition(int i).
   */
  //std::vector<TVector3> fGlobalInteractionPosition; //[fNumberOfInteractions]
  //std::vector<TVector3> fLocalInteractionPosition; //[fNumberOfInteractions]
  //std::vector<Float_t>  fInteractionEnergy; //[fNumberOfInteractions]
  //std::vector<Float_t>  fInteractionFraction; //[fNumberOfInteractions]

  std::vector<TInteractionPoint> fInteractions; //[fNumberOfInteractions];

  ClassDef(TGretinaHit,6)
};


#endif




