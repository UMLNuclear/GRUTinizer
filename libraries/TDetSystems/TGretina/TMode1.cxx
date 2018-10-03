#include "TMode1.h"

#include "TGEBEvent.h"
#include "GCanvas.h"

#include <unistd.h>
#include <fcntl.h>

#include <TROOT.h>

TMode1::TMode1(){
  Clear();
}

TMode1::~TMode1() {
}

void TMode1::Copy(TObject& obj) const {
  TDetector::Copy(obj);

  TMode1& mode1 = (TMode1&)obj;
  mode1.fNGammas = fNGammas;
  mode1.fUnknown = fUnknown;
  mode1.mode1_hits = mode1_hits;
}

void TMode1::InsertHit(const TDetectorHit& hit){
  //TMode1Hit* new_hit = (TMode1Hit*)mode3_hits->ConstructedAt(Size());
  //hit.Copy(*new_hit);
  mode1_hits.emplace_back((TMode1Hit&)hit);
  //fSize++;
}

int IsValid(const void *p,int len) {
  if(!p) return 0;
  int ret =1;
  int nullfd = open("/dev/null",O_WRONLY);
  if(write(nullfd,p,len)<0) {
    ret = 0;
  }
  close(nullfd);
  return ret;
}


int TMode1::BuildHits(std::vector<TRawEvent>& raw_data){
  static int m1_counter=0;
  int counter2=0;
  //std::cout << "here 1 " << std::endl;
  if(raw_data.size() !=1) return 0;
  for(auto& event : raw_data){
    TSmartBuffer buf = event.GetPayloadBuffer();
    if(!IsValid(buf.GetData(),buf.GetSize())) return 0; 
    //TRawEvent::GEBBankType3 raw = *((const TRawEvent::GEBBankType3*)buf.GetData());
    //fNGammas = raw.ngammas;
    //fUnknown = 0;
    int x=0;
    
    fNGammas  = *((int*)buf.GetData()+x); x+=sizeof(int);  if(x>=buf.GetSize()) return 1;
    fUnknown  = *((int*)buf.GetData()+x); x+=sizeof(int);if(x>=buf.GetSize()) return 1;
    //std::cout << fNGammas << std::endl;
    for(int x=0;x<fNGammas;x++) {
      TMode1Hit hit;
      hit.fESum =  *((float*)buf.GetData()+x); x+=sizeof(float); if(x>=buf.GetSize()) return 1;
      hit.fNDet =  *((int*)buf.GetData()+x); x+=sizeof(int); if(x>=buf.GetSize()) return 1;
      hit.fFom =  *((float*)buf.GetData()+x); x+=sizeof(float);if(x>=buf.GetSize()) return 1; 
      hit.fTracked =  *((int*)buf.GetData()+x); x+=sizeof(int); if(x>=buf.GetSize()) return 1;
      hit.SetTimestamp(*((long*)buf.GetData()+x)); x+=sizeof(long);if(x>=buf.GetSize()) return 1;
      hit.fFirstInt.SetX(*((float*)buf.GetData()+x)); x+=sizeof(float);if(x>=buf.GetSize()) return 1; 
      hit.fFirstInt.SetY(*((float*)buf.GetData()+x)); x+=sizeof(float); if(x>=buf.GetSize()) return 1;
      hit.fFirstInt.SetY(*((float*)buf.GetData()+x)); x+=sizeof(float); if(x>=buf.GetSize()) return 1;
      hit.fE0 = *((float*)buf.GetData()+x); x+=sizeof(float); if(x>=buf.GetSize()) return 1;
      hit.fSecondInt.SetX(*((float*)buf.GetData()+x)); x+=sizeof(float); if(x>=buf.GetSize()) return 1;
      hit.fSecondInt.SetY(*((float*)buf.GetData()+x)); x+=sizeof(float); if(x>=buf.GetSize()) return 1;
      hit.fSecondInt.SetY(*((float*)buf.GetData()+x)); x+=sizeof(float); if(x>=buf.GetSize()) return 1;
      hit.fE1 = *((float*)buf.GetData()+x); x+=sizeof(float); if(x>=buf.GetSize()) return 1;
    }
    
    /*
    for(int x=0;x<raw.ngammas;x++) { 
      TMode1Hit hit;
      hit.SetTimestamp(raw.gammas[x].timestamp); 
      hit.SetAddress(0xffffffff);     // to be replaced by xtal id.
      hit.fESum    = raw.gammas[x].esum;       // gamma-ray energy
      hit.fNDet    = raw.gammas[x].ndet;       // number of interaction points
      hit.fFom     = raw.gammas[x].fom;        // figure of merit
      hit.fTracked = raw.gammas[x].tracked;    // 1== if tracked
      hit.fFirstInt.SetXYZ(raw.gammas[x].x0,raw.gammas[x].y0,raw.gammas[x].z0);
      hit.fE0      = raw.gammas[x].e0;
      hit.fSecondInt.SetXYZ(raw.gammas[x].x1,raw.gammas[x].y1,raw.gammas[x].z1);
      hit.fE1      = raw.gammas[x].e1;
      //hit.fFHXId   = raw.gammas[x].fhcrID;
      InsertHit(hit);
    }
   */ 
  }
  //std::cout << "here 2 " << std::endl;
  return Size();
}

void TMode1::Print(Option_t *opt) const { }

void TMode1::Clear(Option_t *opt) {
  //TDetector::Clear(opt);
  mode1_hits.clear(); //->Clear(opt);//("TMode1Hit");
}


