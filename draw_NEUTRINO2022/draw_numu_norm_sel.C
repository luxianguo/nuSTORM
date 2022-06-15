#include "style.h"
#include "TList.h"

void draw(TList * lout)
{
  style::SetGlobalStyle();//publication-level style setting

  //no need to tweak these setting unless really necessary
  //style::fgkTextSize = 0.07; //0.05;
  //style::fgkTitleSize = 0.07;
  //style::fgkYTitleOffset = 0.75;
  //style::fgkXTitleOffset = 1.1;

  TCanvas * cc = new TCanvas("cc","", 800, 600);
  style::PadSetup(cc);
  cc->SetRightMargin(0.03);//right margin as small as possible, not to cut away the x axis label
  cc->SetTopMargin(0.06);//top margin

  TLegend * lg0 = new TLegend(0.66, 0.77, 0.94, 0.91);//adjust the legend box position here
  TLegend * lg = new TLegend(0.78, 0.45, 0.98, 0.77);//adjust the legend box position here

  style::ResetStyle(lg0, 0.2);//width of the key
  style::ResetStyle(lg, 0.2);//width of the key
  lg0->SetHeader("nuSTORM Preliminary","C");//need to change to #nu_{e} everywhere for #nu_{e}
  lg0->AddEntry((TObject*)0, "#bar{#nu}_{#mu} flux (2206v2)", "");
  lg0->AddEntry((TObject*)0, "#it{p}_{#mu} (GeV/#it{c}) #pm 16%", "");

  const int *cols=style::GetColorArray();

  //create new arrays with selected colors for plot and muon central momenta for histograms and legend entries
  const int cols_sel[9] = {cols[0],cols[1],cols[2],cols[5],cols[6],cols[8],cols[10],cols[11],cols[12]};
  const float centPmu[9] = {0.57, 1.16, 1.80, 2.46, 3.12, 3.85, 4.60, 5.30, 6.00};

  for(int ii=0; ii<lout->GetSize(); ii++){
    TH1F * hh = dynamic_cast<TH1F *> (lout->At(ii));
    style::ResetStyle(hh);

    //skip two energies to make plot more legible
    if ((ii != 5) and (ii != 7)) {
      //here set for each histogram
      hh->GetXaxis()->SetTitle("#it{E}_{#nu} (GeV)");
      hh->GetYaxis()->SetTitle("#it{#Phi}_{#bar{#nu}_{#mu}}(#it{E}_{#nu}) (area normalised)");
      hh->GetXaxis()->SetRangeUser(0,6); //used 8 for nu_e and 8.5 for nu_mu
      hh->GetYaxis()->SetRangeUser(0,3.8); //for non-log plot
      // hh->GetYaxis()->SetRangeUser(0.03,4.5); //for log plot
      // hh->GetYaxis()->SetTitleOffset(1.15); //for log plot
      // hh->GetYaxis()->SetLabelOffset(0.001); //for log plot
      hh->SetLineWidth(2);
      hh->SetLineColor(style::GetColor(cols_sel[ii]));
      hh->Draw(ii?"hist same":"hist");//use "hist C" for smooth curve, expected to work for high statistics
      lg->AddEntry(hh, Form("%.2f", centPmu[ii]), "l");
    }
  }

  lg0->Draw();
  lg->Draw();
  cc->Print("hnumuE_norm_sel_ext.png"); //Use this to save non-log plot
  cc->Print("hnumuE_norm_sel_ext.pdf"); //Use this to save non-log plot
  cc->Print("hnumuE_norm_sel_ext.eps"); //Use this to save non-log plot
  // cc->SetLogy(); //Use this to show y-axis in log
  // cc->Print("hnumuE_norm_sel_ext_log.png"); //Use this to save log plot
  // cc->Print("hnumuE_norm_sel_ext_log.pdf"); //Use this to save log plot
  // cc->Print("hnumuE_norm_sel_ext_log.eps"); //Use this to save log plot
}

int main()
{
  TFile *fin = new TFile("hnumuE_norm_ext.root");//first, save your TCanvas by Print("test.root"), assuming your TCanvas has name c1
  TCanvas *c1 = (TCanvas*) fin->Get("c");
  if(!c1){
    printf("no c1\n"); fin->ls(); return 1;
  }
  TList *oldlist = c1->GetListOfPrimitives();
  oldlist->ls();

  TList * lout = new TList;
  for(int ii=0; ii<oldlist->GetSize(); ii++){
    TH1F * hh = dynamic_cast<TH1F *> (oldlist->At(ii));
    if(hh){
      lout->Add(hh);
    }
  }
  lout->ls();

  draw(lout);

  return 0;
}
