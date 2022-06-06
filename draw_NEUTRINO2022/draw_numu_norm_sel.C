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

  TLegend * lg = new TLegend(0.60, 0.50, 0.93, 0.92);//adjust the legend box position here

  style::ResetStyle(lg, 0.2);//width of the key
  lg->SetHeader("nuSTORM #nu_{#mu} Preliminary","C");//need to change to #nu_{e} everywhere for #nu_{e}

  const int *cols=style::GetColorArray();

  TList* hists = new TList;

  int iii = 0;

  for(int ii=0; ii<lout->GetSize(); ii++){
    TH1F * hh = dynamic_cast<TH1F *> (lout->At(ii));
    style::ResetStyle(hh);

    hists->Add(hh);

    std::cout<<iii<<std::endl;

    if ((ii != 5) and (ii != 7)) {

      //here set for each histogram
      hh->GetXaxis()->SetTitle("#it{E}_{#nu} (GeV)");
      hh->GetYaxis()->SetTitle("#it{#Phi}_{#nu_{#mu}}(#it{E}_{#nu}) (area normalised)");//Question: is it really event rates of interaction, or just flux?
      hh->GetXaxis()->SetRangeUser(0,6); //used 8 for nu_e and 8.5 for nu_mu
      hh->SetLineWidth(2);
      // if (ii==7){
      //   hh->SetLineColor(style::GetColor(cols[9]));
      // }
      // else {
      if (iii==4){
        iii = iii+1;
      }
      hh->SetLineColor(style::GetColor(cols[iii]));
      //}
      hh->Draw(ii?"hist same":"hist");//use C for smooth curve, expected to work for high statistics
      const TString tmp=hh->GetName();
      //lg->AddEntry(hh, Form("abc %c", tmp[6]), "l");//Here you have to replace hh->GetName() by the actual identifier of the histogram

    }

    iii = iii+1;
    if ((iii==7) or (iii==3) or (iii==9)){
      iii = iii+1;
    }
  }

  lg->AddEntry(hists->At(0),"#it{p}_{#mu} = 0.57 GeV/#it{c} #pm 16%", "l");
  lg->AddEntry(hists->At(1),"#it{p}_{#mu} = 1.16 GeV/#it{c} #pm 16%", "l");
  lg->AddEntry(hists->At(2),"#it{p}_{#mu} = 1.80 GeV/#it{c} #pm 16%", "l");
  lg->AddEntry(hists->At(3),"#it{p}_{#mu} = 2.46 GeV/#it{c} #pm 16%", "l");
  lg->AddEntry(hists->At(4),"#it{p}_{#mu} = 3.12 GeV/#it{c} #pm 16%", "l");
  // lg->AddEntry(hists->At(5),"#it{p}_{#mu} = 3.85 GeV/#it{c} #pm 16%", "l");
  lg->AddEntry(hists->At(6),"#it{p}_{#mu} = 4.60 GeV/#it{c} #pm 16%", "l");
  // lg->AddEntry(hists->At(7),"#it{p}_{#mu} = 5.30 GeV/#it{c} #pm 16%", "l");
  lg->AddEntry(hists->At(8),"#it{p}_{#mu} = 6.00 GeV/#it{c} #pm 16%", "l");

  lg->Draw();
  cc->Print("hnumuE_norm_sel.png");
  cc->Print("hnumuE_norm_sel.pdf");
  cc->Print("hnumuE_norm_sel.eps");
}

int main()
{
  TFile *fin = new TFile("hnumuE_norm.root");//first, save your TCanvas by Print("test.root"), assuming your TCanvas has name c1
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
