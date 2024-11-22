#include "Scene.h"

class GameLost : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //

    // ————— DESTRUCTOR ————— //
    ~GameLost();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time, int lives) override;
    void render(ShaderProgram *program) override;
};
