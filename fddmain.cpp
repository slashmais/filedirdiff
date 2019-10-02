
#include "fddmain.h"
#include "txtdiff.h"
#include "bindiff.h"
#include "dirdiff.h"
#include <utilfuncs/utilfuncs.h>
#include "fddpics.h"
#include <uppfun/uppfun.h>


//===================================================================================================
EBSel::EBSel()
{
	Add(eb.HSizePosZ(0,36).TopPos(0,20));
	eb.WhenEnter = [&]{ if (EChanged) EChanged(); };
	
	Add(btnD.RightPosZ(1,16).TopPos(2,18));
	btnD.SetImage(GetFDDPic(PICSELDIR));
	btnD.WhenAction = [&]{ String S=SelectDirectory(); if (!S.IsEmpty()) { eb.SetData(S); if (EChanged) EChanged(); }};
	btnD.Tip("Select directory");
	
	Add(btnF.RightPosZ(18,16).TopPos(2,18));
	btnF.SetImage(GetFDDPic(PICSELFILE));
	btnF.WhenAction = [&]{ String S=SelectFileOpen("*"); if (!S.IsEmpty()) { eb.SetData(S); if (EChanged) EChanged(); }};
	btnF.Tip("Select File");
}

//===================================================================================================
SelPane::SelPane()
{
	auto eschg = [&]{ btn.SetLabel("Compare"); if (EChanged) EChanged(); };
	AddFrame(ThinInsetFrame());
	esL.EChanged = eschg;
	btn.SetRectX(0,66);
	btn.SetLabel("Compare");
	btn.WhenAction = [&]{ if (ECompare) ECompare(); };
	esR.EChanged = eschg;
}

void SelPane::Layout()
{
	Size sz=GetSize();
	int bw=66; //btn.GetSize().cx;
	int ebw=((sz.cx-bw)/2-4);
	Add(esL.LeftPos(2, ebw).TopPos(2,20));
	Add(btn.LeftPos(ebw+6, bw).TopPos(2,20));
	Add(esR.LeftPos(ebw+bw+8, ebw).TopPos(2,20));
}

void SelPane::BtnLabel(String S)	{ btn.SetLabel(S); }
String SelPane::BtnLabel()			{ return btn.GetLabel();  }
void SelPane::LText(String S)		{ esL.eb.SetData(S); }
String SelPane::LText()				{ return esL.eb.GetData().ToString(); }
void SelPane::RText(String S)		{ esR.eb.SetData(S); }
String SelPane::RText()				{ return esR.eb.GetData().ToString(); }

//===================================================================================================
FDDMain::FDDMain()
{
	Title("File/Directory Differences");
	Sizeable();
	CenterScreen();
	Add(selpane.HSizePosZ().TopPosZ(0, 22));
	selpane.EChanged = [&]{ selpane.BtnLabel("Compare"); destroy_diffwin(); reset(); };
	selpane.ECompare = THISFN(OnCompare);
}

void FDDMain::OnCompare()
{
	std::string sl=selpane.BtnLabel().ToStd();
	if (pDT&&pDT->is_txt()&&(sieqs(sl.substr(2,5),"focus")))
	{
		bool bL;
		if ((bL=(sl[0]!='R'))) { selpane.BtnLabel("R-Focus"); } else { selpane.BtnLabel("L-Focus"); }
		((FDiffs*)pDT)->sort(bL);
		pDW->Refresh();
	}
	else
	{
		destroy_diffwin();
		if (pDT) { pDT->clear(); delete pDT; pDT=nullptr; }
		ProgressBox PB(this);
		PB.Show();
		auto pbcb=[&PB](int c, int t, std::string s){ PB.ShowProgress(c, t, s); PB.Sync(); };
		if (DoDiff(selpane.LText().ToStd(), selpane.RText().ToStd(), &pDT, pbcb)) { if (create_diffwin()) show_diffwin(); }
	}
}

bool FDDMain::create_diffwin()
{
	destroy_diffwin();
	if (pDT)
	{
		if (pDT->is_txt())		{ pDW=new TxtDiff; }
		else if (pDT->is_bin())	{ pDW=new BinDiff; }
		else if (pDT->is_dir())	{ pDW=new DirDiff; }
		if (pDW) { pDW->pMain=this; Add((*pDW).HSizePosZ().VSizePosZ(selpane.GetSize().cy, 0)); }
	}
	return (pDW!=nullptr);
}

void FDDMain::show_diffwin() { if (pDW) pDW->showdiffs(); }
void FDDMain::destroy_diffwin() { if (pDW) { RemoveChild(pDW); delete pDW; pDW=nullptr; }}

