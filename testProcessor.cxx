#include "Framework.h"
#include "TrackerRawHitProcessor.h"

using namespace ExampleFramework;

int main(int argc, char **argv){
   Framework* theFramework = new Framework("data/w_and_jets.dat", "processorTest.dat");
   theFramework->AddProcessor(new TrackerRawHitProcessor(theFramework));
   theFramework->SuppressWriteOut();
   theFramework->ProcessAll();
}
