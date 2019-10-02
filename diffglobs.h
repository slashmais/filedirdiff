#ifndef _diffglobs_h_
#define _diffglobs_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

//===================================================================================================
extern std::string TAB_SPACE;
extern int DEF_F_HEIGHT;
extern Font FW_FONT;

extern Color COL_SAME; //in both files at same position
extern Color COL_MOVED; //in both files at different positions
extern Color COL_DELETED; //Gray(); //only in left-file
extern Color COL_ADDED; //only in right-file
//Color COL_ERROR=Red(); //?(possible?)-would indicate bug in program?

/* ???
COL_BOTH=Black(); //exists in both files
COL_FOCUS=Blue(); //in focused file only
COL_OTHER=Gray(); //not in focused file
*/

void set_var_placeholder();


#endif
