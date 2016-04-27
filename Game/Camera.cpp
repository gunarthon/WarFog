#include "Camera.h"
#include "ShapePart.h"

Camera::Camera(sf::RenderWindow *window, PartList *allParts)
{
    Init();
    view = window->getView();
    this->window = window;
    this->allParts = allParts;
}

Camera::~Camera()
{
    //dtor
}

void Camera::Init()
{
    pos = sf::Vector2f(0,0);
    speed = sf::Vector2f(0,0);
    acc = 1.0f;
    friction = 0.9f;

    scale = 1.0f;
    zoomSpeed = 0;
    zoomAcc = 0.05f;
    zoomFriction = 0.8f;
    //partToFollow = NULL;
    follow = false;
}

void Camera::SetPos(sf::Vector2f pos)
{
    this->pos = pos;
}

sf::Vector2f Camera::GetPos()
{
    return pos;
}

void Camera::SetScale(Num scale)
{
    this->scale = scale;
}

Num Camera::GetScale()
{
    return scale;
}

void Camera::SetPartToFollow(unsigned id)
{
    partToFollow.clear();
    partToFollow.push_back(id);
}

void Camera::Move(sf::Vector2f dir)
{
    speed += -dir * acc / (scale);
}

void Camera::Update()
{
    speed *= friction;
    pos += speed;

    zoomSpeed *= zoomFriction;
    scale += zoomSpeed * scale;


    if(scale < ZOOM_MIN)
        scale = ZOOM_MIN;
    else if(scale > ZOOM_MAX)
        scale = ZOOM_MAX;

    sf::Vector2f zoomPosOffset = ToWorldPos(beforeZoomCoord);
    view.setSize(sf::Vector2f(window->getSize().x, window->getSize().y) / scale);
    zoomPosOffset -= ToWorldPos(beforeZoomCoord);

    if(zoomSpeed > 0)
    {
        pos += zoomPosOffset;
    }

    if(follow && partToFollow.size())
    {
        ShapePart *part = (ShapePart*) allParts->At(partToFollow[0]);
        if(part)
        {
            b2Body *body = part->GetBody();
            if(body)
            {
                pos = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
            }
            else
                pos = sf::Vector2f(part->centerX, part->centerY);
        }
        else
            pos = sf::Vector2f(part->centerX, part->centerY);
    }

    view.setCenter(pos);
}

void Camera::Zoom(Num delta)
{
    zoomSpeed += delta * zoomAcc;
}

void Camera::ZoomAt(sf::Vector2f pos, Num delta)
{
    Zoom(delta);
    beforeZoomCoord = pos;
}

void Camera::SetFollow(bool follow)
{
    this->follow = follow;
}

sf::Vector2f Camera::ToWorldPos(sf::Vector2f pos)
{
    // convert it to world coordinates
    return window->mapPixelToCoords(sf::Vector2i(pos.x, pos.y), view);
}

sf::Vector2f Camera::ToScreenPos(sf::Vector2f pos)
{
    return sf::Vector2f(window->mapCoordsToPixel(pos, view).x, window->mapCoordsToPixel(pos, view).y);
}
