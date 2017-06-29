# echo "setup PhysicsTaus PhysicsTaus-v1 in /pc2014-data2/ric/AnalysisTaus/PhysicsNtuple"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/atlas.cern.ch/repo/sw/software/AthAnalysisBase/x86_64-slc6-gcc48-opt/2.3.25/CMT/v1r25p20140131
endif
source ${CMTROOT}/mgr/setup.csh
set cmtPhysicsTaustempfile=`${CMTROOT}/${CMTBIN}/cmt.exe -quiet build temporary_name`
if $status != 0 then
  set cmtPhysicsTaustempfile=/tmp/cmt.$$
endif
${CMTROOT}/${CMTBIN}/cmt.exe setup -csh -pack=PhysicsTaus -version=PhysicsTaus-v1 -path=/pc2014-data2/ric/AnalysisTaus/PhysicsNtuple  -no_cleanup $* >${cmtPhysicsTaustempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/${CMTBIN}/cmt.exe setup -csh -pack=PhysicsTaus -version=PhysicsTaus-v1 -path=/pc2014-data2/ric/AnalysisTaus/PhysicsNtuple  -no_cleanup $* >${cmtPhysicsTaustempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtPhysicsTaustempfile}
  unset cmtPhysicsTaustempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtPhysicsTaustempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtPhysicsTaustempfile}
unset cmtPhysicsTaustempfile
exit $cmtsetupstatus

