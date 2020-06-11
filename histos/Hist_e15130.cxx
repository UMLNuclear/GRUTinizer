#include "TRuntimeObjects.h"

#include <map>
#include <iostream>

#include "TGretina.h"
#include "TBank88.h"
#include "TUML.h"

#include "TChannel.h"
#include "GValue.h"
#include "GCutG.h"


//quads as of June 2019.
#define Q1 15
#define Q2 7
#define Q3 11
#define Q4 16
#define Q5 8
#define Q6 14
#define Q7 12
#define Q8 17
#define Q9 19
#define Q10 6
#define Q11 9
//#define Q12 20

//#define BETA .37

/*
std::map<int,int> HoleQMap;
std::map<int,std::string> LayerMap;
std::vector<float> w190e;

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
    HoleQMap[Q11] = 11;
    //  HoleQMap[Q12] = 12;
    LayerMap[0] = "alpha";
    LayerMap[1] = "beta";
    LayerMap[2] = "gamma";
    LayerMap[3] = "delta";
    LayerMap[4] = "epsilon";
    LayerMap[5] = "phi";
}
*/


bool GatesSet=false;
GCutG *pin1_pin2_cut=0;
GCutG *pin1_sssd_cut=0;
GCutG *tof1_tof2_cut=0;
GCutG *ZmQ_Z_cut=0;

void SetGates(TRuntimeObjects &obj) {
  if(GatesSet) return;

  TList *gates = &(obj.GetGates());
  TIter iter(gates);
  while(TObject *key = iter.Next()) {
    GCutG *gate = (GCutG*)key;
    if(strcmp(gate->GetName(),"pin1_pin2_cut")==0) {
       pin1_pin2_cut = gate;
    } else if(strcmp(gate->GetName(),"pin1_sssd_cut")==0) {
       pin1_sssd_cut = gate;
    } else if(strcmp(gate->GetName(),"tof1_tof2_cut")==0) {
       tof1_tof2_cut = gate;
    } else if(strcmp(gate->GetName(),"ZmQ_Z_cuts")==0) {
       ZmQ_Z_cut = gate;
    }
  }  

  GatesSet=true;
}













void HandleUML(TRuntimeObjects &obj) {
  TUML *uml = obj.GetDetector<TUML>();

  for(unsigned int i=0; i<uml->fSssd.size(); i++) {
    TUMLHit hit = uml->fSssd.at(i);
    //hit.Print();
    if(hit.Charge()>10) {
      obj.FillHistogram("uml","strips",20,0,20,hit.GetChannel()-15);
    }
    obj.FillHistogram("uml","strips_aligned",6400,0,64000,hit.GetEnergy(),
                                             20,0,20,hit.GetChannel()-15); // 1 -16?
    obj.FillHistogram("uml","strips_charge",6400,0,64000,hit.Charge(),
                                            20,0,20,hit.GetChannel()-15); // 1 -16?
  }

  

   bool egate=false;
   if(pin1_pin2_cut &&  pin1_sssd_cut && tof1_tof2_cut) {
      egate = pin1_pin2_cut->IsInside(uml->GetPin1().GetEnergy(),uml->GetPin2().GetEnergy()) &&
              pin1_sssd_cut->IsInside(uml->GetPin1().GetEnergy(),uml->GetSssdEnergy()) &&     
              tof1_tof2_cut->IsInside(uml->GetTac1(),uml->GetTac2());     
   }

   bool ZmQgate=false;
   if(ZmQ_Z_cut) {
     //printf("found ZmQ_cut\n"); fflush(stdout);
     ZmQgate = ZmQ_Z_cut->IsInside(uml->ZmQ(),uml->GetZ());
   }


  if(egate && ZmQgate) {
    obj.FillHistogram("uml_egated","pin1_pin2",2048,0,2048,uml->GetPin1().GetEnergy(),
                                      2048,0,2048,uml->GetPin2().GetEnergy());
    
    obj.FillHistogram("uml_egated","ZmQ_Z",1000,-15,20,uml->ZmQ(),
                                    1000,50,90,uml->GetZ());

    obj.FillHistogram("uml_egated","pin1_sssd",2048,0,2048,uml->GetPin1().GetEnergy(),
                                      2048,0,8192,uml->GetSssdEnergy());

    obj.FillHistogram("uml_egated","tof1_tof2",2048,0,600,uml->GetTac1(),
                                      2048,0,600,uml->GetTac2());

    obj.FillHistogram("uml_egated","pid",512,-40,8,uml->Am3Q(),
                                         512,50,90,uml->GetZ());
  }
 
    obj.FillHistogram("uml","pin1_pin2",2048,0,2048,uml->GetPin1().GetEnergy(),
                                      2048,0,2048,uml->GetPin2().GetEnergy());
    
    obj.FillHistogram("uml","ZmQ_Z",1000,-15,20,uml->ZmQ(),
                                    1000,50,90,uml->GetZ());

    obj.FillHistogram("uml","pin1_sssd",2048,0,2048,uml->GetPin1().GetEnergy(),
                                      2048,0,8192,uml->GetSssdEnergy());

    obj.FillHistogram("uml","tof1_tof2",2048,0,600,uml->GetTac1(),
                                      2048,0,600,uml->GetTac2());

    obj.FillHistogram("uml","pid",512,-40,8,uml->Am3Q(),
                                  512,50,90,uml->GetZ());
  
  //printf("Z:       %.3f\n",uml->GetZ());
  //printf("Am3Q:    %.3f\n\n",uml->Am3Q()); 
  //fflush(stdout);

  return;
}

// extern "C" is needed to prevent name mangling.
// The function signature must be exactly as shown here,
//   or else bad things will happen.
extern "C"
void MakeHistograms(TRuntimeObjects& obj) {
    //InitMap();
    TGretina *gretina = obj.GetDetector<TGretina>();
    //TBank88  *bank88  = obj.GetDetector<TBank88>();
    TUML     *uml     = obj.GetDetector<TUML>();


    SetGates(obj);


    if(gretina) {
      for(int x=0;x<gretina->Size();x++) {
        TGretinaHit hit = gretina->GetGretinaHit(x);
        obj.FillHistogram("gretina","energy",16000,0,4000,hit.GetCoreEnergy());
        if(uml)  obj.FillHistogram("gretina","energy_withuml",16000,0,4000,hit.GetCoreEnergy());
      }
    }




    if(uml) {
        uml->CalParameters();   ///THIS MUST BE CALLED BEFORE PID IS PLOTED OR GATED
        HandleUML(obj);
    }


    //TList    *list    = &(obj.GetObjects());
    //int numobj        = list->GetSize();

    //if(gretina && uml ) { printf("found both gretina and uml data.\n");
    // } else if(uml )       { printf("found only uml data.\n");
    //z } else if(gretina )   { printf("found only gretina data.\n"); }


    //std::string histname = "";
    //std::string dirname  = "";

    //SetGates(obj);
    //if(uml) {
    //    uml->CalParameters();   ///THIS MUST BE CALLED BEFORE PID IS PLOTED OR GATED
    //    HandleUML(obj);
    //}


    //if(numobj!=list->GetSize())
    //    list->Sort();
}
