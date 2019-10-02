#ifndef _diffwin_h_
#define _diffwin_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

//===================================================================================================
struct ScrollPane : public Ctrl
{
	using CLASSNAME=ScrollPane;
	ScrollBars SB;
	virtual void DoScroll()=0;
	virtual ~ScrollPane()					{}
	ScrollPane()							{ AddFrame(ThinInsetFrame()); AddFrame(SB.AutoHide()); SB.WhenScroll=[&]{ DoScroll(); }; }
	virtual void Layout()					{ SB.SetPage(GetSize()); }
	virtual void MouseWheel(Point p, int zdelta, dword keyflags) { SB.WheelY(zdelta); DoScroll(); }
	void SetVPT(int p, int t)				{ SB.SetY(p); SB.SetTotalY(t); }
	void SetHPT(int p, int t)				{ SB.SetX(p); SB.SetTotalX(t); }
	void SetHVStep(int x, int y)			{ SB.SetLine(x, y); }
	int GetV()								{ return SB.GetY(); }
	int GetH()								{ return SB.GetX(); }
};

//===================================================================================================
struct FDDMain;
struct DiffWin : public Ctrl
{
	using CLASSNAME=DiffWin;
	FDDMain *pMain;
	virtual ~DiffWin(){};
	virtual void showdiffs()=0;
};

#endif
