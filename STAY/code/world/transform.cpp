#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "../type/vector.hpp"
#include "../utility/typedef.hpp"
#include "transform.hpp"

namespace stay
{
    Transform::Transform()
        : mTRSMatrix(Matrix(1.F))
    {
        updateFromMatrix();
    }

    Transform::Transform(const Matrix& matrix)
    {
        setMatrix(matrix);
    }

    void Transform::setMatrix(const Matrix& matrix)
    {
        mTRSMatrix = matrix;
        updateFromMatrix();
    }

    const Transform::Matrix& Transform::getMatrix() const
    {
        return mTRSMatrix;
    }

    Transform::Matrix Transform::getInverseMatrix() const
    {
        return glm::inverse(mTRSMatrix);
    }

    // NOLINTBEGIN(*-unnecessary-value-param)   
    void Transform::move(Vector2 offset)
    {
        move(Vector3(offset, 0.F));
    }

    void Transform::move(Vector3 offset)
    {
        mTRSMatrix = glm::translate(mTRSMatrix, offset);
        mPosition = mPosition + offset;
    }
    
    void Transform::setPosition(Vector2 pos)
    {
        setPosition(Vector3(pos, mPosition.z));
    }
    
    void Transform::setPosition(Vector3 pos)
    {
        mTRSMatrix = glm::translate(mTRSMatrix, pos - mPosition);
        mPosition = pos;
    }
    
    Vector3 Transform::getPosition() const
    {
        return mPosition;
    }

    void Transform::rotate(float degree)
    {
        rotate(degree, mPosition);
    }

    float Transform::getRotation() const
    {
        return glm::eulerAngles(mRotation).z * RAD2DEG;
    }

    void Transform::setRotation(float degree)
    {
        float delta = degree - getRotation();
        rotate(delta);
    }

    void Transform::rotate(float degree, Vector2 pole)
    {
        move(-pole);
        rotateAroundAxis(degree, vectorForward);
        move(pole);
    }

    void Transform::scale(Vector3 scale, Vector2 pole)
    {
        move(-pole);
        mScale.x *= scale.x;
        mScale.y *= scale.y;
        mScale.z *= scale.z;
        mTRSMatrix = glm::scale(mTRSMatrix, scale);
        move(pole);
    }

    void Transform::scale(Vector3 scale)
    {
        this->scale(scale, mPosition);
    }

    void Transform::setScale(Vector3 scale)
    {
        Vector3 delta(scale.x / mScale.x, scale.y / mScale.y, scale.z / mScale.z);
        this->scale(delta);
    }

    Vector3 Transform::getScale() const
    {
        return mScale;
    }
    // NOLINTEND(*-unnecessary-value-param)   

    void Transform::updateFromMatrix()
    {
        glm::vec3 unused1;
        glm::vec4 unused2;
        glm::decompose(mTRSMatrix, mScale, mRotation, mPosition, unused1, unused2);
        mRotation = glm::conjugate(mRotation);
    }

    void Transform::rotateAroundAxis(float degree, Vector3 axis)
    {
        if (axis == Vector3())
            axis = Vector3(0, 0, -1);
        float angle = degree * DEG2RAD;
        mTRSMatrix = glm::rotate(mTRSMatrix, angle, axis);
        mRotation = glm::rotate(mRotation, angle, axis);
    }

    Transform operator * (const Transform& left, const Transform& right)
    {
        return { left.getMatrix() * right.getMatrix() };
    }
} // namespace stay