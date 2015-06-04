float functionAG_FO4(float pt, float eta, int id){
  if (abs(id) == 11){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.457629;
      if (fabs(pt) < 25) return 0.220728;
      if (fabs(pt) < 35) return 0.118385;
      if (fabs(pt) < 50) return 0.100756;
      return 0.181331;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.475137;
      if (fabs(pt) < 25) return 0.287097;
      if (fabs(pt) < 35) return 0.190476;
      if (fabs(pt) < 50) return 0.12743;
      return 0.278746;
    } 
    if (fabs(eta) < 4){ 
      if (fabs(pt) < 15) return 0.380952;
      if (fabs(pt) < 25) return 0.6;
      if (fabs(pt) < 35) return 0.222222;
      if (fabs(pt) < 50) return 0.263158;
      return 0.15;
    } 
  } 
  if (abs(id) == 13){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.498646;
      if (fabs(pt) < 25) return 0.250212;
      if (fabs(pt) < 35) return 0.131808;
      if (fabs(pt) < 50) return 0.112885;
      return 0.0864425;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.528263;
      if (fabs(pt) < 25) return 0.325999;
      if (fabs(pt) < 35) return 0.18322;
      if (fabs(pt) < 50) return 0.13849;
      return 0.131542;
    } 
    if (fabs(eta) < 2.4){ 
      if (fabs(pt) < 15) return 0.62037;
      if (fabs(pt) < 25) return 0.350427;
      if (fabs(pt) < 35) return 0.168831;
      if (fabs(pt) < 50) return 0.142857;
      return 0.133333;
    } 
  } 
  return 0; 
} 
