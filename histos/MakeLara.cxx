#include <TFile.h>
#include "TRuntimeObjects.h"
#include <TMath.h>

#include <TLara.h>
#include <time.h>

#include <GCutG.h>
#include <GValue.h>

GCutG* promptgate =0;
GCutG* randomgate =0;

// extern "C" is needed to prevent name mangling.
// The function signature must be exactly as shown here,
//   or else bad things will happen.
extern "C" 
void MakeHistograms(TRuntimeObjects& obj) {

  TLara *lara = obj.GetDetector<TLara>();
  if(!lara) return;
  TList *list = &(obj.GetObjects());
  int numobj = list->GetSize();

  double GeWindow = 110;
  double BGOGeWindow = 1200;

  if(!promptgate) {
    TList *gates = &(obj.GetGates());
    TIter iter(gates);
    while(TObject *obj = iter.Next()) {
      GCutG *gate = (GCutG*)obj;
      std::string name = gate->GetName();
      if(!name.compare("promptgate")) {
        promptgate = gate;
        std::cout << "found: << " << gate->GetName() << std::endl;
      } else if(!name.compare("randomgate")) {
        randomgate = gate;
        std::cout << "found: << " << gate->GetName() << std::endl;
      }
    } 
  }


  //lara->Print();
  for(size_t x=0;x<lara->Size();x++) {
    TLaraHit hit = lara->GetLaraHit(x);
    //if(hit.Time()<4000){
    //std::cout << lara->Timestamp() << std::endl;
    obj.FillHistogram("time_energy",2500,0,2500,hit.GetEnergy(),3600,0,36000,lara->Timestamp());

    obj.FillHistogram(Form("LARA%02i_multi",hit.Channel()),3000,0,3000,hit.GetEnergy());
    for(size_t y=x+1;y<lara->Size();y++) {
      TLaraHit other = lara->GetLaraHit(y);
      //obj.FillHistogram(Form("LARA%02i_multi_other",hit.Channel()),3000,0,3000,other.GetEnergy());
      obj.FillHistogramSym("ggmat",3000,0,3000,hit.GetEnergy(),3000,0,3000,other.GetEnergy());
    }
    if(lara->Size()>1) {
      obj.FillHistogram("ggmat_proj",3000,0,3000,hit.GetEnergy());
    }
    
    obj.FillHistogram(Form("time_energy_LARA%02i",hit.Channel()),2500,0,2500,hit.GetEnergy(),662,0,66200,lara->Timestamp());

    //obj.FillHistogram("test",Form("%s_gct",hit.GetName()),1064,-10,4096,hit.Charge(),1064,-10,4096,hit.Time());    
    //obj.FillHistogram("test",Form("%s_bct",hit.GetName()),1064,-10,4096,hit.GetBGOCharge(),1064,-10,4096,hit.GetBGOTime());    
    //obj.FillHistogram("test",Form("%s_gbc",hit.GetName()),1064,-10,4096,hit.Charge(),1064,-10,4096,hit.GetBGOCharge());    
    //obj.FillHistogram("test",Form("%s_gbt",hit.GetName()),1064,-10,4096,hit.Time(),1064,-10,4096,hit.GetBGOTime());    

    obj.FillHistogram("energy_singles",Form("adc%02i",hit.Channel()),4096,0,4096,hit.Charge());
    //obj.FillHistogram("energy_singles",Form("adc%02i_v_time",hit.Channel()),500,0,200000,lara->Timestamp(),400,1200,2000,hit.Charge());
    obj.FillHistogram("energy_singles","sumspec",4096,0,4096,hit.GetEnergy());
    obj.FillHistogram("energy_singles",hit.GetName(),4096,0,4096,hit.GetEnergy());
    
    obj.FillHistogram("energy_singles",Form("%s_bgo",hit.GetName()),4096,0,4096,hit.GetBGOCharge());

    obj.FillHistogram("time_singles",Form("tdc%02i",hit.Channel()),4096,0,4096,hit.Time());
    obj.FillHistogram("time_singles",Form("bgo%02i",hit.Channel()),4096,0,4096,hit.GetBGOTime());

    if(hit.GetBGOTime()<4000){
      obj.FillHistogram("deltas",Form("GeBGOdelta%i",hit.Channel()),1000,-3000,3000,hit.Time()-hit.GetBGOTime());           
      obj.FillHistogram("GeBGOdelta",1000,-3000,3000,hit.Time()-hit.GetBGOTime() + GValue::Value(Form("bgoffset_%i",hit.Channel()))); //aligned GeBGOdelta 
    }
  
    
    //std::cout << " hit.GetBGOTime() = " << hit.GetBGOTime() << std::endl;

    //if(std::abs( hit.Time()-hit.GetBGOTime() + GValue::Value(Form("bgoffset_%i",hit.Channel())) ) > BGOGeWindow){ 
    //if( (hit.Time()-hit.GetBGOTime() < -2000) || (hit.Time()-hit.GetBGOTime() > 1000)){ 
    if((hit.GetBGOTime() > 100) && (hit.GetBGOTime() < 4000))  { 
      obj.FillHistogram("energy_singles",Form("adc%02i_suppressed",hit.Channel()),4096,0,4096,hit.Charge());
      obj.FillHistogram("energy_singles",Form("%s_suppressed",hit.GetName()),4096,0,4096,hit.GetEnergy());
    }
    if(!hit.HasBGO()) { 
      obj.FillHistogram("energy_singles",Form("adc%02i_suppressed_2",hit.Channel()),4096,0,4096,hit.Charge());
      obj.FillHistogram("energy_singles",Form("%s_suppressed_2",hit.GetName()),4096,0,4096,hit.GetEnergy());
    }

    for(size_t y=x+1;y<lara->Size();y++) {         //!!!!!!!!!!!!!!!!!MULTIPLICITY CODE STARTS HERE!!!!!!!!!!
      TLaraHit hit1 = lara->GetLaraHit(y);
      //obj.FillHistogramSym("ggmat_nogate",2500,0,2500,hit.GetEnergy(),2500,0,2500,hit1.GetEnergy());
      //if(hit1.Time()<4000){

        //if(hit.Time()>100 && hit1.Time()>100){ 

          obj.FillHistogramSym("Coincidence_Matrix",4096,0,4096,hit.GetEnergy(),4096,0,4096,hit1.GetEnergy());

          obj.FillHistogram("deltas",Form("Gedelta%i_%i",hit.Channel(),hit1.Channel()),1000,-3000,3000,hit.Time()-hit1.Time());
          obj.FillHistogram("deltas",Form("dT%i",hit.Channel()),4096,-4096,4096,hit.Time()-hit1.Time());

          double offset = GValue::Value(Form("offset_%i_%i",hit.Channel(),hit1.Channel()));     	    

          obj.FillHistogram("delta",6000,-3000,3000,hit.Time()-hit1.Time() + offset);    




          float time = hit.Time();
          float time1 = hit1.Time();
          float energy = hit.GetEnergy();
          float energy1 = hit1.GetEnergy();

          if(energy1<energy){
            std::swap(energy,energy1);
            std::swap(time,time1);
            offset = offset * (-1);
          }

          obj.FillHistogram("delta_v_en",3000,-2000,2000,time-time1 + offset,2000,0,4000,energy);

          if(std::abs( hit.Time()-hit.GetBGOTime() + GValue::Value(Form("bgoffset_%i",hit.Channel())) ) > BGOGeWindow &&
              std::abs( hit1.Time()-hit1.GetBGOTime() + GValue::Value(Form("bgoffset_%i",hit1.Channel())) ) > BGOGeWindow){// Compton Supress 

            if(promptgate->IsInside(time-time1+offset,energy)) { // Fit inside walk gate
              obj.FillHistogram("Prompt_Gate","delta_v_en",3000,-2000,2000,time-time1 + offset,2000,0,4000,energy);

              if(std::abs(hit.Time()-hit1.Time() + offset ) < GeWindow){ // G-G Coincidence
                obj.FillHistogramSym("Prompt_Gate","Coincidence_Matrix",4096,0,4096,hit.GetEnergy(),
                    4096,0,4096,hit1.GetEnergy());
              }

            }

            if(randomgate->IsInside(time-time1+offset,energy)) {
              obj.FillHistogram("Random_Gate","delta_v_en",3000,-2000,2000,time-time1 + offset,2000,0,4000,energy);

              if(std::abs(hit.Time()-hit1.Time() + offset ) < GeWindow){ // G-G Coincidence
                obj.FillHistogramSym("Random_Gate","Coincidence_Matrix",4096,0,4096,hit.GetEnergy(),
                    4096,0,4096,hit1.GetEnergy());
              }
            }

            obj.FillHistogramSym("Coincidence_Matrix",4096,0,4096,hit.GetEnergy(),
                4096,0,4096,hit1.GetEnergy());
          }
        //}
      //}
    }
    //}

    obj.FillHistogram("multiplicity",6,0,6,lara->Size());  

    if(numobj!=list->GetSize())
      list->Sort();
  }    
}

