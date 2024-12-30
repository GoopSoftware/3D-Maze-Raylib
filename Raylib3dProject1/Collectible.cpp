#include "Collectible.h"
#include "raymath.h"


Collectible::Collectible(Vector3 pos) {
    position = pos;
    collected = false;
}

void Collectible::draw() {
    if (!collected) {
        DrawCube(position, 1.0f, 1.0f, 1.0f, GOLD);
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