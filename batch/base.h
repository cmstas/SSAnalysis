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

  //Fastsim
  T1TTTT_1950_700to950 = 10001,
  T1TTTT_1100_1to775 = 10002,
  T1TTTT_1175_950 = 10003,
  T1TTTT_1275_900to975 = 10004,
  T1TTTT_1300_1to1075 = 10005,
  T1TTTT_1300to1325_700to1100 = 10006,
  T1TTTT_1350to1375_50to1025 = 10007,
  T1TTTT_1500to1525_50to1125 = 10008,
  T1TTTT_1800to1850_1to1450 = 10009,
  T1TTTT_1950_700to950 = 10010,
  T1TTTT_625_275to375 = 10011,
  T1TTTT_625to650_200to400 = 10012,
  T1TTTT_650to675_250to425 = 10013,
  T1TTTT_700to750_200to500 = 10014

};

#endif
