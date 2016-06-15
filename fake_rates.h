float electronFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.168384;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.170462;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.245749;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0536174;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0416785;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0660986;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0402907;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0570583;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0525172;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0561272;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0696171;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0572421;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.11196;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.100606;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.133193;
   return 0.;
}
float electronFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0276677;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0277533;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0335481;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0123214;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0104385;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0119009;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.00449776;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00518548;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00428526;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.00938335;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00919875;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00658762;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0274407;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0210568;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0160838;
   return 0.;
}
float electronAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.169133;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.170894;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.246087;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0551309;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0426178;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0667364;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0455504;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0599722;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0543795;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.076997;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0849654;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0661659;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.160472;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.131302;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.148625;
   return 0.;
}
float electronQCDMCFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.293974;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.299313;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.347698;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0496904;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0728707;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.101218;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0379164;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.071715;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0710578;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0687494;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0644791;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.069804;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.108405;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.147836;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.154842;
   return 0.;
}
float muonFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.303176;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.383369;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.474765;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0760743;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0889911;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.117669;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0486928;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0500247;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0606419;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0529314;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0503441;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0524003;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0755424;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.073468;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0322786;
   return 0.;
}
float muonFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0113247;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0156516;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0344229;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00508396;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00672978;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0165933;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00181455;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0021633;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00558127;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00321955;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00364583;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00876463;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.010075;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0108089;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0233983;
   return 0.;
}
float muonAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.303425;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.383608;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.475046;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0765699;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0894597;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.118206;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0502729;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0513112;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0622388;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0589356;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0559251;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0597788;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.10504;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0988333;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0686786;
   return 0.;
}
float muonQCDMCFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.358195;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.421611;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.543816;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   return 0.;
}
float electronFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.227242;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.211376;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.311476;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.104247;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0974708;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.128644;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.112972;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.162849;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.127392;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.162383;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.197195;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.185127;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.29201;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.245932;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.266325;
   return 0.;
}
float electronFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0371891;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.03806;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0433731;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.025403;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0228844;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.023147;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0144844;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0171633;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0123426;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0260548;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0232302;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0179043;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0551305;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0429955;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0273871;
   return 0.;
}
float electronAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.228187;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.211991;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.311895;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.107378;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.09933;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.129818;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.126824;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.170329;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.131804;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.21541;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.234418;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.208836;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.384654;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.3078;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.29146;
   return 0.;
}
float electronQCDMCFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.364548;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.371194;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.464356;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.102098;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.141282;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.180317;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.121292;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.153545;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.14763;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.178724;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.127802;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.158211;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.210804;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.279705;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.260886;
   return 0.;
}
float muonFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.335468;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.424141;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.514064;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0988973;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.114858;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.158991;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0821553;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0854946;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0875663;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0954373;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0914311;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.073187;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.134881;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.167181;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.117328;
   return 0.;
}
float muonFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00851048;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0113599;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0246236;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0046312;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00604919;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0146796;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00333851;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00416388;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00839459;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0063966;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00747728;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0154163;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0208599;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0235036;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0530627;
   return 0.;
}
float muonAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.335717;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.424364;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.514327;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0995269;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.115442;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.159611;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0844963;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0875789;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0896361;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.10432;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.100051;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0835691;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.179482;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.204768;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.166963;
   return 0.;
}
float muonQCDMCFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.375355;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.440386;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.561871;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   return 0.;
}
