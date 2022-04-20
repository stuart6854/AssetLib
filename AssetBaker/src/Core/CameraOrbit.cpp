#include "CameraOrbit.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

CameraOrbit::CameraOrbit(float fieldOfViewDegrees, float aspectRatio, float near, float far) : _fieldOfViewDegrees(fieldOfViewDegrees), _aspectRatio(aspectRatio), _near(near), _far(far)
{
    UpdateMatrices();
}

void CameraOrbit::RotateAzimuth(const float radians)
{
    _azimuthAngle += radians;
    // Keep azimuth angle within range <0..2PI) - it's not necessary, just to have it nicely output
    constexpr auto fullCircle = 2.0f * glm::pi<float>();
    _azimuthAngle = fmodf(_azimuthAngle, fullCircle);
    if (_azimuthAngle < 0.0f)
    {
        _azimuthAngle = fullCircle + _azimuthAngle;
    }

    UpdateMatrices();
}

void CameraOrbit::RotatePolar(const float radians)
{
    _polarAngle += radians;

    // Check if the angle hasn't exceeded quarter of a circle to prevent flip, add a bit of epsilon like 0.001 radians
    constexpr auto polarCap = glm::pi<float>() / 2.0f - 0.001f;
    if (_polarAngle > polarCap)
    {
        _polarAngle = polarCap;
    }

    if (_polarAngle < -polarCap)
    {
        _polarAngle = -polarCap;
    }

    UpdateMatrices();
}

void CameraOrbit::Zoom(const float by)
{
    _radius -= by;
    if (_radius < _minRadius) {
        _radius = _minRadius;
    }

    UpdateMatrices();
}

glm::vec3 CameraOrbit::GetEye() const
{
    // Calculate sines / cosines of angles
    const auto sineAzimuth = sin(_azimuthAngle);
    const auto cosineAzimuth = cos(_azimuthAngle);
    const auto sinePolar = sin(_polarAngle);
    const auto cosinePolar = cos(_polarAngle);

    // Calculate eye position out of them
    const auto x = _centre.x + _radius * cosinePolar * cosineAzimuth;
    const auto y = _centre.y + _radius * sinePolar;
    const auto z = _centre.z + _radius * cosinePolar * sineAzimuth;

    return { x, y, z };
}

void CameraOrbit::UpdateMatrices()
{
    const auto eye = GetEye();
    _view = glm::lookAt(GetEye(), _centre, _up);
    _proj = glm::perspective(glm::radians(_fieldOfViewDegrees), _aspectRatio, _near, _far);

    _projView = _proj * _view;
}
