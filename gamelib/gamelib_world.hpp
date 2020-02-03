#ifndef GAMELIB_WORLD_HPP
#define GAMELIB_WORLD_HPP

#include <gamelib_object.hpp>
#include <gamelib_graphics.hpp>

namespace GameLib {
    // number of screens in the X direction
    constexpr unsigned WorldPagesX = 4;
    // number of screens in the Y direction
    constexpr unsigned WorldPagesY = 4;

    // number of tiles on one page in the X direction
    constexpr unsigned WorldTilesX = 40;
    // number of tiles on one page in the Y direction
    constexpr unsigned WorldTilesY = 22;

    class Tile {
    public:
        Tile() {}
        Tile(unsigned c)
            : charDesc(c) {}
        unsigned charDesc{ 0 };
    };

    class Actor;

    // World represents a composite of Objects that live in a 2D grid world
    class World : public Object {
    public:
        World();
        virtual ~World();

        void resize(unsigned sizeX, unsigned sizeY);

        void update(float deltaTime, Graphics& graphics);

        void setTile(unsigned x, unsigned y, Tile ptr);
        Tile getTile(unsigned x, unsigned y);
        Tile getTile(unsigned x, unsigned y) const;

        std::istream& readCharStream(std::istream& s) override;
        std::ostream& writeCharStream(std::ostream& s) const override;

        std::vector<Tile> tiles;
        std::vector<Actor*> actors;

        // number of tiles in the X direction
        unsigned worldSizeX{ WorldPagesX * WorldTilesX };

        // number of tiles in the Y direction
        unsigned worldSizeY{ WorldPagesY * WorldTilesY };
    };
}

#endif
