#include "lab_m1/Tema1/Tema1.h"

#include <math.h> 
#include <iostream>
#include "lab_m1/Tema1/transform2D.h"

//using namespace objects2D;
using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

Mesh* Tema1::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float height,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, height, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}





void Tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // TODO(student): Create the VAO and bind it

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    // TODO(student): Create the VBO and bind it

    // TODO(student): Send vertices data into the VBO buffer

    unsigned int IBO = 0;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
    // TODO(student): Create the IBO and bind it

    // TODO(student): Send indices data into the IBO buffer

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // TODO(student): Unbind the VAO
    glBindVertexArray(0);
    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

void Tema1::CreateCircle(const char* name, int R, int nr_sections,glm::vec3 color, int Zoffset) {



    float beta = 2 * M_PI / 100;
    unsigned int i = 0;

    vector<VertexFormat> circle_vertices;
    vector<unsigned int> circle_indices;
    circle_vertices.push_back(VertexFormat(glm::vec3(0, 0, Zoffset), color));
    for (i = 0; i <= nr_sections; i++) {
        circle_vertices.push_back(VertexFormat(glm::vec3(R * cos(beta * i), R * sin(beta * i), Zoffset),color));
    }

    for (i = 0; i <= nr_sections; i++) {
        circle_indices.push_back(i);
    }
    circle_indices.push_back(1);
    CreateMesh(name, circle_vertices, circle_indices);
    meshes[name]->SetDrawMode(GL_TRIANGLE_FAN);

}




void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    //playerX = 0;
    //playerY = 0;
    Radius = 4;
    alfa = 3 * M_PI / 2;
    leftLegX = Radius * cos(5 * M_PI / 4);
    leftLegY = Radius * sin(5 * M_PI / 4);
    rightLegX = Radius * cos(7 * M_PI / 4);
    rightLegY = Radius * sin(7 * M_PI / 4);
    cursorAngle = 0;
    showProiectil = 0;
    nrProiectile = 0;
    time = Engine::GetElapsedTime();
    player = Player(0.f, 0.f);;

    

    //Creez mapa
    Mesh* map = Tema1::CreateRectangle("map", glm::vec3(-50, -50, 0),400.0f,300.0f, glm::vec3(128.0/255.0, 96.0/255.0, 67.0/255.0),true);
    AddMeshToList(map);

    Mesh* obstacle1 = Tema1::CreateRectangle("obstacle1", glm::vec3(5, 5, 1), 10.0f, 5.0f, glm::vec3(235.0/255.0, 70.0/255.0, 0.0), true);
    AddMeshToList(obstacle1);

    Mesh* obstacle2 = Tema1::CreateRectangle("obstacle2", glm::vec3(-40, 0, 1), 19.0f, 3.0f, glm::vec3(235.0 / 255.0, 70.0 / 255.0, 0.0), true);
    AddMeshToList(obstacle2);

    Mesh* obstacle3 = Tema1::CreateRectangle("obstacle3", glm::vec3(-37, 14, 1), 5.0f, 10.0f, glm::vec3(235.0 / 255.0, 70.0 / 255.0, 0.0), true);
    AddMeshToList(obstacle3);

    Mesh* obstacle4= Tema1::CreateRectangle("obstacle4", glm::vec3(-30, -30, 1), 15.0f, 5.0f, glm::vec3(235.0 / 255.0, 70.0 / 255.0, 0.0), true);
    AddMeshToList(obstacle4);

    Mesh* obstacle5 = Tema1::CreateRectangle("obstacle5", glm::vec3(-25, -30, 1), 5.0f, 20.0f, glm::vec3(235.0 / 255.0, 70.0 / 255.0, 0.0), true);
    AddMeshToList(obstacle5);

    Mesh* obstacle6 = Tema1::CreateRectangle("obstacle6", glm::vec3(50, -40, 1), 20.0f, 20.0f, glm::vec3(235.0 / 255.0, 70.0 / 255.0, 0.0), true);
    AddMeshToList(obstacle6);

    Mesh* proiectil = Tema1::CreateRectangle("proiectil", glm::vec3(player.x - PROIECTIL_LEN/2, player.y - PROIECTIL_LEN/2, 1), PROIECTIL_LEN, PROIECTIL_LEN, glm::vec3(0, 0 , 0), true);
    AddMeshToList(proiectil);

    //Desenez primul cerc din jucator
    CreateCircle("circle",Radius,100, glm::vec3(3 / 255.0, 123 / 255.0, 252.0 / 255.0),1);

    CreateCircle("legLeft", Radius/4, 100, glm::vec3(0, 0, 0),2.0);

    CreateCircle("legRight", Radius/4, 100, glm::vec3(0, 0, 0),2.0);



    logicSpace.x = -50;       // logic x
    logicSpace.y = -50;       // logic y
    logicSpace.width = 100;   // logic width
    logicSpace.height = 100;  // logic height

  
}

//bool CheckCollision(float playerX,float playerY) // AABB - Circle collision
//{
//    // get center point circle first 
//    glm::vec2 center(one.Position + one.Radius);
//    // calculate AABB info (center, half-extents)
//    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
//    glm::vec2 aabb_center(
//        two.Position.x + aabb_half_extents.x,
//        two.Position.y + aabb_half_extents.y
//    );
//    // get difference vector between both centers
//    glm::vec2 difference = center - aabb_center;
//    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
//    // add clamped value to AABB_center and we get the value of box closest to circle
//    glm::vec2 closest = aabb_center + clamped;
//    // retrieve vector between center circle and closest point AABB and check if length <= radius
//    difference = closest - center;
//    return glm::length(difference) < one.Radius;
//}


void Tema1::DrawBullets(glm::mat3 visMatrix, float deltaTimeSeconds) {
    if (showProiectil) {
        //modelMatrix = visMatrix * transform2D::Translate(playerX, playerY);
        for (int i = 0; i < proiectile.size(); i++) {

            proiectile[i].x += PROIECTIL_SPEED * cos(proiectile[i].angle) * deltaTimeSeconds;
            proiectile[i].y += PROIECTIL_SPEED * sin(proiectile[i].angle) * deltaTimeSeconds;

            modelMatrix = visMatrix * transform2D::Translate(proiectile[i].x, proiectile[i].y);
            modelMatrix *= transform2D::Rotate(proiectile[i].angle);
            RenderMesh2D(meshes["proiectil"], shaders["VertexColor"], modelMatrix);

            if (proiectile[i].x - proiectile[i].initialX > 30.f || proiectile[i].y - proiectile[i].initialY > 30.f || proiectile[i].x - proiectile[i].initialX < -30.f || proiectile[i].y - proiectile[i].initialY < -30.f) {
                proiectile.erase(proiectile.begin() + i);
            }

        }
        
    }
}

void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{

    
    modelMatrix = visMatrix ;
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);


    modelMatrix = visMatrix * transform2D::Translate(player.x ,player.y);
    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(player.x, player.y);
    modelMatrix *= transform2D::Translate(leftLegX, leftLegY);
    RenderMesh2D(meshes["legLeft"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(player.x, player.y);
    modelMatrix *= transform2D::Translate(rightLegX, rightLegY);
    RenderMesh2D(meshes["legRight"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);


    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle4"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle5"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle6"], shaders["VertexColor"], modelMatrix);
    
    


}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw - the left half of the window
    viewSpace = ViewportSpace(0, 0 , resolution.x,resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);


    DrawScene(visMatrix, deltaTimeSeconds);
    
    DrawBullets(visMatrix, deltaTimeSeconds);
    
   


    


   

}


void Tema1::FrameEnd()
{
    
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{

    if (window->KeyHold(GLFW_KEY_W)) {
        player.y += SPEED * deltaTime;
        logicSpace.y += deltaTime * SPEED;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        player.y -= SPEED * deltaTime;
        logicSpace.y -= deltaTime * SPEED;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        player.x += SPEED * deltaTime;
        logicSpace.x += deltaTime * SPEED;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        player.x -= SPEED * deltaTime;
        logicSpace.x -= deltaTime * SPEED;
    }
   

}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    /*float b = Radius;
    float c = sqrt(pow(mouseX, 2) + pow(mouseY, 2));
    float a = sqrt(pow((Radius * cos(alfa) - mouseX), 2) + pow((Radius * sin(alfa) - mouseY), 2));
    float cosAlfa = (pow(a, 2) - pow(b, 2) - pow(c, 2)) / (2 * (double)b * c);
    cusorTranslateX = Radius*cosAlfa;
    cusorTranslateY = Radius * sqrt(1 - pow(cosAlfa, 2));
    alfa = acos(cosAlfa);*/


    cursorAngle = atan2(viewSpace.height/2  - mouseY, mouseX - viewSpace.width/2);
    leftLegX = Radius * cos(cursorAngle + M_PI/4);
    leftLegY = Radius * sin(cursorAngle + M_PI/4);
    rightLegX = Radius * cos(cursorAngle - M_PI/4);
    rightLegY = Radius * sin(cursorAngle - M_PI/4);

    
    

}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
  
    if (Engine::GetElapsedTime() - time >= 0.5f) {


        showProiectil = 1;
        cursorAngle = atan2(viewSpace.height / 2 - mouseY, mouseX - viewSpace.width / 2);

        proiectilX = player.x + Radius * cos(cursorAngle);
        proiectilY = player.y + Radius * sin(cursorAngle);

        Proiectil proiectil = Proiectil(proiectilX, proiectilY, cursorAngle, proiectilX, proiectilY);
        proiectile.push_back(proiectil);
        time = Engine::GetElapsedTime();
    }

    
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
