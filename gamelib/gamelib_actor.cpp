#include "pch.h"
#include <gamelib_actor.hpp>
#include <gamelib_locator.hpp>

namespace GameLib {
    unsigned Actor::idSource_{ 0 };

    Actor::Actor(InputComponent* input, ActorComponent* actor, PhysicsComponent* physics, GraphicsComponent* graphics)
        : transform(1.0f)
        , addlTransform(1.0f)
        , input_(input)
        , actor_(actor)
        , physics_(physics)
        , graphics_(graphics) {
        id_ = idSource_++;
    }

    Actor::~Actor() {
        delete input_;
        delete physics_;
        delete graphics_;
    }

    void Actor::beginPlay() {}

    void Actor::update(float deltaTime, World& world, Graphics& graphics) {
        dt = deltaTime;
        if (input_)
            input_->update(*this);
        if (actor_)
            actor_->update(*this, world);
        if (physics_)
            physics_->update(*this, world);
        if (graphics_)
            graphics_->update(*this, graphics);
    }

    void Actor::startOverlap(const_weak_ptr otherObject) {}

    void Actor::endOverlap(const_weak_ptr otherObject) {}

	bool Actor::isColliding(Actor& other) { 
		bool collisionX = position.x + size.x >= other.position.x && other.position.x + other.size.x >= position.x;
		bool collisionY = position.y + size.y >= other.position.y && other.position.y + other.size.y >= position.y;
        return collisionX && collisionY;
	}
}
