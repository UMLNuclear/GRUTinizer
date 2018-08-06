#ifndef _TLARA_H_
#define _TLARA_H_


#include "TDetector.h"
#include "TLaraHit.h"

class TNSCLEvent;

class TLara : public TDetector {
public:
  TLara();
  virtual ~TLara();

  void Copy(TObject& obj) const;

  virtual void Print(Option_t* opt = "") const;
  virtual void Clear(Option_t* opt = "");

  virtual void InsertHit(const TDetectorHit&);
  virtual TLaraHit      GetLaraHit(int i) const;
  virtual TLaraHit     *FindHit(int ChannelNumber) {
    for(size_t x=0;x<lara_hits.size();x++) {
      if(lara_hits.at(x).Channel()==ChannelNumber) {
        //return &lara_hits[x];
        return lara_hits.data()+x;
      }  
    }
    return 0;
  }

  virtual TDetectorHit& GetHit(int i);
  virtual unsigned int Size() const;

  static TVector3& GetPosition(int); 
  


private:
  virtual int  BuildHits(std::vector<TRawEvent>& raw_data);

  int Build_From(TNSCLEvent &event);

  std::vector<TLaraHit> lara_hits;

  ClassDef(TLara,1);
};

#endif /* _TLARA_H_ */
