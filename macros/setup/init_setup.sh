
#==============================================================================================
# Setup release
#
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh
source $AtlasSetup/scripts/asetup.sh AthAnalysisBase,2.3.25,here

#==============================================================================================
# Check out and compile code
#
svn co --username "$1" svn+ssh://svn.cern.ch/reps/manc/PhysicsNtuple/PhysicsBase/trunk PhysicsNtuple/PhysicsBase
svn co --username "$1" svn+ssh://svn.cern.ch/reps/manc/PhysicsNtuple/PhysicsData/trunk PhysicsNtuple/PhysicsData

pushd $PWD 
cd $TestArea/PhysicsNtuple/PhysicsBase/cmt; cmt config
popd

pushd $PWD 
cd $TestArea/PhysicsNtuple/PhysicsData/cmt; cmt config
popd

pushd $PWD 
cd $TestArea/PhysicsNtuple/PhysicsTaus/cmt
cmt bro cmt config
cmt bro cmt make -j
popd

#==============================================================================================
# Add soft link
#
ln -s PhysicsNtuple/PhysicsTaus/macros/setup/setup_atlas_release_cvmfs.sh .

echo 'Checked out and compiled these packages:'
echo '   PhysicsNtuple/PhysicsBase'
echo '   PhysicsNtuple/PhysicsData'
echo '   PhysicsNtuple/PhysicsTaus'
echo 
echo 'All is done'
