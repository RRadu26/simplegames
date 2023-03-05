#pragma once
#include "components/simple_scene.h"
#include <vector>

namespace m1
{
   class trackg
   {
      public:
         trackg(std::vector<glm::vec3> initialPoints);
         Mesh *mesh;
         std::vector<VertexFormat> vertices;
         std::vector<unsigned int> indices;
      private:
         void init(std::vector<glm::vec3> initialPoints);
         float h = 1;
      protected:

    };
}   // namespace m1
