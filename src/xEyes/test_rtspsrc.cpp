/*
*------------------------------------------------------------------------
*test_rtspsrc.cpp
*
* This code was developed by Shunguang Wu in his spare time. No government
* or any client funds were used.
*
*
* THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
*
* IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTOR BE LIABLE FOR
* ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
* OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
* WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
* LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
* OF THIS SOFTWARE.
*
* Permission to use, copy, modify, distribute, and sell this software and
* its documentation for any purpose is prohibited unless it is granted under
* the author's written notice.
*
* Copyright(c) 2020 by Shunguang Wu, All Right Reserved
*-------------------------------------------------------------------------
*/
//approach 1: w/o opencv
//https://devtalk.nvidia.com/default/topic/1011376/jetson-tx1/gstreamer-decode-live-video-stream-with-the-delay-difference-between-gst-launch-1-0-command-and-appsink-callback/post/5158760/#5158760

//cmd
//~$ gst-launch-1.0 rtspsrc location=rtsp://admin:admin12345@192.168.0.64:554/Streaming/Channels/101?transportmode=unicastprofile=Profile_1 protocols=tcp latency=0 ! decodebin ! videoconvert ! xvimagesink

//code 
