float getPUwECO(int nvtx) {
  float array[] = {1.0, 3.153000427291265, 2.4535501340758543, 2.353696182351581, 2.3718057802881676, 2.3508262193470397, 2.291773427755106, 2.129929297304804, 1.9422796094930384, 1.7126175249202766, 1.4622401225778663, 1.2063711142884181, 0.9608504360968657, 0.7484941355600901, 0.5769889517104192, 0.4315759540480359, 0.3195820162866148, 0.2306052595765186, 0.1691819686464576, 0.12324466445693416, 0.08833641123547825, 0.06306193566475429, 0.047857788612281564, 0.034655984107483044, 0.02358209210941948, 0.019535383939466185, 0.01500976654907348, 0.009407396052837736, 0.006332906010381258, 0.008524617116368491, 0.003145186350322339, 0.0, 0.0, 0.006930381551001214, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  if (nvtx<60) return array[nvtx];
  else return 1.;
}
float getTruePUwECO(int nvtx) {
   if (nvtx>=0.000000 && nvtx<1.000000) return 0.001242;
   if (nvtx>=1.000000 && nvtx<2.000000) return 0.009134;
   if (nvtx>=2.000000 && nvtx<3.000000) return 0.009748;
   if (nvtx>=3.000000 && nvtx<4.000000) return 0.022781;
   if (nvtx>=4.000000 && nvtx<5.000000) return 0.044303;
   if (nvtx>=5.000000 && nvtx<6.000000) return 0.044381;
   if (nvtx>=6.000000 && nvtx<7.000000) return 0.074973;
   if (nvtx>=7.000000 && nvtx<8.000000) return 0.452390;
   if (nvtx>=8.000000 && nvtx<9.000000) return 0.800787;
   if (nvtx>=9.000000 && nvtx<10.000000) return 0.869224;
   if (nvtx>=10.000000 && nvtx<11.000000) return 0.914828;
   if (nvtx>=11.000000 && nvtx<12.000000) return 1.114916;
   if (nvtx>=12.000000 && nvtx<13.000000) return 1.421726;
   if (nvtx>=13.000000 && nvtx<14.000000) return 1.716146;
   if (nvtx>=14.000000 && nvtx<15.000000) return 1.954499;
   if (nvtx>=15.000000 && nvtx<16.000000) return 1.931450;
   if (nvtx>=16.000000 && nvtx<17.000000) return 1.771625;
   if (nvtx>=17.000000 && nvtx<18.000000) return 1.765461;
   if (nvtx>=18.000000 && nvtx<19.000000) return 1.553672;
   if (nvtx>=19.000000 && nvtx<20.000000) return 1.486273;
   if (nvtx>=20.000000 && nvtx<21.000000) return 1.155802;
   if (nvtx>=21.000000 && nvtx<22.000000) return 0.914544;
   if (nvtx>=22.000000 && nvtx<23.000000) return 0.763326;
   if (nvtx>=23.000000 && nvtx<24.000000) return 0.628013;
   if (nvtx>=24.000000 && nvtx<25.000000) return 0.457471;
   if (nvtx>=25.000000 && nvtx<26.000000) return 0.359195;
   if (nvtx>=26.000000 && nvtx<27.000000) return 0.224582;
   if (nvtx>=27.000000 && nvtx<28.000000) return 0.152288;
   if (nvtx>=28.000000 && nvtx<29.000000) return 0.085062;
   if (nvtx>=29.000000 && nvtx<30.000000) return 0.050443;
   if (nvtx>=30.000000 && nvtx<31.000000) return 0.028176;
   if (nvtx>=31.000000 && nvtx<32.000000) return 0.015188;
   if (nvtx>=32.000000 && nvtx<33.000000) return 0.010569;
   if (nvtx>=33.000000 && nvtx<34.000000) return 0.006558;
   if (nvtx>=34.000000 && nvtx<35.000000) return 0.005573;
   if (nvtx>=35.000000 && nvtx<36.000000) return 0.005573;
   if (nvtx>=36.000000 && nvtx<37.000000) return 0.006753;
   if (nvtx>=37.000000 && nvtx<38.000000) return 0.005966;
   if (nvtx>=38.000000 && nvtx<39.000000) return 0.000000;
   if (nvtx>=39.000000 && nvtx<40.000000) return 0.000000;
   if (nvtx>=40.000000 && nvtx<41.000000) return 0.000000;
   if (nvtx>=41.000000 && nvtx<42.000000) return 0.000000;
   if (nvtx>=42.000000 && nvtx<43.000000) return 0.000000;
   if (nvtx>=43.000000 && nvtx<44.000000) return 0.000000;
   if (nvtx>=44.000000 && nvtx<45.000000) return 0.000000;
   if (nvtx>=45.000000 && nvtx<46.000000) return 0.000000;
   if (nvtx>=46.000000 && nvtx<47.000000) return 0.000000;
   if (nvtx>=47.000000 && nvtx<48.000000) return 0.000000;
   if (nvtx>=48.000000 && nvtx<49.000000) return 0.000000;
   if (nvtx>=49.000000 && nvtx<50.000000) return 0.000000;
   return 0.;
}



float getTruePUw(float nvtx) {
   if (nvtx>=0.000000 && nvtx<1.000000) return 0.000000;
   if (nvtx>=1.000000 && nvtx<2.000000) return 0.000000;
   if (nvtx>=2.000000 && nvtx<3.000000) return 0.000000;
   if (nvtx>=3.000000 && nvtx<4.000000) return 0.000000;
   if (nvtx>=4.000000 && nvtx<5.000000) return 15.246564;
   if (nvtx>=5.000000 && nvtx<6.000000) return 3.977942;
   if (nvtx>=6.000000 && nvtx<7.000000) return 2.341931;
   if (nvtx>=7.000000 && nvtx<8.000000) return 2.567554;
   if (nvtx>=8.000000 && nvtx<9.000000) return 3.502121;
   if (nvtx>=9.000000 && nvtx<10.000000) return 3.164413;
   if (nvtx>=10.000000 && nvtx<11.000000) return 2.884023;
   if (nvtx>=11.000000 && nvtx<12.000000) return 2.723691;
   if (nvtx>=12.000000 && nvtx<13.000000) return 2.224248;
   if (nvtx>=13.000000 && nvtx<14.000000) return 1.475022;
   if (nvtx>=14.000000 && nvtx<15.000000) return 0.871136;
   if (nvtx>=15.000000 && nvtx<16.000000) return 0.407562;
   if (nvtx>=16.000000 && nvtx<17.000000) return 0.172213;
   if (nvtx>=17.000000 && nvtx<18.000000) return 0.072634;
   if (nvtx>=18.000000 && nvtx<19.000000) return 0.038881;
   if (nvtx>=19.000000 && nvtx<20.000000) return 0.021570;
   if (nvtx>=20.000000 && nvtx<21.000000) return 0.011562;
   if (nvtx>=21.000000 && nvtx<22.000000) return 0.005226;
   if (nvtx>=22.000000 && nvtx<23.000000) return 0.001878;
   if (nvtx>=23.000000 && nvtx<24.000000) return 0.000634;
   if (nvtx>=24.000000 && nvtx<25.000000) return 0.000215;
   if (nvtx>=25.000000 && nvtx<26.000000) return 0.000097;
   if (nvtx>=26.000000 && nvtx<27.000000) return 0.000053;
   if (nvtx>=27.000000 && nvtx<28.000000) return 0.000036;
   if (nvtx>=28.000000 && nvtx<29.000000) return 0.000024;
   if (nvtx>=29.000000 && nvtx<30.000000) return 0.000018;
   if (nvtx>=30.000000 && nvtx<31.000000) return 0.000013;
   if (nvtx>=31.000000 && nvtx<32.000000) return 0.000010;
   if (nvtx>=32.000000 && nvtx<33.000000) return 0.000003;
   if (nvtx>=33.000000 && nvtx<34.000000) return 0.000002;
   if (nvtx>=34.000000 && nvtx<35.000000) return 0.000001;
   if (nvtx>=35.000000 && nvtx<36.000000) return 0.000000;
   if (nvtx>=36.000000 && nvtx<37.000000) return 0.000000;
   if (nvtx>=37.000000 && nvtx<38.000000) return 0.000000;
   if (nvtx>=38.000000 && nvtx<39.000000) return 0.000000;
   if (nvtx>=39.000000 && nvtx<40.000000) return 0.000000;
   if (nvtx>=40.000000 && nvtx<41.000000) return 0.000000;
   if (nvtx>=41.000000 && nvtx<42.000000) return 0.000000;
   if (nvtx>=42.000000 && nvtx<43.000000) return 0.000000;
   if (nvtx>=43.000000 && nvtx<44.000000) return 0.000000;
   if (nvtx>=44.000000 && nvtx<45.000000) return 0.000000;
   if (nvtx>=45.000000 && nvtx<46.000000) return 0.000000;
   if (nvtx>=46.000000 && nvtx<47.000000) return 0.000000;
   if (nvtx>=47.000000 && nvtx<48.000000) return 0.000000;
   if (nvtx>=48.000000 && nvtx<49.000000) return 0.000000;
   if (nvtx>=49.000000 && nvtx<50.000000) return 0.000000;
   return 0.;
}

float getTruePUwUp(float nvtx) {
   if (nvtx>=0.000000 && nvtx<1.000000) return 0.000000;
   if (nvtx>=1.000000 && nvtx<2.000000) return 0.000000;
   if (nvtx>=2.000000 && nvtx<3.000000) return 0.000000;
   if (nvtx>=3.000000 && nvtx<4.000000) return 0.000000;
   if (nvtx>=4.000000 && nvtx<5.000000) return 12.522049;
   if (nvtx>=5.000000 && nvtx<6.000000) return 2.932354;
   if (nvtx>=6.000000 && nvtx<7.000000) return 1.505608;
   if (nvtx>=7.000000 && nvtx<8.000000) return 1.507453;
   if (nvtx>=8.000000 && nvtx<9.000000) return 2.367344;
   if (nvtx>=9.000000 && nvtx<10.000000) return 2.480088;
   if (nvtx>=10.000000 && nvtx<11.000000) return 2.481849;
   if (nvtx>=11.000000 && nvtx<12.000000) return 2.559506;
   if (nvtx>=12.000000 && nvtx<13.000000) return 2.342126;
   if (nvtx>=13.000000 && nvtx<14.000000) return 1.794335;
   if (nvtx>=14.000000 && nvtx<15.000000) return 1.247522;
   if (nvtx>=15.000000 && nvtx<16.000000) return 0.688426;
   if (nvtx>=16.000000 && nvtx<17.000000) return 0.330647;
   if (nvtx>=17.000000 && nvtx<18.000000) return 0.140597;
   if (nvtx>=18.000000 && nvtx<19.000000) return 0.067310;
   if (nvtx>=19.000000 && nvtx<20.000000) return 0.036772;
   if (nvtx>=20.000000 && nvtx<21.000000) return 0.023380;
   if (nvtx>=21.000000 && nvtx<22.000000) return 0.013898;
   if (nvtx>=22.000000 && nvtx<23.000000) return 0.006735;
   if (nvtx>=23.000000 && nvtx<24.000000) return 0.002928;
   if (nvtx>=24.000000 && nvtx<25.000000) return 0.001105;
   if (nvtx>=25.000000 && nvtx<26.000000) return 0.000459;
   if (nvtx>=26.000000 && nvtx<27.000000) return 0.000217;
   if (nvtx>=27.000000 && nvtx<28.000000) return 0.000133;
   if (nvtx>=28.000000 && nvtx<29.000000) return 0.000087;
   if (nvtx>=29.000000 && nvtx<30.000000) return 0.000068;
   if (nvtx>=30.000000 && nvtx<31.000000) return 0.000052;
   if (nvtx>=31.000000 && nvtx<32.000000) return 0.000045;
   if (nvtx>=32.000000 && nvtx<33.000000) return 0.000015;
   if (nvtx>=33.000000 && nvtx<34.000000) return 0.000014;
   if (nvtx>=34.000000 && nvtx<35.000000) return 0.000007;
   if (nvtx>=35.000000 && nvtx<36.000000) return 0.000002;
   if (nvtx>=36.000000 && nvtx<37.000000) return 0.000001;
   if (nvtx>=37.000000 && nvtx<38.000000) return 0.000000;
   if (nvtx>=38.000000 && nvtx<39.000000) return 0.000000;
   if (nvtx>=39.000000 && nvtx<40.000000) return 0.000000;
   if (nvtx>=40.000000 && nvtx<41.000000) return 0.000000;
   if (nvtx>=41.000000 && nvtx<42.000000) return 0.000000;
   if (nvtx>=42.000000 && nvtx<43.000000) return 0.000000;
   if (nvtx>=43.000000 && nvtx<44.000000) return 0.000000;
   if (nvtx>=44.000000 && nvtx<45.000000) return 0.000000;
   if (nvtx>=45.000000 && nvtx<46.000000) return 0.000000;
   if (nvtx>=46.000000 && nvtx<47.000000) return 0.000000;
   if (nvtx>=47.000000 && nvtx<48.000000) return 0.000000;
   if (nvtx>=48.000000 && nvtx<49.000000) return 0.000000;
   if (nvtx>=49.000000 && nvtx<50.000000) return 0.000000;
   return 0.;
}

float getTruePUwDn(float nvtx) {
   if (nvtx>=0.000000 && nvtx<1.000000) return 0.000000;
   if (nvtx>=1.000000 && nvtx<2.000000) return 0.000000;
   if (nvtx>=2.000000 && nvtx<3.000000) return 0.000000;
   if (nvtx>=3.000000 && nvtx<4.000000) return 0.000000;
   if (nvtx>=4.000000 && nvtx<5.000000) return 18.976666;
   if (nvtx>=5.000000 && nvtx<6.000000) return 5.582817;
   if (nvtx>=6.000000 && nvtx<7.000000) return 3.840839;
   if (nvtx>=7.000000 && nvtx<8.000000) return 4.216011;
   if (nvtx>=8.000000 && nvtx<9.000000) return 4.821103;
   if (nvtx>=9.000000 && nvtx<10.000000) return 3.849269;
   if (nvtx>=10.000000 && nvtx<11.000000) return 3.205660;
   if (nvtx>=11.000000 && nvtx<12.000000) return 2.725626;
   if (nvtx>=12.000000 && nvtx<13.000000) return 1.935857;
   if (nvtx>=13.000000 && nvtx<14.000000) return 1.087614;
   if (nvtx>=14.000000 && nvtx<15.000000) return 0.539181;
   if (nvtx>=15.000000 && nvtx<16.000000) return 0.217024;
   if (nvtx>=16.000000 && nvtx<17.000000) return 0.087994;
   if (nvtx>=17.000000 && nvtx<18.000000) return 0.041315;
   if (nvtx>=18.000000 && nvtx<19.000000) return 0.022912;
   if (nvtx>=19.000000 && nvtx<20.000000) return 0.010731;
   if (nvtx>=20.000000 && nvtx<21.000000) return 0.004320;
   if (nvtx>=21.000000 && nvtx<22.000000) return 0.001428;
   if (nvtx>=22.000000 && nvtx<23.000000) return 0.000397;
   if (nvtx>=23.000000 && nvtx<24.000000) return 0.000123;
   if (nvtx>=24.000000 && nvtx<25.000000) return 0.000047;
   if (nvtx>=25.000000 && nvtx<26.000000) return 0.000025;
   if (nvtx>=26.000000 && nvtx<27.000000) return 0.000015;
   if (nvtx>=27.000000 && nvtx<28.000000) return 0.000010;
   if (nvtx>=28.000000 && nvtx<29.000000) return 0.000006;
   if (nvtx>=29.000000 && nvtx<30.000000) return 0.000004;
   if (nvtx>=30.000000 && nvtx<31.000000) return 0.000003;
   if (nvtx>=31.000000 && nvtx<32.000000) return 0.000002;
   if (nvtx>=32.000000 && nvtx<33.000000) return 0.000001;
   if (nvtx>=33.000000 && nvtx<34.000000) return 0.000000;
   if (nvtx>=34.000000 && nvtx<35.000000) return 0.000000;
   if (nvtx>=35.000000 && nvtx<36.000000) return 0.000000;
   if (nvtx>=36.000000 && nvtx<37.000000) return 0.000000;
   if (nvtx>=37.000000 && nvtx<38.000000) return 0.000000;
   if (nvtx>=38.000000 && nvtx<39.000000) return 0.000000;
   if (nvtx>=39.000000 && nvtx<40.000000) return 0.000000;
   if (nvtx>=40.000000 && nvtx<41.000000) return 0.000000;
   if (nvtx>=41.000000 && nvtx<42.000000) return 0.000000;
   if (nvtx>=42.000000 && nvtx<43.000000) return 0.000000;
   if (nvtx>=43.000000 && nvtx<44.000000) return 0.000000;
   if (nvtx>=44.000000 && nvtx<45.000000) return 0.000000;
   if (nvtx>=45.000000 && nvtx<46.000000) return 0.000000;
   if (nvtx>=46.000000 && nvtx<47.000000) return 0.000000;
   if (nvtx>=47.000000 && nvtx<48.000000) return 0.000000;
   if (nvtx>=48.000000 && nvtx<49.000000) return 0.000000;
   if (nvtx>=49.000000 && nvtx<50.000000) return 0.000000;
   return 0.;
}

float getPUw(int nvtx) {
  if (nvtx>=0.000000 && nvtx<2.000000) return 6.874322;
  if (nvtx>=2.000000 && nvtx<4.000000) return 2.078853;
  if (nvtx>=4.000000 && nvtx<6.000000) return 1.931197;
  if (nvtx>=6.000000 && nvtx<8.000000) return 1.983540;
  if (nvtx>=8.000000 && nvtx<10.000000) return 1.749420;
  if (nvtx>=10.000000 && nvtx<12.000000) return 1.240954;
  if (nvtx>=12.000000 && nvtx<14.000000) return 0.963793;
  if (nvtx>=14.000000 && nvtx<16.000000) return 0.642933;
  if (nvtx>=16.000000 && nvtx<18.000000) return 0.391587;
  if (nvtx>=18.000000 && nvtx<20.000000) return 0.199816;
  if (nvtx>=20.000000 && nvtx<22.000000) return 0.093767;
  if (nvtx>=22.000000 && nvtx<24.000000) return 0.076702;
  if (nvtx>=24.000000 && nvtx<26.000000) return 0.000000;
  if (nvtx>=26.000000 && nvtx<28.000000) return 0.000000;
  if (nvtx>=28.000000 && nvtx<30.000000) return 0.000000;
  if (nvtx>=30.000000 && nvtx<32.000000) return 0.000000;
  if (nvtx>=32.000000 && nvtx<34.000000) return 0.000000;
  if (nvtx>=34.000000 && nvtx<36.000000) return 0.000000;
  if (nvtx>=36.000000 && nvtx<38.000000) return 0.000000;
  if (nvtx>=38.000000 && nvtx<40.000000) return 0.000000;
  return 0.;
}

float getPUwUnblind(int nvtx) {
  if (nvtx>=0.000000 && nvtx<2.000000) return 14.014330;
  if (nvtx>=2.000000 && nvtx<4.000000) return 3.388680;
  if (nvtx>=4.000000 && nvtx<6.000000) return 3.362378;
  if (nvtx>=6.000000 && nvtx<8.000000) return 2.350122;
  if (nvtx>=8.000000 && nvtx<10.000000) return 1.836804;
  if (nvtx>=10.000000 && nvtx<12.000000) return 1.217169;
  if (nvtx>=12.000000 && nvtx<14.000000) return 0.817244;
  if (nvtx>=14.000000 && nvtx<16.000000) return 0.408258;
  if (nvtx>=16.000000 && nvtx<18.000000) return 0.196930;
  if (nvtx>=18.000000 && nvtx<20.000000) return 0.138041;
  if (nvtx>=20.000000 && nvtx<22.000000) return 0.028379;
  if (nvtx>=22.000000 && nvtx<24.000000) return 0.081731;
  if (nvtx>=24.000000 && nvtx<26.000000) return 0.000000;
  if (nvtx>=26.000000 && nvtx<28.000000) return 0.000000;
  if (nvtx>=28.000000 && nvtx<30.000000) return 0.000000;
  if (nvtx>=30.000000 && nvtx<32.000000) return 0.000000;
  if (nvtx>=32.000000 && nvtx<34.000000) return 0.000000;
  if (nvtx>=34.000000 && nvtx<36.000000) return 0.000000;
  if (nvtx>=36.000000 && nvtx<38.000000) return 0.000000;
  if (nvtx>=38.000000 && nvtx<40.000000) return 0.000000;
  return 0.;
}
