#ifndef _TUML_H_
#define _TUML_H_

#include <TVector3.h>

#include <TDetector.h>
#include <TUMLHit.h>

#include <TChannel.h>
#include <GValue.h>

class TUML : public TDetector {
  public:
    TUML() { }
    ~TUML() { }

    void Copy(TObject &obj) const; //TDetector::Copy(obj); }

    void Clear(Option_t *opt="") { TDetector::Clear(opt); uml_hits.clear(); }
    
    void InsertHit(const TDetectorHit&) {   }
    TDetectorHit &GetHit(int i) { return uml_hits.at(i); } 
    static TVector3 GetPosition(int i) { return TVector3(0,0,1); }

    size_t Size()     const    { return uml_hits.size(); }
    size_t SizeSssd() const    { return fSssd.size(); }


    TUMLHit GetUMLHit(int i)   { return uml_hits[i]; }
    TUMLHit GetPin1()    const { return fPin1; }
    TUMLHit GetPin2()    const { return fPin2; }
    TUMLHit GetImplant() const { return fImplant; }
    TUMLHit GetVeto()    const { return fVeto; }

    TUMLHit GetXfp1()    const { return fXfp1; }
    TUMLHit GetXfp2()    const { return fXfp2; }

   // coefficients added by e15130.cal, below returns tof value
    double GetTof1()     const { return fTac1 > 0 ? fTac1*GValue::Value("TOF1_slope") / 1000. +GValue::Value("TOF1_offset") : 0; }
    double GetTof2()     const { return fTac2 > 0 ? fTac2*GValue::Value("TOF2_slope") / 1000. +GValue::Value("TOF2_offset") : 0; }
    double GetTofd1()     const { return fTac1 > 0 ? fTac1*GValue::Value("TOFD1_slope") / 1000. +GValue::Value("TOFD1_offset") : 0; }
    double GetTofd2()     const { return fTac2 > 0 ? fTac2*GValue::Value("TOFD2_slope") / 1000. +GValue::Value("TOFD2_offset") : 0; }

    double GetTac1()     const { return fTac1; }
    double GetTac2()     const { return fTac2; }
    double GetTac3()     const { return fTac3; }
    double GetTac4()     const { return fTac4; }
    double GetTac5()     const { return fTac5; }

    double GetTof()      const { return fTof; }// this should be ns
  
    bool Good() const { return true; }

    double GetSssdGMEnergy() const { return fSssdESum; }
    double GetSssdEnergy()   const { return fSssdECalSum; }
    size_t GetSssdRawMult()  const { return SizeSssd(); }
    int    GetSssdMult()     const { return fSssdMult; }
    double GetXPosition()    const { return fXPosition; }
      
    double Araw() const { return AoQ * Z; }
    double Am2Z() const { return Araw() - 2*Z; }
    double Am3Z() const { return Araw() - 3*Z; }
     
    double A() const    { return AoQ * Q; }
    double Am2Q() const { return A() - 2*Q; }
    double Am3Q() const { return A() - 3*Q; }

    double ZmQ() const  { return Z - Q; }
    
    double GetdE()    const { return dE;     }
    double GetTKE()   const { return TKE;    }
    double GetBeta()  const { return beta;   }
    double GetGamma() const { return gamma;  }
    double GetdPoPx() const { return dPoPx;  }
    double GetdBrho() const { return dbrho;  }
    double GetBrho()  const { return brho;   }
    double GetZ()     const { return Z;      } // Do not call with TTree->Draw
    double GetdPoPz() const { return dPoPz;  }
    double GetdPoP()  const { return dPoP;   }
    double GetAoQ()   const { return AoQ;    }
    double GetQ()     const { return Q;      }

    void CalParameters();
    

  //private:
    int BuildHits(std::vector<TRawEvent>& raw_data);

    double fTac1 = 0.;   // pin1-xfp 
    double fTac2 = 0.;   // pin2-xfp 
    double fTac3 = 0.;   // sssd-xfp 
    double fTac4 = 0.;   // implant-xfp 
    double fTac5 = 0.;   // pin1-xfp
  
    double fTof1  = 0.;
    double fTof2  = 0.;
    double fTofd1 = 0.;
    double fTofd2 = 0.;
    double fTof   = 0.;

    TUMLHit fPin1; 
    TUMLHit fPin2; 
    TUMLHit fImplant;  // pin3
    TUMLHit fVeto; 

    TUMLHit fXfp1; 
    TUMLHit fXfp2; 
    
    std::vector<TUMLHit> fSssd;
    std::vector<TUMLHit> uml_hits;
  
    double fXPosition   = -50.0;
    int    fSssdMult    =   0;
    double fSssdESum    =   0;
    double fSssdECalSum =   0;
    double fSssdEMax    =   0;
    int    fSssdEMaxCh  =  -1;

   ///////////////////////
   //// Oleg ToF Things //
   ///////////////////////

    int    CalcStrips();
    double CalcTKE();
    double CalcTof();
    double CalcdPoPx();
    double CalcBrho();
    void   CalcBetaGamma();
    double CalcZ();
    double CalcdPoPz();
    double CalcdPoP();
    void   ReCalBrho();
    double CalcAoQ();
    double CalcQ();
    double Beta_to_Gamma(double beta) const;
  

    double dE = 0.;
    double TKE = 0.;
  double dPoPx = -100.;
  double dbrho = -100.;
    double brho = 0.;
    double Z = 0.;
    double beta = 0.;
    double gamma = 0.;
  double dPoPz = -100.;
  double dPoP = -100.;
  double AoQ = 0.;
    double Q = 0.;

  double gamma_energy = 0.;
  double gamma_time = 0.;

   ////////////////////
   ////////////////////
   ////////////////////
  ClassDef(TUML,6);
};

#endif
