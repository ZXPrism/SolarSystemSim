#pragma once

#include "CelestialBody.h"

class Terrestrial : public CelestialBody
{
public:
    Terrestrial();

    virtual void tick(float delta_time) override;
    virtual void setOrbitModelMatrix() override;

protected:
    
};
