#ifndef CAMERA_H
#define CAMERA_H

#include "Utils.h"
#include "Part.h"

#define ZOOM_MIN 1
#define ZOOM_MAX 300

class Camera
{
    public:
        explicit Camera(sf::RenderWindow *window, PartList *allParts);
        virtual ~Camera();
        void Init();
        void SetPos(sf::Vector2f pos);
        sf::Vector2f GetPos();
        void SetScale(Num scale);
        Num GetScale();
        void Move(sf::Vector2f dir);
        void Zoom(Num delta);
        void ZoomAt(sf::Vector2f pos, Num delta);
        void Update();
        void SetPartToFollow(unsigned id);
        void SetFollow(bool follow);

        sf::Vector2f ToWorldPos(sf::Vector2f pos);
        sf::Vector2f ToScreenPos(sf::Vector2f pos);
        sf::View view;

    protected:
        sf::Vector2f pos;
        sf::Vector2f speed;

        Num acc;
        Num scale;
        Num friction;
        Num zoomSpeed;
        Num zoomAcc;
        Num zoomFriction;
        sf::RenderWindow *window;
        PartList *allParts;
        Group partToFollow;
        sf::Vector2f beforeZoomCoord;
        bool follow;

    private:
};

#endif // CAMERA_H
