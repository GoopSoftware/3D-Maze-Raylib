#include "Collectible.h"
#include "raymath.h"


Collectible::Collectible(Vector3 pos, const char* modelPath) {
    position = pos;
    collected = false;

    model = LoadModel(modelPath);

    texture = LoadTexture("assets/models/image0.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

}

Collectible::~Collectible() {
    UnloadModel(model);
    UnloadTexture(texture);
}

void Collectible::draw(Vector3 playerPosition) {
    if (!collected) {
        // Calculate direction from the collectible to the player
        Vector3 direction = Vector3Subtract(playerPosition, position);

        // Normalize the direction vector (make it a unit vector)
        direction = Vector3Normalize(direction);

        // Calculate the angle to rotate the collectible around the Y-axis
        float angle = atan2f(direction.x, direction.z);  // Using atan2 to get the angle in radians

        angle -= PI / 2.0f;

        // Apply rotation to the model
        Matrix rotation = MatrixRotateY(angle);

        // Scale and draw the model with the rotation matrix
        DrawModelEx(model, position, { 0.0f, 1.0f, 0.0f }, angle * RAD2DEG, { 0.25f, 0.25f, 0.25f }, WHITE);
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