#!/bin/sh

if [ "$1" = "1" ]; then
for DSID in {361420..361443}
do
nohup nice -19 python macros/procRecoEvent.py /pc2014-data2/vilius/data/data/v26/$DSID.root -o sherpa/$DSID.root &
sleep 1
done

for DSID in {363361..363363}
do
nohup nice -19 python macros/procRecoEvent.py /pc2014-data2/vilius/data/data/v26/$DSID.root -o sherpa/$DSID.root &
sleep 1
done

for DSID in {363102..363120}
do
nohup nice -19 python macros/procRecoEvent.py /pc2014-data2/vilius/data/data/v26/$DSID.root -o sherpa/$DSID.root &
sleep 1
done
fi


if [ "$1" = "2" ]; then
for DSID in {361300..361335}
do
nohup nice -19 python macros/procRecoEvent.py /pc2014-data2/vilius/data/data/v26/$DSID.root -o sherpa/$DSID.root &
sleep 1
done
fi

if [ "$1" = "3" ]; then
for DSID in {361336..361371}
do
nohup nice -19 python macros/procRecoEvent.py /pc2014-data2/vilius/data/data/v26/$DSID.root -o sherpa/$DSID.root &
sleep 1
done

fi

if [ "$1" = "4" ]; then
for DSID in {363436..363483}
do
nohup nice -19 python macros/procRecoEvent.py /pc2014-data2/vilius/data/data/v26/$DSID.root -o sherpa/$DSID.root &
sleep 1
done
fi

if [ "$1" = "5" ]; then
for DSID in {363331..363354}
do
nohup nice -19 python macros/procRecoEvent.py /pc2014-data2/vilius/data/data/v26/$DSID.root -o sherpa/$DSID.root &
sleep 1
done

fi

if [ "$1" = "t" ]; then
for DSID in {361420..361420}
do
nohup nice -19 python macros/procRecoEvent.py /pc2014-data2/vilius/data/data/v26/$DSID.root -o sherpa/$DSID.root &
done
fi