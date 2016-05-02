#ifndef CONTROLLERGAME_H
#define CONTROLLERGAME_H

#define REPLAY_SYNC_FRAMES 3
#define INIT_PHYS_SCALE 30

#include "Draw.h"
#include "ShapePart.h"
#include "Rectangle.h"
#include "Circle.h"
#include "FixedJoint.h"
#include "RevoluteJoint.h"
#include "Utils.h"
#include "Controller.h"
#include "Camera.h"
#include "InputManager.h"
#include "InterfaceManager.h"
#include "PartList.h"
#include "PartsGUI.h"
#include "SaveGUI.h"
#include "LoadGUI.h"
#include "MainMenuGUI.h"
#include "ToolsGUI.h"
#include "DeveloperGUI.h"
#include "ChallengeEndGUI.h"
#include "ChallengeBox.h"
#include "EndBox.h"
#include "Cable.h"

typedef std::vector<ActionId> ActionList;
typedef std::vector<sf::Vector2f> PositionList;

class ControllerGame : public Controller
{
    public:
        explicit ControllerGame(sf::RenderWindow &Window);
        virtual ~ControllerGame();
        virtual void Init();
        virtual void Update();
        virtual void DrawAll();
        void HandleCamera();
        void AddSyncPoint();
        void PlayButton();
        void StopButton();
        void RetryButton();

        bool KeyPress(sf::Keyboard::Key key);
        bool MouseDown(sf::Mouse::Button button, Num x, Num y);
        bool MouseUp(sf::Mouse::Button button, Num x, Num y);
        bool MouseWheel(sf::Vector2f pos, Num delta);
        bool MouseMove(Num x, Num y);
        bool TextEntered(char text);

        bool paused;
        bool playingReplay;
        bool simStarted;
        bool showJoints;
        bool showOutlines;

		PartList allParts;

		ActionList actions;
        int frameCounter;
        int m_vIterations;
        int m_pIterations;

        ActionId currAction;
        unsigned actionStep;
        int mouseClicksN;
        int partsN;

        Draw *draw;
        b2World *m_world;
        Camera *camera;

        GUI *proprietiesInterface;
        GUI *saveInterface;
        GUI *loadInterface;
        GUI *mainMenuInterface;
        GUI *toolsInterface;
        GUI *devInterface;
        ChallengeEndGUI *challengeEndInterface;
        Proprieties devProprieties;


        Num mouseXWorldPhys;
        Num mouseYWorldPhys;
        PositionList mouseClicks;
        sf::Vector2f lastMousePos;

        sf::Vector2f rotateCenter;

        sf::Vector2f cameraPos;
        Num cameraScale;

    protected:
        virtual b2Vec2 GetGravity();
        virtual unsigned FindCandidateParts(sf::Vector2f pos, bool shapeOnly);
        virtual unsigned FindIntersectingParts(sf::Vector2f pos0, sf::Vector2f pos1, Group *group);
        virtual sf::Vector2f FindSnapPoint(sf::Vector2f pos);
        virtual void UpdateTempParts();
        virtual void ChangeAction(ActionId id);
        virtual void ChangeFocus(GUI *gui = NULL);

        virtual void SaveDesign(String fileName);
        virtual void LoadDesign(String fileName);
        virtual void ExitGame();
        virtual void ShowEnd(bool win);
    private:

		void CreateWorld();
};

#endif // CONTROLLERGAME_H
