#include "Scene.h"

class GameWon : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //

    // ————— DESTRUCTOR ————— //
    ~GameWon();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time, int lives) override;
    void render(ShaderProgram *program) override;
};
