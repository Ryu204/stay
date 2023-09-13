#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../type/vector.hpp"

namespace stay
{
    class Transform
    {
        public:
            using Matrix = glm::mat4x4;

            Transform();
            Transform(const Matrix& matrix);
            // `matrix` transform's rotation must be only around OZ
            void setMatrix(const Matrix& matrix);
            const Matrix& getMatrix() const;
            Matrix getInverseMatrix() const;

            void move(Vector2 offset);
            void move(Vector3 offset);
            void setPosition(Vector2 pos);
            void setPosition(Vector3 pos);
            Vector3 getPosition() const;

            // Rotate an angle of `degree` in local space
            void rotate(float degree);
            // Rotate an angle `degree` around the axis which is paralell with OZ+ and projects on OXY as `pole`
            void rotate(float degree, Vector2 pole);
            void setRotation(float degree);
            float getRotation() const;
            
            // Scale with respect to `(pole, mPosition.z)` as origin
            void scale(Vector3 scale, Vector2 pole);
            // Scale in local space
            void scale(Vector3 scale);
            void setScale(Vector3 scale);
            Vector3 getScale() const;
        private:
            // Update other components by decomposing the transform matrix
            // Link: https://stackoverflow.com/questions/17918033/glm-decompose-mat4-into-translation-and-rotation
            void updateFromMatrix();
            // Rotate an angle of `degree` around the line between `axis` and world's origin
            void rotateAroundAxis(float degree, Vector3 axis);
            
            Matrix mTRSMatrix;
            Vector3 mPosition;
            glm::quat mRotation;
            Vector3 mScale;
    };

    Transform operator * (const Transform& left, const Transform& right);
} // namespace stay
