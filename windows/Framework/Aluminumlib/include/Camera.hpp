#ifndef INCLUDE_AL_CAM2_HPP
#define INCLUDE_AL_CAM2_HPP

#if defined(_WIN32)
#include "Includes.hpp"
#elif defined(__APPLE__)
#include "Aluminum/Includes.hpp"
#endif


namespace aluminum {

    using glm::to_string;
    using glm::vec3;
    using glm::vec4;
    using glm::mat4;


    class Camera {

    public:

        Camera();

        Camera(float fovy, float aspect, float nearPlane, float farPlane); //perspective
        Camera(float fovy, float aspect, float nearPlane, float farPlane, bool isStereo, bool isFrontBack); //perspective

        Camera &frontBack(bool _on);

        Camera &stereo(bool _on);

        Camera &stereo(float _convergence, float _eyeSep);

        Camera &convergence(float _convergence);

        Camera &eyeSep(float _eyeSep);

        float convergence();

        float eyeSep();

        //void setupStereoRight(Matrix4f& P, Matrix4f& V);
        //void setupStereoLeft(Matrix4f& P, Matrix4f& V);

        Camera &perspective(float fovy, float aspect, float nearPlane, float farPlane); //perspective
        Camera &resetVectors();

        Camera &transform();

        Camera &translate(vec3 dists);

        Camera &translateX(float dist);

        Camera &translateY(float dist);

        Camera &translateZ(float dist);

        Camera &rotate(vec3 angles);

        Camera &rotateX(float angle);

        Camera &rotateY(float angle);

        Camera &rotateZ(float angle);

        void printCameraInfo();

        float _convergence = 5.0;
        float _eyeSep = 1.0;
        float fovy, aspect, nearPlane, farPlane;
        mat4 projection, view, backView;
        mat4 rightProjection, leftProjection, rightView, leftView, rightBackView, leftBackView, rightTranslate, leftTranslate;
        vec3 posVec, viewVec, rightVec, upVec;

        bool isTransformed;
        bool isFrontBack;
        bool isStereo;

    private:
        vec3 ArbitraryRotate(vec3 p, float theta, vec3 r);
    };
}
#endif

