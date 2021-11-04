#pragma once

#include "components/simple_scene.h"
//#include "2Dobjects.h"
#include "Constants.h"
#include <vector>



namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    public: 
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct Proiectil
        {
            Proiectil (float x, float y, float angle, float initialX, float initialY)
                //: x(x), y(y), angle(angle), initialX(initialX), initialY(initialY) {}

            {
                this->x = x;
                this->y = y;
                this->angle = angle;
                this->initialX = initialX;
                this->initialY = initialY;
               
            }
            float x;
            float y;
            float angle;
            float initialX;
            float initialY;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        struct Player
        {
            Player(): x(0), y(0) {}
            Player(float x, float y)
                : x(x), y(y) {}
            float x;
            float y;
        };
    private:
        
       
        void Tema1::CreateCircle(const char* name,int R, int nr_sections,glm::vec3 color, int Zoffset);
        void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);
        void Tema1::DrawBullets(glm::mat3 visMatrix, float deltaTimeSeconds);
        Mesh* CreateRectangle(const std::string& name,glm::vec3 leftBottomCorner,float height,float width, glm::vec3 color, bool fill);
        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
     
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        std::vector<Proiectil> proiectile;
        glm::mat3 modelMatrix, visMatrix;
        Player  player;


        GLenum polygonMode;

        float time;

        
        float px, py;
        int nrProiectile;
        float proiectilAngleX, proiectilAngleY;
        int showProiectil;
        float proiectilX, proiectilY;
        float proiectilInitialX, proiectilInitialY;
        float playerX, playerY;
        float leftLegX, leftLegY, rightLegX, rightLegY;
        float alfa;
        float cursorAngle;
        float Radius;
        float beta, N;
    };
}   // namespace m1
