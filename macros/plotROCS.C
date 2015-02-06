{

  gROOT->Reset();

  TCanvas *c1_n2 = new TCanvas("c1_n2", "c1_n2",10,10,700,500);
  gStyle->SetOptStat(0);

  bool doMiniIso = false;
  bool doPtrelSub = true;
  bool doPtrelV0 = true;

  //T1ttttmG1500
  TGraph *g_dummy = new TGraph(20);
  g_dummy->SetName("G_dummy");
  g_dummy->SetTitle("");
  g_dummy->SetPoint(0,0.5,0.5);
  g_dummy->SetPoint(1,1.,1.);
  g_dummy->Draw("AP");
  g_dummy->GetXaxis()->SetTitle("T1tttt efficiency");
  g_dummy->GetYaxis()->SetTitle("ttbar rejection");
  g_dummy->Draw("AP");  

  if (doMiniIso) { 
  TGraph *g_miniIso = new TGraph(21);
  g_miniIso->SetName("G_MiniIso");
  g_miniIso->SetTitle("G_MiniIso");
  g_miniIso->SetLineColor(kBlue);
  g_miniIso->SetLineWidth(2);
  g_miniIso->SetLineStyle(2);
  g_miniIso->SetPoint(0,0,1);
  g_miniIso->SetPoint(1,0.73385638,0.9949425827);   
  g_miniIso->SetPoint(2,0.8117079139,0.9877012819); 
  g_miniIso->SetPoint(3,0.8587809205,0.9727589004); 
  g_miniIso->SetPoint(4,0.8823174238,0.9527590722); 
  g_miniIso->SetPoint(5,0.8925769329,0.9257478341); 
  g_miniIso->SetPoint(6,0.9082679749,0.8939090893); 
  g_miniIso->SetPoint(7,0.9143029451,0.8573577851); 
  g_miniIso->SetPoint(8,0.9185274839,0.8221857697); 
  g_miniIso->SetPoint(9,0.9245624542,0.781266734);  
  g_miniIso->SetPoint(10,0.9312009811,0.7341409028);
  g_miniIso->SetPoint(11,0.936029017,0.6844863892); 
  g_miniIso->SetPoint(12,0.9402534962,0.6386248767);
  g_miniIso->SetPoint(13,0.9438744783,0.5923036337);
  g_miniIso->SetPoint(14,0.9468920231,0.5477065146);
  g_miniIso->SetPoint(15,0.9493060112,0.5058679283);
  g_miniIso->SetPoint(16,0.9541339874,0.4690867662);
  g_miniIso->SetPoint(17,0.9589620233,0.4373629689);
  g_miniIso->SetPoint(18,0.9613760114,0.4035702348);
  g_miniIso->SetPoint(19,0.9619795084,0.374605);    
  g_miniIso->SetPoint(20,1,0);
  g_miniIso->Draw("l");  
  TGraph *g_miniIso_12 = new TGraph(21);
  g_miniIso_12->SetName("G_MiniIso");
  g_miniIso_12->SetTitle("G_MiniIso");
  g_miniIso_12->SetLineColor(kBlue);
  g_miniIso_12->SetLineWidth(2);
  g_miniIso_12->SetPoint(0,0,1);
  g_miniIso_12->SetPoint(1,0.4670743048,0.9949425827); 
  g_miniIso_12->SetPoint(2,0.5957115889,0.9877012819); 
  g_miniIso_12->SetPoint(3,0.6753441691,0.9727589004); 
  g_miniIso_12->SetPoint(4,0.7381314039,0.9527590722); 
  g_miniIso_12->SetPoint(5,0.7764162421,0.9257478341); 
  g_miniIso_12->SetPoint(6,0.8131697774,0.8939090893); 
  g_miniIso_12->SetPoint(7,0.8346092701,0.8573577851); 
  g_miniIso_12->SetPoint(8,0.8575801849,0.8221857697); 
  g_miniIso_12->SetPoint(9,0.8728941679,0.781266734);	 
  g_miniIso_12->SetPoint(10,0.8882080913,0.7341409028);
  g_miniIso_12->SetPoint(11,0.9035220742,0.6844863892);
  g_miniIso_12->SetPoint(12,0.9127104282,0.6386248767);
  g_miniIso_12->SetPoint(13,0.9188359976,0.5923036337);
  g_miniIso_12->SetPoint(14,0.9234302044,0.5477065146);
  g_miniIso_12->SetPoint(15,0.9295557737,0.5058679283);
  g_miniIso_12->SetPoint(16,0.9356813431,0.4690867662);
  g_miniIso_12->SetPoint(17,0.9387441874,0.4373629689);
  g_miniIso_12->SetPoint(18,0.941806972,0.4035702348); 
  g_miniIso_12->SetPoint(19,0.9464011192,0.374605);    
  g_miniIso_12->SetPoint(20,1,0);
  g_miniIso_12->Draw("l");
  }

  if (doPtrelSub) {
  TGraph *g_ptrelsub = new TGraph(10);
  g_ptrelsub->SetName("G_Ptrelsub");
  g_ptrelsub->SetTitle("G_Ptrelsub");
  g_ptrelsub->SetLineColor(kRed);
  g_ptrelsub->SetLineWidth(2);
  g_ptrelsub->SetLineStyle(2);
  g_ptrelsub->SetPoint(10,0,1);
  g_ptrelsub->SetPoint(1,0.9873265028,0.3960931897);
  g_ptrelsub->SetPoint(2,0.9547374845,0.7890834361);
  g_ptrelsub->SetPoint(3,0.919734478,0.9091970101);	
  g_ptrelsub->SetPoint(4,0.885334909,0.9520697631);	
  g_ptrelsub->SetPoint(5,0.8473144174,0.9690809902);
  g_ptrelsub->SetPoint(6,0.7996379137,0.9812646881);
  g_ptrelsub->SetPoint(7,0.7616173625,0.9880461805);
  g_ptrelsub->SetPoint(8,0.7302353382,0.9912645165);
  g_ptrelsub->SetPoint(9,0.6825588346,0.9942529728);
  g_ptrelsub->SetPoint(0,1,0);
  g_ptrelsub->Draw("l");
  TGraph *g_ptrelsub_12 = new TGraph(10);
  g_ptrelsub_12->SetName("G_Ptrelsub_12");
  g_ptrelsub_12->SetTitle("G_Ptrelsub_12");
  g_ptrelsub_12->SetLineColor(kRed);
  g_ptrelsub_12->SetLineWidth(2);
  g_ptrelsub_12->SetPoint(10,0,1);
  g_ptrelsub_12->SetPoint(1,0.9509953856,0.3960931897);
  g_ptrelsub_12->SetPoint(2,0.8744257092,0.7890834361);
  g_ptrelsub_12->SetPoint(3,0.7687596083,0.9091970101);
  g_ptrelsub_12->SetPoint(4,0.6401225328,0.9520697631);
  g_ptrelsub_12->SetPoint(5,0.5375191569,0.9690809902);
  g_ptrelsub_12->SetPoint(6,0.430321604,0.9812646881);	
  g_ptrelsub_12->SetPoint(7,0.3522205353,0.9880461805);
  g_ptrelsub_12->SetPoint(8,0.2756508589,0.9912645165);
  g_ptrelsub_12->SetPoint(9,0.2128637284,0.9942529728);
  g_ptrelsub_12->SetPoint(0,1,0);
  g_ptrelsub_12->Draw("l");
  }

  if (doPtrelV0) {
  TGraph *g_ptrelv0 = new TGraph(11);
  g_ptrelv0->SetName("Graph");
  g_ptrelv0->SetTitle("Graph");
  g_ptrelv0->SetLineColor(kBlack);
  g_ptrelv0->SetLineWidth(2);
  g_ptrelv0->SetLineStyle(2);
  g_ptrelv0->SetPoint(0,1,0);
  g_ptrelv0->SetPoint(1,0.9064574242,0.7691841125);
  g_ptrelv0->SetPoint(2,0.8358479142,0.9299973026);
  g_ptrelv0->SetPoint(3,0.7857573628,0.9643664286);
  g_ptrelv0->SetPoint(4,0.7272178531,0.9805739671);
  g_ptrelv0->SetPoint(5,0.6783343554,0.990804242);	
  g_ptrelv0->SetPoint(6,0.628243804,0.9948273865);	
  g_ptrelv0->SetPoint(7,0.5709112883,0.9960918031);
  g_ptrelv0->SetPoint(8,0.5165962577,0.9972412726);
  g_ptrelv0->SetPoint(9,0.4737477303,0.9981608483);
  g_ptrelv0->SetPoint(10,0,1);
  g_ptrelv0->Draw("l");  
  TGraph *g_ptrelv0_12 = new TGraph(11);
  g_ptrelv0_12->SetName("Graph");
  g_ptrelv0_12->SetTitle("Graph");
  g_ptrelv0_12->SetLineColor(kBlack);
  g_ptrelv0_12->SetLineWidth(2);
  g_ptrelv0_12->SetPoint(0,1,0);
  g_ptrelv0_12->SetPoint(1,0.7503828406,0.7691841125); 
  g_ptrelv0_12->SetPoint(2,0.574272573,0.9299973026);	 
  g_ptrelv0_12->SetPoint(3,0.4042878747,0.9643664286); 
  g_ptrelv0_12->SetPoint(4,0.2695252299,0.9805739671); 
  g_ptrelv0_12->SetPoint(5,0.171516031,0.990804242);	 
  g_ptrelv0_12->SetPoint(6,0.1117916852,0.9948273865); 
  g_ptrelv0_12->SetPoint(7,0.07503825426,0.9960918031);
  g_ptrelv0_12->SetPoint(8,0.04900457337,0.9972412726);
  g_ptrelv0_12->SetPoint(9,0.02909646742,0.9981608483);
  g_ptrelv0_12->SetPoint(10,0,1);
  g_ptrelv0_12->Draw("l");
  }

  TLegend* leg  = new TLegend(0.2,0.2,0.5,0.5);
  leg->SetNColumns(1);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  if (doPtrelV0) {
    leg->AddEntry(g_ptrelv0,"T1tttt1500 pTrel v0","L");
    leg->AddEntry(g_ptrelv0_12,"T1tttt1200 pTrel v0","L");
  }
  if (doPtrelSub) {
    leg->AddEntry(g_ptrelsub,"T1tttt1500 pTrel v1","L");
    leg->AddEntry(g_ptrelsub_12,"T1tttt1200 pTrel v1","L");
  }
  if (doMiniIso) {
    leg->AddEntry(g_miniIso,"T1tttt1500 miniIso","L");
    leg->AddEntry(g_miniIso_12,"T1tttt1200 miniIso","L");
  }
  leg->Draw();


}
