#==============================================================================================
if [ "$1" == "" ]
then
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
    
    source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh
    source $AtlasSetup/scripts/asetup.sh AthAnalysisBase,2.3.25,here
fi

#==============================================================================================
if [ "$1" == "first-compile" ]
then
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

    source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh
    source $AtlasSetup/scripts/asetup.sh AthAnalysisBase,2.3.25,here    

    echo 'Will checkout from SVN PhysicsProject packages'

    svn co svn+ssh://urobin@svn.cern.ch/reps/manc/PhysicsNtuple/PhysicsBase/trunk    PhysicsBase
    svn co svn+ssh://urobin@svn.cern.ch/reps/manc/PhysicsNtuple/PhysicsData/trunk    PhysicsData
    svn co svn+ssh://urobin@svn.cern.ch/reps/manc/PhysicsNtuple/PhysicsProject/trunk PhysicsProject

    echo 'Will setup and compile PhysicsProject packages for the first time'

    pushd `pwd`
    
    cd $TestArea/PhysicsNtuple/PhysicsProject/cmt
    cmt bro cmt config
    cmt bro `source setup.sh`
    cmt bro make -j2
    
    popd
fi

#==============================================================================================
if [ "$1" == "compile" ]
then
    echo 'Will re-compile PhysicsProject packages'

    pushd `pwd`
    
    cd $TestArea/PhysicsNtuple/PhysicsProject/cmt
    cmt bro rm -r ../x86_64-slc6-gcc48-opt
    cmt bro make -j2
    
    popd
fi

#==============================================================================================
if [ "$1" == "clean" ]
then
    echo 'Will clean PhysicsProject packages'

    pushd `pwd`
    
    cd $TestArea/PhysicsNtuple/PhysicsProject/cmt
    cmt bro rm -r ../x86_64-slc6-gcc48-opt
    
    popd
fi

#==============================================================================================
if [ "$1" == "light-compile" ]
then
    echo 'Will re-compile PhysicsProject packages without updating header files'

    pushd `pwd`
    
    cd $TestArea/PhysicsNtuple/PhysicsProject/cmt
    cmt bro make -j2
    
    popd
fi

#==============================================================================================
if [ "$1" == "update" ]
then
    echo 'Will SVN update PhysicsProject packages'

    pushd `pwd`    

    cd $TestArea/PhysicsNtuple/PhysicsBase
    svn up    

    cd $TestArea/PhysicsNtuple/PhysicsData
    svn up   

    cd $TestArea/PhysicsNtuple/PhysicsProject
    svn up  

    popd
fi

#==============================================================================================
if [ "$1" == "status" ]
then
    echo 'Will SVN update PhysicsProject packages'

    pushd `pwd`    

    cd $TestArea/PhysicsNtuple/PhysicsBase
    svn st -u

    cd $TestArea/PhysicsNtuple/PhysicsData
    svn st -u

    cd $TestArea/PhysicsNtuple/PhysicsProject
    svn st -u

    popd
fi
