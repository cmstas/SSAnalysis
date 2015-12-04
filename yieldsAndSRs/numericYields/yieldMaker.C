#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TKey.h"
#include "../../classFiles/v5.04/SS.h"
#include "../../CORE/SSSelections.h"
#include "../../software/tableMaker/CTable.h"
#include "../../software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../commonUtils.h"
#include "../../CORE/Tools/dorky/dorky.h"
#include "../../CORE/Tools/utils.h"

//Root files on/off
bool makeRootFiles = 1;

float lumiAG = getLumiUnblind();
string tag = getTag().Data();  

bool doNoData   = false;
bool testFakeSR = false;

float scaleLumi = 1.;//3.0/1.264;//careful!!!

bool doLatex = 0;

bool suppressWarns = 1;

struct yields_t { float EE; float EM; float MM; float TOTAL; }; 
struct SR_t     { TH1F* EE; TH1F* EM; TH1F* MM; TH1F* TOTAL; }; 
struct plots_t  { TH1F* h_ht; TH1F* h_met; TH1F* h_mll; TH1F* h_mtmin; TH1F* h_njets; TH1F* h_nbtags; TH1F* h_l1pt; TH1F* h_l2pt; TH1F* h_l1eta; TH1F* h_l2eta; TH1F* h_type; TH1F* h_lep1_miniIso; TH1F* h_lep2_miniIso; TH1F* h_lep1_ptRatio; TH1F* h_lep2_ptRatio; TH1F* h_lep1_ptRel; TH1F* h_lep2_ptRel; SR_t SRHH; SR_t SRHL; SR_t SRLL; TH1F* h_dxy; TH1F* h_dz; TH1F* h_sip3d; TH1F* h_mva; TH1F* h_nleps; }; 

//Total
yields_t total; 

//function declaration
pair<yields_t, plots_t> run(TChain *chain, bool isData = 0, bool doFlips = 0, int doFakes = 0, int exclude = 0, bool isSignal = 0, bool isGamma = 0);
void avoidNegativeYields(TH1F* plot);
void avoidSmallYieldsDueToNegWeightsInPromptSubtraction(TH1F* plot);
void fillDownMirrorUp(TH1F* central,TH1F* up,TH1F* down);
void writeStatUpDown(TH1F* central,string name,bool down);
void writeStat(TH1F* central,string name);
void writeTTVExtrSyst(TH1F* central,string name,TString kine);
void writeJesSyst(TH1F* central,string name,TString kine);
void writeHTHltSyst(TH1F* central,string name,TString kine);
void writeBSHltSyst(TH1F* central,string name,TString kine);
bool isSRHighHT(TString kine, int sr);
int nbtagsSR(TString kine, int sr);

static float roughSystTTW   = 0.2;
static float roughSystTTZH  = 0.2;
static float roughSystWZ    = 0.3;
static float roughSystWW    = 0.3;
static float roughSystXG    = 0.5;
static float roughSystRARES = 0.5;
static float roughSystFLIPS = 0.3;
static float roughSystFAKES = 0.35;
static float roughSystFAKESHH = 0.40;
static float roughSystFAKESXL = 0.30;

void getyields(){

  if (doNoData) lumiAG = scaleLumi*getLumi();

  cout << "Running with lumi=" << lumiAG << endl;

  //Chains
  //fakes&flips in mc
  TChain *ttbar_chain   = new TChain("t","ttbar");
  TChain *st_chain      = new TChain("t","st");
  TChain *wjets_chain   = new TChain("t","wjets");
  TChain *dy_chain      = new TChain("t","dy");
  //rares
  TChain *ttw_chain     = new TChain("t","ttw");
  TChain *ttzh_chain    = new TChain("t","ttzh");
  TChain *wz_chain      = new TChain("t","wz");
  TChain *ww_chain      = new TChain("t","ww");
  TChain *xg_chain      = new TChain("t","xg");
  TChain *rares_chain   = new TChain("t","rares");
  //data
  TChain *data_chain    = new TChain("t","data"); 
  TChain *flips_chain   = new TChain("t","flips"); 
  TChain *fakes_chain   = new TChain("t","fakes");
  //signals full sim
  TChain *t1tttt_1200_chain = new TChain("t","t1tttt_1200"); 
  TChain *t1tttt_1500_chain = new TChain("t","t1tttt_1500"); 
  TChain *t2ttww_600_chain = new TChain("t","t6ttww_600"); 
  TChain *t5qqqq_1200_chain = new TChain("t", "t5qqqq_1200"); 
  TChain *t5tttt_deg_chain = new TChain("t","t5tttt_deg"); 
  TChain *t6ttww_650_chain = new TChain("t","t6ttww_650"); 
  TChain *t6ttww_600_chain = new TChain("t","t6ttww_600"); 
  TChain *t5qqqqww_1200_chain = new TChain("t","t5qqqqww_1200"); 
  TChain *t5qqqqww_deg_chain = new TChain("t","t5qqqqww_deg"); 

  //signals fast sim
  TChain *fs_t1tttt_1100_600_chain = new TChain("t","fs_t1tttt_m1100_m600" );
  TChain *fs_t1tttt_1100_775_chain = new TChain("t","fs_t1tttt_m1100_m775" );
  TChain *fs_t1tttt_1100_750_chain = new TChain("t","fs_t1tttt_m1100_m750" );
  TChain *fs_t1tttt_1100_0_chain = new TChain("t","fs_t1tttt_m1100_m0" );
  TChain *fs_t1tttt_1100_300_chain = new TChain("t","fs_t1tttt_m1100_m300" );
  TChain *fs_t1tttt_1100_200_chain = new TChain("t","fs_t1tttt_m1100_m200" );
  TChain *fs_t1tttt_1100_500_chain = new TChain("t","fs_t1tttt_m1100_m500" );
  TChain *fs_t1tttt_1100_250_chain = new TChain("t","fs_t1tttt_m1100_m250" );
  TChain *fs_t1tttt_1100_100_chain = new TChain("t","fs_t1tttt_m1100_m100" );
  TChain *fs_t1tttt_1100_450_chain = new TChain("t","fs_t1tttt_m1100_m450" );
  TChain *fs_t1tttt_1100_50_chain = new TChain("t","fs_t1tttt_m1100_m50" );
  TChain *fs_t1tttt_1100_150_chain = new TChain("t","fs_t1tttt_m1100_m150" );
  TChain *fs_t1tttt_1100_350_chain = new TChain("t","fs_t1tttt_m1100_m350" );
  TChain *fs_t1tttt_1100_700_chain = new TChain("t","fs_t1tttt_m1100_m700" );
  TChain *fs_t1tttt_1100_725_chain = new TChain("t","fs_t1tttt_m1100_m725" );
  TChain *fs_t1tttt_1100_400_chain = new TChain("t","fs_t1tttt_m1100_m400" );
  TChain *fs_t1tttt_1100_550_chain = new TChain("t","fs_t1tttt_m1100_m550" );
  TChain *fs_t1tttt_1100_650_chain = new TChain("t","fs_t1tttt_m1100_m650" );
  TChain *fs_t1tttt_1125_700_chain = new TChain("t","fs_t1tttt_m1125_m700" );
  TChain *fs_t1tttt_1125_850_chain = new TChain("t","fs_t1tttt_m1125_m850" );
  TChain *fs_t1tttt_1125_800_chain = new TChain("t","fs_t1tttt_m1125_m800" );
  TChain *fs_t1tttt_1125_875_chain = new TChain("t","fs_t1tttt_m1125_m875" );
  TChain *fs_t1tttt_1100_825_chain = new TChain("t","fs_t1tttt_m1100_m825" );
  TChain *fs_t1tttt_1100_875_chain = new TChain("t","fs_t1tttt_m1100_m875" );
  TChain *fs_t1tttt_1100_850_chain = new TChain("t","fs_t1tttt_m1100_m850" );
  TChain *fs_t1tttt_1125_825_chain = new TChain("t","fs_t1tttt_m1125_m825" );
  TChain *fs_t1tttt_1125_725_chain = new TChain("t","fs_t1tttt_m1125_m725" );
  TChain *fs_t1tttt_1125_750_chain = new TChain("t","fs_t1tttt_m1125_m750" );
  TChain *fs_t1tttt_1100_800_chain = new TChain("t","fs_t1tttt_m1100_m800" );
  TChain *fs_t1tttt_1125_900_chain = new TChain("t","fs_t1tttt_m1125_m900" );
  TChain *fs_t1tttt_1125_775_chain = new TChain("t","fs_t1tttt_m1125_m775" );
  TChain *fs_t1tttt_1150_150_chain = new TChain("t","fs_t1tttt_m1150_m150" );
  TChain *fs_t1tttt_1150_800_chain = new TChain("t","fs_t1tttt_m1150_m800" );
  TChain *fs_t1tttt_1150_200_chain = new TChain("t","fs_t1tttt_m1150_m200" );
  TChain *fs_t1tttt_1150_775_chain = new TChain("t","fs_t1tttt_m1150_m775" );
  TChain *fs_t1tttt_1150_350_chain = new TChain("t","fs_t1tttt_m1150_m350" );
  TChain *fs_t1tttt_1150_50_chain = new TChain("t","fs_t1tttt_m1150_m50" );
  TChain *fs_t1tttt_1150_550_chain = new TChain("t","fs_t1tttt_m1150_m550" );
  TChain *fs_t1tttt_1150_250_chain = new TChain("t","fs_t1tttt_m1150_m250" );
  TChain *fs_t1tttt_1150_400_chain = new TChain("t","fs_t1tttt_m1150_m400" );
  TChain *fs_t1tttt_1150_650_chain = new TChain("t","fs_t1tttt_m1150_m650" );
  TChain *fs_t1tttt_1150_0_chain = new TChain("t","fs_t1tttt_m1150_m0" );
  TChain *fs_t1tttt_1150_100_chain = new TChain("t","fs_t1tttt_m1150_m100" );
  TChain *fs_t1tttt_1150_750_chain = new TChain("t","fs_t1tttt_m1150_m750" );
  TChain *fs_t1tttt_1150_700_chain = new TChain("t","fs_t1tttt_m1150_m700" );
  TChain *fs_t1tttt_1150_300_chain = new TChain("t","fs_t1tttt_m1150_m300" );
  TChain *fs_t1tttt_1150_450_chain = new TChain("t","fs_t1tttt_m1150_m450" );
  TChain *fs_t1tttt_1150_600_chain = new TChain("t","fs_t1tttt_m1150_m600" );
  TChain *fs_t1tttt_1150_500_chain = new TChain("t","fs_t1tttt_m1150_m500" );
  TChain *fs_t1tttt_1175_925_chain = new TChain("t","fs_t1tttt_m1175_m925" );
  TChain *fs_t1tttt_1175_775_chain = new TChain("t","fs_t1tttt_m1175_m775" );
  TChain *fs_t1tttt_1175_850_chain = new TChain("t","fs_t1tttt_m1175_m850" );
  TChain *fs_t1tttt_1175_900_chain = new TChain("t","fs_t1tttt_m1175_m900" );
  TChain *fs_t1tttt_1175_800_chain = new TChain("t","fs_t1tttt_m1175_m800" );
  TChain *fs_t1tttt_1150_900_chain = new TChain("t","fs_t1tttt_m1150_m900" );
  TChain *fs_t1tttt_1175_875_chain = new TChain("t","fs_t1tttt_m1175_m875" );
  TChain *fs_t1tttt_1175_825_chain = new TChain("t","fs_t1tttt_m1175_m825" );
  TChain *fs_t1tttt_1150_925_chain = new TChain("t","fs_t1tttt_m1150_m925" );
  TChain *fs_t1tttt_1150_825_chain = new TChain("t","fs_t1tttt_m1150_m825" );
  TChain *fs_t1tttt_1150_875_chain = new TChain("t","fs_t1tttt_m1150_m875" );
  TChain *fs_t1tttt_1150_850_chain = new TChain("t","fs_t1tttt_m1150_m850" );
  TChain *fs_t1tttt_1175_750_chain = new TChain("t","fs_t1tttt_m1175_m750" );
  TChain *fs_t1tttt_1175_950_chain = new TChain("t","fs_t1tttt_m1175_m950" );
  TChain *fs_t1tttt_1200_350_chain = new TChain("t","fs_t1tttt_m1200_m350" );
  TChain *fs_t1tttt_1200_550_chain = new TChain("t","fs_t1tttt_m1200_m550" );
  TChain *fs_t1tttt_1200_400_chain = new TChain("t","fs_t1tttt_m1200_m400" );
  TChain *fs_t1tttt_1200_500_chain = new TChain("t","fs_t1tttt_m1200_m500" );
  TChain *fs_t1tttt_1200_600_chain = new TChain("t","fs_t1tttt_m1200_m600" );
  TChain *fs_t1tttt_1200_450_chain = new TChain("t","fs_t1tttt_m1200_m450" );
  TChain *fs_t1tttt_1200_50_chain = new TChain("t","fs_t1tttt_m1200_m50" );
  TChain *fs_t1tttt_1200_700_chain = new TChain("t","fs_t1tttt_m1200_m700" );
  TChain *fs_t1tttt_1200_100_chain = new TChain("t","fs_t1tttt_m1200_m100" );
  TChain *fs_t1tttt_1200_150_chain = new TChain("t","fs_t1tttt_m1200_m150" );
  TChain *fs_t1tttt_1200_200_chain = new TChain("t","fs_t1tttt_m1200_m200" );
  TChain *fs_t1tttt_1200_825_chain = new TChain("t","fs_t1tttt_m1200_m825" );
  TChain *fs_t1tttt_1200_300_chain = new TChain("t","fs_t1tttt_m1200_m300" );
  TChain *fs_t1tttt_1200_800_chain = new TChain("t","fs_t1tttt_m1200_m800" );
  TChain *fs_t1tttt_1200_250_chain = new TChain("t","fs_t1tttt_m1200_m250" );
  TChain *fs_t1tttt_1200_650_chain = new TChain("t","fs_t1tttt_m1200_m650" );
  TChain *fs_t1tttt_1200_0_chain = new TChain("t","fs_t1tttt_m1200_m0" );
  TChain *fs_t1tttt_1200_750_chain = new TChain("t","fs_t1tttt_m1200_m750" );
  TChain *fs_t1tttt_1200_975_chain = new TChain("t","fs_t1tttt_m1200_m975" );
  TChain *fs_t1tttt_1225_825_chain = new TChain("t","fs_t1tttt_m1225_m825" );
  TChain *fs_t1tttt_1225_875_chain = new TChain("t","fs_t1tttt_m1225_m875" );
  TChain *fs_t1tttt_1225_850_chain = new TChain("t","fs_t1tttt_m1225_m850" );
  TChain *fs_t1tttt_1225_925_chain = new TChain("t","fs_t1tttt_m1225_m925" );
  TChain *fs_t1tttt_1225_800_chain = new TChain("t","fs_t1tttt_m1225_m800" );
  TChain *fs_t1tttt_1200_900_chain = new TChain("t","fs_t1tttt_m1200_m900" );
  TChain *fs_t1tttt_1200_925_chain = new TChain("t","fs_t1tttt_m1200_m925" );
  TChain *fs_t1tttt_1225_900_chain = new TChain("t","fs_t1tttt_m1225_m900" );
  TChain *fs_t1tttt_1200_950_chain = new TChain("t","fs_t1tttt_m1200_m950" );
  TChain *fs_t1tttt_1225_1000_chain = new TChain("t","fs_t1tttt_m1225_m1000" );
  TChain *fs_t1tttt_1200_875_chain = new TChain("t","fs_t1tttt_m1200_m875" );
  TChain *fs_t1tttt_1200_850_chain = new TChain("t","fs_t1tttt_m1200_m850" );
  TChain *fs_t1tttt_1275_1050_chain = new TChain("t","fs_t1tttt_m1275_m1050" );
  TChain *fs_t1tttt_1275_875_chain = new TChain("t","fs_t1tttt_m1275_m875" );
  TChain *fs_t1tttt_1275_850_chain = new TChain("t","fs_t1tttt_m1275_m850" );
  TChain *fs_t1tttt_1275_1000_chain = new TChain("t","fs_t1tttt_m1275_m1000" );
  TChain *fs_t1tttt_1275_1025_chain = new TChain("t","fs_t1tttt_m1275_m1025" );
  TChain *fs_t1tttt_1250_925_chain = new TChain("t","fs_t1tttt_m1250_m925" );
  TChain *fs_t1tttt_1250_950_chain = new TChain("t","fs_t1tttt_m1250_m950" );
  TChain *fs_t1tttt_1250_700_chain = new TChain("t","fs_t1tttt_m1250_m700" );
  TChain *fs_t1tttt_1250_750_chain = new TChain("t","fs_t1tttt_m1250_m750" );
  TChain *fs_t1tttt_1250_900_chain = new TChain("t","fs_t1tttt_m1250_m900" );
  TChain *fs_t1tttt_1250_800_chain = new TChain("t","fs_t1tttt_m1250_m800" );
  TChain *fs_t1tttt_1250_850_chain = new TChain("t","fs_t1tttt_m1250_m850" );
  TChain *fs_t1tttt_1250_975_chain = new TChain("t","fs_t1tttt_m1250_m975" );
  TChain *fs_t1tttt_1250_875_chain = new TChain("t","fs_t1tttt_m1250_m875" );
  TChain *fs_t1tttt_1275_925_chain = new TChain("t","fs_t1tttt_m1275_m925" );
  TChain *fs_t1tttt_1275_950_chain = new TChain("t","fs_t1tttt_m1275_m950" );
  TChain *fs_t1tttt_1275_975_chain = new TChain("t","fs_t1tttt_m1275_m975" );
  TChain *fs_t1tttt_1275_900_chain = new TChain("t","fs_t1tttt_m1275_m900" );
  TChain *fs_t1tttt_1300_1025_chain = new TChain("t","fs_t1tttt_m1300_m1025" );
  TChain *fs_t1tttt_1300_400_chain = new TChain("t","fs_t1tttt_m1300_m400" );
  TChain *fs_t1tttt_1300_500_chain = new TChain("t","fs_t1tttt_m1300_m500" );
  TChain *fs_t1tttt_1300_550_chain = new TChain("t","fs_t1tttt_m1300_m550" );
  TChain *fs_t1tttt_1300_50_chain = new TChain("t","fs_t1tttt_m1300_m50" );
  TChain *fs_t1tttt_1300_1075_chain = new TChain("t","fs_t1tttt_m1300_m1075" );
  TChain *fs_t1tttt_1300_600_chain = new TChain("t","fs_t1tttt_m1300_m600" );
  TChain *fs_t1tttt_1300_450_chain = new TChain("t","fs_t1tttt_m1300_m450" );
  TChain *fs_t1tttt_1300_0_chain = new TChain("t","fs_t1tttt_m1300_m0" );
  TChain *fs_t1tttt_1300_1000_chain = new TChain("t","fs_t1tttt_m1300_m1000" );
  TChain *fs_t1tttt_1300_1050_chain = new TChain("t","fs_t1tttt_m1300_m1050" );
  TChain *fs_t1tttt_1300_150_chain = new TChain("t","fs_t1tttt_m1300_m150" );
  TChain *fs_t1tttt_1300_100_chain = new TChain("t","fs_t1tttt_m1300_m100" );
  TChain *fs_t1tttt_1300_650_chain = new TChain("t","fs_t1tttt_m1300_m650" );
  TChain *fs_t1tttt_1300_300_chain = new TChain("t","fs_t1tttt_m1300_m300" );
  TChain *fs_t1tttt_1300_200_chain = new TChain("t","fs_t1tttt_m1300_m200" );
  TChain *fs_t1tttt_1300_350_chain = new TChain("t","fs_t1tttt_m1300_m350" );
  TChain *fs_t1tttt_1300_250_chain = new TChain("t","fs_t1tttt_m1300_m250" );
  TChain *fs_t1tttt_1300_800_chain = new TChain("t","fs_t1tttt_m1300_m800" );
  TChain *fs_t1tttt_1300_925_chain = new TChain("t","fs_t1tttt_m1300_m925" );
  TChain *fs_t1tttt_1300_950_chain = new TChain("t","fs_t1tttt_m1300_m950" );
  TChain *fs_t1tttt_1325_1075_chain = new TChain("t","fs_t1tttt_m1325_m1075" );
  TChain *fs_t1tttt_1300_900_chain = new TChain("t","fs_t1tttt_m1300_m900" );
  TChain *fs_t1tttt_1300_850_chain = new TChain("t","fs_t1tttt_m1300_m850" );
  TChain *fs_t1tttt_1325_1000_chain = new TChain("t","fs_t1tttt_m1325_m1000" );
  TChain *fs_t1tttt_1325_900_chain = new TChain("t","fs_t1tttt_m1325_m900" );
  TChain *fs_t1tttt_1325_1025_chain = new TChain("t","fs_t1tttt_m1325_m1025" );
  TChain *fs_t1tttt_1325_1050_chain = new TChain("t","fs_t1tttt_m1325_m1050" );
  TChain *fs_t1tttt_1300_975_chain = new TChain("t","fs_t1tttt_m1300_m975" );
  TChain *fs_t1tttt_1300_700_chain = new TChain("t","fs_t1tttt_m1300_m700" );
  TChain *fs_t1tttt_1300_750_chain = new TChain("t","fs_t1tttt_m1300_m750" );
  TChain *fs_t1tttt_1325_1100_chain = new TChain("t","fs_t1tttt_m1325_m1100" );
  TChain *fs_t1tttt_1350_350_chain = new TChain("t","fs_t1tttt_m1350_m350" );
  TChain *fs_t1tttt_1350_400_chain = new TChain("t","fs_t1tttt_m1350_m400" );
  TChain *fs_t1tttt_1350_550_chain = new TChain("t","fs_t1tttt_m1350_m550" );
  TChain *fs_t1tttt_1350_450_chain = new TChain("t","fs_t1tttt_m1350_m450" );
  TChain *fs_t1tttt_1350_500_chain = new TChain("t","fs_t1tttt_m1350_m500" );
  TChain *fs_t1tttt_1350_600_chain = new TChain("t","fs_t1tttt_m1350_m600" );
  TChain *fs_t1tttt_1375_1025_chain = new TChain("t","fs_t1tttt_m1375_m1025" );
  TChain *fs_t1tttt_1350_700_chain = new TChain("t","fs_t1tttt_m1350_m700" );
  TChain *fs_t1tttt_1350_750_chain = new TChain("t","fs_t1tttt_m1350_m750" );
  TChain *fs_t1tttt_1350_950_chain = new TChain("t","fs_t1tttt_m1350_m950" );
  TChain *fs_t1tttt_1350_975_chain = new TChain("t","fs_t1tttt_m1350_m975" );
  TChain *fs_t1tttt_1375_1000_chain = new TChain("t","fs_t1tttt_m1375_m1000" );
  TChain *fs_t1tttt_1350_50_chain = new TChain("t","fs_t1tttt_m1350_m50" );
  TChain *fs_t1tttt_1350_800_chain = new TChain("t","fs_t1tttt_m1350_m800" );
  TChain *fs_t1tttt_1350_850_chain = new TChain("t","fs_t1tttt_m1350_m850" );
  TChain *fs_t1tttt_1350_900_chain = new TChain("t","fs_t1tttt_m1350_m900" );
  TChain *fs_t1tttt_1350_650_chain = new TChain("t","fs_t1tttt_m1350_m650" );
  TChain *fs_t1tttt_1400_800_chain = new TChain("t","fs_t1tttt_m1400_m800" );
  TChain *fs_t1tttt_1400_950_chain = new TChain("t","fs_t1tttt_m1400_m950" );
  TChain *fs_t1tttt_1400_850_chain = new TChain("t","fs_t1tttt_m1400_m850" );
  TChain *fs_t1tttt_1400_900_chain = new TChain("t","fs_t1tttt_m1400_m900" );
  TChain *fs_t1tttt_1425_1050_chain = new TChain("t","fs_t1tttt_m1425_m1050" );
  TChain *fs_t1tttt_1425_1100_chain = new TChain("t","fs_t1tttt_m1425_m1100" );
  TChain *fs_t1tttt_1400_550_chain = new TChain("t","fs_t1tttt_m1400_m550" );
  TChain *fs_t1tttt_1400_750_chain = new TChain("t","fs_t1tttt_m1400_m750" );
  TChain *fs_t1tttt_1400_600_chain = new TChain("t","fs_t1tttt_m1400_m600" );
  TChain *fs_t1tttt_1425_1000_chain = new TChain("t","fs_t1tttt_m1425_m1000" );
  TChain *fs_t1tttt_1425_1075_chain = new TChain("t","fs_t1tttt_m1425_m1075" );
  TChain *fs_t1tttt_1425_1025_chain = new TChain("t","fs_t1tttt_m1425_m1025" );
  TChain *fs_t1tttt_1400_50_chain = new TChain("t","fs_t1tttt_m1400_m50" );
  TChain *fs_t1tttt_1400_700_chain = new TChain("t","fs_t1tttt_m1400_m700" );
  TChain *fs_t1tttt_1400_450_chain = new TChain("t","fs_t1tttt_m1400_m450" );
  TChain *fs_t1tttt_1400_500_chain = new TChain("t","fs_t1tttt_m1400_m500" );
  TChain *fs_t1tttt_1400_650_chain = new TChain("t","fs_t1tttt_m1400_m650" );
  TChain *fs_t1tttt_1425_1125_chain = new TChain("t","fs_t1tttt_m1425_m1125" );
  TChain *fs_t1tttt_1425_1150_chain = new TChain("t","fs_t1tttt_m1425_m1150" );
  TChain *fs_t1tttt_1425_1175_chain = new TChain("t","fs_t1tttt_m1425_m1175" );
  TChain *fs_t1tttt_1450_1075_chain = new TChain("t","fs_t1tttt_m1450_m1075" );
  TChain *fs_t1tttt_1450_1125_chain = new TChain("t","fs_t1tttt_m1450_m1125" );
  TChain *fs_t1tttt_1450_1150_chain = new TChain("t","fs_t1tttt_m1450_m1150" );
  TChain *fs_t1tttt_1450_1175_chain = new TChain("t","fs_t1tttt_m1450_m1175" );
  TChain *fs_t1tttt_1450_1100_chain = new TChain("t","fs_t1tttt_m1450_m1100" );
  TChain *fs_t1tttt_1450_1200_chain = new TChain("t","fs_t1tttt_m1450_m1200" );
  TChain *fs_t1tttt_1450_1050_chain = new TChain("t","fs_t1tttt_m1450_m1050" );
  TChain *fs_t1tttt_1450_0_chain = new TChain("t","fs_t1tttt_m1450_m0" );
  TChain *fs_t1tttt_1450_150_chain = new TChain("t","fs_t1tttt_m1450_m150" );
  TChain *fs_t1tttt_1450_200_chain = new TChain("t","fs_t1tttt_m1450_m200" );
  TChain *fs_t1tttt_1450_1000_chain = new TChain("t","fs_t1tttt_m1450_m1000" );
  TChain *fs_t1tttt_1450_100_chain = new TChain("t","fs_t1tttt_m1450_m100" );
  TChain *fs_t1tttt_1450_300_chain = new TChain("t","fs_t1tttt_m1450_m300" );
  TChain *fs_t1tttt_1475_1075_chain = new TChain("t","fs_t1tttt_m1475_m1075" );
  TChain *fs_t1tttt_1450_350_chain = new TChain("t","fs_t1tttt_m1450_m350" );
  TChain *fs_t1tttt_1450_600_chain = new TChain("t","fs_t1tttt_m1450_m600" );
  TChain *fs_t1tttt_1450_750_chain = new TChain("t","fs_t1tttt_m1450_m750" );
  TChain *fs_t1tttt_1475_1050_chain = new TChain("t","fs_t1tttt_m1475_m1050" );
  TChain *fs_t1tttt_1450_450_chain = new TChain("t","fs_t1tttt_m1450_m450" );
  TChain *fs_t1tttt_1450_650_chain = new TChain("t","fs_t1tttt_m1450_m650" );
  TChain *fs_t1tttt_1450_550_chain = new TChain("t","fs_t1tttt_m1450_m550" );
  TChain *fs_t1tttt_1450_700_chain = new TChain("t","fs_t1tttt_m1450_m700" );
  TChain *fs_t1tttt_1450_950_chain = new TChain("t","fs_t1tttt_m1450_m950" );
  TChain *fs_t1tttt_1450_800_chain = new TChain("t","fs_t1tttt_m1450_m800" );
  TChain *fs_t1tttt_1450_850_chain = new TChain("t","fs_t1tttt_m1450_m850" );
  TChain *fs_t1tttt_1450_250_chain = new TChain("t","fs_t1tttt_m1450_m250" );
  TChain *fs_t1tttt_1450_50_chain = new TChain("t","fs_t1tttt_m1450_m50" );
  TChain *fs_t1tttt_1450_400_chain = new TChain("t","fs_t1tttt_m1450_m400" );
  TChain *fs_t1tttt_1450_900_chain = new TChain("t","fs_t1tttt_m1450_m900" );
  TChain *fs_t1tttt_1450_500_chain = new TChain("t","fs_t1tttt_m1450_m500" );
  TChain *fs_t1tttt_1500_1125_chain = new TChain("t","fs_t1tttt_m1500_m1125" );
  TChain *fs_t1tttt_1500_1150_chain = new TChain("t","fs_t1tttt_m1500_m1150" );
  TChain *fs_t1tttt_1475_1100_chain = new TChain("t","fs_t1tttt_m1475_m1100" );
  TChain *fs_t1tttt_1475_1125_chain = new TChain("t","fs_t1tttt_m1475_m1125" );
  TChain *fs_t1tttt_1475_1150_chain = new TChain("t","fs_t1tttt_m1475_m1150" );
  TChain *fs_t1tttt_1500_1050_chain = new TChain("t","fs_t1tttt_m1500_m1050" );
  TChain *fs_t1tttt_1475_1175_chain = new TChain("t","fs_t1tttt_m1475_m1175" );
  TChain *fs_t1tttt_1500_0_chain = new TChain("t","fs_t1tttt_m1500_m0" );
  TChain *fs_t1tttt_1500_1000_chain = new TChain("t","fs_t1tttt_m1500_m1000" );
  TChain *fs_t1tttt_1500_1250_chain = new TChain("t","fs_t1tttt_m1500_m1250" );
  TChain *fs_t1tttt_1500_1175_chain = new TChain("t","fs_t1tttt_m1500_m1175" );
  TChain *fs_t1tttt_1500_1200_chain = new TChain("t","fs_t1tttt_m1500_m1200" );
  TChain *fs_t1tttt_1500_1100_chain = new TChain("t","fs_t1tttt_m1500_m1100" );
  TChain *fs_t1tttt_1500_150_chain = new TChain("t","fs_t1tttt_m1500_m150" );
  TChain *fs_t1tttt_1500_100_chain = new TChain("t","fs_t1tttt_m1500_m100" );
  TChain *fs_t1tttt_1500_350_chain = new TChain("t","fs_t1tttt_m1500_m350" );
  TChain *fs_t1tttt_1500_50_chain = new TChain("t","fs_t1tttt_m1500_m50" );
  TChain *fs_t1tttt_1500_700_chain = new TChain("t","fs_t1tttt_m1500_m700" );
  TChain *fs_t1tttt_1500_650_chain = new TChain("t","fs_t1tttt_m1500_m650" );
  TChain *fs_t1tttt_1500_600_chain = new TChain("t","fs_t1tttt_m1500_m600" );
  TChain *fs_t1tttt_1500_800_chain = new TChain("t","fs_t1tttt_m1500_m800" );
  TChain *fs_t1tttt_1500_850_chain = new TChain("t","fs_t1tttt_m1500_m850" );
  TChain *fs_t1tttt_1500_900_chain = new TChain("t","fs_t1tttt_m1500_m900" );
  TChain *fs_t1tttt_1500_950_chain = new TChain("t","fs_t1tttt_m1500_m950" );
  TChain *fs_t1tttt_1500_750_chain = new TChain("t","fs_t1tttt_m1500_m750" );
  TChain *fs_t1tttt_1525_1125_chain = new TChain("t","fs_t1tttt_m1525_m1125" );
  TChain *fs_t1tttt_1525_1100_chain = new TChain("t","fs_t1tttt_m1525_m1100" );
  TChain *fs_t1tttt_1500_250_chain = new TChain("t","fs_t1tttt_m1500_m250" );
  TChain *fs_t1tttt_1500_300_chain = new TChain("t","fs_t1tttt_m1500_m300" );
  TChain *fs_t1tttt_1500_400_chain = new TChain("t","fs_t1tttt_m1500_m400" );
  TChain *fs_t1tttt_1500_500_chain = new TChain("t","fs_t1tttt_m1500_m500" );
  TChain *fs_t1tttt_1500_550_chain = new TChain("t","fs_t1tttt_m1500_m550" );
  TChain *fs_t1tttt_1500_450_chain = new TChain("t","fs_t1tttt_m1500_m450" );
  TChain *fs_t1tttt_1500_200_chain = new TChain("t","fs_t1tttt_m1500_m200" );
  TChain *fs_t1tttt_1600_1050_chain = new TChain("t","fs_t1tttt_m1600_m1050" );
  TChain *fs_t1tttt_1600_1150_chain = new TChain("t","fs_t1tttt_m1600_m1150" );
  TChain *fs_t1tttt_1600_1250_chain = new TChain("t","fs_t1tttt_m1600_m1250" );
  TChain *fs_t1tttt_1600_1200_chain = new TChain("t","fs_t1tttt_m1600_m1200" );
  TChain *fs_t1tttt_1600_1100_chain = new TChain("t","fs_t1tttt_m1600_m1100" );
  TChain *fs_t1tttt_1600_150_chain = new TChain("t","fs_t1tttt_m1600_m150" );
  TChain *fs_t1tttt_1600_750_chain = new TChain("t","fs_t1tttt_m1600_m750" );
  TChain *fs_t1tttt_1600_800_chain = new TChain("t","fs_t1tttt_m1600_m800" );
  TChain *fs_t1tttt_1600_1000_chain = new TChain("t","fs_t1tttt_m1600_m1000" );
  TChain *fs_t1tttt_1600_100_chain = new TChain("t","fs_t1tttt_m1600_m100" );
  TChain *fs_t1tttt_1600_550_chain = new TChain("t","fs_t1tttt_m1600_m550" );
  TChain *fs_t1tttt_1650_1000_chain = new TChain("t","fs_t1tttt_m1650_m1000" );
  TChain *fs_t1tttt_1600_950_chain = new TChain("t","fs_t1tttt_m1600_m950" );
  TChain *fs_t1tttt_1600_600_chain = new TChain("t","fs_t1tttt_m1600_m600" );
  TChain *fs_t1tttt_1650_0_chain = new TChain("t","fs_t1tttt_m1650_m0" );
  TChain *fs_t1tttt_1650_100_chain = new TChain("t","fs_t1tttt_m1650_m100" );
  TChain *fs_t1tttt_1650_1050_chain = new TChain("t","fs_t1tttt_m1650_m1050" );
  TChain *fs_t1tttt_1600_850_chain = new TChain("t","fs_t1tttt_m1600_m850" );
  TChain *fs_t1tttt_1600_650_chain = new TChain("t","fs_t1tttt_m1600_m650" );
  TChain *fs_t1tttt_1600_900_chain = new TChain("t","fs_t1tttt_m1600_m900" );
  TChain *fs_t1tttt_1600_1300_chain = new TChain("t","fs_t1tttt_m1600_m1300" );
  TChain *fs_t1tttt_1600_0_chain = new TChain("t","fs_t1tttt_m1600_m0" );
  TChain *fs_t1tttt_1600_200_chain = new TChain("t","fs_t1tttt_m1600_m200" );
  TChain *fs_t1tttt_1600_250_chain = new TChain("t","fs_t1tttt_m1600_m250" );
  TChain *fs_t1tttt_1600_400_chain = new TChain("t","fs_t1tttt_m1600_m400" );
  TChain *fs_t1tttt_1600_450_chain = new TChain("t","fs_t1tttt_m1600_m450" );
  TChain *fs_t1tttt_1600_300_chain = new TChain("t","fs_t1tttt_m1600_m300" );
  TChain *fs_t1tttt_1600_350_chain = new TChain("t","fs_t1tttt_m1600_m350" );
  TChain *fs_t1tttt_1600_1350_chain = new TChain("t","fs_t1tttt_m1600_m1350" );
  TChain *fs_t1tttt_1600_700_chain = new TChain("t","fs_t1tttt_m1600_m700" );
  TChain *fs_t1tttt_1600_500_chain = new TChain("t","fs_t1tttt_m1600_m500" );
  TChain *fs_t1tttt_1600_50_chain = new TChain("t","fs_t1tttt_m1600_m50" );
  TChain *fs_t1tttt_1650_1100_chain = new TChain("t","fs_t1tttt_m1650_m1100" );
  TChain *fs_t1tttt_1650_150_chain = new TChain("t","fs_t1tttt_m1650_m150" );
  TChain *fs_t1tttt_1650_1200_chain = new TChain("t","fs_t1tttt_m1650_m1200" );
  TChain *fs_t1tttt_1650_200_chain = new TChain("t","fs_t1tttt_m1650_m200" );
  TChain *fs_t1tttt_1650_1250_chain = new TChain("t","fs_t1tttt_m1650_m1250" );
  TChain *fs_t1tttt_1650_700_chain = new TChain("t","fs_t1tttt_m1650_m700" );
  TChain *fs_t1tttt_1650_750_chain = new TChain("t","fs_t1tttt_m1650_m750" );
  TChain *fs_t1tttt_1650_1300_chain = new TChain("t","fs_t1tttt_m1650_m1300" );
  TChain *fs_t1tttt_1650_300_chain = new TChain("t","fs_t1tttt_m1650_m300" );
  TChain *fs_t1tttt_1650_900_chain = new TChain("t","fs_t1tttt_m1650_m900" );
  TChain *fs_t1tttt_1650_950_chain = new TChain("t","fs_t1tttt_m1650_m950" );
  TChain *fs_t1tttt_1650_350_chain = new TChain("t","fs_t1tttt_m1650_m350" );
  TChain *fs_t1tttt_1650_800_chain = new TChain("t","fs_t1tttt_m1650_m800" );
  TChain *fs_t1tttt_1650_650_chain = new TChain("t","fs_t1tttt_m1650_m650" );
  TChain *fs_t1tttt_1650_500_chain = new TChain("t","fs_t1tttt_m1650_m500" );
  TChain *fs_t1tttt_1700_1000_chain = new TChain("t","fs_t1tttt_m1700_m1000" );
  TChain *fs_t1tttt_1650_1400_chain = new TChain("t","fs_t1tttt_m1650_m1400" );
  TChain *fs_t1tttt_1650_50_chain = new TChain("t","fs_t1tttt_m1650_m50" );
  TChain *fs_t1tttt_1650_550_chain = new TChain("t","fs_t1tttt_m1650_m550" );
  TChain *fs_t1tttt_1700_100_chain = new TChain("t","fs_t1tttt_m1700_m100" );
  TChain *fs_t1tttt_1650_1350_chain = new TChain("t","fs_t1tttt_m1650_m1350" );
  TChain *fs_t1tttt_1650_850_chain = new TChain("t","fs_t1tttt_m1650_m850" );
  TChain *fs_t1tttt_1650_250_chain = new TChain("t","fs_t1tttt_m1650_m250" );
  TChain *fs_t1tttt_1650_400_chain = new TChain("t","fs_t1tttt_m1650_m400" );
  TChain *fs_t1tttt_1650_600_chain = new TChain("t","fs_t1tttt_m1650_m600" );
  TChain *fs_t1tttt_1700_0_chain = new TChain("t","fs_t1tttt_m1700_m0" );
  TChain *fs_t1tttt_1700_1050_chain = new TChain("t","fs_t1tttt_m1700_m1050" );
  TChain *fs_t1tttt_1700_1200_chain = new TChain("t","fs_t1tttt_m1700_m1200" );
  TChain *fs_t1tttt_1700_1100_chain = new TChain("t","fs_t1tttt_m1700_m1100" );
  TChain *fs_t1tttt_1650_450_chain = new TChain("t","fs_t1tttt_m1650_m450" );
  TChain *fs_t1tttt_1650_1150_chain = new TChain("t","fs_t1tttt_m1650_m1150" );
  TChain *fs_t1tttt_1700_1150_chain = new TChain("t","fs_t1tttt_m1700_m1150" );
  TChain *fs_t1tttt_1700_1450_chain = new TChain("t","fs_t1tttt_m1700_m1450" );
  TChain *fs_t1tttt_1700_350_chain = new TChain("t","fs_t1tttt_m1700_m350" );
  TChain *fs_t1tttt_1700_500_chain = new TChain("t","fs_t1tttt_m1700_m500" );
  TChain *fs_t1tttt_1700_250_chain = new TChain("t","fs_t1tttt_m1700_m250" );
  TChain *fs_t1tttt_1700_300_chain = new TChain("t","fs_t1tttt_m1700_m300" );
  TChain *fs_t1tttt_1700_450_chain = new TChain("t","fs_t1tttt_m1700_m450" );
  TChain *fs_t1tttt_1700_400_chain = new TChain("t","fs_t1tttt_m1700_m400" );
  TChain *fs_t1tttt_1700_550_chain = new TChain("t","fs_t1tttt_m1700_m550" );
  TChain *fs_t1tttt_1700_850_chain = new TChain("t","fs_t1tttt_m1700_m850" );
  TChain *fs_t1tttt_1750_1300_chain = new TChain("t","fs_t1tttt_m1750_m1300" );
  TChain *fs_t1tttt_1700_150_chain = new TChain("t","fs_t1tttt_m1700_m150" );
  TChain *fs_t1tttt_1700_200_chain = new TChain("t","fs_t1tttt_m1700_m200" );
  TChain *fs_t1tttt_1750_100_chain = new TChain("t","fs_t1tttt_m1750_m100" );
  TChain *fs_t1tttt_1750_1250_chain = new TChain("t","fs_t1tttt_m1750_m1250" );
  TChain *fs_t1tttt_1700_1350_chain = new TChain("t","fs_t1tttt_m1700_m1350" );
  TChain *fs_t1tttt_1700_1300_chain = new TChain("t","fs_t1tttt_m1700_m1300" );
  TChain *fs_t1tttt_1700_50_chain = new TChain("t","fs_t1tttt_m1700_m50" );
  TChain *fs_t1tttt_1700_1400_chain = new TChain("t","fs_t1tttt_m1700_m1400" );
  TChain *fs_t1tttt_1750_1150_chain = new TChain("t","fs_t1tttt_m1750_m1150" );
  TChain *fs_t1tttt_1750_1200_chain = new TChain("t","fs_t1tttt_m1750_m1200" );
  TChain *fs_t1tttt_1700_700_chain = new TChain("t","fs_t1tttt_m1700_m700" );
  TChain *fs_t1tttt_1700_650_chain = new TChain("t","fs_t1tttt_m1700_m650" );
  TChain *fs_t1tttt_1700_750_chain = new TChain("t","fs_t1tttt_m1700_m750" );
  TChain *fs_t1tttt_1750_1000_chain = new TChain("t","fs_t1tttt_m1750_m1000" );
  TChain *fs_t1tttt_1700_800_chain = new TChain("t","fs_t1tttt_m1700_m800" );
  TChain *fs_t1tttt_1700_950_chain = new TChain("t","fs_t1tttt_m1700_m950" );
  TChain *fs_t1tttt_1700_900_chain = new TChain("t","fs_t1tttt_m1700_m900" );
  TChain *fs_t1tttt_1750_0_chain = new TChain("t","fs_t1tttt_m1750_m0" );
  TChain *fs_t1tttt_1750_1050_chain = new TChain("t","fs_t1tttt_m1750_m1050" );
  TChain *fs_t1tttt_1700_600_chain = new TChain("t","fs_t1tttt_m1700_m600" );
  TChain *fs_t1tttt_1700_1250_chain = new TChain("t","fs_t1tttt_m1700_m1250" );
  TChain *fs_t1tttt_1750_1100_chain = new TChain("t","fs_t1tttt_m1750_m1100" );
  TChain *fs_t1tttt_1750_1350_chain = new TChain("t","fs_t1tttt_m1750_m1350" );
  TChain *fs_t1tttt_1750_450_chain = new TChain("t","fs_t1tttt_m1750_m450" );
  TChain *fs_t1tttt_1750_1400_chain = new TChain("t","fs_t1tttt_m1750_m1400" );
  TChain *fs_t1tttt_1750_500_chain = new TChain("t","fs_t1tttt_m1750_m500" );
  TChain *fs_t1tttt_1750_50_chain = new TChain("t","fs_t1tttt_m1750_m50" );
  TChain *fs_t1tttt_1750_1450_chain = new TChain("t","fs_t1tttt_m1750_m1450" );
  TChain *fs_t1tttt_1750_300_chain = new TChain("t","fs_t1tttt_m1750_m300" );
  TChain *fs_t1tttt_1750_350_chain = new TChain("t","fs_t1tttt_m1750_m350" );
  TChain *fs_t1tttt_1750_400_chain = new TChain("t","fs_t1tttt_m1750_m400" );
  TChain *fs_t1tttt_1750_600_chain = new TChain("t","fs_t1tttt_m1750_m600" );
  TChain *fs_t1tttt_1750_550_chain = new TChain("t","fs_t1tttt_m1750_m550" );
  TChain *fs_t1tttt_1750_850_chain = new TChain("t","fs_t1tttt_m1750_m850" );
  TChain *fs_t1tttt_1750_900_chain = new TChain("t","fs_t1tttt_m1750_m900" );
  TChain *fs_t1tttt_1750_700_chain = new TChain("t","fs_t1tttt_m1750_m700" );
  TChain *fs_t1tttt_1750_650_chain = new TChain("t","fs_t1tttt_m1750_m650" );
  TChain *fs_t1tttt_1750_800_chain = new TChain("t","fs_t1tttt_m1750_m800" );
  TChain *fs_t1tttt_1750_200_chain = new TChain("t","fs_t1tttt_m1750_m200" );
  TChain *fs_t1tttt_1750_950_chain = new TChain("t","fs_t1tttt_m1750_m950" );
  TChain *fs_t1tttt_1750_150_chain = new TChain("t","fs_t1tttt_m1750_m150" );
  TChain *fs_t1tttt_1750_250_chain = new TChain("t","fs_t1tttt_m1750_m250" );
  TChain *fs_t1tttt_1750_750_chain = new TChain("t","fs_t1tttt_m1750_m750" );
  TChain *fs_t1tttt_1850_1150_chain = new TChain("t","fs_t1tttt_m1850_m1150" );
  TChain *fs_t1tttt_1800_1300_chain = new TChain("t","fs_t1tttt_m1800_m1300" );
  TChain *fs_t1tttt_1800_1350_chain = new TChain("t","fs_t1tttt_m1800_m1350" );
  TChain *fs_t1tttt_1800_1400_chain = new TChain("t","fs_t1tttt_m1800_m1400" );
  TChain *fs_t1tttt_1850_100_chain = new TChain("t","fs_t1tttt_m1850_m100" );
  TChain *fs_t1tttt_1800_500_chain = new TChain("t","fs_t1tttt_m1800_m500" );
  TChain *fs_t1tttt_1800_950_chain = new TChain("t","fs_t1tttt_m1800_m950" );
  TChain *fs_t1tttt_1800_800_chain = new TChain("t","fs_t1tttt_m1800_m800" );
  TChain *fs_t1tttt_1800_550_chain = new TChain("t","fs_t1tttt_m1800_m550" );
  TChain *fs_t1tttt_1800_50_chain = new TChain("t","fs_t1tttt_m1800_m50" );
  TChain *fs_t1tttt_1850_0_chain = new TChain("t","fs_t1tttt_m1850_m0" );
  TChain *fs_t1tttt_1800_1150_chain = new TChain("t","fs_t1tttt_m1800_m1150" );
  TChain *fs_t1tttt_1800_1450_chain = new TChain("t","fs_t1tttt_m1800_m1450" );
  TChain *fs_t1tttt_1800_350_chain = new TChain("t","fs_t1tttt_m1800_m350" );
  TChain *fs_t1tttt_1800_1250_chain = new TChain("t","fs_t1tttt_m1800_m1250" );
  TChain *fs_t1tttt_1800_200_chain = new TChain("t","fs_t1tttt_m1800_m200" );
  TChain *fs_t1tttt_1800_1200_chain = new TChain("t","fs_t1tttt_m1800_m1200" );
  TChain *fs_t1tttt_1800_150_chain = new TChain("t","fs_t1tttt_m1800_m150" );
  TChain *fs_t1tttt_1800_300_chain = new TChain("t","fs_t1tttt_m1800_m300" );
  TChain *fs_t1tttt_1800_650_chain = new TChain("t","fs_t1tttt_m1800_m650" );
  TChain *fs_t1tttt_1850_1100_chain = new TChain("t","fs_t1tttt_m1850_m1100" );
  TChain *fs_t1tttt_1800_400_chain = new TChain("t","fs_t1tttt_m1800_m400" );
  TChain *fs_t1tttt_1800_450_chain = new TChain("t","fs_t1tttt_m1800_m450" );
  TChain *fs_t1tttt_1800_250_chain = new TChain("t","fs_t1tttt_m1800_m250" );
  TChain *fs_t1tttt_1800_1000_chain = new TChain("t","fs_t1tttt_m1800_m1000" );
  TChain *fs_t1tttt_1800_1050_chain = new TChain("t","fs_t1tttt_m1800_m1050" );
  TChain *fs_t1tttt_1800_1100_chain = new TChain("t","fs_t1tttt_m1800_m1100" );
  TChain *fs_t1tttt_1800_100_chain = new TChain("t","fs_t1tttt_m1800_m100" );
  TChain *fs_t1tttt_1800_0_chain = new TChain("t","fs_t1tttt_m1800_m0" );
  TChain *fs_t1tttt_1800_700_chain = new TChain("t","fs_t1tttt_m1800_m700" );
  TChain *fs_t1tttt_1800_600_chain = new TChain("t","fs_t1tttt_m1800_m600" );
  TChain *fs_t1tttt_1800_850_chain = new TChain("t","fs_t1tttt_m1800_m850" );
  TChain *fs_t1tttt_1800_900_chain = new TChain("t","fs_t1tttt_m1800_m900" );
  TChain *fs_t1tttt_1850_1000_chain = new TChain("t","fs_t1tttt_m1850_m1000" );
  TChain *fs_t1tttt_1850_1050_chain = new TChain("t","fs_t1tttt_m1850_m1050" );
  TChain *fs_t1tttt_1800_750_chain = new TChain("t","fs_t1tttt_m1800_m750" );
  TChain *fs_t1tttt_1850_150_chain = new TChain("t","fs_t1tttt_m1850_m150" );
  TChain *fs_t1tttt_1850_350_chain = new TChain("t","fs_t1tttt_m1850_m350" );
  TChain *fs_t1tttt_1850_550_chain = new TChain("t","fs_t1tttt_m1850_m550" );
  TChain *fs_t1tttt_1850_400_chain = new TChain("t","fs_t1tttt_m1850_m400" );
  TChain *fs_t1tttt_1850_450_chain = new TChain("t","fs_t1tttt_m1850_m450" );
  TChain *fs_t1tttt_1850_600_chain = new TChain("t","fs_t1tttt_m1850_m600" );
  TChain *fs_t1tttt_1850_200_chain = new TChain("t","fs_t1tttt_m1850_m200" );
  TChain *fs_t1tttt_1850_650_chain = new TChain("t","fs_t1tttt_m1850_m650" );
  TChain *fs_t1tttt_1900_1150_chain = new TChain("t","fs_t1tttt_m1900_m1150" );
  TChain *fs_t1tttt_1900_1200_chain = new TChain("t","fs_t1tttt_m1900_m1200" );
  TChain *fs_t1tttt_1900_200_chain = new TChain("t","fs_t1tttt_m1900_m200" );
  TChain *fs_t1tttt_1850_900_chain = new TChain("t","fs_t1tttt_m1850_m900" );
  TChain *fs_t1tttt_1900_1350_chain = new TChain("t","fs_t1tttt_m1900_m1350" );
  TChain *fs_t1tttt_1850_950_chain = new TChain("t","fs_t1tttt_m1850_m950" );
  TChain *fs_t1tttt_1900_1400_chain = new TChain("t","fs_t1tttt_m1900_m1400" );
  TChain *fs_t1tttt_1900_1450_chain = new TChain("t","fs_t1tttt_m1900_m1450" );
  TChain *fs_t1tttt_1900_150_chain = new TChain("t","fs_t1tttt_m1900_m150" );
  TChain *fs_t1tttt_1900_100_chain = new TChain("t","fs_t1tttt_m1900_m100" );
  TChain *fs_t1tttt_1900_1050_chain = new TChain("t","fs_t1tttt_m1900_m1050" );
  TChain *fs_t1tttt_1900_1250_chain = new TChain("t","fs_t1tttt_m1900_m1250" );
  TChain *fs_t1tttt_1900_1300_chain = new TChain("t","fs_t1tttt_m1900_m1300" );
  TChain *fs_t1tttt_1900_0_chain = new TChain("t","fs_t1tttt_m1900_m0" );
  TChain *fs_t1tttt_1900_1000_chain = new TChain("t","fs_t1tttt_m1900_m1000" );
  TChain *fs_t1tttt_1850_1200_chain = new TChain("t","fs_t1tttt_m1850_m1200" );
  TChain *fs_t1tttt_1850_1300_chain = new TChain("t","fs_t1tttt_m1850_m1300" );
  TChain *fs_t1tttt_1850_1450_chain = new TChain("t","fs_t1tttt_m1850_m1450" );
  TChain *fs_t1tttt_1850_500_chain = new TChain("t","fs_t1tttt_m1850_m500" );
  TChain *fs_t1tttt_1900_1100_chain = new TChain("t","fs_t1tttt_m1900_m1100" );
  TChain *fs_t1tttt_1850_1350_chain = new TChain("t","fs_t1tttt_m1850_m1350" );
  TChain *fs_t1tttt_1850_1400_chain = new TChain("t","fs_t1tttt_m1850_m1400" );
  TChain *fs_t1tttt_1850_1250_chain = new TChain("t","fs_t1tttt_m1850_m1250" );
  TChain *fs_t1tttt_1850_50_chain = new TChain("t","fs_t1tttt_m1850_m50" );
  TChain *fs_t1tttt_1850_250_chain = new TChain("t","fs_t1tttt_m1850_m250" );
  TChain *fs_t1tttt_1850_850_chain = new TChain("t","fs_t1tttt_m1850_m850" );
  TChain *fs_t1tttt_1850_700_chain = new TChain("t","fs_t1tttt_m1850_m700" );
  TChain *fs_t1tttt_1850_800_chain = new TChain("t","fs_t1tttt_m1850_m800" );
  TChain *fs_t1tttt_1850_750_chain = new TChain("t","fs_t1tttt_m1850_m750" );
  TChain *fs_t1tttt_1850_300_chain = new TChain("t","fs_t1tttt_m1850_m300" );
  TChain *fs_t1tttt_600_250_chain = new TChain("t","fs_t1tttt_m600_m250" );
  TChain *fs_t1tttt_600_300_chain = new TChain("t","fs_t1tttt_m600_m300" );
  TChain *fs_t1tttt_600_325_chain = new TChain("t","fs_t1tttt_m600_m325" );
  TChain *fs_t1tttt_600_275_chain = new TChain("t","fs_t1tttt_m600_m275" );
  TChain *fs_t1tttt_625_325_chain = new TChain("t","fs_t1tttt_m625_m325" );
  TChain *fs_t1tttt_625_350_chain = new TChain("t","fs_t1tttt_m625_m350" );
  TChain *fs_t1tttt_625_375_chain = new TChain("t","fs_t1tttt_m625_m375" );
  TChain *fs_t1tttt_625_275_chain = new TChain("t","fs_t1tttt_m625_m275" );
  TChain *fs_t1tttt_625_300_chain = new TChain("t","fs_t1tttt_m625_m300" );
  TChain *fs_t1tttt_650_200_chain = new TChain("t","fs_t1tttt_m650_m200" );
  TChain *fs_t1tttt_650_250_chain = new TChain("t","fs_t1tttt_m650_m250" );
  TChain *fs_t1tttt_650_325_chain = new TChain("t","fs_t1tttt_m650_m325" );
  TChain *fs_t1tttt_625_400_chain = new TChain("t","fs_t1tttt_m625_m400" );
  TChain *fs_t1tttt_650_275_chain = new TChain("t","fs_t1tttt_m650_m275" );
  TChain *fs_t1tttt_650_300_chain = new TChain("t","fs_t1tttt_m650_m300" );
  TChain *fs_t1tttt_650_425_chain = new TChain("t","fs_t1tttt_m650_m425" );
  TChain *fs_t1tttt_675_300_chain = new TChain("t","fs_t1tttt_m675_m300" );
  TChain *fs_t1tttt_675_275_chain = new TChain("t","fs_t1tttt_m675_m275" );
  TChain *fs_t1tttt_675_250_chain = new TChain("t","fs_t1tttt_m675_m250" );
  TChain *fs_t1tttt_650_350_chain = new TChain("t","fs_t1tttt_m650_m350" );
  TChain *fs_t1tttt_650_375_chain = new TChain("t","fs_t1tttt_m650_m375" );
  TChain *fs_t1tttt_650_400_chain = new TChain("t","fs_t1tttt_m650_m400" );
  TChain *fs_t1tttt_675_400_chain = new TChain("t","fs_t1tttt_m675_m400" );
  TChain *fs_t1tttt_675_450_chain = new TChain("t","fs_t1tttt_m675_m450" );
  TChain *fs_t1tttt_675_425_chain = new TChain("t","fs_t1tttt_m675_m425" );
  TChain *fs_t1tttt_675_375_chain = new TChain("t","fs_t1tttt_m675_m375" );
  TChain *fs_t1tttt_675_350_chain = new TChain("t","fs_t1tttt_m675_m350" );
  TChain *fs_t1tttt_675_325_chain = new TChain("t","fs_t1tttt_m675_m325" );
  TChain *fs_t1tttt_700_250_chain = new TChain("t","fs_t1tttt_m700_m250" );
  TChain *fs_t1tttt_700_300_chain = new TChain("t","fs_t1tttt_m700_m300" );
  TChain *fs_t1tttt_700_450_chain = new TChain("t","fs_t1tttt_m700_m450" );
  TChain *fs_t1tttt_700_200_chain = new TChain("t","fs_t1tttt_m700_m200" );
  TChain *fs_t1tttt_700_400_chain = new TChain("t","fs_t1tttt_m700_m400" );
  TChain *fs_t1tttt_700_425_chain = new TChain("t","fs_t1tttt_m700_m425" );
  TChain *fs_t1tttt_700_0_chain = new TChain("t","fs_t1tttt_m700_m0" );
  TChain *fs_t1tttt_700_325_chain = new TChain("t","fs_t1tttt_m700_m325" );
  TChain *fs_t1tttt_700_100_chain = new TChain("t","fs_t1tttt_m700_m100" );
  TChain *fs_t1tttt_700_375_chain = new TChain("t","fs_t1tttt_m700_m375" );
  TChain *fs_t1tttt_700_350_chain = new TChain("t","fs_t1tttt_m700_m350" );
  TChain *fs_t1tttt_725_425_chain = new TChain("t","fs_t1tttt_m725_m425" );
  TChain *fs_t1tttt_725_450_chain = new TChain("t","fs_t1tttt_m725_m450" );
  TChain *fs_t1tttt_725_475_chain = new TChain("t","fs_t1tttt_m725_m475" );
  TChain *fs_t1tttt_700_475_chain = new TChain("t","fs_t1tttt_m700_m475" );
  TChain *fs_t1tttt_725_375_chain = new TChain("t","fs_t1tttt_m725_m375" );
  TChain *fs_t1tttt_725_300_chain = new TChain("t","fs_t1tttt_m725_m300" );
  TChain *fs_t1tttt_725_350_chain = new TChain("t","fs_t1tttt_m725_m350" );
  TChain *fs_t1tttt_750_200_chain = new TChain("t","fs_t1tttt_m750_m200" );
  TChain *fs_t1tttt_750_250_chain = new TChain("t","fs_t1tttt_m750_m250" );
  TChain *fs_t1tttt_725_500_chain = new TChain("t","fs_t1tttt_m725_m500" );
  TChain *fs_t1tttt_750_300_chain = new TChain("t","fs_t1tttt_m750_m300" );
  TChain *fs_t1tttt_725_325_chain = new TChain("t","fs_t1tttt_m725_m325" );
  TChain *fs_t1tttt_725_400_chain = new TChain("t","fs_t1tttt_m725_m400" );
  TChain *fs_t1tttt_775_375_chain = new TChain("t","fs_t1tttt_m775_m375" );
  TChain *fs_t1tttt_775_425_chain = new TChain("t","fs_t1tttt_m775_m425" );
  TChain *fs_t1tttt_775_450_chain = new TChain("t","fs_t1tttt_m775_m450" );
  TChain *fs_t1tttt_775_400_chain = new TChain("t","fs_t1tttt_m775_m400" );
  TChain *fs_t1tttt_775_350_chain = new TChain("t","fs_t1tttt_m775_m350" );
  TChain *fs_t1tttt_750_500_chain = new TChain("t","fs_t1tttt_m750_m500" );
  TChain *fs_t1tttt_750_525_chain = new TChain("t","fs_t1tttt_m750_m525" );
  TChain *fs_t1tttt_750_375_chain = new TChain("t","fs_t1tttt_m750_m375" );
  TChain *fs_t1tttt_750_450_chain = new TChain("t","fs_t1tttt_m750_m450" );
  TChain *fs_t1tttt_750_475_chain = new TChain("t","fs_t1tttt_m750_m475" );
  TChain *fs_t1tttt_750_425_chain = new TChain("t","fs_t1tttt_m750_m425" );
  TChain *fs_t1tttt_750_350_chain = new TChain("t","fs_t1tttt_m750_m350" );
  TChain *fs_t1tttt_750_400_chain = new TChain("t","fs_t1tttt_m750_m400" );
  TChain *fs_t1tttt_775_500_chain = new TChain("t","fs_t1tttt_m775_m500" );
  TChain *fs_t1tttt_775_550_chain = new TChain("t","fs_t1tttt_m775_m550" );
  TChain *fs_t1tttt_775_525_chain = new TChain("t","fs_t1tttt_m775_m525" );
  TChain *fs_t1tttt_775_475_chain = new TChain("t","fs_t1tttt_m775_m475" );
  TChain *fs_t1tttt_800_400_chain = new TChain("t","fs_t1tttt_m800_m400" );
  TChain *fs_t1tttt_800_425_chain = new TChain("t","fs_t1tttt_m800_m425" );
  TChain *fs_t1tttt_800_525_chain = new TChain("t","fs_t1tttt_m800_m525" );
  TChain *fs_t1tttt_800_0_chain = new TChain("t","fs_t1tttt_m800_m0" );
  TChain *fs_t1tttt_800_250_chain = new TChain("t","fs_t1tttt_m800_m250" );
  TChain *fs_t1tttt_800_350_chain = new TChain("t","fs_t1tttt_m800_m350" );
  TChain *fs_t1tttt_800_200_chain = new TChain("t","fs_t1tttt_m800_m200" );
  TChain *fs_t1tttt_800_300_chain = new TChain("t","fs_t1tttt_m800_m300" );
  TChain *fs_t1tttt_800_575_chain = new TChain("t","fs_t1tttt_m800_m575" );
  TChain *fs_t1tttt_800_450_chain = new TChain("t","fs_t1tttt_m800_m450" );
  TChain *fs_t1tttt_800_500_chain = new TChain("t","fs_t1tttt_m800_m500" );
  TChain *fs_t1tttt_800_550_chain = new TChain("t","fs_t1tttt_m800_m550" );
  TChain *fs_t1tttt_800_475_chain = new TChain("t","fs_t1tttt_m800_m475" );
  TChain *fs_t1tttt_825_400_chain = new TChain("t","fs_t1tttt_m825_m400" );
  TChain *fs_t1tttt_800_100_chain = new TChain("t","fs_t1tttt_m800_m100" );
  TChain *fs_t1tttt_825_525_chain = new TChain("t","fs_t1tttt_m825_m525" );
  TChain *fs_t1tttt_825_550_chain = new TChain("t","fs_t1tttt_m825_m550" );
  TChain *fs_t1tttt_850_400_chain = new TChain("t","fs_t1tttt_m850_m400" );
  TChain *fs_t1tttt_850_250_chain = new TChain("t","fs_t1tttt_m850_m250" );
  TChain *fs_t1tttt_850_475_chain = new TChain("t","fs_t1tttt_m850_m475" );
  TChain *fs_t1tttt_825_500_chain = new TChain("t","fs_t1tttt_m825_m500" );
  TChain *fs_t1tttt_825_575_chain = new TChain("t","fs_t1tttt_m825_m575" );
  TChain *fs_t1tttt_850_350_chain = new TChain("t","fs_t1tttt_m850_m350" );
  TChain *fs_t1tttt_850_450_chain = new TChain("t","fs_t1tttt_m850_m450" );
  TChain *fs_t1tttt_850_300_chain = new TChain("t","fs_t1tttt_m850_m300" );
  TChain *fs_t1tttt_825_450_chain = new TChain("t","fs_t1tttt_m825_m450" );
  TChain *fs_t1tttt_825_425_chain = new TChain("t","fs_t1tttt_m825_m425" );
  TChain *fs_t1tttt_825_475_chain = new TChain("t","fs_t1tttt_m825_m475" );
  TChain *fs_t1tttt_850_200_chain = new TChain("t","fs_t1tttt_m850_m200" );
  TChain *fs_t1tttt_825_600_chain = new TChain("t","fs_t1tttt_m825_m600" );
  TChain *fs_t1tttt_850_575_chain = new TChain("t","fs_t1tttt_m850_m575" );
  TChain *fs_t1tttt_850_625_chain = new TChain("t","fs_t1tttt_m850_m625" );
  TChain *fs_t1tttt_875_550_chain = new TChain("t","fs_t1tttt_m875_m550" );
  TChain *fs_t1tttt_850_600_chain = new TChain("t","fs_t1tttt_m850_m600" );
  TChain *fs_t1tttt_875_475_chain = new TChain("t","fs_t1tttt_m875_m475" );
  TChain *fs_t1tttt_875_575_chain = new TChain("t","fs_t1tttt_m875_m575" );
  TChain *fs_t1tttt_875_600_chain = new TChain("t","fs_t1tttt_m875_m600" );
  TChain *fs_t1tttt_875_525_chain = new TChain("t","fs_t1tttt_m875_m525" );
  TChain *fs_t1tttt_850_525_chain = new TChain("t","fs_t1tttt_m850_m525" );
  TChain *fs_t1tttt_875_450_chain = new TChain("t","fs_t1tttt_m875_m450" );
  TChain *fs_t1tttt_850_550_chain = new TChain("t","fs_t1tttt_m850_m550" );
  TChain *fs_t1tttt_850_500_chain = new TChain("t","fs_t1tttt_m850_m500" );
  TChain *fs_t1tttt_875_500_chain = new TChain("t","fs_t1tttt_m875_m500" );
  TChain *fs_t1tttt_900_200_chain = new TChain("t","fs_t1tttt_m900_m200" );
  TChain *fs_t1tttt_900_300_chain = new TChain("t","fs_t1tttt_m900_m300" );
  TChain *fs_t1tttt_900_575_chain = new TChain("t","fs_t1tttt_m900_m575" );
  TChain *fs_t1tttt_900_350_chain = new TChain("t","fs_t1tttt_m900_m350" );
  TChain *fs_t1tttt_900_600_chain = new TChain("t","fs_t1tttt_m900_m600" );
  TChain *fs_t1tttt_900_550_chain = new TChain("t","fs_t1tttt_m900_m550" );
  TChain *fs_t1tttt_900_625_chain = new TChain("t","fs_t1tttt_m900_m625" );
  TChain *fs_t1tttt_875_625_chain = new TChain("t","fs_t1tttt_m875_m625" );
  TChain *fs_t1tttt_900_525_chain = new TChain("t","fs_t1tttt_m900_m525" );
  TChain *fs_t1tttt_900_500_chain = new TChain("t","fs_t1tttt_m900_m500" );
  TChain *fs_t1tttt_875_650_chain = new TChain("t","fs_t1tttt_m875_m650" );
  TChain *fs_t1tttt_900_450_chain = new TChain("t","fs_t1tttt_m900_m450" );
  TChain *fs_t1tttt_900_400_chain = new TChain("t","fs_t1tttt_m900_m400" );
  TChain *fs_t1tttt_900_0_chain = new TChain("t","fs_t1tttt_m900_m0" );
  TChain *fs_t1tttt_900_100_chain = new TChain("t","fs_t1tttt_m900_m100" );
  TChain *fs_t1tttt_900_250_chain = new TChain("t","fs_t1tttt_m900_m250" );
  TChain *fs_t1tttt_950_450_chain = new TChain("t","fs_t1tttt_m950_m450" );
  TChain *fs_t1tttt_950_500_chain = new TChain("t","fs_t1tttt_m950_m500" );
  TChain *fs_t1tttt_950_550_chain = new TChain("t","fs_t1tttt_m950_m550" );
  TChain *fs_t1tttt_950_675_chain = new TChain("t","fs_t1tttt_m950_m675" );
  TChain *fs_t1tttt_975_575_chain = new TChain("t","fs_t1tttt_m975_m575" );
  TChain *fs_t1tttt_950_625_chain = new TChain("t","fs_t1tttt_m950_m625" );
  TChain *fs_t1tttt_950_700_chain = new TChain("t","fs_t1tttt_m950_m700" );
  TChain *fs_t1tttt_975_550_chain = new TChain("t","fs_t1tttt_m975_m550" );
  TChain *fs_t1tttt_950_575_chain = new TChain("t","fs_t1tttt_m950_m575" );
  TChain *fs_t1tttt_950_350_chain = new TChain("t","fs_t1tttt_m950_m350" );
  TChain *fs_t1tttt_950_725_chain = new TChain("t","fs_t1tttt_m950_m725" );
  TChain *fs_t1tttt_950_600_chain = new TChain("t","fs_t1tttt_m950_m600" );
  TChain *fs_t1tttt_950_650_chain = new TChain("t","fs_t1tttt_m950_m650" );
  TChain *fs_t1tttt_950_400_chain = new TChain("t","fs_t1tttt_m950_m400" );
  TChain *fs_t1tttt_975_650_chain = new TChain("t","fs_t1tttt_m975_m650" );
  TChain *fs_t1tttt_975_700_chain = new TChain("t","fs_t1tttt_m975_m700" );
  TChain *fs_t1tttt_975_725_chain = new TChain("t","fs_t1tttt_m975_m725" );
  TChain *fs_t1tttt_975_675_chain = new TChain("t","fs_t1tttt_m975_m675" );
  TChain *fs_t1tttt_975_750_chain = new TChain("t","fs_t1tttt_m975_m750" );
  TChain *fs_t1tttt_975_600_chain = new TChain("t","fs_t1tttt_m975_m600" );
  TChain *fs_t1tttt_975_625_chain = new TChain("t","fs_t1tttt_m975_m625" );
  TChain *fs_t1tttt_1950_700_chain = new TChain("t","fs_t1tttt_m1950_m700" );
  TChain *fs_t1tttt_1950_850_chain = new TChain("t","fs_t1tttt_m1950_m850" );
  TChain *fs_t1tttt_1950_750_chain = new TChain("t","fs_t1tttt_m1950_m750" );
  TChain *fs_t1tttt_1950_950_chain = new TChain("t","fs_t1tttt_m1950_m950" );
  TChain *fs_t1tttt_1950_800_chain = new TChain("t","fs_t1tttt_m1950_m800" );
  TChain *fs_t1tttt_1950_900_chain = new TChain("t","fs_t1tttt_m1950_m900" );
  
  //Fill chains
  ttbar_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTBAR_PH.root"       , tag.c_str())); 
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP1.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP2.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP3.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP4.root"     , tag.c_str()));
  st_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/SINGLETOP5.root"     , tag.c_str()));
  wjets_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WJets_LO.root"       , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_high.root"        , tag.c_str()));
  dy_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DY_low.root"         , tag.c_str()));

  ttw_chain    ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"            , tag.c_str())); 
  ttzh_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"           , tag.c_str())); 
  ttzh_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZLOW.root"         , tag.c_str())); 
  ttzh_chain   ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTHtoNonBB.root"     , tag.c_str()));
  wz_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ.root"             , tag.c_str()));
  ww_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"           , tag.c_str()));
  xg_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TG.root"             , tag.c_str()));
  xg_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTG.root"            , tag.c_str()));
  xg_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WGToLNuG.root"       , tag.c_str()));
  xg_chain     ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/ZG.root"             , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/ZZ.root"             , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/GGHtoZZto4L.root"    , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WWZ.root"            , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZZ.root"            , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/VHtoNonBB.root"      , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TZQ.root"            , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTTT.root"           , tag.c_str()));
  rares_chain  ->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WWDPS.root"          , tag.c_str()));

  //data
  data_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGC_05oct.root"    , tag.c_str()));
  data_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonC_05oct.root"  , tag.c_str()));
  data_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGC_05oct.root"      , tag.c_str()));
  data_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_05oct.root"    , tag.c_str()));
  data_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_05oct.root"  , tag.c_str()));
  data_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_05oct.root"      , tag.c_str()));
  data_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_v4.root"       , tag.c_str()));
  data_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_v4.root"     , tag.c_str()));
  data_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_v4.root"         , tag.c_str()));

  //flips
  flips_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGC_05oct.root"   , tag.c_str()));
  flips_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGC_05oct.root"     , tag.c_str()));
  flips_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_05oct.root"   , tag.c_str()));
  flips_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_05oct.root"     , tag.c_str()));
  flips_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_v4.root"      , tag.c_str()));
  flips_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_v4.root"        , tag.c_str()));

  //fakes
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGC_05oct.root"   , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonC_05oct.root" , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGC_05oct.root"     , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_05oct.root"   , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_05oct.root" , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_05oct.root"     , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleEGD_v4.root"      , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataDoubleMuonD_v4.root"    , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/DataMuonEGD_v4.root"        , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTW.root"                   , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTZL.root"                  , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/WZ.root"                    , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/TTHtoNonBB.root"            , tag.c_str()));
  fakes_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/QQWW.root"                  , tag.c_str()));

  //signals fullsim
  t1tttt_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T1TTTT_1200.root"              , tag.c_str()));
  t1tttt_1500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T1TTTT_1500.root"              , tag.c_str()));
  t5tttt_deg_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T5ttttDeg_1000_300_285_280.root", tag.c_str()));
  t5qqqq_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T5qqqqWW_1200_1000_800.root"     , tag.c_str()));
  t6ttww_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T6TTWW_650_150_50.root"         , tag.c_str()));
  t6ttww_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T6TTWW_600_425_50.root"         , tag.c_str()));
  t5qqqqww_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T5qqqqWW_1200_1000_800.root"   , tag.c_str()));
  t5qqqqww_deg_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s/T5qqqqWWDeg_1000_315_300.root"   , tag.c_str()));

  //signals fast sim
  fs_t1tttt_1100_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_775_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_725_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1100_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100_1to775.root", tag.c_str()));
  fs_t1tttt_1125_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1125_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1125_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1125_875_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1100_825_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1100_875_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1100_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1125_825_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1125_725_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1125_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1100_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1125_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1125_775_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1100to1125_700to900.root", tag.c_str()));
  fs_t1tttt_1150_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_775_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1150_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150_1to800.root", tag.c_str()));
  fs_t1tttt_1175_925_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1175_775_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1175_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1175_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1175_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1150_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1175_875_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1175_825_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1150_925_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1150_825_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1150_875_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1150_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1175_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1150to1175_750to925.root", tag.c_str()));
  fs_t1tttt_1175_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1175_950.root", tag.c_str()));
  fs_t1tttt_1200_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_825_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200_1to825.root", tag.c_str()));
  fs_t1tttt_1200_975_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1225_825_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1225_875_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1225_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1225_925_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1225_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1200_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1200_925_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1225_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1200_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1225_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1200_875_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1200_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1200to1225_800to1000.root", tag.c_str()));
  fs_t1tttt_1275_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1275_875_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1275_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1275_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1275_1025_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1250_925_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1250_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1250_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1250_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1250_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1250_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1250_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1250_975_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1250_875_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1250to1275_700to1050.root", tag.c_str()));
  fs_t1tttt_1275_925_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1275_900to975.root", tag.c_str()));
  fs_t1tttt_1275_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1275_900to975.root", tag.c_str()));
  fs_t1tttt_1275_975_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1275_900to975.root", tag.c_str()));
  fs_t1tttt_1275_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1275_900to975.root", tag.c_str()));
  fs_t1tttt_1300_1025_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_1075_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300_1to1075.root", tag.c_str()));
  fs_t1tttt_1300_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1300_925_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1300_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1325_1075_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1300_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1300_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1325_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1325_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1325_1025_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1325_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1300_975_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1300_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1300_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1325_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1300to1325_700to1100.root", tag.c_str()));
  fs_t1tttt_1350_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1375_1025_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_975_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1375_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1350_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1350to1375_50to1025.root", tag.c_str()));
  fs_t1tttt_1400_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1425_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1425_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1425_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1425_1075_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1425_1025_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1400_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1400to1425_50to1100.root", tag.c_str()));
  fs_t1tttt_1425_1125_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1425_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1425_1175_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_1075_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_1125_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_1175_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1425to1450_1to1200.root", tag.c_str()));
  fs_t1tttt_1450_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1475_1075_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1475_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1450_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1450to1475_50to1075.root", tag.c_str()));
  fs_t1tttt_1500_1125_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1475_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1475_1125_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1475_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1475_1175_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_1250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_1175_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1475to1500_1to1250.root", tag.c_str()));
  fs_t1tttt_1500_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1525_1125_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1525_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1500_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1500to1525_50to1125.root", tag.c_str()));
  fs_t1tttt_1600_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_1250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1650_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1650_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1650_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1650_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_1300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_1350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1600_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1600to1650_1to1350.root", tag.c_str()));
  fs_t1tttt_1650_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_1250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_1300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1700_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_1400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1700_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_1350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1700_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1700_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1700_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1700_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1650_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1700_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1650to1700_1to1400.root", tag.c_str()));
  fs_t1tttt_1700_1450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_1300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_1250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_1350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_1300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_1400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1700_1250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1700to1750_1to1450.root", tag.c_str()));
  fs_t1tttt_1750_1350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_1400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_1450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1750_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1750_50to1450.root", tag.c_str()));
  fs_t1tttt_1850_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1800_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1800to1850_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_150_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1050_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1000_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_1200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_1300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_1450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1900_1100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_1350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_1400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_1250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_50_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_1850_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1850to1900_1to1450.root", tag.c_str()));
  fs_t1tttt_600_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_600_250to325.root", tag.c_str()));
  fs_t1tttt_600_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_600_250to325.root", tag.c_str()));
  fs_t1tttt_600_325_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_600_250to325.root", tag.c_str()));
  fs_t1tttt_600_275_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_600_250to325.root", tag.c_str()));
  fs_t1tttt_625_325_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625_275to375.root", tag.c_str()));
  fs_t1tttt_625_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625_275to375.root", tag.c_str()));
  fs_t1tttt_625_375_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625_275to375.root", tag.c_str()));
  fs_t1tttt_625_275_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625_275to375.root", tag.c_str()));
  fs_t1tttt_625_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625_275to375.root", tag.c_str()));
  fs_t1tttt_650_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625to650_200to400.root", tag.c_str()));
  fs_t1tttt_650_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625to650_200to400.root", tag.c_str()));
  fs_t1tttt_650_325_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625to650_200to400.root", tag.c_str()));
  fs_t1tttt_625_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625to650_200to400.root", tag.c_str()));
  fs_t1tttt_650_275_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625to650_200to400.root", tag.c_str()));
  fs_t1tttt_650_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_625to650_200to400.root", tag.c_str()));
  fs_t1tttt_650_425_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_650to675_250to425.root", tag.c_str()));
  fs_t1tttt_675_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_650to675_250to425.root", tag.c_str()));
  fs_t1tttt_675_275_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_650to675_250to425.root", tag.c_str()));
  fs_t1tttt_675_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_650to675_250to425.root", tag.c_str()));
  fs_t1tttt_650_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_650to675_250to425.root", tag.c_str()));
  fs_t1tttt_650_375_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_650to675_250to425.root", tag.c_str()));
  fs_t1tttt_650_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_650to675_250to425.root", tag.c_str()));
  fs_t1tttt_675_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_675_325to450.root", tag.c_str()));
  fs_t1tttt_675_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_675_325to450.root", tag.c_str()));
  fs_t1tttt_675_425_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_675_325to450.root", tag.c_str()));
  fs_t1tttt_675_375_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_675_325to450.root", tag.c_str()));
  fs_t1tttt_675_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_675_325to450.root", tag.c_str()));
  fs_t1tttt_675_325_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_675_325to450.root", tag.c_str()));
  fs_t1tttt_700_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_425_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_325_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_375_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_700_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700_1to450.root", tag.c_str()));
  fs_t1tttt_725_425_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_725_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_725_475_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_700_475_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_725_375_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_725_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_725_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_750_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_750_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_725_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_750_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_725_325_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_725_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_700to750_200to500.root", tag.c_str()));
  fs_t1tttt_775_375_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_775_425_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_775_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_775_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_775_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_750_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_750_525_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_750_375_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_750_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_750_475_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_750_425_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_750_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_750_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_750to775_350to525.root", tag.c_str()));
  fs_t1tttt_775_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_775_475to550.root", tag.c_str()));
  fs_t1tttt_775_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_775_475to550.root", tag.c_str()));
  fs_t1tttt_775_525_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_775_475to550.root", tag.c_str()));
  fs_t1tttt_775_475_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_775_475to550.root", tag.c_str()));
  fs_t1tttt_800_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_425_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_525_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_575_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_475_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_825_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_800_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_800to825_1to575.root", tag.c_str()));
  fs_t1tttt_825_525_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_825_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_850_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_850_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_850_475_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_825_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_825_575_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_850_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_850_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_850_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_825_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_825_425_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_825_475_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_850_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_825_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_825to850_200to600.root", tag.c_str()));
  fs_t1tttt_850_575_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_850_625_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_875_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_850_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_875_475_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_875_575_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_875_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_875_525_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_850_525_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_875_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_850_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_850_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_875_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_850to875_450to625.root", tag.c_str()));
  fs_t1tttt_900_200_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_300_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_575_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_625_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_875_625_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_525_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_875_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_0_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_100_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_900_250_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_875to900_1to650.root", tag.c_str()));
  fs_t1tttt_950_450_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_500_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_675_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_975_575_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_625_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_975_550_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_575_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_350_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_725_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_950_400_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_950to975_350to725.root", tag.c_str()));
  fs_t1tttt_975_650_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_975_600to750.root", tag.c_str()));
  fs_t1tttt_975_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_975_600to750.root", tag.c_str()));
  fs_t1tttt_975_725_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_975_600to750.root", tag.c_str()));
  fs_t1tttt_975_675_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_975_600to750.root", tag.c_str()));
  fs_t1tttt_975_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_975_600to750.root", tag.c_str()));
  fs_t1tttt_975_600_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_975_600to750.root", tag.c_str()));
  fs_t1tttt_975_625_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_975_600to750.root", tag.c_str()));
  fs_t1tttt_1950_700_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1950_700to950.root", tag.c_str()));
  fs_t1tttt_1950_750_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1950_700to950.root", tag.c_str()));
  fs_t1tttt_1950_950_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1950_700to950.root", tag.c_str()));
  fs_t1tttt_1950_800_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1950_700to950.root", tag.c_str()));
  fs_t1tttt_1950_900_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1950_700to950.root", tag.c_str()));
  fs_t1tttt_1950_850_chain->Add(Form("/nfs-7/userdata/ss2015/ssBabies/%s-fs/T1TTTT_1950_700to950.root", tag.c_str()));


  //Get yields
  pair<yields_t, plots_t> results_ttbar    = run(ttbar_chain);
  ttbar_chain->SetTitle("ttbar_fa");
  pair<yields_t, plots_t> results_ttbar_fa = run(ttbar_chain, 0, 0, 0, 1);
  ttbar_chain->SetTitle("ttbar_fl");
  pair<yields_t, plots_t> results_ttbar_fl = run(ttbar_chain, 0, 0, 0, 2);
  ttbar_chain->SetTitle("ttbar_ff");
  pair<yields_t, plots_t> results_ttbar_ff = run(ttbar_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_st       = run(st_chain);
  st_chain->SetTitle("st_ff");
  pair<yields_t, plots_t> results_st_ff    = run(st_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_wjets    = run(wjets_chain);
  wjets_chain->SetTitle("wjets_ff");
  pair<yields_t, plots_t> results_wjets_ff = run(wjets_chain, 0, 0, 0, 3);
  pair<yields_t, plots_t> results_dy       = run(dy_chain);
  dy_chain->SetTitle("dy_ff");
  pair<yields_t, plots_t> results_dy_ff    = run(dy_chain, 0, 0, 0, 3);

  pair<yields_t, plots_t> results_ttw      = run(ttw_chain);
  pair<yields_t, plots_t> results_ttzh     = run(ttzh_chain);
  pair<yields_t, plots_t> results_wz       = run(wz_chain);
  pair<yields_t, plots_t> results_ww       = run(ww_chain);

  pair<yields_t, plots_t> results_xg       = run(xg_chain, 0, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_rares    = run(rares_chain);

  pair<yields_t, plots_t> results_data     = run(data_chain, 1);
  duplicate_removal::clear_list();
  pair<yields_t, plots_t> results_flips    = run(flips_chain, 1, 1);
  duplicate_removal::clear_list();
  ttbar_chain->SetTitle("fakes_mc");
  pair<yields_t, plots_t> results_fakes_mc = run(ttbar_chain, 0, 0, 1);
  pair<yields_t, plots_t> results_fakes    = run(fakes_chain, 1, 0, 1);
  duplicate_removal::clear_list();
  fakes_chain->SetTitle("fakes_is");
  pair<yields_t, plots_t> results_fakes_is = run(fakes_chain, 1, 0, 2);

  pair<yields_t, plots_t> results_t1tttt_1200 = run(t1tttt_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t1tttt_1500 = run(t1tttt_1500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t5tttt_deg  = run(t5tttt_deg_chain , 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t5qqqq_1200 = run(t5qqqq_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t6ttww_650  = run(t6ttww_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t6ttww_600  = run(t6ttww_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t5qqqqww_1200 = run(t5qqqqww_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_t5qqqqww_deg  = run(t5qqqqww_deg_chain, 0, 0, 0, 0, 1);

  pair<yields_t, plots_t> results_fs_t1tttt_1100_600 = run(fs_t1tttt_1100_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_775 = run(fs_t1tttt_1100_775_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_750 = run(fs_t1tttt_1100_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_0 = run(fs_t1tttt_1100_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_300 = run(fs_t1tttt_1100_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_200 = run(fs_t1tttt_1100_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_500 = run(fs_t1tttt_1100_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_250 = run(fs_t1tttt_1100_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_100 = run(fs_t1tttt_1100_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_450 = run(fs_t1tttt_1100_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_50 = run(fs_t1tttt_1100_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_150 = run(fs_t1tttt_1100_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_350 = run(fs_t1tttt_1100_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_700 = run(fs_t1tttt_1100_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_725 = run(fs_t1tttt_1100_725_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_400 = run(fs_t1tttt_1100_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_550 = run(fs_t1tttt_1100_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_650 = run(fs_t1tttt_1100_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1125_700 = run(fs_t1tttt_1125_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1125_850 = run(fs_t1tttt_1125_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1125_800 = run(fs_t1tttt_1125_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1125_875 = run(fs_t1tttt_1125_875_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_825 = run(fs_t1tttt_1100_825_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_875 = run(fs_t1tttt_1100_875_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_850 = run(fs_t1tttt_1100_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1125_825 = run(fs_t1tttt_1125_825_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1125_725 = run(fs_t1tttt_1125_725_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1125_750 = run(fs_t1tttt_1125_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1100_800 = run(fs_t1tttt_1100_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1125_900 = run(fs_t1tttt_1125_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1125_775 = run(fs_t1tttt_1125_775_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_150 = run(fs_t1tttt_1150_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_800 = run(fs_t1tttt_1150_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_200 = run(fs_t1tttt_1150_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_775 = run(fs_t1tttt_1150_775_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_350 = run(fs_t1tttt_1150_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_50 = run(fs_t1tttt_1150_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_550 = run(fs_t1tttt_1150_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_250 = run(fs_t1tttt_1150_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_400 = run(fs_t1tttt_1150_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_650 = run(fs_t1tttt_1150_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_0 = run(fs_t1tttt_1150_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_100 = run(fs_t1tttt_1150_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_750 = run(fs_t1tttt_1150_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_700 = run(fs_t1tttt_1150_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_300 = run(fs_t1tttt_1150_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_450 = run(fs_t1tttt_1150_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_600 = run(fs_t1tttt_1150_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_500 = run(fs_t1tttt_1150_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1175_925 = run(fs_t1tttt_1175_925_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1175_775 = run(fs_t1tttt_1175_775_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1175_850 = run(fs_t1tttt_1175_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1175_900 = run(fs_t1tttt_1175_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1175_800 = run(fs_t1tttt_1175_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_900 = run(fs_t1tttt_1150_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1175_875 = run(fs_t1tttt_1175_875_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1175_825 = run(fs_t1tttt_1175_825_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_925 = run(fs_t1tttt_1150_925_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_825 = run(fs_t1tttt_1150_825_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_875 = run(fs_t1tttt_1150_875_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1150_850 = run(fs_t1tttt_1150_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1175_750 = run(fs_t1tttt_1175_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1175_950 = run(fs_t1tttt_1175_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_350 = run(fs_t1tttt_1200_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_550 = run(fs_t1tttt_1200_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_400 = run(fs_t1tttt_1200_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_500 = run(fs_t1tttt_1200_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_600 = run(fs_t1tttt_1200_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_450 = run(fs_t1tttt_1200_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_50 = run(fs_t1tttt_1200_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_700 = run(fs_t1tttt_1200_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_100 = run(fs_t1tttt_1200_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_150 = run(fs_t1tttt_1200_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_200 = run(fs_t1tttt_1200_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_825 = run(fs_t1tttt_1200_825_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_300 = run(fs_t1tttt_1200_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_800 = run(fs_t1tttt_1200_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_250 = run(fs_t1tttt_1200_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_650 = run(fs_t1tttt_1200_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_0 = run(fs_t1tttt_1200_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_750 = run(fs_t1tttt_1200_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_975 = run(fs_t1tttt_1200_975_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1225_825 = run(fs_t1tttt_1225_825_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1225_875 = run(fs_t1tttt_1225_875_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1225_850 = run(fs_t1tttt_1225_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1225_925 = run(fs_t1tttt_1225_925_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1225_800 = run(fs_t1tttt_1225_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_900 = run(fs_t1tttt_1200_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_925 = run(fs_t1tttt_1200_925_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1225_900 = run(fs_t1tttt_1225_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_950 = run(fs_t1tttt_1200_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1225_1000 = run(fs_t1tttt_1225_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_875 = run(fs_t1tttt_1200_875_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1200_850 = run(fs_t1tttt_1200_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1275_1050 = run(fs_t1tttt_1275_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1275_875 = run(fs_t1tttt_1275_875_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1275_850 = run(fs_t1tttt_1275_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1275_1000 = run(fs_t1tttt_1275_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1275_1025 = run(fs_t1tttt_1275_1025_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1250_925 = run(fs_t1tttt_1250_925_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1250_950 = run(fs_t1tttt_1250_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1250_700 = run(fs_t1tttt_1250_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1250_750 = run(fs_t1tttt_1250_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1250_900 = run(fs_t1tttt_1250_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1250_800 = run(fs_t1tttt_1250_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1250_850 = run(fs_t1tttt_1250_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1250_975 = run(fs_t1tttt_1250_975_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1250_875 = run(fs_t1tttt_1250_875_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1275_925 = run(fs_t1tttt_1275_925_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1275_950 = run(fs_t1tttt_1275_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1275_975 = run(fs_t1tttt_1275_975_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1275_900 = run(fs_t1tttt_1275_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_1025 = run(fs_t1tttt_1300_1025_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_400 = run(fs_t1tttt_1300_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_500 = run(fs_t1tttt_1300_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_550 = run(fs_t1tttt_1300_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_50 = run(fs_t1tttt_1300_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_1075 = run(fs_t1tttt_1300_1075_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_600 = run(fs_t1tttt_1300_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_450 = run(fs_t1tttt_1300_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_0 = run(fs_t1tttt_1300_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_1000 = run(fs_t1tttt_1300_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_1050 = run(fs_t1tttt_1300_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_150 = run(fs_t1tttt_1300_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_100 = run(fs_t1tttt_1300_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_650 = run(fs_t1tttt_1300_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_300 = run(fs_t1tttt_1300_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_200 = run(fs_t1tttt_1300_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_350 = run(fs_t1tttt_1300_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_250 = run(fs_t1tttt_1300_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_800 = run(fs_t1tttt_1300_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_925 = run(fs_t1tttt_1300_925_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_950 = run(fs_t1tttt_1300_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1325_1075 = run(fs_t1tttt_1325_1075_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_900 = run(fs_t1tttt_1300_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_850 = run(fs_t1tttt_1300_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1325_1000 = run(fs_t1tttt_1325_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1325_900 = run(fs_t1tttt_1325_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1325_1025 = run(fs_t1tttt_1325_1025_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1325_1050 = run(fs_t1tttt_1325_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_975 = run(fs_t1tttt_1300_975_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_700 = run(fs_t1tttt_1300_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1300_750 = run(fs_t1tttt_1300_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1325_1100 = run(fs_t1tttt_1325_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_350 = run(fs_t1tttt_1350_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_400 = run(fs_t1tttt_1350_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_550 = run(fs_t1tttt_1350_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_450 = run(fs_t1tttt_1350_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_500 = run(fs_t1tttt_1350_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_600 = run(fs_t1tttt_1350_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1375_1025 = run(fs_t1tttt_1375_1025_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_700 = run(fs_t1tttt_1350_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_750 = run(fs_t1tttt_1350_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_950 = run(fs_t1tttt_1350_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_975 = run(fs_t1tttt_1350_975_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1375_1000 = run(fs_t1tttt_1375_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_50 = run(fs_t1tttt_1350_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_800 = run(fs_t1tttt_1350_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_850 = run(fs_t1tttt_1350_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_900 = run(fs_t1tttt_1350_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1350_650 = run(fs_t1tttt_1350_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_800 = run(fs_t1tttt_1400_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_950 = run(fs_t1tttt_1400_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_850 = run(fs_t1tttt_1400_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_900 = run(fs_t1tttt_1400_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1425_1050 = run(fs_t1tttt_1425_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1425_1100 = run(fs_t1tttt_1425_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_550 = run(fs_t1tttt_1400_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_750 = run(fs_t1tttt_1400_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_600 = run(fs_t1tttt_1400_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1425_1000 = run(fs_t1tttt_1425_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1425_1075 = run(fs_t1tttt_1425_1075_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1425_1025 = run(fs_t1tttt_1425_1025_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_50 = run(fs_t1tttt_1400_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_700 = run(fs_t1tttt_1400_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_450 = run(fs_t1tttt_1400_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_500 = run(fs_t1tttt_1400_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1400_650 = run(fs_t1tttt_1400_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1425_1125 = run(fs_t1tttt_1425_1125_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1425_1150 = run(fs_t1tttt_1425_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1425_1175 = run(fs_t1tttt_1425_1175_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_1075 = run(fs_t1tttt_1450_1075_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_1125 = run(fs_t1tttt_1450_1125_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_1150 = run(fs_t1tttt_1450_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_1175 = run(fs_t1tttt_1450_1175_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_1100 = run(fs_t1tttt_1450_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_1200 = run(fs_t1tttt_1450_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_1050 = run(fs_t1tttt_1450_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_0 = run(fs_t1tttt_1450_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_150 = run(fs_t1tttt_1450_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_200 = run(fs_t1tttt_1450_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_1000 = run(fs_t1tttt_1450_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_100 = run(fs_t1tttt_1450_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_300 = run(fs_t1tttt_1450_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1475_1075 = run(fs_t1tttt_1475_1075_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_350 = run(fs_t1tttt_1450_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_600 = run(fs_t1tttt_1450_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_750 = run(fs_t1tttt_1450_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1475_1050 = run(fs_t1tttt_1475_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_450 = run(fs_t1tttt_1450_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_650 = run(fs_t1tttt_1450_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_550 = run(fs_t1tttt_1450_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_700 = run(fs_t1tttt_1450_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_950 = run(fs_t1tttt_1450_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_800 = run(fs_t1tttt_1450_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_850 = run(fs_t1tttt_1450_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_250 = run(fs_t1tttt_1450_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_50 = run(fs_t1tttt_1450_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_400 = run(fs_t1tttt_1450_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_900 = run(fs_t1tttt_1450_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1450_500 = run(fs_t1tttt_1450_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_1125 = run(fs_t1tttt_1500_1125_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_1150 = run(fs_t1tttt_1500_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1475_1100 = run(fs_t1tttt_1475_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1475_1125 = run(fs_t1tttt_1475_1125_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1475_1150 = run(fs_t1tttt_1475_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_1050 = run(fs_t1tttt_1500_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1475_1175 = run(fs_t1tttt_1475_1175_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_0 = run(fs_t1tttt_1500_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_1000 = run(fs_t1tttt_1500_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_1250 = run(fs_t1tttt_1500_1250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_1175 = run(fs_t1tttt_1500_1175_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_1200 = run(fs_t1tttt_1500_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_1100 = run(fs_t1tttt_1500_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_150 = run(fs_t1tttt_1500_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_100 = run(fs_t1tttt_1500_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_350 = run(fs_t1tttt_1500_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_50 = run(fs_t1tttt_1500_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_700 = run(fs_t1tttt_1500_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_650 = run(fs_t1tttt_1500_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_600 = run(fs_t1tttt_1500_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_800 = run(fs_t1tttt_1500_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_850 = run(fs_t1tttt_1500_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_900 = run(fs_t1tttt_1500_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_950 = run(fs_t1tttt_1500_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_750 = run(fs_t1tttt_1500_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1525_1125 = run(fs_t1tttt_1525_1125_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1525_1100 = run(fs_t1tttt_1525_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_250 = run(fs_t1tttt_1500_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_300 = run(fs_t1tttt_1500_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_400 = run(fs_t1tttt_1500_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_500 = run(fs_t1tttt_1500_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_550 = run(fs_t1tttt_1500_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_450 = run(fs_t1tttt_1500_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1500_200 = run(fs_t1tttt_1500_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_1050 = run(fs_t1tttt_1600_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_1150 = run(fs_t1tttt_1600_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_1250 = run(fs_t1tttt_1600_1250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_1200 = run(fs_t1tttt_1600_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_1100 = run(fs_t1tttt_1600_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_150 = run(fs_t1tttt_1600_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_750 = run(fs_t1tttt_1600_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_800 = run(fs_t1tttt_1600_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_1000 = run(fs_t1tttt_1600_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_100 = run(fs_t1tttt_1600_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_550 = run(fs_t1tttt_1600_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_1000 = run(fs_t1tttt_1650_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_950 = run(fs_t1tttt_1600_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_600 = run(fs_t1tttt_1600_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_0 = run(fs_t1tttt_1650_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_100 = run(fs_t1tttt_1650_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_1050 = run(fs_t1tttt_1650_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_850 = run(fs_t1tttt_1600_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_650 = run(fs_t1tttt_1600_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_900 = run(fs_t1tttt_1600_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_1300 = run(fs_t1tttt_1600_1300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_0 = run(fs_t1tttt_1600_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_200 = run(fs_t1tttt_1600_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_250 = run(fs_t1tttt_1600_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_400 = run(fs_t1tttt_1600_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_450 = run(fs_t1tttt_1600_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_300 = run(fs_t1tttt_1600_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_350 = run(fs_t1tttt_1600_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_1350 = run(fs_t1tttt_1600_1350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_700 = run(fs_t1tttt_1600_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_500 = run(fs_t1tttt_1600_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1600_50 = run(fs_t1tttt_1600_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_1100 = run(fs_t1tttt_1650_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_150 = run(fs_t1tttt_1650_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_1200 = run(fs_t1tttt_1650_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_200 = run(fs_t1tttt_1650_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_1250 = run(fs_t1tttt_1650_1250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_700 = run(fs_t1tttt_1650_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_750 = run(fs_t1tttt_1650_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_1300 = run(fs_t1tttt_1650_1300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_300 = run(fs_t1tttt_1650_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_900 = run(fs_t1tttt_1650_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_950 = run(fs_t1tttt_1650_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_350 = run(fs_t1tttt_1650_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_800 = run(fs_t1tttt_1650_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_650 = run(fs_t1tttt_1650_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_500 = run(fs_t1tttt_1650_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1000 = run(fs_t1tttt_1700_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_1400 = run(fs_t1tttt_1650_1400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_50 = run(fs_t1tttt_1650_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_550 = run(fs_t1tttt_1650_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_100 = run(fs_t1tttt_1700_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_1350 = run(fs_t1tttt_1650_1350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_850 = run(fs_t1tttt_1650_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_250 = run(fs_t1tttt_1650_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_400 = run(fs_t1tttt_1650_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_600 = run(fs_t1tttt_1650_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_0 = run(fs_t1tttt_1700_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1050 = run(fs_t1tttt_1700_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1200 = run(fs_t1tttt_1700_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1100 = run(fs_t1tttt_1700_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_450 = run(fs_t1tttt_1650_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1650_1150 = run(fs_t1tttt_1650_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1150 = run(fs_t1tttt_1700_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1450 = run(fs_t1tttt_1700_1450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_350 = run(fs_t1tttt_1700_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_500 = run(fs_t1tttt_1700_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_250 = run(fs_t1tttt_1700_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_300 = run(fs_t1tttt_1700_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_450 = run(fs_t1tttt_1700_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_400 = run(fs_t1tttt_1700_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_550 = run(fs_t1tttt_1700_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_850 = run(fs_t1tttt_1700_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1300 = run(fs_t1tttt_1750_1300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_150 = run(fs_t1tttt_1700_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_200 = run(fs_t1tttt_1700_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_100 = run(fs_t1tttt_1750_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1250 = run(fs_t1tttt_1750_1250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1350 = run(fs_t1tttt_1700_1350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1300 = run(fs_t1tttt_1700_1300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_50 = run(fs_t1tttt_1700_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1400 = run(fs_t1tttt_1700_1400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1150 = run(fs_t1tttt_1750_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1200 = run(fs_t1tttt_1750_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_700 = run(fs_t1tttt_1700_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_650 = run(fs_t1tttt_1700_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_750 = run(fs_t1tttt_1700_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1000 = run(fs_t1tttt_1750_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_800 = run(fs_t1tttt_1700_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_950 = run(fs_t1tttt_1700_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_900 = run(fs_t1tttt_1700_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_0 = run(fs_t1tttt_1750_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1050 = run(fs_t1tttt_1750_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_600 = run(fs_t1tttt_1700_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1700_1250 = run(fs_t1tttt_1700_1250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1100 = run(fs_t1tttt_1750_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1350 = run(fs_t1tttt_1750_1350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_450 = run(fs_t1tttt_1750_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1400 = run(fs_t1tttt_1750_1400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_500 = run(fs_t1tttt_1750_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_50 = run(fs_t1tttt_1750_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_1450 = run(fs_t1tttt_1750_1450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_300 = run(fs_t1tttt_1750_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_350 = run(fs_t1tttt_1750_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_400 = run(fs_t1tttt_1750_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_600 = run(fs_t1tttt_1750_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_550 = run(fs_t1tttt_1750_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_850 = run(fs_t1tttt_1750_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_900 = run(fs_t1tttt_1750_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_700 = run(fs_t1tttt_1750_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_650 = run(fs_t1tttt_1750_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_800 = run(fs_t1tttt_1750_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_200 = run(fs_t1tttt_1750_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_950 = run(fs_t1tttt_1750_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_150 = run(fs_t1tttt_1750_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_250 = run(fs_t1tttt_1750_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1750_750 = run(fs_t1tttt_1750_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1150 = run(fs_t1tttt_1850_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1300 = run(fs_t1tttt_1800_1300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1350 = run(fs_t1tttt_1800_1350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1400 = run(fs_t1tttt_1800_1400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_100 = run(fs_t1tttt_1850_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_500 = run(fs_t1tttt_1800_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_950 = run(fs_t1tttt_1800_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_800 = run(fs_t1tttt_1800_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_550 = run(fs_t1tttt_1800_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_50 = run(fs_t1tttt_1800_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_0 = run(fs_t1tttt_1850_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1150 = run(fs_t1tttt_1800_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1450 = run(fs_t1tttt_1800_1450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_350 = run(fs_t1tttt_1800_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1250 = run(fs_t1tttt_1800_1250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_200 = run(fs_t1tttt_1800_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1200 = run(fs_t1tttt_1800_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_150 = run(fs_t1tttt_1800_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_300 = run(fs_t1tttt_1800_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_650 = run(fs_t1tttt_1800_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1100 = run(fs_t1tttt_1850_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_400 = run(fs_t1tttt_1800_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_450 = run(fs_t1tttt_1800_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_250 = run(fs_t1tttt_1800_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1000 = run(fs_t1tttt_1800_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1050 = run(fs_t1tttt_1800_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_1100 = run(fs_t1tttt_1800_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_100 = run(fs_t1tttt_1800_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_0 = run(fs_t1tttt_1800_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_700 = run(fs_t1tttt_1800_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_600 = run(fs_t1tttt_1800_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_850 = run(fs_t1tttt_1800_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_900 = run(fs_t1tttt_1800_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1000 = run(fs_t1tttt_1850_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1050 = run(fs_t1tttt_1850_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1800_750 = run(fs_t1tttt_1800_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_150 = run(fs_t1tttt_1850_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_350 = run(fs_t1tttt_1850_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_550 = run(fs_t1tttt_1850_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_400 = run(fs_t1tttt_1850_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_450 = run(fs_t1tttt_1850_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_600 = run(fs_t1tttt_1850_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_200 = run(fs_t1tttt_1850_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_650 = run(fs_t1tttt_1850_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1150 = run(fs_t1tttt_1900_1150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1200 = run(fs_t1tttt_1900_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_200 = run(fs_t1tttt_1900_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_900 = run(fs_t1tttt_1850_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1350 = run(fs_t1tttt_1900_1350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_950 = run(fs_t1tttt_1850_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1400 = run(fs_t1tttt_1900_1400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1450 = run(fs_t1tttt_1900_1450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_150 = run(fs_t1tttt_1900_150_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_100 = run(fs_t1tttt_1900_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1050 = run(fs_t1tttt_1900_1050_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1250 = run(fs_t1tttt_1900_1250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1300 = run(fs_t1tttt_1900_1300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_0 = run(fs_t1tttt_1900_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1000 = run(fs_t1tttt_1900_1000_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1200 = run(fs_t1tttt_1850_1200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1300 = run(fs_t1tttt_1850_1300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1450 = run(fs_t1tttt_1850_1450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_500 = run(fs_t1tttt_1850_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1900_1100 = run(fs_t1tttt_1900_1100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1350 = run(fs_t1tttt_1850_1350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1400 = run(fs_t1tttt_1850_1400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_1250 = run(fs_t1tttt_1850_1250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_50 = run(fs_t1tttt_1850_50_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_250 = run(fs_t1tttt_1850_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_850 = run(fs_t1tttt_1850_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_700 = run(fs_t1tttt_1850_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_800 = run(fs_t1tttt_1850_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_750 = run(fs_t1tttt_1850_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1850_300 = run(fs_t1tttt_1850_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_600_250 = run(fs_t1tttt_600_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_600_300 = run(fs_t1tttt_600_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_600_325 = run(fs_t1tttt_600_325_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_600_275 = run(fs_t1tttt_600_275_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_625_325 = run(fs_t1tttt_625_325_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_625_350 = run(fs_t1tttt_625_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_625_375 = run(fs_t1tttt_625_375_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_625_275 = run(fs_t1tttt_625_275_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_625_300 = run(fs_t1tttt_625_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_650_200 = run(fs_t1tttt_650_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_650_250 = run(fs_t1tttt_650_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_650_325 = run(fs_t1tttt_650_325_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_625_400 = run(fs_t1tttt_625_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_650_275 = run(fs_t1tttt_650_275_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_650_300 = run(fs_t1tttt_650_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_650_425 = run(fs_t1tttt_650_425_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_675_300 = run(fs_t1tttt_675_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_675_275 = run(fs_t1tttt_675_275_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_675_250 = run(fs_t1tttt_675_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_650_350 = run(fs_t1tttt_650_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_650_375 = run(fs_t1tttt_650_375_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_650_400 = run(fs_t1tttt_650_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_675_400 = run(fs_t1tttt_675_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_675_450 = run(fs_t1tttt_675_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_675_425 = run(fs_t1tttt_675_425_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_675_375 = run(fs_t1tttt_675_375_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_675_350 = run(fs_t1tttt_675_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_675_325 = run(fs_t1tttt_675_325_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_250 = run(fs_t1tttt_700_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_300 = run(fs_t1tttt_700_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_450 = run(fs_t1tttt_700_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_200 = run(fs_t1tttt_700_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_400 = run(fs_t1tttt_700_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_425 = run(fs_t1tttt_700_425_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_0 = run(fs_t1tttt_700_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_325 = run(fs_t1tttt_700_325_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_100 = run(fs_t1tttt_700_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_375 = run(fs_t1tttt_700_375_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_350 = run(fs_t1tttt_700_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_725_425 = run(fs_t1tttt_725_425_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_725_450 = run(fs_t1tttt_725_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_725_475 = run(fs_t1tttt_725_475_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_700_475 = run(fs_t1tttt_700_475_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_725_375 = run(fs_t1tttt_725_375_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_725_300 = run(fs_t1tttt_725_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_725_350 = run(fs_t1tttt_725_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_200 = run(fs_t1tttt_750_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_250 = run(fs_t1tttt_750_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_725_500 = run(fs_t1tttt_725_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_300 = run(fs_t1tttt_750_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_725_325 = run(fs_t1tttt_725_325_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_725_400 = run(fs_t1tttt_725_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_775_375 = run(fs_t1tttt_775_375_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_775_425 = run(fs_t1tttt_775_425_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_775_450 = run(fs_t1tttt_775_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_775_400 = run(fs_t1tttt_775_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_775_350 = run(fs_t1tttt_775_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_500 = run(fs_t1tttt_750_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_525 = run(fs_t1tttt_750_525_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_375 = run(fs_t1tttt_750_375_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_450 = run(fs_t1tttt_750_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_475 = run(fs_t1tttt_750_475_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_425 = run(fs_t1tttt_750_425_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_350 = run(fs_t1tttt_750_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_750_400 = run(fs_t1tttt_750_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_775_500 = run(fs_t1tttt_775_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_775_550 = run(fs_t1tttt_775_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_775_525 = run(fs_t1tttt_775_525_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_775_475 = run(fs_t1tttt_775_475_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_400 = run(fs_t1tttt_800_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_425 = run(fs_t1tttt_800_425_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_525 = run(fs_t1tttt_800_525_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_0 = run(fs_t1tttt_800_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_250 = run(fs_t1tttt_800_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_350 = run(fs_t1tttt_800_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_200 = run(fs_t1tttt_800_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_300 = run(fs_t1tttt_800_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_575 = run(fs_t1tttt_800_575_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_450 = run(fs_t1tttt_800_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_500 = run(fs_t1tttt_800_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_550 = run(fs_t1tttt_800_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_475 = run(fs_t1tttt_800_475_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_825_400 = run(fs_t1tttt_825_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_800_100 = run(fs_t1tttt_800_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_825_525 = run(fs_t1tttt_825_525_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_825_550 = run(fs_t1tttt_825_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_400 = run(fs_t1tttt_850_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_250 = run(fs_t1tttt_850_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_475 = run(fs_t1tttt_850_475_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_825_500 = run(fs_t1tttt_825_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_825_575 = run(fs_t1tttt_825_575_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_350 = run(fs_t1tttt_850_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_450 = run(fs_t1tttt_850_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_300 = run(fs_t1tttt_850_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_825_450 = run(fs_t1tttt_825_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_825_425 = run(fs_t1tttt_825_425_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_825_475 = run(fs_t1tttt_825_475_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_200 = run(fs_t1tttt_850_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_825_600 = run(fs_t1tttt_825_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_575 = run(fs_t1tttt_850_575_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_625 = run(fs_t1tttt_850_625_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_875_550 = run(fs_t1tttt_875_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_600 = run(fs_t1tttt_850_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_875_475 = run(fs_t1tttt_875_475_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_875_575 = run(fs_t1tttt_875_575_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_875_600 = run(fs_t1tttt_875_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_875_525 = run(fs_t1tttt_875_525_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_525 = run(fs_t1tttt_850_525_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_875_450 = run(fs_t1tttt_875_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_550 = run(fs_t1tttt_850_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_850_500 = run(fs_t1tttt_850_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_875_500 = run(fs_t1tttt_875_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_200 = run(fs_t1tttt_900_200_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_300 = run(fs_t1tttt_900_300_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_575 = run(fs_t1tttt_900_575_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_350 = run(fs_t1tttt_900_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_600 = run(fs_t1tttt_900_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_550 = run(fs_t1tttt_900_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_625 = run(fs_t1tttt_900_625_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_875_625 = run(fs_t1tttt_875_625_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_525 = run(fs_t1tttt_900_525_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_500 = run(fs_t1tttt_900_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_875_650 = run(fs_t1tttt_875_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_450 = run(fs_t1tttt_900_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_400 = run(fs_t1tttt_900_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_0 = run(fs_t1tttt_900_0_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_100 = run(fs_t1tttt_900_100_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_900_250 = run(fs_t1tttt_900_250_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_450 = run(fs_t1tttt_950_450_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_500 = run(fs_t1tttt_950_500_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_550 = run(fs_t1tttt_950_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_675 = run(fs_t1tttt_950_675_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_975_575 = run(fs_t1tttt_975_575_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_625 = run(fs_t1tttt_950_625_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_700 = run(fs_t1tttt_950_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_975_550 = run(fs_t1tttt_975_550_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_575 = run(fs_t1tttt_950_575_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_350 = run(fs_t1tttt_950_350_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_725 = run(fs_t1tttt_950_725_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_600 = run(fs_t1tttt_950_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_650 = run(fs_t1tttt_950_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_950_400 = run(fs_t1tttt_950_400_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_975_650 = run(fs_t1tttt_975_650_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_975_700 = run(fs_t1tttt_975_700_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_975_725 = run(fs_t1tttt_975_725_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_975_675 = run(fs_t1tttt_975_675_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_975_750 = run(fs_t1tttt_975_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_975_600 = run(fs_t1tttt_975_600_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_975_625 = run(fs_t1tttt_975_625_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1950_750 = run(fs_t1tttt_1950_750_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1950_950 = run(fs_t1tttt_1950_950_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1950_800 = run(fs_t1tttt_1950_800_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1950_900 = run(fs_t1tttt_1950_900_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1950_850 = run(fs_t1tttt_1950_850_chain, 0, 0, 0, 0, 1);
  pair<yields_t, plots_t> results_fs_t1tttt_1950_700 = run(fs_t1tttt_1950_700_chain, 0, 0, 0, 0, 1);

  yields_t& ttbar    = results_ttbar.first;
  yields_t& ttbar_ff = results_ttbar_ff.first;
  yields_t& wjets_ff = results_wjets_ff.first;
  yields_t& dy_ff    = results_dy_ff.first;
  yields_t& st_ff    = results_st_ff.first;
  yields_t& ttbar_fa = results_ttbar_fa.first;
  yields_t& ttbar_fl = results_ttbar_fl.first;
  yields_t& st       = results_st.first;
  yields_t& wjets    = results_wjets.first;
  yields_t& dy       = results_dy.first;

  yields_t& ttw      = results_ttw.first;
  yields_t& ttzh     = results_ttzh.first;
  yields_t& wz       = results_wz.first;
  yields_t& ww       = results_ww.first;
  yields_t& xg       = results_xg.first;
  yields_t& rares    = results_rares.first;

  yields_t& data     = results_data.first;
  yields_t& flips    = results_flips.first;
  yields_t& fakes    = results_fakes.first;
  yields_t& fakes_is = results_fakes_is.first;
  yields_t& fakes_mc = results_fakes_mc.first;

  yields_t& t1tttt_1200    = results_t1tttt_1200.first;
  yields_t& t6ttww_650    = results_t6ttww_650.first;

  //Make yield table
  CTable table; 
  table.setPrecision(2); 
  table.setTitle("SS Baseline Yields"); 
  table.useTitle(); 
  table.setTable() (                  "EE"  , "EM"       , "MM"         , "TOTAL"      )
                   ("ttw"      , ttw.EE      , ttw.EM      , ttw.MM      , ttw.TOTAL    )
                   ("ttzh"     , ttzh.EE     , ttzh.EM     , ttzh.MM     , ttzh.TOTAL   )
                   ("wz"       , wz.EE       , wz.EM       , wz.MM       , wz.TOTAL     )
                   ("ww"       , ww.EE       , ww.EM       , ww.MM       , ww.TOTAL     )
                   ("xg"       , xg.EE       , xg.EM       , xg.MM       , xg.TOTAL     )
                   ("rares"    , rares.EE    , rares.EM    , rares.MM    , rares.TOTAL  )
                   ("flips"    , flips.EE    , flips.EM    , flips.MM    , flips.TOTAL  )
                   ("fakes"    , fakes.EE    , fakes.EM    , fakes.MM    , fakes.TOTAL  )
                   ("total"    , total.EE    , total.EM    , total.MM    , total.TOTAL  )
                   ("data"     , data.EE     , data.EM     , data.MM     , data.TOTAL   )
                   ("fakes_is" , fakes_is.EE , fakes_is.EM , fakes_is.MM , fakes_is.TOTAL  )
                   ("fakes_mc" , fakes_mc.EE , fakes_mc.EM , fakes_mc.MM , fakes_mc.TOTAL  )
                   ("ttbar"    , ttbar.EE    , ttbar.EM    , ttbar.MM    , ttbar.TOTAL  )
                   ("ttbar_fa" , ttbar_fa.EE , ttbar_fa.EM , ttbar_fa.MM , ttbar_fa.TOTAL)
                   ("ttbar_fl" , ttbar_fl.EE , ttbar_fl.EM , ttbar_fl.MM , ttbar_fl.TOTAL)
                   ("st"       , st.EE       , st.EM       , st.MM       , st.TOTAL     )
                   ("wjets"    , wjets.EE    , wjets.EM    , wjets.MM    , wjets.TOTAL  )
                   ("dy"       , dy.EE       , dy.EM       , dy.MM       , dy.TOTAL     )
                   ("t1tttt_1200"    , t1tttt_1200.EE    , t1tttt_1200.EM    , t1tttt_1200.MM    , t1tttt_1200.TOTAL  )
                   ("t6ttww_650"    , t6ttww_650.EE    , t6ttww_650.EM    , t6ttww_650.MM    , t6ttww_650.TOTAL  );
  table.print();

  plots_t& p_ttbar    = results_ttbar.second;
  plots_t& p_ttbar_ff = results_ttbar_ff.second;
  plots_t& p_wjets_ff = results_wjets_ff.second;
  plots_t& p_dy_ff    = results_dy_ff.second;
  plots_t& p_st_ff    = results_st_ff.second;
  plots_t& p_ttbar_fa = results_ttbar_fa.second;
  plots_t& p_ttbar_fl = results_ttbar_fl.second;
  plots_t& p_st       = results_st.second;
  plots_t& p_wjets    = results_wjets.second;
  plots_t& p_dy       = results_dy.second;

  plots_t& p_ttw      = results_ttw.second;
  plots_t& p_ttzh     = results_ttzh.second;
  plots_t& p_wz       = results_wz.second;
  plots_t& p_ww       = results_ww.second;
  plots_t& p_xg       = results_xg.second;
  plots_t& p_rares    = results_rares.second;

  plots_t& p_data     = results_data.second;
  plots_t& p_flips    = results_flips.second;
  plots_t& p_fakes    = results_fakes.second;
  plots_t& p_fakes_is = results_fakes_is.second;

  plots_t& p_t1tttt_1200 = results_t1tttt_1200.second;

  //Titles for legend
  vector <string> titles;
  titles.push_back("ttW"); 
  titles.push_back("ttZ/H"); 
  titles.push_back("WZ"); 
  titles.push_back("WW"); 
  titles.push_back("X+G"); 
  titles.push_back("Rares"); 
  titles.push_back("Flips"); 
  titles.push_back("Fakes"); 

  vector <Color_t> colors;
  colors.push_back(kGreen+3); 
  colors.push_back(kGreen-6); 
  colors.push_back(kOrange); 
  colors.push_back(kOrange-3); 
  colors.push_back(kViolet+2); 
  colors.push_back(kMagenta-7); 
  colors.push_back(kGray+2); 
  colors.push_back(18); 

  //Titles for MC-only plots
  vector <string> titles2;
  titles2.push_back("ttW"); 
  titles2.push_back("ttZ/H"); 
  titles2.push_back("WZ"); 
  titles2.push_back("WW"); 
  titles2.push_back("X+G"); 
  titles2.push_back("Rares"); 
  titles2.push_back("ttbar"); 
  titles2.push_back("dy"); 
  titles2.push_back("wjets"); 
  titles2.push_back("st"); 

  vector <Color_t> colors2;
  colors2.push_back(kGreen+3); 
  colors2.push_back(kGreen-6); 
  colors2.push_back(kOrange); 
  colors2.push_back(kOrange-3); 
  colors2.push_back(kViolet+2); 
  colors2.push_back(kMagenta-7); 
  colors2.push_back(18); 
  colors2.push_back(kGray+2); 
  colors2.push_back(18); 
  colors2.push_back(18); 

  TString header = "        | %15s | %15s | %15s | %15s | %15s | %15s | %15s | %15s | %15s | %5s | %10s";
  TString HHline = "HH SR%2i | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.0f | %10.2f";
  TString HLline = "HL SR%2i | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.0f | %10.2f";
  TString LLline = "LL SR%2i | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.2f +/- %5.2f | %5.0f | %10.2f";
  if (doLatex) {
    header = "     & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %5s & %10s \\\\";
    HHline = "SR%2i & %5.2f $\\pm$ %5.2f & %5.2f $\\pm$ %5.2f & %5.2f $\\pm$ %5.2f & %5.2f $\\pm$ %5.2f & %5.2f $\\pm$ %5.2f & %5.2f $\\pm$ %5.2f & %5.2f $\\pm$ %5.2f & %5.2f $\\pm$ %5.2f & %5.2f $\\pm$ %5.2f & %5.0f & %10.2f \\\\";
    HLline = HHline;
    LLline = HHline;
    cout << "\\documentclass[12pt,a4paper]{article}" << endl;
    cout << "\\usepackage{lscape}" << endl;
    cout << "\\begin{document}" << endl;
  }

  cout << endl;
  if (doLatex) {
    cout << "\\begin{landscape}" << endl;
    cout << "\\begin{table}" << endl;
    cout << "\\footnotesize" << endl;
    cout << "\\begin{center}" << endl;
    cout << "\\caption{Event yields in HH regions.}" << endl;
    cout << "\\label{tab:yieldsHH}" << endl;
    cout << "\\begin{tabular}{c|cccccccc|c|c|c}\\hline" << endl;
  }
  cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
  if (doLatex) cout << "\\hline\\hline" << endl;
  for (int bin=1;bin<=p_ttw.SRHH.TOTAL->GetNbinsX(); ++bin) {
    cout << Form(HHline.Data(),bin,
		 p_ttw.SRHH.TOTAL->GetBinContent(bin),p_ttw.SRHH.TOTAL->GetBinContent(bin)*roughSystTTW,
		 p_ttzh.SRHH.TOTAL->GetBinContent(bin),p_ttzh.SRHH.TOTAL->GetBinContent(bin)*roughSystTTZH,
		 p_wz.SRHH.TOTAL->GetBinContent(bin),p_wz.SRHH.TOTAL->GetBinContent(bin)*roughSystWZ,
		 p_ww.SRHH.TOTAL->GetBinContent(bin),p_ww.SRHH.TOTAL->GetBinContent(bin)*roughSystWW,
		 p_xg.SRHH.TOTAL->GetBinContent(bin),p_xg.SRHH.TOTAL->GetBinContent(bin)*roughSystXG,
		 p_rares.SRHH.TOTAL->GetBinContent(bin),p_rares.SRHH.TOTAL->GetBinContent(bin)*roughSystRARES,
		 p_flips.SRHH.TOTAL->GetBinContent(bin),p_flips.SRHH.TOTAL->GetBinContent(bin)*roughSystFLIPS,
		 p_fakes.SRHH.TOTAL->GetBinContent(bin),p_fakes.SRHH.TOTAL->GetBinContent(bin)*roughSystFAKESHH,
		 (p_ttw.SRHH.TOTAL->GetBinContent(bin)+p_ttzh.SRHH.TOTAL->GetBinContent(bin)+p_wz.SRHH.TOTAL->GetBinContent(bin)+
		  p_ww.SRHH.TOTAL->GetBinContent(bin)+p_xg.SRHH.TOTAL->GetBinContent(bin)+
		  p_rares.SRHH.TOTAL->GetBinContent(bin)+p_flips.SRHH.TOTAL->GetBinContent(bin)+p_fakes.SRHH.TOTAL->GetBinContent(bin)),
		 sqrt(pow(p_ttw.SRHH.TOTAL->GetBinContent(bin)*roughSystTTW,2)+pow(p_ttzh.SRHH.TOTAL->GetBinContent(bin)*roughSystTTZH,2)+pow(p_wz.SRHH.TOTAL->GetBinContent(bin)*roughSystWZ,2)+
		  pow(p_ww.SRHH.TOTAL->GetBinContent(bin)*roughSystWW,2)+pow(p_xg.SRHH.TOTAL->GetBinContent(bin)*roughSystXG,2)+
		  pow(p_rares.SRHH.TOTAL->GetBinContent(bin)*roughSystRARES,2)+pow(p_flips.SRHH.TOTAL->GetBinContent(bin)*roughSystFLIPS,2)+
		      pow(p_fakes.SRHH.TOTAL->GetBinContent(bin)*roughSystFAKESHH,2)),
		 p_data.SRHH.TOTAL->GetBinContent(bin),
		 p_t1tttt_1200.SRHH.TOTAL->GetBinContent(bin)
		 )
	 << endl;
  }
  if (doLatex) {
    cout << "\\hline" << endl;
    cout << "\\end{tabular}" << endl;
    cout << "\\end{center}" << endl;
    cout << "\\end{table}" << endl;
    cout << "\\end{landscape}" << endl;
  }

  cout << endl;
  if (doLatex) {
    cout << "\\begin{landscape}" << endl;
    cout << "\\begin{table}" << endl;
    cout << "\\footnotesize" << endl;
    cout << "\\begin{center}" << endl;
    cout << "\\caption{Event yields in HL regions.}" << endl;
    cout << "\\label{tab:yieldsHL}" << endl;
    cout << "\\begin{tabular}{c|cccccccc|c|c|c}\\hline" << endl;
  }
  cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
  if (doLatex) cout << "\\hline\\hline" << endl;
  for (int bin=1;bin<=p_ttw.SRHL.TOTAL->GetNbinsX(); ++bin) {
    cout << Form(HLline.Data(),bin,
		 p_ttw.SRHL.TOTAL->GetBinContent(bin),p_ttw.SRHL.TOTAL->GetBinContent(bin)*roughSystTTW,
		 p_ttzh.SRHL.TOTAL->GetBinContent(bin),p_ttzh.SRHL.TOTAL->GetBinContent(bin)*roughSystTTZH,
		 p_wz.SRHL.TOTAL->GetBinContent(bin),p_wz.SRHL.TOTAL->GetBinContent(bin)*roughSystWZ,
		 p_ww.SRHL.TOTAL->GetBinContent(bin),p_ww.SRHL.TOTAL->GetBinContent(bin)*roughSystWW,
		 p_xg.SRHL.TOTAL->GetBinContent(bin),p_xg.SRHL.TOTAL->GetBinContent(bin)*roughSystXG,
		 p_rares.SRHL.TOTAL->GetBinContent(bin),p_rares.SRHL.TOTAL->GetBinContent(bin)*roughSystRARES,
		 p_flips.SRHL.TOTAL->GetBinContent(bin),p_flips.SRHL.TOTAL->GetBinContent(bin)*roughSystFLIPS,
		 p_fakes.SRHL.TOTAL->GetBinContent(bin),p_fakes.SRHL.TOTAL->GetBinContent(bin)*roughSystFAKESXL,
		 (p_ttw.SRHL.TOTAL->GetBinContent(bin)+p_ttzh.SRHL.TOTAL->GetBinContent(bin)+p_wz.SRHL.TOTAL->GetBinContent(bin)+
		  p_ww.SRHL.TOTAL->GetBinContent(bin)+p_xg.SRHL.TOTAL->GetBinContent(bin)+
		  p_rares.SRHL.TOTAL->GetBinContent(bin)+p_flips.SRHL.TOTAL->GetBinContent(bin)+p_fakes.SRHL.TOTAL->GetBinContent(bin)),
		 sqrt(pow(p_ttw.SRHL.TOTAL->GetBinContent(bin)*roughSystTTW,2)+pow(p_ttzh.SRHL.TOTAL->GetBinContent(bin)*roughSystTTZH,2)+pow(p_wz.SRHL.TOTAL->GetBinContent(bin)*roughSystWZ,2)+
		  pow(p_ww.SRHL.TOTAL->GetBinContent(bin)*roughSystWW,2)+pow(p_xg.SRHL.TOTAL->GetBinContent(bin)*roughSystXG,2)+
		  pow(p_rares.SRHL.TOTAL->GetBinContent(bin)*roughSystRARES,2)+pow(p_flips.SRHL.TOTAL->GetBinContent(bin)*roughSystFLIPS,2)+
		      pow(p_fakes.SRHL.TOTAL->GetBinContent(bin)*roughSystFAKESXL,2)),
		 p_data.SRHL.TOTAL->GetBinContent(bin),
		 p_t1tttt_1200.SRHL.TOTAL->GetBinContent(bin)
		 )
	 << endl;
  }
  if (doLatex) {
    cout << "\\hline" << endl;
    cout << "\\end{tabular}" << endl;
    cout << "\\end{center}" << endl;
    cout << "\\end{table}" << endl;
    cout << "\\end{landscape}" << endl;
  }

  cout << endl;
  if (doLatex) {
    cout << "\\begin{landscape}" << endl;
    cout << "\\begin{table}" << endl;
    cout << "\\footnotesize" << endl;
    cout << "\\begin{center}" << endl;
    cout << "\\caption{Event yields in LL regions.}" << endl;
    cout << "\\label{tab:yieldsLL}" << endl;
    cout << "\\begin{tabular}{c|cccccccc|c|c|c}\\hline" << endl;
  }
  cout <<   Form(header.Data(),"TTW","TTZ/H","WZ","WW","XG","RARES","FLIPS","FAKES","TOTAL","DATA","T1TTTT1200") << endl;
  if (doLatex) cout << "\\hline\\hline" << endl;
  for (int bin=1;bin<=p_ttw.SRLL.TOTAL->GetNbinsX(); ++bin) {
    cout << Form(LLline.Data(),bin,
		 p_ttw.SRLL.TOTAL->GetBinContent(bin),p_ttw.SRLL.TOTAL->GetBinContent(bin)*roughSystTTW,
		 p_ttzh.SRLL.TOTAL->GetBinContent(bin),p_ttzh.SRLL.TOTAL->GetBinContent(bin)*roughSystTTZH,
		 p_wz.SRLL.TOTAL->GetBinContent(bin),p_wz.SRLL.TOTAL->GetBinContent(bin)*roughSystWZ,
		 p_ww.SRLL.TOTAL->GetBinContent(bin),p_ww.SRLL.TOTAL->GetBinContent(bin)*roughSystWW,
		 p_xg.SRLL.TOTAL->GetBinContent(bin),p_xg.SRLL.TOTAL->GetBinContent(bin)*roughSystXG,
		 p_rares.SRLL.TOTAL->GetBinContent(bin),p_rares.SRLL.TOTAL->GetBinContent(bin)*roughSystRARES,
		 p_flips.SRLL.TOTAL->GetBinContent(bin),p_flips.SRLL.TOTAL->GetBinContent(bin)*roughSystFLIPS,
		 p_fakes.SRLL.TOTAL->GetBinContent(bin),p_fakes.SRLL.TOTAL->GetBinContent(bin)*roughSystFAKESXL,
		 (p_ttw.SRLL.TOTAL->GetBinContent(bin)+p_ttzh.SRLL.TOTAL->GetBinContent(bin)+p_wz.SRLL.TOTAL->GetBinContent(bin)+
		  p_ww.SRLL.TOTAL->GetBinContent(bin)+p_xg.SRLL.TOTAL->GetBinContent(bin)+
		  p_rares.SRLL.TOTAL->GetBinContent(bin)+p_flips.SRLL.TOTAL->GetBinContent(bin)+p_fakes.SRLL.TOTAL->GetBinContent(bin)),
		 sqrt(pow(p_ttw.SRLL.TOTAL->GetBinContent(bin)*roughSystTTW,2)+pow(p_ttzh.SRLL.TOTAL->GetBinContent(bin)*roughSystTTZH,2)+pow(p_wz.SRLL.TOTAL->GetBinContent(bin)*roughSystWZ,2)+
		  pow(p_ww.SRLL.TOTAL->GetBinContent(bin)*roughSystWW,2)+pow(p_xg.SRLL.TOTAL->GetBinContent(bin)*roughSystXG,2)+
		  pow(p_rares.SRLL.TOTAL->GetBinContent(bin)*roughSystRARES,2)+pow(p_flips.SRLL.TOTAL->GetBinContent(bin)*roughSystFLIPS,2)+
		      pow(p_fakes.SRLL.TOTAL->GetBinContent(bin)*roughSystFAKESXL,2)),
		 p_data.SRLL.TOTAL->GetBinContent(bin),
		 p_t1tttt_1200.SRLL.TOTAL->GetBinContent(bin)
		 )
	 << endl;
  }
  if (doLatex) {
    cout << "\\hline" << endl;
    cout << "\\end{tabular}" << endl;
    cout << "\\end{center}" << endl;
    cout << "\\end{table}" << endl;
    cout << "\\end{landscape}" << endl;

    cout << "\\end{document}" << endl;
  }

  //SR plots
  vector<pair<TH1F*, float> > SRHH_plots;
  SRHH_plots.push_back(pair<TH1F*, float>(p_ttw.SRHH.TOTAL  , roughSystTTW     ));
  SRHH_plots.push_back(pair<TH1F*, float>(p_ttzh.SRHH.TOTAL , roughSystTTZH    ));
  SRHH_plots.push_back(pair<TH1F*, float>(p_wz.SRHH.TOTAL   , roughSystWZ      ));
  SRHH_plots.push_back(pair<TH1F*, float>(p_ww.SRHH.TOTAL   , roughSystWW      ));
  SRHH_plots.push_back(pair<TH1F*, float>(p_xg.SRHH.TOTAL   , roughSystXG      ));
  SRHH_plots.push_back(pair<TH1F*, float>(p_rares.SRHH.TOTAL, roughSystRARES   ));
  SRHH_plots.push_back(pair<TH1F*, float>(p_flips.SRHH.TOTAL, roughSystFLIPS   ));
  SRHH_plots.push_back(pair<TH1F*, float>(p_fakes.SRHH.TOTAL, roughSystFAKESHH ));
  dataMCplotMaker(p_data.SRHH.TOTAL, SRHH_plots, titles, "H-H SRs", "data-driven", Form("--lumi %.2f --outputName HHSR.pdf --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.05 --noOverflow --systInclStat --noRatioPlot", lumiAG), vector <TH1F*>(), vector <string>(), colors);
  dataMCplotMaker(p_data.SRHH.TOTAL, SRHH_plots, titles, "H-H SRs", "data-driven", Form("--lumi %.2f --outputName HHSR_log.pdf --xAxisLabel SR --noXaxisUnit --legendUp -.05 --noOverflow --systInclStat --noRatioPlot", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > SRHL_plots;
  SRHL_plots.push_back(pair<TH1F*, float>(p_ttw.SRHL.TOTAL  , roughSystTTW     ));
  SRHL_plots.push_back(pair<TH1F*, float>(p_ttzh.SRHL.TOTAL , roughSystTTZH    ));
  SRHL_plots.push_back(pair<TH1F*, float>(p_wz.SRHL.TOTAL   , roughSystWZ      ));
  SRHL_plots.push_back(pair<TH1F*, float>(p_ww.SRHL.TOTAL   , roughSystWW      ));
  SRHL_plots.push_back(pair<TH1F*, float>(p_xg.SRHL.TOTAL   , roughSystXG      ));
  SRHL_plots.push_back(pair<TH1F*, float>(p_rares.SRHL.TOTAL, roughSystRARES   ));
  SRHL_plots.push_back(pair<TH1F*, float>(p_flips.SRHL.TOTAL, roughSystFLIPS   ));
  SRHL_plots.push_back(pair<TH1F*, float>(p_fakes.SRHL.TOTAL, roughSystFAKESXL ));
  dataMCplotMaker(p_data.SRHL.TOTAL, SRHL_plots, titles, "H-L SRs", "data-driven", Form("--lumi %.2f --outputName HLSR.pdf --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.05 --noOverflow --systInclStat --noRatioPlot", lumiAG), vector <TH1F*>(), vector <string>(), colors);
  dataMCplotMaker(p_data.SRHL.TOTAL, SRHL_plots, titles, "H-L SRs", "data-driven", Form("--lumi %.2f --outputName HLSR_log.pdf --xAxisLabel SR --noXaxisUnit --legendUp -.05 --noOverflow --systInclStat --noRatioPlot", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > SRLL_plots;
  SRLL_plots.push_back(pair<TH1F*, float>(p_ttw.SRLL.TOTAL  , roughSystTTW     ));
  SRLL_plots.push_back(pair<TH1F*, float>(p_ttzh.SRLL.TOTAL , roughSystTTZH    ));
  SRLL_plots.push_back(pair<TH1F*, float>(p_wz.SRLL.TOTAL   , roughSystWZ      ));
  SRLL_plots.push_back(pair<TH1F*, float>(p_ww.SRLL.TOTAL   , roughSystWW      ));
  SRLL_plots.push_back(pair<TH1F*, float>(p_xg.SRLL.TOTAL   , roughSystXG      ));
  SRLL_plots.push_back(pair<TH1F*, float>(p_rares.SRLL.TOTAL, roughSystRARES   ));
  SRLL_plots.push_back(pair<TH1F*, float>(p_flips.SRLL.TOTAL, roughSystFLIPS   ));
  SRLL_plots.push_back(pair<TH1F*, float>(p_fakes.SRLL.TOTAL, roughSystFAKESXL ));
  dataMCplotMaker(p_data.SRLL.TOTAL, SRLL_plots, titles, "L-L SRs", "data-driven", Form("--lumi %.2f --outputName LLSR.pdf --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.05 --noOverflow --systInclStat --noRatioPlot", lumiAG), vector <TH1F*>(), vector <string>(), colors);
  dataMCplotMaker(p_data.SRLL.TOTAL, SRLL_plots, titles, "L-L SRs", "data-driven", Form("--lumi %.2f --outputName LLSR_log.pdf --xAxisLabel SR --noXaxisUnit --legendUp -.05 --noOverflow --systInclStat --noRatioPlot", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  //SR plots
  vector <TH1F*> SRHHMC_plots;
  SRHHMC_plots.push_back(p_ttw.SRHH.TOTAL  );
  SRHHMC_plots.push_back(p_ttzh.SRHH.TOTAL );
  SRHHMC_plots.push_back(p_wz.SRHH.TOTAL   );
  SRHHMC_plots.push_back(p_ww.SRHH.TOTAL   );
  SRHHMC_plots.push_back(p_xg.SRHH.TOTAL   );
  SRHHMC_plots.push_back(p_rares.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_ttbar_ff.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_dy_ff.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_wjets_ff.SRHH.TOTAL);
  SRHHMC_plots.push_back(p_st_ff.SRHH.TOTAL);
  dataMCplotMaker(p_data.SRHH.TOTAL, SRHHMC_plots, titles2, "H-H SRs", "from MC", Form("--lumi %.2f --outputName HHSRMC.pdf --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.05 --noOverflow", lumiAG), vector <TH1F*>(), vector <string>(), colors2);

  vector <TH1F*> SRHLMC_plots;
  SRHLMC_plots.push_back(p_ttw.SRHL.TOTAL  );
  SRHLMC_plots.push_back(p_ttzh.SRHL.TOTAL );
  SRHLMC_plots.push_back(p_wz.SRHL.TOTAL   );
  SRHLMC_plots.push_back(p_ww.SRHL.TOTAL   );
  SRHLMC_plots.push_back(p_xg.SRHL.TOTAL   );
  SRHLMC_plots.push_back(p_rares.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_ttbar_ff.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_dy_ff.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_wjets_ff.SRHL.TOTAL);
  SRHLMC_plots.push_back(p_st_ff.SRHL.TOTAL);
  dataMCplotMaker(p_data.SRHL.TOTAL, SRHLMC_plots, titles2, "H-L SRs", "from MC", Form("--lumi %.2f --outputName HLSRMC.pdf --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.05 --noOverflow", lumiAG), vector <TH1F*>(), vector <string>(), colors2);

  vector <TH1F*> SRLLMC_plots;
  SRLLMC_plots.push_back(p_ttw.SRLL.TOTAL  );
  SRLLMC_plots.push_back(p_ttzh.SRLL.TOTAL );
  SRLLMC_plots.push_back(p_wz.SRLL.TOTAL   );
  SRLLMC_plots.push_back(p_ww.SRLL.TOTAL   );
  SRLLMC_plots.push_back(p_xg.SRLL.TOTAL   );
  SRLLMC_plots.push_back(p_rares.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_ttbar_ff.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_dy_ff.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_wjets_ff.SRLL.TOTAL);
  SRLLMC_plots.push_back(p_st_ff.SRLL.TOTAL);
  dataMCplotMaker(p_data.SRLL.TOTAL, SRLLMC_plots, titles2, "L-L SRs", "from MC", Form("--lumi %.2f --outputName LLSRMC.pdf --xAxisLabel SR --noXaxisUnit --isLinear --legendUp -.05 --noOverflow", lumiAG), vector <TH1F*>(), vector <string>(), colors2);

  vector<pair<TH1F*, float> > type_plots;
  type_plots.push_back(pair<TH1F*, float>(p_ttw.h_type  , roughSystTTW   ));
  type_plots.push_back(pair<TH1F*, float>(p_ttzh.h_type , roughSystTTZH  ));
  type_plots.push_back(pair<TH1F*, float>(p_wz.h_type   , roughSystWZ    ));
  type_plots.push_back(pair<TH1F*, float>(p_ww.h_type   , roughSystWW    ));
  type_plots.push_back(pair<TH1F*, float>(p_xg.h_type   , roughSystXG    ));
  type_plots.push_back(pair<TH1F*, float>(p_rares.h_type, roughSystRARES ));
  type_plots.push_back(pair<TH1F*, float>(p_flips.h_type, roughSystFLIPS ));
  type_plots.push_back(pair<TH1F*, float>(p_fakes.h_type, roughSystFAKES ));
  dataMCplotMaker(p_data.h_type, type_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName type_all_SS.pdf --xAxisLabel type (mm, me, em, ee) --isLinear --legendUp -.05 --noDivisionLabel --noXaxisUnit --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > lep1_miniIso_plots;
  lep1_miniIso_plots.push_back(pair<TH1F*, float>(p_ttw.h_lep1_miniIso  , roughSystTTW   ));
  lep1_miniIso_plots.push_back(pair<TH1F*, float>(p_ttzh.h_lep1_miniIso , roughSystTTZH  ));
  lep1_miniIso_plots.push_back(pair<TH1F*, float>(p_wz.h_lep1_miniIso   , roughSystWZ    ));
  lep1_miniIso_plots.push_back(pair<TH1F*, float>(p_ww.h_lep1_miniIso   , roughSystWW    ));
  lep1_miniIso_plots.push_back(pair<TH1F*, float>(p_xg.h_lep1_miniIso   , roughSystXG    ));
  lep1_miniIso_plots.push_back(pair<TH1F*, float>(p_rares.h_lep1_miniIso, roughSystRARES ));
  lep1_miniIso_plots.push_back(pair<TH1F*, float>(p_flips.h_lep1_miniIso, roughSystFLIPS ));
  lep1_miniIso_plots.push_back(pair<TH1F*, float>(p_fakes.h_lep1_miniIso, roughSystFAKES ));
  dataMCplotMaker(p_data.h_lep1_miniIso, lep1_miniIso_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName lep1_miniIso_all_SS.pdf --xAxisLabel lep1_miniIso --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > lep2_miniIso_plots;
  lep2_miniIso_plots.push_back(pair<TH1F*, float>(p_ttw.h_lep2_miniIso  , roughSystTTW   ));
  lep2_miniIso_plots.push_back(pair<TH1F*, float>(p_ttzh.h_lep2_miniIso , roughSystTTZH  ));
  lep2_miniIso_plots.push_back(pair<TH1F*, float>(p_wz.h_lep2_miniIso   , roughSystWZ    ));
  lep2_miniIso_plots.push_back(pair<TH1F*, float>(p_ww.h_lep2_miniIso   , roughSystWW    ));
  lep2_miniIso_plots.push_back(pair<TH1F*, float>(p_xg.h_lep2_miniIso   , roughSystXG    ));
  lep2_miniIso_plots.push_back(pair<TH1F*, float>(p_rares.h_lep2_miniIso, roughSystRARES ));
  lep2_miniIso_plots.push_back(pair<TH1F*, float>(p_flips.h_lep2_miniIso, roughSystFLIPS ));
  lep2_miniIso_plots.push_back(pair<TH1F*, float>(p_fakes.h_lep2_miniIso, roughSystFAKES ));
  dataMCplotMaker(p_data.h_lep2_miniIso, lep2_miniIso_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName lep2_miniIso_all_SS.pdf --xAxisLabel lep2_miniIso --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > lep1_ptRel_plots;
  lep1_ptRel_plots.push_back(pair<TH1F*, float>(p_ttw.h_lep1_ptRel  , roughSystTTW   ));
  lep1_ptRel_plots.push_back(pair<TH1F*, float>(p_ttzh.h_lep1_ptRel , roughSystTTZH  ));
  lep1_ptRel_plots.push_back(pair<TH1F*, float>(p_wz.h_lep1_ptRel   , roughSystWZ    ));
  lep1_ptRel_plots.push_back(pair<TH1F*, float>(p_ww.h_lep1_ptRel   , roughSystWW    ));
  lep1_ptRel_plots.push_back(pair<TH1F*, float>(p_xg.h_lep1_ptRel   , roughSystXG    ));
  lep1_ptRel_plots.push_back(pair<TH1F*, float>(p_rares.h_lep1_ptRel, roughSystRARES ));
  lep1_ptRel_plots.push_back(pair<TH1F*, float>(p_flips.h_lep1_ptRel, roughSystFLIPS ));
  lep1_ptRel_plots.push_back(pair<TH1F*, float>(p_fakes.h_lep1_ptRel, roughSystFAKES ));
  dataMCplotMaker(p_data.h_lep1_ptRel, lep1_ptRel_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName lep1_ptRel_all_SS.pdf --xAxisLabel lep1_ptRel --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > lep2_ptRel_plots;
  lep2_ptRel_plots.push_back(pair<TH1F*, float>(p_ttw.h_lep2_ptRel  , roughSystTTW   ));
  lep2_ptRel_plots.push_back(pair<TH1F*, float>(p_ttzh.h_lep2_ptRel , roughSystTTZH  ));
  lep2_ptRel_plots.push_back(pair<TH1F*, float>(p_wz.h_lep2_ptRel   , roughSystWZ    ));
  lep2_ptRel_plots.push_back(pair<TH1F*, float>(p_ww.h_lep2_ptRel   , roughSystWW    ));
  lep2_ptRel_plots.push_back(pair<TH1F*, float>(p_xg.h_lep2_ptRel   , roughSystXG    ));
  lep2_ptRel_plots.push_back(pair<TH1F*, float>(p_rares.h_lep2_ptRel, roughSystRARES ));
  lep2_ptRel_plots.push_back(pair<TH1F*, float>(p_flips.h_lep2_ptRel, roughSystFLIPS ));
  lep2_ptRel_plots.push_back(pair<TH1F*, float>(p_fakes.h_lep2_ptRel, roughSystFAKES ));
  dataMCplotMaker(p_data.h_lep2_ptRel, lep2_ptRel_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName lep2_ptRel_all_SS.pdf --xAxisLabel lep2_ptRel --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > lep1_ptRatio_plots;
  lep1_ptRatio_plots.push_back(pair<TH1F*, float>(p_ttw.h_lep1_ptRatio  , roughSystTTW   ));
  lep1_ptRatio_plots.push_back(pair<TH1F*, float>(p_ttzh.h_lep1_ptRatio , roughSystTTZH  ));
  lep1_ptRatio_plots.push_back(pair<TH1F*, float>(p_wz.h_lep1_ptRatio   , roughSystWZ    ));
  lep1_ptRatio_plots.push_back(pair<TH1F*, float>(p_ww.h_lep1_ptRatio   , roughSystWW    ));
  lep1_ptRatio_plots.push_back(pair<TH1F*, float>(p_xg.h_lep1_ptRatio   , roughSystXG    ));
  lep1_ptRatio_plots.push_back(pair<TH1F*, float>(p_rares.h_lep1_ptRatio, roughSystRARES ));
  lep1_ptRatio_plots.push_back(pair<TH1F*, float>(p_flips.h_lep1_ptRatio, roughSystFLIPS ));
  lep1_ptRatio_plots.push_back(pair<TH1F*, float>(p_fakes.h_lep1_ptRatio, roughSystFAKES ));
  dataMCplotMaker(p_data.h_lep1_ptRatio, lep1_ptRatio_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName lep1_ptRatio_all_SS.pdf --xAxisLabel lep1_ptRatio --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > lep2_ptRatio_plots;
  lep2_ptRatio_plots.push_back(pair<TH1F*, float>(p_ttw.h_lep2_ptRatio  , roughSystTTW   ));
  lep2_ptRatio_plots.push_back(pair<TH1F*, float>(p_ttzh.h_lep2_ptRatio , roughSystTTZH  ));
  lep2_ptRatio_plots.push_back(pair<TH1F*, float>(p_wz.h_lep2_ptRatio   , roughSystWZ    ));
  lep2_ptRatio_plots.push_back(pair<TH1F*, float>(p_ww.h_lep2_ptRatio   , roughSystWW    ));
  lep2_ptRatio_plots.push_back(pair<TH1F*, float>(p_xg.h_lep2_ptRatio   , roughSystXG    ));
  lep2_ptRatio_plots.push_back(pair<TH1F*, float>(p_rares.h_lep2_ptRatio, roughSystRARES ));
  lep2_ptRatio_plots.push_back(pair<TH1F*, float>(p_flips.h_lep2_ptRatio, roughSystFLIPS ));
  lep2_ptRatio_plots.push_back(pair<TH1F*, float>(p_fakes.h_lep2_ptRatio, roughSystFAKES ));
  dataMCplotMaker(p_data.h_lep2_ptRatio, lep2_ptRatio_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName lep2_ptRatio_all_SS.pdf --xAxisLabel lep2_ptRatio --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors);
  
  //Redo iso with MC only
  vector <TH1F* > lep1_miniIso_plotsMC;
  lep1_miniIso_plotsMC.push_back(p_ttw.h_lep1_miniIso  );
  lep1_miniIso_plotsMC.push_back(p_ttzh.h_lep1_miniIso );
  lep1_miniIso_plotsMC.push_back(p_wz.h_lep1_miniIso   );
  lep1_miniIso_plotsMC.push_back(p_ww.h_lep1_miniIso   );
  lep1_miniIso_plotsMC.push_back(p_xg.h_lep1_miniIso   );
  lep1_miniIso_plotsMC.push_back(p_rares.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_ttbar_ff.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_dy_ff.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_wjets_ff.h_lep1_miniIso);
  lep1_miniIso_plotsMC.push_back(p_st_ff.h_lep1_miniIso);
  dataMCplotMaker(p_data.h_lep1_miniIso, lep1_miniIso_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName lep1_miniIso_all_SSMC.pdf --xAxisLabel lep1_miniIso --isLinear --legendUp -.05 --noXaxisUnit --noDivisionLabel ", lumiAG), vector <TH1F*>(), vector <string>(), colors2);

  vector <TH1F* > lep2_miniIso_plotsMC;
  lep2_miniIso_plotsMC.push_back(p_ttw.h_lep2_miniIso  );
  lep2_miniIso_plotsMC.push_back(p_ttzh.h_lep2_miniIso );
  lep2_miniIso_plotsMC.push_back(p_wz.h_lep2_miniIso   );
  lep2_miniIso_plotsMC.push_back(p_ww.h_lep2_miniIso   );
  lep2_miniIso_plotsMC.push_back(p_xg.h_lep2_miniIso   );
  lep2_miniIso_plotsMC.push_back(p_rares.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_ttbar_ff.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_dy_ff.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_wjets_ff.h_lep2_miniIso);
  lep2_miniIso_plotsMC.push_back(p_st_ff.h_lep2_miniIso);
  dataMCplotMaker(p_data.h_lep2_miniIso, lep2_miniIso_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName lep2_miniIso_all_SSMC.pdf --xAxisLabel lep2_miniIso --isLinear --legendUp -.05 --noXaxisUnit --noDivisionLabel", lumiAG), vector <TH1F*>(), vector <string>(), colors2);

  vector <TH1F* > lep1_ptRel_plotsMC;
  lep1_ptRel_plotsMC.push_back(p_ttw.h_lep1_ptRel  );
  lep1_ptRel_plotsMC.push_back(p_ttzh.h_lep1_ptRel );
  lep1_ptRel_plotsMC.push_back(p_wz.h_lep1_ptRel   );
  lep1_ptRel_plotsMC.push_back(p_ww.h_lep1_ptRel   );
  lep1_ptRel_plotsMC.push_back(p_xg.h_lep1_ptRel   );
  lep1_ptRel_plotsMC.push_back(p_rares.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_ttbar_ff.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_dy_ff.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_wjets_ff.h_lep1_ptRel);
  lep1_ptRel_plotsMC.push_back(p_st_ff.h_lep1_ptRel);
  dataMCplotMaker(p_data.h_lep1_ptRel, lep1_ptRel_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName lep1_ptRel_all_SSMC.pdf --xAxisLabel lep1_ptRel --isLinear --legendUp -.05 --noXaxisUnit --noDivisionLabel", lumiAG), vector <TH1F*>(), vector <string>(), colors2);

  vector <TH1F* > lep2_ptRel_plotsMC;
  lep2_ptRel_plotsMC.push_back(p_ttw.h_lep2_ptRel  );
  lep2_ptRel_plotsMC.push_back(p_ttzh.h_lep2_ptRel );
  lep2_ptRel_plotsMC.push_back(p_wz.h_lep2_ptRel   );
  lep2_ptRel_plotsMC.push_back(p_ww.h_lep2_ptRel   );
  lep2_ptRel_plotsMC.push_back(p_xg.h_lep2_ptRel   );
  lep2_ptRel_plotsMC.push_back(p_rares.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_ttbar_ff.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_dy_ff.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_wjets_ff.h_lep2_ptRel);
  lep2_ptRel_plotsMC.push_back(p_st_ff.h_lep2_ptRel);
  dataMCplotMaker(p_data.h_lep2_ptRel, lep2_ptRel_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName lep2_ptRel_all_SSMC.pdf --xAxisLabel lep2_ptRel --isLinear --legendUp -.05 --noXaxisUnit --noDivisionLabel", lumiAG), vector <TH1F*>(), vector <string>(), colors2);

  vector <TH1F* > lep1_ptRatio_plotsMC;
  lep1_ptRatio_plotsMC.push_back(p_ttw.h_lep1_ptRatio  );
  lep1_ptRatio_plotsMC.push_back(p_ttzh.h_lep1_ptRatio );
  lep1_ptRatio_plotsMC.push_back(p_wz.h_lep1_ptRatio   );
  lep1_ptRatio_plotsMC.push_back(p_ww.h_lep1_ptRatio   );
  lep1_ptRatio_plotsMC.push_back(p_xg.h_lep1_ptRatio   );
  lep1_ptRatio_plotsMC.push_back(p_rares.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_ttbar_ff.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_dy_ff.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_wjets_ff.h_lep1_ptRatio);
  lep1_ptRatio_plotsMC.push_back(p_st_ff.h_lep1_ptRatio);
  dataMCplotMaker(p_data.h_lep1_ptRatio, lep1_ptRatio_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName lep1_ptRatio_all_SSMC.pdf --xAxisLabel lep1_ptRatio --isLinear --legendUp -.05 --noXaxisUnit --noDivisionLabel", lumiAG), vector <TH1F*>(), vector <string>(), colors2);

  vector <TH1F* > lep2_ptRatio_plotsMC;
  lep2_ptRatio_plotsMC.push_back(p_ttw.h_lep2_ptRatio  );
  lep2_ptRatio_plotsMC.push_back(p_ttzh.h_lep2_ptRatio );
  lep2_ptRatio_plotsMC.push_back(p_wz.h_lep2_ptRatio   );
  lep2_ptRatio_plotsMC.push_back(p_ww.h_lep2_ptRatio   );
  lep2_ptRatio_plotsMC.push_back(p_xg.h_lep2_ptRatio   );
  lep2_ptRatio_plotsMC.push_back(p_rares.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_ttbar_ff.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_dy_ff.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_wjets_ff.h_lep2_ptRatio);
  lep2_ptRatio_plotsMC.push_back(p_st_ff.h_lep2_ptRatio);
  dataMCplotMaker(p_data.h_lep2_ptRatio, lep2_ptRatio_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName lep2_ptRatio_all_SSMC.pdf --xAxisLabel lep2_ptRatio --isLinear --legendUp -.05 --noXaxisUnit --noDivisionLabel", lumiAG), vector <TH1F*>(), vector <string>(), colors2);

  vector<pair<TH1F*, float> > ht_plots;
  ht_plots.push_back(pair<TH1F*, float>(p_ttw.h_ht  , roughSystTTW   ));
  ht_plots.push_back(pair<TH1F*, float>(p_ttzh.h_ht , roughSystTTZH  ));
  ht_plots.push_back(pair<TH1F*, float>(p_wz.h_ht   , roughSystWZ    ));
  ht_plots.push_back(pair<TH1F*, float>(p_ww.h_ht   , roughSystWW    ));
  ht_plots.push_back(pair<TH1F*, float>(p_xg.h_ht   , roughSystXG    ));
  ht_plots.push_back(pair<TH1F*, float>(p_rares.h_ht, roughSystRARES ));
  ht_plots.push_back(pair<TH1F*, float>(p_flips.h_ht, roughSystFLIPS ));
  ht_plots.push_back(pair<TH1F*, float>(p_fakes.h_ht, roughSystFAKES ));
  dataMCplotMaker(p_data.h_ht, ht_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName ht_all_SS.pdf --xAxisLabel HT --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > met_plots;
  met_plots.push_back(pair<TH1F*, float>(p_ttw.h_met  , roughSystTTW   ));
  met_plots.push_back(pair<TH1F*, float>(p_ttzh.h_met , roughSystTTZH  ));
  met_plots.push_back(pair<TH1F*, float>(p_wz.h_met   , roughSystWZ    ));
  met_plots.push_back(pair<TH1F*, float>(p_ww.h_met   , roughSystWW    ));
  met_plots.push_back(pair<TH1F*, float>(p_xg.h_met   , roughSystXG    ));
  met_plots.push_back(pair<TH1F*, float>(p_rares.h_met, roughSystRARES ));
  met_plots.push_back(pair<TH1F*, float>(p_flips.h_met, roughSystFLIPS ));
  met_plots.push_back(pair<TH1F*, float>(p_fakes.h_met, roughSystFAKES ));
  dataMCplotMaker(p_data.h_met, met_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName met_all_SS.pdf --xAxisLabel MET --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > dxy_plots;
  dxy_plots.push_back(pair<TH1F*, float>(p_ttw.h_dxy  , roughSystTTW   ));
  dxy_plots.push_back(pair<TH1F*, float>(p_ttzh.h_dxy , roughSystTTZH  ));
  dxy_plots.push_back(pair<TH1F*, float>(p_wz.h_dxy   , roughSystWZ    ));
  dxy_plots.push_back(pair<TH1F*, float>(p_ww.h_dxy   , roughSystWW    ));
  dxy_plots.push_back(pair<TH1F*, float>(p_xg.h_dxy   , roughSystXG    ));
  dxy_plots.push_back(pair<TH1F*, float>(p_rares.h_dxy, roughSystRARES ));
  dxy_plots.push_back(pair<TH1F*, float>(p_flips.h_dxy, roughSystFLIPS ));
  dxy_plots.push_back(pair<TH1F*, float>(p_fakes.h_dxy, roughSystFAKES ));
  dataMCplotMaker(p_data.h_met, dxy_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName dxy_all_SS.pdf --xAxisLabel dxy --isLinear --legendUp -.05 --noDivisionLabel --noXaxisUnit", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > dz_plots;
  dz_plots.push_back(pair<TH1F*, float>(p_ttw.h_dz  , roughSystTTW   ));
  dz_plots.push_back(pair<TH1F*, float>(p_ttzh.h_dz , roughSystTTZH  ));
  dz_plots.push_back(pair<TH1F*, float>(p_wz.h_dz   , roughSystWZ    ));
  dz_plots.push_back(pair<TH1F*, float>(p_ww.h_dz   , roughSystWW    ));
  dz_plots.push_back(pair<TH1F*, float>(p_xg.h_dz   , roughSystXG    ));
  dz_plots.push_back(pair<TH1F*, float>(p_rares.h_dz, roughSystRARES ));
  dz_plots.push_back(pair<TH1F*, float>(p_flips.h_dz, roughSystFLIPS ));
  dz_plots.push_back(pair<TH1F*, float>(p_fakes.h_dz, roughSystFAKES ));
  dataMCplotMaker(p_data.h_dz, dz_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName dz_all_SS.pdf --xAxisLabel dz --isLinear --legendUp -.05 --noDivisionLabel --noXaxisUnit", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > sip3d_plots;
  sip3d_plots.push_back(pair<TH1F*, float>(p_ttw.h_sip3d  , roughSystTTW   ));
  sip3d_plots.push_back(pair<TH1F*, float>(p_ttzh.h_sip3d , roughSystTTZH  ));
  sip3d_plots.push_back(pair<TH1F*, float>(p_wz.h_sip3d   , roughSystWZ    ));
  sip3d_plots.push_back(pair<TH1F*, float>(p_ww.h_sip3d   , roughSystWW    ));
  sip3d_plots.push_back(pair<TH1F*, float>(p_xg.h_sip3d   , roughSystXG    ));
  sip3d_plots.push_back(pair<TH1F*, float>(p_rares.h_sip3d, roughSystRARES ));
  sip3d_plots.push_back(pair<TH1F*, float>(p_flips.h_sip3d, roughSystFLIPS ));
  sip3d_plots.push_back(pair<TH1F*, float>(p_fakes.h_sip3d, roughSystFAKES ));
  dataMCplotMaker(p_data.h_sip3d, sip3d_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName sip3d_all_SS.pdf --xAxisLabel SIP3D --isLinear --legendUp -.05 --noDivisionLabel --noXaxisUnit", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > mva_plots;
  mva_plots.push_back(pair<TH1F*, float>(p_ttw.h_mva  , roughSystTTW   ));
  mva_plots.push_back(pair<TH1F*, float>(p_ttzh.h_mva , roughSystTTZH  ));
  mva_plots.push_back(pair<TH1F*, float>(p_wz.h_mva   , roughSystWZ    ));
  mva_plots.push_back(pair<TH1F*, float>(p_ww.h_mva   , roughSystWW    ));
  mva_plots.push_back(pair<TH1F*, float>(p_xg.h_mva   , roughSystXG    ));
  mva_plots.push_back(pair<TH1F*, float>(p_rares.h_mva, roughSystRARES ));
  mva_plots.push_back(pair<TH1F*, float>(p_flips.h_mva, roughSystFLIPS ));
  mva_plots.push_back(pair<TH1F*, float>(p_fakes.h_mva, roughSystFAKES ));
  dataMCplotMaker(p_data.h_mva, mva_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName mva_all_SS.pdf --xAxisLabel MVA --isLinear --legendUp -.05 --noDivisionLabel --noXaxisUnit", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > mll_plots;
  mll_plots.push_back(pair<TH1F*, float>(p_ttw.h_mll  , roughSystTTW   ));
  mll_plots.push_back(pair<TH1F*, float>(p_ttzh.h_mll , roughSystTTZH  ));
  mll_plots.push_back(pair<TH1F*, float>(p_wz.h_mll   , roughSystWZ    ));
  mll_plots.push_back(pair<TH1F*, float>(p_ww.h_mll   , roughSystWW    ));
  mll_plots.push_back(pair<TH1F*, float>(p_xg.h_mll   , roughSystXG    ));
  mll_plots.push_back(pair<TH1F*, float>(p_rares.h_mll, roughSystRARES ));
  mll_plots.push_back(pair<TH1F*, float>(p_flips.h_mll, roughSystFLIPS ));
  mll_plots.push_back(pair<TH1F*, float>(p_fakes.h_mll, roughSystFAKES ));
  dataMCplotMaker(p_data.h_mll, mll_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName mll_all_SS.pdf --xAxisLabel M_{LL} --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > mtmin_plots;
  mtmin_plots.push_back(pair<TH1F*, float>(p_ttw.h_mtmin  , roughSystTTW   ));
  mtmin_plots.push_back(pair<TH1F*, float>(p_ttzh.h_mtmin , roughSystTTZH  ));
  mtmin_plots.push_back(pair<TH1F*, float>(p_wz.h_mtmin   , roughSystWZ    ));
  mtmin_plots.push_back(pair<TH1F*, float>(p_ww.h_mtmin   , roughSystWW    ));
  mtmin_plots.push_back(pair<TH1F*, float>(p_xg.h_mtmin   , roughSystXG    ));
  mtmin_plots.push_back(pair<TH1F*, float>(p_rares.h_mtmin, roughSystRARES ));
  mtmin_plots.push_back(pair<TH1F*, float>(p_flips.h_mtmin, roughSystFLIPS ));
  mtmin_plots.push_back(pair<TH1F*, float>(p_fakes.h_mtmin, roughSystFAKES ));
  dataMCplotMaker(p_data.h_mtmin, mtmin_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName mtmin_all_SS.pdf --xAxisLabel MT^{min} --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > njets_plots;
  njets_plots.push_back(pair<TH1F*, float>(p_ttw.h_njets  , roughSystTTW   ));
  njets_plots.push_back(pair<TH1F*, float>(p_ttzh.h_njets , roughSystTTZH  ));
  njets_plots.push_back(pair<TH1F*, float>(p_wz.h_njets   , roughSystWZ    ));
  njets_plots.push_back(pair<TH1F*, float>(p_ww.h_njets   , roughSystWW    ));
  njets_plots.push_back(pair<TH1F*, float>(p_xg.h_njets   , roughSystXG    ));
  njets_plots.push_back(pair<TH1F*, float>(p_rares.h_njets, roughSystRARES ));
  njets_plots.push_back(pair<TH1F*, float>(p_flips.h_njets, roughSystFLIPS ));
  njets_plots.push_back(pair<TH1F*, float>(p_fakes.h_njets, roughSystFAKES ));
  dataMCplotMaker(p_data.h_njets, njets_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName njets_all_SS.pdf --xAxisLabel Number of Jets --noXaxisUnit --noDivisionLabel --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector<pair<TH1F*, float> > nbtags_plots;
  nbtags_plots.push_back(pair<TH1F*, float>(p_ttw.h_nbtags  , roughSystTTW   ));
  nbtags_plots.push_back(pair<TH1F*, float>(p_ttzh.h_nbtags , roughSystTTZH  ));
  nbtags_plots.push_back(pair<TH1F*, float>(p_wz.h_nbtags   , roughSystWZ    ));
  nbtags_plots.push_back(pair<TH1F*, float>(p_ww.h_nbtags   , roughSystWW    ));
  nbtags_plots.push_back(pair<TH1F*, float>(p_xg.h_nbtags   , roughSystXG    ));
  nbtags_plots.push_back(pair<TH1F*, float>(p_rares.h_nbtags, roughSystRARES ));
  nbtags_plots.push_back(pair<TH1F*, float>(p_flips.h_nbtags, roughSystFLIPS ));
  nbtags_plots.push_back(pair<TH1F*, float>(p_fakes.h_nbtags, roughSystFAKES ));
  dataMCplotMaker(p_data.h_nbtags, nbtags_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName nbtags_all_SS.pdf --noDivisionLabel --noXaxisUnit --xAxisLabel Number of BTags --noXaxisUnit --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  //lep1 pt plots
  vector<pair<TH1F*, float> > l1pt_plots;
  l1pt_plots.push_back(pair<TH1F*, float>(p_ttw.h_l1pt  , roughSystTTW   ));
  l1pt_plots.push_back(pair<TH1F*, float>(p_ttzh.h_l1pt , roughSystTTZH  ));
  l1pt_plots.push_back(pair<TH1F*, float>(p_wz.h_l1pt   , roughSystWZ    ));
  l1pt_plots.push_back(pair<TH1F*, float>(p_ww.h_l1pt   , roughSystWW    ));
  l1pt_plots.push_back(pair<TH1F*, float>(p_xg.h_l1pt   , roughSystXG    ));
  l1pt_plots.push_back(pair<TH1F*, float>(p_rares.h_l1pt, roughSystRARES ));
  l1pt_plots.push_back(pair<TH1F*, float>(p_flips.h_l1pt, roughSystFLIPS ));
  l1pt_plots.push_back(pair<TH1F*, float>(p_fakes.h_l1pt, roughSystFAKES ));
  dataMCplotMaker(p_data.h_l1pt, l1pt_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName l1pt_all_SS.pdf --xAxisLabel Leading Lepton pT --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector <TH1F* > l1pt_plotsMC;
  l1pt_plotsMC.push_back(p_ttw.h_l1pt  );
  l1pt_plotsMC.push_back(p_ttzh.h_l1pt );
  l1pt_plotsMC.push_back(p_wz.h_l1pt   );
  l1pt_plotsMC.push_back(p_ww.h_l1pt   );
  l1pt_plotsMC.push_back(p_xg.h_l1pt   );
  l1pt_plotsMC.push_back(p_rares.h_l1pt);
  l1pt_plotsMC.push_back(p_ttbar_ff.h_l1pt);
  l1pt_plotsMC.push_back(p_dy_ff.h_l1pt);
  l1pt_plotsMC.push_back(p_wjets_ff.h_l1pt);
  l1pt_plotsMC.push_back(p_st_ff.h_l1pt);
  dataMCplotMaker(p_data.h_l1pt, l1pt_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName l1pt_all_SSMC.pdf --xAxisLabel Leading Lepton pT --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors2);
 
  //lep2 pt plots
  vector<pair<TH1F*, float> > l2pt_plots;
  l2pt_plots.push_back(pair<TH1F*, float>(p_ttw.h_l2pt  , roughSystTTW   ));
  l2pt_plots.push_back(pair<TH1F*, float>(p_ttzh.h_l2pt , roughSystTTZH  ));
  l2pt_plots.push_back(pair<TH1F*, float>(p_wz.h_l2pt   , roughSystWZ    ));
  l2pt_plots.push_back(pair<TH1F*, float>(p_ww.h_l2pt   , roughSystWW    ));
  l2pt_plots.push_back(pair<TH1F*, float>(p_xg.h_l2pt   , roughSystXG    ));
  l2pt_plots.push_back(pair<TH1F*, float>(p_rares.h_l2pt, roughSystRARES ));
  l2pt_plots.push_back(pair<TH1F*, float>(p_flips.h_l2pt, roughSystFLIPS ));
  l2pt_plots.push_back(pair<TH1F*, float>(p_fakes.h_l2pt, roughSystFAKES ));
  dataMCplotMaker(p_data.h_l2pt, l2pt_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName l2pt_all_SS.pdf --xAxisLabel Trailing Lepton pT --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector <TH1F*> l2pt_plotsMC;
  l2pt_plotsMC.push_back(p_ttw.h_l2pt  );
  l2pt_plotsMC.push_back(p_ttzh.h_l2pt );
  l2pt_plotsMC.push_back(p_wz.h_l2pt   );
  l2pt_plotsMC.push_back(p_ww.h_l2pt   );
  l2pt_plotsMC.push_back(p_xg.h_l2pt   );
  l2pt_plotsMC.push_back(p_rares.h_l2pt);
  l2pt_plotsMC.push_back(p_ttbar_ff.h_l2pt);
  l2pt_plotsMC.push_back(p_dy_ff.h_l2pt);
  l2pt_plotsMC.push_back(p_wjets_ff.h_l2pt);
  l2pt_plotsMC.push_back(p_st_ff.h_l2pt);
  dataMCplotMaker(p_data.h_l2pt, l2pt_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName l2pt_all_SSMC.pdf --xAxisLabel Trailing Lepton pT --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors2); 

  //lep1 eta plots
  vector<pair<TH1F*, float> > l1eta_plots;
  l1eta_plots.push_back(pair<TH1F*, float>(p_ttw.h_l1eta  , roughSystTTW   ));
  l1eta_plots.push_back(pair<TH1F*, float>(p_ttzh.h_l1eta , roughSystTTZH  ));
  l1eta_plots.push_back(pair<TH1F*, float>(p_wz.h_l1eta   , roughSystWZ    ));
  l1eta_plots.push_back(pair<TH1F*, float>(p_ww.h_l1eta   , roughSystWW    ));
  l1eta_plots.push_back(pair<TH1F*, float>(p_xg.h_l1eta   , roughSystXG    ));
  l1eta_plots.push_back(pair<TH1F*, float>(p_rares.h_l1eta, roughSystRARES ));
  l1eta_plots.push_back(pair<TH1F*, float>(p_flips.h_l1eta, roughSystFLIPS ));
  l1eta_plots.push_back(pair<TH1F*, float>(p_fakes.h_l1eta, roughSystFAKES ));
  dataMCplotMaker(p_data.h_l1eta, l1eta_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName l1eta_all_SS.pdf --xAxisLabel Leading Lepton #eta --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector <TH1F* > l1eta_plotsMC;
  l1eta_plotsMC.push_back(p_ttw.h_l1eta  );
  l1eta_plotsMC.push_back(p_ttzh.h_l1eta );
  l1eta_plotsMC.push_back(p_wz.h_l1eta   );
  l1eta_plotsMC.push_back(p_ww.h_l1eta   );
  l1eta_plotsMC.push_back(p_xg.h_l1eta   );
  l1eta_plotsMC.push_back(p_rares.h_l1eta);
  l1eta_plotsMC.push_back(p_ttbar_ff.h_l1eta);
  l1eta_plotsMC.push_back(p_dy_ff.h_l1eta);
  l1eta_plotsMC.push_back(p_wjets_ff.h_l1eta);
  l1eta_plotsMC.push_back(p_st_ff.h_l1eta);
  dataMCplotMaker(p_data.h_l1eta, l1eta_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName l1eta_all_SSMC.pdf --xAxisLabel Leading Lepton #eta --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors2);
 
  //lep2 eta plots
  vector<pair<TH1F*, float> > l2eta_plots;
  l2eta_plots.push_back(pair<TH1F*, float>(p_ttw.h_l2eta  , roughSystTTW   ));
  l2eta_plots.push_back(pair<TH1F*, float>(p_ttzh.h_l2eta , roughSystTTZH  ));
  l2eta_plots.push_back(pair<TH1F*, float>(p_wz.h_l2eta   , roughSystWZ    ));
  l2eta_plots.push_back(pair<TH1F*, float>(p_ww.h_l2eta   , roughSystWW    ));
  l2eta_plots.push_back(pair<TH1F*, float>(p_xg.h_l2eta   , roughSystXG    ));
  l2eta_plots.push_back(pair<TH1F*, float>(p_rares.h_l2eta, roughSystRARES ));
  l2eta_plots.push_back(pair<TH1F*, float>(p_flips.h_l2eta, roughSystFLIPS ));
  l2eta_plots.push_back(pair<TH1F*, float>(p_fakes.h_l2eta, roughSystFAKES ));
  dataMCplotMaker(p_data.h_l2eta, l2eta_plots, titles, "", "SS Baseline", Form("--lumi %.2f --outputName l2eta_all_SS.pdf --xAxisLabel Trailing Lepton #eta --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors);

  vector <TH1F*> l2eta_plotsMC;
  l2eta_plotsMC.push_back(p_ttw.h_l2eta  );
  l2eta_plotsMC.push_back(p_ttzh.h_l2eta );
  l2eta_plotsMC.push_back(p_wz.h_l2eta   );
  l2eta_plotsMC.push_back(p_ww.h_l2eta   );
  l2eta_plotsMC.push_back(p_xg.h_l2eta   );
  l2eta_plotsMC.push_back(p_rares.h_l2eta);
  l2eta_plotsMC.push_back(p_ttbar_ff.h_l2eta);
  l2eta_plotsMC.push_back(p_dy_ff.h_l2eta);
  l2eta_plotsMC.push_back(p_wjets_ff.h_l2eta);
  l2eta_plotsMC.push_back(p_st_ff.h_l2eta);
  dataMCplotMaker(p_data.h_l2eta, l2eta_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName l2eta_all_SSMC.pdf --xAxisLabel Trailing Lepton #eta --isLinear --legendUp -.05", lumiAG), vector <TH1F*>(), vector <string>(), colors2); 

  vector <TH1F* > nleps_plotsMC;
  nleps_plotsMC.push_back(p_ttw.h_nleps  );
  nleps_plotsMC.push_back(p_ttzh.h_nleps );
  nleps_plotsMC.push_back(p_wz.h_nleps   );
  nleps_plotsMC.push_back(p_ww.h_nleps   );
  nleps_plotsMC.push_back(p_xg.h_nleps   );
  nleps_plotsMC.push_back(p_rares.h_nleps);
  nleps_plotsMC.push_back(p_ttbar_ff.h_nleps);
  nleps_plotsMC.push_back(p_dy_ff.h_nleps);
  nleps_plotsMC.push_back(p_wjets_ff.h_nleps);
  nleps_plotsMC.push_back(p_st_ff.h_nleps);
  dataMCplotMaker(p_data.h_nleps, nleps_plotsMC, titles2, "from MC", "SS Baseline", Form("--lumi %.2f --outputName nleps_all_SSMC.pdf --xAxisLabel N_{leptons} --isLinear --legendUp -.05 --systInclStat --dontShowZeroRatios --topYaxisTitle data/pred", lumiAG), vector <TH1F*>(), vector <string>(), colors2);


}

//doFakes = 1 for QCD, 2 for inSitu
pair<yields_t, plots_t> run(TChain *chain, bool isData, bool doFlips, int doFakes, int exclude, bool isSignal, bool isGamma){

  TString chainTitle = chain->GetTitle();
  const char* chainTitleCh = chainTitle.Data();

  bool isWZ = (chainTitle=="wz");

  bool isFastSimSignal = false;
  vector<float> mysparms;
  if (isSignal && chainTitle.Contains("fs_")) {
    isFastSimSignal = true;
    TObjArray *tx = chainTitle.Tokenize("_");
    for (int i = 0; i < tx->GetEntries(); i++) {
      if (((TObjString *)(tx->At(i)))->String().BeginsWith("m")) {
	float asparm = ((TObjString *)(tx->At(i)))->String().ReplaceAll("m","").Atof();
	//std::cout << asparm << std::endl;
	mysparms.push_back(asparm);
      }
    }
  }

  yields_t y_result;
  plots_t  p_result;

  //Initialize
  y_result.EE    = 0;
  y_result.EM    = 0;
  y_result.MM    = 0;
  y_result.TOTAL = 0;

  p_result.h_dxy          = new TH1F(Form("dxy_%s"         , chainTitleCh) , Form("dxy_%s"         , chainTitleCh), 20, 0   ,  0.02); p_result.h_dxy         ->Sumw2();
  p_result.h_dz           = new TH1F(Form("dz_%s"          , chainTitleCh) , Form("dz_%s"          , chainTitleCh), 20, 0   , 0.03); p_result.h_dz          ->Sumw2();
  p_result.h_sip3d        = new TH1F(Form("sip3d_%s"       , chainTitleCh) , Form("sip3d_%s"       , chainTitleCh), 20, 0   ,   5); p_result.h_sip3d       ->Sumw2();
  p_result.h_mva          = new TH1F(Form("mva_%s"         , chainTitleCh) , Form("mva_%s"         , chainTitleCh), 20, 0   , 1.5); p_result.h_mva         ->Sumw2();
  p_result.h_ht           = new TH1F(Form("ht_%s"          , chainTitleCh) , Form("ht_%s"          , chainTitleCh), 15, 0   , 600); p_result.h_ht          ->Sumw2();
  p_result.h_met          = new TH1F(Form("met_%s"         , chainTitleCh) , Form("met_%s"         , chainTitleCh), 10, 0   , 200); p_result.h_met         ->Sumw2();
  p_result.h_mll          = new TH1F(Form("mll_%s"         , chainTitleCh) , Form("mll_%s"         , chainTitleCh), 10, 0   , 200); p_result.h_mll         ->Sumw2();
  p_result.h_mtmin        = new TH1F(Form("mtmin_%s"       , chainTitleCh) , Form("mtmin_%s"       , chainTitleCh), 10, 0   , 200); p_result.h_mtmin       ->Sumw2();
  p_result.h_njets        = new TH1F(Form("njets_%s"       , chainTitleCh) , Form("njets_%s"       , chainTitleCh), 8 , 0   , 8);   p_result.h_njets       ->Sumw2();
  p_result.h_nleps        = new TH1F(Form("nleps_%s"        , chainTitleCh) , Form("nleps_%s"        , chainTitleCh) , 6   , 0 , 6);    p_result.h_nleps        ->Sumw2();
  p_result.h_nbtags       = new TH1F(Form("nbtags_%s"      , chainTitleCh) , Form("nbtags_%s"      , chainTitleCh), 6 , 0   , 6);   p_result.h_nbtags      ->Sumw2();
  p_result.h_l1pt         = new TH1F(Form("l1pt_%s"        , chainTitleCh) , Form("l1pt_%s"        , chainTitleCh), 10, 0   , 200); p_result.h_l1pt        ->Sumw2();
  p_result.h_l2pt         = new TH1F(Form("l2pt_%s"        , chainTitleCh) , Form("l2pt_%s"        , chainTitleCh), 10, 0   , 100); p_result.h_l2pt        ->Sumw2();
  p_result.h_l1eta        = new TH1F(Form("l1eta_%s"      , chainTitleCh) , Form("l1eta_%s"        , chainTitleCh), 24, -2.5, 2.5); p_result.h_l1eta       ->Sumw2();
  p_result.h_l2eta        = new TH1F(Form("l2eta_%s"      , chainTitleCh) , Form("l2eta_%s"        , chainTitleCh), 24, -2.5, 2.5); p_result.h_l2eta       ->Sumw2();
  p_result.SRHH.EE        = new TH1F(Form("SRHH_EE_%s"     , chainTitleCh) , Form("SRHH_EE_%s"     , chainTitleCh), 32, 1   , 33);  p_result.SRHH.EE       ->Sumw2();
  p_result.SRHH.EM        = new TH1F(Form("SRHH_EM_%s"     , chainTitleCh) , Form("SRHH_EM_%s"     , chainTitleCh), 32, 1   , 33);  p_result.SRHH.EM       ->Sumw2();
  p_result.SRHH.MM        = new TH1F(Form("SRHH_MM_%s"     , chainTitleCh) , Form("SRHH_MM_%s"     , chainTitleCh), 32, 1   , 33);  p_result.SRHH.MM       ->Sumw2();
  p_result.SRHH.TOTAL     = new TH1F(Form("SRHH_TOTAL_%s"  , chainTitleCh) , Form("SRHH_TOTAL_%s"  , chainTitleCh), 32, 1   , 33);  p_result.SRHH.TOTAL    ->Sumw2();
  p_result.SRHL.EE        = new TH1F(Form("SRHL_EE_%s"     , chainTitleCh) , Form("SRHL_EE_%s"     , chainTitleCh), 26, 1   , 27);  p_result.SRHL.EE       ->Sumw2();
  p_result.SRHL.EM        = new TH1F(Form("SRHL_EM_%s"     , chainTitleCh) , Form("SRHL_EM_%s"     , chainTitleCh), 26, 1   , 27);  p_result.SRHL.EM       ->Sumw2();
  p_result.SRHL.MM        = new TH1F(Form("SRHL_MM_%s"     , chainTitleCh) , Form("SRHL_MM_%s"     , chainTitleCh), 26, 1   , 27);  p_result.SRHL.MM       ->Sumw2();
  p_result.SRHL.TOTAL     = new TH1F(Form("SRHL_TOTAL_%s"  , chainTitleCh) , Form("SRHL_TOTAL_%s"  , chainTitleCh), 26, 1   , 27);  p_result.SRHL.TOTAL    ->Sumw2();
  p_result.SRLL.EE        = new TH1F(Form("SRLL_EE_%s"     , chainTitleCh) , Form("SRLL_EE_%s"     , chainTitleCh), 8 , 1   , 9);   p_result.SRLL.EE       ->Sumw2();
  p_result.SRLL.EM        = new TH1F(Form("SRLL_EM_%s"     , chainTitleCh) , Form("SRLL_EM_%s"     , chainTitleCh), 8 , 1   , 9);   p_result.SRLL.EM       ->Sumw2();
  p_result.SRLL.MM        = new TH1F(Form("SRLL_MM_%s"     , chainTitleCh) , Form("SRLL_MM_%s"     , chainTitleCh), 8 , 1   , 9);   p_result.SRLL.MM       ->Sumw2();
  p_result.SRLL.TOTAL     = new TH1F(Form("SRLL_TOTAL_%s"  , chainTitleCh) , Form("SRLL_TOTAL_%s"  , chainTitleCh), 8 , 1   , 9);   p_result.SRLL.TOTAL    ->Sumw2();
  p_result.h_type         = new TH1F(Form("type_%s"        , chainTitleCh) , Form("type_%s"        , chainTitleCh), 4 , 0   , 4);   p_result.h_type        ->Sumw2();
  p_result.h_lep1_miniIso = new TH1F(Form("lep1_miniIso_%s", chainTitleCh) , Form("lep1_miniIso_%s", chainTitleCh), 30, 0   , 0.2); p_result.h_lep1_miniIso->Sumw2();
  p_result.h_lep2_miniIso = new TH1F(Form("lep2_miniIso_%s", chainTitleCh) , Form("lep2_miniIso_%s", chainTitleCh), 30, 0   , 0.2); p_result.h_lep2_miniIso->Sumw2();
  p_result.h_lep1_ptRatio = new TH1F(Form("lep1_ptRatio_%s", chainTitleCh) , Form("lep1_ptRatio_%s", chainTitleCh), 30, 0   , 1.5); p_result.h_lep1_ptRatio->Sumw2();
  p_result.h_lep2_ptRatio = new TH1F(Form("lep2_ptRatio_%s", chainTitleCh) , Form("lep2_ptRatio_%s", chainTitleCh), 30, 0   , 1.5); p_result.h_lep2_ptRatio->Sumw2();
  p_result.h_lep1_ptRel   = new TH1F(Form("lep1_ptRel_%s"  , chainTitleCh) , Form("lep1_ptRel_%s"  , chainTitleCh), 25, 0   , 50);  p_result.h_lep1_ptRel  ->Sumw2();
  p_result.h_lep2_ptRel   = new TH1F(Form("lep2_ptRel_%s"  , chainTitleCh) , Form("lep2_ptRel_%s"  , chainTitleCh), 25, 0   , 50);  p_result.h_lep2_ptRel  ->Sumw2();

  //For FR variations
  plots_t p_fake_alt_up;
  if (doFakes == 1) {
    p_fake_alt_up.SRHH.TOTAL        = new TH1F(Form("SRHH_FR_TOTAL_%s"      , chainTitleCh) , Form("SRHH_FR_TOTAL_%s"      , chainTitleCh) , 32  , 1 , 33); p_fake_alt_up.SRHH.TOTAL->Sumw2();
    p_fake_alt_up.SRHL.TOTAL        = new TH1F(Form("SRHL_FR_TOTAL_%s"      , chainTitleCh) , Form("SRHL_FR_TOTAL_%s"      , chainTitleCh) , 26  , 1 , 27); p_fake_alt_up.SRHL.TOTAL->Sumw2();
    p_fake_alt_up.SRLL.TOTAL        = new TH1F(Form("SRLL_FR_TOTAL_%s"      , chainTitleCh) , Form("SRLL_FR_TOTAL_%s"      , chainTitleCh) , 8   , 1 , 9);  p_fake_alt_up.SRLL.TOTAL->Sumw2();
  } else {
    p_fake_alt_up.SRHH.TOTAL    = 0;
    p_fake_alt_up.SRHL.TOTAL    = 0;
    p_fake_alt_up.SRLL.TOTAL    = 0;
  }

  //For JES variations
  plots_t p_jes_alt_up;
  plots_t p_jes_alt_dn;
  if (doFakes == 1 || isData==0) {
    p_jes_alt_up.SRHH.TOTAL     = new TH1F(Form("SRHH_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JES_UP_TOTAL_%s"   , chainTitleCh) , 32  , 1 , 33);p_jes_alt_up.SRHH.TOTAL->Sumw2();
    p_jes_alt_up.SRHL.TOTAL     = new TH1F(Form("SRHL_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JES_UP_TOTAL_%s"   , chainTitleCh) , 26  , 1 , 27);p_jes_alt_up.SRHL.TOTAL->Sumw2();
    p_jes_alt_up.SRLL.TOTAL     = new TH1F(Form("SRLL_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JES_UP_TOTAL_%s"   , chainTitleCh) , 8   , 1 , 9); p_jes_alt_up.SRLL.TOTAL->Sumw2();
    p_jes_alt_dn.SRHH.TOTAL     = new TH1F(Form("SRHH_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JES_DN_TOTAL_%s"   , chainTitleCh) , 32  , 1 , 33);p_jes_alt_dn.SRHH.TOTAL->Sumw2();
    p_jes_alt_dn.SRHL.TOTAL     = new TH1F(Form("SRHL_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JES_DN_TOTAL_%s"   , chainTitleCh) , 26  , 1 , 27);p_jes_alt_dn.SRHL.TOTAL->Sumw2();
    p_jes_alt_dn.SRLL.TOTAL     = new TH1F(Form("SRLL_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JES_DN_TOTAL_%s"   , chainTitleCh) , 8   , 1 , 9); p_jes_alt_dn.SRLL.TOTAL->Sumw2();
  } else {
    p_jes_alt_up.SRHH.TOTAL     = 0;
    p_jes_alt_up.SRHL.TOTAL     = 0;
    p_jes_alt_up.SRLL.TOTAL     = 0;
    p_jes_alt_dn.SRHH.TOTAL     = 0;
    p_jes_alt_dn.SRHL.TOTAL     = 0;
    p_jes_alt_dn.SRLL.TOTAL     = 0;
  }
 
  //For btag SF variations
  plots_t p_btagSF_alt_up;
  plots_t p_btagSF_alt_dn;
  if (isData==0){
    p_btagSF_alt_up.SRHH.TOTAL = new TH1F(Form("SRHH_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSF_UP_TOTAL_%s", chainTitleCh), 32, 1, 33);
    p_btagSF_alt_up.SRHL.TOTAL = new TH1F(Form("SRHL_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSF_UP_TOTAL_%s", chainTitleCh), 26, 1, 27);
    p_btagSF_alt_up.SRLL.TOTAL = new TH1F(Form("SRLL_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSF_UP_TOTAL_%s", chainTitleCh),  8, 1,  9);
    p_btagSF_alt_dn.SRHH.TOTAL = new TH1F(Form("SRHH_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSF_DN_TOTAL_%s", chainTitleCh), 32, 1, 33);
    p_btagSF_alt_dn.SRHL.TOTAL = new TH1F(Form("SRHL_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSF_DN_TOTAL_%s", chainTitleCh), 26, 1, 27);
    p_btagSF_alt_dn.SRLL.TOTAL = new TH1F(Form("SRLL_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSF_DN_TOTAL_%s", chainTitleCh),  8, 1,  9);
    p_btagSF_alt_up.SRHH.TOTAL->Sumw2();
    p_btagSF_alt_up.SRHL.TOTAL->Sumw2();
    p_btagSF_alt_up.SRLL.TOTAL->Sumw2();
    p_btagSF_alt_dn.SRHH.TOTAL->Sumw2();
    p_btagSF_alt_dn.SRHL.TOTAL->Sumw2();
    p_btagSF_alt_dn.SRLL.TOTAL->Sumw2();
  } 
  else {
    p_btagSF_alt_up.SRHH.TOTAL = 0;
    p_btagSF_alt_up.SRHL.TOTAL = 0;
    p_btagSF_alt_up.SRLL.TOTAL = 0;
    p_btagSF_alt_dn.SRHH.TOTAL = 0;
    p_btagSF_alt_dn.SRHL.TOTAL = 0;
    p_btagSF_alt_dn.SRLL.TOTAL = 0;
  }

  //For PU variations
  plots_t p_pu_alt_up;
  plots_t p_pu_alt_dn;
  if (doFakes == 1 || isData==0) {
    p_pu_alt_up.SRHH.TOTAL     = new TH1F(Form("SRHH_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PU_UP_TOTAL_%s"   , chainTitleCh) , 32  , 1 , 33);p_pu_alt_up.SRHH.TOTAL->Sumw2();
    p_pu_alt_up.SRHL.TOTAL     = new TH1F(Form("SRHL_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PU_UP_TOTAL_%s"   , chainTitleCh) , 26  , 1 , 27);p_pu_alt_up.SRHL.TOTAL->Sumw2();
    p_pu_alt_up.SRLL.TOTAL     = new TH1F(Form("SRLL_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PU_UP_TOTAL_%s"   , chainTitleCh) , 8   , 1 , 9); p_pu_alt_up.SRLL.TOTAL->Sumw2();
    p_pu_alt_dn.SRHH.TOTAL     = new TH1F(Form("SRHH_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PU_DN_TOTAL_%s"   , chainTitleCh) , 32  , 1 , 33);p_pu_alt_dn.SRHH.TOTAL->Sumw2();
    p_pu_alt_dn.SRHL.TOTAL     = new TH1F(Form("SRHL_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PU_DN_TOTAL_%s"   , chainTitleCh) , 26  , 1 , 27);p_pu_alt_dn.SRHL.TOTAL->Sumw2();
    p_pu_alt_dn.SRLL.TOTAL     = new TH1F(Form("SRLL_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PU_DN_TOTAL_%s"   , chainTitleCh) , 8   , 1 , 9); p_pu_alt_dn.SRLL.TOTAL->Sumw2();
  } else {
    p_pu_alt_up.SRHH.TOTAL     = 0;
    p_pu_alt_up.SRHL.TOTAL     = 0;
    p_pu_alt_up.SRLL.TOTAL     = 0;
    p_pu_alt_dn.SRHH.TOTAL     = 0;
    p_pu_alt_dn.SRHL.TOTAL     = 0;
    p_pu_alt_dn.SRLL.TOTAL     = 0;
  }

  //For ISR variations
  plots_t p_isr_alt_up;
  if (isFastSimSignal) {
    p_isr_alt_up.SRHH.TOTAL     = new TH1F(Form("SRHH_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ISR_UP_TOTAL_%s"   , chainTitleCh) , 32  , 1 , 33);p_isr_alt_up.SRHH.TOTAL->Sumw2();
    p_isr_alt_up.SRHL.TOTAL     = new TH1F(Form("SRHL_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ISR_UP_TOTAL_%s"   , chainTitleCh) , 26  , 1 , 27);p_isr_alt_up.SRHL.TOTAL->Sumw2();
    p_isr_alt_up.SRLL.TOTAL     = new TH1F(Form("SRLL_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ISR_UP_TOTAL_%s"   , chainTitleCh) , 8   , 1 , 9); p_isr_alt_up.SRLL.TOTAL->Sumw2();
  } else {
    p_isr_alt_up.SRHH.TOTAL     = 0;
    p_isr_alt_up.SRHL.TOTAL     = 0;
    p_isr_alt_up.SRLL.TOTAL     = 0;
  }

  //For XSEC variations
  plots_t p_xsec_alt_up;
  plots_t p_xsec_alt_dn;
  if (isFastSimSignal) {
    p_xsec_alt_up.SRHH.TOTAL = new TH1F(Form("SRHH_XSEC_UP_TOTAL_%s", chainTitleCh), Form("SRHH_XSEC_UP_TOTAL_%s", chainTitleCh), 32, 1, 33); p_xsec_alt_up.SRHH.TOTAL->Sumw2();
    p_xsec_alt_up.SRHL.TOTAL = new TH1F(Form("SRHL_XSEC_UP_TOTAL_%s", chainTitleCh), Form("SRHL_XSEC_UP_TOTAL_%s", chainTitleCh), 26, 1, 27); p_xsec_alt_up.SRHL.TOTAL->Sumw2();
    p_xsec_alt_up.SRLL.TOTAL = new TH1F(Form("SRLL_XSEC_UP_TOTAL_%s", chainTitleCh), Form("SRLL_XSEC_UP_TOTAL_%s", chainTitleCh), 8 , 1, 9);  p_xsec_alt_up.SRLL.TOTAL->Sumw2();
    p_xsec_alt_dn.SRHH.TOTAL = new TH1F(Form("SRHH_XSEC_DN_TOTAL_%s", chainTitleCh), Form("SRHH_XSEC_DN_TOTAL_%s", chainTitleCh), 32, 1, 33); p_xsec_alt_dn.SRHH.TOTAL->Sumw2();
    p_xsec_alt_dn.SRHL.TOTAL = new TH1F(Form("SRHL_XSEC_DN_TOTAL_%s", chainTitleCh), Form("SRHL_XSEC_DN_TOTAL_%s", chainTitleCh), 26, 1, 27); p_xsec_alt_dn.SRHL.TOTAL->Sumw2();
    p_xsec_alt_dn.SRLL.TOTAL = new TH1F(Form("SRLL_XSEC_DN_TOTAL_%s", chainTitleCh), Form("SRLL_XSEC_DN_TOTAL_%s", chainTitleCh), 8 , 1, 9);  p_xsec_alt_dn.SRLL.TOTAL->Sumw2();
  } 
  else {
    p_xsec_alt_up.SRHH.TOTAL = 0;
    p_xsec_alt_up.SRHL.TOTAL = 0;
    p_xsec_alt_up.SRLL.TOTAL = 0;
    p_xsec_alt_dn.SRHH.TOTAL = 0;
    p_xsec_alt_dn.SRHL.TOTAL = 0;
    p_xsec_alt_dn.SRLL.TOTAL = 0;
  }

  //nEvents in chain
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();

  //Set up iterator
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //Number of selected events
  //int nSelected = 0; 

  //File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){

    // Get File Content
    TFile *file = new TFile(currentFile->GetTitle());
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    //Now that the file is open, we can read the histograms and get the XSEC values
    if (isFastSimSignal){ 

      //First, find right histogram inside the baby
      TH1F *cs_hist = 0; 
      for (auto&& keyAsObj : *file->GetListOfKeys()){
       auto key = (TKey*) keyAsObj;
       TH1F *hist = (TH1F*)key->ReadObj(); 
       string title = hist->GetTitle();
       if (title == "SS2015 Baby Ntuple") continue;
       int find = title.find('_');
       int find2 = title.find('_', find+1);
       int find3 = title.length();
       float mGluino = atof(title.substr(find+1, find2-find-1).c_str());
       float mLSP = atof(title.substr(find2+1, find3-find2).c_str()); 
       //cout << "mGluino: " << mGluino << endl;
       //cout << "mLSP: " << mLSP << endl;
       //cout << "mysparms[0]: " << mysparms[0] << endl;
       //cout << "mysparms[1]: " << mysparms[1] << endl;
       if (mysparms[0] != mGluino || mysparms[1] != mLSP){ /*cout << "not ok" << endl;*/  continue;  }
       else { cs_hist = (TH1F*)hist->Clone("cs_hist"); break;   } 
      }

      //Next figure out scale, PDF, and alpha_s variations and fill the histograms
      for (int i = 1; i < 67; i++){
        float nominal  = cs_hist->GetBinContent(200+i); 
        //cout << __LINE__ << " " << i << " "  << nominal << endl;
        float scale_up = cs_hist->GetBinContent(300+i); 
        //cout << __LINE__ << " " << i << " "  << scale_up << endl;
        float scale_dn = cs_hist->GetBinContent(400+i); 
        //cout << __LINE__ << " " << i << " "  << scale_dn << endl;
        float alpha_up = cs_hist->GetBinContent(7600+i); 
        //cout << __LINE__ << " " << i << " "  << scale_dn << endl;
        float alpha_dn = cs_hist->GetBinContent(7700+i); 
        TH1F *h_pdf = new TH1F("pdf", "pdf", 10000, 0, 1000); 
        for (int j = 0; j < 100; j++){
          h_pdf->Fill(cs_hist->GetBinContent(1000+100*(i-1)+j)); 
        }
        float pdf = h_pdf->GetMean() > 0 ? h_pdf->GetRMS()/nominal : 0; 
        //cout << __LINE__ << " " << i << " "  << pdf << endl;
        delete h_pdf; 
        float total_up = sqrt( scale_up*scale_up + pdf*pdf + alpha_up*alpha_up ); 
        //cout << __LINE__ << " " << i << " "  << total_up << endl;
        float total_dn = sqrt( scale_dn*scale_dn + pdf*pdf + alpha_dn*alpha_dn ); 
        if      (i <= 32) p_xsec_alt_up.SRHH.TOTAL->Fill(i,    total_up); 
        else if (i <= 58) p_xsec_alt_up.SRHL.TOTAL->Fill(i-32, total_up); 
        else              p_xsec_alt_up.SRLL.TOTAL->Fill(i-58, total_up); 
        if      (i <= 32) p_xsec_alt_dn.SRHH.TOTAL->Fill(i,    total_dn); 
        else if (i <= 58) p_xsec_alt_dn.SRHL.TOTAL->Fill(i-32, total_dn); 
        else              p_xsec_alt_dn.SRLL.TOTAL->Fill(i-58, total_dn); 
      }
      //cout << p_xsec_alt_up.SRHH.TOTAL->Integral() << endl;
    }

    // Loop over Events in current file
    for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){
    
      //Get Event Content
      samesign.GetEntry(event);
      nEventsTotal++;

      if (ss::is_real_data()){
        if (doNoData && !doFlips && !doFakes && !testFakeSR) continue; 
        else if (!isUnblindRun(ss::run())) continue;
      }

      if (isFastSimSignal) {
	bool isTheRightSample = true;
	for (unsigned int isp=0;isp<ss::sparms().size();isp++) {
	  if (fabs(ss::sparms()[isp]-mysparms[isp])>0.1) isTheRightSample = false;
	}
	if (!isTheRightSample) continue;
      }

      //Reject not triggered
      if (!ss::fired_trigger() && !isFastSimSignal) continue;
      if (!ss::passedFilterList()) continue;
      if (!ss::passes_met_filters()) continue;

      float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG*getTruePUw(ss::trueNumInt()[0])*ss::weight_btagsf();
      weight*=scaleLumi;
      if (isWZ) weight*=getWZSF();

      //apply lepton scale factors
      if (ss::is_real_data()==0 && isWZ==0) {
	weight*=eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
	if (isFastSimSignal) {
	  weight *= eventScaleFactorFastSim(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht(), ss::nGoodVertices());
	}
      }

      float weight_alt_FR = weight;
      float weight_alt_ISR = weight;
      float weight_btag_up_alt = weight;
      float weight_btag_dn_alt = weight;
      float weight_pu_up_alt = weight;
      float weight_pu_dn_alt = weight;
      if (ss::is_real_data()==0) {
	weight_btag_up_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_UP()/ss::weight_btagsf() : weight;
	weight_btag_dn_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_DN()/ss::weight_btagsf() : weight;
	weight_pu_up_alt = getTruePUw(ss::trueNumInt()[0])>0 ? weight*getTruePUwUp(ss::trueNumInt()[0])/getTruePUw(ss::trueNumInt()[0]) : weight;
	weight_pu_dn_alt = getTruePUw(ss::trueNumInt()[0])>0 ? weight*getTruePUwDn(ss::trueNumInt()[0])/getTruePUw(ss::trueNumInt()[0]) : weight;
      }

      if (isFastSimSignal) {
	if (ss::glglpt()>600.) weight_alt_ISR*=1.30;
	else if (ss::glglpt()>400.) weight_alt_ISR*=1.15;
      }

      // if (doFakes!=1) continue;
      // if (ss::event()!=99054862) continue;
      // cout << "found it:" << ss::event() << " hyp_class=" << ss::hyp_class() << endl;
      // cout << "nbtags=" << ss::nbtags() << " " << ss::nbtags() << endl;
      // cout << "met=" << ss::met() << endl;
      // cout << ss::ht() << " " << passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA()) << " " << passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA()) << endl;
      // for (int j=0;j<ss::mostJets().size();++j) {
      // 	cout << "most jet pt=" << ss::mostJets()[j].pt()*ss::mostJets_undoJEC()[j]*ss::mostJets_JEC()[j] << " eta=" << ss::mostJets()[j].eta() 
      // 	     << " ptUp=" << ss::mostJets()[j].pt()*ss::mostJets_undoJEC()[j]*ss::mostJets_JEC()[j]*(1.0+ss::mostJets_unc()[j])
      // 	     << " ptDn=" << ss::mostJets()[j].pt()*ss::mostJets_undoJEC()[j]*ss::mostJets_JEC()[j]*(1.0-ss::mostJets_unc()[j])
      // 	     << " disc=" << ss::mostJets_disc()[j]
      // 	     << endl;
      // }
      // for (int j=0;j<ss::jets().size();++j) {
      // 	cout << "jet pt=" << ss::jets()[j].pt() << " eta=" << ss::jets()[j].eta() 
      // 	     << " disc=" << ss::jets_disc()[j]
      // 	     << endl;
      // }
      // for (int j=0;j<ss::btags().size();++j) {
      // 	cout << "btag pt=" << ss::btags()[j].pt() << " eta=" << ss::btags()[j].eta() 
      // 	     << " disc=" << ss::btags_disc()[j]
      // 	     << endl;
      // }
      // unsigned int triggerBits = ss::triggers();
      // if ( (triggerBits & (1<<0))==(1<<0) or (triggerBits & (1<<5))==(1<<5) or (triggerBits & (1<<7))==(1<<7) ) cout << "pass ht trigs" << endl;
      // if ( (triggerBits & (1<<1))==(1<<1) or (triggerBits & (1<<2))==(1<<2) or (triggerBits & (1<<3))==(1<<3) or (triggerBits & (1<<4))==(1<<4) or (triggerBits & (1<<6))==(1<<6) ) cout <<"pass iso trigs" << endl;
      // exit(0);

      // if ( (triggerBits & (1<<0))==(1<<0) ) cout << "pass bit=" << 0 << endl;
      // if ( (triggerBits & (1<<1))==(1<<1) ) cout << "pass bit=" << 1 << endl;
      // if ( (triggerBits & (1<<2))==(1<<2) ) cout << "pass bit=" << 2 << endl;
      // if ( (triggerBits & (1<<3))==(1<<3) ) cout << "pass bit=" << 3 << endl;
      // if ( (triggerBits & (1<<4))==(1<<4) ) cout << "pass bit=" << 4 << endl;
      // if ( (triggerBits & (1<<5))==(1<<5) ) cout << "pass bit=" << 5 << endl;
      // if ( (triggerBits & (1<<6))==(1<<6) ) cout << "pass bit=" << 6 << endl;
      // if ( (triggerBits & (1<<7))==(1<<7) ) cout << "pass bit=" << 7 << endl;

      //Progress
      //SSAG::progress(nEventsTotal, nEventsChain);

      // unsigned int triggerBits = ss::triggers();
      // if ( (triggerBits & (1<<0))!=(1<<0) and (triggerBits & (1<<5))!=(1<<5) and (triggerBits & (1<<7))!=(1<<7) ) continue;
      // if ( (triggerBits & (1<<1))!=(1<<1) and (triggerBits & (1<<2))!=(1<<2) and (triggerBits & (1<<3))!=(1<<3) and (triggerBits & (1<<4))!=(1<<4) and (triggerBits & (1<<6))!=(1<<6) ) continue;

      //electron FO is tighter for iso triggers, make sure it is passed
      if (ss::ht()<300.) {
        if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA())) continue;
        if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA())) continue;
      } 

      // cout << abs(ss::lep1_id()) << " " << ss::lep1_p4().pt() << " " << ss::lep1_coneCorrPt() << endl;
      // cout << abs(ss::lep2_id()) << " " << ss::lep2_p4().pt() << " " << ss::lep2_coneCorrPt() << endl;

      float lep1_pt = ss::lep1_coneCorrPt();
      float lep2_pt = ss::lep2_coneCorrPt();
      //now recompute mtmin
      float mtl1 = MT(lep1_pt, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
      float mtl2 = MT(lep2_pt, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
      float mtmin = mtl1 > mtl2 ? mtl2 : mtl1;

      //drop electrons below 15 GeV
      if (abs(ss::lep1_id())==11 && lep1_pt<15) continue;
      if (abs(ss::lep2_id())==11 && lep2_pt<15) continue;	

      //Only keep good events
      if (!doFlips && !doFakes && exclude == 0) {
	    if (ss::hyp_class() != 3) continue;
	    //first check the charge (since isPrompt does not know about it)
	    if (!isData && !isGamma && ss::lep1_motherID()==2) continue;
	    if (!isData && !isGamma && ss::lep2_motherID()==2) continue;
	    //then do the usual checks
	    if (!isData && !isGamma && ss::lep1_motherID()!=1 && ss::lep1_isPrompt()!=1 && ss::lep1_isDirectPrompt()!=1) continue;
	    if (!isData && !isGamma && ss::lep2_motherID()!=1 && ss::lep2_isPrompt()!=1 && ss::lep2_isDirectPrompt()!=1) continue;
	    //photons can give fakes from conversions (not accounted by data-driven method) 
	    //but flips need to be excluded to avoid double counting
	    if (isGamma && (ss::lep1_motherID()==2 || ss::lep2_motherID()==2)) continue;
      }

      if (!doFlips && !doFakes && exclude == 1){
	    if (ss::hyp_class() != 3) continue;
	    if (!isData && !((ss::lep1_motherID() == 1 && ss::lep2_motherID() < 0) || (ss::lep1_motherID() < 0 && ss::lep2_motherID() == 1))) continue;
      }

      if (!doFlips && !doFakes && exclude == 2){
	    if (ss::hyp_class() != 3) continue;
	    if (!isData && !((ss::lep1_motherID() == 1 && ss::lep2_motherID() == 2) || (ss::lep1_motherID() == 2 && ss::lep2_motherID() == 1))) continue;
      }

      if (!doFlips && !doFakes && exclude == 3){
	    if (ss::hyp_class() != 3) continue;
	    if (!isData && !((ss::lep1_motherID() == 1 || ss::lep2_motherID() == 1))) continue;
      }

      if (doFlips) {
        if (ss::hyp_class() != 4) continue;
          float flipFact = 0.;
          if (abs(ss::lep1_id())==11){
            float flr = flipRate(ss::lep1_p4().pt(), ss::lep1_p4().eta());
            flipFact += (flr/(1-flr));
          }
          if (abs(ss::lep2_id())==11){
            float flr = flipRate(ss::lep2_p4().pt(), ss::lep2_p4().eta());
            flipFact += (flr/(1-flr));
          }
          weight *= flipFact;
      } 
      
      //QCD Fakes
      if (doFakes == 1){
        if (ss::hyp_class() != 2 && ss::hyp_class() != 1) continue;
        if (ss::lep1_passes_id()==0) {
	  float fr = fakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht());
	  weight *= fr/(1.-fr);
	  float fra = alternativeFakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht());
	  weight_alt_FR *= fra/(1.-fra);
	}
        if (ss::lep2_passes_id()==0) {
	  float fr = fakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
	  weight *= fr/(1.-fr);
	  float fra = alternativeFakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
	  weight_alt_FR *= fra/(1.-fra);
	}
	//subtract double FO
	if (ss::hyp_class() == 1) weight *= -1.;
        if (!ss::is_real_data() && isData) {
	  weight *= -1.;
	  weight_alt_FR *= -1.;
	}
      }

      //Get pt ratio
      float ptratio_1 = ss::lep1_closeJet().pt() > 0 ? ss::lep1_p4().pt()/ss::lep1_closeJet().pt() : 1.0;
      float ptratio_2 = ss::lep2_closeJet().pt() > 0 ? ss::lep2_p4().pt()/ss::lep2_closeJet().pt() : 1.0; 

      //In Situ Fakes
      if (doFakes == 2){
        if (ss::hyp_class() != 5 && ss::hyp_class() != 2) continue;
	if (ss::lep1_id()*ss::lep2_id()<0) continue;
        //Isolation stuff
        float ptratio_cut_1  = (abs(ss::lep1_id()) == 11 ? 0.80 : 0.76);
        float ptratio_cut_2  = (abs(ss::lep2_id()) == 11 ? 0.80 : 0.76);
        float ptrel_cut_1  = (abs(ss::lep1_id()) == 11 ? 7.20 : 7.20);
        float ptrel_cut_2  = (abs(ss::lep2_id()) == 11 ? 7.20 : 7.20);
        bool lep1_denom_iso  = ((ss::lep1_miniIso() < 0.4) && ((ss::lep1_ptrel_v1() > ptrel_cut_1) || (ptratio_1 < (1.0/ptratio_cut_1 + ss::lep1_miniIso()))));
        bool lep2_denom_iso  = ((ss::lep2_miniIso() < 0.4) && ((ss::lep2_ptrel_v1() > ptrel_cut_2) || (ptratio_2 < (1.0/ptratio_cut_2 + ss::lep2_miniIso()))));
        float fr = 0.;
        if (ss::lep1_passes_id()==0 && passesNumeratorMVA(ss::lep1_id(),ss::lep1_p4().eta(),ss::lep1_MVA()) && ss::lep2_passes_id() && ss::lep1_dxyPV()<0.05 && ss::lep1_sip()<4 && lep1_denom_iso) fr = fakeRateInSitu(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta());
        if (ss::lep2_passes_id()==0 && passesNumeratorMVA(ss::lep2_id(),ss::lep2_p4().eta(),ss::lep2_MVA()) && ss::lep1_passes_id() && ss::lep2_dxyPV()<0.05 && ss::lep2_sip()<4 && lep2_denom_iso) fr = fakeRateInSitu(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta());
        weight *= fr/(1.-fr);
        if (!ss::is_real_data()) weight *= -1.;
      }

      //Reject duplicates (after selection otherwise flips are ignored...)
      if (isData && ss::is_real_data()){
        duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
        if (duplicate_removal::is_duplicate(id)){ continue; }
      }

      //Require baseline selections
      int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);
      // cout << "BR=" << BR << endl;
      // cout << Form("nj=%i nb=%i met=%f ht=%f",ss::njets(), ss::nbtags(), ss::met(), ss::ht()) << endl;
      if (BR < 0) continue;

      //Get the SR
      int SR = signalRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);

      //SR variation due to JES
      float mtl1_unc_up = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
      float mtl2_unc_up = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
      float mtmin_unc_up = mtl1_unc_up > mtl2_unc_up ? mtl2_unc_up : mtl1_unc_up;
      float mtl1_unc_dn = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
      float mtl2_unc_dn = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
      float mtmin_unc_dn = mtl1_unc_dn > mtl2_unc_dn ? mtl2_unc_dn : mtl1_unc_dn;
      int SR_unc_up = signalRegion(ss::njets_unc_up(), ss::nbtags_unc_up(), ss::met_unc_up(), ss::ht_unc_up(), mtmin_unc_up, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);
      int SR_unc_dn = signalRegion(ss::njets_unc_dn(), ss::nbtags_unc_dn(), ss::met_unc_dn(), ss::ht_unc_dn(), mtmin_unc_dn, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt);

      // cout << "SR=" << SR << endl;

      int nleps = 2;
      if (ss::lep3_passes_id()) nleps++;
      if (ss::lep4_passes_id()) nleps++;

      //Get the category
      anal_type_t categ = analysisCategory(ss::lep1_id(), ss::lep2_id(), ss::lep1_coneCorrPt(), ss::lep2_coneCorrPt());
	  string cat = "HH";
	  if (categ==1) cat = "HL";
	  if (categ==2) cat = "LL";

      //bool avi = 0; 
      //if (categ == 0 && SR == 1)  avi = 1;
      //if (categ == 0 && SR == 2)  avi = 1;
      //if (categ == 0 && SR == 9)  avi = 1;
      //if (categ == 0 && SR == 10) avi = 1;
      //if (categ == 0 && SR == 32) avi = 1;
      //if (categ == 1 && SR ==  1) avi = 1;
      //if (categ == 1 && SR ==  2) avi = 1;
      //if (categ == 1 && SR ==  4) avi = 1;
      //if (categ == 1 && SR ==  7) avi = 1;
      //if (categ == 1 && SR ==  8) avi = 1;
      //if (avi == 0) continue; 

      if (doNoData && testFakeSR) {
        if (categ==0) {
          if (SR!=9) continue;
        } 
        else if (categ==1) {
          if (SR!=1 && SR!=7) continue;
        } 
        else continue;
      }

      if (doFakes == 1 && ss::is_real_data() && ss::hyp_class()==2 && SR>0) {
	float ptT = (ss::lep1_passes_id()==0 ? ss::lep2_coneCorrPt() : ss::lep1_coneCorrPt());
	float ptCL = (ss::lep1_passes_id()==0 ? ss::lep1_coneCorrPt() : ss::lep2_coneCorrPt());
	float ptR = (ss::lep1_passes_id()==0 ? ss::lep1_ptrel_v1() : ss::lep2_ptrel_v1());
	float ptJ = (ss::lep1_passes_id()==0 ? ss::jet_close_lep1().pt() : ss::jet_close_lep2().pt());
	float iso = (ss::lep1_passes_id()==0 ? ss::lep1_miniIso() : ss::lep2_miniIso());
	//cout << Form("%20i       %6.2f %6.2f %6.2f %6.2f %6.2f %s SR%i",ss::event(),ptT,ptCL,ptR,ptJ,iso,cat.c_str(),SR) << endl;
      }

      if (isData && !doFakes && !doFlips) {
	float ptT  = (ss::lep1_passes_id()==0 ? lep2_pt : lep1_pt);
	float ptCL = (ss::lep1_passes_id()==0 ? lep1_pt : lep2_pt);
	float ptR  = (ss::lep1_passes_id()==0 ? ss::lep1_ptrel_v1() : ss::lep2_ptrel_v1());
	float ptJ  = (ss::lep1_passes_id()==0 ? ss::jet_close_lep1().pt() : ss::jet_close_lep2().pt());
	float iso  = (ss::lep1_passes_id()==0 ? ss::lep1_miniIso() : ss::lep2_miniIso());
	string cat = "HH";
	if (categ==1) cat = "HL";
	if (categ==2) cat = "LL";
	//cout << Form("%20i       %6.2f %6.2f %6.2f %6.2f %6.2f %s SR%i",ss::event(),ptT,ptCL,ptR,ptJ,iso,cat.c_str(),SR) << endl;
      }

      int mytype = ss::hyp_type();
      if (mytype==2 && abs(ss::lep1_id())==13) mytype = 1;

      //Calculate the baseline yield
      if      (ss::hyp_type() == 3) y_result.EE    += weight;
      else if (ss::hyp_type() == 0) y_result.MM    += weight;
      else                          y_result.EM    += weight;
                                    y_result.TOTAL += weight;

      //Fill kinem plots
      p_result.h_dxy         ->Fill(ss::lep1_dxyPV()                                                                        , weight);
      p_result.h_dxy         ->Fill(ss::lep2_dxyPV()                                                                        , weight);
      p_result.h_dz          ->Fill(ss::lep1_dZ()                                                                           , weight);
      p_result.h_dz          ->Fill(ss::lep2_dZ()                                                                           , weight);
      p_result.h_sip3d       ->Fill(ss::lep1_sip()                                                                          , weight);
      p_result.h_sip3d       ->Fill(ss::lep1_sip()                                                                          , weight);
      if (abs(ss::lep1_id()) == 11) p_result.h_mva->Fill(ss::lep1_MVA()                                                     , weight);
      if (abs(ss::lep2_id()) == 11) p_result.h_mva->Fill(ss::lep2_MVA()                                                     , weight);
      p_result.h_ht          ->Fill(ss::ht()                                                                                , weight);
      p_result.h_met         ->Fill(ss::met()                                                                               , weight);
      p_result.h_mll         ->Fill((ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()).M() , weight);
      p_result.h_mtmin       ->Fill(mtmin                                                                                   , weight);
      p_result.h_njets       ->Fill(ss::njets()                                                                             , weight);
      p_result.h_nbtags      ->Fill(ss::nbtags()                                                                            , weight);
      p_result.h_l1pt        ->Fill(lep1_pt                                                                                 , weight);
      p_result.h_l2pt        ->Fill(lep2_pt                                                                                 , weight);
      p_result.h_l1eta       ->Fill(ss::lep1_p4().eta()                                                                     , weight);
      p_result.h_l2eta       ->Fill(ss::lep2_p4().eta()                                                                     , weight);
      p_result.h_type        ->Fill(mytype                                                                                  , weight);
      p_result.h_lep1_miniIso->Fill(ss::lep1_miniIso()                                                                      , weight);
      p_result.h_lep2_miniIso->Fill(ss::lep2_miniIso()                                                                      , weight);
      p_result.h_lep1_ptRel  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
      p_result.h_lep2_ptRel  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
      p_result.h_lep1_ptRatio->Fill(ptratio_1                                                                               , weight);
      p_result.h_lep2_ptRatio->Fill(ptratio_2                                                                               , weight);
      p_result.h_nleps       ->Fill(nleps                                                                                   , weight);
      // if (!isData && !doFakes && !doFlips && SR == 1 && cat == "HH"){
      // cout << Form("%1d %9d %llu\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%s%2d\n", ss::run(), ss::lumi(), ss::event(), ss::nVetoElectrons7()+ss::nVetoMuons5(), ss::lep1_id(), ss::lep1_p4().pt(), ss::lep2_id(), ss::lep2_p4().pt(), ss::njets(), ss::nbtags(), ss::met(), ss::ht(), cat.c_str(), SR);
      // cout << Form("%1d %9d %llu\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%s%2d\n", ss::run(), ss::lumi(), ss::event(), ss::lep1_id(), ss::lep1_p4().pt(), ss::lep2_id(), ss::lep2_p4().pt(), ss::njets(), ss::nbtags(), ss::met(), ss::ht(), cat.c_str(), SR);
      // 	cout << ss::filename() << " " << ss::filenumber() << endl;
      // cout << Form("%1d\t%9d\t%llu\t%+2d\t%5.1f\t%+2d\t%5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%s%2d\t%5.1f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%6.4f\t%s\t%i\n", ss::run(), ss::lumi(), ss::event(), ss::lep1_id(), ss::lep1_p4().pt(), ss::lep2_id(), ss::lep2_p4().pt(), ss::njets(), ss::nbtags(), ss::met(), ss::ht(), cat.c_str(), SR,ss::trueNumInt()[0],getTruePUw(ss::trueNumInt()[0]),ss::weight_btagsf(),triggerScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::ht()),leptonScaleFactor(ss::lep1_id(), ss::lep1_p4().pt(), ss::lep1_p4().eta(), ss::ht()),leptonScaleFactor(ss::lep2_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::ht()),ss::scale1fb()*lumiAG,ss::filename().Data(),ss::filenumber());
      // 	// cout << ss::lep1_p4().eta() << " " << ss::lep2_p4().eta() << " " << ss::lep1_p4().phi() << " " << ss::lep2_p4().phi() << " " << ss::lep3_p4().pt() << " " << ss::lep3_p4().eta() << " " << ss::lep3_p4().phi() << " " << ss::lep3_id() << " " << ss::lep4_p4().pt() << " " << ss::lep4_p4().eta() << " " << ss::lep4_p4().phi() << " " << ss::lep4_id() << " " << ss::lep3_passes_id() << " " << ss::lep4_passes_id() << endl;
      // 	// for (unsigned int i = 0; i < ss::btags().size(); i++) cout << ss::btags().at(i).eta() << ",";
      // 	// cout << endl;
      // 	// nSelected++; 
      // }

      //Fill SR plots
      if (categ == HighHigh){
        if      (ss::hyp_type() == 3) p_result.SRHH.EE->Fill(SR, weight); 
        else if (ss::hyp_type() == 0) p_result.SRHH.MM->Fill(SR, weight); 
        else                          p_result.SRHH.EM->Fill(SR, weight); 
                                      p_result.SRHH.TOTAL->Fill(SR, weight); 
        if (doFakes == 1 )            p_fake_alt_up.SRHH.TOTAL->Fill(SR, weight_alt_FR); 
        if (isFastSimSignal)          p_isr_alt_up.SRHH.TOTAL->Fill(SR, weight_alt_ISR); 
        if (isData  == 0 )            p_jes_alt_up.SRHH.TOTAL->Fill(SR_unc_up, weight); 
        if (isData  == 0 )            p_jes_alt_dn.SRHH.TOTAL->Fill(SR_unc_dn, weight); 
        if (isData  == 0 )            p_btagSF_alt_up.SRHH.TOTAL->Fill(SR, weight_btag_up_alt); 
        if (isData  == 0 )            p_btagSF_alt_dn.SRHH.TOTAL->Fill(SR, weight_btag_dn_alt); 
        if (isData  == 0 )            p_pu_alt_up.SRHH.TOTAL->Fill(SR, weight_pu_up_alt); 
        if (isData  == 0 )            p_pu_alt_dn.SRHH.TOTAL->Fill(SR, weight_pu_dn_alt); 
      }

      if (categ == HighLow){
        if      (ss::hyp_type() == 3) p_result.SRHL.EE->Fill(SR, weight); 
        else if (ss::hyp_type() == 0) p_result.SRHL.MM->Fill(SR, weight); 
        else                          p_result.SRHL.EM->Fill(SR, weight); 
                                      p_result.SRHL.TOTAL->Fill(SR, weight); 
        if (doFakes == 1 )            p_fake_alt_up.SRHL.TOTAL->Fill(SR, weight_alt_FR); 
        if (isFastSimSignal)          p_isr_alt_up.SRHL.TOTAL->Fill(SR, weight_alt_ISR); 
        if (isData  == 0 )            p_jes_alt_up.SRHL.TOTAL->Fill(SR_unc_up, weight); 
        if (isData  == 0 )            p_jes_alt_dn.SRHL.TOTAL->Fill(SR_unc_dn, weight); 
        if (isData  == 0 )            p_btagSF_alt_up.SRHL.TOTAL->Fill(SR, weight_btag_up_alt); 
        if (isData  == 0 )            p_btagSF_alt_dn.SRHL.TOTAL->Fill(SR, weight_btag_dn_alt); 
        if (isData  == 0 )            p_pu_alt_up.SRHL.TOTAL->Fill(SR, weight_pu_up_alt); 
        if (isData  == 0 )            p_pu_alt_dn.SRHL.TOTAL->Fill(SR, weight_pu_dn_alt); 
      }

      if (categ == LowLow){
        if      (ss::hyp_type() == 3) p_result.SRLL.EE->Fill(SR, weight); 
        else if (ss::hyp_type() == 0) p_result.SRLL.MM->Fill(SR, weight); 
        else                          p_result.SRLL.EM->Fill(SR, weight); 
                                      p_result.SRLL.TOTAL->Fill(SR, weight); 
        if (doFakes == 1 )            p_fake_alt_up.SRLL.TOTAL->Fill(SR, weight_alt_FR); 
        if (isFastSimSignal)          p_isr_alt_up.SRLL.TOTAL->Fill(SR, weight_alt_ISR); 
        if (isData  == 0 )            p_jes_alt_up.SRLL.TOTAL->Fill(SR_unc_up, weight); 
        if (isData  == 0 )            p_jes_alt_dn.SRLL.TOTAL->Fill(SR_unc_dn, weight); 
        if (isData  == 0 )            p_btagSF_alt_up.SRLL.TOTAL->Fill(SR, weight_btag_up_alt); 
        if (isData  == 0 )            p_btagSF_alt_dn.SRLL.TOTAL->Fill(SR, weight_btag_dn_alt); 
        if (isData  == 0 )            p_pu_alt_up.SRLL.TOTAL->Fill(SR, weight_pu_up_alt); 
        if (isData  == 0 )            p_pu_alt_dn.SRLL.TOTAL->Fill(SR, weight_pu_dn_alt); 
      }


    }//event loop
    file->Close(); 
  }//file loop

  //Print out number selected
  //cout << "Number selected: " << nSelected << endl;

  if (y_result.EE<0) y_result.EE=0.;
  if (y_result.MM<0) y_result.MM=0.;
  if (y_result.EM<0) y_result.EM=0.;
  if (y_result.TOTAL<0) y_result.TOTAL=0.;

  avoidNegativeYields(p_result.SRHH.TOTAL);
  avoidNegativeYields(p_result.SRHL.TOTAL);
  avoidNegativeYields(p_result.SRLL.TOTAL);
  if (doFakes == 1) {
    avoidNegativeYields(p_fake_alt_up.SRHH.TOTAL);
    avoidNegativeYields(p_fake_alt_up.SRHL.TOTAL);
    avoidNegativeYields(p_fake_alt_up.SRLL.TOTAL);
    avoidSmallYieldsDueToNegWeightsInPromptSubtraction(p_fake_alt_up.SRHH.TOTAL);
    avoidSmallYieldsDueToNegWeightsInPromptSubtraction(p_fake_alt_up.SRHL.TOTAL);
    avoidSmallYieldsDueToNegWeightsInPromptSubtraction(p_fake_alt_up.SRLL.TOTAL);
  }
  if (isData  == 0) {
    avoidNegativeYields(p_jes_alt_up.SRHH.TOTAL);
    avoidNegativeYields(p_jes_alt_up.SRHL.TOTAL);
    avoidNegativeYields(p_jes_alt_up.SRLL.TOTAL);
    avoidNegativeYields(p_jes_alt_dn.SRHH.TOTAL);
    avoidNegativeYields(p_jes_alt_dn.SRHL.TOTAL);
    avoidNegativeYields(p_jes_alt_dn.SRLL.TOTAL);
    avoidNegativeYields(p_btagSF_alt_up.SRHH.TOTAL);
    avoidNegativeYields(p_btagSF_alt_up.SRHL.TOTAL);
    avoidNegativeYields(p_btagSF_alt_up.SRLL.TOTAL);
    avoidNegativeYields(p_btagSF_alt_dn.SRHH.TOTAL);
    avoidNegativeYields(p_btagSF_alt_dn.SRHL.TOTAL);
    avoidNegativeYields(p_btagSF_alt_dn.SRLL.TOTAL);
    avoidNegativeYields(p_pu_alt_up.SRHH.TOTAL);
    avoidNegativeYields(p_pu_alt_up.SRHL.TOTAL);
    avoidNegativeYields(p_pu_alt_up.SRLL.TOTAL);
    avoidNegativeYields(p_pu_alt_dn.SRHH.TOTAL);
    avoidNegativeYields(p_pu_alt_dn.SRHL.TOTAL);
    avoidNegativeYields(p_pu_alt_dn.SRLL.TOTAL);
  }

  //Update total
  if (!exclude && (!isData || doFlips || doFakes == 1) && !isSignal){
    total.EE    += y_result.EE;
    total.EM    += y_result.EM;
    total.MM    += y_result.MM;
    total.TOTAL += y_result.TOTAL;
  }

  //now save the root files with nominal hists and shape variations
  //Make root files
  if (makeRootFiles) {

    TString kinRegs[] = {"hihi","hilow","lowlow"};

    for (int kr = 0; kr<3;kr++) {

      string name = chainTitle.Data();

      TFile *fileOut = TFile::Open(Form("../../cards/%s/%s_histos_%s_%.1fifb.root",tag.c_str(),name.c_str(),kinRegs[kr].Data(),lumiAG),"RECREATE");
      TH1F* plot = 0;
      if (kinRegs[kr]=="hihi") plot=p_result.SRHH.TOTAL;
      else if (kinRegs[kr]=="hilow") plot=p_result.SRHL.TOTAL;
      else if (kinRegs[kr]=="lowlow") plot=p_result.SRLL.TOTAL;
      else exit(1);

      //hack to mitigate statistical uncertainty due to empty control regions for fakes
      if (name=="fakes") {
	//in case prediction is zero, take statistical upper limit from fakes_mc 
	TFile *file_fakes_mc = TFile::Open(Form("../../cards/%s/fakes_mc_histos_%s_%.1fifb.root",tag.c_str(),kinRegs[kr].Data(),lumiAG),"OPEN");
	if (file_fakes_mc==0) {
	  if (!suppressWarns) cout << "warning, need fakes_mc to set stat for fakes" << endl;
	  assert(0);
	}
	TH1F* h_mc = (TH1F*) file_fakes_mc->Get("sr");
	float scaleFact = 1.2;//from T+L control region
	for (int bin=1;bin<=plot->GetNbinsX();++bin) {
	  if (plot->GetBinContent(bin)<=2E-6) {
	    if (!suppressWarns) cout << "warning: plot " << plot->GetName() << " has zero stat unc in bin " << bin << " value=" << plot->GetBinContent(bin) << "; setting to MC pred=" << scaleFact*h_mc->GetBinContent(bin) << endl;
	    plot->SetBinError(bin,scaleFact*h_mc->GetBinContent(bin));
	  } 
	}
	file_fakes_mc->Close();
	fileOut->cd();
      }

      //write the central sr plot
      TH1F* h_sr = (TH1F*) plot->Clone("sr");
      h_sr->Write();
      
      //now do systematics

      //stats
      writeStat(plot,name);

      //signal isr
      if (isFastSimSignal) {
	    TH1F* plot_alt = 0;
        if      (kinRegs[kr] == "hihi")   plot_alt = p_isr_alt_up.SRHH.TOTAL;
        else if (kinRegs[kr] == "hilow")  plot_alt = p_isr_alt_up.SRHL.TOTAL;
        else if (kinRegs[kr] == "lowlow") plot_alt = p_isr_alt_up.SRLL.TOTAL;
        else exit(1);
        TH1F* isrUp    = (TH1F*) plot_alt->Clone("isrUp");
	isrUp->Scale(h_sr->Integral()/isrUp->Integral());
        TH1F* isrDown  = (TH1F*) plot_alt->Clone("isrDown");
        fillDownMirrorUp(h_sr,isrUp,isrDown);
        isrUp->Write();
        isrDown->Write();
      }

      //signal xsec
      if (isFastSimSignal) {
	    TH1F* plot_alt = 0;
        if      (kinRegs[kr] == "hihi")   plot_alt = p_xsec_alt_up.SRHH.TOTAL;
        else if (kinRegs[kr] == "hilow")  plot_alt = p_xsec_alt_up.SRHL.TOTAL;
        else if (kinRegs[kr] == "lowlow") plot_alt = p_xsec_alt_up.SRLL.TOTAL;
        else exit(1);
        TH1F* xsecUp = (TH1F*) plot_alt->Clone("xsecUp");
        avoidNegativeYields(xsecUp);
        xsecUp->Scale(h_sr->Integral()/xsecUp->Integral());
        xsecUp->Write();
        if      (kinRegs[kr] == "hihi")   plot_alt = p_xsec_alt_dn.SRHH.TOTAL;
        else if (kinRegs[kr] == "hilow")  plot_alt = p_xsec_alt_dn.SRHL.TOTAL;
        else if (kinRegs[kr] == "lowlow") plot_alt = p_xsec_alt_dn.SRLL.TOTAL;
        else exit(1);
        TH1F* xsecDn = (TH1F*) plot_alt->Clone("xsecDown");
        avoidNegativeYields(xsecDn);
        xsecDn->Scale(h_sr->Integral()/xsecDn->Integral());
        xsecDn->Write();
      }

      //fakes ewk
      if (doFakes == 1) {
        TH1F* plot_alt = 0;
        if      (kinRegs[kr] == "hihi")   plot_alt = p_fake_alt_up.SRHH.TOTAL;
        else if (kinRegs[kr] == "hilow")  plot_alt = p_fake_alt_up.SRHL.TOTAL;
        else if (kinRegs[kr] == "lowlow") plot_alt = p_fake_alt_up.SRLL.TOTAL;
        else exit(1);
        TH1F* fakes_EWKUp    = (TH1F*) plot_alt->Clone("fakes_EWKUp");
        TH1F* fakes_EWKDown  = (TH1F*) plot_alt->Clone("fakes_EWKDown");
        fillDownMirrorUp(h_sr,fakes_EWKUp,fakes_EWKDown);
        fakes_EWKUp->Write();
        fakes_EWKDown->Write();
      }
      
      //ttv
      if (name=="ttw" || name=="ttzh"){
        writeTTVExtrSyst(h_sr,name,kinRegs[kr]);
      }
      
      if (!isData && !doFlips && !doFakes) {
        //btag: init
        TH1F* plot_btagSF_up_alt = 0; 
        TH1F* plot_btagSF_ct_alt = 0; 
        TH1F* plot_btagSF_dn_alt = 0; 
        if      (kinRegs[kr]=="hihi")   plot_btagSF_up_alt = p_btagSF_alt_up.SRHH.TOTAL;
        else if (kinRegs[kr]=="hilow")  plot_btagSF_up_alt = p_btagSF_alt_up.SRHL.TOTAL;
        else if (kinRegs[kr]=="lowlow") plot_btagSF_up_alt = p_btagSF_alt_up.SRLL.TOTAL;
        if      (kinRegs[kr]=="hihi")   plot_btagSF_dn_alt = p_btagSF_alt_dn.SRHH.TOTAL;
        else if (kinRegs[kr]=="hilow")  plot_btagSF_dn_alt = p_btagSF_alt_dn.SRHL.TOTAL;
        else if (kinRegs[kr]=="lowlow") plot_btagSF_dn_alt = p_btagSF_alt_dn.SRLL.TOTAL;
        if      (kinRegs[kr]=="hihi")   plot_btagSF_ct_alt = p_result.SRHH.TOTAL;
        else if (kinRegs[kr]=="hilow")  plot_btagSF_ct_alt = p_result.SRHL.TOTAL;
        else if (kinRegs[kr]=="lowlow") plot_btagSF_ct_alt = p_result.SRLL.TOTAL;
        //btag: normalize (be careful with bins inclusive in btags, they are not affected by this syst)
	int nBins = plot_btagSF_ct_alt->GetNbinsX();
	int nInclBins = 2;
	if (kinRegs[kr]=="hilow") nInclBins = 4;
	if (kinRegs[kr]=="lowlow") nInclBins = 1;
        float scaleBtag_up = plot_btagSF_ct_alt->Integral(1,nBins-nInclBins)/plot_btagSF_up_alt->Integral(1,nBins-nInclBins);
        float scaleBtag_dn = plot_btagSF_ct_alt->Integral(1,nBins-nInclBins)/plot_btagSF_dn_alt->Integral(1,nBins-nInclBins);
        plot_btagSF_up_alt->Scale(scaleBtag_up); 
        plot_btagSF_dn_alt->Scale(scaleBtag_dn); 
	for (int ibin=1;ibin<=nInclBins;ibin++) {
	  plot_btagSF_up_alt->SetBinContent(nBins-nInclBins+ibin,plot_btagSF_ct_alt->GetBinContent(nBins-nInclBins+ibin));
	  plot_btagSF_dn_alt->SetBinContent(nBins-nInclBins+ibin,plot_btagSF_ct_alt->GetBinContent(nBins-nInclBins+ibin));
	}
        //btag: now save the plot
        TH1F* btagSFUp   = (TH1F*)plot_btagSF_up_alt->Clone("btagUp"); 
        TH1F* btagSFDown = (TH1F*)plot_btagSF_dn_alt->Clone("btagDown"); 
        btagSFUp  ->SetTitle("btagUp");
        btagSFDown->SetTitle("btagDown");
        btagSFUp  ->Write(); 
        btagSFDown->Write(); 

        //jes
        TH1F* plot_alt_jes_up = 0;
        TH1F* plot_alt_jes_dn = 0;
        if      (kinRegs[kr]=="hihi")   plot_alt_jes_up=p_jes_alt_up.SRHH.TOTAL;
        else if (kinRegs[kr]=="hilow")  plot_alt_jes_up=p_jes_alt_up.SRHL.TOTAL;
        else if (kinRegs[kr]=="lowlow") plot_alt_jes_up=p_jes_alt_up.SRLL.TOTAL;
        else exit(1);
        if      (kinRegs[kr]=="hihi")   plot_alt_jes_dn=p_jes_alt_dn.SRHH.TOTAL;
        else if (kinRegs[kr]=="hilow")  plot_alt_jes_dn=p_jes_alt_dn.SRHL.TOTAL;
        else if (kinRegs[kr]=="lowlow") plot_alt_jes_dn=p_jes_alt_dn.SRLL.TOTAL;
        else exit(1);
        TH1F* jesUp   = (TH1F*) plot_alt_jes_up->Clone("jesUp");
        TH1F* jesDown = (TH1F*) plot_alt_jes_dn->Clone("jesDown");
	if (name=="wz") {
	  //wz is normalized in data, so we want only the sr migration
	  jesUp->Scale(h_sr->Integral()/jesUp->Integral());  
	  jesDown->Scale(h_sr->Integral()/jesDown->Integral());  
	}
        jesUp->Write();
        jesDown->Write();

        //pu
        TH1F* plot_alt_pu_up = 0;
        TH1F* plot_alt_pu_dn = 0;
        if      (kinRegs[kr]=="hihi")   plot_alt_pu_up=p_pu_alt_up.SRHH.TOTAL;
        else if (kinRegs[kr]=="hilow")  plot_alt_pu_up=p_pu_alt_up.SRHL.TOTAL;
        else if (kinRegs[kr]=="lowlow") plot_alt_pu_up=p_pu_alt_up.SRLL.TOTAL;
        else exit(1);
        if      (kinRegs[kr]=="hihi")   plot_alt_pu_dn=p_pu_alt_dn.SRHH.TOTAL;
        else if (kinRegs[kr]=="hilow")  plot_alt_pu_dn=p_pu_alt_dn.SRHL.TOTAL;
        else if (kinRegs[kr]=="lowlow") plot_alt_pu_dn=p_pu_alt_dn.SRLL.TOTAL;
        else exit(1);
        TH1F* puUp   = (TH1F*) plot_alt_pu_up->Clone("puUp");
        TH1F* puDown = (TH1F*) plot_alt_pu_dn->Clone("puDown");
	if (name=="wz") {
	  //wz is normalized in data, so we want only the sr migration
	  puUp->Scale(h_sr->Integral()/puUp->Integral());  
	  puDown->Scale(h_sr->Integral()/puDown->Integral());  
	}
        puUp->Write();
        puDown->Write();

        //leptons
        if (name!="wz") {
	  writeHTHltSyst(h_sr,name,kinRegs[kr]);
	  writeBSHltSyst(h_sr,name,kinRegs[kr]);
	}
    }
      
      //end systematics
      fileOut->Close();
    }
    
  }

  //Return yield
  return pair<yields_t, plots_t>(y_result,p_result); 

}

bool isSRHighHT(TString kine, int sr) {
  if (kine.Contains("hihi")) {
    if (sr==1 || sr==3 || sr==9 || sr==11 || sr==17 || sr==19 || sr==25 || sr==27 || sr==29) return false;
  } else if (kine.Contains("hilow")) {
    if (sr==1 || sr==3 || sr==7 || sr==9 || sr==13 || sr==15 || sr==19 || sr==21 || sr==23) return false;
  } 
  return true;
}

int nbtagsSR(TString kine, int sr) {
  if (kine.Contains("hihi")) {
    if (sr>=1 && sr<=8)        return  0;//0 btag
    else if (sr>=9  && sr<=16) return  1;//1 btag
    else if (sr>=17 && sr<=24) return  2;//2 btag
    else if (sr>=17 && sr<=24) return  3;//3+btag
    else                       return -1;//inclusive regions
  } else if (kine.Contains("hilow")) {
    if (sr>=1 && sr<=6)        return  0;//0 btag
    else if (sr>=7  && sr<=12) return  1;//1 btag
    else if (sr>=13 && sr<=18) return  2;//2 btag
    else if (sr>=19 && sr<=22) return  3;//3+btag
    else                       return -1;//inclusive regions
  } else if (kine.Contains("lowlow")) {
    if (sr>=1 && sr<=2)        return  0;//0 btag
    else if (sr>=3 && sr<=4)   return  1;//1 btag
    else if (sr>=5 && sr<=6)   return  2;//2 btag
    else if (sr==7)            return  3;//3+btag
    else                       return -1;//inclusive regions
  }
  cout << "error! cannot find this SR!" << endl;
  return -999;
}


void avoidNegativeYields(TH1F* plot) {
  for (int bin=1;bin<=plot->GetNbinsX();++bin) {
    if (plot->GetBinContent(bin)<0) {
      if (!suppressWarns) cout << "warning: plot " << plot->GetName() << " has negative yield in bin " << bin << " value=" << plot->GetBinContent(bin) << "; setting to 1E-6."<< endl;
      plot->SetBinContent(bin,1E-6);
    } 
  }
}

void avoidSmallYieldsDueToNegWeightsInPromptSubtraction(TH1F* plot) {
  for (int bin=1;bin<=plot->GetNbinsX();++bin) {
    if (plot->GetBinContent(bin)<1E-4) {
      if (!suppressWarns) cout << "warning: plot " << plot->GetName() << " has yield<1E-4 in bin " << bin << " value=" << plot->GetBinContent(bin) << "; setting to 1E-6."<< endl;
      plot->SetBinContent(bin,1E-6);
    } 
  }
}

void writeStatUpDown(TH1F* central,string name,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* statUpDown = new TH1F(Form("%s_stat%s",name.c_str(),updown.Data()),
			      Form("%s_stat%s",name.c_str(),updown.Data()),
			      central->GetNbinsX(),central->GetXaxis()->GetXmin(),central->GetXaxis()->GetXmax());
  for (int bin=1;bin<=statUpDown->GetNbinsX();++bin) {
    float val = down ? (central->GetBinContent(bin)-central->GetBinError(bin)) : (central->GetBinContent(bin)+central->GetBinError(bin));
    //if (name=="fakes") cout << bin << " val=" << val << " c=" << central->GetBinContent(bin) << " e=" << central->GetBinError(bin) << endl;
    if (val>0) statUpDown->SetBinContent(bin,val);
    else statUpDown->SetBinContent(bin,1E-6);
  }
  statUpDown->Write();
}

void writeStat(TH1F* central,string name) {
  writeStatUpDown(central,name,0);
  writeStatUpDown(central,name,1);
}

void writeTTVExtrSyst(TH1F* central,string name,TString kine) {
  TString up = "Up";
  TString down = "Down";
  TH1F* systUpHTH = (TH1F*) central->Clone(Form("%s_extr_hth%s",name.c_str(),up.Data()));
  systUpHTH->SetTitle(Form("%s_extr_hth%s",name.c_str(),up.Data()));
  TH1F* systDownHTH = (TH1F*) central->Clone(Form("%s_extr_hth%s",name.c_str(),down.Data()));
  systDownHTH->SetTitle(Form("%s_extr_hth%s",name.c_str(),down.Data()));
  TH1F* systUpHTL = (TH1F*) central->Clone(Form("%s_extr_htl%s",name.c_str(),up.Data()));
  systUpHTL->SetTitle(Form("%s_extr_htl%s",name.c_str(),up.Data()));
  TH1F* systDownHTL = (TH1F*) central->Clone(Form("%s_extr_htl%s",name.c_str(),down.Data()));
  systDownHTL->SetTitle(Form("%s_extr_htl%s",name.c_str(),down.Data()));
  TH1F* systUpLL = (TH1F*) central->Clone(Form("%s_extr_ll%s",name.c_str(),up.Data()));
  systUpLL->SetTitle(Form("%s_extr_ll%s",name.c_str(),up.Data()));
  TH1F* systDownLL = (TH1F*) central->Clone(Form("%s_extr_ll%s",name.c_str(),down.Data()));
  systDownLL->SetTitle(Form("%s_extr_ll%s",name.c_str(),down.Data()));
  if (kine.Contains("lowlow")) {
    float systValue = 1.08;
    for (int bin=1;bin<=systUpLL->GetNbinsX();++bin) {
      float val = central->GetBinContent(bin)*systValue;
      if (val>0) systUpLL->SetBinContent(bin,val);
    }
    fillDownMirrorUp(central,systUpLL,systDownLL);
  } else {
    //hihi and hilow have different extrapolation unceratinties for hi and low HT
    //HTHigh
    float systValue = 1.08;
    for (int bin=1;bin<=systUpHTH->GetNbinsX();++bin) {
      //skip low ht regions
      if (!isSRHighHT(kine,bin)) continue;
      float val = central->GetBinContent(bin)*systValue;
      if (val>0) systUpHTH->SetBinContent(bin,val);
    }
    fillDownMirrorUp(central,systUpHTH,systDownHTH);
    //HTLow
    systValue = 1.03;
    for (int bin=1;bin<=systUpHTL->GetNbinsX();++bin) {
      if (isSRHighHT(kine,bin)) continue;
      float val = central->GetBinContent(bin)*systValue;
      if (val>0) systUpHTL->SetBinContent(bin,val);
    }
    fillDownMirrorUp(central,systUpHTL,systDownHTL);
  }
  systUpHTH->Write();
  systDownHTH->Write();
  systUpHTL->Write();
  systDownHTL->Write();
  systUpLL->Write();
  systDownLL->Write();
}

void writeHTHltSyst(TH1F* central,string name,TString kine) {
  TString up = "Up";
  TH1F* systUp = (TH1F*) central->Clone(Form("hthlt%s",up.Data()));
  systUp->SetTitle(Form("hthlt%s",up.Data()));
  float systValue = 1.02;
  for (int bin=1;bin<=systUp->GetNbinsX();++bin) {
    if (!isSRHighHT(kine,bin)) continue;
    float val = central->GetBinContent(bin)*systValue;
    if (val>0) systUp->SetBinContent(bin,val);
  }
  TString down = "Down";
  TH1F* systDown = (TH1F*) central->Clone(Form("hthlt%s",down.Data()));
  systDown->SetTitle(Form("hthlt%s",down.Data()));
  fillDownMirrorUp(central,systUp,systDown);
  systUp->Write();
  systDown->Write();
}

void writeBSHltSyst(TH1F* central,string name,TString kine) {
  TString up = "Up";
  TH1F* systUp = (TH1F*) central->Clone(Form("bshlt%s",up.Data()));
  systUp->SetTitle(Form("bshlt%s",up.Data()));
  float systValue = 1.02;
  for (int bin=1;bin<=systUp->GetNbinsX();++bin) {
    if (isSRHighHT(kine,bin)) continue;
    float val = central->GetBinContent(bin)*systValue;
    if (val>0) systUp->SetBinContent(bin,val);
  }
  TString down = "Down";
  TH1F* systDown = (TH1F*) central->Clone(Form("bshlt%s",down.Data()));
  systDown->SetTitle(Form("bshlt%s",down.Data()));
  fillDownMirrorUp(central,systUp,systDown);
  systUp->Write();
  systDown->Write();
}

void fillDownMirrorUp(TH1F* central,TH1F* up,TH1F* down) {
  down->Reset();
  down->Add(up);
  down->Scale(-1);
  down->Add(central);
  down->Add(central);
  //need to avoid negative values...
  avoidNegativeYields(down);
}
