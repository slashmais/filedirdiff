#ifndef _fddpics_h_
#define _fddpics_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

enum
{
	PIC_UNK_ERR=0,
	PICSELDIR,
	PICSELFILE,
	PICROOT,
	PICLEFT,
	PICRIGHT,
	PICSAME,
	PICERROR,
};

Image GetFDDPic(int npic);

#endif
