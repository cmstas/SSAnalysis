#include "fits.h"

float electron_prob(float x){ 
  return 0.597626*erf((x-10)/38.4902) + 0.135192*(1-erf((x-10)/38.4902));
}
 
float muon_prob(float x){ 
  return 0.616886*erf((x-10)/30.2167) + 0.292419*(1-erf((x-10)/30.2167));
}


//float electron_prob(float x){ 
//  return 0.640052*erf((x-10)/36.9437) + 0.170425*(1-erf((x-10)/36.9437));
//}
// 
//float muon_prob(float x){ 
//  return 0.673556*erf((x-10)/29.6532) + 0.332195*(1-erf((x-10)/29.6532));
//}
 
float btag_prob_part(float x){ 
  float fitval = 0;
  if (x > 120) fitval = 0.669023 + -8.73419e-05*x;
  if (x <= 120) fitval = 8.63317e-07 *x*x*x +  -0.000237359*x*x + 0.0217418*x + -0.0194392;
  return fitval;
}
 
float btag_prob_jets(float x){ 
  float fitval = 0;
  if (x > 120) fitval = 0.768051 + -0.000326801*x;
  if (x <= 120) fitval = 1.55199e-06 *x*x*x +  -0.000426303*x*x + 0.0390608*x + -0.496123;
  return fitval;
}
 
float met_prob_30(float x){
return .5*0.999889*(1 + erf((x-13.8682)/42.9167));
}
 
float met_prob_50(float x){
return .5*0.999733*(1 + erf((x-42.9681)/37.4652));
}
 
float met_prob_120(float x){
return .5*0.999371*(1 + erf((x-117.85)/36.9028));
}
 
float ht_prob_part_200(float x){
return .5*0.992568*(1 + erf((x-168.574)/148.131));
}
 
float ht_prob_part_400(float x){
return .5*0.929056*(1 + erf((x-399.971)/173.374));
}
 
float ht_prob_jets_200(float x){
return .5*0.999832*(1 + erf((x-185.21)/44.4486));
}
 
float ht_prob_jets_400(float x){
return .5*0.999799*(1 + erf((x-378.691)/59.4101));
}
 
float jet_prob_part(float x){
return .5*0.98608*(1 + erf((x-28.2891)/38.0267));
}
 
float jet_prob_jets(float x){
return .5*0.999653*(1 + erf((x-29.8098)/18.7481));
}
 
