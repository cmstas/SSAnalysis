float flipRate(float pt, float eta) {
   if (pt>=15 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 3.52922e-05;
   if (pt>=15 && pt<40 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00017848;
   if (pt>=15 && pt<40 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00136623;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 2.16482e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00025168;
   if (pt>=40 && pt<60 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00187796;
   if (pt>=60 && pt<80 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 2.88849e-05;
   if (pt>=60 && pt<80 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00048463;
   if (pt>=60 && pt<80 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00303315;
   if (pt>=80 && pt<100 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 6.15569e-05;
   if (pt>=80 && pt<100 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000611966;
   if (pt>=80 && pt<100 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00419241;
   if (pt>=100 && pt<200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 5.97279e-05;
   if (pt>=100 && pt<200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000817347;
   if (pt>=100 && pt<200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00443864;
   if (pt>=200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000830532;
   if (pt>=200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00632193;
   return 0.;
}
float flipRateError(float pt, float eta) {
   if (pt>=15 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 5.06201e-06;
   if (pt>=15 && pt<40 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 1.4378e-05;
   if (pt>=15 && pt<40 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 4.35732e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 4.11215e-06;
   if (pt>=40 && pt<60 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 1.71322e-05;
   if (pt>=40 && pt<60 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 5.67547e-05;
   if (pt>=60 && pt<80 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 1.30344e-05;
   if (pt>=60 && pt<80 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 7.08868e-05;
   if (pt>=60 && pt<80 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.000218568;
   if (pt>=80 && pt<100 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 3.66782e-05;
   if (pt>=80 && pt<100 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000144729;
   if (pt>=80 && pt<100 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.000581726;
   if (pt>=100 && pt<200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 3.26881e-05;
   if (pt>=100 && pt<200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000185914;
   if (pt>=100 && pt<200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.000572306;
   if (pt>=200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000605259;
   if (pt>=200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00270871;
   return 0.;
}

