int test_gpu_availability(int argc, char **argv)
{
	return 0;
}

#if 0
#include "libUtil/DataTypes.h"
#include "libUtil/UtilFuncs.h"
#include <opencv2/cudaarithm.hpp>
#include <cmath>
#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaimgproc.hpp"

using namespace std;
using namespace cv;
using namespace cv::cuda;
double getPSNR(const Mat& I1, const Mat& I2, int n);
double getPSNR_CUDA(const Mat& I1, const Mat& I2, int n);
int test_hough(int argc, char**argv);
int test_matrix_op(int argc, char **argv);

int test_gpu_availability(int argc, char **argv)
{
	//test_matrix_op(argc, argv);
	test_hough( argc, argv);
	return 0;
}

int test_matrix_op(int argc, char **argv)
{
	int nn = 10;
	int w = 50000, h = 4000;
	if (argc == 3) {
		w = atoi(argv[1]);
		h = atoi(argv[2]);
	}
	cout << "cv version " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << ", CV_VERSION=" << CV_VERSION << endl;
	cout << "w=" << w << ",h=" << h << endl;
	Mat I1(h, w, CV_32FC3);
	Mat I2(h, w, CV_32FC3);

	randu(I1, Scalar(-100), Scalar(100));
	randu(I2, Scalar(-100), Scalar(100));

	boost::posix_time::ptime t0 = POSIX_LOCAL_TIME;
	double  a1 = getPSNR(I1, I2, nn);
	uint32_t t1 = ngv::timeIntervalMillisec(t0);

	int num_devices = cv::cuda::getCudaEnabledDeviceCount();
	if (num_devices < 1) {
		std::cout << "at least one GPU is required\n";
		return -1;
	}
	for (int i = 0; i < num_devices; ++i) {
		cv::cuda::printShortCudaDeviceInfo(i);
		cv::cuda::setDevice(i);

		cv::cuda::DeviceInfo dev_info(i);
		if (!dev_info.isCompatible()) {
			std::cout << "CUDA module isn't built for GPU #" << i << " ("
				<< dev_info.name() << ", CC " << dev_info.majorVersion()
				<< dev_info.minorVersion() << "\n";
			return -1;
		}

		cout << "for device:" << i << endl;
		t0 = POSIX_LOCAL_TIME;
		double  a2 = getPSNR_CUDA(I1, I2, nn);
		uint32_t t2 = ngv::timeIntervalMillisec(t0);

		cout << "t1=" << t1 << ", t2=" << t2 << endl;
		cout << "a1=" << a1 << ", a2=" << a2 << endl;
	}

	return 0;
}

double getPSNR(const Mat& I1, const Mat& I2, int n)
{
	Mat s1;
	double ret = 0;
	for (int i = 0; i < n; ++i) {
		cv::absdiff(I1, I2, s1);       // |I1 - I2|
		s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
		s1 = s1.mul(s1);           // |I1 - I2|^2
		Scalar s = cv::sum(s1);         // sum elements per channel
		double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
		if (sse > 1e-10) // for small values return zero
		{
			double  mse = sse / (double)(I1.channels() * I1.total());
			ret += 10.0*log10((255 * 255) / mse);
		}
	}
	return ret / n;
}
double getPSNR_CUDA(const Mat& I1, const Mat& I2, int n)
{
	cuda::GpuMat gI1, gI2, gs;
	double ret = 0;
	gI1.upload(I1);
	gI2.upload(I2);

	for (int i = 0; i < n; ++i) {
		cuda::absdiff(gI1.reshape(1), gI2.reshape(1), gs);
		cuda::multiply(gs, gs, gs);
		Scalar s = cuda::sum(gs);
		double sse = s.val[0] + s.val[1] + s.val[2];
		if (sse > 1e-10) // for small values return zero
		{
			double  mse = sse / (double)(gI1.channels() * I1.total());
			ret += 10.0*log10((255 * 255) / mse);
		}
	}
	gI1.release();
	gI2.release();
	gs.release();
	return ret / n;
}



static void help()
{
    cout << "This program demonstrates line finding with the Hough transform." << endl;
    cout << "Usage:" << endl;
    cout << "./gpu-example-houghlines <image_name>, Default is ../data/pic1.png\n" << endl;
}

int test_hough(int argc, char**argv)
{
    const string filename = argc >= 2 ? argv[1] : "../data/pic1.png";

    Mat src = imread(filename, IMREAD_GRAYSCALE);
    if (src.empty())
    {
        help();
        cout << "can not open " << filename << endl;
        return -1;
    }

    Mat mask;
    cv::Canny(src, mask, 100, 200, 3);

    Mat dst_cpu;
    cv::cvtColor(mask, dst_cpu, COLOR_GRAY2BGR);
    Mat dst_gpu = dst_cpu.clone();

    vector<Vec4i> lines_cpu;
    {
        const int64 start = getTickCount();

        cv::HoughLinesP(mask, lines_cpu, 1, CV_PI / 180, 50, 60, 5);

        const double timeSec = (getTickCount() - start) / getTickFrequency();
        cout << "CPU Time : " << timeSec * 1000 << " ms" << endl;
        cout << "CPU Found : " << lines_cpu.size() << endl;
    }

    for (size_t i = 0; i < lines_cpu.size(); ++i)
    {
        Vec4i l = lines_cpu[i];
        line(dst_cpu, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
    }

    GpuMat d_src(mask);
    GpuMat d_lines;
    {
        const int64 start = getTickCount();

        Ptr<cuda::HoughSegmentDetector> hough = cuda::createHoughSegmentDetector(1.0f, (float) (CV_PI / 180.0f), 50, 5);

        hough->detect(d_src, d_lines);

        const double timeSec = (getTickCount() - start) / getTickFrequency();
        cout << "GPU Time : " << timeSec * 1000 << " ms" << endl;
        cout << "GPU Found : " << d_lines.cols << endl;
    }
    vector<Vec4i> lines_gpu;
    if (!d_lines.empty())
    {
        lines_gpu.resize(d_lines.cols);
        Mat h_lines(1, d_lines.cols, CV_32SC4, &lines_gpu[0]);
        d_lines.download(h_lines);
    }

    for (size_t i = 0; i < lines_gpu.size(); ++i)
    {
        Vec4i l = lines_gpu[i];
        line(dst_gpu, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
    }

    imshow("source", src);
    imshow("detected lines [CPU]", dst_cpu);
    imshow("detected lines [GPU]", dst_gpu);
    waitKey();

    return 0;
}
#endif
