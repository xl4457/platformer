#include "Menu.h"
#include "Utility.h"

#define LEVEL_WIDTH 1
#define LEVEL_HEIGHT 1

unsigned int MENU_DATA[] = { 0 };
constexpr char FONTS_FILEPATH[] = "assets/fonts/font1.png";

MenuScreen::~MenuScreen()
{
    //delete[] m_state.enemies;
    //delete    m_state.player;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeMusic(m_game_state.bgm);
}

void MenuScreen::initialise() {
    m_game_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/images/platformA.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, MENU_DATA, map_texture_id, 1.0f, 1, 1);
    
    m_game_state.player = new Entity();
    m_game_state.player->set_entity_type(PLAYER);
    m_game_state.player->set_position(glm::vec3(0.0f));
    m_game_state.player->set_movement(glm::vec3(0.0f));
    m_game_state.player->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    m_game_state.player->deactivate();

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_game_state.bgm = Mix_LoadMUS("assets/audio/bgm.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 13.0f);
}

void MenuScreen::update(float delta_time, int lives)
{
    if (delta_time == -1) {
        m_game_state.next_scene_id = 1;
        }
}


void MenuScreen::render(ShaderProgram* program)
{
    m_game_state.map->render(program);
    GLuint text_texture_id = Utility::load_texture(FONTS_FILEPATH);
    Utility::draw_text(program, text_texture_id, "PLATFORMER", 0.5f, -0.1f, glm::vec3(3.0f, -3.0f, 0.0f));
    Utility::draw_text(program, text_texture_id, "PRESS ENTER TO START", 0.5f, -0.1f, glm::vec3(1.2f, -4.5f, 0.0f));

}
