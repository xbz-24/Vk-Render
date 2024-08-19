#include "scene.hpp"

Scene::Scene()
{
    triangle_positions_.emplace_back(-0.5f, 0.0f, 0.0f);
    triangle_positions_.emplace_back(0.5f, 0.0f, 0.0f);
}