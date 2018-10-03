#include "TMode1Hit.h"

#include "TMath.h"
#include "TRandom.h"

#include "TGEBEvent.h"
#include "TGRUTOptions.h"
#include "TChannel.h"

ClassImp(TMode1Hit)

TMode1Hit::TMode1Hit(){ }

TMode1Hit::~TMode1Hit() { }



void TMode1Hit::Copy(TObject& obj) const {
  TDetectorHit::Copy(obj);
  TMode1Hit& mode1 = (TMode1Hit&)obj;
  mode1.fESum      = fESum;      
  mode1.fNDet      = fNDet;    
  mode1.fFom       = fFom;     
  mode1.fTracked   = fTracked; 
  mode1.fFirstInt  = fFirstInt;
  mode1.fE0        = fE0;      
  mode1.fSecondInt = fSecondInt;
  mode1.fE1        = fE1;      

}


void TMode1Hit::Print(Option_t *opt) const { }

void TMode1Hit::Clear(Option_t *opt) {
  TDetectorHit::Clear(opt);
  fESum     = sqrt(-1);     
  fNDet     = -1;    
  fFom      = sqrt(-1);     
  fTracked  = -1; 
  fFirstInt.Clear();
  fE0 = sqrt(-1);      
  fSecondInt.Clear();
  fE1 = sqrt(-1);      
  fFHXId = -1;
}

double TMode1Hit::GetDoppler(double beta,const TVector3 *vec) {
  //if(Size()<1)
  //  return 0.0;
  TVector3 unit(0,0,1);
  if(vec==0) {
    vec = &unit;
  }
  double tmp = 0.0;
  double gamma = 1/(sqrt(1-pow(beta,2)));
  tmp = fESum*gamma *(1 - beta*TMath::Cos(GetFirstIntPosition().Angle(*vec)));
  return tmp;
} 
