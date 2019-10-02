#ifndef _difftxt_h_
#define _difftxt_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <string>
#include <vector>
#include "diffwin.h"
#include "nopane.h"

//===================================================================================================
struct TxtDiff;
struct TextPane : public ScrollPane
{
	using CLASSNAME=TextPane;
	TxtDiff *pD;
	Event<> EScrolled;
	virtual void DoScroll()					{ Refresh(); Sync(); if (EScrolled) EScrolled(); }
	virtual ~TextPane()						{ }//vsf.clear(); }
	TextPane()								{}
	void paint_diff_data(Draw &drw, int L, int M, int R, int H);
	virtual void Paint(Draw &drw);
};

//------------------------------------------------
struct TxtDiff : public DiffWin
{
	using CLASSNAME=TxtDiff;
	NOPane nopane;
	TextPane tpane;
	virtual ~TxtDiff();
	TxtDiff();

	virtual void showdiffs();
};



#endif
