#ifndef DIClusteredTrackerHit_H
#define DIClusteredTrackerHit_H

#include "DataItem.h"
#include "DITrackerHit.h"
#include "TString.h"

namespace ExampleFramework {

/*! 
  A track passing through a trackerlayer might hit several neighboring cells. This class represents this cluster of cell. The position is the center-of-gravity of the cells.
 */
class DIClusteredTrackerHit : public DITrackerHit
{
 private:
   // data 
   vector <const DataItem* > fTrackerHits; //!< array containing all hits belonging to the cluster.

   ~DIClusteredTrackerHit(){ } //!< private destructor
   
 public:
   // empty constructor
   DIClusteredTrackerHit() :  DITrackerHit(), fTrackerHits (0) { } 
 
   //! main constructor
   DIClusteredTrackerHit(int uniqueid)
      : DITrackerHit(uniqueid), fTrackerHits (0) { }
   
   //! dysfunctional copy constructor
   DIClusteredTrackerHit( const DIClusteredTrackerHit& di) { ReportError("no copy constructor defined"); }
   //! dysfunctional assignment operator
   DIClusteredTrackerHit& operator = ( const DIClusteredTrackerHit& di){ ReportError("no assignment operator defined");}
   
   // Getter Functions
   double GetPulseHeight(){return fPulseHeight;}
   
   //! main function to add a hit to the cluster
   void Add(const DITrackerHit* hit); 
   void Add(const DataItem* hit){Add(dynamic_cast <const DITrackerHit*>(hit));}
   int GetSize() const {return fTrackerHits.size();} //!< number of hits in cluster
   MyDef(DIClusteredTrackerHit) //!< implement GetName function 
protected:
   void Read();
   void Write() const;
};

}
#endif
