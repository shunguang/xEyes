//approach 1: w/o opencv
//https://devtalk.nvidia.com/default/topic/1011376/jetson-tx1/gstreamer-decode-live-video-stream-with-the-delay-difference-between-gst-launch-1-0-command-and-appsink-callback/post/5158760/#5158760

//cmd
//~$ gst-launch-1.0 rtspsrc location=rtsp://admin:admin12345@192.168.0.64:554/Streaming/Channels/101?transportmode=unicastprofile=Profile_1 protocols=tcp latency=0 ! decodebin ! videoconvert ! xvimagesink

//code 