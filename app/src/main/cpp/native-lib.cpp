#include <jni.h>
#include <string>
#include <opencv2/core.hpp>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    // This is the new code to get the OpenCV version
    std::string version = cv::getVersionString();
    return env->NewStringUTF(version.c_str());
}