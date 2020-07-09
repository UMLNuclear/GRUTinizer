
#include <TMath.h>
#include <TRandom.h>

#include <TUML.h>

#include <TRawEvent.h>
#include <DDASDataFormat.h>
#include <DDASBanks.h>

ClassImp(TUML)

//______________________________________________________________________________
int TUML::BuildHits(std::vector<TRawEvent>& raw_data) {
  for(auto& event : raw_data){
    SetTimestamp(event.GetTimestamp());
    TSmartBuffer buf = event.GetPayloadBuffer();
    int total_size = *(int*)buf.GetData();
    const char* buffer_end = buf.GetData() + total_size;
    buf.Advance(sizeof(int));
    buf.Advance(sizeof(short)); // i am now even more confused.  extra 0x0000 after first ncsl ts
    //buf.Advance(sizeof(int));  // i dont know why this is???  2019-Pt run pcb. 
    //int ptr = sizeof(int);
    while(buf.GetData() < buffer_end) {
      // Constructor advances the buffer to end of each channel
      TDDASEvent<DDASGEBHeader> ddas(buf);
      //std::cout << ddas.GetTimestamp() << "     ddas.GetAddress():   " << ddas.GetAddress() << std::endl;
      
      //
      TUMLHit hit;
      //printf("uml_hit address: 0x%08x\n",ddas.GetAddress()); fflush(stdout);
      hit.SetAddress(ddas.GetAddress());
      hit.SetCharge(ddas.GetEnergy());
      hit.SetTime(ddas.GetCFDTime());
      hit.SetTimestamp(ddas.GetTimestamp());
      hit.SetExternalTimestamp(ddas.GetExternalTimestamp());
      //InsertHit(hit);
      //hit.Print();
      uml_hits.push_back(hit);
      
      int ddas_card    = ddas.GetSlotID();
      int ddas_channel = ddas.GetChannelID();
      //int ddas_energy  = ddas.GetEnergy();
      //unsigned int ddas_address = ddas.GetAddress();
    /*
      std::cout << "address\t0x" << std::hex << ddas_address << std::dec << std::endl; 
      std::cout << "chan   \t" <<  ddas_channel << std::endl; 
      std::cout << "card   \t" <<  ddas_card    << std::endl; 
      std::cout << "eng    \t" <<  ddas_energy << std::endl; 
      std::cout << "--------------------- "  << std::endl;
     */ 
     
        if(ddas_card==2) { // 
          switch(ddas_channel) {
            case 0:     //pin1
              fPin1.SetAddress(ddas.GetAddress());
              fPin1.SetCharge(ddas.GetEnergy());
              fPin1.SetTime(ddas.GetCFDTime());
              fPin1.SetTimestamp(ddas.GetTimestamp());
              fPin1.SetExternalTimestamp(ddas.GetExternalTimestamp());
              break;
            case 1:     //pin2
              fPin2.SetAddress(ddas.GetAddress());
              fPin2.SetCharge(ddas.GetEnergy());
              fPin2.SetTime(ddas.GetCFDTime());
              fPin2.SetTimestamp(ddas.GetTimestamp());
              fPin2.SetExternalTimestamp(ddas.GetExternalTimestamp());
              break;
            case 2:     //implant
              fImplant.SetAddress(ddas.GetAddress());
              fImplant.SetCharge(ddas.GetEnergy());
              fImplant.SetTime(ddas.GetCFDTime());
              fImplant.SetTimestamp(ddas.GetTimestamp());
              fImplant.SetExternalTimestamp(ddas.GetExternalTimestamp());
              break;
            case 3:     //veto
              fVeto.SetAddress(ddas.GetAddress());
              fVeto.SetCharge(ddas.GetEnergy());
              fVeto.SetTime(ddas.GetCFDTime());
              fVeto.SetTimestamp(ddas.GetTimestamp());
              fVeto.SetExternalTimestamp(ddas.GetExternalTimestamp());
              break;
            case 4:     //xfp cfd
              fXfp1.SetAddress(ddas.GetAddress());
              fXfp1.SetCharge(ddas.GetEnergy());
              fXfp1.SetTime(ddas.GetCFDTime());
              fXfp1.SetTimestamp(ddas.GetTimestamp());
              fXfp1.SetExternalTimestamp(ddas.GetExternalTimestamp());
              break;
            case 5:     //xfp analog
              fXfp2.SetAddress(ddas.GetAddress());
              fXfp2.SetCharge(ddas.GetEnergy());
              fXfp2.SetTime(ddas.GetCFDTime());
              fXfp2.SetTimestamp(ddas.GetTimestamp());
              fXfp2.SetExternalTimestamp(ddas.GetExternalTimestamp());
              break;
            case 6:     //tac1  pin1-xfp
              fTac1 = ddas.GetEnergy();
	      //std::cout << fTac1 << std::endl;
              break;
            case 7:     //tac2  pin2-xfp
              fTac2 = ddas.GetEnergy();
              break; 
            case 8:     //tac3  sssd-xfp
              fTac3 = ddas.GetEnergy();
              break;
            case 9:     //tac4  implant-xfp
              fTac4 = ddas.GetEnergy();
              break;
            case 10:    //tac5  pin1-rf
              fTac5 = ddas.GetEnergy();
              break;
            case 11:    //"pin4 high-gain"  -> veto high gain
              break;
            case 12:    //xtal q6p4
              gamma_energy = ddas.GetEnergy(); //!
              gamma_time = ddas.GetTimestamp();   //!
              //std::cout << "ge:  " << gamma_energy << "\t";
              //std::cout << "gt:  " << gamma_time   << "\n";
              break;
            case 13:    //
              break;
            case 14:    // 
              break;
            case 15:    // low hertz pulser
              break;
          };

        } else if(ddas_card==3) { //sssd detector
          TUMLHit hit;
          hit.SetAddress(ddas.GetAddress());
          hit.SetCharge(ddas.GetEnergy());
          hit.SetTime(ddas.GetCFDTime());
          hit.SetTimestamp(ddas.GetTimestamp());
          hit.SetExternalTimestamp(ddas.GetExternalTimestamp());
          //InsertHit(hit);
          fSssd.push_back(hit);
        }

       
    }
  }
  CalParameters();  // Calculate any derived quantities
  fSize = uml_hits.size();
  return fSize;
}


////______________________________________________________________________________
//double TUML::GetSssdEnergy() const {
//  double sum = 0.00;
//  for(size_t i=0; i<fSssd.size(); i++) {
//    sum += fSssd.at(i).GetEnergy();
//  }
//  //std::cout << "sssd energy:   "  << sum << std::endl;
//
//  return sum * GValue::Value("Strip_Slope");
//}


//______________________________________________________________________________
int TUML::CalcStrips() {
  // Determine x position based on centroid calculation.
  double esumch =  0;  // Energy-weighted channel sum
  fSssdESum     =  0;  // Sum of all strip energies
  fSssdEMax     =  0;  // Max energy
  fSssdEMaxCh   = -1;  // Strip with max energy
  fSssdMult     =  0;  // Number of true strips fired
  
  // Loop over all sssd strips
  for(size_t i=0; i<fSssd.size(); i++) {
    if(fSssd.at(i).GetEnergy()<60000 && fSssd.at(i).GetEnergy()>100) {
      fSssdESum += fSssd.at(i).GetEnergy();          // Increment energy sum
      esumch    += (fSssd.at(i).GetChannel()-16)*fSssd.at(i).GetEnergy();// Increment weighted sum
      fSssdMult += 1;    // Number of stips fired for that event
      if(fSssd.at(i).GetEnergy() > fSssdEMax && fSssd.at(i).GetEnergy()<60000) {
        fSssdEMax   = fSssd.at(i).GetEnergy();       // Record maximum energy
        fSssdEMaxCh = (fSssd.at(i).GetChannel()-16); // Record location of maximum energy
      }
    }
  } // end loop over all sssd strips
  
  // Calculate centroid
  // Return an unphysical value if no strips fired.
  if(fSssd.size()<=0) fXPosition = -50;
  double cent= -50;
  if(fSssdESum !=0) {
    cent       = esumch/fSssdESum; // Centroid
    fXPosition = (cent - 7.5 + (gRandom->Uniform()-0.5)) *3.15;
  }
  
  // Calibrate the total SSSD energy sum -- Not needed!
  //fSssdESum = fSssdESum * GValue::Value("Strip_Slope");
  
  return 1;
}


//______________________________________________________________________________
double TUML::CalcTKE() {
//  TKE = GetPin1().GetEnergy() * GValue::Value("TKE_Slope0") +
//        GetPin2().GetEnergy() * GValue::Value("TKE_Slope1") +
//        GetSssdEnergy()       * GValue::Value("TKE_Slope2") +
//        GetImplant().GetEnergy() * GValue::Value("TKE_Slope3") +
//        GValue::Value("TKE_Offset");
  
  // Calculate dE, must use CHARGE since 
  dE  = 0;
  dE += GetPin1().Charge()    * GValue::Value("Tke_Slope0");
  dE += GetPin2().Charge()    * GValue::Value("Tke_Slope1");
  dE += GetSssdEnergy()       * GValue::Value("Tke_Slope2");

  
  // Calculate total kinetic energy
//  TKE = GetPin1().GetEnergy() +
//        GetPin2().GetEnergy() +
//        GetSssdEnergy()       +
//        GetImplant().GetEnergy() +
//        GValue::Value("TKE_Offset");
  
  TKE = GetdE() +
        GetImplant().Charge() * GValue::Value("Tke_Slope3") +
        GValue::Value("Tke_Offset");

  return TKE;
}

//______________________________________________________________________________
double TUML::CalcTof() {
  //  tofselect
  // 0 - tac1
  // 1 - tac2
  // 2 - tac1+tac2  /2
  // 3 - tofd1
  // 4 - tofd2
  // 5 - tofd1+tofd2  /2
  // 6 - tofd1+tofd2+tac1+tac2 /4

  if      (GValue::Value("TOF_select") == 0)
          {
           fTof = GetTof1();
          }
  else if(GValue::Value("TOF_select") == 1)
          {
            fTof = GetTof2();
          }
  else if(GValue::Value("TOF_select") == 2)
          {
            fTof = (GetTof1() + GetTof2())/2.;
          }
  else if(GValue::Value("TOF_select") == 3)
          {
            fTof = GetTofd1();
          }
  else if(GValue::Value("TOF_select") == 4)
          {
             fTof = GetTofd2();
          }
  else if(GValue::Value("TOF_select") == 5)
          {
          fTof = (GetTofd1() + GetTofd2())/2.;
          }
  else if(GValue::Value("TOF_select") == 6)
          {
          fTof = (GetTof1() + GetTof2() + GetTofd1() + GetTofd2())/4.;
          }
  else fTof = 0;
  
  return GetTof();
}


//______________________________________________________________________________
double TUML::CalcdPoPx() {
  if(GetXPosition()>-50) dPoPx = GetXPosition()/GValue::Value("Dispersion");
  else                   dPoPx = -10;
  
  return GetdPoPx();
}

//______________________________________________________________________________
double TUML::CalcBrho() {
  if(GetdPoPx()>-10) brho = GValue::Value("Brho0") * ( 1. + dPoPx / 100.);
  else             brho = 0;
  return brho;
}

//______________________________________________________________________________
void TUML::CalcBetaGamma(){
  if(GetTof() > 0){
    beta = GValue::Value("Length") * (1. + GetdPoPx() * GValue::Value("Disp_Length") /100.)  / GetTof() / GValue::Value("VC");
  }
  
  if(beta < 0.001 && beta > 0.9999) { beta  = 0; gamma = 0; }
  else                                gamma = Beta_to_Gamma(beta);
  
}

//______________________________________________________________________________
double TUML::CalcZ() {
  Z = 0;
  if (GetBeta() > 0) {
    double beta2 = GetBeta()*GetBeta();
    if (beta2 > 0) {
      double dE_v = -1. + log(5930./(1./beta2 - 1.))/beta2; // See eq. A.5 in Tarasov et al., PRC 80 (2009)
      if (dE_v > 0 && dE > 0) {
        //double dE = GetPin1().GetEnergy()*GValue::Value("Tke_Slope0") + GetPin2().GetEnergy()*GValue::Value("Tke_Slope1") + GetSssdEnergy()*GValue::Value("Tke_Slope2") + GValue::Value("TKE_Offset");
//        double dE = GetPin1().GetEnergy()+GetPin2().GetEnergy()+GetSssdEnergy();
        double v = sqrt(GetdE() / dE_v);
        Z =  v*v * GValue::Value("Z2_slope") + v * GValue::Value("Z_slope") + GValue::Value("Z_offset");
        
        //int Zi = (double)Z + 0.5;
        //dZ = (double)Z  - Zi;
      }
    }
  }
  //set Z
  return Z;

  
//
//      b2 = pid.beta*pid.beta;
//      dE_v = -1.+log(5930./(1./b2 - 1.))/b2; // See eq. A.5 in Tarasov et al., PRC 80 (2009)
//
//  //    if (dE_v > 0 && detectors.pin01.energy.isValid()) {// Z is determined from a quadratic-polynom calibration
//      if (dE_v > 0 && pid.dE.isValid()) {// Z is determined from a quadratic-polynom calibration
//  //        v = sqrt(detectors.pin01.energy / dE_v);
//          v = sqrt(pid.dE / dE_v);
//          pid.Z = v * pid.Z_slope + pid.Z_offset;
//          }
//      }
//

}


//______________________________________________________________________________
double TUML::CalcdPoPz() {
  if(Z>0) {
    double dZ = GValue::Value("Z_disp") - GetZ();
    dPoPz = sqrt(fabs(dZ))* GValue::Value("Disp_Z") / 100. ;   // in %
    if(dZ<0) dPoPz = -dPoPz;
  }
  
  return GetdPoPz();
}

//______________________________________________________________________________
double TUML::CalcdPoP(){
  dPoP = GetdPoPz() + GetdPoPx();
  return GetdPoP();
}

//______________________________________________________________________________
void TUML::ReCalBrho(){
  brho = GValue::Value("Brho0") * (1. + GetdPoP() / 100.);
}


//______________________________________________________________________________
double TUML::CalcAoQ() {
//  double AoQ_local = -1;
  if(GetTof()>0 && beta>0 && beta<1 && gamma>0) {
    AoQ = brho / (3.1071 * beta * gamma);
  }else{
    AoQ = 0;
  }
////   double dPoPx = GetXPosition()/GValue::Value("Dispersion");
////   double beta = GValue::Value("Length") * (1 + dPoPx * GValue::Value("Disp_Length") /100.)  / GetTof() / GValue::Value("VC");
////   double gamma = Beta_to_Gamma(beta);
//   if (beta > 0 && beta < 1 )   {
////       brho  = GValue::Value("Brho0") * ( 1 + dPoPx / 100.);
////       if(Z>0) {
////         double dZ = GValue::Value("Z_disp") - GetZ();
////
////         double dPoPz = sqrt(fabs(dZ))* GValue::Value("Disp_Z") / 100. ;
////         if(dZ<0) dPoPz = -dPoPz;
////
////         double dPoP = dPoPz + dPoPx;
////
////        brho = GValue::Value("Brho0") * ( 1 + dPoP/100.);
////
////
////       }
//     AoQ_local = brho / 3.1071 / beta / gamma;
//   } else  {
//     gamma=1;
//     beta=0;
//    }
// }
  return GetAoQ();
  
}


//______________________________________________________________________________
double TUML::CalcQ() {

  if(AoQ > 0 && TKE > 0 && gamma>0) {
     Q = TKE / (gamma- 1.) / (GValue::Value("AEM") * AoQ);
   }
  
  // !!! MAY NEED TO ADD THIS ALTHOUGH OLEG's qi_array IS ALWAYS 0 !!!
//  if (pid.AoQ.isValid() && pid.tke.isValid() && pid.gamma.isValid())
//    if (pid.AoQ > 0 && pid.tke > 0 && pid.gamma > 0)
//  {
//  pid.Q = pid.tke / (pid.gamma - 1.) / (931.494013 * pid.AoQ) ;
//
//  //    Qi -- array from 70 to 75
//  double lef;
//
//    if(pid.Q + pid.qi_array[0] < 69.5) lef =  pid.Q + pid.qi_array[0];
//  else   if(pid.Q + pid.qi_array[0] < 70.5) lef =  pid.Q + pid.qi_array[0];
//  else   if(pid.Q + pid.qi_array[1] < 71.5) lef =  pid.Q + pid.qi_array[1];
//  else   if(pid.Q + pid.qi_array[2] < 72.5) lef =  pid.Q + pid.qi_array[2];
//  else   if(pid.Q + pid.qi_array[3] < 73.5) lef =  pid.Q + pid.qi_array[3];
//  else   if(pid.Q + pid.qi_array[4] < 74.5) lef =  pid.Q + pid.qi_array[4];
//  else   if(pid.Q + pid.qi_array[5] < 75.5) lef =  pid.Q + pid.qi_array[5];
//  else              lef =  pid.Q + pid.qi_array[5];
//
//  int qiv = lef + 0.5;
//  double dev = lef-qiv;
//  if(fabs(dev)< pid.q_gate ) pid.Qi = qiv + 0.1*dev;
//
//  if(pid.Z.isValid()) pid.Zi = pid.Z - pid.zi_slope*dev;
//  }
  
  return GetQ();
}


//______________________________________________________________________________
double TUML::Beta_to_Gamma(double beta) const {
  if(beta<=0) return 1;
  double beta2 = beta*beta;
  double k = 1.- beta2;
  #define minv 1e-40
  if(fabs(k)< minv)  k=minv;
  return sqrt(1./k);
}



//______________________________________________________________________________
void TUML::CalParameters() {
  // Calculate any derived parameters and set values.
  CalcStrips();
  CalcTKE();
  CalcTof();
  CalcdPoPx();
  CalcBrho();
  CalcBetaGamma();
  CalcZ();
  CalcdPoPz();
  CalcdPoP();
  ReCalBrho();
  CalcAoQ();
  CalcQ();
  // CalA
  // CalAraw
  // CalZmQ

//  // Calculate PID parameters
//  double dPoPx = GetXPosition()/GValue::Value("Dispersion");
//  brho  = GValue::Value("Brho0") * ( 1 + dPoPx / 100.);
//  if(GetTof()>0){
//    beta = GValue::Value("Length") * (1 + dPoPx * GValue::Value("Disp_Length") /100.)  / GetTof() / GValue::Value("VC");
//  } else beta = 0;
//  gamma = Beta_to_Gamma(beta);
//  Z = SetZ();
  
//  if(Z>0) {
//    double dZ = GValue::Value("Z_disp") - GetZ();
//
//    double dPoPz = sqrt(fabs(dZ))* GValue::Value("Disp_Z") / 100. ;
//    if(dZ<0) dPoPz = -dPoPz;
//
//    double dPoP = dPoPz + dPoPx;
//
//    brho = GValue::Value("Brho0") * ( 1 + dPoP/100.);
//
//
//  }
//
//  //double AoQ; //!
//  AoQ   = SetAoQ();
  //double Q; //!
//  if(AoQ > 0 && TKE > 0 && gamma>0) {
//    Q = TKE / (gamma- 1.) / GValue::Value("AEM") / AoQ;
//  }
}





//______________________________________________________________________________
void TUML::Copy(TObject& obj) const {
  TDetector::Copy(obj);

  TUML& uml = (TUML&)obj;
  uml.uml_hits = uml_hits;
}
