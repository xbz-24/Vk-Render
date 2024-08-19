#ifndef INC_3DLOADERVK_SCENE_HPP
#define INC_3DLOADERVK_SCENE_HPP

#include <vector>
#include <glm/glm.hpp>

class Scene
{
public:
    Scene();
    std::vector<glm::vec3> triangle_positions_;
};
#endif //INC_3DLOADERVK_SCENE_HPP
