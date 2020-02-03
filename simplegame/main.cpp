// Simple Game
// by Dr. Jonathan Metzgar et al
// UAF CS Game Design and Architecture Course
#include <gamelib.hpp>
#include <string>
using std::to_string;

#pragma comment(lib, "gamelib.lib")

class PlaySoundCommand : public GameLib::InputCommand {
public:
    PlaySoundCommand(int audioClipId, bool stopPrevious)
        : musicClipId_{ audioClipId }
        , stopPrevious_{ stopPrevious } {}

    const char* type() const override { return "PlaySoundCommand"; }

    bool execute(float amount) override {
        GameLib::Locator::getAudio()->playAudio(musicClipId_, stopPrevious_);
        return true;
    }

private:
    int musicClipId_{ 0 };
    bool stopPrevious_{ false };
};

class PlayMusicCommand : public GameLib::InputCommand {
public:
    PlayMusicCommand(int musicClipId)
        : musicClipId_{ musicClipId } {}

    const char* type() const override { return "PlayMusicCommand"; }

    bool execute(float amount) override {
        GameLib::Locator::getAudio()->playMusic(musicClipId_, -1, 0);
        return true;
    }

private:
    int musicClipId_{ 0 };
    bool stopPrevious_{ false };
};

class QuitCommand : public GameLib::InputCommand {
public:
    const char* type() const override { return "QuitCommand"; }
    bool execute(float amount) override {
        GameLib::Locator::getContext()->quitRequested = true;
        return true;
    }
};

class MovementCommand : public GameLib::InputCommand {
public:
    const char* type() const override { return "MovementCommand"; }
    bool execute(float amount) override {
        // apply slight curve
        if (amount < 0.1f && amount > -0.1f)
            amount = 0.0f;
        else if (amount > 0.5f)
            amount = 1.0f;
        else if (amount < -0.5f)
            amount = -1.0f;
        else if (amount > 0.0f)
            amount = 0.5f;
        else
            amount = -0.5f;
        return InputCommand::execute(amount);
    }
};

namespace GameLib {
    class Font {
    public:
        static constexpr int HALIGN_LEFT = 0;
        static constexpr int HALIGN_CENTER = 1 << 0;
        static constexpr int HALIGN_RIGHT = 1 << 1;
        static constexpr int VALIGN_TOP = 1 << 2;
        static constexpr int VALIGN_CENTER = 1 << 3;
        static constexpr int VALIGN_BOTTOM = 1 << 4;
        static constexpr int SHADOWED = 1 << 5;

        Font(Context* context)
            : context_(context) {}
        ~Font();
        bool load(const std::string& path, int ptsize);
        SDL_Texture* render(const char* text, SDL_Color fg);
        SDL_Texture* lastRender() { return texture_; }
        void newRender() {
            if (texture_) {
                SDL_DestroyTexture(texture_);
                texture_ = nullptr;
            }
            if (surface_) {
                SDL_FreeSurface(surface_);
                surface_ = nullptr;
            }
        }

		// calculates the width of the string text
        int calcWidth(const char* text) {
            int w{ 0 };
            if (font_)
                TTF_SizeUTF8(font_, text, &w, nullptr);
            return w;
        }

		// calculates the height of the loaded font
        int calcHeight() const { return TTF_FontHeight(font_); }

		// returns the height of the rendered string
		int height() const { return rect_.h; }
		// returns the width of the renderered string
        int width() const { return rect_.w; }

        void draw(int x, int y);

        void draw(int x, int y, const char* text, SDL_Color fg, int flags) {
            if (flags & HALIGN_CENTER) {
                x -= calcWidth(text) >> 1;
            } else if (flags & HALIGN_RIGHT) {
                x -= calcWidth(text);
            }
            if (flags & VALIGN_TOP) {
            } else if (flags & VALIGN_CENTER) {
                y -= calcHeight() >> 1;
            } else if (flags & VALIGN_BOTTOM) {
                y -= calcHeight();
			}
            if (flags & SHADOWED) {
                render(text, Black);
                draw(x + 2, y + 2);
            }
            render(text, fg);
            draw(x, y);
        }

    private:
        Context* context_{ nullptr };
        TTF_Font* font_{ nullptr };
        SDL_Texture* texture_{ nullptr };
        SDL_Surface* surface_{ nullptr };
        SDL_Rect rect_;
    };

    Font::~Font() {
        if (font_) {
            TTF_CloseFont(font_);
            font_ = nullptr;
        }
    }

    bool Font::load(const std::string& filename, int ptsize) {
        std::string path = context_->findSearchPath(filename);
        font_ = TTF_OpenFont(path.c_str(), ptsize);
        return font_ != nullptr;
    }

    SDL_Texture* Font::render(const char* text, SDL_Color fg) {
        if (!font_)
            return nullptr;
        newRender();
        surface_ = TTF_RenderText_Blended(font_, text, fg);
        if (surface_) {
            rect_.w = surface_->w;
            rect_.h = surface_->h;
            texture_ = SDL_CreateTextureFromSurface(context_->renderer(), surface_);
        }
        return texture_;
    }

    void Font::draw(int x, int y) {
        rect_.x = x;
        rect_.y = y;
        SDL_Renderer* renderer_ = context_->renderer();
        SDL_RenderCopy(renderer_, texture_, nullptr, &rect_);
    }
}

void testSprites(GameLib::Context& context, int spriteCount, int& spritesDrawn, SDL_Texture* testPNG, SDL_Texture* testJPG);

int main(int argc, char** argv) {
    GameLib::Context context(1280, 720, GameLib::WindowDefault);
    GameLib::Audio audio;
    GameLib::InputHandler input;
    GameLib::Graphics graphics{ &context };

    GameLib::Locator::provide(&context);
    if (context.audioInitialized())
        GameLib::Locator::provide(&audio);
    GameLib::Locator::provide(&input);
    GameLib::Locator::provide(&graphics);

    QuitCommand quitCommand;
    MovementCommand xaxisCommand;
    MovementCommand yaxisCommand;

    input.back = &quitCommand;
    input.axis1X = &xaxisCommand;
    input.axis1Y = &yaxisCommand;

    context.addSearchPath("./assets");
    context.addSearchPath("../assets");
    SDL_Texture* testPNG = context.loadImage("godzilla.png");
    SDL_Texture* testJPG = context.loadImage("parrot.jpg");
    graphics.setTileSize(32, 32);
    int spriteCount = context.loadTileset(0, 32, 32, "Tiles32x32.png");
    if (!spriteCount) {
        HFLOGWARN("Tileset not found");
    }

    /*context.loadAudioClip(0, "starbattle-bad.wav");*/
    context.loadMusicClip(0, "Jason_Shaw_-_Big_Car_Theft.mp3");
    context.loadMusicClip(1, "IWBTB-You-died-Song.mp3");

    GameLib::Font gothicfont(&context);
    GameLib::Font minchofont(&context);
    gothicfont.load("fonts-japanese-gothic.ttf", 36);
    minchofont.load("fonts-japanese-mincho.ttf", 36);

    GameLib::World world;
    GameLib::Locator::provide(&world);
    std::string worldPath = context.findSearchPath("world.txt");
    if (!world.load(worldPath)) {
        HFLOGWARN("world.txt not found");
    }

    Hf::StopWatch stopwatch;

    GameLib::Actor player(new GameLib::SimpleInputComponent(),
                          new GameLib::SimpleActorComponent(),
                          new GameLib::SimplePhysicsComponent(),
                          new GameLib::SimpleGraphicsComponent());
    world.actors.push_back(&player);
    player.spriteId = 2;
    player.position.x = 25;
    player.position.y = 18;
    auto playerPos = player.position;
    player.speed = (float)graphics.getTileSizeX();
    

    GameLib::Actor chasePlayer(new GameLib::ChaseInputComponent(),
                                new GameLib::SimpleActorComponent(),
                                new GameLib::SimplePhysicsComponent(),
                                new GameLib::SimpleGraphicsComponent());
    world.actors.push_back(&chasePlayer);
    chasePlayer.spriteId = 1;
    chasePlayer.position.x = 0;
    chasePlayer.position.y = 0;
    auto chasePlayerPos = chasePlayer.position;
    chasePlayer.speed = player.speed / 4;

	GameLib::Actor scrollingWall1(new GameLib::StationaryInputComponent(), 
		                       new GameLib::SimpleActorComponent(), 
		                       new GameLib::StationaryPhysicsComponent(), 
		                       new GameLib::SimpleGraphicsComponent());
    world.actors.push_back(&scrollingWall1);
    scrollingWall1.spriteId = 1080 - 38;
    scrollingWall1.position.x = 10;
    scrollingWall1.position.y = 0;
    scrollingWall1.speed = player.speed * 2;

	GameLib::Actor scrollingWall2(new GameLib::StationaryInputComponent(),
                                 new GameLib::SimpleActorComponent(),
                                 new GameLib::StationaryPhysicsComponent(),
                                 new GameLib::SimpleGraphicsComponent());
    world.actors.push_back(&scrollingWall2);
    scrollingWall2.spriteId = 1080 - 38;
    scrollingWall2.position.x = 10;
    scrollingWall2.position.y = graphics.getCenterY() / (float)graphics.getTileSizeY();
    scrollingWall2.speed = player.speed * 2;

	GameLib::Actor scrollingWall3(new GameLib::StationaryInputComponent(),
                                 new GameLib::SimpleActorComponent(),
                                 new GameLib::StationaryPhysicsComponent(),
                                 new GameLib::SimpleGraphicsComponent());
    world.actors.push_back(&scrollingWall3);
    scrollingWall3.spriteId = 1080 - 38;
    scrollingWall3.position.x = graphics.getCenterX() / (float)graphics.getTileSizeX();
    scrollingWall3.position.y = 0;
    scrollingWall3.speed = player.speed * 2;

	GameLib::Actor scrollingWall4(new GameLib::StationaryInputComponent(),
                                 new GameLib::SimpleActorComponent(),
                                 new GameLib::StationaryPhysicsComponent(),
                                 new GameLib::SimpleGraphicsComponent());
    world.actors.push_back(&scrollingWall4);
    scrollingWall4.spriteId = 1080 - 38;
    scrollingWall4.position.x = graphics.getCenterX() / (float)graphics.getTileSizeX();
    scrollingWall4.position.y = graphics.getCenterY() / (float)graphics.getTileSizeY();
    scrollingWall4.speed = player.speed * 2;

	GameLib::Actor scrollingWall5(new GameLib::StationaryInputComponent(),
                                 new GameLib::SimpleActorComponent(),
                                 new GameLib::StationaryPhysicsComponent(),
                                 new GameLib::SimpleGraphicsComponent());
    world.actors.push_back(&scrollingWall5);
    scrollingWall5.spriteId = 1080 - 38;
    scrollingWall5.position.x = 30;
    scrollingWall5.position.y = 0;
    scrollingWall5.speed = player.speed * 2;

	GameLib::Actor scrollingWall6(new GameLib::StationaryInputComponent(),
                                 new GameLib::SimpleActorComponent(),
                                 new GameLib::StationaryPhysicsComponent(),
                                 new GameLib::SimpleGraphicsComponent());
    world.actors.push_back(&scrollingWall6);
    scrollingWall6.spriteId = 1080 - 38;
    scrollingWall6.position.x = 30;
    scrollingWall6.position.y = graphics.getCenterY() / (float)graphics.getTileSizeY();
    scrollingWall6.speed = player.speed * 2;

	GameLib::Actor scoreNode(new GameLib::RandomInputComponent(),
                                  new GameLib::SimpleActorComponent(),
                                  new GameLib::SimplePhysicsComponent(),
                                  new GameLib::SimpleGraphicsComponent());
    world.actors.push_back(&scoreNode);
    scoreNode.spriteId = 40 * 14;
    scoreNode.position.x = 0;
    scoreNode.position.y = 0;
    scoreNode.speed = player.speed;

    float t0 = stopwatch.Stop_sf();

	GameLib::Locator::getAudio()->playMusic(0, -1, 0);

	bool sceneActive{ true };

	int score = 0;

    while (!context.quitRequested) {
        float t1 = stopwatch.Stop_sf();
        float dt = t1 - t0;
        t0 = t1;
        GameLib::Context::deltaTime = dt;
        GameLib::Context::currentTime_s = t1;
        GameLib::Context::currentTime_ms = t1 * 1000;

		// Event and input handling
        context.getEvents();
        input.handle();

		// Update graphics
        context.clearScreen(GameLib::Azure);

        for (unsigned x = 0; x < world.worldSizeX; x++) {
            for (unsigned y = 0; y < world.worldSizeY; y++) {
                GameLib::SPRITEINFO s;
                s.position = { x * 32, y * 32 };
                auto t = world.getTile(x, y);
                context.drawTexture(s.position, 0, t.charDesc);
            }
        }

        world.update(dt, graphics);

		// Game Logic
        auto direction = glm::normalize(player.position - chasePlayer.position);
        chasePlayer.velocity = direction;

        if (player.isColliding(chasePlayer) || player.isColliding(scrollingWall1) || player.isColliding(scrollingWall2) || player.isColliding(scrollingWall3)
            || player.isColliding(scrollingWall4) || player.isColliding(scrollingWall5) || player.isColliding(scrollingWall6)) {
            if (sceneActive) {
                sceneActive = false;
                player.active = false;
                chasePlayer.active = false;
                GameLib::Locator::getAudio()->playMusic(1, 0, 0);
            }
        }

        if (player.isColliding(scoreNode)) {
            score += 1;
            float x = (rand() % (39 - 1));
            float y = (rand() % (21 - 1));
            scoreNode.position = glm::vec3(x, y, 0);
        }

        if (!sceneActive) {
            float s = GameLib::wave(t1, 1.0f);
            SDL_Color c = GameLib::MakeColorHI(7, 4, s, false);
            gothicfont.draw((int)graphics.getCenterX(),
                            (int)graphics.getCenterY(),
                            "Game over!\n Press R or Start to restart",
                            c,
                            GameLib::Font::SHADOWED | GameLib::Font::HALIGN_CENTER | GameLib::Font::VALIGN_CENTER);
			if (context.keyboard.scancodes[SDL_SCANCODE_R] || context.gameControllers->start) {
                sceneActive = true;
                player.active = true;
                player.position = playerPos;
                chasePlayer.active = true;
                chasePlayer.position = chasePlayerPos;
                score = 0;
                GameLib::Locator::getAudio()->playMusic(0, -1, 0);
            }
        }

		minchofont.draw(0, (int)graphics.getHeight()-2, ("Score: " + to_string(score)).c_str(), GameLib::Gold, GameLib::Font::VALIGN_BOTTOM | GameLib::Font::SHADOWED);

        context.swapBuffers();
        std::this_thread::yield();
    }
    
    return 0;
}

void testSprites(GameLib::Context& context, int spriteCount, int& spritesDrawn, SDL_Texture* testPNG, SDL_Texture* testJPG) {
    // if (context.keyboard.scancodes[SDL_SCANCODE_ESCAPE]) {
    //    context.quitRequested = true;
    //}

    // An arbitrary number roughly representing 4k at 8 layers, 32x32 sprites
    // constexpr int SpritesToDraw = 128 * 72 * 8;
    // An arbitrary number roughly representing HD at 4 layers, 32x32 sprites
    constexpr int SpritesToDraw = 5;
    // 60 * 34 * 4;
    for (int i = 0; i < SpritesToDraw; i++) {
        GameLib::SPRITEINFO s;
        s.position = { rand() % 1280, rand() % 720 };
        s.center = { 0.0f, 0.0f };
        s.flipFlags = 0;
        s.angle = (float)(rand() % 360);
        // context.drawTexture(0, rand() % spriteCount, s);
        s.position = { rand() % 1280, rand() % 720 };
        context.drawTexture(s.position, 0, rand() % spriteCount);
    }
    spritesDrawn += SpritesToDraw;

    context.drawTexture({ 50, 0 }, { 100, 100 }, testPNG);
    context.drawTexture({ 250, 250 }, { 100, 100 }, testJPG);
}
