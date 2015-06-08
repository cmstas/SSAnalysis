float functionAG_FO4(float pt, float eta, int id){
  if (abs(id) == 11){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.442767;
      if (fabs(pt) < 25) return 0.20985;
      if (fabs(pt) < 35) return 0.109183;
      if (fabs(pt) < 50) return 0.108513;
      return 0.211361;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.4823;
      if (fabs(pt) < 25) return 0.273586;
      if (fabs(pt) < 35) return 0.204869;
      if (fabs(pt) < 50) return 0.125378;
      return 0.279747;
    } 
    if (fabs(eta) < 4){ 
      if (fabs(pt) < 15) return 0.5;
      if (fabs(pt) < 25) return 0.565218;
      if (fabs(pt) < 35) return 0.222222;
      if (fabs(pt) < 50) return 0.197183;
      return 0.22449;
    } 
  } 
  if (abs(id) == 13){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.501349;
      if (fabs(pt) < 25) return 0.247875;
      if (fabs(pt) < 35) return 0.131749;
      if (fabs(pt) < 50) return 0.112613;
      return 0.0845199;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.530437;
      if (fabs(pt) < 25) return 0.321576;
      if (fabs(pt) < 35) return 0.181209;
      if (fabs(pt) < 50) return 0.133215;
      return 0.13324;
    } 
    if (fabs(eta) < 2.4){ 
      if (fabs(pt) < 15) return 0.614679;
      if (fabs(pt) < 25) return 0.347457;
      if (fabs(pt) < 35) return 0.166667;
      if (fabs(pt) < 50) return 0.153846;
      return 0.12766;
    } 
  } 
  return 0; 
} 
