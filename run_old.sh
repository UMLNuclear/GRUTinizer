#!/bin/bash

for i in $(seq -f "%04g" 199 218 )
do
    grutinizer -qH ../data/Run$i/Global.dat.gz ./lib/libMakee15130_v5.so e15130_run163torun197.val e15130_brun231.cal zcuts_189Hf_Ni.cuts zcuts_noReactions.cuts -o /dev/null
done
