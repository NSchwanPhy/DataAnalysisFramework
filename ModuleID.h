#ifndef MODULEID_H
#define MODULEID_H

#include "TRandom3.h"
#include <vector>
using std::vector;
#include "HistogramService.h"

namespace ExampleFramework{
/*! This is the class that knows the geometry of the detector and the location of modules and their ID. 
Translation of spatial positions into a module id, returning -1 if no module found, also filling expected distance from nedxt module
length in mm, phi in radians, polar angle in pseudorapidity eta 
(c) E. von Toerne, U. of Bonn 2009
ToDo let ModuleID inherit from Service
 */
class ModuleID {
protected:
   vector <double> fR; 
   vector <double> fBarrelEta;
   vector <double> fMaxEta;
   vector <double> fZ; // calculated from radius and barrel eta
   vector <double> fDPhi;
   vector <double> fDEta;
   static const int    fMaxPhiBin=400; 
   static const int    fMaxEtaBin=400;
   static const int    fLayerOffset    =1000000;
   static const int    fBarrelOffset   =100000;
   static const int    fEndcap1Offset  =300000;
   static const int    fEndcap2Offset  =500000;
   bool fDebug;
   HistogramService* fHisto;

public:
   static const int    fNTrackerLayers = 5;
   static const int    fNECALLayers = 3; // included in calolayer count
   static const int    fNCaloLayers = 8;
   static const int    fNLayers = fNTrackerLayers+fNCaloLayers;

   ModuleID();
   ~ModuleID(){ }
   void SetHistogramService(HistogramService* hist);

   /*! returns true if this is a valid ID.*/
   bool ValidID(int id);
   /*! returns ID of module at position. Must match almost exactly, position must be on cylinder surface.*/
   int GetIDFromPosition(double x, double y, double z, int lay);   
   /*! returns by reference layer and position of module and by reference if this is a Calo layer. Input is the module ID. Return value is if this is a vaqlid ID.*/
   bool GetPositionFromID(int id, int& layer, double& x, double& y, double& z, bool& isCalo);

   // Getters
   int GetNLayers(){ return fNLayers;}
   int GetNTrackerLayers(){ return fNTrackerLayers;}
   int GetNECALLayers(){ return fNECALLayers;}
   int GetMaxID(){ return fNLayers*fLayerOffset;}
   bool GetLayerIsCalo(int layer){ return (layer>=fNTrackerLayers);}
   bool IsCaloID(int id){ return id >= (  fLayerOffset * GetNTrackerLayers());}
   bool IsECALID(int id){ return IsCaloID(id) && (id <= fLayerOffset * (GetNTrackerLayers()+GetNECALLayers()));}

protected:
   //! converter eta --> bin
   int GetEtaBin(double eta, double mineta, double delta);
   //! converter phi --> bin
   int GetPhiBin(double phi, double delta);
   //! converter bin --> eta 
   double GetEtaFromBin(int bin, double mineta, double delta);
   //! converter bin --> phi
   double GetPhiFromBin(int bin, double delta);
   void ReportError(TString name);
};

} // namespace

#endif // MODULEID_H
