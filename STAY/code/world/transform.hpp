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
            const Matrix& getMatrix();
            const Matrix& getInverseMatrix();

            void move(Vector2 offset);
            void move(Vector3 offset);
            void setPosition(Vector2 pos);
            void setPosition(Vector3 pos);
            Vector3 getPosition() const;

            // Rotate an angle of `degree` clockwise looking from OZ-
            void rotate(float degree);
            void setRotation(float degree);
            float getRotation() const;
            
            // Scale in local space
            void scale(Vector3 scale);
            void scale(Vector2 scale);
            void scaleAddition(Vector3 add);
            void scaleAddition(Vector2 add);
            void setScale(Vector3 scale);
            void setScale(Vector2 scale);
            Vector3 getScale() const;
        private:
            // Update other components by decomposing the transform matrix
            // Link: https://stackoverflow.com/questions/17918033/glm-decompose-mat4-into-translation-and-rotation
            void updateFromMatrix();

            void buildMatrix();
            void buildInverseMatrix();
            
            Vector3 mPosition;
            float mRotation;
            Vector3 mScale;

            bool mMatrixNeedRebuild;
            bool mInverseNeedRebuild;
            Matrix mTRSMatrix;
            Matrix mInverseMatrix;
    };

    Transform operator * (const Transform& left, const Transform& right);
} // namespace stay
