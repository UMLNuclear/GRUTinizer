

#include <iostream>
#include <cstdio>
#include <fstream>

//#include "GEBStructs.h"
#include "TRawEvent.h"
#include "TRawSource.h"
#include "TRawBanks.h"
#include "Globals.h"

//#include "TGretina.h"
//#include "TGretinaHit.h"

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


  int count1 = 0;
  int count3 = 0;
  while(infile.Read(&event)>0) {
    //printf("I AM HERE\n");
    TSmartBuffer buf = event.GetPayloadBuffer();
    switch(event.GetEventType()) {
      case 1: 
        {
          //TRawEvent::GEBBankType1 raw = *(const TRawEvent::GEBBankType1*)buf.GetData();
          //std::cout << raw << std::endl;
          count1++;
        }
        break;
      case 3:
        {
          const char *data = buf.GetData();
          TRawEvent::GEBBankType3 raw = *(const TRawEvent::GEBBankType3*)data;
          std::cout << raw << std::endl;
          //event.Print();
          //std::cout << event.GetBodySize() << std::endl;
          //int size = event.GetBodySize()-8;
          //for(int i=0;i<size;i+=4) {
          //  std::cout << i << "\t" <<*((int*)(buf.GetData()+(i))) << "\t" <<
          //                           *((float*)(buf.GetData()+(i))) << std::endl;
          //}
          
          count3++;
        }
        break;
    };
    if(count3>0 && count1>0) break;
  }
  printf("\n count1: %i\n",count1);
  printf(  " count3: %i\n",count3);
  printf("\n done.\n\n");
  return 0;
}
