#ifndef THESSBATCHBASE_H
#define THESSBATCHBASE_H

enum sample_t { 
  //Backgrounds
  TTBAR = 0,
  TTW = 1,
  TTZL = 2,
  WZ = 3,
  DY_low = 4,
  DY_high = 5,
  WJets = 6,
  TTWQQ = 7,
  TTZQ = 8,
  TTG = 9,
  SINGLETOP1 = 10,
  SINGLETOP2 = 11,
  SINGLETOP3 = 12,
  SINGLETOP4 = 13,
  SINGLETOP5 = 14,
  QQWW = 15,
  TTTT = 16,
  WWDPS = 17, 
  TTBAR_PH = 18,
  WJets_LO = 19, 
  WWZ = 20,
  ZZ = 21,
  TG = 22,
  ZG = 23,
  WZZ = 24,
  WGToLNuG = 25,
  TZQ = 26,
  TTHtoNonBB = 27,
  VHtoNonBB = 28,
  TTZlow = 29,
  GGHtoZZto4L = 30,
  WZMG = 31,

  //Signals
  T1TTTT_1500 = 100,
  T1TTTT_1200 = 101,
  T5QQQQWW_1200_1000_800 = 102,
  T5ttttDeg_1000_300_285_280 = 103,
  T5QQQQWWDeg_1000_315_300 = 104,
  T6TTWW_600_425_50 = 105,
  T6TTWW_650_150_50 = 106,

  //Data
  DataDoubleEGC_05oct = 1000,
  DataDoubleMuonC_05oct = 1001,
  DataMuonEGC_05oct = 1002,
  DataDoubleEGD_05oct = 1003,
  DataDoubleMuonD_05oct = 1004,
  DataMuonEGD_05oct = 1005,
  DataDoubleEGD_v4 = 1006,
  DataDoubleMuonD_v4 = 1007,
  DataMuonEGD_v4 = 1008,

  //fastsim
  FASTSIMTEST = 10001

};

#endif
