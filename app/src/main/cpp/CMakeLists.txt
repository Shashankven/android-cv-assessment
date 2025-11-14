cmake_minimum_required(VERSION 3.22.1)
project("myapplication")

# THIS IS THE FIX: Find the OpenGL ES 2 library
find_library(gles-lib GLESv2)

# This points to your OpenCV include headers
include_directories(${OpenCV_DIR}/include)

add_library(myapplication SHARED
        native-lib.cpp)

# This finds the Android logging library
find_library(log-lib log)

# This adds the pre-built OpenCV library file
add_library(lib_opencv SHARED IMPORTED)
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION
        ${OpenCV_DIR}/../libs/${ANDROID_ABI}/libopencv_java4.so)
# (If 4.9.0, this might be libopencv_java490.so. Let's try 4 first.)


target_link_libraries(
        myapplication
        lib_opencv   # Link OpenCV
        ${gles-lib}  # <-- THIS IS THE NEW LINE TO LINK OPENGL
        ${log-lib} )