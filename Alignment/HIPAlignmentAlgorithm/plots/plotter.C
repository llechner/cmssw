#include <Riostream.h>
#include "TROOT.h"
#include "TSystem.h"
#include "HIPplots.h"
	
using namespace std;

// det: 0=all,1=PXB, 2=PXF, 3=TIB, 4=TID, 5=TOB, 6=TEC
// plotType: cov, shift, chi2, param, hitmap
// only_plotting: turn on/off generating root file with histograms 

void plotter(const char* base_path, const char* dir, int iov=262922, TString plotType = "cov", int param_iter=20, int det=0, bool only_plotting=false){

  char path[512], tag[256], outpath[512];

  TString detname[6]={ "PXB", "PXF", "TIB", "TID", "TOB", "TEC" };
  if (det>=1&&det<=6)
    sprintf(tag, "%s_%s", dir, detname[det-1].Data());
  else if (det==0)
    sprintf(tag, "%s_ALL", dir);
  else{
    cout << "sub-detector id undefined! Please choose between 1 to 6!" << endl;
    return;
  }

  sprintf(path, "%s/%s/main", base_path, dir);
  sprintf(outpath, "./%s/HIP_%s_%s_IOV%i_Iter%i.root", dir, plotType.Data(), tag, iov, param_iter);
  cout << "Path is " << path << endl;
  gSystem->Exec("mkdir -p " + TString(dir));

  HIPplots* c=new HIPplots(iov, path, outpath);
  int ierr=0;
  c->CheckFiles(ierr);
  if (ierr>0){
    cout << " ERROR !!!! Missing " << ierr << " files. Aborting." << endl;
    return;
  }
  else if (ierr<0){
    if (!only_plotting){
      cout << "ERROR !!!! Output file exists already. I don't want to overwrite." << endl;
      cout << "Please, change name of output file or delete the old one: " << outpath << endl;
      //cout << "Aborting." << endl;
      //return;
    }
  }


  cout << "\n\nSTARTING TO Generate Root Files " << endl << endl;

  if (!only_plotting && plotType =="cov"){
    //========= Produce histograms ============
    for (int i = 0; i < 6; i++){
      if (det==0) c->extractAlignParams(i, 20);
      else c->extractAlignParams(i, 20, det);
    }
  }

  if (!only_plotting && (plotType =="shift"||plotType =="param")){
    for (int i = 0; i < 6; i++){
      if (det==0) c->extractAlignShifts(i, 20);
      else c->extractAlignShifts(i, 20, det);
    }
  }

  if (!only_plotting && plotType =="chi2"){
    if (det==0) c->extractAlignableChiSquare(20);
    else c->extractAlignableChiSquare(20, det);
  }

  cout << "\n\nSTARTING TO PLOT " << endl << endl;


  gROOT->ProcessLine(" .L tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle()");

  if (plotType == "hitmap"){
    sprintf(outpath, "./%s/HitMaps_%s_IOV%i/", dir, tag, iov);
    gSystem->Exec("mkdir -p " + TString(outpath));
    for (int i=1; i<=6; ++i){//loop over subdets
      c->plotHitMap(outpath, i, 0);
    }
  }
  else if (plotType == "cov"){
    sprintf(outpath, "./%s/parsViters_%s_IOV%i.png", dir, tag, iov);
    c->plotAlignParams("PARAMS", outpath);
  }
  else if (plotType == "shift"){
    sprintf(outpath, "./%s/shiftsViters_%s_IOV%i.png", dir, tag, iov);
    c->plotAlignParams("SHIFTS", outpath);
  }
  else if (plotType == "param"){
    sprintf(outpath, "./%s/shiftsAtIter%d_%s_IOV%i.png", dir, param_iter, tag, iov);
    c->plotAlignParamsAtIter(param_iter, "SHIFTS", outpath);
  }
  else if (plotType == "chi2"){
    sprintf(outpath, "./%s/AlignableChi2n_%s_IOV%i", dir, tag, iov);//do not put the file extension here!!!!
    c->plotAlignableChiSquare(outpath, 0.1);
  }
  else
    cout << "Plotting type \"" << plotType << "\" is not defined!" << endl;

  cout << "Deleting... " << flush;
  // delete c;
  cout << "cleaned up HIPplots object ! We are done  and good luck !" << endl;

}
