
#include "dirdiff.h"
#include "diffglobs.h"
#include <utilfuncs/utilfuncs.h>
//#include "fdd.h"
#include "fddmain.h"
#include "fddpics.h"

int pint(int d) { return  (d==DS_EQU)?PICSAME:(d==DS_LEFT)?PICLEFT:(d==DS_RIGHT)?PICRIGHT:PICERROR; };


DirDiff::DirDiff()
{
	Add(dpane.HSizePosZ().VSizePosZ());
}

void DirDiff::showdiffs()
{
	FW_FONT=StdFont().Height(DEF_F_HEIGHT);
	dpane.ClearTree();
	dpane.AddColumn("tree", GetSize().cx-10);
	dpane.ShowHeader(false);
	dpane.ListExpandingNodes(EXD_FIRST);
	dpane.ShowTreeLines(true);
	dpane.UseTGHelpMenu(false);
	PNode troot=dpane.AddNode(0, GetFDDPic(PICROOT), "Comparing directories..", "");
	troot->Expandable();
	showdiffs(troot, ((DDiffs*)(pMain->pDT))->dd);
	dpane.Expand(troot);
	dpane.RefreshTreeGrid();
	Sync();
}

void DirDiff::showdiffs(PNode N, DDiff &dd)
{
	for (auto pp:dd.content)
	{
		N->AddNode(pp.first, "", GetFDDPic(pint(pp.second)));
	}
	for (auto p:dd)
	{
		PNode NN=N->AddNode(p.first, "", GetFDDPic(pint(p.second.disp)));
		NN->Expandable();
		showdiffs(NN, p.second);
		dpane.Expand(NN);
	}
	dpane.RefreshTreeGrid();
	Sync();
}
