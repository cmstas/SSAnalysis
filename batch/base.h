#ifndef THESSBATCHBASE_H
#define THESSBATCHBASE_H

enum sample_t { 
  //Backgrounds
  TTBAR = 0,
  TTW = 1,
  TTZ = 2,
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
  TTBAR_PH = 18, //19 removed
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
  WGMG = 32,
  TTHtoNonBBext = 33,
  ZZZ = 34,
  WWW = 35,

  TTZnlo = 36,
  TTWnlo = 37,

  //Signals
  T1TTTT_1500 = 100,
  T1TTTT_1200 = 101,
  T5QQQQWW_1200_1000_800 = 102,
  T5ttttDeg_1000_300_285_280 = 103,
  T5QQQQWWDeg_1000_315_300 = 104,
  T6TTWW_600_425_50 = 105,
  T6TTWW_650_150_50 = 106,

  //Data
  DataDoubleEGv1 = 1000,
  DataDoubleMuonv1 = 1001,
  DataMuonEGv1 = 1002,
  DataDoubleEGv2 = 1003,
  DataDoubleMuonv2 = 1004,
  DataMuonEGv2 = 1005,
  // DataDoubleEGD = 1003,
  // DataDoubleMuonD = 1004,
  // DataMuonEGD = 1005,
  // DataDoubleEGD_v4 = 1006,
  // DataDoubleMuonD_v4 = 1007,
  // DataMuonEGD_v4 = 1008,

  //Fastsim
  T1TTTT_main                 = 10001,
  T1TTTT_1100_1to775          = 10002,
  T1TTTT_1175_950             = 10003,
  T1TTTT_1275_900to975        = 10004,
  T1TTTT_1300_1to1075         = 10005,
  T1TTTT_1300to1325_700to1100 = 10006,
  T1TTTT_1350to1375_50to1025  = 10007,
  T1TTTT_1500to1525_50to1125  = 10008,
  T1TTTT_1800to1850_1to1450   = 10009,
  T1TTTT_1100to1125_700to900  = 10010,
  T1TTTT_625_275to375         = 10011,
  T1TTTT_625to650_200to400    = 10012,
  T1TTTT_650to675_250to425    = 10013,
  T1TTTT_700to750_200to500    = 10014,
  T1TTTT_1150_1to800          = 10015,
  T1TTTT_1150to1175_750to925  = 10016,
  T1TTTT_1200_1to825          = 10017,
  T1TTTT_1200to1225_800to1000 = 10018,
  T1TTTT_1250to1275_700to1050 = 10019,
  T1TTTT_1325to1350_1to1125   = 10020,
  T1TTTT_1400_1to1175         = 10021,
  T1TTTT_1400to1425_50to1100  = 10022,
  T1TTTT_1425to1450_1to1200   = 10023,
  T1TTTT_1450to1475_50to1075  = 10024,
  T1TTTT_1475to1500_1to1250   = 10025,
  T1TTTT_1525to1550_1to1300   = 10026,
  T1TTTT_1600to1650_1to1350   = 10027,
  T1TTTT_1650to1700_1to1400   = 10028,
  T1TTTT_1700to1750_1to1450   = 10029,
  T1TTTT_1750_50to1450        = 10030,
  T1TTTT_1850to1900_1to1450   = 10031,
  T1TTTT_1900to1950_0to1450   = 10032, 
  T1TTTT_600_250to325         = 10033,
  T1TTTT_675_325to450         = 10034,
  T1TTTT_700_1to450           = 10035,
  T1TTTT_750to775_350to525    = 10036,
  T1TTTT_775_475to550         = 10037,
  T1TTTT_800to825_1to575      = 10038,
  T1TTTT_825to850_200to600    = 10039,
  T1TTTT_850to875_450to625    = 10040,
  T1TTTT_875to900_1to650      = 10041,
  T1TTTT_950to975_350to725    = 10042,
  T1TTTT_975_600to750         = 10043,
  T1TTTT_1375_950to1150       = 10044,
  T1TTTT_1000to1050_1to800    = 10045,
  T1TTTT_1000_1to700          = 10046,    //cutoff
  T1TTTT_1050_50to775         = 10047,
  T1TTTT_1050to1075_650to850  = 10048, 
  T1TTTT_1225to1250_1to1025   = 10049,
  T1TTTT_1550to1575_500to1175 = 10050,
  T1TTTT_600_1to225           = 10051,
  T1TTTT_600to625_250to375    = 10052,
  T1TTTT_900to950_200to700    = 10053,

  T5QQQQVV_1000To1075_1To950  = 11000,
  T5QQQQVV_1100To1175_1to1050 = 11001,
  T5QQQQVV_1200To1275_1to1150 = 11002,
  T5QQQQVV_1300To1375_1to1250 = 11003,
  T5QQQQVV_1400To1550_1To1275 = 11004,
  T5QQQQVV_1600To1750_1To950  = 11005,
  T5QQQQVV_600To675_1to550    = 11006,
  T5QQQQVV_700To775_1To650    = 11007,
  T5QQQQVV_800To975_1To850    = 11008,

  T6TTWW_50_300to600_225to575 = 12000,
  T6TTWW_50_300to600_75to125  = 12001,
  T6TTWW_50_350to600_150to400 = 12002,
  T6TTWW_50_625to950_500to850 = 12003,
  T6TTWW_50_625to950_550to875 = 12004,
  T6TTWW_50_625to950_75to125  = 12005,
  T6TTWW_50_650to950_150to750 = 12006,
  T6TTWW_50_300to600_150to500 = 12007,

  T5QQQQWW_1025to1200_0to1175 = 13001,
  T5QQQQWW_1225to1400_0to1175 = 13002,
  T5QQQQWW_1425to1600_0to1175 = 13003,
  T5QQQQWW_1650to1700_0to1150 = 13004,
  T5QQQQWW_600to800_0to725    = 13005,
  T5QQQQWW_825to1000_0to925   = 13006,

  T1TTBB_1225to1400_0to1050     = 14001,
  T1TTBB_1425to1600_0to1250     = 14002,
  T1TTBB_1425to1600_1100to1350  = 14003,
  T1TTBB_600to800_275to575      = 14004,
  T1TTBB_1225to1400_900to1175   = 14005,
  T1TTBB_1650to2000_0to1450     = 14006,
  T1TTBB_825to1000_500to775     = 14007,
  T1TTBB_1650to1750_1350to1450  = 14008,
  T1TTBB_825to1000_0to625       = 14009,
  T1TTBB_600to800_0to450        = 14010,
  T1TTBB_1025to1200_700to975    = 14011,
  T1TTBB_1025to1200_0to850      = 14012,

  T5ttttDM175_825to1000_500to725    = 15001,
  T5ttttDM175_1025to1200_700to925   = 15002,
  T5ttttDM175_1225to1400_0to1050    = 15003,
  T5ttttDM175_1225to1400_900to1125  = 15004,
  T5ttttDM175_1425to1625_1100to1300 = 15005,
  T5ttttDM175_600to800_275to525     = 15006,
  T5ttttDM175_825to1000_0to650      = 15007,
  T5ttttDM175_1425to1600_0to1250    = 15008, 
  T5ttttDM175_1025to1200_0to850     = 15009,
  T5ttttDM175_1625to1700_0to1300    = 15010,
  T5ttttDM175_600to800_0to450       = 15011,

  T5tttt_degen_1225to1400_1075to1275 = 16001,
  T5tttt_degen_1425to1600_1275to1375 = 16002,
  T5tttt_degen_825to1000_0to825      = 16003,
  T5tttt_degen_825to1000_675to875    = 16004,
  T5tttt_degen_1225to1400_0to1225    = 16005,
  T5tttt_degen_600to800_450to675     = 16006,
  T5tttt_degen_1425to1600_0to1350    = 16007,
  T5tttt_degen_1650to1700_0to1350    = 16008,
  T5tttt_degen_1025to1200_0to1025    = 16009,
  T5tttt_degen_600to800_0to625       = 16010,
  T5tttt_degen_1025to1200_875to1075  = 16011,

  T5ttcc_825to1000_675to875          = 17001,
  T5ttcc_1025to1200_875to1075        = 17002,
  T5ttcc_825to1000_0to825            = 17003,
  T5ttcc_600to800_450to675           = 17004,
  T5ttcc_1225to1400_1075to1225       = 17005,
  T5ttcc_1225to1400_0to1225          = 17006,
  T5ttcc_1650to1700_0to1350          = 17007,
  T5ttcc_1425to1525_1275to1375       = 17008,
  T5ttcc_1025to1200_0to1025          = 17009,
  T5ttcc_600to800_0to625             = 17010,
  T5ttcc_1425to1600_0to1350          = 17011,

  ANA1                               = 100000,
  ANA2                               = 100001,
  ANA3                               = 100002,

  SYNCH_TTW                          = 101010,
  SYNCH_TTW80                        = 101011,
  DY_high_LO                         = 101012,

};

#endif
