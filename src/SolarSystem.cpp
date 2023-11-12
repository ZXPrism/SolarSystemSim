#include "SolarSystem.h"

#include "CelestialBody.h"
#include "Shader.h"

#include <vector>
#include <cmath>
#include <stdio.h>

void SolarSystem::init()
{
    // 1. Generate the vertices of a sphere with given parameters:
    constexpr float PI = 3.1415926f;
    constexpr int latitude_density = 256;
    constexpr int longtitude_density = 256;
    std::vector<GLfloat> vertices;

    // helper function
    auto pushVertex = [&](GLfloat x, GLfloat y, GLfloat z)
    {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    };

    // pre-allocate sufficient memory to avoid future allocation & move cost
    vertices.reserve(2 * latitude_density * longtitude_density * 12 * 3);

    float y, y1;
    for (int i = 0; i < latitude_density; i++)
    {
        y = sin(i * 2 * PI / latitude_density);
        y1 = sin((i + 1) * 2 * PI / latitude_density);
        float radius = sqrtf(1 - y * y);
        float radius1 = sqrtf(1 - y1 * y1);
        float angle = 0, angle1;
        for (int j = 0; j < longtitude_density; j++)
        {
            angle1 = angle + 2 * PI / longtitude_density;

            float coords[8] =
                {radius * cosf(angle), radius * sinf(angle),
                 radius * cosf(angle1), radius * sinf(angle1),
                 radius1 * cosf(angle), radius1 * sinf(angle),
                 radius1 * cosf(angle1), radius1 * sinf(angle1)};

            // triangle #1 & normal #1
            pushVertex(coords[0], y, coords[1]);
            pushVertex(coords[2] + coords[4], y + y1, coords[3] + coords[5]);
            pushVertex(coords[2], y, coords[3]);
            pushVertex(coords[2] + coords[4], y + y1, coords[3] + coords[5]);
            pushVertex(coords[4], y1, coords[5]);
            pushVertex(coords[2] + coords[4], y + y1, coords[3] + coords[5]);

            // triangle #2 & normal #2
            // normal #2 is the same as normal #1, for convenience
            // PS: these two normals are equivalent when the "density" is big enough
            pushVertex(coords[2], y, coords[3]);
            pushVertex(coords[2] + coords[4], y + y1, coords[3] + coords[5]);
            pushVertex(coords[6], y1, coords[7]);
            pushVertex(coords[2] + coords[4], y + y1, coords[3] + coords[5]);
            pushVertex(coords[4], y1, coords[5]);
            pushVertex(coords[2] + coords[4], y + y1, coords[3] + coords[5]);

            angle = angle1;
        }
        y = y1;
    }
    m_vertex_num = vertices.size();
    for (int i = 0; i < m_vertex_num; i++)
    {
        if (i % 3 == 1)
            vertices.push_back(-vertices[i]);
        else
            vertices.push_back(vertices[i]);
    }
    m_vertex_num = vertices.size() / 6;

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(float *)));
    glEnableVertexAttribArray(1);

    // 2. Generate the vertices of a circle with given parameters:
    constexpr int density = 512;
    std::vector<GLfloat> orbit_vertices;
    float angle = 0.0f;
    for (int i = 0; i <= density; i++)
    {
        orbit_vertices.push_back(cosf(angle));
        orbit_vertices.push_back(0.0f);
        orbit_vertices.push_back(sinf(angle));
        angle += 2 * PI / density;
    }
    m_orbit_vertex_num = orbit_vertices.size() / 3;

    glGenBuffers(1, &m_orbit_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_orbit_VBO);
    glBufferData(GL_ARRAY_BUFFER, orbit_vertices.size() * sizeof(GLfloat), &orbit_vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_orbit_VAO);
    glBindVertexArray(m_orbit_VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    m_projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f);
}

void SolarSystem::tick(float delta_time)
{
    logicTick(delta_time);
    renderTick();
}

void SolarSystem::logicTick(float delta_time)
{
    for (auto obj : m_objects)
        obj->tick(delta_time);
}

void SolarSystem::renderTick()
{
    static float rnd = 0;
    glm::mat4 view(1.0f);
    view = glm::lookAt(glm::vec3(60, 40, 60), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    for (auto obj : m_objects)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(obj->m_x, obj->m_y, obj->m_z));
        model = glm::scale(model, obj->m_scale * glm::vec3(1.0f));
        obj->m_shader->activate();
        obj->m_shader->setFloat1("rnd", rnd);
        obj->m_shader->setMat4("model", model);
        obj->m_shader->setMat4("view", view);
        obj->m_shader->setMat4("proj", m_projection);
        obj->m_shader->setVec3("color", obj->m_color);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_vertex_num);

        obj->setOrbitModelMatrix();
        obj->m_orbit_shader->setMat4("view", view);
        obj->m_orbit_shader->setMat4("proj", m_projection);
        glBindVertexArray(m_orbit_VAO);
        glDrawArrays(GL_LINE_STRIP, 0, m_orbit_vertex_num);
    }
    rnd += 0.01f;
}
