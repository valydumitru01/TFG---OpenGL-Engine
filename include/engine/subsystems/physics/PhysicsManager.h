/*******************************************************************************
 *
 * Copyright (c) 2023.
 * Licensed under the MIT License. See LICENSE.txt in the project root for license information.
 ******************************************************************************/

#pragma once
#include "Physics.h"
#include "PhysicsTypes.h"
#include "Collider.h"
#include "engine/subsystems/transform/Transform.h"

namespace GLESC::Physics {
    struct CollisionInformation {
        Vec3B collisionAxis;
        std::vector<const Collider*> colliders;
    };

    class PhysicsManager {
    public:
        void updatePhysics(Physics& physics, Collider& collider, Transform::Transform& transform) {
            physics = getNextPhysics(physics, transform);
            Transform::Transform nextTransform = getNextTransform(transform, physics);
            if (!collider.isSolid()) {
                transform = nextTransform;
                return;
            }
            Transform::Transform oldTransform = transform;

            // Check if the X translation in the next frame will cause a collision
            Transform::Transform nextTransformX = nextTransform;
            nextTransformX.setPosition({
                nextTransformX.getPosition().getX(),
                oldTransform.getPosition().getY(),
                oldTransform.getPosition().getZ()
            });
            Collider nextColliderX = getNextCollider(collider, nextTransformX);


            // Check if the Y translation in the next frame will cause a collision
            Transform::Transform nextTransformY = nextTransform;
            nextTransformY.setPosition({
                oldTransform.getPosition().getX(),
                nextTransformY.getPosition().getY(),
                oldTransform.getPosition().getZ()
            });
            Collider nextColliderY = getNextCollider(collider, nextTransformY);


            // Check if the Z translation in the next frame will cause a collision
            Transform::Transform nextTransformZ = nextTransform;
            nextTransformY.setPosition({
                oldTransform.getPosition().getX(),
                nextTransformY.getPosition().getY(),
                oldTransform.getPosition().getZ()
            });
            Collider nextColliderZ = getNextCollider(collider, nextTransformZ);

            // Cancel the component that causes the collision
            CollisionInformation information = collidesOnNextFrame(collider, nextColliderX, nextColliderY,
                                                                   nextColliderZ);


            if (information.collisionAxis.getX() ||
                information.collisionAxis.getY() ||
                information.collisionAxis.getZ()) {
                if (collider.collisionCallback)
                    collider.collisionCallback();
                for(auto& otherCollider : information.colliders) {
                    // Check if there is a specific callback for this collider (otherCollider
                    auto it = collider.collisionCallbacksForSpecificColliders.find(otherCollider);
                    if (it != collider.collisionCallbacksForSpecificColliders.end()) {
                        it->second();
                    }
                }
            }
            // Cancel the components that cause the collision
            if (information.collisionAxis.getX()) {
                physics.setVelocity({0.0F, physics.getVelocity().getY(), physics.getVelocity().getZ()});
            }
            if (information.collisionAxis.getY()) {
                physics.setVelocity({physics.getVelocity().getX(), 0.0F, physics.getVelocity().getZ()});
            }
            if (information.collisionAxis.getZ()) {
                physics.setVelocity({physics.getVelocity().getX(), physics.getVelocity().getY(), 0.0F});
            }

            nextTransform = getNextTransform(transform, physics);

            transform = nextTransform;
        }

        Transform::Transform getNextTransform(const Transform::Transform& transformParam,
                                              const Physics& nextPhysics) const {
            Transform::Transform nextTransform = transformParam;

            // Calculate position
            nextTransform.addPosition(nextPhysics.getVelocity() * velocityScalar);
            nextTransform.addPosition(nextTransform.forward() * nextPhysics.getForwardVelocity() * velocityScalar);

            return nextTransform;
        }

        Physics getNextPhysics(const Physics& oldPhysics, const Transform::Transform& transform) const {
            Physics physics = oldPhysics;
            // Calculate acceleration
            physics.addAcceleration(physics.getForce() / physics.getMass());
            physics.addAcceleration(transform.forward() * physics.getForwardForce() / physics.getMass());
            if (physics.isAffectedByGravity())
                physics.addAcceleration(gravity);

            // Calculate velocity
            physics.addVelocity(physics.getAcceleration());
            physics.addVelocity(transform.forward() * physics.getForwardAcceleration());

            // Apply friction
            physics.addVelocity(-physics.getVelocity() * airFriction);

            // Clear forces
            physics.giveForce(Force(0.0F, 0.0F, 0.0F));
            physics.giveForwardForce(0.0F);

            // Clear acceleration
            physics.setAcceleration(Acceleration(0.0F, 0.0F, 0.0F));

            return physics;
        }

        Collider getNextCollider(const Collider& oldCollider, const Transform::Transform& nextTransform) const {
            Collider nextCollider = oldCollider;
            nextCollider.boundingVolume = Transform::Transformer::transformBoundingVolume(oldCollider.boundingVolume,
                nextTransform);
            return nextCollider;
        }


        CollisionInformation collidesOnNextFrame(const Collider& originalCollider,
                                                 const Collider& nextColliderX,
                                                 const Collider& nextColliderY,
                                                 const Collider& nextColliderZ) {
            CollisionInformation information;
            for (const auto& otherCollider : colliders) {
                if (otherCollider == &originalCollider) continue;
                Vec3B boolsForCollider{
                    collidesWithCollider(nextColliderX, *otherCollider),
                    collidesWithCollider(nextColliderY, *otherCollider),
                    collidesWithCollider(nextColliderZ, *otherCollider)
                };
                if (boolsForCollider.getX() || boolsForCollider.getY() || boolsForCollider.getZ()) {
                    information.colliders.push_back(otherCollider);
                }
                information.collisionAxis.setX(information.collisionAxis.getX() || boolsForCollider.getX());
                information.collisionAxis.setY(information.collisionAxis.getY() || boolsForCollider.getY());
                information.collisionAxis.setZ(information.collisionAxis.getZ() || boolsForCollider.getZ());
            }
            return information;
        }

        bool collidesWithCollider(const Collider& collider, const Collider& otherCollider) {
            if (collider.boundingVolume.intersects(otherCollider.boundingVolume)) {
                return true;
            }
            return false;
        }

        void addCollider(const Collider& collider) {
            colliders.push_back(&collider);
        }

        void clearColliders() {
            colliders.clear();
        }

    private:
        std::vector<const Collider*> colliders;

        Acceleration gravity{0.f, -8.91f, 0.f};
        Friction airFriction{0.01f};
        /**
         * @brief This scalar is used to scale the velocity of the object
         * @details We need this because using fixed update time steps makes normal values too bit,
         * and the object would move too fast. Using a scalar makes the used be able to handle
         * more realistic numbers.
         */
        Scalar velocityScalar{0.001f};
    };
} // namespace GLESC
