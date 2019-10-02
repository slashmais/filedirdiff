#ifndef _fddiffmain_h_
#define _fddiffmain_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <string>
#include <vector>
#include "diffwin.h"
#include "fdd.h"

struct EBSel : public Ctrl //edit-box with selection-button
{
	using CLASSNAME=EBSel;
	EditString eb;
	Button btnD;
	Button btnF;
	virtual ~EBSel(){}
	EBSel();
	Event<> EChanged;
};

struct SelPane : public Ctrl
{
	using CLASSNAME=SelPane;
	EBSel esL, esR;
	Button btn;
	int focus{};
	Event<> EChanged;
	Event<> ECompare;
	virtual ~SelPane(){}
	SelPane();
	void Layout();
	void BtnLabel(String lbl);
	String BtnLabel();
	void LText(String S);
	String LText();
	void RText(String S);
	String RText();
};

struct FDDMain : public TopWindow
{
	typedef FDDMain CLASSNAME;
	SelPane selpane;
	DiffType *pDT{nullptr};
	DiffWin *pDW{nullptr};
	void reset() { if (pDW) { delete pDW; pDW=nullptr; } if (pDT) { delete pDT; pDT=nullptr; }}
	virtual ~FDDMain() { reset(); }
	FDDMain();
	void OnCompare();
	bool create_diffwin();
	void show_diffwin();
	void destroy_diffwin();

};

#endif
