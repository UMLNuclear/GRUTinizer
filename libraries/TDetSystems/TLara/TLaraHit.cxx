#include "TLaraHit.h"
#include "TLara.h"
#include "TRandom.h"


TLaraHit::TLaraHit():fBGOTime(-1),fBGOCharge(-1) {
  Clear();
}


TLaraHit::TLaraHit(const TDetectorHit &hit) {
  Clear();
  hit.Copy(*this);
}

void TLaraHit::Copy(TObject& obj) const {
  //Clear();
  //fBGOTime=0;
  //fBGOCharge=0;
  TDetectorHit::Copy(obj);
  //TLaraHit& hit = (TLaraHit&)obj;
  ((TLaraHit&)obj).fBGOTime = fBGOTime;
  ((TLaraHit&)obj).fBGOCharge = fBGOCharge;
}

void TLaraHit::Clear(Option_t* opt) {
  TDetectorHit::Clear(opt);
  fBGOTime=-1;
  fBGOCharge=-1;
}

void TLaraHit::Print(Option_t *opt) const {
  //printf("LaBrHit[%i]:   chg[%04i] -> %.02f  |  time[%04i] -> %.02f \n",
  //        fChannel,Charge(),GetEnergy(),Time(),GetTime());
      printf("[%s]\t 0x%08x  HPGe[0x%08x 0x%04x]\tBGO[0x%04x 0x%04x]\n",GetName(),Address(),
                    Charge(),Time(),GetBGOCharge(),GetBGOTime());
}


TVector3 &TLaraHit::GetPosition() const {  } //return TLara::GetPosition(fChannel); }

