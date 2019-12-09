
void convert_rgb_to_yuv420(const unsigned char* rgb, unsigned width, unsigned height, unsigned channels, unsigned char* yuv) {
	//
	Npp8u* srcMemory;
	Npp8u* dstMemory;

	if (!rgb || !yuv || (width <= 0) || (height <= 0) || (channels < 3) || (channels > 4))
		return;

	cudaMalloc((void **)&dstMemory, width*height * 3 / 2);
	if (!dstMemory)
		return;

	cudaMalloc((void **)&srcMemory, width*height*channels);
	if (!srcMemory) {
		cudaFree(dstMemory);
		return;
	}

	cudaMemcpy(srcMemory, rgb, width*height*channels, cudaMemcpyHostToDevice);
	NppiSize roi = { width, height };
	Npp8u* dstBuff[3] = { dstMemory, dstMemory + width*height, dstMemory + width*height * 5 / 4 };
	int dstSteps[3] = { width, width / 2, width / 2 };

	if (channels == 3) {
		nppiRGBToYCbCr420_8u_C3P3R(srcMemory, width*channels, dstBuff, dstSteps, roi);
	}
	else {
		nppiRGBToYCrCb420_8u_AC4P3R(srcMemory, width*channels, dstBuff, dstSteps, roi);
	}

	cudaMemcpy(yuv, dstMemory, width*height * 3 / 2, cudaMemcpyDeviceToHost);
	cudaFree(dstMemory);
	cudaFree(srcMemory);
}

void H26xEncoder::pushFrame(uint8_t** framePlanes, int* framePlaneSizes, int Planes)
{
	if (Initialized == true && framePlanes != NULL) {
		// GPU time 1: Measure time for buffer processing + cudaMemcpy
		const int64 start1 = getTickCount();

		struct v4l2_buffer v4l2_buf;
		struct v4l2_plane planes[MAX_PLANES];

		memset(&v4l2_buf, 0, sizeof(v4l2_buf));
		memset(planes, 0, MAX_PLANES * sizeof(struct v4l2_plane));

		v4l2_buf.m.planes = planes;


		// Check if we need dqBuffer first
		if (bufferIndex < MAX_ENCODER_FRAMES &&
			ctx.enc->output_plane.getNumQueuedBuffers() <
			ctx.enc->output_plane.getNumBuffers())
		{
			// The queue is not full, no need to dqBuffer
			// Prepare buffer index for the following qBuffer
			printf("bufferIndex: %d\n", bufferIndex);
			v4l2_buf.index = bufferIndex++;

			NvBufferCreateParams init_params = { 0 };
			init_params.width = 2064;
			init_params.height = 1544;
			init_params.layout = NvBufferLayout_Pitch;
			init_params.colorFormat = NvBufferColorFormat_YUV420;

			if (NvBufferCreateEx(&fd, &init_params) == -1) {
				printf("Failed to create dma_buf\n");
			}
			else {
				printf("fd=%d\n", fd);
			}
		}
		else{
			ctx.enc->output_plane.dqBuffer(v4l2_buf, NULL, NULL, 10); // 10
			fd = v4l2_buf.m.planes[0].m.fd;
		}


		NvBufferParams params;
		NvBufferGetParams(fd, &params);
		status = cuGraphicsEGLRegisterImage(&resource, eglImage, CU_GRAPHICS_MAP_RESOURCE_FLAGS_WRITE_DISCARD);
		if (status != CUDA_SUCCESS) {
			printf("cuGraphicsEGLRegisterImage failed: %d.\n", status);
		}
		status = cuGraphicsResourceGetMappedEglFrame(&eglFrame, resource, 0, 0);
		if (status != CUDA_SUCCESS) {
			printf("cuGraphicsResourceGetMappedEglFrame failed: %d.\n", status);
		}
		eglImage = NvEGLImageFromFd(display, fd);
		if (eglImage == NULL) {
			cout << "create eglImage failed" << endl;
		}

		int bytes_to_read;
		bytes_to_read = params.pitch[0] * params.height[0];
		cudaMemcpy((cudaArray_t)eglFrame.frame.pArray[0], framePlanes[0], bytes_to_read, cudaMemcpyDeviceToDevice);
		bytes_to_read = params.pitch[1] * params.height[1];
		cudaMemcpy((cudaArray_t)eglFrame.frame.pArray[1], framePlanes[1], bytes_to_read, cudaMemcpyDeviceToDevice);

		bytes_to_read = params.pitch[2] * params.height[2];
		cudaMemcpy((cudaArray_t)eglFrame.frame.pArray[2], framePlanes[2], bytes_to_read, cudaMemcpyDeviceToDevice);

		// Push the frame into V4L2.
		v4l2_buf.m.planes[0].m.fd = fd;
		v4l2_buf.m.planes[0].bytesused = 1; // byteused must be non-zero
		ctx.enc->output_plane.qBuffer(v4l2_buf, NULL);

		// Measure time until pushing into buffer
		const double timeSec1 = (getTickCount() - start1) / getTickFrequency();
		cout << "GPU Time 1 : " << timeSec1 * 1000 << " ms" << endl;

		// Measure time for resource unregister function
		const int64 start = getTickCount();
		status = cuGraphicsUnregisterResource(resource);
		if (status != CUDA_SUCCESS){
			printf("cuGraphicsEGLUnRegisterResource failed: %d\n", status);
		}
		const double timeSec = (getTickCount() - start) / getTickFrequency();
		cout << "GPU Time 2 : " << timeSec * 1000 << " ms" << endl;

	} // end of if initialized==true
} // end of pushFrame function