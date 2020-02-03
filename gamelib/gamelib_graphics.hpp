#ifndef GAMELIB_GRAPHICS_HPP
#define GAMELIB_GRAPHICS_HPP

#include <gamelib_context.hpp>

namespace GameLib {
    class IGraphics {
    public:
        virtual ~IGraphics() {}

        float getCenterX() const { return getWidth() * 0.5f; }
        float getCenterY() const { return getHeight() * 0.5f; }

        virtual float getWidth() const { return 1; }
        virtual float getHeight() const { return 1; }
        virtual int getTileSizeX() const { return 1; }
        virtual int getTileSizeY() const { return 1; }
        virtual void setTileSize(int w, int h) {}
        virtual void draw(int tileSetId, int tileId, int x, int y) {}
        virtual void draw(int tileSetId, int tileId, float x, float y) {}
        virtual void draw(int tileSetId, int tileId, int x, int y, int flipFlags) {}
    };

    class Graphics : public IGraphics {
    public:
        Graphics(Context* ctx);
        virtual ~Graphics();

        float getWidth() const override { return (float)context->screenWidth; }
        float getHeight() const override { return (float)context->screenHeight; }
        int getTileSizeX() const override { return tileSizeX_; }
        int getTileSizeY() const override { return tileSizeY_; }
        void setTileSize(int w, int h) override {
            tileSizeX_ = w;
            tileSizeY_ = h;
        }

        void draw(int tileSetId, int tileId, int x, int y) override;
        void draw(int tileSetId, int tileId, float x, float y) override;
        void draw(int tileSetId, int tileId, int x, int y, int flipFlags) override;
    private:
        int tileSizeX_{ 0 };
        int tileSizeY_{ 0 };
        Context* context{ nullptr };
    };
}

#endif
