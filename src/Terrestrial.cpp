#include "Terrestrial.h"

#include "Shader.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Terrestrial::Terrestrial()
{
    m_type = CBType::TERRESTRIAL;
}

void Terrestrial::tick(float delta_time)
{
    m_angle += delta_time * m_speed;
    if (m_angle > 31.41592653f) // anti saturation
        m_angle -= 31.41592653f;
    m_x = ORBIT_COEF * m_radius * cosf(m_angle);
    m_z = ORBIT_COEF * m_radius * sinf(m_angle);
}

void Terrestrial::setOrbitModelMatrix()
{
    glm::mat4 model_orbit(1.0f);
    model_orbit = glm::scale(model_orbit, m_radius * ORBIT_COEF * glm::vec3(1.0f));
    m_orbit_shader->activate();
    m_orbit_shader->setMat4("model", model_orbit);
}
