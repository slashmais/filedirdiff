#include "fddpics.h"

#define  IMAGEFILE <filedirdiff/fddpics.iml>
#define  IMAGECLASS FDDPICS
#include <Draw/iml.h>


Image GetFDDPic(int npic)
{
	switch(npic)
	{
		case PICSELDIR:			return FDDPICS::PicSelDir(); break;
		case PICSELFILE:		return FDDPICS::PicSelFile(); break;
		case PICROOT:			return FDDPICS::PicRoot(); break;
		case PICLEFT:			return FDDPICS::PicLeft(); break;
		case PICRIGHT:			return FDDPICS::PicRight(); break;
		case PICSAME:			return FDDPICS::PicSame(); break;
		case PICERROR:			return FDDPICS::PicError(); break;

	}
	return FDDPICS::PicUnkErr();
}

