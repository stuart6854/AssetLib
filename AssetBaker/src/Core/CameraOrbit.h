#pragma once

#include <glm/mat4x4.hpp>

class CameraOrbit
{
public:
    CameraOrbit(float fieldOfViewDegrees, float aspectRatio, float near = 0.1f, float far = 1000.0f);

    auto GetProjView() const -> const glm::mat4& { return _projView; }

    void RotateAzimuth(const float radians);
    void RotatePolar(const float radians);
    void Zoom(const float by);

    glm::vec3 GetEye() const;

private:
    void UpdateMatrices();

private:
    float _fieldOfViewDegrees;
    float _aspectRatio;
    float _near;
    float _far;

    glm::vec3 _centre = { 0.0f, 0.0f, 0.0f }; // Centre of the orbit camera sphere (the point upon which the camera looks)
    glm::vec3 _up = { 0.0f, 1.0f, 0.0f }; // Up vector of the camera
    float _radius = 5.0f; // Radius of the orbit camera sphere
    float _minRadius = 0.1f; // Minimal radius of the orbit camera sphere (cannot fall below this value)
    float _azimuthAngle = 0.0f; // Azimuth angle on the orbit camera sphere
    float _polarAngle = 0.0f; // Polar angle on the orbit camera sphere

    glm::mat4 _view = glm::mat4(1.0f);
    glm::mat4 _proj = glm::mat4(1.0f);

    glm::mat4 _projView;
};
