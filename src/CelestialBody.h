#pragma once

#include <string>
#include <glm/vec3.hpp>

constexpr int ORBIT_COEF = 30;

enum class CBType
{
    UNDEFINED,
    TERRESTRIAL,
    SUN,
    SATELLITE
};

class Shader;

class CelestialBody
{
public:
    virtual void tick(float delta_time) = 0;
    virtual void setOrbitModelMatrix() = 0;

public:
    // intrinsic attributes
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_z = 0.0f;
    float m_scale = 1.0f;
    glm::vec3 m_color;

    // orbit attributes
    float m_radius = 0.0f;
    float m_speed = 0.0f;
    float m_angle = 0.0f;

    CBType m_type = CBType::UNDEFINED;

    Shader *m_shader = nullptr;
    Shader *m_orbit_shader = nullptr;
    std::string m_name;
};
