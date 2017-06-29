export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
export DQ2_LOCAL_SITE_ID=ROAMING

alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'

setupATLAS
localSetupDQ2Client --skipConfirm
voms-proxy-init --voms atlas 