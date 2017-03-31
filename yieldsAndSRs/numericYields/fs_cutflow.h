TChain *fs_t1tttt_1400_1000_chain = new TChain("t","fs_t1tttt_m1400_m1000" );
fs_t1tttt_1400_1000_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/T1TTTT_main.root");
pair<yields_t, plots_t> results_fs_t1tttt_1400_1000 = run(fs_t1tttt_1400_1000_chain, 0, 0, 0, 0, 1);
delete fs_t1tttt_1400_1000_chain;

TChain *fs_t1tttt_1500_200_chain = new TChain("t","fs_t1tttt_m1500_m200" );
fs_t1tttt_1500_200_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/T1TTTT_main.root");
pair<yields_t, plots_t> results_fs_t1tttt_1500_200 = run(fs_t1tttt_1500_200_chain, 0, 0, 0, 0, 1);
delete fs_t1tttt_1500_200_chain;

TChain *fs_t5qqqqvv_1000_700_chain = new TChain("t","fs_t5qqqqvv_m1000_m700" );
fs_t5qqqqvv_1000_700_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/T5QQQQVV_main.root");
pair<yields_t, plots_t> results_fs_t5qqqqvv_1000_700 = run(fs_t5qqqqvv_1000_700_chain, 0, 0, 0, 0, 1);
delete fs_t5qqqqvv_1000_700_chain;

TChain *fs_t5qqqqvv_1200_400_chain = new TChain("t","fs_t5qqqqvv_m1200_m400" );
fs_t5qqqqvv_1200_400_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/T5QQQQVV_main.root");
pair<yields_t, plots_t> results_fs_t5qqqqvv_1200_400 = run(fs_t5qqqqvv_1200_400_chain, 0, 0, 0, 0, 1);
delete fs_t5qqqqvv_1200_400_chain;

TChain *fs_t5qqqqvv_dm20_1200_400_chain = new TChain("t","fs_t5qqqqvv_dm20_m1200_m400" );
fs_t5qqqqvv_dm20_1200_400_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/T5QQQQVV_dm20.root");
pair<yields_t, plots_t> results_fs_t5qqqqvv_dm20_1200_400 = run(fs_t5qqqqvv_dm20_1200_400_chain, 0, 0, 0, 0, 1);
delete fs_t5qqqqvv_dm20_1200_400_chain;

TChain *fs_t5qqqqvv_dm20_1000_700_chain = new TChain("t","fs_t5qqqqvv_dm20_m1000_m700" );
fs_t5qqqqvv_dm20_1000_700_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/T5QQQQVV_dm20.root");
pair<yields_t, plots_t> results_fs_t5qqqqvv_dm20_1000_700 = run(fs_t5qqqqvv_dm20_1000_700_chain, 0, 0, 0, 0, 1);
delete fs_t5qqqqvv_dm20_1000_700_chain;
