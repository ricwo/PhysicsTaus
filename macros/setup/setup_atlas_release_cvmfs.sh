#==============================================================================================
MY_ATH_ANALYSIS_VERSION=2.3.25

#==============================================================================================
if [ "$1" = "" ]
then
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
    
    source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh
    source $AtlasSetup/scripts/asetup.sh AthAnalysisBase,${MY_ATH_ANALYSIS_VERSION},here
fi

#==============================================================================================
if [ "$1" = "first-compile" ]
then
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

    source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh
    source $AtlasSetup/scripts/asetup.sh AthAnalysisBase,${MY_ATH_ANALYSIS_VERSION},here    

    echo 'Will setup and compile PhysicsTaus packages for the first time'

    pushd `pwd`
    
    cd $TestArea/PhysicsNtuple/PhysicsTaus/cmt
    cmt bro cmt config
    cmt bro `source setup.sh`
    cmt bro make -j
    
    popd
fi

#==============================================================================================
if [ "$1" = "compile" ]
then
    echo 'Will re-compile PhysicsTaus packages'

    pushd `pwd`
    
    cd $TestArea/PhysicsNtuple/PhysicsTaus/cmt
    cmt bro rm -r ../x86_64-slc6-gcc48-opt
    cmt bro make -j
    
    popd
fi

#==============================================================================================
if [ "$1" = "clean" ]
then
    echo 'Will clean PhysicsTaus packages'

    pushd `pwd`
    
    cd $TestArea/PhysicsNtuple/PhysicsTaus/cmt
    cmt bro rm -r ../x86_64-slc6-gcc48-opt
    
    popd
fi

#==============================================================================================
if [ "$1" = "light-compile" ]
then
    echo 'Will re-compile PhysicsTaus packages without updating header files'

    pushd `pwd`
    
    cd $TestArea/PhysicsNtuple/PhysicsTaus/cmt
    cmt bro make -j
    
    popd
fi

#==============================================================================================
if [ "$1" = "update" ]
then
    echo 'Will SVN update PhysicsTaus packages'

    pushd `pwd`    

    cd $TestArea/PhysicsNtuple/PhysicsBase
    svn up    

    cd $TestArea/PhysicsNtuple/PhysicsData
    svn up   

    cd $TestArea/PhysicsNtuple/PhysicsTaus
    svn up  

    popd
fi

#==============================================================================================
if [ "$1" = "status" ]
then
    echo 'Will check SVN status for PhysicsTaus packages'

    pushd `pwd`    

    cd $TestArea/PhysicsNtuple/PhysicsBase
    svn st -u

    cd $TestArea/PhysicsNtuple/PhysicsData
    svn st -u

    cd $TestArea/PhysicsNtuple/PhysicsTaus
    svn st -u

    popd
fi

qmake()
{
   DID_I_PUSH="no"
   if [ -d "$TestArea/PhysicsNtuple/PhysicsTaus/cmt" ]
   then
       DID_I_PUSH="yes"
       pushd "$TestArea/PhysicsNtuple/PhysicsTaus/cmt"
   fi

   if [ -e "Makefile" ]
   then
       if [ "$1" = "bro" ]
       then
	   cmt broadcast make -j4 QUICK=1
       else
           make -j4 QUICK=1
       fi
   fi 

   if [ "$DID_I_PUSH" = "yes" ]
   then
       popd
   fi 
}

