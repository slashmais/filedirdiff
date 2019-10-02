#ifndef _nopane_h_
#define _nopane_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

//===================================================================================================
struct NOPane : public Ctrl // lineNumber / Offset ..
{
	using CLASSNAME=NOPane;
	int topline{};
	int numlines{};
	int lineheight{};
	int scrollpos{};
	bool bText{};
	int offsetfactor{};
	virtual ~NOPane()					{}
	NOPane(bool btxt=true, int ofac=16)	{ bText=btxt; offsetfactor=ofac; } //AddFrame(BlackFrame()); } //AddFrame(ThinInsetFrame()); }
	void settopline(int n)				{ topline=n; Refresh(); }
	void setnumlines(int n)				{ numlines=n; Refresh(); }
	void setlines(int t, int n)			{ topline=t; numlines=n; Refresh(); }
	void setscrollpos(int p)			{ scrollpos=p; Refresh(); }
	virtual void Paint(Draw &drw);
};



#endif
