#ifndef _diffdir_h_
#define _diffdir_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <string>
#include <vector>
#include "diffwin.h"
#include "nopane.h"
#include "fdd.h"
#include <TreeGrid/treegrid.h>


struct DirDiff;

//------------------------------------------------
struct DirDiff : public DiffWin
{
	using CLASSNAME=DirDiff;
	TreeGrid dpane;
	virtual ~DirDiff() {}
	DirDiff();

	virtual void showdiffs();
	virtual void Refresh() { showdiffs(); }
	void showdiffs(PNode N, DDiff &dd);
};


#endif
