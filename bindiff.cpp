
#include "bindiff.h"
#include "diffglobs.h"
#include <utilfuncs/utilfuncs.h>
#include "fdd.h"
#include "fddmain.h"


//===================================================================================================
BinPane::BinPane()
{
	AddFrame(HS.AutoHide());
	std::string sh{}; fshex("0123456789abcdef ", sh);
	Size tsz=GetTextSize(sh, FW_FONT);
	HS.SetPage(GetSize().cx);
	HS.SetTotal(tsz.cx); HS.SetLine(tsz.cy);
	HS.WhenScroll=THISFN(DoScroll);
}

void BinPane::Layout() { HS.SetPage(GetSize().cx); }

void BinPane::DoScroll() { Refresh(); }

void BinPane::Paint(Draw &drw)
{
	int X=HS.Get();
	int Y=pD->vspane.GetV();
	Size sz=GetSize();
	HS.SetPage(sz.cx);
	drw.DrawRect(sz, White());
	std::string s{};
	Color col;
	int x=0, y=0;
	Size tsz=GetTextSize("W", FW_FONT);
	pD->nopane.setscrollpos(Y);
	FDiffs *pFD=(FDiffs*)pD->pMain->pDT;
	for (int i=0; i<pFD->vd.size(); i++)
	{
		FDiff &d=pFD->vd[i];
		y=(i*tsz.cy-Y);
		if (y<0) continue;
		if (y>sz.cy) break;
		if (d.disp==DS_EQU) { col=COL_SAME; fshex(d.block.data[0], s, 16); }
		else if (d.disp==DS_NEQ)
		{
			std::string st=d.block.data[0];
			size_t pos=0;
			int len=0;
			if ((pos=st.find(':'))!=std::string::npos)
			{
				len=stot<int>(st.substr(0, pos));
				if (bLeft) { col=COL_DELETED; st=st.substr(pos+1, len); }
				else { col=COL_ADDED; st=st.substr(pos+len+1); }
			} //else just show whatever is in the buffer..
			fshex(st, s, 16);
		}
		else if (d.disp==DS_NEW) { if (bLeft) { col=Gray(); s="[-]"; } else { col=COL_ADDED; fshex(d.block.data[0], s, 16); }}
		else if (d.disp==DS_OLD) { if (bLeft) { col=COL_DELETED; fshex(d.block.data[0], s, 16); } else { col=Gray(); s="[-]"; }}
		drw.DrawText(x-X, y, s.c_str(), FW_FONT, col);
		if (y>sz.cy) break;
	}
}

void BinPane::MouseWheel(Point p, int zdelta, dword keyflags) { pD->vspane.MouseWheel(p, zdelta, keyflags); }

//===================================================================================================
BinDiff::BinDiff()
{
	FW_FONT=Monospace(DEF_F_HEIGHT);
	Add(nopane.LeftPosZ(0, 60).VSizePosZ(0, 0));
	nopane.bText=false;
	lpane.pD=this;
	lpane.bLeft=true;
	rpane.pD=this;
	rpane.bLeft=false;
	vspane.EScrolled = [&]{ lpane.DoScroll(); rpane.DoScroll(); };
	vspane.SetVPT(0, 0); vspane.SetVStep(1);
}

void BinDiff::Layout()
{
	Size sz=GetSize();
	int nw=nopane.GetSize().cx+2;
	int vsw=28;//vspane.GetSize().cx;
	int wd=(sz.cx-nw-vsw)/2;
	Add(vspane.LeftPos(nw+wd, vsw).VSizePosZ(0,0));
	Add(lpane.LeftPos(nw,wd).VSizePosZ(0,0));
	Add(rpane.LeftPos(nw+wd+vsw,wd).VSizePosZ(0,0));
}

void BinDiff::showdiffs()
{
	FW_FONT=Monospace(DEF_F_HEIGHT);
	FDiffs *pFD=(FDiffs*)pMain->pDT;
	int i=0, nl=(int)pFD->getlines();
	int h=GetTextSize("W", FW_FONT).cy;
	nopane.lineheight=h; //tsz.cy;
	nopane.setlines(0, nl);
	vspane.SetVPT(0, nl*h+h); vspane.SetVStep(h); //tsz.cy);
}


