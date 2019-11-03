#include "CudaUtil.h"

using namespace std;
int xeyes :: cudaDeviceInit(int argc, const char **argv)
{
	int deviceCount;
	checkCudaErrors(cudaGetDeviceCount(&deviceCount));

	if (deviceCount == 0)
	{
		std::cerr << "CUDA error: no devices supporting CUDA." << std::endl;
		exit(EXIT_FAILURE);
	}

	int dev = findCudaDevice(argc, argv);

	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, dev);
	std::cerr << "cudaSetDevice GPU" << dev << " = " << deviceProp.name << std::endl;

	checkCudaErrors(cudaSetDevice(dev));

	return dev;
}
