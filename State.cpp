#include "State.hpp"

State::State(const Vector3& p, const Vector3& v) : position(p), velocity(v) {}

const Vector3& State::getPos() const { return position; }

void State::setPos(const Vector3& p) { position = p; }

const Vector3& State::getVel() const { return velocity; }

void State::setVel(const Vector3& v) { velocity = v; }

