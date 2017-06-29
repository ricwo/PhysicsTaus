 pushd `pwd`    
 
 cd $TestArea/PhysicsNtuple/PhysicsBase
 svn up    
 
 cd $TestArea/PhysicsNtuple/PhysicsData
 svn up   
 
 cd $TestArea/PhysicsNtuple/PhysicsTaus
 svn up  
 
 cd $TestArea/PhysicsNtuple/PhysicsTaus/cmt
 cmt bro rm -r ../x86_64-slc6-gcc48-opt
 cmt bro make -j

 popd

