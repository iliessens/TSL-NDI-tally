#pragma once
#include <Processing.NDI.Lib.h>
#include "NDIlib_loader.h"
#include <string>

using namespace std;

class NDITally {
public:
    NDITally(NDILib_loader* loader, string source);
    ~NDITally();

    bool setState(bool program, bool preview);
    string get_name();

private:
    NDIlib_recv_instance_t pNDI_recv;
    const NDIlib_v4* p_NDILib;
    string name;
};