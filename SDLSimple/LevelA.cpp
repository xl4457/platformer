#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

constexpr char PLAYER_FILEPATH[] = "assets/images/player.png",
           PLATFORM_FILEPATH[]    = "assets/images/platformA.png",
ENEMY_FILEPATH[]       = "assets/images/enemy.png";

unsigned int LEVELA_DATA[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

LevelA::~LevelA()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise()
{
    GLuint map_texture_id = Utility::load_texture(PLATFORM_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 4, 1);
    
    GLuint player_texture_id = Utility::load_texture(PLAYER_FILEPATH);

    m_game_state.player = new Entity();
    m_game_state.player->set_entity_type(PLAYER);
    m_game_state.player->set_texture_id(player_texture_id);
    m_game_state.player->set_speed(5.0f);
    m_game_state.player->set_position(glm::vec3(2.0f, -5.0f, 0.0f));
    m_game_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_game_state.player->set_jumping_power(7.0f);

    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);
    
    m_game_state.enemies = new Entity();
    m_game_state.enemies->set_entity_type(ENEMY);
    m_game_state.enemies->set_ai_type(JUMPER);
    m_game_state.enemies->set_ai_state(JUMPING);
    m_game_state.enemies->set_texture_id(enemy_texture_id);
    m_game_state.enemies->set_position(glm::vec3(7.0f, -1.0f, 0.0f));
    m_game_state.enemies->set_movement(glm::vec3(0.0f));
    m_game_state.enemies->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_game_state.bgm = Mix_LoadMUS("assets/audio/bgm.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 13.0f);

    m_game_state.jump_sfx = Mix_LoadWAV("assets/audio/jump.wav");
}

void LevelA::update(float delta_time, int lives)
{
    m_game_state.player->lives = lives;
    
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
    
    if (m_game_state.player->lives == 0) m_game_state.player->killed = true;
}


void LevelA::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < m_number_of_enemies; i++)
        m_game_state.enemies[i].render(program);
}
