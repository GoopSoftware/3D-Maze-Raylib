#include "Collectible.h"
#include "raymath.h"


Collectible::Collectible(Vector3 pos, const char* modelPath) {
    position = pos;
    collected = false;

    model = LoadModel(modelPath);

    texture = LoadTexture("assets/models/image0.jpg");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

}

Collectible::~Collectible() {
    UnloadModel(model);
    UnloadTexture(texture);
}

void Collectible::draw() {
    if (!collected) {
        DrawModel(model, position, 1.0f, WHITE);
    }
}

bool Collectible::checkCollision(Vector3 playerPosition) {
    float distance = Vector3Distance(playerPosition, position);
    if (distance < 1.0f) {
        collected = true;
        return true;
    }
    return false;
}