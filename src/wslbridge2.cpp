/* 
 * This file is part of wslbridge2 project.
 * Licensed under the terms of the GNU General Public License v3 or later.
 * Copyright (C) 2019-2020 Biswapriyo Nath.
 */

/* DO NOT include winsock.h, conflicts with poll.h. */
#include <windows.h>
#include <assert.h>
#include <getopt.h>
// #include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#ifdef __CYGWIN__
#include <sys/cygwin.h>
#endif
// #include <sys/ioctl.h>
// #include <termios.h>
#include <unistd.h>

#include <array>
#include <string>
#include <thread>
#include <vector>

#include "common.hpp"
#include "GetVmId.hpp"
#include "Helpers.hpp"
#include "Environment.hpp"
#include "WindowsSock.hpp"

void printf_guid(GUID guid) {
    printf("%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX", 
      guid.Data1, guid.Data2, guid.Data3, 
      guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
      guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

int main(int argc, char *argv[]) {
    /* Minimum requirement Windows 10 build 17763 aka. version 1809 */
    if (GetWindowsBuild() < 17763)
        fatal("Windows 10 version is older than minimal requirement.\n");

    /* Initialize WinSock. */
    WindowsSock();

    /* Intialize COM. */
    ComInit();

    std::string distroName = "";
    GUID DistroId, VmId;

    /* Detect WSL version. Assume distroName is initialized empty. */
    const bool wslTwo = IsWslTwo(&DistroId, mbsToWcs(distroName));
    if (!wslTwo)
        fatal("WSL 2 is required\n");

    const HRESULT hRes = GetVmId(&DistroId, &VmId);
    if (hRes != 0)
        fatal("GetVmId: %s\n", GetErrorMessage(hRes).c_str());
    
    printf_guid(VmId);

    return 0;
}
