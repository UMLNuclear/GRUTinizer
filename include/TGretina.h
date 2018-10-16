#ifndef TGRETINA_H
#define TGRETINA_H

#ifndef __CINT__
#include <functional>
#endif

#include <TObject.h>
#include <TMath.h>

#include "TDetector.h"
#include "TGretinaHit.h"

class TGretina : public TDetector {

public:
  TGretina();
  ~TGretina();

  virtual void Copy(TObject& obj) const;
  virtual void Print(Option_t *opt = "") const;
  virtual void PrintInteractions(Option_t *opt = "") const;
  virtual void Clear(Option_t *opt = "");

  virtual UInt_t Size() const { return gretina_hits.size(); }
  virtual Int_t AddbackSize(int EngRange=-1) { BuildAddback(EngRange); return addback_hits.size(); }
  void ResetAddback() { addback_hits.clear();}

  virtual void InsertHit(const TDetectorHit& hit);
  virtual TDetectorHit& GetHit(int i)            { return gretina_hits.at(i); }

  //const TGretinaHit& GetGretinaHit(int i) const { return gretina_hits.at(i); }
  TGretinaHit GetGretinaHit(int i) const { return gretina_hits.at(i); }
  const TGretinaHit& GetAddbackHit(int i) const { return addback_hits.at(i); }


  void PrintHit(int i){ gretina_hits.at(i).Print(); }

  static TVector3 CrystalToGlobal(int cryId,
                                  Float_t localX=0,Float_t localY=0,Float_t localZ=0);
  static TVector3 GetSegmentPosition(int cryid,int segment); //return the position of the segemnt in the lab system
  static TVector3 GetCrystalPosition(int cryid); //return the position of the crysal in the lab system

  //static void DrawDopplerGamma(Double_t Beta=0.1,Option_t *gate="",Option_t *opt="",Long_t entries=kMaxLong,TChain *chain=0);
  //static void DrawDopplerBeta(Option_t *gate="",Option_t *opt="",Long_t entries=kMaxLong,TChain *chain=0);
  //static void DrawEnVsTheta(Double_t Beta=0.1,Option_t *gate="",Option_t *opt="",Long_t entries=kMaxLong,TChain *chain=0);
  //static void DrawCoreSummary(Option_t *gate="",Option_t *opt="",Long_t entries=kMaxLong,TChain *chain=0);



  static double ComptonAngle(double eoriginal,double escatterer);
  static double ComptonEnergy(double eoriginal,double theta);



  
#ifndef __CINT__ 
  static void SetAddbackCondition(std::function<bool(const TGretinaHit&,const TGretinaHit&)> condition) {
    fAddbackCondition = condition;
  }
  static std::function<bool(const TGretinaHit&,const TGretinaHit&)> GetAddbackCondition() {
    return fAddbackCondition;
  }
#endif
  const std::vector<TGretinaHit> &GetAllHits() const { return gretina_hits; }

  void  Sort() { }
  void  SortHits();

private:
  void BuildAddback(int EngRange=-1) const;
#ifndef __CINT__ 
  static std::function<bool(const TGretinaHit&,const TGretinaHit&)> fAddbackCondition;  
#endif
  virtual int BuildHits(std::vector<TRawEvent>& raw_data);

  std::vector<TGretinaHit> gretina_hits;
  mutable std::vector<TGretinaHit> addback_hits; //!

  static Float_t crmat[32][4][4][4];
  static Float_t m_segpos[2][36][3];
  static void SetCRMAT();
  static void SetSegmentCRMAT();
  static bool fCRMATSet;


  ClassDef(TGretina,3);
};


class TCluster { 
  public:
    TCluster()  { Clear(); }
    ~TCluster() {          }

    int  Size() const { return fClusterPoints.size(); }
    void Clear(Option_t *opt="") {
      fEnergySum = 0.00;
      fCenterOfMass.SetXYZ(0,0,0);
      fClusterPoints.clear();
    }
    void Add(TClusterPoint &cp)    { 
      if(Size()==0) {
        fCenterOfMass = cp.GetPosition(); 
      } else {
        double esum = fEnergySum + cp.GetEnergy();
        double x = (fCenterOfMass.X()*fEnergySum  + cp.GetPosition().X()*cp.GetEnergy())/esum;
        double y = (fCenterOfMass.Y()*fEnergySum  + cp.GetPosition().Y()*cp.GetEnergy())/esum;
        double z = (fCenterOfMass.Z()*fEnergySum  + cp.GetPosition().Z()*cp.GetEnergy())/esum;
        fCenterOfMass.SetXYZ(x,y,z);
      }
      fEnergySum+=cp.GetEnergy();
      fClusterPoints.push_back(TClusterPoint(cp)); 
    }
    TClusterPoint Get(int i) const { return fClusterPoints.at(i); }

  private:
    double   fEnergySum;
    TVector3 fCenterOfMass; 
    std::vector<TClusterPoint> fClusterPoints; 

  ClassDef(TCluster,1)  
};

#endif
