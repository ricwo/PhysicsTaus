# echo "cleanup PhysicsTaus PhysicsTaus-v1 in /pc2014-data2/ric/AnalysisTaus/PhysicsNtuple"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/atlas.cern.ch/repo/sw/software/AthAnalysisBase/x86_64-slc6-gcc48-opt/2.3.25/CMT/v1r25p20140131; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtPhysicsTaustempfile=`${CMTROOT}/${CMTBIN}/cmt.exe -quiet build temporary_name`
if test ! $? = 0 ; then cmtPhysicsTaustempfile=/tmp/cmt.$$; fi
${CMTROOT}/${CMTBIN}/cmt.exe cleanup -sh -pack=PhysicsTaus -version=PhysicsTaus-v1 -path=/pc2014-data2/ric/AnalysisTaus/PhysicsNtuple  $* >${cmtPhysicsTaustempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/${CMTBIN}/cmt.exe cleanup -sh -pack=PhysicsTaus -version=PhysicsTaus-v1 -path=/pc2014-data2/ric/AnalysisTaus/PhysicsNtuple  $* >${cmtPhysicsTaustempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtPhysicsTaustempfile}
  unset cmtPhysicsTaustempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtPhysicsTaustempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtPhysicsTaustempfile}
unset cmtPhysicsTaustempfile
return $cmtcleanupstatus

