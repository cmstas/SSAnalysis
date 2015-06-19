float functionAG_FO4(float pt, float eta, int id){
  if (abs(id) == 11){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.392252;
      if (fabs(pt) < 25) return 0.201227;
      if (fabs(pt) < 35) return 0.11471;
      if (fabs(pt) < 50) return 0.107817;
      return 0.203614;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.438776;
      if (fabs(pt) < 25) return 0.250704;
      if (fabs(pt) < 35) return 0.17942;
      if (fabs(pt) < 50) return 0.12854;
      return 0.322581;
    } 
    if (fabs(eta) < 4){ 
      if (fabs(pt) < 15) return 0.347826;
      if (fabs(pt) < 25) return 0.5625;
      if (fabs(pt) < 35) return 0.214286;
      if (fabs(pt) < 50) return 0.27027;
      return 0.166667;
    } 
  } 
  if (abs(id) == 13){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.439665;
      if (fabs(pt) < 25) return 0.231379;
      if (fabs(pt) < 35) return 0.130811;
      if (fabs(pt) < 50) return 0.122981;
      return 0.0979381;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.468208;
      if (fabs(pt) < 25) return 0.305412;
      if (fabs(pt) < 35) return 0.179229;
      if (fabs(pt) < 50) return 0.144737;
      return 0.152459;
    } 
    if (fabs(eta) < 2.4){ 
      if (fabs(pt) < 15) return 0.603604;
      if (fabs(pt) < 25) return 0.322835;
      if (fabs(pt) < 35) return 0.164557;
      if (fabs(pt) < 50) return 0.140625;
      return 0.157895;
    } 
  } 
  return 0; 
} 
