#!/bin/bash

# 186Hf runs

COMMAND="grutinizer -qH "

INPUTS="e15130.val e15130.cal zcuts_run136.cuts ./lib/libMakee15130_v7.so -o/dev/null " 

#FILES="/global/data1y/gretina/e15130/Run0108/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0109/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0110/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0111/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0112/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0113/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0114/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0115/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0116/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0117/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0118/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0119/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0120/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0121/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0122/Global.dat "
#FILES=$FILES"/global/data1y/gretina/e15130/Run0123/Global.dat "


#FILES=$FILES"/mnt/Projects2/Experiments/e15130/events/Global/Run0132/Global.dat.gz "
#FILES=$FILES"/mnt/Projects2/Experiments/e15130/events/Global/Run0133/Global.dat.gz "
#FILES=$FILES"/mnt/Projects2/Experiments/e15130/events/Global/Run0134/Global.dat.gz "
#FILES=$FILES"/mnt/Projects2/Experiments/e15130/events/Global/Run0135/Global.dat.gz "
#FILES=$FILES"/mnt/Projects2/Experiments/e15130/events/Global/Run0136/Global.dat.gz "
#FILES=$FILES"/mnt/Projects2/Experiments/e15130/events/Global/Run0137/Global.dat.gz "
#FILES=$FILES"/mnt/Projects2/Experiments/e15130/events/Global/Run0138/Global.dat.gz "
#FILES=$FILES"/mnt/Projects2/Experiments/e15130/events/Global/Run0139/Global.dat.gz "


FILES=$FILES"/mnt/Projects2/Experiments/e15130/events/Global/Run0110/Global.dat.gz "



#parallel -j2 $COMMAND ::: $INPUTS ::: $FILES

#for i in $FILES
#do
#  grutinizer -qH $i ./lib/libMakee15130_v7.so e15130_latest.val e15130_latest.cal zcuts_run136.cuts zcuts_noReactions.cuts -o /dev/null
#done

for i in $FILES
do
  echo grutinizer -qH $i ./lib/libMakee15130_v7.so e15130_latest.val e15130_nocal.cal zcuts_noReactions.cuts -o /dev/null
  grutinizer -qH $i ./lib/libMakee15130_v7.so e15130_latest.val e15130_nocal.cal zcuts_noReactions.cuts -o /dev/null
done


#for i in $FILES
#do
#  grutinizer -qH $i ./lib/libMakee15130_v7.so e15130_latest.val e15130_latest.cal -o /dev/null
#done
