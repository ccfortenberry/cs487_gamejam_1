#ifndef GAMELIB_BASE_HPP
#define GAMELIB_BASE_HPP

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <czmq.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <hatchetfish.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <memory>
#include <thread>
#include <stdexcept>

namespace GameLib {
    struct SPRITEINFO {
        glm::vec2 position;
        float angle;
        glm::vec2 center;
        int flipFlags;
    };

    struct TILEIMAGE {
        SDL_Texture* texture{ nullptr };
        int tileId{ 0 };
        int tilesetId{ 0 };
        int w{ 0 };
        int h{ 0 };
    };

    struct AUDIOINFO {
        Mix_Chunk* chunk{ nullptr };
        std::string name;

        operator bool() const { return chunk != nullptr; }

        void free() {
            Mix_FreeChunk(chunk);
            chunk = nullptr;
        }

        ~AUDIOINFO() {
            if (chunk) {
                HFLOGERROR("Should not delete sound in destructor");
            }
        }
    };

    struct MUSICINFO {
        Mix_Music* chunk{ nullptr };
        std::string name;

        operator bool() const { return chunk != nullptr; }

        void free() {
            Mix_FreeMusic(chunk);
            chunk = nullptr;
        }

        ~MUSICINFO() {
            if (chunk) {
                HFLOGERROR("Should not delete sound in destructor");
            }
        }
    };

	template <typename T>
	T clamp(T x, T a, T b) {
        return (x < a) ? a : (x > b) ? b : x;
	}

	// returns range 0 to 1 with 1 cycle per second
	template <typename T>
	T wave(T x, T cyclesPerSecond) {
        return (T)std::sin(x * cyclesPerSecond * 6.28318531f) * 0.5f + 0.5f;
	}

	template <typename T>
	T triangle(T x, T cyclesPerSecond) {
        float t = x * cyclesPerSecond;
        return (T)(2.0f * std::abs(t - std::floor(t + 0.5f)));
	}

	// Colors
    static const SDL_Color Black{ 0, 0, 0, 255 };
    static const SDL_Color Gray33{ 85, 85, 85, 255 };
    static const SDL_Color Gray67{ 170, 170, 170, 255 };
    static const SDL_Color White{ 255, 255, 255, 255 };
    static const SDL_Color Red{ 255, 0, 0, 255 };
    static const SDL_Color Orange{ 228, 114, 0, 255 };
    static const SDL_Color Yellow{ 228, 228, 0, 255 };
    static const SDL_Color Green{ 0, 255, 0, 255 };
    static const SDL_Color Cyan{ 0, 180, 180, 255 };
    static const SDL_Color Azure{ 0, 114, 228, 255 };
    static const SDL_Color Blue{ 0, 0, 255, 255 };
    static const SDL_Color Violet{ 114, 0, 228, 255 };
    static const SDL_Color Rose{ 228, 0, 114, 255 };
    static const SDL_Color Brown{ 127, 63, 0, 255 };
    static const SDL_Color Gold{ 212, 171, 56, 255 };
    static const SDL_Color ForestGreen{ 63, 127, 63, 255 };
    static const SDL_Color LibXORColors[] = {
        { 0, 0, 0, 255 },     { 85, 85, 85, 255 }, { 170, 170, 170, 255 }, { 255, 255, 255, 255 }, { 255, 0, 0, 255 }, { 228, 114, 0, 255 },
        { 228, 228, 0, 255 }, { 0, 255, 0, 255 },  { 0, 180, 180, 255 },   { 0, 114, 228, 255 },   { 0, 0, 255, 255 }, { 114, 0, 228, 255 },
        { 228, 0, 114, 255 }, { 127, 63, 0, 255 }, { 212, 171, 56, 255 },  { 63, 127, 63, 255 },
    };

	// color1 (0-15), color2 (0-15), mix(0 - 7), neg(true/false)
	inline SDL_Color MakeColor(int color1, int color2, int mix, bool neg) { 
		const SDL_Color& c1 = LibXORColors[color1];
        const SDL_Color& c2 = LibXORColors[color2];
        float t = clamp<float>(mix * 0.142857f, 0.0f, 1.0f);
        Uint8 r = (Uint8)clamp<int>((int)(t * c1.r + (1.0f - t) * c2.r), 0, 255);
        Uint8 g = (Uint8)clamp<int>((int)(t * c1.g + (1.0f - t) * c2.g), 0, 255);
        Uint8 b = (Uint8)clamp<int>((int)(t * c1.b + (1.0f - t) * c2.b), 0, 255);
        if (neg) {
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
        }
		return { r, g, b, 255 };
	}

	// color1 (0-15), color2 (0-15), mix (0-1), neg(true/false)
	inline SDL_Color MakeColorHI(int color1, int color2, float mix, bool neg) {
        const SDL_Color& c1 = LibXORColors[color1];
        const SDL_Color& c2 = LibXORColors[color2];
        float t = clamp<float>(mix, 0.0f, 1.0f);
        Uint8 r = (Uint8)clamp<int>((int)(t * c1.r + (1.0f - t) * c2.r), 0, 255);
        Uint8 g = (Uint8)clamp<int>((int)(t * c1.g + (1.0f - t) * c2.g), 0, 255);
        Uint8 b = (Uint8)clamp<int>((int)(t * c1.b + (1.0f - t) * c2.b), 0, 255);
        if (neg) {
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
        }
        return { r, g, b, 255 };
    }
}

#endif
