#ifndef STATE_H
#define STATE_H

#include "Vector3.hpp"

class State {
private:
    Vector3 position;
    Vector3 velocity;

public:
    State(const Vector3& p, const Vector3& v);

    const Vector3& getPos() const;
    void setPos(const Vector3& p);

    const Vector3& getVel() const;
    void setVel(const Vector3& v);
};

#endif
