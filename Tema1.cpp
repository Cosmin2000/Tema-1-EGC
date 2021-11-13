#include "lab_m1/Tema1/Tema1.h"

#include <math.h> 
#include <iostream>
#include "lab_m1/Tema1/transform2D.h"


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
 
    Radius = 4;
    leftLeg.x = Radius * cos(5 * M_PI / 4);
    leftLeg.y = Radius * sin(5 * M_PI / 4);
    rightLeg.x = Radius * cos(7 * M_PI / 4);
    rightLeg.y = Radius * sin(7 * M_PI / 4);
    cursorAngle = 0;
    showProiectil = 0;
    time = Engine::GetElapsedTime();
    enemyTime = Engine::GetElapsedTime();
    pickUpTime = Engine::GetElapsedTime();
    player = Coordonate(0.f, 0.f);
    score = 0;
    leftOrightSide = 0;
    glm::vec3 wallColor = glm::vec3(84.f/255.f, 89.f/255.f, 83.f/255.f);

    color = glm::vec3(19.f/255.f, 109.f/255.f, 21.f/255.f);

    leftWall = Figure(-98.f, -98.f, 346.0f, 2.0f);
    rightWall = Figure(196.f, -98.f, 346.0f, 2.0f);
    topWall = Figure(-98.f, 246.f, 2.0f, 296.0f);
    bottomWall = Figure(-98.f, -98.f, 2.0f, 296.0f);

    walls.push_back(leftWall);
    walls.push_back(rightWall);
    walls.push_back(topWall);
    walls.push_back(bottomWall);
   
   Figure obstacle1C = Figure(5.f, 5.f, 10.0f, 5.0f);
   Figure obstacle2C = Figure(-40.f, 0.f, 19.0f, 10.0f);
   Figure obstacle3C = Figure(-30.f, 14.f, 5.0f, 10.0f);
   Figure obstacle4C = Figure(20.f, 25.f, 15.0f, 5.0f);
   Figure obstacle5C = Figure(-70.f, -70.f, 10.0f, 20.0f);
   Figure obstacle6C = Figure(50.f, -40.f, 20.0f, 20.0f);
   Figure obstacle7C = Figure(10.f, -60.f, 10.0f, 20.0f);
   Figure obstacle8C = Figure(80.f, 10.f, 40.0f, 10.0f);
   Figure obstacle9C = Figure(90.f, 10.f, 10.0f, 30.0f);
   Figure obstacle10C = Figure(140.f, -70.f, 50.0f, 10.0f);
   Figure obstacle11C = Figure(-80.f, 50.f, 50.0f, 10.0f);
   Figure obstacle12C = Figure(20.f, 80.f, 50.0f, 10.0f);
   Figure obstacle13C = Figure(30.f, 80.f, 10.0f, 40.0f);
   Figure obstacle14C = Figure(-70.f, 150.f, 10.0f, 60.0f);
   Figure obstacle15C = Figure(60.f, 170.f, 10.0f, 70.0f);
   Figure obstacle16C = Figure(120.f, 100.f, 70.0f, 10.0f);
   Figure obstacle17C = Figure(-40.f, 190.f, 45.0f, 45.0f);
   Figure obstacle18C = Figure(140.f, 200.f, 30.0f, 30.0f);
   
    obstacles.push_back(obstacle1C);
    obstacles.push_back(obstacle2C);
    obstacles.push_back(obstacle3C);
    obstacles.push_back(obstacle4C);
    obstacles.push_back(obstacle5C);
    obstacles.push_back(obstacle6C);
    obstacles.push_back(obstacle7C);
    obstacles.push_back(obstacle9C);
    obstacles.push_back(obstacle8C);
    obstacles.push_back(obstacle10C);
    obstacles.push_back(obstacle11C);
    obstacles.push_back(obstacle12C);
    obstacles.push_back(obstacle13C);
    obstacles.push_back(obstacle14C);
    obstacles.push_back(obstacle15C);
    obstacles.push_back(obstacle16C);
    obstacles.push_back(obstacle17C);
    obstacles.push_back(obstacle18C);

    healthBar = Figure(50,40,8,35);
    health = 1;
    
   


    //Creez mapa
    Mesh* map = Tema1::CreateRectangle("map", glm::vec3(-100, -100, 0),350.0f,300.0f, glm::vec3(128.0/255.0, 96.0/255.0, 67.0/255.0),true);
    AddMeshToList(map);

    Mesh* healthBarVolMesh = Tema1::CreateRectangle("healthBarVol", glm::vec3(0, 0, 2), healthBar.height, healthBar.width, glm::vec3(1,0,0), true);
    AddMeshToList(healthBarVolMesh);

    Mesh* healthBarMesh = Tema1::CreateRectangle("healthBar", glm::vec3(0, 0, 2), healthBar.height, healthBar.width, glm::vec3(1, 0, 0), false);
    AddMeshToList(healthBarMesh);

    //============================================= WALLS ==========================================================

    Mesh* leftWallMesh = Tema1::CreateRectangle("leftWall", glm::vec3(leftWall.x,leftWall.y,1) , leftWall.height, leftWall.width, wallColor, true);
    AddMeshToList(leftWallMesh);

    Mesh* bottomWallMesh = Tema1::CreateRectangle("bottomWall", glm::vec3(bottomWall.x, bottomWall.y, 1), bottomWall.height, bottomWall.width, wallColor, true);
    AddMeshToList(bottomWallMesh);

    Mesh* rightWallMesh = Tema1::CreateRectangle("rightWall", glm::vec3(rightWall.x, rightWall.y, 1), rightWall.height, rightWall.width, wallColor, true);
    AddMeshToList(rightWallMesh);

    Mesh* topWallMesh = Tema1::CreateRectangle("topWall", glm::vec3(topWall.x, topWall.y, 1), topWall.height, topWall.width, wallColor, true);
    AddMeshToList(topWallMesh);

    Mesh* WallMesh = Tema1::CreateRectangle("Wall", glm::vec3(bottomWall.x, bottomWall.y, 1), bottomWall.height, bottomWall.width, wallColor, true);
    AddMeshToList(WallMesh);

    // ============================================= ENEMY -============================================
    Mesh* enemyMesh = Tema1::CreateRectangle("enemy", glm::vec3( 0, 0, 1.2), ENEMY_LEN, ENEMY_LEN, glm::vec3(150.f/255.f, 58.f/255.f, 39.f/255.f), true);
    AddMeshToList(enemyMesh);

    Mesh* enemyLeft = Tema1::CreateRectangle("enemyLeft", glm::vec3(0 - ENEMY_LEN/8, 0 + ENEMY_LEN - ENEMY_LEN/8, 1.4), ENEMY_LEN/4, ENEMY_LEN/4, glm::vec3(77.f/255.f, 36.f/255.f, 28.f/255.f), true);
    AddMeshToList(enemyLeft);

    Mesh* enemyRight = Tema1::CreateRectangle("enemyRight", glm::vec3(0 + ENEMY_LEN - ENEMY_LEN/8, 0 + ENEMY_LEN - ENEMY_LEN / 8, 1.4), ENEMY_LEN/4, ENEMY_LEN/4, glm::vec3(77.f/255.f, 36.f/255.f, 28.f/255.f), true);
    AddMeshToList(enemyRight);
    // ============================================= OBSTACOLE ============================================
    Mesh* obstacle1 = Tema1::CreateRectangle("obstacle1", glm::vec3(obstacle1C.x, obstacle1C.y, 1), obstacle1C.height, obstacle1C.width, color, true);
    AddMeshToList(obstacle1);

    Mesh* obstacle2 = Tema1::CreateRectangle("obstacle2", glm::vec3(obstacle2C.x, obstacle2C.y, 1), obstacle2C.height, obstacle2C.width, color, true);
    AddMeshToList(obstacle2);

    Mesh* obstacle3 = Tema1::CreateRectangle("obstacle3", glm::vec3(obstacle3C.x, obstacle3C.y, 1), obstacle3C.height, obstacle3C.width,color , true);
    AddMeshToList(obstacle3);

    Mesh* obstacle4= Tema1::CreateRectangle("obstacle4", glm::vec3(obstacle4C.x, obstacle4C.y, 1), obstacle4C.height, obstacle4C.width, color, true);
    AddMeshToList(obstacle4);

    Mesh* obstacle5 = Tema1::CreateRectangle("obstacle5", glm::vec3(obstacle5C.x, obstacle5C.y, 1), obstacle5C.height, obstacle5C.width, color, true);
    AddMeshToList(obstacle5);

    Mesh* obstacle6 = Tema1::CreateRectangle("obstacle6", glm::vec3(obstacle6C.x, obstacle6C.y, 1), obstacle6C.height, obstacle6C.width,color , true);
    AddMeshToList(obstacle6);

    Mesh* obstacle7 = Tema1::CreateRectangle("obstacle7", glm::vec3(obstacle7C.x, obstacle7C.y, 1), obstacle7C.height, obstacle7C.width, color, true);
    AddMeshToList(obstacle7);

    Mesh* obstacle9 = Tema1::CreateRectangle("obstacle9", glm::vec3(obstacle9C.x, obstacle9C.y, 1), obstacle9C.height, obstacle9C.width, color, true);
    AddMeshToList(obstacle9);

    Mesh* obstacle8 = Tema1::CreateRectangle("obstacle8", glm::vec3(obstacle8C.x, obstacle8C.y, 1), obstacle8C.height, obstacle8C.width, color, true);
    AddMeshToList(obstacle8);

    Mesh* obstacle10 = Tema1::CreateRectangle("obstacle10", glm::vec3(obstacle10C.x, obstacle10C.y, 1), obstacle10C.height, obstacle10C.width, color, true);
    AddMeshToList(obstacle10);

    Mesh* obstacle11 = Tema1::CreateRectangle("obstacle11", glm::vec3(obstacle11C.x , obstacle11C.y, 1), obstacle11C.height, obstacle11C.width, color, true);
    AddMeshToList(obstacle11);

    Mesh* obstacle12 = Tema1::CreateRectangle("obstacle12", glm::vec3(obstacle12C.x, obstacle12C.y, 1), obstacle12C.height, obstacle12C.width, color, true);
    AddMeshToList(obstacle12);

    Mesh* obstacle13 = Tema1::CreateRectangle("obstacle13", glm::vec3(obstacle13C.x, obstacle13C.y, 1), obstacle13C.height, obstacle13C.width, color, true);
    AddMeshToList(obstacle13);

    Mesh* obstacle14 = Tema1::CreateRectangle("obstacle14", glm::vec3(obstacle14C.x, obstacle14C.y, 1), obstacle14C.height, obstacle14C.width, color, true);
    AddMeshToList(obstacle14);

    Mesh* obstacle16 = Tema1::CreateRectangle("obstacle16", glm::vec3(obstacle16C.x, obstacle16C.y, 1), obstacle16C.height, obstacle16C.width, color, true);
    AddMeshToList(obstacle16);

    Mesh* obstacle15 = Tema1::CreateRectangle("obstacle15", glm::vec3(obstacle15C.x, obstacle15C.y, 1), obstacle15C.height, obstacle15C.width, color, true);
    AddMeshToList(obstacle15);

    Mesh* obstacle17 = Tema1::CreateRectangle("obstacle17", glm::vec3(obstacle17C.x, obstacle17C.y, 1), obstacle17C.height, obstacle17C.width, color, true);
    AddMeshToList(obstacle17);

    Mesh* obstacle18 = Tema1::CreateRectangle("obstacle18", glm::vec3(obstacle18C.x, obstacle18C.y, 1), obstacle18C.height, obstacle18C.width, color, true);
    AddMeshToList(obstacle18);
    // ================================= PROIECTIL====================================================
    Mesh* proiectil = Tema1::CreateRectangle("proiectil", glm::vec3(player.x - PROIECTIL_LEN/2, player.y - PROIECTIL_LEN/2, 1), PROIECTIL_LEN, PROIECTIL_LEN, glm::vec3(0, 0 , 0), true);
    AddMeshToList(proiectil);

    //Desenez primul cerc din jucator
    CreateCircle("circle",Radius,100, glm::vec3(3 / 255.0, 123 / 255.0, 252.0 / 255.0),1);

    CreateCircle("legLeft", Radius/4, 100, glm::vec3(0, 0, 0),2.0);

    CreateCircle("legRight", Radius/4, 100, glm::vec3(0, 0, 0),2.0);

   
    //================== PICK-UP
    Mesh* pickUpMesh = Tema1::CreateRectangle("pickUp", glm::vec3(2, -10, 0.8), PICK_UP_LEN, PICK_UP_LEN, glm::vec3(1, 1, 1), true);
    AddMeshToList(pickUpMesh);


    logicSpace.x = -50;       // logic x
    logicSpace.y = -50;       // logic y
    logicSpace.width = 100;   // logic width
    logicSpace.height = 100;  // logic height

  
}

void Tema1::DrawBullets(glm::mat3 visMatrix, float deltaTimeSeconds) {
    if (showProiectil) {
        //modelMatrix = visMatrix * transform2D::Translate(playerX, playerY);
        for (int i = 0; i < proiectile.size(); i++) {

            bool checkDist = true;

            proiectile[i].x += PROIECTIL_SPEED * cos(proiectile[i].angle) * deltaTimeSeconds;
            proiectile[i].y += PROIECTIL_SPEED * sin(proiectile[i].angle) * deltaTimeSeconds;

            modelMatrix = visMatrix * transform2D::Translate(proiectile[i].x, proiectile[i].y);
            modelMatrix *= transform2D::Rotate(proiectile[i].angle);
            RenderMesh2D(meshes["proiectil"], shaders["VertexColor"], modelMatrix);
            Figure fig = Figure(proiectile[i].x, proiectile[i].y, PROIECTIL_LEN, PROIECTIL_LEN);

            for (int j = 0; j < obstacles.size(); j++) {
                if (intersectRectRect(fig, obstacles[j])) {
                    proiectile.erase(proiectile.begin() + i);
                    checkDist = false;
                    break;
                    
                }
            }

            for (int j = 0; j < enemies.size(); j++) {
                Figure fig2 = Figure(enemies[j].x, enemies[j].y, ENEMY_LEN, ENEMY_LEN);
                if (intersectRectRect(fig, fig2)) {
                    score++;
                    cout << "SCORE : " << score << endl;
                    proiectile.erase(proiectile.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    checkDist = false;
                    break;

                }
            }
            for (int j = 0; j < walls.size(); j++) {
                if (intersectRectRect(fig, walls[j])) {
                    proiectile.erase(proiectile.begin() + i);
                    checkDist = false;
                    break;

                }
            }

            if (checkDist) {
                if (proiectile[i].x - proiectile[i].initialX > 50.f || proiectile[i].y - proiectile[i].initialY > 50.f || proiectile[i].x - proiectile[i].initialX < -50.f || proiectile[i].y - proiectile[i].initialY < -50.f) {
                    proiectile.erase(proiectile.begin() + i);
                }
            }

        }
        
    }
    // ================================== HEALTHBAR ==================================
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(healthBar.x, healthBar.y);
    RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= transform2D::Scale(health, 1);
    RenderMesh2D(meshes["healthBarVol"], shaders["VertexColor"], modelMatrix);

}


bool Tema1::intersectRectRect(Tema1::Figure rect1, Tema1::Figure rect2) { 
    return (rect1.x <= rect2.x + rect2.width && rect1.x + rect1.width >= rect2.x) &&
        (rect1.y <= rect2.y + rect2.height && rect1.y + rect1.height >= rect2.y);
    
}


bool Tema1::intersect(Tema1::Coordonate circle, float radius, Tema1::Figure rect) {
    
    float x = max(rect.x, min(circle.x, rect.x + rect.width));
    float y = max(rect.y, min(circle.y, rect.y + rect.height));
  

    float distance = sqrt((x - circle.x) * (x - circle.x) +
        (y - circle.y) * (y - circle.y));

    return distance < radius;
}

bool Tema1::circleRect(Tema1::Coordonate circle, float radius,Tema1::Figure rect, int key) {

    
    float distance = 0;
    float testX = circle.x;
    float testY = circle.y;
    bool coll = false;
    // which edge is closest?
    
    if (circle.x  < rect.x && circle.y - radius < (rect.y + rect.height) && circle.y + radius > rect.y && key == 3)
    {
        coll = true;
        testX = rect.x;

    } // test left edge
    else if (circle.x > rect.width + rect.x && circle.y - radius < (rect.y + rect.height) && circle.y + radius > rect.y && key == 4) {
        coll = true;
        testX = rect.x + rect.width; 
    }   // right edge
    if (circle.y < rect.y && (circle.x + radius) > rect.x && (circle.x - radius) < (rect.x + rect.width) && key == 1) {
        testY = rect.y;
        coll = true;
    }   // top edge
    else if (circle.y > rect.y + rect.height && (circle.x + radius) > rect.x && (circle.x - radius) < (rect.x + rect.width) && key == 2) {
        testY = rect.y + rect.height;
        coll = true;
    }  // bottom edge

    // get distance from closest edges
    if (coll) {
        float distX = circle.x - testX;
        float distY = circle.y - testY;
        distance = (float)sqrt(pow(distX, 2) + pow(distY, 2));

        if (distance <= radius) {
            return true;
        }
        
    }
    return false;
    // if the distance is less than the radius, collision!
    
}

void Tema1::DrawElements(glm::mat3 visMatrix, float deltaTimeSeconds) {
    //===================================== ENEMY =================================

    for (int i = 0; i < enemies.size(); i++) {
        modelMatrix = visMatrix;

        enemies[i].angle = atan2(player.y - enemies[i].y, player.x - enemies[i].x);

        enemies[i].x += enemies[i].speed * deltaTimeSeconds * cos(enemies[i].angle);



        enemies[i].y += enemies[i].speed * deltaTimeSeconds * sin(enemies[i].angle);

        // ROTATIA
        modelMatrix *= transform2D::Translate(enemies[i].x, enemies[i].y);
        modelMatrix *= transform2D::Translate(ENEMY_LEN / 2, ENEMY_LEN / 2);
        modelMatrix *= transform2D::Rotate(enemies[i].angle - M_PI / 2);
        modelMatrix *= transform2D::Translate(-ENEMY_LEN / 2, -ENEMY_LEN / 2);

  
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);

        RenderMesh2D(meshes["enemyLeft"], shaders["VertexColor"], modelMatrix);

        RenderMesh2D(meshes["enemyRight"], shaders["VertexColor"], modelMatrix);

        Figure enemyFig = Tema1::Figure(enemies[i].x, enemies[i].y, ENEMY_LEN, ENEMY_LEN);
        if (intersect(player, Radius, enemyFig)) {
            health -= 0.1;
            enemies.erase(enemies.begin() + i);
            if (health <= 0.0) {
                cout << "                 YOU LOST!                     " << endl;
                window->Close();
            }
        }

        for (int j = 0; j < walls.size(); j++) {
            if (intersectRectRect(enemyFig, walls[j])) {
                enemies.erase(enemies.begin() + i);
                break;

            }
        }
    }

    //=========================== SPAWNARE ENEMY LA 3 SEC=======================================
    if (Engine::GetElapsedTime() - enemyTime >= 3)
    {
        float x = 0, y = 0;
        float angle = atan2(viewSpace.height / 2 - player.y, player.x - viewSpace.width / 2);
        float xRand = fmod((float(rand())), 30.f);
        float yRand = fmod((float(rand())), 30.f);
        if ((leftOrightSide && player.x - 20.f - xRand > -92.f) || (!leftOrightSide && player.x + 20.f + xRand > 192.f)) {
            x = player.x - 20.f - xRand;
            leftOrightSide = 0;
        }
        else if ((!leftOrightSide && player.x - 20.f - xRand < 192.f) || (leftOrightSide && player.x - 20.f - xRand < -92.f)) {

            x = player.x + 20.f + xRand;
            leftOrightSide = 1;
        }

        if ((leftOrightSide && player.y + 15.f + xRand < 242.f) || (!leftOrightSide && player.y - 15 - yRand < -92.f)) {
            y = player.y + 15 + yRand;

        }
        else if ((!leftOrightSide && player.x - 15.f - xRand > -92.f) || (leftOrightSide && player.y + 15 + yRand > 242.f)) {
            y = player.y - 15 - yRand;
        }
        Enemy enemy = Tema1::Enemy(x, y, 15.f + fmod((float(rand())), 10.f), angle);
        enemies.push_back(enemy);
        enemyTime = Engine::GetElapsedTime();
    }

  

    //================================== PICK-UP ===================================
    for (int i = 0; i < healthPickups.size(); i++) {

        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(healthPickups[i].x, healthPickups[i].y);
        RenderMesh2D(meshes["pickUp"], shaders["VertexColor"], modelMatrix);
        Figure pickFig = Tema1::Figure(healthPickups[i].x, healthPickups[i].y, PICK_UP_LEN, PICK_UP_LEN);
        if (intersect(player, Radius, pickFig)) {

            healthPickups.erase(healthPickups.begin() + i);
            if (health < 1.0) {
                health += 0.1;
            }

        }

    }

    // ================================== GENERARE PICK UP ========================
    if (Engine::GetElapsedTime() - pickUpTime >= 10.0) {
        if (healthPickups.size() < 3) {
            bool ok = false;
            float x, y;
            while (!ok) {
                ok = true;
                x = fmod(rand(), 275.f) - 90.f;
                y = fmod(rand(), 330.f) - 84.f;


                for (int j = 0; j < obstacles.size(); j++) {
                    if (((x >= obstacles[j].x - 2.f && x <= obstacles[j].x + obstacles[j].width + 2.f) || (x + PICK_UP_LEN >= obstacles[j].x - 2.f && x + PICK_UP_LEN <= obstacles[j].x + obstacles[j].width + 2.f))
                        && ((y >= obstacles[j].y - 2.f && y <= obstacles[j].y + obstacles[j].height + 2.f) || (y + PICK_UP_LEN >= obstacles[j].y - 2.f && y + PICK_UP_LEN <= obstacles[j].y + obstacles[j].height + 2.f))) {
                        ok = false;
                        break;
                    }

                }
            }

            Coordonate pick = Tema1::Coordonate(x, y);
            healthPickups.push_back(pick);
            pickUpTime = Engine::GetElapsedTime();
        }
        
    }
}

void Tema1::DrawScene(glm::mat3 visMatrix)
{


    modelMatrix = visMatrix;
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);

    //============================== WALLS =============================

   /* modelMatrix = visMatrix;
    RenderMesh2D(meshes["Wall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    
    modelMatrix *= transform2D::Rotate( 3*M_PI/2);
    modelMatrix *= transform2D::Scale(leftWall.height / bottomWall.width, 1);
    modelMatrix *= transform2D::Translate(-115 , 1);
   
    
 
    RenderMesh2D(meshes["Wall"], shaders["VertexColor"], modelMatrix);*/
    modelMatrix = visMatrix;
    RenderMesh2D(meshes["leftWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["bottomWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["topWall"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["rightWall"], shaders["VertexColor"], modelMatrix);

    // ===================== PLAYER ========================

    modelMatrix = visMatrix * transform2D::Translate(player.x, player.y);
    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(player.x, player.y);
    modelMatrix *= transform2D::Translate(leftLeg.x, leftLeg.y);
    RenderMesh2D(meshes["legLeft"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(player.x, player.y);
    modelMatrix *= transform2D::Translate(rightLeg.x, rightLeg.y);
    RenderMesh2D(meshes["legRight"], shaders["VertexColor"], modelMatrix);

    // ============================ OBSTACOLE ============================
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

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle7"], shaders["VertexColor"], modelMatrix);


    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle8"], shaders["VertexColor"], modelMatrix);


    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle9"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle10"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle11"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle12"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle13"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle14"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle15"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle16"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle17"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle18"], shaders["VertexColor"], modelMatrix);

    
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

glm::mat3 Tema1::VisualizationTransf2DUnif1(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width /(2.5 * logicSpace.width);
    sy = viewSpace.height /(3 * logicSpace.height);
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


    DrawScene(visMatrix);

    DrawElements(visMatrix, deltaTimeSeconds);
    
    DrawBullets(visMatrix, deltaTimeSeconds);
    
    //============ MINIMAP VARIANTA 1 =====================
   // viewSpace1 = ViewportSpace(0, 0, resolution.x /5, resolution.y/4);
   // SetViewportArea(viewSpace1, glm::vec3(0), true);
   // //logicSpace1 = LogicSpace(-98, -98, 300, 350);
   // // Compute uniform 2D visualization matrix
   // visMatrix = glm::mat3(1);
   //// visMatrix *= transform2D::Translate(10, 0);
   // visMatrix *= VisualizationTransf2DUnif1(logicSpace, viewSpace1);
   // DrawScene(visMatrix);
   // DrawElements(visMatrix, deltaTimeSeconds);
   //// DrawBullets(visMatrix, deltaTimeSeconds);

    //========================= MINIMAP VARIANTA 2 ===========================

    viewSpace1 = ViewportSpace(0, 0, resolution.x / 5, resolution.y / 4);
    SetViewportArea(viewSpace1, glm::vec3(0), true);
    logicSpace1 = LogicSpace(-98, -98, 300, 350);
    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    // visMatrix *= transform2D::Translate(10, 0);
    visMatrix *= VisualizationTransf2DUnif(logicSpace1, viewSpace1);
    DrawScene(visMatrix);
    DrawElements(visMatrix, deltaTimeSeconds);
    // DrawBullets(visMatrix, deltaTimeSeconds);


   

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
        
        bool ok = true;
        for (int i = 0; i < obstacles.size(); i++) {
            if (circleRect(player, Radius, obstacles[i], 1)) {
                ok = false;
                break;
            }
        }

        for (int i = 0; i < walls.size(); i++) {
            if (circleRect(player, Radius, walls[i], 1)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            player.y += SPEED * deltaTime;
            logicSpace.y += deltaTime * SPEED;
            healthBar.y += SPEED * deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        ;
        bool ok = true;
        for (int i = 0; i < obstacles.size(); i++) {
            if (circleRect(player, Radius, obstacles[i], 2)) {
                ok = false;
                break;
            }
        }

        for (int i = 0; i < walls.size(); i++) {
            if (circleRect(player, Radius, walls[i], 2)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            player.y -= SPEED * deltaTime;
            logicSpace.y -= deltaTime * SPEED;
            healthBar.y -= SPEED * deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_D)) {

        
        bool ok = true;
        for (int i = 0; i < obstacles.size(); i++) {
            if (circleRect(player, Radius, obstacles[i], 3)) {
                ok = false;
                break;
            }
        }

        for (int i = 0; i < walls.size(); i++) {
            if (circleRect(player, Radius, walls[i], 3)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            player.x += SPEED * deltaTime;
            logicSpace.x += deltaTime * SPEED;
            healthBar.x += SPEED * deltaTime;
        }
        
    }
    if (window->KeyHold(GLFW_KEY_A)) {

        
        bool ok = true;
        for (int i = 0; i < obstacles.size(); i++) {
            if (circleRect(player, Radius, obstacles[i], 4)) {
                ok = false;
                break;
            }
        }

        for (int i = 0; i < walls.size(); i++) {
            if (circleRect(player, Radius, walls[i], 4)) {
                ok = false;
                break;
            }
        }
        
        if (ok) {
            player.x -= SPEED * deltaTime;
            logicSpace.x -= deltaTime * SPEED;
            healthBar.x -= SPEED * deltaTime;
        }

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
    cursorAngle = atan2(viewSpace.height/2  - mouseY, mouseX - viewSpace.width/2);
    leftLeg.x = Radius * cos(cursorAngle + M_PI/4);
    leftLeg.y = Radius * sin(cursorAngle + M_PI/4);
    rightLeg.x = Radius * cos(cursorAngle - M_PI/4);
    rightLeg.y = Radius * sin(cursorAngle - M_PI/4);

    
    

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
