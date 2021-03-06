//
//  GameController.hpp
//  OpenGLBreakout
//
//  Created by 梅宇宸 on 16/12/23.
//  Copyright © 2016年 梅宇宸. All rights reserved.
//

#ifndef GameController_hpp
#define GameController_hpp

#include "opengl_includes.hpp"

#include <vector>

#include "Particle.hpp"
#include "GameLevel.hpp"
#include "Ball.hpp"
#include "PostProcessor.hpp"
#include "PowerUp.hpp"
#include "TextRenderer.hpp"

#include <irrKlang.h>

class SpriteRenderer;

using namespace irrklang;

#define SOUND_FULL_DIR "./data/wavs/"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2, glm::vec2> Collision;

class CollisionPairs
{
public:
    CollisionPairs (Ball& gameObj1, GameObject& gameObj2) : ball (gameObj1), brick (gameObj2) {}

    Ball& ball;
    GameObject& brick;
};

class GameController
{
public:
    GameController (GLuint width, GLuint height);
    ~GameController ();

    void Init (GLuint frameBufferWidth, GLuint frameBufferHeight);

    void ProcessInput (GLfloat dt);
    void Update (GLfloat dt);
    void Render ();

    std::vector<CollisionPairs> cpVector;
    Direction VectorDirection (glm::vec2 target);
    void BroadPhaseCollisionDetect ();
    Collision NarrowPhaseCollisionDetect (Ball& one, GameObject& two);
    GLboolean NarrowPhaseCollisionDetect (GameObject& one, GameObject& two);
    void DetectCollision ();

    void ResetLevel ();
    void ResetPlayer ();

    void SpawnPowerUps (GameObject &block);
    void UpdatePowerUps (GLfloat dt);
    void ActivatePowerUp (PowerUp &powerUp);

    GameState mState;
    GLboolean mKeysProcessed[1024];
    GLboolean mKeys[1024];
    GLuint mWidth, mHeight;
    GLuint mFrameBufferWidth, mFrameBufferHeight;

    std::vector<GameLevel> mLevels;
    GLuint mLevel;

    SpriteRenderer* mRenderer;
    ParticleGenerator* mParticles;
    GameObject* mPlayer;
    Ball* mBall;

    GLfloat ShakeTime;
    PostProcessor* mEffects;

    std::vector<PowerUp> PowerUps;

    static ISoundEngine* SoundEngine;

    TextRenderer* mTextRenderer;

    GLuint mLives;
};

#endif /* GameController_hpp */
