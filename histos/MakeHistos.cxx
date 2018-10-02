
#include "TRuntimeObjects.h"

//#include <map>

//#include <TH1.h>
//#include <TH2.h>
//#include <TMath.h>
//#include <TRandom.h>
//#include <TObject.h>
//#include <TLine.h>

#include "TGretina.h"
//#include "TBank29.h"

//#include "TChannel.h"
//#include "GValue.h"
//#include "GCutG.h"

#define Q1 15
#define Q2 7
#define Q3 11
#define Q4 16
#define Q5 8
#define Q6 14
#define Q7 12
#define Q8 6
#define Q9 17
#define Q10 9

//#define BETA .37
/*
   std::map<int,int> HoleQMap;
   std::map<int,std::string> LayerMap;

   void InitMap() {
   HoleQMap[Q1] = 1;
   HoleQMap[Q2] = 2;
   HoleQMap[Q3] = 3;
   HoleQMap[Q4] = 4;
   HoleQMap[Q5] = 5;
   HoleQMap[Q6] = 6;
   HoleQMap[Q7] = 7;
   HoleQMap[Q8] = 8;
   HoleQMap[Q9] = 9;
   HoleQMap[Q10] = 10;

   LayerMap[0] = "alpha";
   LayerMap[1] = "beta";
   LayerMap[2] = "gamma";
   LayerMap[3] = "delta";
   LayerMap[4] = "epsilon";
   LayerMap[5] = "phi";

   }
   */
// extern "C" is needed to prevent name mangling.
// The function signature must be exactly as shown here,
//   or else bad things will happen.
extern "C"
void MakeHistograms(TRuntimeObjects& obj) {
  //InitMap();
  TGretina *gretina = obj.GetDetector<TGretina>();
  //TBank29  *bank29  = obj.GetDetector<TBank29>();
  //TList    *list    = &(obj.GetObjects());
  //int numobj        = list->GetSize();

  std::string histname = "";
  std::string dirname  = "";

  //if(bank29) {
  //}

  if(gretina) {

    obj.FillHistogram("mult",40,0,40,gretina->Size());
    for(unsigned int i=0;i<gretina->Size();i++) {
      TGretinaHit hit = gretina->GetGretinaHit(i);
      for(unsigned int j=i+1;j<gretina->Size();j++) {
        TGretinaHit hit2 = gretina->GetGretinaHit(j);
        obj.FillHistogram("ggmat",2000,0,2000,hit.GetCoreEnergy(),
            2000,0,2000,hit2.GetCoreEnergy());
        obj.FillHistogram("ggmat",2000,0,2000,hit2.GetCoreEnergy(),
            2000,0,2000,hit.GetCoreEnergy());

      }


      obj.FillHistogram("summary",2000,0,2000,hit.GetCoreEnergy(),
          200,0,200,hit.GetCrystalId());
      obj.FillHistogram("sum",4000,0,4000,hit.GetCoreEnergy());
      obj.FillHistogram("map",180,1,180,hit.GetThetaDeg(),
          360,1,360,hit.GetPhiDeg());


      //for(int x=0;x<hit.NumberOfInteractions();x++) {
      //  TVector3 ip = hit.GetLocalPosition(x);
      //  obj.FillHistogram(Form("xtal%03i",hit.GetCrystalId()),"position_xy",100,-50,50,ip.X(),
      //      100,-50,50,ip.Y());
      //  obj.FillHistogram(Form("xtal%03i",hit.GetCrystalId()),"position_xz",100,-50,50,ip.X(),
      //      100,0,100,ip.Z());
      //}
    }
  }

  //if(numobj!=list->GetSize())
  //  list->Sort();
}







