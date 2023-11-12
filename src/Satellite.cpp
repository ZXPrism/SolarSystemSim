#include "Satellite.h"

#include "Terrestrial.h"
#include "Shader.h"

#include <cmath>

Satellite::Satellite()
{
    m_type = CBType::SATELLITE;
}

void Satellite::tick(float delta_time)
{
    m_angle += delta_time * m_speed;
    if (m_angle > 31.41592653f) // anti saturation
        m_angle -= 31.41592653f;
    m_x = ORBIT_COEF * m_radius * cosf(m_angle) + m_terrestrial->m_x;
    m_z = ORBIT_COEF * m_radius * sinf(m_angle) + m_terrestrial->m_z;
}

void Satellite::setOrbitModelMatrix()
{
    glm::mat4 model_orbit(1.0f);
    model_orbit = glm::translate(model_orbit,
                                 glm::vec3(m_terrestrial->m_x,
                                           m_terrestrial->m_y,
                                           m_terrestrial->m_z));
    model_orbit = glm::scale(model_orbit, m_radius * ORBIT_COEF * glm::vec3(1.0f));
    m_orbit_shader->activate();
    m_orbit_shader->setMat4("model", model_orbit);
}
