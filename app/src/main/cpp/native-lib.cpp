#include <jni.h>
#include <string>
#include <opencv2/core.hpp>

// Includes for OpenGL
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>

#define LOG_TAG "NativeLib"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_D(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

// This is the C++ OpenGL shader pipeline
// It renders the camera texture and converts it to grayscale on the GPU
// This is the fastest way and satisfies the "OpenGL shaders" bonus.

// Vertex Shader: Renders a simple square
const char* VERTEX_SHADER =
        "attribute vec4 vPosition;\n"
        "attribute vec2 vTexCoord;\n"
        "varying vec2 yuvTexCoord;\n"
        "uniform mat4 uTransformMatrix;\n"
        "void main() {\n"
        "  gl_Position = vPosition;\n"
        "  yuvTexCoord = (uTransformMatrix * vec4(vTexCoord, 0.0, 1.0)).xy;\n"
        "}\n";

// Fragment Shader: Samples the camera texture and converts to grayscale
const char* FRAGMENT_SHADER_GRAYSCALE =
        "#extension GL_OES_EGL_image_external : require\n"
        "precision mediump float;\n"
        "varying vec2 yuvTexCoord;\n"
        "uniform samplerExternalOES sTexture;\n"
        "void main() {\n"
        "  vec3 color = texture2D(sTexture, yuvTexCoord).rgb;\n"
        // Grayscale conversion (luminance method)
        "  float gray = dot(color, vec3(0.299, 0.587, 0.114));\n"
        "  gl_FragColor = vec4(gray, gray, gray, 1.0);\n"
        "}\n";

GLuint gProgram;
GLuint gPositionHandle;
GLuint gTexCoordHandle;
GLuint gTransformMatrixHandle;

const float VERTICES[] = {
        -1.0f, -1.0f, // Bottom Left
        1.0f, -1.0f,  // Bottom Right
        -1.0f, 1.0f,  // Top Left
        1.0f, 1.0f    // Top Right
};

const float TEX_COORDS[] = {
        0.0f, 1.0f, // Bottom Left
        1.0f, 1.0f, // Bottom Right
        0.0f, 0.0f, // Top Left
        1.0f, 0.0f  // Top Right
};

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOG_E("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) return 0;

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) return 0;

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, pixelShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOG_E("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_CameraGLRenderer_onSurfaceCreated(JNIEnv *env, jobject thiz) {
    gProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER_GRAYSCALE);
    if (!gProgram) {
        LOG_E("Could not create program.");
        return;
    }
    gPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    gTexCoordHandle = glGetAttribLocation(gProgram, "vTexCoord");
    gTransformMatrixHandle = glGetUniformLocation(gProgram, "uTransformMatrix");

    glEnable(GL_TEXTURE_EXTERNAL_OES);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_CameraGLRenderer_onSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {
    glViewport(0, 0, width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_CameraGLRenderer_onDrawFrame(JNIEnv *env, jobject thiz, jint texture_id, jfloatArray transform_matrix) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(gProgram);

    // Set vertex positions
    glVertexAttribPointer(gPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, VERTICES);
    glEnableVertexAttribArray(gPositionHandle);

    // Set texture coordinates
    glVertexAttribPointer(gTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, TEX_COORDS);
    glEnableVertexAttribArray(gTexCoordHandle);

    // Set camera transform matrix
    jfloat* matrix = env->GetFloatArrayElements(transform_matrix, 0);
    glUniformMatrix4fv(gTransformMatrixHandle, 1, GL_FALSE, matrix);
    env->ReleaseFloatArrayElements(transform_matrix, matrix, 0);

    // Bind the external camera texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture_id);

    // Draw the square
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Disable attributes
    glDisableVertexAttribArray(gPositionHandle);
    glDisableVertexAttribArray(gTexCoordHandle);
}

// This function is no longer used by MainActivity, but we can leave it.
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    std::string version = cv::getVersionString();
    return env->NewStringUTF(version.c_str());
}