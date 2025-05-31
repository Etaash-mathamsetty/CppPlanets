#ifndef BODY_H
#define BODY_H

#include <string>
#include "State.hpp"

class Body {
private:
    std::string name;
    State initial;
    State current;
    State* pending;

    double mass;
    double radius;

    static constexpr double dt = 1e-4;
    static constexpr double G = 1e6;

public:
    Body(const std::string& name, const State& initial, double mass, double radius);
    ~Body();

    std::string getName() const;

    double getPot(const Vector3& p, Vector3 offset) const;

    void compute(const Vector3& force);

    void addMass(double a)
    {
        mass += a;
    }

    Vector3 getPos() const;

    Vector3 getVel() const;

    void reset();

    void commit();

    double getRadius() const;

    double getMass() const;
};

#endif
