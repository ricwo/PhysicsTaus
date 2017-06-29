
hostname
echo "Current directory: $PWD"
ls -l
ls -l input/

df -h
du -h --max-depth=1

echo 'Will setup ATLAS release'

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

ls -l ${ATLAS_LOCAL_ROOT_BASE}
ls -l ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

echo 'Will attempt setupATLAS'

setupATLAS
asetup AthAnalysisBase,2.3.25,here

echo 'Will compile PhysicsLight packages'

pushd `pwd`

cd $TestArea/PhysicsNtuple/PhysicsLight/cmt
cmt bro rm -r ../x86_64-slc6-gcc48-opt
cmt bro cmt config
cmt bro `source setup.sh`
cmt bro make -j

popd
