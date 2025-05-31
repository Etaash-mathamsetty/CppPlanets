#pragma once
#include <vector>
#include "Body.hpp"
#include "Vector3.hpp"

class World {
private:
    std::vector<Body*> bodies;
    std::vector<Body*> org;

    double compute_dUdv(Body* in, const Vector3& v) {
        double pot = 0.0;
        for (auto b : bodies) {
            if (b != in) {
                pot += b->getPot(in->getPos(), v);
            }
        }

        for (auto b : bodies) {
            if (b != in) {
                pot -= b->getPot(in->getPos(), Vector3());
            }
        }

        /*
        std::cout << "v" << std::endl;
        v.print();
        std::cout << "pot diff " << pot << std::endl;
        */

        return pot / v.norm();
    }

    Vector3 getForce(Body* in) {
        double U_x = compute_dUdv(in, Vector3(0.01, 0, 0));
        double U_y = compute_dUdv(in, Vector3(0, 0.01, 0));
        double U_z = compute_dUdv(in, Vector3(0, 0, 0.01));
        return Vector3(U_x, U_y, U_z).negate();
    }

    void compute() {
        for (Body* b : bodies) {
            Vector3 f = getForce(b);
            //std::cout << "force: " << std::endl;
            f.print();
            b->compute(f);
        }
    }

    void commit() {
        for (Body* b : bodies) {
            b->commit();
        }
    }


public:
    World() = default;

    void addBody(Body* b) {
        bodies.push_back(b);
        org.push_back(b);
    }

    const std::vector<Body *>& getBodies() {
        return bodies;
    }

    void reset() {
        bodies = org;
        for (auto b : bodies) {
            b->reset();
        }
    }

    void tick() {
        compute();
        commit();

        try_again:
        for (auto it = bodies.begin(); it != bodies.end(); it++)
        {
            for (auto jt = bodies.begin(); jt != bodies.end(); jt++)
            {
                if (*it == *jt) continue;

                if ((*it)->getPos().getDist((*jt)->getPos()) < (*it)->getRadius() + (*jt)->getRadius())
                {
                    std::cout << (*it)->getName() << " colided with " << (*jt)->getName() << std::endl;
                    if ((*jt)->getMass() < (*it)->getMass())
                    {
                        (*it)->addMass((*jt)->getMass());
                        jt = bodies.erase(jt);
                    } else {
                        (*jt)->addMass((*it)->getMass());
                        it = bodies.erase(it);
                    }
                    goto try_again;
                }
            }
        }
    }

};