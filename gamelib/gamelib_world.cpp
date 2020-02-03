#include "pch.h"
#include <gamelib_world.hpp>
#include <gamelib_actor.hpp>

namespace GameLib {
    namespace Tokens {
#define WORLD_TOKENS(ENUM)                                                                                                                                     \
    ENUM(WORLDSIZE)                                                                                                                                            \
    ENUM(WORLD)                                                                                                                                                \
    ENUM(DEFINE)
#define ENUM_VAL(x) x,
#define ENUM_MAP(x) { #x, Tiles::x },

        enum class Tiles { WORLD_TOKENS(ENUM_VAL) };
        std::map<std::string, Tiles> worldTokens{ WORLD_TOKENS(ENUM_MAP) };
        std::map<char, unsigned> mapCell{};
    }

    World::World() { resize(worldSizeX, worldSizeY); }

    World::~World() {
        tiles.clear();
        actors.clear();
    }

    void World::resize(unsigned sizeX, unsigned sizeY) {
        unsigned numTiles = sizeX * sizeY;
        tiles.resize(numTiles);
        worldSizeX = sizeX;
        worldSizeY = sizeY;
    }

    void World::update(float deltaTime, Graphics& graphics) {
        for (auto& actor : actors) {
            if (!actor->active)
                continue;
            actor->update(deltaTime, *this, graphics);
        }
    }

    void World::setTile(unsigned x, unsigned y, Tile tile) {
        if (x >= worldSizeX || y >= worldSizeY)
            return;
        auto index = y * worldSizeX + x;
        tiles[index] = std::move(tile);
    }

    Tile World::getTile(unsigned x, unsigned y) {
        if (x >= worldSizeX || y >= worldSizeY)
            return Tile();
        auto index = y * worldSizeX + x;
        return tiles[index];
    }

    Tile World::getTile(unsigned x, unsigned y) const {
        if (x >= worldSizeX || y >= worldSizeY)
            return Tile();
        auto index = y * worldSizeX + x;
        return tiles[index];
    }

    std::istream& World::readCharStream(std::istream& s) {
        std::string cmd;
        s >> cmd;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
        if (!Tokens::worldTokens.count(cmd))
            return s;
        Tokens::Tiles token = Tokens::worldTokens[cmd];
        switch (token) {
        case Tokens::Tiles::WORLDSIZE:
            unsigned w, h;
            s >> w;
            s >> h;
            resize(w, h);
            break;
        case Tokens::Tiles::WORLD:
            int row;
            s >> row;
            for (unsigned i = 0; i < worldSizeX; i++) {
                char c;
                s >> c;
                unsigned val = c;
                if (Tokens::mapCell.count(c))
                    val = Tokens::mapCell[val];
                setTile(i, row, Tile(val));
            }
            break;
        case Tokens::Tiles::DEFINE:
            char c;
            unsigned val;
            s >> c;
            s >> val;
            Tokens::mapCell[c] = val;
            break;
        default:
            HFLOGWARN("cmd '%' not implemented", cmd.c_str());
        }
        return s;
    }

    std::ostream& World::writeCharStream(std::ostream& s) const {
        // TODO: rows should be written out with characters representing background tiles
        // Main Idea is to use a command with parameters
        // Example:
        // # tile is used to remap a character to a sprite index
        // tile # 34
        // tile A 35
        // # actor define type char NAME
        // # actor text TEXT
        // actor define NPC a "Justine Shellock"
        // actor state BossAlive
        // actor text "Howdy, how are you?"
        // actor text "Have you heard about the monolith?"
        // actor state BossDead
        // actor text "Oh thank you! Now I can go back to carpentry!"
        // actor define NPC b "Samson Hallock"
        // actor state BossAlive
        // actor text "It arrived at midnight the day before"
        // actor text "It makes us sad, can you destroy it?"
        // actor state BossDead
        // actor text "The Monolith is dead. We can not be at peace."
        // actor state BossDead
        // actor define BOSS B "Monolith"
        // actor state BossAlive
        // actor text "What? You think you can destroy me?!"
        // actor text "Ha! Ha! Ha! Do your worst!"
        // actor state BossDead
        // actor text "Oh no! What a world!"
        // # world N ... is the information for row N
        // # the space character is the value 0, or no tile
        // # the < character is the player start position
        // # the > character is the player end position
        // worldsize width height
        // world 0 #######################
        // world 1 #      a              #
        // world 2 #  AAAAA     b        #
        // world 3 #<     #  AAAA     B >#
        // world 4 #######################

		std::map<unsigned int, char> cellToChar;
        for (auto& [k, v] : Tokens::mapCell) {
            s << "define " << k << " " << v;
            cellToChar[v] = k;
        }

        s << "worldsize " << worldSizeX << " " << worldSizeY << "\n";
        for (unsigned y = 0; y < worldSizeY; ++y) {
            s << "world " << std::setw(2) << y << " ";
            for (unsigned x = 0; x < worldSizeX; ++x) {
                auto t = getTile(x, y);
                if (cellToChar.count(t.charDesc)) {
                    s << cellToChar[t.charDesc];
                } else {
                    s << (char)t.charDesc;
                }
            }
            s << "\n";
        }
        return s;
    }
}
