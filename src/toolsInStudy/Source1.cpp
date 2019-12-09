void RTPTransmitter::sendToNetwork(unsigned char* IMG_data)
{
	// Push the image in appsrc
	GstFlowReturn ret;
	GstMapInfo info;

	guint buffersize = static_cast<guint>(mDataSize);

	GstClockTime timestamp = mNbFrames * 33000000;

	GstBuffer * buffer = gst_buffer_new_allocate(NULL, buffersize, NULL);

	if (!buffer)
		throw std::runtime_error("Can't allocate buffer in sendToNetwork");

	if (gst_buffer_map(buffer, &info, (GstMapFlags)GST_MAP_WRITE))
	{
		memcpy(info.data, IMG_data, buffersize);
		gst_buffer_unmap(buffer, &info);
	}
	else
	{
		gst_buffer_unref(buffer);
		throw std::runtime_error("Can't map buffer in sendToNetwork");
	}

	GST_BUFFER_PTS(buffer) = timestamp;
	if (!GST_BUFFER_PTS_IS_VALID(buffer))
		printf("Failed to setup PTS");

	GST_BUFFER_DTS(buffer) = timestamp;
	if (!GST_BUFFER_DTS_IS_VALID(buffer))
		printf("Failed to setup DTS");

	GST_BUFFER_DURATION(buffer) = 33000000;
	if (!GST_BUFFER_DURATION_IS_VALID(buffer))
		printf("Failed to setup duration\n");

	GST_BUFFER_OFFSET(buffer) = mNbFrames;
	if (!GST_BUFFER_OFFSET_IS_VALID(buffer))
		printf("Failed to setup offset");

	g_signal_emit_by_name(mpVideoSource, "push-buffer", buffer, &ret);

	if (ret != GST_FLOW_OK)
	{
		gst_buffer_unref(buffer);
		throw std::runtime_error("Can't push buffer in sendToNetwork");
	}

	gst_buffer_unref(buffer);

	mNbFrames++;
}
