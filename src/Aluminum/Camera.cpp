#include "Aluminum/Camera.hpp"

namespace aluminum {

    using glm::to_string;
    using glm::vec3;
    using glm::vec4;
    using glm::mat4;
    using glm::radians;
  

    Camera::Camera() {
      resetVectors();
      isFrontBack = false;
      isStereo = false;
    };



    //Perspective Camera
    Camera::Camera(float _fovy, float _aspect, float _nearPlane, float _farPlane) {
        isStereo = false;
        isFrontBack = false;
        resetVectors();

      
      perspective(_fovy, _aspect, _nearPlane, _farPlane);

    }

    /*
    //Perspective Camera
    Camera::Camera(float _fovy, float _aspect, float _nearPlane, float _farPlane, bool _isStereo, bool _isFrontBack) {
      isStereo = _isStereo;
      isFrontBack = _isFrontBack;

      resetVectors();
      perspective(_fovy, _aspect, _nearPlane, _farPlane);
    }
    */

    Camera& Camera::resetVectors() {
        posVec = vec3(0.0, 0.0, 0.0);
        viewVec = vec3(0.0f, 0.0f, -1.0f);
        rightVec = vec3(1.0, 0.0, 0.0);
        upVec = vec3(0.0, 1.0, 0.0);
        isTransformed = true;
        return *this;
    }

    Camera& Camera::perspective(float _fovy, float _aspect, float _nearPlane, float _farPlane) {
        fovy = _fovy;
        nearPlane = _nearPlane;
        farPlane = _farPlane;
        aspect = _aspect;

        projection = glm::perspective(fovy, aspect, nearPlane, farPlane);


        if (isStereo) {
            stereo(convergence(), eyeSep());
        }

        isTransformed = true;
        return *this;
    }

    Camera& Camera::convergence(float _c) {
        _convergence = _c;
        stereo(convergence(), eyeSep());
        return *this;
    }

    Camera& Camera::eyeSep(float _e) {
        _eyeSep = _e;
        stereo(convergence(), eyeSep());
        return *this;
    }

    float Camera::convergence() {
        return _convergence;
    }

    float Camera::eyeSep() {
        return _eyeSep;
    }


    Camera& Camera::transform() {

        mat4 rM = mat4(
                rightVec.x, upVec.x, viewVec.x, 0,
                rightVec.y, upVec.y, viewVec.y, 0,
                rightVec.z, upVec.z, viewVec.z, 0,
                0,0,0,1);

        mat4 tM = glm::translate( mat4(1.0), vec3(posVec.x, posVec.y, posVec.z) );

        view = rM * tM;

        if (isFrontBack) {
            mat4 rrM = mat4(
                    -rightVec.x, upVec.x, -viewVec.x, 0,
                    -rightVec.y, upVec.y, -viewVec.y, 0,
                    -rightVec.z, upVec.z, -viewVec.z, 0,
                    0,0,0,1);

            backView = rrM * tM;
        }

        if (isStereo) {
            //printf("isStereo = TRUE\n");
            rightView = rightTranslate * view;
            leftView = leftTranslate * view;

            if (isFrontBack) {
                rightBackView = rightTranslate * backView;
                leftBackView = leftTranslate * backView;
            }
        }

        isTransformed = false;

        return *this;
    }

    Camera& Camera::frontBack(bool _on) {
        isFrontBack = _on;
        return *this;
    }

    Camera& Camera::stereo(bool _on) {
        isStereo = _on;
        if (_on) {
            stereo(convergence(), eyeSep());
        }
        return *this;
    }

    /* code adapted from http://quiescentspark.blogspot.com/2011/05/rendering-3d-anaglyph-in-opengl.html */
    /* The practical value of convergence depth is chosen on the basis of the shot being prepared and the type of effect (out of the screen or inside screen) used. Eye separation is typically kept at 1/30th of the convergence distance and objects closer than half the convergence distance are avoided in the scene. */

    Camera& Camera::stereo(float _c, float _e) {

        isStereo = true;

        _convergence = _c;
        _eyeSep = _e;


        float top, bottom, leftL, rightL, leftR, rightR;

     // top     = nearPlane * tan(radians(fovy)/2);
      top     = nearPlane * tan(fovy/2);
      
      bottom  = -top;

      //  float a = aspect * tan(radians(fovy)/2) * convergence();
      float a = aspect * tan(fovy/2) * convergence();

        float b = a - eyeSep()/2;
        float c = a + eyeSep()/2;

        leftR   =  -c * nearPlane/convergence();
        rightR   =   b * nearPlane/convergence();

        leftL   =  -b * nearPlane/convergence();
        rightL   =   c * nearPlane/convergence();

        //rightProjection = Matrix4f::perspective(leftR, rightR, bottom, top, nearPlane, farPlane );
        //rightTranslate = Matrix4f::translate( -eyeSep/2, 0.0f, 0.0f );
        //leftProjection = Matrix4f::perspective(leftL, rightL, bottom, top, nearPlane, farPlane );
        //leftTranslate = Matrix4f::translate( eyeSep/2, 0.0f, 0.0f );
        rightProjection = glm::frustum(leftR, rightR, bottom, top, nearPlane, farPlane );
        rightTranslate = glm::translate(mat4(1.0), vec3(-eyeSep()/2, 0.0f, 0.0f) );
        leftProjection = glm::frustum(leftL, rightL, bottom, top, nearPlane, farPlane );
        leftTranslate = glm::translate(mat4(1.0), vec3(eyeSep()/2, 0.0f, 0.0f) );

        isTransformed = true;

        return *this;
    }

    //Camera& Camera::translate(Vec3f dists) {
    Camera& Camera::translate(vec3 dists) {
        translateX(dists.x);
        translateY(dists.y);
        translateZ(dists.z);
        isTransformed = true;
        return *this;
    }

    Camera& Camera::translateX(float dist) {
        posVec += (rightVec * dist);
        isTransformed = true;
        return *this;
    }

    Camera& Camera::translateY(float dist) {
        posVec += (upVec * dist);
        isTransformed = true;
        return *this;
    }

    Camera& Camera::translateZ(float dist) {
        posVec += (viewVec * dist);
        isTransformed = true;
        return *this;
    }

    Camera& Camera::rotate(vec3 angles) {
        rotateX(angles.x);
        rotateY(angles.y);
        rotateZ(angles.z);
        isTransformed = true;
        return *this;
    }

    Camera& Camera::rotateX (float angle) {
        viewVec = ArbitraryRotate(viewVec, angle, rightVec);
        upVec = glm::cross(viewVec, rightVec);
        upVec *= -1;
        isTransformed = true;
        return *this;
    }

    Camera& Camera::rotateY (float angle) {
        viewVec = ArbitraryRotate(viewVec, angle, upVec);
        rightVec = glm::cross(viewVec, upVec);
        //  rightVec *= -1;
        isTransformed = true;
        return *this;
    }

    Camera& Camera::rotateZ (float angle) {
        rightVec = ArbitraryRotate(rightVec, angle, viewVec);
        upVec = glm::cross(viewVec, rightVec);
        upVec *= -1.0;
        isTransformed = true;
        return *this;
    }

    //via Paul Bourke
    //theta is in radians...
    vec3 Camera::ArbitraryRotate(vec3 p, float theta, vec3 r) {
        vec3 q = vec3(0.0,0.0,0.0);
        double costheta, sintheta;

        //p = p.normalize();
        //r = r.normalize();
        p = glm::normalize(vec3(p));
        r = glm::normalize(vec3(r));

        //float rad = radians(theta);
        //costheta = cos(rad);
        //sintheta = sin(rad);

        costheta = cos(theta);
        sintheta = sin(theta);

        q.x += (costheta + (1 - costheta) * r.x * r.x) * p.x;
        q.x += ((1 - costheta) * r.x * r.y - r.z * sintheta) * p.y;
        q.x += ((1 - costheta) * r.x * r.z + r.y * sintheta) * p.z;

        q.y += ((1 - costheta) * r.x * r.y + r.z * sintheta) * p.x;
        q.y += (costheta + (1 - costheta) * r.y * r.y) * p.y;
        q.y += ((1 - costheta) * r.y * r.z - r.x * sintheta) * p.z;

        q.z += ((1 - costheta) * r.x * r.z - r.y * sintheta) * p.x;
        q.z += ((1 - costheta) * r.y * r.z + r.x * sintheta) * p.y;
        q.z += (costheta + (1 - costheta) * r.z * r.z) * p.z;

        //q = q.normalize();
        q = glm::normalize(vec3(q));

        return(q);
    }

    void Camera::printCameraInfo() {
        /*
          float _convergence = 5.0;
          float _eyeSep = 1.0;
          float fovy, aspect, nearPlane, farPlane;
          mat4 projection, view, backView;
          mat4 rightProjection, leftProjection, rightView, leftView, rightBackView, leftBackView, rightTranslate, leftTranslate;
          vec3 posVec, viewVec, rightVec, upVec;

          bool isTransformed;
          bool isFrontBack;
          bool isStereo;
       */
        printf("\n===================== Floats =====================\n");
        printf("\t       fov: %f \n",fovy);
        printf("\t nearPlane: %f \n",nearPlane);
        printf("\t  farPlane: %f \n",farPlane);
        printf("\t    aspect: %f \n\n",aspect);

        printf("===================== Vectors =====================\n");
        printf("\t   rtVec: %f, %f, %f \n",rightVec.x,rightVec.y,rightVec.z);
        printf("\t   upVec: %f, %f, %f \n",upVec.x,upVec.y,upVec.z);
        printf("\t  posVec: %f, %f, %f \n",posVec.x,posVec.y,posVec.z);
        printf("\t viewVec: %f, %f, %f \n\n",viewVec.x,viewVec.y,viewVec.z);

        printf("===================== Matrices =====================\n");
        printf("\t      view: %f, %f, %f, %f \n",view[0].x,view[0].y,view[0].z,view[0].a);
        printf("\t            %f, %f, %f, %f \n",view[1].x,view[1].y,view[1].z,view[1].a);
        printf("\t            %f, %f, %f, %f \n",view[2].x,view[2].y,view[2].z,view[2].a);
        printf("\t            %f, %f, %f, %f \n\n",view[3].x,view[3].y,view[3].z,view[3].a);

        printf("\t  backView: %f, %f, %f, %f \n",backView[0].x,backView[0].y,backView[0].z,backView[0].a);
        printf("\t            %f, %f, %f, %f \n",backView[1].x,backView[1].y,backView[1].z,backView[1].a);
        printf("\t            %f, %f, %f, %f \n",backView[2].x,backView[2].y,backView[2].z,backView[2].a);
        printf("\t            %f, %f, %f, %f \n\n",backView[3].x,backView[3].y,backView[3].z,backView[3].a);

        printf("\t  leftView: %f, %f, %f, %f \n",leftView[0].x,leftView[0].y,leftView[0].z,leftView[0].a);
        printf("\t            %f, %f, %f, %f \n",leftView[1].x,leftView[1].y,leftView[1].z,leftView[1].a);
        printf("\t            %f, %f, %f, %f \n",leftView[2].x,leftView[2].y,leftView[2].z,leftView[2].a);
        printf("\t            %f, %f, %f, %f \n\n",leftView[3].x,leftView[3].y,leftView[3].z,leftView[3].a);

        printf("\t rightView: %f, %f, %f, %f \n",rightView[0].x,rightView[0].y,rightView[0].z,rightView[0].a);
        printf("\t            %f, %f, %f, %f \n",rightView[1].x,rightView[1].y,rightView[1].z,rightView[1].a);
        printf("\t            %f, %f, %f, %f \n",rightView[2].x,rightView[2].y,rightView[2].z,rightView[2].a);
        printf("\t            %f, %f, %f, %f \n\n",rightView[3].x,rightView[3].y,rightView[3].z,rightView[3].a);

        printf("\tprojection: %f, %f, %f, %f \n",projection[0].x,projection[0].y,projection[0].z,projection[0].a);
        printf("\t            %f, %f, %f, %f \n",projection[1].x,projection[1].y,projection[1].z,projection[1].a);
        printf("\t            %f, %f, %f, %f \n",projection[2].x,projection[2].y,projection[2].z,projection[2].a);
        printf("\t            %f, %f, %f, %f \n\n",projection[3].x,projection[3].y,projection[3].z,projection[3].a);

    }
};

