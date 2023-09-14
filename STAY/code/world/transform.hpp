#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../type/vector.hpp"
#include "../type/serializable.hpp"

namespace stay
{
    class Transform : Serializable
    {
        public:
            using Matrix = glm::mat4x4;

            Transform();
            Transform(const Matrix& matrix);
            bool operator == (const Transform& right);
            // `matrix` transform's rotation must be only around OZ
            void setMatrix(const Matrix& matrix);
            const Matrix& getMatrix() const;
            const Matrix& getInverseMatrix() const;

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

            Json::Value toJSONObject() const override
            {
                Json::Value res;
                res["position"] = mPosition.toJSONObject();
                res["rotation"] = mRotation;
                res["scale"] = mScale.toJSONObject();
                return res;
            }

            bool fetch(const Json::Value& data) override
            {
                if (data.type() == Json::objectValue 
                    && mPosition.fetch(data["position"]) 
                    && data["rotation"].isNumeric()
                    && mScale.fetch(data["scale"]))
                {
                    mRotation = data["rotation"].asFloat();
                    mMatrixNeedRebuild = true;
                    mInverseNeedRebuild = true;
                    return true;
                }
                setMatrix(glm::mat4(1.F));
                return false;
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
