#include "TMode3.h"

#include "TGEBEvent.h"
#include "GCanvas.h"

#include <TROOT.h>

TMode3::TMode3(){
  //mode3_hits = new TClonesArray("TMode3Hit");
  Clear();
}

TMode3::~TMode3() {
  //delete mode3_hits;
}

void TMode3::Copy(TObject& obj) const {
  TDetector::Copy(obj);

  TMode3& mode3 = (TMode3&)obj;
  mode3.mode3_hits = mode3_hits;
  //mode3_hits->Copy(*mode3.mode3_hits);
}

void TMode3::InsertHit(const TDetectorHit& hit){
  //TMode3Hit* new_hit = (TMode3Hit*)mode3_hits->ConstructedAt(Size());
  //hit.Copy(*new_hit);
  mode3_hits.emplace_back((TMode3Hit&)hit);
  fSize++;
}

int TMode3::BuildHits(std::vector<TRawEvent>& raw_data){
  for(auto& event : raw_data){
    SetTimestamp(event.GetTimestamp());
    TMode3Hit hit;
    TSmartBuffer buf = event.GetPayloadBuffer();
    hit.BuildFrom(buf);
    hit.SetTimestamp(event.GetTimestamp());
    InsertHit(hit);
  }
  return Size();
}

void TMode3::Print(Option_t *opt) const { }

void TMode3::Clear(Option_t *opt) {
  //TDetector::Clear(opt);
  mode3_hits.clear(); //->Clear(opt);//("TMode3Hit");
}

void TMode3::Draw(Option_t *opt) {
  std::map<int,int> pad_counter;
  for(int x=0;x<Size();x++) {
    TMode3Hit m3hit = GetMode3Hit(x);
    std::string cname = Form("xtal_%03i",m3hit.GetCrystalId());
    TCanvas *c = (TCanvas*)gROOT->GetListOfCanvases()->FindObject(cname.c_str());
    if(!c) { 
      c = new GCanvas(cname.c_str(),cname.c_str());
      c->Divide(8,5);
      pad_counter[m3hit.GetCrystalId()] = 1;
    }
    //int col = m3hit.GetSegmentId()%8;
    //int row = m3hit.GetSegmentId()/8;
    //c->cd(col*row+1);
    c->cd(pad_counter[m3hit.GetCrystalId()]++);
    m3hit.Draw();
  }


}

