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

Int_t IsValid(const void *p,Int_t len) {
  if(!p) return 0;
  Int_t ret =1;
  Int_t nullfd = open("/dev/null",O_WRONLY);
  if(write(nullfd,p,len)<0) {
    ret = 0;
  }
  close(nullfd);
  return ret;
}


Int_t TMode1::BuildHits(std::vector<TRawEvent>& raw_data){
  static Int_t m1_counter=0;
  Int_t counter2=0;
  //std::cout << "here 1 " << std::endl;
  //if(raw_data.size() !=1) return 0;
  for(auto& event : raw_data){
    TSmartBuffer buf = event.GetPayloadBuffer();
    //if(!IsValid(buf.GetData(),buf.GetSize())) return 0; 
    //iTRawEvent::GEBBankType3 raw = *((const TRawEvent::GEBBankType3*)buf.GetData());
    //fNGammas = raw.ngammas;
    //fUnknown = 0;
    Int_t x=0;
    //event.Print("all");
    fNGammas  = *((Int_t*)(buf.GetData()+x)); x+=sizeof(Int_t);  //if(x>=buf.GetSize()) return 1;
    fUnknown  = *((Int_t*)(buf.GetData()+x)); x+=sizeof(Int_t);  //if(x>=buf.GetSize()) return 1;
    //std::cout << std::dec <<fNGammas << std::endl;
    //std::cout << fUnknown << std::endl;
    for(Int_t y=0;y<fNGammas;y++) {
      TMode1Hit hit;
      hit.fESum =  *((Float_t*)(buf.GetData()+x)); x+=sizeof(Float_t); //if(x>=buf.GetSize()) return 1;
      //std::cout << std::hex << hit.fESum << std::endl;
      hit.fNDet =  *((Int_t*)(buf.GetData()+x)); x+=sizeof(Int_t); //if(x>=buf.GetSize()) return 1;
      hit.fFom =  *((Float_t*)(buf.GetData()+x)); x+=sizeof(Float_t);//if(x>=buf.GetSize()) return 1; 
      hit.fTracked =  *((Int_t*)(buf.GetData()+x)); x+=sizeof(Int_t); ///if(x>=buf.GetSize()) return 1;
      hit.SetTimestamp(*((Long_t*)(buf.GetData()+x))); x+=sizeof(Long_t);//if(x>=buf.GetSize()) return 1;
      hit.fFirstInt.SetX(*((Float_t*)(buf.GetData()+x))); x+=sizeof(Float_t);//if(x>=buf.GetSize()) return 1; 
      hit.fFirstInt.SetY(*((Float_t*)(buf.GetData()+x))); x+=sizeof(Float_t);// if(x>=buf.GetSize()) return 1;
      hit.fFirstInt.SetY(*((Float_t*)(buf.GetData()+x))); x+=sizeof(Float_t);// if(x>=buf.GetSize()) return 1;
      hit.fE0 = *((Float_t*)(buf.GetData()+x)); x+=sizeof(Float_t);// if(x>=buf.GetSize()) return 1;
      hit.fSecondInt.SetX(*((Float_t*)(buf.GetData()+x))); x+=sizeof(Float_t);// if(x>=buf.GetSize()) return 1;
      hit.fSecondInt.SetY(*((Float_t*)(buf.GetData()+x))); x+=sizeof(Float_t);// if(x>=buf.GetSize()) return 1;
      hit.fSecondInt.SetY(*((Float_t*)(buf.GetData()+x))); x+=sizeof(Float_t); //if(x>=buf.GetSize()) return 1;
      hit.fE1 = *((Float_t*)(buf.GetData()+x)); x+=sizeof(Float_t); //if(x>=buf.GetSize()) return 1;
      hit.fFHXId = *((Short_t*)(buf.GetData()+x)); x+=sizeof(Short_t); //if(x>=buf.GetSize()) return 1;
      InsertHit(hit);
    }
     
    /*
    for(Int_t x=0;x<raw.ngammas;x++) { 
      TMode1Hit hit;
      hit.SetTimestamp(raw.gammas[x].timestamp); 
      hit.SetAddress(0xffffffff);     // to be replaced by xtal id.
      hit.fESum    = raw.gammas[x].esum;       // gamma-ray energy
      hit.fNDet    = raw.gammas[x].ndet;       // number of Int_teraction poInt_ts
      hit.fFom     = raw.gammas[x].fom;        // figure of merit
      hit.fTracked = raw.gammas[x].tracked;    // 1== if tracked
      hit.fFirstInt_t.SetXYZ(raw.gammas[x].x0,raw.gammas[x].y0,raw.gammas[x].z0);
      hit.fE0      = raw.gammas[x].e0;
      hit.fSecondInt_t.SetXYZ(raw.gammas[x].x1,raw.gammas[x].y1,raw.gammas[x].z1);
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


