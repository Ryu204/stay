#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "../type/vector.hpp"
#include "../utility/typedef.hpp"
#include "transform.hpp"

namespace stay
{
    Transform::Transform()
        : mTRSMatrix(glm::mat4(1.F))
        , mMatrixNeedRebuild(false)
        , mInverseNeedRebuild(true)
    {
        updateFromMatrix();
    }

    Transform::Transform(const Matrix& matrix)
        : mTRSMatrix(matrix)
        , mMatrixNeedRebuild(false)
        , mInverseNeedRebuild(true)
    {
        updateFromMatrix();
    }

    Transform::Transform(const Vector3& position, float rotation, const Vector3& scale) // NOLINT(*-pass-by-value)
        : mPosition(position)
        , mRotation(rotation)
        , mScale(scale)
        , mMatrixNeedRebuild(true)
        , mInverseNeedRebuild(true)
    { }

    void Transform::setMatrix(const Matrix& matrix)
    {
        mTRSMatrix = matrix;
        updateFromMatrix();
        mInverseNeedRebuild = true;
        mMatrixNeedRebuild = false;
    }

    const Transform::Matrix& Transform::getMatrix() const
    {
        if (mMatrixNeedRebuild)
        {
            buildMatrix();
            mMatrixNeedRebuild = false;
        }
        return mTRSMatrix;
    }

    const Transform::Matrix& Transform::getInverseMatrix() const
    {
        if (mMatrixNeedRebuild)
        {
            buildMatrix();
            mMatrixNeedRebuild = false;
        }
        if (mInverseNeedRebuild)
        {
            buildInverseMatrix();
            mInverseNeedRebuild = false;
        }
        return mInverseMatrix;
    }

    void Transform::move(const Vector2& offset)
    {
        move(Vector3(offset, 0.F));
    }

    void Transform::move(const Vector3& offset)
    {
        mPosition = mPosition + offset;

        mMatrixNeedRebuild = true;
        mInverseNeedRebuild = true;
    }
    
    void Transform::setPosition(const Vector2& pos)
    {
        setPosition(Vector3(pos, mPosition.z));
    }
    
    void Transform::setPosition(const Vector3& pos)
    {
        mPosition = pos;

        mMatrixNeedRebuild = true;
        mInverseNeedRebuild = true;
    }
    
    Vector3 Transform::getPosition() const
    {
        return mPosition;
    }

    void Transform::rotate(float degree)
    {
        mRotation += degree;

        mMatrixNeedRebuild = true;
        mInverseNeedRebuild = true;
    }

    float Transform::getRotation() const
    {
        return mRotation;
    }

    void Transform::setRotation(float degree)
    {
        rotate(degree - mRotation);
    }

    void Transform::scale(const Vector3& scale)
    {
        mScale.x *= scale.x;
        mScale.y *= scale.y;
        mScale.z *= scale.z;

        mMatrixNeedRebuild = true;
        mInverseNeedRebuild = true;
    }

    void Transform::scale(const Vector2& scale)
    {
        this->scale(Vector3(scale, 1.F));
    }

    void Transform::scaleAddition(const Vector3& add)
    {
        mScale = mScale + add;

        mMatrixNeedRebuild = true;
        mInverseNeedRebuild = true;
    }

    void Transform::scaleAddition(const Vector2& add)
    {
        scaleAddition(Vector3(add, 0.F));
    }

    void Transform::setScale(const Vector3& scale)
    {
        const Vector3 delta(scale.x / mScale.x, scale.y / mScale.y, scale.z / mScale.z);
        this->scale(delta);
    }

    void Transform::setScale(const Vector2& scale)
    {
        setScale(Vector3(scale, mScale.z));
    }

    Vector3 Transform::getScale() const
    {
        return mScale;
    }

    void Transform::updateFromMatrix()
    {
        glm::vec3 unused1{};
        glm::vec4 unused2{};
        glm::quat rotation{};
        glm::decompose(mTRSMatrix, mScale, rotation, mPosition, unused1, unused2);
        
        mRotation = glm::yaw(rotation) * RAD2DEG;
        // This line is for glm version <= 0.9.7
        // mRotation = glm::eulerAngles(glm::conjugate(rotation)).z * RAD2DEG;
    }

    void Transform::buildMatrix() const
    {
        mTRSMatrix = glm::mat4(1.F);
        mTRSMatrix = glm::translate(mTRSMatrix, mPosition);
        mTRSMatrix = glm::rotate(mTRSMatrix, mRotation * DEG2RAD, vectorForward);
        mTRSMatrix = glm::scale(mTRSMatrix, mScale);
    }

    void Transform::buildInverseMatrix() const
    {
        mInverseMatrix = glm::inverse(mTRSMatrix);
    }

    Transform operator * (Transform& left, Transform& right)
    {
        return { left.getMatrix() * right.getMatrix() };
    }

    bool Transform::operator == (const Transform& right)
    {
        return (mPosition == right.mPosition && mRotation == right.mRotation && mScale == right.mScale);
    }

    Transform& Transform::operator = (const Transform& right)
    {
        mRotation = right.mRotation;
        mPosition = right.mPosition;
        mScale = right.mScale;
        mMatrixNeedRebuild = true;
        mInverseNeedRebuild = true;
        return *this;
    }
} // namespace stay