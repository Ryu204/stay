#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../type/vector.hpp"
#include "../type/serializable.hpp"

namespace stay
{
    class Transform : public Serializable
    {
        public:
            using Matrix = glm::mat4x4;

            Transform();
            Transform(const Matrix& matrix);
            Transform(const Vector3& position, float rotation = 0.F, const Vector3& scale = Vector3(1.F, 1.F, 1.F));
            bool operator == (const Transform& right);
            Transform& operator = (const Transform& right);
            // `matrix` transform's rotation must be only around OZ
            void setMatrix(const Matrix& matrix);
            const Matrix& getMatrix() const;
            const Matrix& getInverseMatrix() const;

            void move(const Vector2& offset);
            void move(const Vector3& offset);
            void setPosition(const Vector2& pos);
            void setPosition(const Vector3& pos);
            Vector3 getPosition() const;

            // Rotate an angle of `degree` clockwise looking from OZ-
            void rotate(float degree);
            void setRotation(float degree);
            float getRotation() const;
            
            // Scale in local space
            void scale(const Vector3& scale);
            void scale(const Vector2& scale);
            void scaleAddition(const Vector3& add);
            void scaleAddition(const Vector2& add);
            void setScale(const Vector3& scale);
            void setScale(const Vector2& scale);
            Vector3 getScale() const;

            SERIALIZE_POSTPROCESSING(mPosition, mRotation, mScale)
            void postSerialization()
            {
                mMatrixNeedRebuild = true;
                mInverseNeedRebuild = true;
            }
        private:
            // Update other components by decomposing the transform matrix
            // Link: https://stackoverflow.com/questions/17918033/glm-decompose-mat4-into-translation-and-rotation
            void updateFromMatrix();

            void buildMatrix() const;
            void buildInverseMatrix() const;
            
            Vector3 mPosition;
            float mRotation;
            Vector3 mScale;

            mutable bool mMatrixNeedRebuild;
            mutable bool mInverseNeedRebuild;
            mutable Matrix mTRSMatrix;
            mutable Matrix mInverseMatrix;
    };

    Transform operator * (Transform& left, Transform& right);
} // namespace stay
