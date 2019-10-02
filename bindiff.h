#ifndef _diffbin_h_
#define _diffbin_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <string>
#include <vector>
#include "diffwin.h"
#include "nopane.h"


//===================================================================================================
struct BinDiff;
struct BinVScrollPane : public Ctrl
{
	using CLASSNAME=BinVScrollPane;
	VScrollBar VS;
	Event<> EScrolled;
	virtual ~BinVScrollPane() {}
	BinVScrollPane()						{ SetRectX(0,34); Add(VS.HSizePosZ(5,5).VSizePosZ()); AddFrame(BlackFrame()); VS.WhenScroll=THISFN(OnScroll); } //ThinInsetFrame()); }
	virtual void Layout()					{ VS.SetPage(GetSize().cy); }
	virtual void MouseWheel(Point p, int zdelta, dword keyflags) { VS.Wheel(zdelta); if (EScrolled) EScrolled(); }
	void OnScroll()							{ if (EScrolled) EScrolled(); }
	void SetVPT(int p, int t)				{ int tt=VS.GetTotal(); VS.Set(p); VS.SetTotal(max(t,tt)); }
	void SetVStep(int y)					{ VS.SetLine(y); }
	int GetV()								{ return VS.Get(); }
};

struct BinPane : public Ctrl
{
	using CLASSNAME=BinPane;
	HScrollBar HS;
	BinDiff *pD;
	bool bLeft{true};
	virtual ~BinPane() {}
	BinPane();
	virtual void DoScroll(); // { Refresh(); pD->nopane.settopline(pD->vspane.GetV()); } //Sync(); }
	virtual void Layout(); //{ HS.SetPage(GetSize().cy); }
	virtual void Paint(Draw &drw);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
};

//------------------------------------------------
struct BinDiff : public DiffWin
{
	using CLASSNAME=BinDiff;
	NOPane nopane;
	BinPane lpane;
	BinVScrollPane vspane;
	BinPane rpane;
	virtual ~BinDiff() {}
	BinDiff();
	virtual void Layout();
	virtual void showdiffs();
};



#endif
