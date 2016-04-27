#include "ControllerGame.h"

ControllerGame::ControllerGame(sf::RenderWindow &Window)
{
    draw = new Draw(Window);
    camera = new Camera(&Window, &allParts);
    camera->SetScale(50);
    m_world = NULL;

    paused = false;
    playingReplay = false;
    simStarted = false;
    showJoints = true;
    showOutlines = true;

    frameCounter = 0;
    m_vIterations = 10;
    m_pIterations = 1; //TODO: check if good

    devProprieties.list.push_back(new Propriety("currAction", true, (int*) &currAction));
    devProprieties.list.push_back(new Propriety("actionStep", true, (int*) &actionStep));
    devProprieties.list.push_back(new Propriety("mouseClicks", true, (int*) &mouseClicksN));
    devProprieties.list.push_back(new Propriety("parts", true, (int*) &partsN));

    String title = "Proprieties";
    proprietiesInterface = new PartsGUI(title, &allParts);
    title = "Tools";
    toolsInterface = new ToolsGUI(title, [this](ActionId s) {return this->ChangeAction(s); });
    title = "Save Design";
    saveInterface = new SaveGUI(title, [this](String s) {return this->SaveDesign(s); });
    title = "Load Design";
    loadInterface = new LoadGUI(title, [this](String s) {return this->LoadDesign(s); });
    title = "Menu";
    mainMenuInterface = new MainMenuGUI(title, [this]() {return this->ExitGame(); },
                                                [this]() {return this->ChangeFocus(saveInterface);},
                                                [this]() {return this->ChangeFocus(loadInterface);},
                                                [&](String s) {return draw->ChangeResolution(s); });
    title = "Developer GUI";
    devInterface = new DeveloperGUI(title, &devProprieties);

    interface.list.push_back(proprietiesInterface);
    interface.list.push_back(saveInterface);
    interface.list.push_back(loadInterface);
    interface.list.push_back(mainMenuInterface);
    interface.list.push_back(toolsInterface);
    interface.list.push_back(devInterface);

    interface.Show(toolsInterface);
    interface.Show(devInterface);
    interface.Show(mainMenuInterface);

    ChangeAction(DEFAULT_ACTION);
    actionStep = 0;

    Rectangle *p = new Rectangle(-250, 6.25, 500, 12.5, false);
					p->isStatic = true;
					p->isEditable = false;
					p->drawAnyway = false;
					p->isSandbox = true;
					p->color = Color(165, 216, 40, 255);

    allParts.Add(p);
    Circle *c = new Circle(sf::Vector2f(-250,12.5), 6.25, false);
					c->isStatic = true;
					c->isEditable = false;
					c->drawAnyway = false;
					c->isSandbox = true;
					c->color = Color(165, 216, 40, 255);
    allParts.Add(c);
    c = new Circle(sf::Vector2f(250,12.5), 6.25, false);
					c->isStatic = true;
					c->isEditable = false;
					c->drawAnyway = false;
					c->isSandbox = true;
					c->color = Color(165, 216, 40, 255);
    allParts.Add(c);
    p = new Rectangle(-250, 6.25, 500, 12.5, false);
					p->isStatic = true;
					p->isEditable = false;
					p->drawAnyway = false;
					p->isSandbox = true;
					p->outline = false;
					p->collide = false;
					p->color = Color(165, 216, 40, 255);

    allParts.Add(p);
    p = new Rectangle(-251, 10.25, 502, 8.5, false);
					p->isStatic = true;
					p->isEditable = false;
					p->drawAnyway = false;
					p->isSandbox = true;
					p->outline = false;
					p->collide = false;
					p->color = Color(81, 122, 35, 50);

    allParts.Add(p);
    for(unsigned i = 0; i < 500; i++)
    {
        Num x = p->x - 1.5 + (Num) (std::rand() % (int) ((p->w+3) * 100)) /64.6;
        Num y = 6.25 + (Num) (std::rand() % (int) (100*p->h)) /100;
        Rectangle *d = new Rectangle(x, y + 0.3f, 0.5, 0.5, false);
        d->isStatic = true;
        d->isEditable = false;
        d->drawAnyway = false;
        d->isSandbox = true;
        d->outline = false;
        d->color = Color(165 + rand() % 20, 216 + rand() % 20, 40 + rand() % 20, 255);

        allParts.Add(d);

    }

    p = new Rectangle(-250, 16.25, 500, 2.5, false);
					p->isStatic = true;
					p->isEditable = false;
					p->drawAnyway = false;
					p->isSandbox = true;
					p->outline = false;
					p->collide = false;
					p->color = Color(81, 122, 35, 50);

    allParts.Add(p);
}

ControllerGame::~ControllerGame()
{
    delete draw;

    delete camera;

    delete m_world;
}

void ControllerGame::Init()
{

}

void ControllerGame::Update()
{
    HandleCamera();
    interface.Update();
    mouseClicksN = mouseClicks.size();
    partsN = allParts.list.size();
    //update physics
    if(!paused)
    {
        if(!playingReplay) {
            /*if(frameCounter % REPLAY_SYNC_FRAMES == 0)
                AddSyncPoint();*/
        }
        frameCounter++;
        //m_guiPanel.SetTimer(frameCounter);

        /*if(hasPanned || hasZoomed || !paused || draggingPart || curAction != -1 || redrawRobot)
        {*/


            if(simStarted)
            {
                m_world->Step(1.0f/60, m_vIterations, m_pIterations);
                for(PartList::It i = allParts.list.begin(); i != allParts.list.end(); i++)
                    i->second->Update(m_world, input);
            }
            else
            {
            }

            //redrawRobot = false;
        //}
    }
}

void ControllerGame::DrawAll()
{
    if(!paused)
    {
        draw->Clear();
        draw->DrawWorld(camera, &allParts, m_world, !simStarted, true, simStarted ? false : showJoints, showOutlines, NULL);
            //m_world->DrawDebugData();
        if(!simStarted)
        {
            draw->DrawInterface(&interface);
        }
    }
}

void ControllerGame::HandleCamera()
{
    sf::Vector2f dir = sf::Vector2f(0,0);

    if(input->KeyDown(sf::Keyboard::Up))
        dir += sf::Vector2f(0,1);
    if(input->KeyDown(sf::Keyboard::Down))
        dir += sf::Vector2f(0,-1);
    if(input->KeyDown(sf::Keyboard::Left))
        dir += sf::Vector2f(1,0);
    if(input->KeyDown(sf::Keyboard::Right))
        dir += sf::Vector2f(-1,0);

    if(dir != sf::Vector2f(0,0))
        camera->SetFollow(false);
    static Utils utils;

    camera->Move(utils.Normalize(dir));
    camera->Update();
}

void ControllerGame::AddSyncPoint()
{

}


void ControllerGame::CreateWorld()
{
    // Construct a world object
    m_world = new b2World(GetGravity());
    m_world->SetDebugDraw(draw);
    draw->SetFlags(b2Draw::e_shapeBit);

    /*ContactFilter *filter = new ContactFilter();
    m_world->SetContactFilter(filter);
    ContactListener *listener = new ContactListener(this);
    m_world->SetContactListener(listener);*/
}

void ControllerGame::PlayButton()
{
    allParts.Delete(&allParts.temp);
    CreateWorld();
    paused = false;
    simStarted = true;
    playingReplay = false;
    cameraPos = camera->GetPos();
    cameraScale = camera->GetScale();
    camera->SetFollow(false);

    for(PartList::RIt i = allParts.list.rbegin(); i != allParts.list.rend(); ++i)
    {
        if(i->second->isCameraFocus)
        {
            camera->SetPartToFollow(i->first);
            camera->SetFollow(true);
            break;
        }
    }

    ChangeFocus();
    ChangeAction(DEFAULT_ACTION);


    for(PartList::It i = allParts.list.begin(); i != allParts.list.end(); i++)
        i->second->Init(m_world);
}

void ControllerGame::StopButton()
{
    paused = false;
    simStarted = false;
    playingReplay = false;
    camera->SetFollow(false);
    camera->SetPos(cameraPos);
    camera->SetScale(cameraScale);
    ChangeAction(DEFAULT_ACTION);

    for(PartList::It i = allParts.list.begin(); i != allParts.list.end(); i++)
        i->second->UnInit(m_world);
}

b2Vec2 ControllerGame::GetGravity()
{
    return b2Vec2(0.0, 15.0);
}

bool ControllerGame::MouseDown(sf::Mouse::Button button, Num x, Num y)
{
    if(Controller::MouseDown(button, x, y))
        return true;
    if(button != sf::Mouse::Left)
        return false;

    mouseClicks.push_back(camera->ToWorldPos(sf::Vector2f(x, y)));

    if(currAction == DEFAULT_ACTION)
    {

        if(FindCandidateParts(mouseClicks.back(), false) >= 1)
        {
            bool dragging = false;
            for(int i = allParts.selected.size()-1; i >= 0; --i)
            {
                if(allParts.selected[i] == allParts.candidate[0])
                {
                    if(input->KeyDown(sf::Keyboard::LShift))
                        allParts.selected.erase(allParts.selected.begin()+i);

                    dragging = true;
                    break;
                }
            }
            if(!dragging)
            {
                if(!input->KeyDown(sf::Keyboard::LShift))
                    allParts.selected.clear();
                allParts.selected.push_back(allParts.candidate[0]);
                interface.Show(proprietiesInterface);
            }

            ChangeAction(DEFAULT_ACTION);
        }
        else
        {
            ChangeAction(BOX_SELECTING);
            mouseClicks.push_back(camera->ToWorldPos(sf::Vector2f(x, y)));
        }

    }
    if(currAction == BOX_SELECTING)
    {
        if(actionStep == 0)
        {
            UpdateTempParts();
            actionStep++;
        }
    }
    else if(currAction == NEW_RECT)
    {
        if(actionStep == 0)
        {
            UpdateTempParts();
            actionStep++;
        }
        else if(actionStep == 1)
        {
            if(mouseClicks.size() >= 2)
            {
                Rectangle *rect = new Rectangle(mouseClicks[0].x, mouseClicks[0].y,
                                        mouseClicks[1].x - mouseClicks[0].x, mouseClicks[1].y - mouseClicks[0].y);

                allParts.selected.clear();
                allParts.Add(rect, &allParts.selected);
                ChangeFocus(proprietiesInterface);

                ChangeAction(DEFAULT_ACTION);
            }
            else
            {
                mouseClicks.pop_back();
                actionStep = 1;
            }
        }
    }
    else if(currAction == NEW_CIRCLE)
    {
        if(actionStep == 0)
        {
            UpdateTempParts();
            actionStep++;
        }
        else if(actionStep == 1)
        {
            if(mouseClicks.size() >= 2)
            {
                Circle *cir = new Circle(sf::Vector2f(mouseClicks[0].x, mouseClicks[0].y),
                                        utils.GetLength(sf::Vector2f(mouseClicks[1] - mouseClicks[0])));

                allParts.selected.clear();
                allParts.Add(cir, &allParts.selected);
                ChangeFocus(proprietiesInterface);

                ChangeAction(DEFAULT_ACTION);
            }
            else
            {
                mouseClicks.pop_back();
                actionStep = 1;
            }
        }
    }
    else if(currAction == NEW_FIXED_JOINT)
    {
        if(FindCandidateParts(FindSnapPoint(mouseClicks[0]), true) >= 2)
        {
            FixedJoint *f = new FixedJoint((ShapePart*) allParts.At(allParts.candidate[0]),
                                        (ShapePart*) allParts.At(allParts.candidate[1]),
                                           FindSnapPoint(mouseClicks[0]));

            allParts.selected.clear();
            allParts.Add(f, &allParts.selected);
            ChangeFocus(proprietiesInterface);

            ChangeAction(DEFAULT_ACTION);
        }
        else
            ChangeAction(NEW_FIXED_JOINT);
    }
    else if(currAction == NEW_REVOLUTE_JOINT)
    {
        if(FindCandidateParts(FindSnapPoint(mouseClicks[0]), true) >= 2)
        {
            RevoluteJoint *r = new RevoluteJoint((ShapePart*) allParts.At(allParts.candidate[0]),
                                                 (ShapePart*) allParts.At(allParts.candidate[1]),
                                                 FindSnapPoint(mouseClicks[0]));
            allParts.selected.clear();
            allParts.Add(r, &allParts.selected);
            ChangeFocus(proprietiesInterface);

            ChangeAction(DEFAULT_ACTION);
        }
        else
            ChangeAction(NEW_REVOLUTE_JOINT);
    }
    else if(currAction == NEW_THRUSTER)
    {
        if(FindCandidateParts(FindSnapPoint(mouseClicks[0]), true) >= 1)
        {
            Thruster *t = new Thruster((ShapePart*) allParts.At(allParts.candidate[0]),
                                                 FindSnapPoint(mouseClicks[0]));

            allParts.selected.clear();
            allParts.Add(t, &allParts.selected);
            ChangeFocus(proprietiesInterface);

            ChangeAction(DEFAULT_ACTION);
        }
        else
            ChangeAction(NEW_THRUSTER);
    }
    else if(currAction == NEW_CHALLENGE_BOX)
    {
        if(actionStep == 0)
        {
            UpdateTempParts();
            actionStep++;
        }
        else if(actionStep == 1)
        {
            if(mouseClicks.size() >= 2)
            {
                ChallengeBox *box = new ChallengeBox(mouseClicks[0], mouseClicks[1] - mouseClicks[0]);

                allParts.selected.clear();
                allParts.Add(box, &allParts.selected);
                ChangeFocus(proprietiesInterface);

                ChangeAction(DEFAULT_ACTION);
            }
            else
            {
                mouseClicks.pop_back();
                actionStep = 1;
            }
        }
    }
    return false;
}

bool ControllerGame::MouseUp(sf::Mouse::Button button, Num x, Num y)
{
    if(Controller::MouseUp(button, x, y))
        return true;

    if(currAction == BOX_SELECTING)
    {

        if(actionStep >= 1)
        {
            if(mouseClicks.size() >= 1)
            {
                mouseClicks.push_back(camera->ToWorldPos(sf::Vector2f(x, y)));
                if(!input->KeyDown(sf::Keyboard::LShift))
                    allParts.selected.clear();
                FindIntersectingParts(mouseClicks[0], mouseClicks[1], &allParts.selected);

                ChangeAction(DEFAULT_ACTION);
            }
            else
            {
                mouseClicks.pop_back();
                actionStep = 1;
            }
        }
    }
    return false;
}

bool ControllerGame::MouseMove(Num x, Num y)
{
    if(Controller::MouseMove(x, y))
        return true;

    if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
    {
        camera->SetPos(camera->GetPos() - camera->ToWorldPos(sf::Vector2f(x,y)) + camera->ToWorldPos(lastMousePos));
        camera->SetFollow(false);
    }

    if(!simStarted)
    {
        UpdateTempParts();
        if(currAction == DEFAULT_ACTION)
        {
            if(mouseDown)
            {
                bool dragScreen = true;
                Group attached;
                for(unsigned i = 0; i < allParts.selected.size(); ++i)
                    if(allParts.At(allParts.selected[i])->InsideShape(camera->ToWorldPos(lastMousePos), camera->GetScale(), false))
                        dragScreen = false;

                if(!dragScreen)
                {
                    for(unsigned i = 0; i < allParts.selected.size(); ++i)
                        allParts.At(allParts.selected[i])->GetAttachedParts(&attached, true);
                    for(GroupIt i = attached.begin(); i != attached.end(); i++)
                        allParts.At(*i)->Move(camera->ToWorldPos(sf::Vector2f(x,y)) - camera->ToWorldPos(lastMousePos));
                }
            }
        }
    }
    lastMousePos = sf::Vector2f(x,y);
    return false;
}

bool ControllerGame::KeyPress(sf::Keyboard::Key key)
{
    if(Controller::KeyPress(key))
        return true;

    if(key == sf::Keyboard::Space)
    {
        if(!simStarted)
            PlayButton();
        else
            StopButton();

        ChangeAction(DEFAULT_ACTION);
        ChangeFocus();
    }
    else if(key == sf::Keyboard::Escape)
    {
        if(simStarted)
            StopButton();
        if(currAction == DEFAULT_ACTION && allParts.selected.size() == 0)
            ChangeFocus(mainMenuInterface);
        ChangeAction(DEFAULT_ACTION);
        allParts.selected.clear();
    }
    else if(key == sf::Keyboard::Up || key == sf::Keyboard::Down || key == sf::Keyboard::Left ||key == sf::Keyboard::Right)
    {
        //camera movement
    }

    if(!simStarted)
    {
        if(key == sf::Keyboard::Num0 || key == sf::Keyboard::Numpad0)
        {
            ChangeAction(BOX_SELECTING);
        }
        else if(key == sf::Keyboard::Num1 || key == sf::Keyboard::Numpad1)
        {
            ChangeAction(NEW_RECT);
        }

        else if(key == sf::Keyboard::Num2 || key == sf::Keyboard::Numpad2)
        {
            ChangeAction(NEW_CIRCLE);
        }

        else if(key == sf::Keyboard::Num4 || key == sf::Keyboard::Numpad4)
        {
            ChangeAction(NEW_FIXED_JOINT);
        }

        else if(key == sf::Keyboard::Num5 || key == sf::Keyboard::Numpad5)
        {
            ChangeAction(NEW_REVOLUTE_JOINT);
        }
        else if(key == sf::Keyboard::Num6 || key == sf::Keyboard::Numpad6)
        {
            ChangeAction(NEW_THRUSTER);
        }
        else if(key == sf::Keyboard::Num9 || key == sf::Keyboard::Numpad9)
        {
            ChangeAction(NEW_CHALLENGE_BOX);
        }
        else if(key == sf::Keyboard::Delete)
        {
            if(!simStarted)
            {
                allParts.Delete(&allParts.selected);
            }
            ChangeAction(DEFAULT_ACTION);
        }
        else if(key == sf::Keyboard::F5)
        {
           ChangeFocus(saveInterface);
        }
        else if(key == sf::Keyboard::F6)
        {
           ChangeFocus(loadInterface);
        }
        else if(key == sf::Keyboard::C && (input->KeyDown(sf::Keyboard::LControl) || input->KeyDown(sf::Keyboard::RControl)))
        {
            allParts.Copy(&allParts.selected);
        }
        else if(key == sf::Keyboard::X && (input->KeyDown(sf::Keyboard::LControl) || input->KeyDown(sf::Keyboard::RControl)))
        {
            allParts.Copy(&allParts.selected);
            allParts.Delete(&allParts.selected);
        }
        else if(key == sf::Keyboard::V && (input->KeyDown(sf::Keyboard::LControl) || input->KeyDown(sf::Keyboard::RControl)))
        {
            allParts.Paste();
        }
        else if(key == sf::Keyboard::PageDown)
        {
            allParts.MoveToBack(&allParts.selected);
        }
        else if(key == sf::Keyboard::PageUp)
        {
            allParts.MoveToFront(&allParts.selected);
        }
        UpdateTempParts();
    }
    return false;
}

bool ControllerGame::MouseWheel(sf::Vector2f pos, Num delta)
{
    Controller::MouseWheel(pos, delta);
    camera->ZoomAt(pos, delta);

    UpdateTempParts();
}

bool ControllerGame::TextEntered(char text)
{
    if(Controller::TextEntered(text))
        return true;
    return false;
}

void ControllerGame::UpdateTempParts()
{
    if(simStarted)
        return;

    int a = allParts.temp.size();
    if(a)
        allParts.Delete(&(allParts.temp));

   // if(current && (current != interface))
   //     return;

    if(currAction == BOX_SELECTING)
    {
        if(mouseClicks.size() == 1)
        {
            Rectangle *box = new Rectangle(mouseClicks[0].x, mouseClicks[0].y,
                    camera->ToWorldPos(sf::Vector2f(mouseX, mouseY)).x - mouseClicks[0].x,
                    camera->ToWorldPos(sf::Vector2f(mouseX, mouseY)).y - mouseClicks[0].y, false);
                    box->color = Color(255,234,241,100);
                    box->outline = false;
            allParts.Add(box, &allParts.temp);
        }
    }
    else if(currAction == NEW_RECT)
    {
        if(mouseClicks.size() == 1)
        {
            allParts.Add(new Rectangle(mouseClicks[0].x, mouseClicks[0].y,
                    camera->ToWorldPos(sf::Vector2f(mouseX, mouseY)).x - mouseClicks[0].x,
                    camera->ToWorldPos(sf::Vector2f(mouseX, mouseY)).y - mouseClicks[0].y),
                    &allParts.temp);
        }
    }
    else if(currAction == NEW_CIRCLE)
    {
        if(mouseClicks.size() == 1)
        {
            allParts.Add(new Circle(mouseClicks[0], utils.GetLength(
                    camera->ToWorldPos(sf::Vector2f(mouseX, mouseY)) - mouseClicks[0])),
                    &allParts.temp);
        }
    }
    else if(currAction == NEW_FIXED_JOINT)
    {
        bool valid = (FindCandidateParts(FindSnapPoint(camera->ToWorldPos(sf::Vector2f(mouseX, mouseY))), true) >= 2);
        allParts.Add(new FixedJoint(NULL, NULL, FindSnapPoint(camera->ToWorldPos(sf::Vector2f(mouseX, mouseY))), valid),
                      &allParts.temp);
    }
    else if(currAction == NEW_REVOLUTE_JOINT)
    {
        bool valid = (FindCandidateParts(camera->ToWorldPos(sf::Vector2f(mouseX, mouseY)), true) >= 2);
        allParts.Add(new RevoluteJoint(NULL, NULL, FindSnapPoint(camera->ToWorldPos(sf::Vector2f(mouseX, mouseY))), valid),
                      &allParts.temp);
    }

    else if(currAction == NEW_THRUSTER)
    {
        bool valid = (FindCandidateParts(camera->ToWorldPos(sf::Vector2f(mouseX, mouseY)), true) >= 1);
        allParts.Add(new Thruster(NULL, FindSnapPoint(camera->ToWorldPos(sf::Vector2f(mouseX, mouseY))), valid),
                     &allParts.temp);
    }
    else if(currAction == NEW_CHALLENGE_BOX)
    {
        if(mouseClicks.size() == 1)
        {
            allParts.Add(new ChallengeBox(mouseClicks[0],
                    camera->ToWorldPos(sf::Vector2f(mouseX, mouseY)) - mouseClicks[0]),
                    &allParts.temp);
        }
    }
}

unsigned ControllerGame::FindCandidateParts(sf::Vector2f pos, bool shapeOnly)
{
    allParts.candidate.clear();

    for(int i = allParts.drawOrder.size() -1; i >= 0; --i)
    {
        Part *part = allParts.At(allParts.drawOrder[i]);
        if(part)
            if(part->isEditable && part->isEnabled)
            {
                if(part->InsideShape(pos, camera->GetScale(), shapeOnly))
                {
                    allParts.candidate.push_back(part->id);
                }
            }
    }
    return allParts.candidate.size();
}

unsigned ControllerGame::FindIntersectingParts(sf::Vector2f pos0, sf::Vector2f pos1, Group *group)
{
    for(int i = allParts.drawOrder.size() -1; i >= 0; --i)
    {
        Part *part = allParts.At(allParts.drawOrder[i]);
        if(part)
            if(part->isEditable && part->isEnabled)
            {
                if(part->IntersectsBox(std::min(pos0.x, pos1.x), std::min(pos0.y, pos1.y),
                                       utils.Abs(pos1.x - pos0.x), utils.Abs(pos1.y - pos0.y)))
                {
                    group->push_back(part->id);
                }
            }
    }
    return group->size();
}

sf::Vector2f ControllerGame::FindSnapPoint(sf::Vector2f pos)
{
    FindCandidateParts(pos, true);
    sf::Vector2f closest = pos + sf::Vector2f(800, 800);
    for(unsigned i = 0; i < allParts.candidate.size(); ++i)
    {
        ShapePart *part = (ShapePart*) allParts.At(allParts.candidate[i]);
        if(utils.GetLength(pos - sf::Vector2f(part->centerX, part->centerY)) < utils.GetLength(pos - closest))
            closest = sf::Vector2f(part->centerX, part->centerY);
    }
    if(utils.GetLength(camera->ToScreenPos(pos) - camera->ToScreenPos(closest)) < 15.0f)
        return closest;
    return pos;
}

void ControllerGame::ChangeAction(ActionId id)
{
    currAction = currAction == id ? DEFAULT_ACTION : id;
    if(currAction != DEFAULT_ACTION && currAction != BOX_SELECTING)
    {
        ChangeFocus();
        allParts.selected.clear();
    }

    actionStep = 0;
    mouseClicks.clear();
    UpdateTempParts();
}

void ControllerGame::SaveDesign(String fileName)
{
    if(!simStarted)
    {
        std::vector<String> design;
        allParts.ToString(design);
        file.Save(design, fileName);
    }
}

void ControllerGame::LoadDesign(String fileName)
{
    if(!simStarted)
    {
        FileContents design;
        file.Load(design, fileName);
        allParts.Load(design);
    }
}

void ControllerGame::ExitGame()
{
    exit = true;
}

void ControllerGame::ChangeFocus(GUI *gui)
{
    interface.Show(gui);
}
