#include "pch.h"
#include <gamelib_random.hpp>
#include <gamelib_locator.hpp>
#include <gamelib_component.hpp>

namespace GameLib {
    void SimpleInputComponent::update(Actor& actor) {
        auto axis1 = Locator::getInput()->axis1X;
        if (axis1) {
            actor.velocity.x = axis1->getAmount();
        }
        auto axis2 = Locator::getInput()->axis1Y;
        if (axis2) {
            actor.velocity.y = axis2->getAmount();
        }

		if (actor.velocity.x > 0)
            actor.spriteFlipX = true;
        else if (actor.velocity.x < 0)
            actor.spriteFlipX = false;
    }

    void StationaryInputComponent::update(Actor& actor) {
        actor.velocity.x = 0;
		actor.velocity.y = actor.dt * actor.speed;
        glm::normalize(actor.velocity);
    }

    void RandomInputComponent::update(Actor& actor) {
        actor.velocity.x = random.normal();
        actor.velocity.y = random.normal();
        glm::normalize(actor.velocity);
    }

    void ChaseInputComponent::getPlyPosition(Actor& ply) { plyPos_ = ply.position; }

    void ChaseInputComponent::update(Actor& actor) {
        // Handled in main...
        if (actor.velocity.x > 0)
            actor.spriteFlipX = true;
        else if (actor.velocity.x < 0)
            actor.spriteFlipX = false;
    }

    void SimpleActorComponent::update(Actor& actor, World& world) {
        // We could make decisions here, start actions, etc
    }

    void RandomActorComponent::update(Actor& actor, World& world) {
        for (auto& a : world.actors) {
            if (a->getId() == actor.getId())
                continue;
        }
    }

    void SimplePhysicsComponent::update(Actor& actor, World& world) {
        actor.position += actor.dt * actor.speed * actor.velocity;
        if (actor.clipToWorld) {
            actor.position.x = clamp<float>(actor.position.x, 0, (float)world.worldSizeX - actor.size.x);
            actor.position.y = clamp<float>(actor.position.y, 0, (float)world.worldSizeY - actor.size.y);
        }
    }

	void StationaryPhysicsComponent::update(Actor& actor, World& world) {
        actor.position += actor.dt * actor.speed * actor.velocity;
        if (actor.clipToWorld) {
            actor.position.x = clamp<float>(actor.position.x, 0, (float)world.worldSizeX - actor.size.x);
            actor.position.y = clamp<float>(actor.position.y, 0, (float)world.worldSizeY - actor.size.y);
        } 
		if (actor.position.y >= (world.worldSizeY - actor.size.y)) {
			actor.position.y = 0;
		}
    }

    void SimpleGraphicsComponent::update(Actor& actor, Graphics& graphics) {
        glm::vec3 tileSize{ graphics.getTileSizeX(), graphics.getTileSizeY(), 0 };
        glm::vec3 pos = actor.position * tileSize;
        int flipFlags = actor.spriteFlipX ? 1 : actor.spriteFlipY ? 2 : 0;
        graphics.draw(actor.spriteLibId, actor.spriteId, pos.x, pos.y, flipFlags);
    }

}
