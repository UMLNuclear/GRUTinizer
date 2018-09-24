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


  for(size_t x=0;x<lara->Size();x++) {
    TLaraHit hit = lara->GetLaraHit(x);

    obj.FillHistogram("time_energy",2500,0,2500,hit.GetEnergy(),1500,0,1500,lara->Timestamp()/60.);
    obj.FillHistogram(Form("time_energy_LARA%02i",hit.Channel()),2500,0,2500,hit.GetEnergy(),662,0,66200,lara->Timestamp());

    for(size_t y=x+1;y<lara->Size();y++) {
	
      TLaraHit other = lara->GetLaraHit(y);

      obj.FillHistogramSym("ggmat",4000,0,2000,hit.GetEnergy(),4000,0,2000,other.GetEnergy());

if(hit.Channel()!=1 && other.Channel()!=1){

      obj.FillHistogramSym("ggmat134",4000,0,2000,hit.GetEnergy(),4000,0,2000,other.GetEnergy());
    }}

    if(lara->Size()>1) {
      obj.FillHistogram("ggmat_proj",3000,0,3000,hit.GetEnergy());
    }
    
  
 

    obj.FillHistogram("energy_singles",Form("adc%02i",hit.Channel()),4096,0,4096,hit.Charge());

    obj.FillHistogram("energy_singles","sumspec",4096,0,4096,hit.GetEnergy());

    if(hit.Channel()!=1){
      obj.FillHistogram("energy_singles","sumspec134",4096,0,4096,hit.GetEnergy());
    }


    obj.FillHistogram("energy_singles",hit.GetName(),4096,0,4096,hit.GetEnergy());
    
    obj.FillHistogram("energy_singles",Form("%s_bgo",hit.GetName()),4096,0,4096,hit.GetBGOCharge());

    obj.FillHistogram("time_singles",Form("tdc%02i",hit.Channel()),4096,0,4096,hit.Time());
    obj.FillHistogram("time_singles",Form("bgo%02i",hit.Channel()),4096,0,4096,hit.GetBGOTime());

    if(hit.GetBGOTime()<4000){
      obj.FillHistogram("deltas",Form("GeBGOdelta%i",hit.Channel()),1000,-3000,3000,hit.Time()-hit.GetBGOTime());           
      obj.FillHistogram("GeBGOdelta",1000,-3000,3000,hit.Time()-hit.GetBGOTime() + GValue::Value(Form("bgoffset_%i",hit.Channel()))); //aligned GeBGOdelta 
    }
  
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

      if(hit.Channel()>hit1.Channel() && hit.GetEnergy()>hit1.GetEnergy()){

          obj.FillHistogram("deltas",Form("GeDeltaT%1i%1i", hit1.Channel(), hit.Channel()),2048,-2048,2048,hit.Time()-hit1.Time(),1024,0,4096,hit1.GetEnergy());

          obj.FillHistogram("deltas","GeDeltaT",2048,-2048,2048,hit.Time()-hit1.Time(),1024,0,4096,hit1.GetEnergy());//

        }

	else if(hit.Channel()>hit1.Channel() && hit.GetEnergy()<hit1.GetEnergy()){//

          obj.FillHistogram("deltas",Form("GeDeltaT%1i%1i", hit1.Channel(), hit.Channel()),2048,-2048,2048,hit1.Time()-hit.Time(),1024,0,4096,hit.GetEnergy());

          obj.FillHistogram("deltas","GeDeltaT",2048,-2048,2048,hit1.Time()-hit.Time(),1024,0,4096,hit.GetEnergy());//

      	}
	
	else if(hit.Channel()<hit1.Channel() && hit.GetEnergy()>hit1.GetEnergy()){

          obj.FillHistogram("deltas",Form("GeDeltaT%1i%1i", hit.Channel(), hit1.Channel()),2048,-2048,2048,hit.Time()-hit1.Time(),1024,0,4096,hit1.GetEnergy());

          obj.FillHistogram("deltas","GeDeltaT",2048,-2048,2048,hit.Time()-hit1.Time(),1024,0,4096,hit1.GetEnergy());//


        }

	else if(hit.Channel()<hit1.Channel() && hit.GetEnergy()<hit1.GetEnergy()) {//

          obj.FillHistogram("deltas",Form("GeDeltaT%1i%1i", hit.Channel(), hit1.Channel()),2048,-2048,2048,hit1.Time()-hit.Time(),1024,0,4096,hit.GetEnergy());

          obj.FillHistogram("deltas","GeDeltaT",2048,-2048,2048,hit1.Time()-hit.Time(),1024,0,4096,hit.GetEnergy());//
        }


	if(fabs(hit1.Time()-hit.Time())>20 && fabs(hit1.Time()-hit.Time())<40){

	if(hit.Channel()!=1 && hit1.Channel()!=1){

            obj.FillHistogramSym("Random_Background_Matrix34",4000,0,2000,hit.GetEnergy(),
                4000,0,2000,hit1.GetEnergy());
	}
	}

	if(fabs(hit1.Time()-hit.Time())>20 && fabs(hit1.Time()-hit.Time())<40){

            obj.FillHistogramSym("Random_Background_Matrix",4000,0,2000,hit.GetEnergy(),
                4000,0,2000,hit1.GetEnergy());
	}

	if(fabs(hit1.Time()-hit.Time())>20) continue;

	if(hit.Channel()!=1 && hit1.Channel()!=1){

            obj.FillHistogramSym("Coincidence_Matrix34",4000,0,2000,hit.GetEnergy(),4000,0,2000,hit1.GetEnergy());
        }

            obj.FillHistogramSym("Coincidence_Matrix",4000,0,2000,hit.GetEnergy(), 4000,0,2000,hit1.GetEnergy());
          

    }
 

    obj.FillHistogram("multiplicity",6,0,6,lara->Size());  

    if(numobj!=list->GetSize())
      list->Sort();
   
}}

