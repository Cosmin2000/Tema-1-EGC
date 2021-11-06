
#include "2Dobjects.h"

#include <vector>

#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;




//void objects2D::CreateCircle(const char* name, int R, int nr_sections, glm::vec3 color, int Zoffset) {
//
//
//
//    float beta = 2 * M_PI / 100;
//    unsigned int i = 0;
//
//    vector<VertexFormat> circle_vertices;
//    vector<unsigned int> circle_indices;
//    circle_vertices.push_back(VertexFormat(glm::vec3(0, 0, Zoffset), color));
//    for (i = 0; i <= nr_sections; i++) {
//        circle_vertices.push_back(VertexFormat(glm::vec3(R * cos(beta * i), R * sin(beta * i), Zoffset), color));
//    }
//
//    for (i = 0; i <= nr_sections; i++) {
//        circle_indices.push_back(i);
//    }
//    circle_indices.push_back(1);
//    Tema1::CreateMesh(name, circle_vertices, circle_indices);
//    meshes[name]->SetDrawMode(GL_TRIANGLE_FAN);
//
//}
//
