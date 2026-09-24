#pragma once

enum class ColliderShape : uint8_t { Box, Sphere, Capsule };

struct RigidbodyComponent {
    ColliderShape shape        = ColliderShape::Box;
    glm::vec3     half_extents = { 0.5f, 0.5f, 0.5f };
    float         radius       = 0.5f;
    float         height       = 1.0f;
    float         mass         = 1.0f;
    bool          is_static    = false;
    bool          is_sensor    = false;
};