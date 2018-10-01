
struct GEBHeader {
  int type;
  int size;
  long timestamp;
};


struct IntPoint { // HPGe Segment Hit Type 1;
   float     x;
   float     y;
   float     z;
   float     e;        //fraction of the energy
   int       seg;
   float     seg_e; //energy (charge) collected in the segment
};

#define MAX_INTPTS 16

struct Mode2Bank { // Decomposed GRETINA Data
   int     type;
   int     xtal_id;
   int     num;
   float   tot_e;
   int     core_e[4];
   long    timestamp;
   long    trig_time;
   float   t0;
   float   cfd;
   float   chisq;
   float   norm_chisq;
   float   baseline;
   float   prestep;
   float   poststep;
   int     pad;
   IntPoint intpts[MAX_INTPTS];
};

