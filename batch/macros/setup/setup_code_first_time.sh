
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh
source $AtlasSetup/scripts/asetup.sh AthAnalysisBase,2.3.25,here    

echo 'Will checkout from SVN PhysicsProject packages'

svn co svn+ssh://svn.cern.ch/reps/manc/PhysicsNtuple/PhysicsBase/trunk    PhysicsNtuple/PhysicsBase
svn co svn+ssh://svn.cern.ch/reps/manc/PhysicsNtuple/PhysicsData/trunk    PhysicsNtuple/PhysicsData
svn co svn+ssh://svn.cern.ch/reps/manc/PhysicsNtuple/PhysicsProject/trunk PhysicsNtuple/PhysicsProject

echo 'Will setup and compile PhysicsProject packages for the first time'

pushd `pwd`

cd $TestArea/PhysicsNtuple/PhysicsProject/cmt
cmt bro cmt config
cmt bro `source setup.sh`
cmt bro make -j

popd

echo "All done"
