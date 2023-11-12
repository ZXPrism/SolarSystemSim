#pragma once

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CelestialBody;

class SolarSystem
{
public:
    template <typename CelestialBodyType>
    CelestialBodyType *addCelestrialBody()
    {
        auto obj = new CelestialBodyType();
        m_objects.push_back(obj);
        return obj;
    }

    void init();

    void tick(float delta_time);
    void logicTick(float delta_time);
    void renderTick();

private:
    std::vector<CelestialBody *> m_objects;
    std::size_t m_vertex_num;
    std::size_t m_orbit_vertex_num;
    glm::mat4 m_projection;

    GLuint m_VAO, m_VBO;
    GLuint m_orbit_VAO, m_orbit_VBO;
};
