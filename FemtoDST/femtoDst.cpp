// C++ headers
#include <iostream>
#include <cmath>

// ROOT headers
#include "TCut.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;
  int femtoDst()
{
    TFile *oldfile = new TFile("/home/manukhov/DUBNA/STAR/st_physics_19130071_raw_2500009.picoDst.root");
    TTree *oldtree = (TTree*)oldfile->Get("PicoDst");
    oldtree->SetBranchStatus("*",0);
    oldtree->SetBranchStatus("Event*",1);
    oldtree->SetBranchStatus("Track*",1);
    oldtree->SetBranchStatus("BTowHit*",1);
    oldtree->SetBranchStatus("BTofHit*",1);
    oldtree->SetBranchStatus("BTofPidTraits*",1);

    TCut rv = "sqrt(pow((Event.mPrimaryVertexX),2) + pow((Event.mPrimaryVertexY),2)) <0.35";
    TCut c1 = "abs(Event.mPrimaryVertexZ)<35.";
    TCut c2 = "Track.mNHitsFit >10.";
    TCut c3 = "Track.mNHitsDedx>10 ";
    TCut c4 = "((Track.mNHitsFit)/(Track.mNHitsMax))  >0.18";
    TCut c5 = "sqrt(pow((Track.mPMomentumX),2) + pow((Track.mPMomentumY),2)) > 0.1";
    TFile newfile ("femto.picoDst.root","RECREATE");
    TTree *newtree;
    newtree = oldtree->CopyTree(rv && c1 && c2 && c3 && c4 && c5);
    newtree->Print();
    newfile.Write();
    newfile.Close();
    return 0;
  }






/*
    TCut rv = "sqrt(pow((Event.mPrimaryVertexX),2) + pow((Event.mPrimaryVertexY),2)) <0.35";
    TCut c1 = "abs(Event.mPrimaryVertexZ)<35.";
    TCut c2 = "Track.mNHitsFit >10.";
    TCut c3 = "Track.mNHitsDedx>10 ";
    TCut c4 = "((Track.mNHitsFit)/(Track.mNHitsMax))  >0.18";
 //   TCut c5 = "Track.Pt()>0.1";
 //   TCut c6 = "Event->primaryVertex().Mag()<1.";*/

  /*  oldtree->SetBranchStatus("Event.mBunchCrossId",0);
    oldtree->SetBranchStatus("Event.mJetPatchThreshold[4]",0);
    oldtree->SetBranchStatus("Event.mHighTowerThreshold[4]",0);
    oldtree->SetBranchStatus("Event.mBbcAdcWest[24]",0);
    oldtree->SetBranchStatus("Event.mBbcAdcEast[24]",0);
    oldtree->SetBranchStatus("Event.mZdcSmdWestVertical[8]",0);
    oldtree->SetBranchStatus("Event.mZdcSmdWestHorizontal[8]",0);
    oldtree->SetBranchStatus("Event.mZdcSmdEastVertical[8]",0);
    oldtree->SetBranchStatus("Event.mZdcSmdEastHorizontal[8]",0);
    oldtree->SetBranchStatus("Event.mZdcSumAdcWest",0);
    oldtree->SetBranchStatus("Event.mZdcSumAdcEast",0);
    oldtree->SetBranchStatus("Event.mZdcWestRate",0);
    oldtree->SetBranchStatus("Event.mZdcEastRate",0);
    oldtree->SetBranchStatus("Event.mBbcWestRate",0);
    oldtree->SetBranchStatus("Event.mBbcEastRate",0);
    oldtree->SetBranchStatus("Event.mBbcYellowBackgroundRate",0);
    oldtree->SetBranchStatus("Event.mBbcBlueBackgroundRate",0);
    oldtree->SetBranchStatus("Event.mBackgroundRate",0);
    oldtree->SetBranchStatus("Event.mVzVpd",0);
    oldtree->SetBranchStatus("Event.mNTofT0",0);
    oldtree->SetBranchStatus("Event.mNVpdHitsWest",0);
    oldtree->SetBranchStatus("Event.mNVpdHitsEast",0);
    oldtree->SetBranchStatus("Event.mNHitsHFT[4]",0);
    oldtree->SetBranchStatus("Event.mbTofTrayMultiplicity",0);
    oldtree->SetBranchStatus("Event.mRefMultHalfPosWest",0);
    oldtree->SetBranchStatus("Event.mRefMultHalfNegWest",0);
    oldtree->SetBranchStatus("Event.mRefMultHalfPosEast",0);
    oldtree->SetBranchStatus("Event.mRefMultHalfNegEast",0);
    oldtree->SetBranchStatus("Event.mRefMult4PosWest",0);
    oldtree->SetBranchStatus("Event.mRefMult4NegWest",0);
    oldtree->SetBranchStatus("Event.mRefMult4PosEast",0);
    oldtree->SetBranchStatus("Event.mRefMult4NegEast",0);
    oldtree->SetBranchStatus("Event.mRefMult3PosWest",0);
    oldtree->SetBranchStatus("Event.mRefMult3NegWest",0);
    oldtree->SetBranchStatus("Event.mRefMult3PosEast",0);
    oldtree->SetBranchStatus("Event.mRefMult3NegEast",0);
    oldtree->SetBranchStatus("Event.mRefMult2PosWest",0);
    oldtree->SetBranchStatus("Event.mRefMult2NegWest",0);
    oldtree->SetBranchStatus("Event.mRefMult2PosEast",0);
    oldtree->SetBranchStatus("Event.mRefMult2NegEast",0);
    oldtree->SetBranchStatus("Event.mRefMultFtcpWest",0);
    oldtree->SetBranchStatus("Event.mRefMultFtcpEast",0);
    oldtree->SetBranchStatus("Event.mTriggerIds",0);
    oldtree->SetBranchStatus("Event.mNBTOFMatch",0);
    oldtree->SetBranchStatus("Event.mNBEMCMatch",0);*/

/*  newfile.cd();
  newfile.SaveSelf();
  TTree *newtree;
  newtree= oldtree->CopyTree(); //rv && c1 && c2 && c3 && c4);
  cout<<"DDD"<<endl;
  //newtree = oldtree ->CloneTree();
  gDirectory->Write("", TObject::kOverwrite);
  newtree->Write("", TObject::kOverwrite);
  newfile.Write("", TObject::kOverwrite);
            newfile.Close();
            oldfile->Close();*/



