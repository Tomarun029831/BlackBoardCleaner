#ifndef _KIC_
#define _KIC_
#include "./SPool.h"
#include <WString.h>

//ex) KIC:V1;1437;1090010001130;208000900;8;9;/
//ex) KIC:V1;1437;1_0900_1000_1130;2_0800_0900;8;9;/

#define KICVERSION "KIC:V1"
#define KICSEGMENTCHAR ';'
#define KICEND "/"

namespace KIC {
    SPool* convertToSPool(String& kic);
}
#endif // !_KIC_

