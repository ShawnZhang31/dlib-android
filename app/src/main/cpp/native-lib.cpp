#include <android/log.h>
#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

#define LOG_TAG "Native-out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

using namespace std;
using namespace cv;
using namespace dlib;
dlib::frontal_face_detector faceDetector = dlib::get_frontal_face_detector();

extern "C" JNIEXPORT jstring JNICALL Java_com_seventythree_cvdlibdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL Java_com_seventythree_cvdlibdemo_MainActivity_cannyDetect(
        JNIEnv *env,
        jobject thiz,
        jlong matAddr) {

    Mat & grayMat = *(Mat * )matAddr;
    matAddr;
    cv::Canny(grayMat, grayMat, 50, 100);

}

extern "C" JNIEXPORT void JNICALL Java_com_seventythree_cvdlibdemo_MainActivity_faceDetect(
        JNIEnv *env,
        jobject thiz,
        jlong matAddr) {
    cv::Mat &img = *(cv::Mat *) matAddr;
    if (img.channels() ==4)
        cv::cvtColor(img, img, CV_RGBA2BGR);

    cv::Mat rotatedMat = img.clone();
    cv::rotate(rotatedMat, rotatedMat, ROTATE_90_COUNTERCLOCKWISE);

    dlib::cv_image<bgr_pixel> dlibImg(rotatedMat);
//
    std::vector<dlib::rectangle> faceRects = faceDetector(dlibImg);
    if (faceRects.size() > 0) {
        LOGD("检测到%d个人脸", faceRects.size());
        for (int i = 0; i < faceRects.size(); ++i) {
            cv::rectangle(rotatedMat,
                          cv::Point((int)faceRects[i].left(), (int)faceRects[i].top()),
                          cv::Point((int)faceRects[i].right(), (int)faceRects[i].bottom()),
                          cv::Scalar(255, 0, 255),
                          2, LINE_8, 0);
        }
        img = rotatedMat.clone();
        cv::rotate(img, img, ROTATE_90_CLOCKWISE);
    } else
    {
        LOGD("未能检测到人脸");
    }

    cv::cvtColor(img, img, CV_BGR2RGBA);
}


