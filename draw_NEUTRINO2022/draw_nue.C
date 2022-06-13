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

  TLegend * lg = new TLegend(0.60, 0.40, 0.93, 0.92);//adjust the legend box position here

  style::ResetStyle(lg, 0.2);//width of the key
  lg->SetHeader("nuSTORM #nu_{e} Preliminary","C");//need to change to #nu_{e} everywhere for #nu_{e}

  const int *cols=style::GetColorArray();

  //create new arrays with selected colors for plot and muon central momenta for histograms and legend entries --> arrays need to be same size as lout !
  const int cols_sel[9] = {cols[0],cols[1],cols[2],cols[5],cols[6],cols[8],cols[10],cols[11],cols[12]};
  const float centPmu[9] = {0.57, 1.16, 1.80, 2.46, 3.12, 3.85, 4.60, 5.30, 6.00};
  const TString percent = "%"; //needs to be created because single % will give error in Form()

  for(int ii=0; ii<lout->GetSize(); ii++){
    TH1F * hh = dynamic_cast<TH1F *> (lout->At(ii));
    style::ResetStyle(hh);

    //here set for each histogram
    hh->GetXaxis()->SetTitle("#it{E}_{#nu} (GeV)");
    hh->GetYaxis()->SetTitle("#it{#Phi}_{#nu_{e}}(#it{E}_{#nu}) (a.u.)");//Question: is it really event rates of interaction, or just flux?
    hh->GetXaxis()->SetRangeUser(0,8); //used 8 for nu_e and 8.5 for nu_mu
    hh->SetLineWidth(2);
    hh->SetLineColor(style::GetColor(cols_sel[lout->GetSize()-1-ii]));
    hh->Draw(ii?"hist same":"hist");//use C for smooth curve, expected to work for high statistics
    const TString tmp=Form("#it{p}_{#mu} = %.2f GeV/#it{c} #pm 16", centPmu[lout->GetSize()-1-ii]);
    lg->AddEntry(hh, tmp+percent, "l");
  }

  lg->Draw();
  cc->Print("hnueE.png");
  cc->Print("hnueE.pdf");
}

int main()
{
  TFile *fin = new TFile("hnueE.root");//first, save your TCanvas by Print("test.root"), assuming your TCanvas has name c1
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
