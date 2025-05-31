#include "Body.hpp"

Body::Body(const std::string& name, const State& initial, double mass, double radius)
    : name(name), initial(initial), current(initial.clone()), pending(nullptr),
      mass(mass), radius(radius) {}

Body::~Body() {
    delete pending;
}

std::string Body::getName() const {
    return name;
}

double Body::getPot(const Vector3& p, Vector3 offset) const {
    return G * mass / std::max(p.getDist(current.getPos().add(offset)), radius);
}

void Body::compute(const Vector3& force) {
    Vector3 a = force.div(mass);
    if (pending)
        delete pending;
    pending = new State(current.clone());
    Vector3 updatedVel = pending->getVel().add(a.times(dt));
    pending->setVel(updatedVel);
    Vector3 updatedPos = pending->getPos().add(updatedVel.times(dt));
    pending->setPos(updatedPos);
}

Vector3 Body::getPos() const {
    return current.getPos();
}

Vector3 Body::getVel() const {
    return current.getVel();
}

void Body::reset() {
    current = initial.clone();
}

void Body::commit() {
    if (pending) {
        current = pending->clone();
        delete pending;
        pending = nullptr;
    }
}

double Body::getRadius() const {
    return radius;
}

double Body::getMass() const {
    return mass;
}
