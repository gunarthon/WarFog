#include "Draw.h"

Draw::Draw(sf::RenderWindow &Window)
{
    window = &Window;
    s_jointColor = b2Color(0.4, 0.8, 0.8);
    s_selectedColor = b2Color(0.9, 0.4, 0.4);
    s_normalColor = b2Color(0.5, 0.5, 0.5);
    s_uneditableColor = b2Color(0.6, 0.6, 0.6);
    s_jointCreatingColor = b2Color(0.4, 0.4, 0.9);
    s_staticColor = b2Color(0.4, 0.9, 0.4);
    s_staticEditableColor = b2Color(0.6, 0.8, 0.6);

    font.loadFromFile("Carlito-Bold.ttf");
    background.loadFromFile("background.png");
    background.setRepeated(true);
    background.setSmooth(true);

    selectedTexture.loadFromFile("selectedTexture.png");
    selectedTexture.setRepeated(true);
    selectedTexture.setSmooth(true);

    selectedShader.loadFromFile("light.frag", sf::Shader::Fragment);
    selectedShader.setParameter("texture", sf::Shader::CurrentTexture);

    challengeShader.loadFromFile("challengeBox.frag", sf::Shader::Fragment);
    challengeShader.setParameter("texture", sf::Shader::CurrentTexture);

    message.setFont(font);
    message.setCharacterSize(12);

    // Choose a color
    message.setColor(sf::Color::White);
    std::stringstream howto;
    howto << "How to play:" << std::endl << std::endl;
    howto << " - mouse scroll to zoom" << std::endl;
    howto << " - arrow keys to move camera" << std::endl;
    howto << " - delete to erase shape" << std::endl;
    howto << " - Ctrl+C to copy" << std::endl;
    howto << " - Ctrl+X to cut" << std::endl;
    howto << " - Ctrl+V to paste" << std::endl;
    howto << " - Page Up to move to front" << std::endl;
    howto << " - Page Down to move to back" << std::endl;

    howto << std::endl;
    howto << " - F5 to save design" << std::endl;
    howto << " - F6 to load design" << std::endl;

    howto << std::endl;
    howto << " - space to start/stop simulation" << std::endl;

    howto << std::endl;
    howto << " - A and D to control motor" << std::endl;
    howto << " - W to activate thrusters" << std::endl;

    message.setString(howto.str());

    m_world = NULL;
}

Draw::~Draw()
{

}

void Draw::Clear()
{
    window->clear();
}

void Draw::ChangeResolution(String newRes)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    if(newRes == "FULLSCREEN")
    {
        window->create(sf::VideoMode::getFullscreenModes()[0], "Gambearra", sf::Style::Fullscreen, settings);
    }
    else
        window->create(sf::VideoMode(800, 800), "Gambearra", sf::Style::Default, settings);
}

void Draw::DrawWorld(Camera *camera, PartList *allParts, b2World *world, bool notStarted,
       bool drawStatic, bool showJoints, bool showOutlines, Challenge *challenge)
{

    window->setView(camera->view);
    sf::Sprite sprite;
    sprite.setTexture(background);
    sprite.setOrigin(background.getSize().x / 2 - 60,background.getSize().y /2 + 50);
    window->draw(sprite);

    message.setPosition(0, -6);
    message.setScale(0.03f, 0.03f);
    window->draw(message);

    /*sf::Sprite mario;
    mario.setTexture(image);
    sprite.setOrigin(image.getSize().x /2, image.getSize().y /2);
    window->draw(mario, &selectedShader);*/

    for(unsigned i = 0; i < allParts->drawOrder.size(); i++)
    {
        Part *part = allParts->At(allParts->drawOrder[i]);

        if(notStarted)
        {
            if(allParts->InsideGroup(part->id, &allParts->selected) || allParts->InsideGroup(part->id, &allParts->temp))
                part->Draw(window, camera, drawStatic, showJoints, showOutlines, &selectedTexture, &selectedShader);
            else
                part->Draw(window, camera, drawStatic, showJoints, showOutlines);
        }
        else
            part->Draw(window, camera, drawStatic, showJoints, showOutlines);

    }
}

void Draw::DrawInterface(InterfaceManager *interface)
{
    interface->Draw(window, &font);
}

void Draw::DrawJoint(b2Joint *joint)
{

}

void Draw::DrawShape(b2Shape *shape,/* b2XForm *xf,*/ b2Color color, Num alpha, bool showOutlines, bool cannonball)
{

}

void Draw::DrawCannon(b2Shape *shape,/* b2XForm *xf,*/ b2Color color, Num alpha, bool showOutlines)
{

}

void Draw::DrawShapeForOutline(b2Shape *shape,/* b2XForm *xf,*/ b2Color color, Num alpha)
{

}

void Draw::DrawCannonForOutline(b2Shape *shape,/* b2XForm *xf,*/ b2Color color, Num alpha)
{

}

void Draw::DrawTempShape(int creatingItem, int actionStep, Num firstClickX, Num firstClickY, Num secondClickX, Num secondClickY, Num mouseX, Num mouseY)
{

}
