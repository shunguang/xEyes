#ifndef _CUDA_UTIL_H_
#define _CUDA_UTIL_H_

#include <helper_string.h>
#include <helper_cuda.h>
//#include <cuda_runtime.h>
#include "DsDefs.h"
namespace xeyes {
    int DS_EXPORT cudaDeviceInit(int argc, const char **argv);
}
#endif
