#ifndef TGRETINA_H
#define TGRETINA_H

#ifndef __CINT__
#include <functional>
#endif

#include <set>

#include <TObject.h>
#include <TMath.h>

#include "TDetector.h"
#include "TGretinaHit.h"

#include "GRootFunctions.h"

#define CLUSTER_ANGLE 0.383972
//22 degrees in rad. 
class TCluster { 
  public:
    TCluster()  { Clear(); }
    ~TCluster() {          }

    int  Size() const { return fClusterPoints.size(); }
    void Clear(Option_t *opt="") {
      opt=opt; //quiet the warnings.
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
    TVector3 GetPosition()   const { return fCenterOfMass; }
    
    void Print(Option_t *opt="") const {
      printf("---- %6.1f-keV Cluster at ",GetEnergy()); fCenterOfMass.Print(opt); 
      
      for(int x=0;x<Size();x++) {
        printf("\t");  fClusterPoints.at(x).Print(opt);
      }
      printf("\n");
      for(int x=0;x<Size();x++) {
        for(int y=0;y<Size();y++) {
          if(x==y) continue;
          TVector3 v1 = fClusterPoints.at(x).GetPosition();
          TVector3 v2 = fClusterPoints.at(y).GetPosition();
          double scattered_energy = GetEnergy() - fClusterPoints.at(x).GetEnergy();
          double total_energy = GetEnergy();
          double scattered_angle  = v1.Angle(v2-v1) * TMath::RadToDeg(); // 1 -  (2-1)

          double calculated_angle  = GRootFunctions::ComptonAngle(&scattered_energy,&total_energy);  // i am in degrees!
          double calculated_energy  = GRootFunctions::ComptonEnergy(&scattered_angle,&total_energy);  // i am in degrees!
          
          double fom = fabs(scattered_angle-calculated_angle)/
          
          printf("[%i][%i] angle %3.1f / %3.1f = %2.1f    \t  energy %4.1f / %4.1f = %4.1f \n",x,y,
                                               scattered_angle,calculated_angle,scattered_angle/calculated_angle,
                                               scattered_energy,calculated_energy,scattered_energy/calculated_energy);
        }
      }
    }

    void CompressInteractions() { 
      std::vector<TClusterPoint>::iterator it1;
      std::vector<TClusterPoint>::iterator it2;
      for(it1=fClusterPoints.begin();it1!=fClusterPoints.end();it1++) {
        for(it2=it1+1;it2!=fClusterPoints.end();) {
          if(*it1 == *it2) {
            it1->Combine(*it2);
            it2 = fClusterPoints.erase(it2);
          } else {
            it2++;
          }
        }
      }
    }
 
    double GetEnergy() const { return fEnergySum; }
    
    int UniqueXtals() const { 
      std::set<int> xtal; 
      for(unsigned int x=0;x<fClusterPoints.size();x++) xtal.insert(fClusterPoints.at(x).GetXtal());
      return xtal.size();
    }
    int GetXtal(int i=0)  const { return fClusterPoints.at(i).GetXtal(); }
    int GetWedge(int i=0) const { return fClusterPoints.at(i).GetWedge(); }

    TClusterPoint GetPoint(int i) const { return fClusterPoints.at(i); }

  private:
    double   fEnergySum;
    double fFOM_FEP;
    double fFOM_Order;
    
    TVector3 fCenterOfMass; 
    std::vector<TClusterPoint> fClusterPoints; 

  ClassDef(TCluster,22)  
};


















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

  //static double ComptonAngle(double eoriginal,double escatterer);
  //static double ComptonEnergy(double eoriginal,double theta);

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

  int  BuildAddback(int EngRange=-1) const;
  
  int  BuildClusters() const;
  void PrintClusters(Option_t *opt="") const;
  void CompressClusters() { for(unsigned int x=0;x<clusters.size();x++) clusters[x].CompressInteractions(); }
  
  int       ClusterSize()     const { return clusters.size(); }
  TCluster &GetCluster(int i) const { return clusters.at(i);  } 

private:
#ifndef __CINT__ 
  static std::function<bool(const TGretinaHit&,const TGretinaHit&)> fAddbackCondition;  
#endif
  virtual int BuildHits(std::vector<TRawEvent>& raw_data);

  std::vector<TGretinaHit> gretina_hits;
  mutable std::vector<TGretinaHit> addback_hits; //!
  mutable std::vector<TCluster> clusters; //!

  static Float_t crmat[32][4][4][4];
  static Float_t m_segpos[2][36][3];
  static void SetCRMAT();
  static void SetSegmentCRMAT();
  static bool fCRMATSet;

  ClassDef(TGretina,3);
};



#endif
