#pragma once

#include "CelestialBody.h"

class Terrestrial;

class Satellite : public CelestialBody
{
public:
    Satellite();

    virtual void tick(float delta_time) override;
    virtual void setOrbitModelMatrix() override;

public:
    Terrestrial *m_terrestrial = nullptr;
};
