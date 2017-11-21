#include "TLaraHit.h"
#include "TLara.h"
#include "TRandom.h"


TLaraHit::TLaraHit() {
  Clear();
}


TLaraHit::TLaraHit(const TDetectorHit &hit) {
  Clear();
  hit.Copy(*this);
}

void TLaraHit::Copy(TObject& obj) const {
  TDetectorHit::Copy(obj);
  TLaraHit& hit = (TLaraHit&)obj;
}

void TLaraHit::Clear(Option_t* opt) {
  TDetectorHit::Clear(opt);
}

void TLaraHit::Print(Option_t *opt) const {
  //printf("LaBrHit[%i]:   chg[%04i] -> %.02f  |  time[%04i] -> %.02f \n",
  //        fChannel,Charge(),GetEnergy(),Time(),GetTime());
}


TVector3 &TLaraHit::GetPosition() const {  } //return TLara::GetPosition(fChannel); }

