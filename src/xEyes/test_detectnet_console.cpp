/*
 * Copyright (c) 2017, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
Pre-trained Detection Models Available
Below is a table of the pre-trained object detection networks available for download, and the associated --network argument to detectnet-console used for loading the pre-trained models:

Model                   CLI argument	        NetworkType enum	Object classes
SSD-Mobilenet-v1	    ssd-mobilenet-v1	    SSD_MOBILENET_V1	91 (COCO classes)
SSD-Mobilenet-v2	    ssd-mobilenet-v2	    SSD_MOBILENET_V2	91 (COCO classes)
SSD-Inception-v2	    ssd-inception-v2	    SSD_INCEPTION_V2	91 (COCO classes)
DetectNet-COCO-Dog	    coco-dog	            COCO_DOG	        dogs
DetectNet-COCO-Bottle	coco-bottle	            COCO_BOTTLE	        bottles
DetectNet-COCO-Chair	coco-chair	            COCO_CHAIR	        chairs
DetectNet-COCO-Airplane	coco-airplane	        COCO_AIRPLANE	    airplanes
ped-100	                pednet	                PEDNET	            pedestrians
multiped-500	        multiped	            PEDNET_MULTI	    pedestrians, luggage
facenet-120	            facenet	                FACENET	            faces
*/
#include "detectNet.h"
#include "loadImage.h"

#include "commandLine.h"
#include "cudaMappedMemory.h"
#include "libUtil/util.h"

using namespace xeyes;
using namespace std;

void test_rgb2float4();
static int usage()
{
	printf("usage: detectnet-console [-h] [--network NETWORK] [--threshold THRESHOLD]\n");
	printf("                         file_in [file_out]\n\n");
	printf("Locate objects in an image using an object detection DNN.\n\n");
	printf("positional arguments:\n");
	printf("  file_in           filename of the input image to process\n");
	printf("  file_out          filename of the output image to save (optional)\n\n");
	printf("optional arguments:\n");
	printf("  --help            show this help message and exit\n");
	printf("  --network NETWORK pre-trained model to load (see below for options)\n");	
	printf("  --overlay OVERLAY detection overlay flags (e.g. --overlay=box,labels,conf)\n");
	printf("                    valid combinations are:  'box', 'labels', 'conf', 'none'\n");
	printf("  --alpha ALPHA     overlay alpha blending value, range 0-255 (default: 120)\n");
	printf("  --threshold VALUE minimum threshold for detection (default is 0.5)\n\n");
	printf("%s\n", detectNet::Usage());

	return 0;
}

int test_detectnet_console( int argc, char** argv )
{
	test_rgb2float4();
	return;

	usage();
	/*
	 * parse command line
	 */
	commandLine cmdLine(argc, argv);
	if( cmdLine.GetFlag("help") ){
		return -1;
	}


	/*
	 * create detection network
	 */
	detectNet* net = detectNet::Create(argc, argv);
	if( !net ){
		printf("detectnet-console:   failed to initialize detectNet\n");
		return 0;
	}

	// parse overlay flags
	const uint32_t overlayFlags = detectNet::OverlayFlagsFromStr(cmdLine.GetString("overlay", "box,labels,conf"));

	/*
	 * load image from disk
	 */
	const std::string inpdirName="/home/swu/projects/dataSets/PennFudanPed/PNGImages";
	const std::string outDirName=".";
	const std::string ext="png";
	std::vector<std::string> vFileNames; 
	uint32_t nImages = getFileNameList( inpdirName, ext, vFileNames );
	for(int i=0; i<nImages; ++i){
		boost::posix_time::ptime t1 = APP_LOCAL_TIME;

		//assume the size of each img is different
		float* imgCPU    = NULL;
		float* imgCUDA   = NULL;
		int    imgWidth  = 0;
		int    imgHeight = 0;

		std::string inpImgFilename= inpdirName + "/" + vFileNames[i];
		std::string outputFilename= outDirName + "/" + vFileNames[i];
		
		if( !loadImageRGBA(inpImgFilename.c_str(), (float4**)&imgCPU, (float4**)&imgCUDA, &imgWidth, &imgHeight) ){
			printf("failed to load image: '%s'\n", inpImgFilename.c_str());
			return 0;
		}
		int dt = timeIntervalMillisec(t1);		

		printf("%d/%d --- load image imgSize = (%d,%d), dt=%d(ms)\n", i, nImages, imgWidth, imgHeight, dt);
	

		/*
	 	* detect objects in image
	 	*/
		t1 = APP_LOCAL_TIME;
		detectNet::Detection* detections = NULL;
		const int numDetections = net->Detect(imgCUDA, imgWidth, imgHeight, &detections, overlayFlags);

		// print out the detection results
		dt = timeIntervalMillisec(t1);		
		printf("%i objects detected, detetction time=%d(ms)\n", numDetections,dt);
	
		for( int n=0; n < numDetections; n++ ){
			printf("detected obj %u  class #%u (%s)  confidence=%f\n", detections[n].Instance, detections[n].ClassID, net->GetClassDesc(detections[n].ClassID), detections[n].Confidence);
			printf("bounding box %u  (%f, %f)  (%f, %f)  w=%f  h=%f\n", detections[n].Instance, detections[n].Left, detections[n].Top, detections[n].Right, detections[n].Bottom, detections[n].Width(), detections[n].Height()); 
		}
	
		// print out timing info
		net->PrintProfilerTimes();
	
		// save image to disk
		if( !outputFilename.empty() ){
			printf("detectnet-console:  writing %ix%i image to '%s'\n", imgWidth, imgHeight, outputFilename.c_str());
		
			if( !saveImageRGBA(outputFilename.c_str(), (float4*)imgCPU, imgWidth, imgHeight, 255.0f) )
				printf("detectnet-console:  failed saving %ix%i image to '%s'\n", imgWidth, imgHeight, outputFilename.c_str());
			else	
				printf("detectnet-console:  successfully wrote %ix%i image to '%s'\n", imgWidth, imgHeight, outputFilename.c_str());
		}
		//assume the size of each img is different
		CUDA(cudaFreeHost(imgCPU));
	}

	/*
	 * destroy resources
	 */
	printf("detectnet-console:  shutting down...\n");

	SAFE_DELETE(net);

	printf("detectnet-console:  shutdown complete\n");
	return 0;
}


#include "libDet/DetThreadDeepNet.h"
void test_rgb2float4()
{
	cv::Mat I = imread("C:\\images\\apple.jpg", 0);
	if (I.empty()){
		printf("!!! Failed imread(): image not found");
		return;
	}

	float4* cpuRgba;
	float4* gpuRgba;
	int w = I.cols, h = I.rows;
	uint32_t  imgSzBytes = w * h * 4 * sizeof(float);
	if (!cudaAllocMapped((void**)&cpuRgba, (void**)&gpuRgba, imgSzBytes)) {
		printf("DetThreadDeepNet::procInit(): failed to allocate %zu bytes", imgSzBytes);
		return;
	}

	bool f = DetThreadDeepNet::rgb2float4(cpuRgba, w, h, I);
	if (f) {
		saveImageRGBA("./tmp.png", cpuRgba, w, h, 255.0f);
	}
	CUDA( cudaFreeHost(cpuRgba) );
}
