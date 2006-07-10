float functionAG_FO4(float pt, float eta, int id){
  if (abs(id) == 11){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.471186;
      if (fabs(pt) < 25) return 0.216346;
      if (fabs(pt) < 35) return 0.110357;
      if (fabs(pt) < 50) return 0.100917;
      return 0.189445;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.503226;
      if (fabs(pt) < 25) return 0.284047;
      if (fabs(pt) < 35) return 0.207921;
      if (fabs(pt) < 50) return 0.139842;
      return 0.299107;
    } 
    if (fabs(eta) < 4){ 
      if (fabs(pt) < 15) return 0.333333;
      if (fabs(pt) < 25) return 0.590909;
      if (fabs(pt) < 35) return 0.272727;
      if (fabs(pt) < 50) return 0.230769;
      return 0.0714286;
    } 
  } 
  if (abs(id) == 13){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.486971;
      if (fabs(pt) < 25) return 0.251042;
      if (fabs(pt) < 35) return 0.126904;
      if (fabs(pt) < 50) return 0.109608;
      return 0.0912201;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.522911;
      if (fabs(pt) < 25) return 0.325733;
      if (fabs(pt) < 35) return 0.184805;
      if (fabs(pt) < 50) return 0.138528;
      return 0.134752;
    } 
    if (fabs(eta) < 2.4){ 
      if (fabs(pt) < 15) return 0.648936;
      if (fabs(pt) < 25) return 0.38;
      if (fabs(pt) < 35) return 0.181818;
      if (fabs(pt) < 50) return 0.131148;
      return 0.0833333;
    } 
  } 
  return 0; 
} 
