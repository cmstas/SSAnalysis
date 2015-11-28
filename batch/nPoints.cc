//sample: 1-T1tttt
int nPoints(int sample, int mgl, int mlsp){
  if (sample == 1){
    if (mgl == 875 && mlsp == 625) return 140950; 
    if (mgl == 900 && mlsp == 200) return 93828; 
    if (mgl == 1950 && mlsp == 700) return 59440; 
    if (mgl == 1950 && mlsp == 750) return 59153; 
    if (mgl == 1950 && mlsp == 800) return 59352; 
    if (mgl == 1950 && mlsp == 850) return 59128; 
    if (mgl == 1950 && mlsp == 900) return 59218; 
    if (mgl == 1950 && mlsp == 950) return 59475; 
  }
  return -99999999; 
}
