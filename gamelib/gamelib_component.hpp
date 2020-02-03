#ifndef GAMELIB_COMPONENT_HPP
#define GAMELIB_COMPONENT_HPP

#include <gamelib_graphics.hpp>
#include <gamelib_actor.hpp>
#include <gamelib_world.hpp>

namespace GameLib {
    class InputComponent {
    public:
        virtual ~InputComponent() {}
        virtual void update(Actor& actor) = 0;
    };

    class ActorComponent {
    public:
        virtual ~ActorComponent() {}
        virtual void update(Actor& actor, World& world) = 0;
    };

    class PhysicsComponent {
    public:
        virtual ~PhysicsComponent() {}
        virtual void update(Actor& actor, World& world) = 0;
    };

    class GraphicsComponent {
    public:
        virtual ~GraphicsComponent() {}
        virtual void update(Actor& actor, Graphics& graphics) = 0;
    };

    class SimpleInputComponent : public InputComponent {
    public:
        virtual ~SimpleInputComponent() {}
        void update(Actor& actor) override;
    };

	class StationaryInputComponent : public InputComponent {
    public:
        virtual ~StationaryInputComponent() {}
        void update(Actor& actor) override;
    };

    class RandomInputComponent : public InputComponent {
    public:
        virtual ~RandomInputComponent() {}
        void update(Actor& actor) override;
    };

	class ChaseInputComponent : public InputComponent {
    public:
        virtual ~ChaseInputComponent() {}
        void getPlyPosition(Actor& ply);
        void update(Actor& actor) override;

	private:
        glm::vec3 plyPos_{0,0,0};
    };

    class SimpleActorComponent : public ActorComponent {
    public:
        virtual ~SimpleActorComponent() {}
        void update(Actor& actor, World& world) override;
    };

    class RandomActorComponent : public ActorComponent {
    public:
        virtual ~RandomActorComponent() {}
        void update(Actor& actor, World& world) override;
    };

    class SimplePhysicsComponent : public PhysicsComponent {
    public:
        virtual ~SimplePhysicsComponent() {}

        void update(Actor& actor, World& world) override;
    };

	class StationaryPhysicsComponent : public PhysicsComponent {
    public:
        virtual ~StationaryPhysicsComponent() {}

        void update(Actor& actor, World& world) override;
    };

    class SimpleGraphicsComponent : public GraphicsComponent {
    public:
        virtual ~SimpleGraphicsComponent() {}
        virtual void update(Actor& actor, Graphics& graphics);
    };
}

#endif // GAMELIB_COMPONENT_HPP
