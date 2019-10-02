
#include "txtdiff.h"
#include "diffglobs.h"
#include <utilfuncs/utilfuncs.h>
#include "fdd.h"
#include "fddmain.h"


void TextPane::paint_diff_data(Draw &drw, int L, int M, int R, int H)
{
	int TW=0, X=GetH();
	int Y=GetV(), TH=0;
	int x{}, xd{}, y{};
	y=-SB.GetY();// VSB.Get();
	xd=SB.GetX();//HSB.Get();
	Size csz=GetTextSize("W", FW_FONT);
	FDiffs *pFD=(FDiffs*)pD->pMain->pDT;
	TH=(pFD->getlines()*csz.cy);
	for (auto d:pFD->vd)
	{
		Color col;
		switch(d.disp)
		{
			case DS_EQU: { x=M; col=COL_SAME; } break;
			case DS_MOV: { x=M; col=COL_MOVED; } break;
			case DS_ADD: { x=R; col=COL_ADDED; } break;
			case DS_DEL: { x=L; col=COL_DELETED; } break;
			default: { x=M; col=Red(); } break;
		}
		for (size_t i=0; i<d.block.data.size(); i++)
		{
			std::string s=d.block.data[i];
			if (d.disp==DS_DEL) s=spf("[", d.block.lpos[i]+1, "] ", s);
			if (d.disp==DS_ADD) s=spf("[", d.block.rpos[i]+1, "] ", s);
			Size tsz=GetTextSize(s.c_str(), FW_FONT);
			if (TW<(x+tsz.cx)) TW=(x+tsz.cx);
			drw.DrawText(x-xd, y, s.c_str(), FW_FONT, col);
			y+=tsz.cy;
			if (y>H) break;
		}
	}
	SetHPT(X, TW);
	SetVPT(Y, TH);
	SetHVStep(csz.cx, csz.cy);
}
	
void TextPane::Paint(Draw &drw)
{
	Size sz=GetSize();
	drw.DrawRect(1,1,sz.cx-2,sz.cy-2,White());
	FDiffs *pFD=(FDiffs*)pD->pMain->pDT;
	if (pD&&!pFD->vd.empty())
	{
//		FW_FONT=StdFont().Height(DEF_F_HEIGHT);
		paint_diff_data(drw, 2, sz.cx/4, sz.cx/2, sz.cy);
	}
}

//===================================================================================================
TxtDiff::~TxtDiff() {}

TxtDiff::TxtDiff()
{
	Add(nopane.LeftPosZ(2, 60).VSizePosZ(2, 2));
	tpane.pD=this;
	Add(tpane.HSizePosZ(60, 2).VSizePosZ(2, 2));
	tpane.EScrolled = [&]
		{
			int h=GetTextSize("W", FW_FONT).cy;
			int v=tpane.GetV();
			nopane.scrollpos=v;
			nopane.settopline(v/h);
		};
}

void TxtDiff::showdiffs()
{
	FW_FONT=StdFont().Height(DEF_F_HEIGHT);
	tpane.Refresh();
	Sync();
	int h=GetTextSize("W", FW_FONT).cy;
	nopane.lineheight=h;
	int v=tpane.GetV();
	nopane.scrollpos=v;
	FDiffs *pFD=(FDiffs*)tpane.pD->pMain->pDT;
	nopane.setlines(v/h, pFD->getlines());
	tpane.pD->pMain->selpane.BtnLabel("R-Focus"); Sync();
}

