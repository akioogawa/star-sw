/***************************************************************************
 *
 * $Id: StParentGeantTrack.h,v 2.1 2021/01/11 20:25:37 ullrich Exp $
 *
 * Author: Akio Ogawaa, Aug 2018
 *
 * Description: StParentGeantTrack contains parent Geant Track Id and Energy
 *
 **************************************************************************/
#ifndef StParentGeantTrack_hh
#define StParentGeantTrack_hh

#include "StObject.h"

class StParentGeantTrack : public StObject {
public:
    StParentGeantTrack();
    StParentGeantTrack(unsigned int id, unsigned int primaryId, float e) {mId=id; mPrimaryId=primaryId; mEnergy=e;}
    ~StParentGeantTrack();

    void addEnergy(float e) {mEnergy += e;}
    UInt_t id() {return mId;}
    UInt_t primaryId() {return mPrimaryId;}
    Float_t energy() {return mEnergy;}
    
protected:
    UInt_t mId=0;
    UInt_t mPrimaryId=0;
    Float_t mEnergy=0.0;

    ClassDef(StParentGeantTrack,1)

};

#endif
