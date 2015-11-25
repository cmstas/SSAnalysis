{

  gSystem->Load("../CORE/CMS3_CORE.so"); 
  gROOT->ProcessLine(".L babymaker.C+");
  gROOT->ProcessLine("babymaker()"); 

}
