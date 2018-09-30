#include "TMode1.h"

#include "TGEBEvent.h"
#include "GCanvas.h"

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

int TMode1::BuildHits(std::vector<TRawEvent>& raw_data){
  for(auto& event : raw_data){
    //SetTimestamp(event.GetTimestamp());
    TSmartBuffer buf = event.GetPayloadBuffer();
    //hit.BuildFrom(buf);
    //hit.SetTimestamp(event.GetTimestamp());
    //InsertHit(hit);
    TRawEvent::GEBBankType3 raw = *(const TRawEvent::GEBBankType3*)buf.GetData();
    //std::cout << raw << std::endl;
    fNGammas = raw.ngammas;
    fUnknown = raw.unknown;
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
      //hit.fFHXId   = raw.fhcrID;
      InsertHit(hit);
    }
  }
  return Size();
}

void TMode1::Print(Option_t *opt) const { }

void TMode1::Clear(Option_t *opt) {
  //TDetector::Clear(opt);
  mode1_hits.clear(); //->Clear(opt);//("TMode1Hit");
}


