#include "pch.h"
#include <gamelib_locator.hpp>
#include <gamelib_graphics.hpp>

namespace GameLib {

    Graphics::Graphics(Context* ctx) { context = ctx; }

    Graphics::~Graphics() {}

    void Graphics::draw(int tileSetId, int tileId, float x, float y) { draw(tileSetId, tileId, (int)x, (int)y); }

    void Graphics::draw(int tileSetId, int tileId, int x, int y) {
        auto tileImage = context->getTile(tileSetId, tileId);
        if (!tileImage)
            return;
        context->drawTexture({ x, y }, { tileImage->w, tileImage->h }, tileImage->texture);
    }

    void Graphics::draw(int tileSetId, int tileId, int x, int y, int flipFlags) {
        SPRITEINFO spriteInfo;
        spriteInfo.position = { x, y };
        spriteInfo.flipFlags = flipFlags;
        spriteInfo.center = { 0, 0 };
        spriteInfo.angle = 0.0f;
        context->drawTexture(tileSetId, tileId, spriteInfo);
    }
}
