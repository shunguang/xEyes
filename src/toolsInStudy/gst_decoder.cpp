Decoder::Decoder(string url)
{
    pthread_mutex_init(&mutex_, NULL);

    /* Initialize GStreamer */
    gst_init(NULL, NULL);

    string launch_string;
    launch_string = "rtspsrc location=" + url + " ! rtph264depay ! ";
    launch_string += "h264parse ! omxh264dec ! ";
    launch_string += "appsink name=mysink";

    GError *error = NULL;
    pipeline_ = (GstPipeline *)gst_parse_launch(launch_string.c_str(), &error);
    if (pipeline_ == NULL) {
        g_print("Failed to parse launch: %s\n", error->message);
        return;
    }

    appsink_ = (GstElement *)gst_bin_get_by_name(GST_BIN(pipeline_), "mysink");
    g_object_set(appsink_, "emit-signals", TRUE, NULL);
    g_signal_connect(appsink_, "new-sample", G_CALLBACK(new_buffer_fcn), this);

    /* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
    GstBus *bus = NULL;
    bus = gst_pipeline_get_bus(pipeline_);
    gst_bus_add_signal_watch(bus);
    g_signal_connect(G_OBJECT(bus), "message::error", (GCallback)error_cb, main_loop_);
    gst_object_unref(bus);

    /* Start playing the pipeline */
    gst_element_set_state((GstElement *)pipeline_, GST_STATE_PLAYING);

    /* Create a GLib Main Loop and set it to run */
    main_loop_ = g_main_loop_new(NULL, FALSE);
    pthread_create(&thread_, NULL, main_loop_fcn, main_loop_);

    return;
}


Decoder::~Decoder()
{
    gst_element_set_state((GstElement*)pipeline_, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline_));
    gst_object_unref(GST_OBJECT(appsink_));
    g_main_loop_quit(main_loop_);
    g_main_loop_unref(main_loop_);
    pthread_mutex_destroy(&mutex_);
}