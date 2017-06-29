python $TestArea/PhysicsNtuple/PhysicsProject/macros/runMPhys.py /pc2012-data3/rustem/data/tth/v4_mc_data/361106.root -n 100000 -o hist_361106.root;
python $TestArea/PhysicsNtuple/PhysicsProject/macros/runMPhys.py /pc2012-data3/rustem/data/tth/v4_mc_data/361107.root -n 100000 -o hist_361107.root;
python $TestArea/PhysicsNtuple/PhysicsProject/macros/runMPhys.py /pc2012-data3/rustem/data/tth/v4_mc_data/361108.root -n 100000 -o hist_361108.root;
python $TestArea/PhysicsNtuple/PhysicsProject/macros/runMPhys.py /pc2012-data3/rustem/data/tth/v4_mc_data/410000.root -n 100000 -o hist_410000.root;

python $TestArea/PhysicsNTuple/PhysicsProject/macros/plotHists.py hist_361106.root --rkey=.*nocut -o plots_361106 -s
python $TestArea/PhysicsNTuple/PhysicsProject/macros/plotHists.py hist_361107.root --rkey=.*nocut -o plots_361107 -s
python $TestArea/PhysicsNTuple/PhysicsProject/macros/plotHists.py hist_361108.root --rkey=.*nocut -o plots_361108 -s
python $TestArea/PhysicsNTuple/PhysicsProject/macros/plotHists.py hist_410000.root --rkey=.*nocut -o plots_410000 -s

cp plots_361106/NTruthTaus ~/Manchester/MPhys/Week5/Thurs/361106/
cp plots_361107/NTruthTaus ~/Manchester/MPhys/Week5/Thurs/361107/
cp plots_361108/NTruthTaus ~/Manchester/MPhys/Week5/Thurs/361108/
cp plots_410000/NTruthTaus ~/Manchester/MPhys/Week5/Thurs/410000/
