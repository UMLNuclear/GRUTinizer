#ifndef TMODE1_H
#define TMODE1_H

#include <TObject.h>
#include <TMath.h>

#include <TClonesArray.h>

#include "TDetector.h"
#include "TMode1Hit.h"

class TMode1 : public TDetector {

public:
  TMode1();
  ~TMode1();

  virtual void Copy(TObject& obj) const;
  virtual void Print(Option_t *opt = "") const;
  virtual void Clear(Option_t *opt = "");
  virtual unsigned int Size() const { return mode1_hits.size(); }

  virtual void          InsertHit(const TDetectorHit& hit);
  virtual TDetectorHit& GetHit(int i)            { return mode1_hits.at(i); }


  const TMode1Hit& GetMode1Hit(int i) { return mode1_hits.at(i); }
  void PrintHit(int i){ mode1_hits.at(i).Print(); }

  void Sort() { } //std::sort(mode1_hits.begin(),mode1_hits.end()); }
  //void Draw(Option_t *opt=""); 


private:
  virtual int BuildHits(std::vector<TRawEvent>& raw_data);
  Int_t fNGammas;
  Int_t fUnknown;
  std::vector<TMode1Hit> mode1_hits;

  ClassDef(TMode1,1);
};





#endif
