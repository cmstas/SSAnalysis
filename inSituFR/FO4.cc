float functionAG_FO4(float pt, float eta, int id){
  if (abs(id) == 11){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.364764;
      if (fabs(pt) < 25) return 0.176829;
      if (fabs(pt) < 35) return 0.0838253;
      if (fabs(pt) < 50) return 0.0694716;
      return 0.0875866;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.407035;
      if (fabs(pt) < 25) return 0.209115;
      if (fabs(pt) < 35) return 0.150812;
      if (fabs(pt) < 50) return 0.0915493;
      return 0.162844;
    } 
    if (fabs(eta) < 4){ 
      if (fabs(pt) < 15) return 0.2;
      if (fabs(pt) < 25) return 0.435897;
      if (fabs(pt) < 35) return 0.176471;
      if (fabs(pt) < 50) return 0.152542;
      return 0.0322581;
    } 
  } 
  if (abs(id) == 13){ 
    if (fabs(eta) < 1){ 
      if (fabs(pt) < 15) return 0.494689;
      if (fabs(pt) < 25) return 0.253398;
      if (fabs(pt) < 35) return 0.127381;
      if (fabs(pt) < 50) return 0.11054;
      return 0.0890269;
    } 
    if (fabs(eta) < 2){ 
      if (fabs(pt) < 15) return 0.529563;
      if (fabs(pt) < 25) return 0.325228;
      if (fabs(pt) < 35) return 0.176806;
      if (fabs(pt) < 50) return 0.133874;
      return 0.139683;
    } 
    if (fabs(eta) < 2.4){ 
      if (fabs(pt) < 15) return 0.637255;
      if (fabs(pt) < 25) return 0.373832;
      if (fabs(pt) < 35) return 0.169014;
      if (fabs(pt) < 50) return 0.140351;
      return 0.128205;
    } 
  } 
  return 0; 
} 
