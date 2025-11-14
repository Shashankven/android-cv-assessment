Android OpenCV/OpenGL Grayscale Viewer

This is an Android app that shows a live, real-time grayscale camera feed. It uses C++ and JNI to handle the camera rendering with OpenGL.

It also includes a small TypeScript web page as required.

App Demo
A live, grayscale feed rendered with OpenGL.
![color](https://github.com/user-attachments/assets/612fd454-1bce-4e2b-bc83-a4bfc59a0359)

![gray](https://github.com/user-attachments/assets/45ba2a7b-35c2-460c-8e9c-9f141a7cc08a)


1. Features Implemented

Android App

Live Camera: Uses the Camera2 API to get a live video feed.

OpenGL Rendering: Uses GLSurfaceView to show the camera feed on the screen.

C++ / JNI: The Java code calls C++ code (in native-lib.cpp) on every frame to handle the rendering.

Grayscale Filter: A fast GLSL shader (managed by C++) converts the camera feed to grayscale in real-time.

Web Viewer

A simple index.html page styled with TailwindCSS.

A main.ts (TypeScript) file that updates text (like "FPS" and "Resolution") on the page when it loads.

Version Control

The project was built with Git, and commits were made at each major step (Setup, App, Web, Docs).

2. How It Works (Architecture)

MainActivity.java asks for CAMERA permission and starts the GLSurfaceView.

CameraGLRenderer.java opens the camera and tells the GLSurfaceView to render a new frame whenever one is available.

On every frame, the Java code calls the native C++ function onDrawFrame (in native-lib.cpp).

The C++ code then tells OpenGL to draw the video frame, applying a grayscale shader directly on the GPU. This is very fast and efficient.

3. How to Run This Project

Clone the Repo:

git clone [https://github.com/YOUR_USERNAME/YOUR_REPO.git](https://github.com/YOUR_USERNAME/YOUR_REPO.git)


Open in Android Studio:

Open the project in Android Studio.

Make sure you have the Android SDK and NDK installed (via Tools > SDK Manager).

Link OpenCV:

Download the OpenCV Android SDK (e.g., 4.9.0) from opencv.org.

Unzip it somewhere on your computer (e.g., D:\opencv_sdk).

Open the gradle.properties file in this project.

Change the opencvsdk path to point to your new location:

opencvsdk=D:/opencv_sdk


Run:

Click the Run (▶) button to build and run the app on your emulator or phone.
