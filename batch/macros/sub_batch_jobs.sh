
#
# To run plotting jobs on CERN batch system
#

run_one()
{
    pexe="python $TestArea/PhysicsNtuple/PhysicsProject/batch/subCERN.py"

    dtag="$1"
    inpl="$2"
    outd="/afs/cern.ch/work/${USER:0:1}/$USER/jobs/hists-"`date +%Y-%m-%d`${osuf}/"plots-${dtag}"
    parg="-o out.root $3"

    usr_macro="$TestArea/PhysicsNtuple/PhysicsProject/macros/run2Plot.py"
    run_macro="$TestArea/PhysicsNtuple/PhysicsProject/batch/runCERN.py"
    
    $pexe $popt $4 --input=$inpl --arg="$parg" --output="$outd" --macro="$usr_macro" --run-macro="$run_macro"
}

run_siml()
{
    run_one "siml_v04" "run2/input_cern_tth_siml_v04.txt,run2/input_cern_tth_siml_v04_priority2.txt" "$1" 
}

run_data()
{
    run_one "data_v04" "run2/input_cern_tth_data_v04_topq1.txt" "$1"
}

(
    popt="--setup-script=setup_atlas_release_cvmfs.sh --mb-per-job=4000 --overwrite --copy-log --request-pool -n 0 $1"
    osuf="$3"

    run_siml "$2"
    run_data "$2"
)
