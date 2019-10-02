
#include "nopane.h"
#include <utilfuncs/utilfuncs.h>


void NOPane::Paint(Draw &drw)
{
	Size sz=GetSize();
	drw.DrawRect(sz, SColorFace());
	std::string sn;
	for (int i=topline; i<numlines; i++)
	{
		if (bText) sn=spf(i+1); else sn=tohex(i*offsetfactor, 8);
		int fontheight=((lineheight<10)?6:(lineheight<15)?10:13);
		Size tsz=GetTextSize(sn.c_str(), Monospace(fontheight));
		int x=(sz.cx-tsz.cx)/2;
		int y=(i*lineheight-scrollpos)+((lineheight-tsz.cy)/2);
		if ((y>=0)&&(y<sz.cy)) drw.DrawText(x, y, sn.c_str(), Monospace(fontheight), Color(20, 80, 80));
		if (y>sz.cy) break;
	}
}
