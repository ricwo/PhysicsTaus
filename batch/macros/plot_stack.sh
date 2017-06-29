(
    dpath="$1"
    opath="${dpath}/plots-siml_v04/job*.root --data-file=${dpath}/plots-data_v04/add.root"
    opt="--xsec-list=data/xsec_list_run2.txt --get-regions"
    python macros/plotRun2Cand.py $opath $opt -o plots -s -r $2
)
