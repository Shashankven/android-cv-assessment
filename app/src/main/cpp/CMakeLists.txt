# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# --- START OF OPENCV CONFIG ---

# 1. This fixes your "file not found" error by telling C++
#    where to find the OpenCV header files (like <opencv2/core.hpp>)
include_directories(${OpenCV_DIR}/include)

# --- END OF OPENCV CONFIG ---


# Declares the project name.
project("myapplication")

# Creates and names a library, sets it as SHARED,
# and provides the relative paths to its source code.
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp)


# --- START OF OPENCV CONFIG ---

# 2. This finds and imports the pre-compiled OpenCV .so library file
#    from the SDK folder.
add_library(lib_opencv SHARED IMPORTED)
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION
${OpenCV_DIR}/../libs/${ANDROID_ABI}/libopencv_java4.so)
# (NOTE: If your version is 4.9.0, the file might be libopencv_java490.so.
#  Check your D:/DEV_android_cv/OpenCV-android-sdk/sdk/native/libs folder
#  to be sure)

# --- END OF OPENCV CONFIG ---


# Searches for a specified prebuilt library and stores the path
# as a variable.
find_library(log-lib log)


# Specifies libraries CMake should link to your target library.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library

        # 3. This links your "myapplication" library against the
        #    OpenCV library we just imported.
        lib_opencv

        android
        log)