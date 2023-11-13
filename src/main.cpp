#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>

#include "SolarSystem.h"
#include "Terrestrial.h"
#include "Satellite.h"
#include "Sun.h"
#include "Shader.h"

#define WINDOW_TITLE "SolarSystemSim @ZXPrism"
#define PI 3.1415926f

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow *window = glfwCreateWindow(1024, 1024, WINDOW_TITLE, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGL();
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glViewport(0, 0, 1024, 1024);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Create a "SolarSystem": (sounds COOOOOOOL!)
    SolarSystem solar_system;
    solar_system.init();

    // Load shaders
    Shader terrestrial_shader("terrestrial.vshader", "terrestrial.fshader");
    Shader sun_shader("sun.vshader", "sun.fshader");
    Shader orbit_shader("orbit.vshader", "orbit.fshader");

    // Now that we have a world, it's time to add something to it!

#if 1
    // Add the Sun. Long may the sun shine!
    auto sun = solar_system.addCelestrialBody<Sun>();
    sun->m_name = "Sun";
    sun->m_shader = &sun_shader;
    sun->m_orbit_shader = &orbit_shader; // Sun should not have an orbit, though
    sun->m_scale = 5;
    sun->m_angle = 0;
    sun->m_speed = 0;
    sun->m_radius = 0;
#endif

#if 1
    // Add Mercury
    auto mercury = solar_system.addCelestrialBody<Terrestrial>();
    mercury->m_name = "Mercury";
    mercury->m_shader = &terrestrial_shader;
    mercury->m_orbit_shader = &orbit_shader;
    mercury->m_scale = 1.149;
    mercury->m_angle = PI / 12;
    mercury->m_speed = 1.535;
    mercury->m_radius = 0.39;
    mercury->m_color = {0.6f, 0.6f, 0.6f};
#endif

#if 1
    // Add Venus
    auto venus = solar_system.addCelestrialBody<Terrestrial>();
    venus->m_name = "Venus";
    venus->m_shader = &terrestrial_shader;
    venus->m_orbit_shader = &orbit_shader;
    venus->m_scale = 2.847;
    venus->m_angle = PI / 6;
    venus->m_speed = 0.495;
    venus->m_radius = 0.72;
    venus->m_color = {0.87f, 0.72f, 0.53f};
#endif

    // Add our mother Earth
    auto earth = solar_system.addCelestrialBody<Terrestrial>();
    earth->m_name = "Earth";
    earth->m_shader = &terrestrial_shader;
    earth->m_orbit_shader = &orbit_shader;
    earth->m_scale = 3;
    earth->m_angle = 0;
    earth->m_speed = 0.25;
    earth->m_radius = 1;
    earth->m_color = {0.25f, 0.5f, 1.0f};

    // Add the moon
    auto moon = solar_system.addCelestrialBody<Satellite>();
    moon->m_name = "Moon";
    moon->m_shader = &terrestrial_shader;
    moon->m_orbit_shader = &orbit_shader;
    moon->m_scale = 0.8;
    moon->m_angle = 0;
    moon->m_speed = 5;
    moon->m_radius = 0.2;
    moon->m_color = {0.6f, 0.6f, 0.6f};
    moon->m_terrestrial = earth;

    // Start main loop
    std::chrono::time_point<std::chrono::steady_clock> prev, now;
    float delta_time, delta_time_avg = 0.016f;
    char title_buf[64];
    while (!glfwWindowShouldClose(window))
    {
        now = std::chrono::steady_clock::now();
        delta_time = std::chrono::duration_cast<std::chrono::microseconds>(now - prev).count() / 1000000.0f;
        delta_time_avg = delta_time_avg * 0.8f + delta_time * 0.2f;
        // Using a terminology from signal-processing - this is called "filtering"
        // Through filtering, we can avoid (possible) massive response due to the noise
        prev = now;
        std::sprintf(title_buf, WINDOW_TITLE " [FPS: %.0f]", 1.0f / delta_time_avg);
        glfwSetWindowTitle(window, title_buf);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the states of those celestial bodies
        // and then render them in the back buffer
        solar_system.tick(delta_time_avg);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return 0;
}
