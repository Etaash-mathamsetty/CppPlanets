#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include "Body.hpp"
#include "Vector3.hpp"

#ifndef _WORLD_H_
#define _WORLD_H_

class World {
private:
    std::vector<Body*> bodies;
    std::vector<Body*> org;

    double compute_dUdv(Body* in, const Vector3& v) {
        double pot = 0.0;
        for (auto b : bodies) {
            if (b != in) {
                pot += b->getPot(in->getPos(), v) - b->getPot(in->getPos());
            }
        }
        /*
        std::cout << "v" << std::endl;
        v.print();
        std::cout << "pot diff " << pot << std::endl;
        */

        return pot / v.norm() * in->getMass();
    }

    Vector3 getForce(Body* in) {
        double U_x = compute_dUdv(in, Vector3(0.01, 0, 0));
        double U_y = compute_dUdv(in, Vector3(0, 0.01, 0));
        double U_z = compute_dUdv(in, Vector3(0, 0, 0.01));
        return Vector3(U_x, U_y, U_z).negate();
    }

    std::atomic_uint index;

    static void worker_thread(World *world)
    {
        while (true)
        {
            uint index = world->index++;
            if (index >= world->bodies.size()) break;
            Body *body = world->bodies[index];
            body->compute(world->getForce(body));
        }
    }

    void compute() {
        uint thread_count = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        index = 0;

        threads.reserve(thread_count);

        for (uint i = 0; i < thread_count; i++)
        {
            threads.emplace_back(
                worker_thread,
                this);
        }

        /* wait for completion */
        for (auto& t : threads)
        {
            t.join();
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
            if ((*it)->collisionEnabled())
            for (auto jt = bodies.begin(); jt != bodies.end(); jt++)
            {
                if (*it == *jt) continue;

                if ((*jt)->collisionEnabled())
                {
                    if ((*it)->getPos().getDist((*jt)->getPos()) < (*it)->getRadius() + (*jt)->getRadius())
                    {
                        std::cout << (*it)->getName() << " colided with " << (*jt)->getName() << std::endl;
                        // conserve momentum
                        (*it)->setVel((*it)->getVel().times((*it)->getMass()).add((*jt)->getVel().times((*jt)->getMass())).div((*jt)->getMass() + (*it)->getMass()));
                        (*jt)->setVel((*it)->getVel().times((*it)->getMass()).add((*jt)->getVel().times((*jt)->getMass())).div((*jt)->getMass() + (*it)->getMass()));
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
    }

};
#endif
