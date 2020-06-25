

#include <iostream>
#include <cstdio>
#include <fstream>

//#include "GEBStructs.h"
#include "TRawEvent.h"
#include "TRawSource.h"
#include "TRawBanks.h"
#include "Globals.h"

#include "TGretina.h"
#include "TGretinaHit.h"

#include "TSmartBuffer.h"

using namespace std;

int main(int argc, char **argv) {

  if(argc <2) {
    //printf("try ./.aout Merge###.dat instead\n");
    return 1;
  }

  TRawFileIn infile(argv[1]);
  TRawEvent  event;

  int print =1;
  if(argc >2)
    print = atoi(argv[2]);
  printf("printing %i fragment.\n",print);

  bool foundone =false;

  int count = 1;
  bool exitloop=false;
  while(infile.Read(&event)>0) {
    //printf("I AM HERE\n");
    cout << count << " EventType = " << event.GetEventType() << endl;
    switch(event.GetEventType()) {
      case 1:
        if(count==print) {
          event.Print("all");
          foundone = true;

          TSmartBuffer buf = event.GetPayloadBuffer();
          TRawEvent::GEBBankType1 raw = *(const TRawEvent::GEBBankType1*)buf.GetData();

          std::cout << raw << std::endl;

          TGretinaHit hit;
          hit.BuildFrom(buf);
//          hit.Print("all");
          hit.Print();

          exitloop = false;
        }
        count++;
        //gretina data.
        break;
    };
    if(argc>3) {if(count+1 > atoi(argv[3])) exitloop =true;}
    else {if(count+1 >1000) exitloop = true;}
    if(exitloop)
      break;

  }
  (void)foundone; // Stop g++ from complaining that it isn't used.
  printf("\n done.\n\n");
  return 0;
}
