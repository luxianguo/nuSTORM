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
  cc->SetTopMargin(0.01);//top margin

  TLegend * lg = new TLegend(0.72, 0.35, 0.93, 0.95);//adjust the legend box position here
  
  style::ResetStyle(lg, 0.2);//width of the key
  lg->SetHeader("nuSTORM #nu_{#mu} flux");//need to change to #nu_{e} everywhere for #nu_{e}

  const int *cols=style::GetColorArray();
  
  for(int ii=0; ii<lout->GetSize(); ii++){
    TH1F * hh = dynamic_cast<TH1F *> (lout->At(ii));
    style::ResetStyle(hh);

    //here set for each histogram
    hh->GetXaxis()->SetTitle("#it{E}_{#nu} (GeV)");
    hh->GetYaxis()->SetTitle("#it{#Phi}_{#nu_{#mu}}(#it{E}_{#nu}) (a.u.)");//Question: is it really event rates of interaction, or just flux?
    hh->GetXaxis()->SetRangeUser(0,8);//8 is just a dummy here, need to update as necessary
    hh->SetLineWidth(2);
    hh->SetLineColor(style::GetColor(cols[ii]));
    hh->Draw(ii?"C same":"C");//use C for smooth curve, expected to work for high statistics
    const TString tmp=hh->GetName();
    lg->AddEntry(hh, Form("abc %c", tmp[6]), "l");//Here you have to replace hh->GetName() by the actual identifier of the histogram
  }

  lg->Draw();
  cc->Print("out.png");
}

int main()
{
  TFile *fin = new TFile("test.root");//first, save your TCanvas by Print("test.root"), assuming your TCanvas has name c1
  TCanvas *c1 = (TCanvas*) fin->Get("c1");
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

