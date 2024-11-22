#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 9

constexpr char PLAYER_FILEPATH[]    = "assets/images/player.png",
               PLATFORM_FILEPATH[]  = "assets/images/platformC.png",
ENEMY_FILEPATH[]     = "assets/images/enemy.png";

unsigned int LEVELC_DATA[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 3,
    3, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 0, 0, 0, 0, 0, 1, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 3,
    3, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3
};

LevelC::~LevelC()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelC::initialise()
{
    m_number_of_enemies = ENEMY_COUNT;

    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture(PLATFORM_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 4, 1);
    
    GLuint player_texture_id = Utility::load_texture(PLAYER_FILEPATH);
    
    m_game_state.player = new Entity();
    m_game_state.player->set_entity_type(PLAYER);
    m_game_state.player->set_texture_id(player_texture_id);
    m_game_state.player->set_speed(5.0f);
    m_game_state.player->set_position(glm::vec3(1.0f, 0.0f, 0.0f));
    m_game_state.player->set_jumping_power(7.0f);
    m_game_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity [ENEMY_COUNT];
    for (size_t i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].set_entity_type(ENEMY);
        switch (i)
        {
            case 0:
                m_game_state.enemies[i].set_ai_type(WALKER);
                m_game_state.enemies[i].set_ai_state(WALKING);
                m_game_state.enemies[i].set_movement(glm::vec3(0.0f));
                m_game_state.enemies[i].set_speed(3.0f);
                m_game_state.enemies[i].set_position(glm::vec3(13.0f, -6.0f, 0.0f));
                break;
            case 1:
                m_game_state.enemies[i].set_ai_type(GUARD);
                m_game_state.enemies[i].set_ai_state(IDLE);
                m_game_state.enemies[i].set_movement(glm::vec3(0.0f));
                m_game_state.enemies[i].set_speed(3.0f);
                m_game_state.enemies[i].set_position(glm::vec3(3.0f, -4.0f, 0.0f));
                break;
            case 2:
                m_game_state.enemies[i].set_ai_type(JUMPER);
                m_game_state.enemies[i].set_ai_state(JUMPING);
                m_game_state.enemies[i].set_movement(glm::vec3(0.0f));
                m_game_state.enemies[i].set_speed(4.0f);
                m_game_state.enemies[i].set_position(glm::vec3(12.0f, -4.0f, 0.0f));
                m_game_state.enemies[i].set_jumping_power(9.0f);
                break;
        }
        m_game_state.enemies[i].set_texture_id(enemy_texture_id);
        m_game_state.enemies[i].set_acceleration(glm::vec3(0.0f,-9.81f, 0.0f));

    }
            
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/audio/bgm.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 13.0f);

    m_game_state.jump_sfx = Mix_LoadWAV("assets/audio/jump.wav");
}

void LevelC::update(float delta_time, int lives)
{
    m_game_state.player->lives = lives;

    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
    
    if (m_game_state.player->lives == 0) m_game_state.player->killed = true;

}

void LevelC::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < m_number_of_enemies; i++)
        m_game_state.enemies[i].render(program);
}
