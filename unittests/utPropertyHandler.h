#ifndef UTPROPERTYHANDLER_H_
#define UTPROPERTYHANDLER_H_

#include "UnitTest.h"
#include "TString.h"
#include "PropertyHandler.h"
/*
 *
 */

namespace ExampleFramework{

   class testPHGood : public PropertyHandler {
   public:
      int i1; int i2; double d1; bool b1; TString t1;
      testPHGood(){
         DefineProperty("i1", &i1, 2);  
         DefineProperty("i2", &i2, 3);  
         DefineProperty("d1", &d1, 4.0);  
      }

   };

   class testPHFail : public PropertyHandler {
   public:
      int i1; int i2; double d1; bool b1; TString t1;
      testPHFail(){
         DefineProperty("i1", &i1, 2);  
         DefineProperty("i1", &i2, 3);  
         DefineProperty("d1", &d1, 4.0);  
      }

   };

   class utPropertyHandler : public UnitTesting::UnitTest
{
private:

public:
   utPropertyHandler() : UnitTest("PropertyHandler")  { };
   virtual ~utPropertyHandler() {};

   void run() {
      {
         testPHGood p;
         test_(p.IsConsistent());
         test_(p.i1==2);
         test_(p.i2==3);
         test_(fabs(p.d1-4.0)<1.e-4);
         p.SetProperty("i1",5);
         test_(p.i1==5);
         p.PrintProperties();
         p.SetProperty("d1",0.);
         test_(fabs(p.d1)<1.e-4);
         test_(fabs(p.GetProperty<double>("d1"))<1.e-4);
         test_(p.GetProperty<int>("i1")==5);
         test_(p.GetProperty<int>("i2")==3);
         test_(p.IsConsistent());
      }

      cout << "test if we get desired error exceptions"<<endl;
      bool caught1=false,caught2=false,caught3=false;
      testPHGood p1,p2;

      try {p1.SetProperty("xxx",5);}
      catch(...) {caught1=true;}
      test_(!p1.IsConsistent()) ;
      
      try {p2.SetProperty("d1",5);}
      catch(...) {caught2=true;} 
      test_(!p2.IsConsistent()) ;
      
      try { testPHFail p3;}
      catch(...) { caught3=true; }

      test_(caught1);
      test_(caught2);
      test_(caught3);
      cout << "error exception tests done"<<endl;

   }

};

}

#endif /* UTHISTOGRAMSERVICE_H_ */
