
#include "diffglobs.h"

std::string TAB_SPACE=std::string("    ");
int DEF_F_HEIGHT=15;
Font FW_FONT=StdFont().Height(DEF_F_HEIGHT);

Color COL_SAME=Black(); //in both files at same position
Color COL_MOVED=Green(); //in both files at different positions
Color COL_DELETED=Red(); //Gray(); //only in left-file
Color COL_ADDED=Blue(); //only in right-file
//Color COL_ERROR=Red(); //?(possible?)-would indicate bug in program?

/* ???
COL_BOTH=Black(); //exists in both files
COL_FOCUS=Blue(); //in focused file only
COL_OTHER=Gray(); //not in focused file
*/


void set_var_placeholder() {}

