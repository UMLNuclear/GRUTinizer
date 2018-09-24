#include "TLara.h"

#include <cassert>
#include <iostream>

#include "TNSCLEvent.h"
//#include "TH2.h"
#include "TMath.h"


TLara::TLara() {
}

TLara::~TLara(){
}

void TLara::Copy(TObject& obj) const {
  TDetector::Copy(obj);

  TLara& lara = (TLara&)obj;
  lara.lara_hits = lara_hits;
}

void TLara::Clear(Option_t* opt){
  TDetector::Clear(opt);
  //std::cout << __PRETTY_FUNCTION__ << std::endl;
  lara_hits.clear();
}


int TLara::BuildHits(std::vector<TRawEvent>& raw_data){
  //std::cout << __PRETTY_FUNCTION__ << std::endl;
  Clear();
  for(auto& event : raw_data){
    TNSCLEvent& nscl = (TNSCLEvent&)event;
    SetTimestamp(nscl.GetTimestamp());
    Build_From(nscl);
  }
  return (int)Size();
}

void TLara::InsertHit(const TDetectorHit& hit){
  lara_hits.push_back((TLaraHit)hit);
}

unsigned int TLara::Size() const {
  return lara_hits.size(); //->GetEntries();
}



TLaraHit TLara::GetLaraHit(int i) const {
  const TLaraHit hit = lara_hits.at(i);
  return hit;
}

TDetectorHit& TLara::GetHit(int i){
  return lara_hits.at(i);
}




void TLara::Print(Option_t *opt) const {
  printf("TLara det @ ts: %lu\n",Timestamp());
  printf("\tname\taddr\tchg\ttime\tbgo\n");
  TString sopt(opt);
  for(int i=0;i<Size();i++) {
    TLaraHit hit = GetLaraHit(i);
    if(sopt.Contains("hex")) {
      printf("\t%s\t0x%08x\t0x%04x\t0x%04x\t0x%04x\n",hit.GetName(),hit.Address(),
          hit.Charge(),hit.Time(),hit.GetBGOTime());
    } else {
      printf("\t%s\t0x%08x\t%i\t%i\t%i\n",hit.GetName(),hit.Address(),hit.Charge(),hit.Time(),hit.GetBGOTime());
    }
  }
  printf("---------------------------------\n\n\n");
}

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

// This is where the unpacking happens:
int TLara::Build_From(TNSCLEvent &event){
  Clear();
  //std::cout << __PRETTY_FUNCTION__ << std::endl;
  //printf("\n\n\n\n");
  //Print("hex");

  //event.Print("all");
  Int_t PayloadSize = event.GetPayloadSize();
  if(PayloadSize == 1)
    return 1;
  const char* data = event.GetPayload();
  //data+=2;
  //printf("payloaded size = %i\n",PayloadSize);

  SetTimestamp(event.GetTimestamp());

  int size = *((int*)data);  data += sizeof(unsigned short);
  size*=2;
  unsigned short adcid  = *((short*)data); data+=sizeof(unsigned short);
  unsigned short adchit = *((short*)data); data+=sizeof(unsigned short);
  int nadc=0; 
  unsigned short n = adchit;
  while(n) { nadc+=n&1;n>>=1; }
  //printf("nadc = %i  | 0x%04x\n",nadc,adchit);
  for(int i=0;i<nadc;i++)  {
    unsigned short datum = *((unsigned short*)data); data +=2;
    int chan=-1;
    bool is_bgo=false;
    //std::cout << "datem&0xf00 = " << ((datum&0xf000)>>12) << std::endl;
    if( ((datum&0xf000)>>12) >5) {
      chan = ((datum&0xf000)>>12)-6; 
      is_bgo=true;
    } else {
      chan = ((datum&0xf000)>>12); 
    }

    
  //  if(!FindHit(chan)) {
      TLaraHit hit;
      hit.SetAddress(0x07000000 + chan);
      if(is_bgo)  hit.SetBGOCharge((datum&0x0fff));
      else        hit.SetCharge((datum&0x0fff));
      //printf("[made]  ");hit.Print();
      InsertHit(hit);
  //  } else { 
  //    TLaraHit *hit = FindHit(chan);
  //    if(is_bgo)  hit->SetBGOCharge((datum&0x0fff));
  //    else        hit->SetCharge((datum&0x0fff));
  //    printf("[found] ");hit->Print();
  //  }
    //fflush(stdout);

  }
  //std::cout << std::endl;
  unsigned short tdcid  = *((short*)data); data+=sizeof(unsigned short);
  unsigned short tdchit = *((short*)data); data+=sizeof(unsigned short);
  n = tdchit;
  int ntdc=0; 
  while(n) { ntdc+=n&1;n>>=1; }
  for(int i=0;i<ntdc;i++)  {
    unsigned short datum = *((unsigned short*)data); data +=2;
    //printf("tdc[%i] =\t%i\n",(datum&0xf000)>>12,(datum&0x0fff));
    int tdc=(datum&0xf000)>>12;
    TLaraHit *hit=0;
    switch(tdc) {
      case 4:
        hit = FindHit(0);
        if(hit) hit->SetTime(datum&0x0fff);
        break;
      case 5:
        hit = FindHit(0);
        if(hit) hit->SetBGOTime(datum&0x0fff);
        break;
      case 6:
        hit = FindHit(1);
        if(hit) hit->SetTime(datum&0x0fff);
        break;
      case 7:
        hit = FindHit(1);
        if(hit) hit->SetBGOTime(datum&0x0fff);
        break;
      case 8:
        hit = FindHit(2);
        if(hit) hit->SetTime(datum&0x0fff);
        break;
      case 9:
        hit = FindHit(2);
        if(hit) hit->SetBGOTime(datum&0x0fff);
        break;
      case 10:
        hit = FindHit(3);
        if(hit) hit->SetTime(datum&0x0fff);
        break;
      case 11:
        hit = FindHit(3);
        if(hit) hit->SetBGOTime(datum&0x0fff);
        break;
      case 12:
        hit = FindHit(4);
        if(hit) hit->SetTime(datum&0x0fff);
        break;
      case 13:
        hit = FindHit(4);
        if(hit) hit->SetBGOTime(datum&0x0fff);
        break;
      case 14:
        hit = FindHit(5);
        if(hit) hit->SetTime(datum&0x0fff);
        break;
      case 15:
        hit = FindHit(5);
        if(hit) hit->SetBGOTime(datum&0x0fff);
        break;

    }
  }

  //event.Print("all");
  //Print("hex");

}







TVector3 &TLara::GetPosition(int detector) { } 


