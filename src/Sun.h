#pragma once

#include "CelestialBody.h"

class Sun : public CelestialBody
{
public:
    Sun();

    virtual void tick(float delta_time) override;
    virtual void setOrbitModelMatrix() override;

protected:
    
};
