#include <TH1.h>
#include <TDirectory.h>
#include <TFile.h>
#include <string>
#include <iterator>
#include <iostream>
#include <TMath.h>
#include <sstream>

void changeName(int DSIDi, int DSIDf){

    for (int i = DSIDi; i < DSIDf+1; i++){
        stringstream fileName; 

        fileName << "sherpa/" << i << ".root";
        const char* name = (fileName.str()).c_str();
        cout << "adding file: " << name << endl;
        TFile *file = new TFile(name,"UPDATE");

        if (file->IsZombie()) {
            cout << "Could not open file" << endl; 
            exit(-1);
        }

        else {
            stringstream directory;
            // change this to your TDirectory path
            directory << i << "/plotEventAlgs/SigOS";

            if (!gDirectory->GetDirectory(directory.str().c_str())) {
                cout << "Dir does not exist" << endl;

            }

            else {
                gDirectory->cd(directory.str().c_str());
                TDirectory *dummy = gDirectory;
                gDirectory->ReadAll();

                stringstream fileNameOut;
                fileNameOut << "sherpa/" << i << "_out.root";
                const char* nameOut = (fileNameOut.str()).c_str();
                cout << "output file: " << nameOut << endl;
                TFile *fileOut = new TFile(nameOut,"RECREATE");
                //fileOut->mkdir("muon");
                //fileOut->cd("muon");
                dummy->GetList()->Write();
            }

        }

    }

}
