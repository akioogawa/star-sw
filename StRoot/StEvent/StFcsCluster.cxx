/*****************************************************************************
 *
 * $Id: StFcsCluster.cxx,v 2.1 2021/01/11 20:25:37 ullrich Exp $
 *
 * Author: Akio Ogawa 2018
 * ***************************************************************************
 *
 * Description: Implementation of StFcsCluster, a group of adjacent FCS towers
 *
 * ***************************************************************************
 *
 * $Log: StFcsCluster.cxx,v $
 * Revision 2.1  2021/01/11 20:25:37  ullrich
 * Initial Revision
 *
 *****************************************************************************/
#include "StFcsCluster.h"

#include "StMessMgr.h"
#include "StFcsHit.h"
#include "StFcsPoint.h"
#include "StParentGeantTrack.h"

static const char rcsid[] = "$Id: StFcsCluster.cxx,v 2.1 2021/01/11 20:25:37 ullrich Exp $";

StFcsCluster::StFcsCluster(): StObject(), mFourMomentum(0.,0.,0.,0.) { /* no op */ }

StFcsCluster::~StFcsCluster() { /* no op */ }

void StFcsCluster::addNeighbor(StFcsCluster* neighbor) {
    int n=nNeighbor();
    for(int i=0; i<n; i++) if(mNeighbor[i]==neighbor) return; //already there, do nothing
    mNeighbor.push_back(neighbor);
}

void StFcsCluster::addPoint(StFcsPoint* p) {
    mPoints.push_back(p);
}

void StFcsCluster::addPoint(StFcsPoint* p1, StFcsPoint* p2) {
    mPoints.push_back(p1);
    mPoints.push_back(p2);
}


void StFcsCluster::geantTrack(StSPtrVecParentGeantTrack& mGeantTrack, float& mDETot){
    mDETot=0.0;
    int nhit = mHits.size();	
    for(int ihit=0; ihit<nhit; ihit++){
	StSPtrVecParentGeantTrack& trk= mHits[ihit]->geantTrack();
	int ntrk = trk.size();
	for(int itrk=0; itrk<ntrk; itrk++){				
	    unsigned int id=trk[itrk]->id();
	    float e=trk[itrk]->energy();
	    mDETot += e;
	    int n = mGeantTrack.size();
	    int found=0;
	    for(int jtrk=0; jtrk<n; jtrk++){
		if(mGeantTrack[jtrk]->id() == id) {mGeantTrack[jtrk]->addEnergy(e); found=1; break;}
	    }		
	    if(found==0) {mGeantTrack.push_back(new StParentGeantTrack(id,trk[itrk]->primaryId(),e));}
	}
    }
    std::sort(mGeantTrack.begin(), mGeantTrack.end(), [](StParentGeantTrack* a, StParentGeantTrack* b){
	    return b->energy() < a->energy();
	});   
}

void StFcsCluster::primaryGeantTrack(StSPtrVecParentGeantTrack& mGeantTrack, float& mDETot){
    mDETot=0.0;
    int nhit = mHits.size();	
    for(int ihit=0; ihit<nhit; ihit++){
	StSPtrVecParentGeantTrack& trk= mHits[ihit]->geantTrack();
	int ntrk = trk.size();
	for(int itrk=0; itrk<ntrk; itrk++){				
	    unsigned int id=trk[itrk]->primaryId();
	    float e=trk[itrk]->energy();
	    mDETot += e;
	    int n = mGeantTrack.size();
	    int found=0;
	    for(int jtrk=0; jtrk<n; jtrk++){
		if(mGeantTrack[jtrk]->primaryId() == id) {mGeantTrack[jtrk]->addEnergy(e); found=1; break;}
	    }		
	    if(found==0) {mGeantTrack.push_back(new StParentGeantTrack(0,id,e));}
	}
    }
    std::sort(mGeantTrack.begin(), mGeantTrack.end(), [](StParentGeantTrack* a, StParentGeantTrack* b){
	    return b->energy() < a->energy();
	});   
}

void StFcsCluster::print(Option_t *option) const {
    cout << Form(
      "StFcsCluster id=%4d ctg=%1d n=%2d nNeigh=%1d nPoints=%1d loc=%7.2f %7.2f PXYZE=%7.2lf %7.2lf %7.2lf %7.2lf E=%7.2lf sigMin/max=%7.2f %7.2f Chi2=%7.2f %7.2f",
      id(), category(), nTowers(), nNeighbor(),nPoints(),
      x(), y(), 
      fourMomentum().px(),fourMomentum().py(),fourMomentum().pz(),fourMomentum().e(),
      energy(), sigmaMin(), sigmaMax(), chi2Ndf1Photon(), chi2Ndf2Photon()) << endl;
    /*
    int n=mGeantTrack.size();
    if(n>0){
	cout << Form("  NGeantTrack=%3d  dETot=%f",n,mDETot) << endl;
	for(int i=0; i<n; i++){
	    int id=mGeantTrack[i]->id();
	    float e=mGeantTrack[i]->energy();
	    cout << Form("  GeantTrackId=%4d E=%8.3f Fraction=%6.4f",id,e,e/mDETot)<<endl;
	}
    }
    */
}

