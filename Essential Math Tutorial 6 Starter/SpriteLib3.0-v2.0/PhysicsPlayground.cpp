#include "PhysicsPlayground.h"
#include "Utilities.h"
#include <random>


PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{	
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	

	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		//vec4 temp = vec4(-80.f, 80.f, -80.f, 80.f);
		vec4 temp = vec4(-350.f, 350.f, -350.f, 350.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//nubby
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<Player>(entity);
		
		//Sets up the components
		std::string fileName = "spritesheets/Hamster_Sprite.png";
		std::string animations = "Hamster.json";
		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		//ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 20, 20, &ECS::GetComponent<Sprite>(entity), &ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, GROUND | ENVIRONMENT | CIRCLE | TRIANGLE | SQUARE | PENTAGON | HEXAGON | HEPTAGON | OCTAGON | TRIGGER, 0.5f, 3.f);
		//std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(0, tempSpr.GetHeight() / 2.f) };
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.5);
		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	makeImage("Boogaloo.png", 1100, 590, 0.2, 0, 0, -5);

	//Setup static Top Platform

	//makeStaticObject(std::string filename, int width, int height, int x, int y, int z, int physx, int physy, float shrinkX, float shrinkY, EntityCategories type, float r, float g, float b, float opacity, int rotate);
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, 15, -15, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//start platfoom
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, -40, 15, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//left cubby wall
	makeStaticObject("Hamster_square.png", 120, 10, 30, -10, 2, 15, 50, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 0);//top cubby roof (trigger for cercle) #5
	makeStaticObject("Red.png", 60, 10, 30, -10, 2, 70, 15, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//right door (trigger needs to be attached) 6th created 
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, 125, 14, 0, 0, GROUND, 0, 1, 0, 0.3, 30);//ramp to the right 
	makeStaticObject("wood.png", 65, 11, 30, -10, 2, 207, 43, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//triangle cubby roof
	makeStaticObject("wood.png", 70, 10, 30, -10, 2, 180, 10, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//left bubby wall
	makeStaticObject("wood.png", 73, 10, 30, -10, 2, 240, 12, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//right cubby wall
	makeStaticObject("Blue.png", 70, 10, 30, -10, 2, 210, -20, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 0);//cubby door (triggers) trinagle 
	makeStaticObject("wood.png", 440, 10, 30, -10, 2, 160, -60, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//beeg lower platfoom 
	makeStaticObject("wood.png", 85, 10, 30, -10, 2, 340, 20, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//wall to the right with jump plat foom on it
	makeStaticObject("Red.png", 42, 10, 30, -10, 2, 340, -40, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//door thats triggers(c) 14th created
	makeStaticObject("wood.png", 30, 10, 30, -10, 2, 330, 10, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//side jump platform 
	makeStaticObject("wood.png", 190, 10, 30, -10, 2, 455, -5, 0, 0, GROUND, 0, 1, 0, 0.3, 35);//ramp after c door 
	makeStaticObject("wood.png", 80, 11, 30, -10, 2, 570, 48, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//platfoom hold (p) button
	makeStaticObject("wood.png", 35, 10, 30, -10, 2, 400, 15, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//ramp jump thing flat 
	makeStaticObject("wood.png", 160, 10, 30, -10, 2, 330, 70, 0, 0, GROUND, 0, 1, 0, 0.3, -45);//ramp the one angled yes that one
	makeStaticObject("wood.png", 261, 10, 30, -10, 2, 147, 125, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//platform on top 
	makeStaticObject("wood.png", 202, 10, 30, -10, 2, -65, 175, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//the other ramp to the left ish on top
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -200, 225, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat top left between ramps
	makeStaticObject("wood.png", 105, 10, 30, -10, 2, -295, 243, 0, 0, GROUND, 0, 1, 0, 0.3, -20);//the mini ramp
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -392, 261, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat top left 
	makeStaticObject("wood.png", 112, 10, 30, -10, 2, -438, 210, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//wall on the left 
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -540, 220, 0, 0, GROUND, 0, 1, 0, 0.3, -35);//coin showwt thing i cant spell left one
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -480, 150, 0, 0, GROUND, 0, 1, 0, 0.3, 35);//right slide thing
	makeStaticObject("wood.png", 35, 10, 30, -10, 2, -20, 210, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//moving back up right ramp jump thing tiny flat
	makeStaticObject("Green.png", 95, 10, 30, -10, 2, -80, 210, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//(P) door trigger
	makeStaticObject("wood.png", 173, 10, 30, -10, 2, 80, 225, 0, 0, GROUND, 0, 1, 0, 0.3, 10);//ramp after it
	makeStaticObject("wood.png", 435, 10, 30, -10, 2, 380, 240, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//long platfoom on top right
	makeStaticObject("Pink.png", 80, 10, 30, -10, 2, 460, 280, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//door for hexagon (trigger)
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, 180, 215, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//septagon cubby wall left
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, 230, 215, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//wall right
	makeStaticObject("Yellow.png", 60, 10, 30, -10, 2, 205, 190, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//floor septagon (trigger) #35
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -110, -30, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//ramp on teh left after lower beeg platfoom
	makeStaticObject("wood.png", 120, 11, 30, -10, 2, -219, -1, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat and floor of too cubbys
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, -274, 30, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//pentagon cubby left wall 
	makeStaticObject("Red.png", 60, 10, 30, -10, 2, -205, 30, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//right wall (trigger) #39
	makeStaticObject("wood.png", 70, 10, 30, -10, 2, -244, 55, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//cubby roof / floor
	makeStaticObject("Green.png", 60, 10, 30, -10, 2, -274, 90, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//Square cubby left wall (trigger #41
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, -205, 90, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//cubby right wall
	makeStaticObject("wood.png", 70, 10, 30, -10, 2, -244, 115, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//cubby roof
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -335, 100, 0, 0, GROUND, 0, 1, 0, 0.3, 15);//ramp coming down
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, -415, 85, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat thingy
	makeStaticObject("Yellow.png", 80, 10, 30, -10, 2, -438, 120, 0, 0, GROUND, 0, 1, 0, 0.3, 90);// (s) door trigger thing #46
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -560, 65, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//platform thing that is like a z idk im tired
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -469, 40, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//ramp thing
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, -405, 15, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat thing
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -318, 14, 0, 0, GROUND, 0, 1, 0, 0.3, 45);//ramp from the square cubby thing
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, -380, -30, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//ramp flat thing
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -550, -30, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//Z platform 2 electrijnsdfdkjnksd
	makeStaticObject("Pink.png", 120, 10, 30, -10, 2, -450, -30, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//heptagont 7 (TRIGGER) #53
	makeStaticObject("wood.png", 121, 10, 30, -10, 2, -450, -60, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//ramp thging
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -350, -90, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//that platfoom lower platform bottom left
	makeStaticObject("wood.png", 200, 10, 30, -10, 2, -195, -116, 0, 0, GROUND, 0, 1, 0, 0.3, -15);//other ramp thing 
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, -75, -142, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//ramp flat thing
	makeStaticObject("Green.png", 80, 10, 30, -10, 2, 50, -100, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//(trigger) 'H' over octogon cubby #58
	makeStaticObject("wood.png", 101, 10, 30, -10, 2, 80, -142, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//cubby roof 
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -20, -171, 0, 0, GROUND, 0, 1, 0, 0.3, 30);//left ramp of the octogon cubby 
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, 180, -171, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//right ramp
    makeStaticObject("wood.png", 60, 10, 30, -10, 2, 40, -170, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//hexagon cubby left wall #62
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, 120, -170, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//cubby wall right
	makeStaticObject("Orange.png", 90, 10, 30, -10, 2, 80, -200, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flor thing (TRIGGER) tri
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, 250, -200, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//right platfoom flat pog
	makeStaticObject("wood.png", 80, 10, 30, -10, 2, -100, -200, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat thing in the middle
	makeStaticObject("wood.png", 156, 10, 30, -10, 2, -210, -175, 0, 0, GROUND, 0, 1, 0, 0.3, -20);//ramp on the bottom left
	makeStaticObject("wood.png", 101, 10, 30, -10, 2, -285, -195, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//right wall donky donk thing
	makeStaticObject("Hamster_square.png", 80, 10, 30, -10, 2, -285, -250, 0, 0, GROUND, 0, 1, 0, 0.3,90);//TRIGGER (h) door at bottom 
	makeStaticObject("Green.png", 320, 10, 30, -10, 2, -450, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//TRIGGER (p) unleash the shaps #70
	makeStaticObject("Purple.png", 170, 10, 30, -10, 2, 480, -70, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//(8) wall TRIGGER thingnskdfg
	makeStaticObject("Red.png", 200, 10, 30, -10, 2, 540, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//wINNer thing on top SHAPES TRIGGER TO WIN #72
	makeStaticObject("wood.png", 200, 10, 30, -10, 2, 445, -250, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//wall thing
	makeStaticObject("wood.png", 300, 10, 30, -10, 2, 313, -225, 0, 0, GROUND, 0, 1, 0, 0.3, 30);//long ramp derp bottom right
	makeStaticObject("wood.png", 200, 10, 30, -10, 2, 540, -225, 0, 0, GROUND, 0, 1, 0, 0.3, 0);// HAMSTER TRIGGER to win 
	makeStaticObject("wood.png", 1500, 80, 30, -10, 3, 0, -330, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//floor
	makeStaticObject("wood.png", 1500, 80, 30, -10, 3, -626, 0, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//left
	makeStaticObject("wood.png", 1500, 80, 30, -10, 3, 635, 0, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//right
	makeStaticObject("wood.png", 1500, 80, 30, -10, 3, 0, 350, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//roof
	makeStaticObject("Orange.png", 200, 10, 30, -10, 2, 540, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//wINNer thing on top SHAPES TRIGGER TO WIN#80
	makeStaticObject("Yellow.png", 200, 10, 30, -10, 2, 540, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//wINNer thing on top SHAPES TRIGGER TO WIN#81
	makeStaticObject("Green.png", 200, 10, 30, -10, 2, 540, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//wINNer thing on top SHAPES TRIGGER TO WIN#82
	makeStaticObject("Blue.png", 200, 10, 30, -10, 2, 540, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//wINNer thing on top SHAPES TRIGGER TO WIN#83
	makeStaticObject("Pink.png", 200, 10, 30, -10, 2, 540, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//wINNer thing on top SHAPES TRIGGER TO WIN#84
	makeStaticObject("Purple.png", 200, 10, 30, -10, 2, 540, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//wINNer thing on top SHAPES TRIGGER TO WIN#85
	makeStaticObject("wood.png", 20, 10, 30, -10, 2, -438, 300, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//platfoom that hamster can fit only 
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, 560, 280, 0, 0, GROUND, 0, 1, 0, 0.3, -45);//Angled puzzles top right
	makeStaticObject("wood.png", 120, 50, 30, -10, 2, 590, 280, 0, 0, GROUND, 0, 1, 0, 0.3, -45);//^^^ that part 2 

	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -560, -185, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 1, 1    GOES LEFT TO RIGHT
	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -480, -185, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 1, 2
	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -400, -185, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 1, 3
	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -320, -185, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 1, 4

	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -520, -215, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 2, 1
	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -440, -215, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 2, 2
	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -360, -215, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 2, 3

	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -560, -240, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 3, 1
	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -480, -240, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 3, 2
	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -400, -240, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 3, 3
	makeStaticObjectTriangle("woodT.png", 20, 20, 30, -10, 5, -320, -240, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//row 3, 4
	
	
	//hampster trigger
	makeDestroyTrigger("Hamster_Button.png", 10, 10, 30, -10, 2, 5, 0, 0, -30, -8, TRIGGER, PLAYER, 0, 0, 1, 0.3);

	//circle trigger
	makeDestroyTrigger("Red_Button.png", 10, 10, 30, -10, 2, 6, 0, 0, 60, -8, TRIGGER, CIRCLE, 0, 0, 1, 0.3);
	makeDestroyTrigger("Red_Button.png", 10, 10, 30, -10, 2, 14, 0, 0, 60, -8, TRIGGER, CIRCLE, 0, 0, 1, 0.3);
	makeDestroyTrigger("Red_Button.png", 10, 10, 30, -10, 2, 39, 0, 0, 60, -8, TRIGGER, CIRCLE, 0, 0, 1, 0.3);

	//pentagon trigger
	makeDestroyTrigger("Green_Button.png", 10, 10, 30, -10, 2, 29, 0, 0, 540, 55, TRIGGER, PENTAGON, 0, 0, 1, 0.3);
	makeDestroyTrigger("Green_Button.png", 10, 10, 30, -10, 2, 41, 0, 0, 540, 55, TRIGGER, PENTAGON, 0, 0, 1, 0.3);

	//square trigger
	makeDestroyTrigger("Yellow_Button.png", 10, 10, 30, -10, 2, 35, 0, 0, -520, -24, TRIGGER, SQUARE, 0, 0, 1, 0.3);
	makeDestroyTrigger("Yellow_Button.png", 10, 10, 30, -10, 2, 46, 0, 0, -520, -24, TRIGGER, SQUARE, 0, 0, 1, 0.3);

	//heptagon trigger
	makeDestroyTrigger("Pink_Button.png", 10, 10, 30, -10, 2, 53, 0, 0, -220, 60, TRIGGER, HEPTAGON, 0, 0, 1, 0.3);
	makeDestroyTrigger("Pink_Button.png", 10, 10, 30, -10, 2, 32, 0, 0, -220, 60, TRIGGER, HEPTAGON, 0, 0, 1, 0.3);

	//pentagon prime trigger
	makeDestroyTrigger("Green_Button.png", 10, 10, 30, -10, 2, 70, 0, 0, -520, -145, TRIGGER, PENTAGON, 0, 0, 1, 0.3);
	makeDestroyTrigger("Green_Button.png", 10, 10, 30, -10, 2, 58, 0, 0, -520, -145, TRIGGER, PENTAGON, 0, 0, 1, 0.3);
	
	//hampster prime trigger
	makeDestroyTrigger("Hamster_Button.png", 10, 10, 30, -10, 2, 69, 0, 0, -320, -290, TRIGGER, PLAYER, 0, 0, 1, 0.3);

	//triangle trigger
	makeDestroyTrigger("Orange_Button.png", 10, 10, 30, -10, 2, 64, 0, 0, 580, 246, TRIGGER, TRIANGLE, 0, 0, 1, 0.3);

	//hexagon trigger
	makeDestroyTrigger("Blue.png", 70, 10, 30, -10, 2, 11, 0, 0, -360, -245, TRIGGER, HEXAGON, 0, 0, 1, 0.3);

	//octagon trigger
	makeDestroyTrigger("Purple.png", 70, 10, 30, -10, 2, 71, 0, 0, -440, -245, TRIGGER, OCTAGON, 0, 0, 1, 0.3);

	//all star triggers
	makeDestroyTrigger("Red_Button.png", 10, 10, 30, -10, 2, 72, 0, 0, -200, -290, TRIGGER, CIRCLE, 0, 0, 1, 0.3);
	makeDestroyTrigger("Orange_Button.png", 10, 10, 30, -10, 2, 80, 0, 0, -150, -290, TRIGGER, TRIANGLE, 0, 0, 1, 0.3);
	makeDestroyTrigger("Yellow_Button.png", 10, 10, 30, -10, 2, 81, 0, 0, -100, -290, TRIGGER, SQUARE, 0, 0, 1, 0.3);
	makeDestroyTrigger("Green_Button.png", 10, 10, 30, -10, 2, 82, 0, 0, -50, -290, TRIGGER, PENTAGON, 0, 0, 1, 0.3);
	makeDestroyTrigger("Blue_Button.png", 10, 10, 30, -10, 2, 83, 0, 0, 0, -290, TRIGGER, HEXAGON, 0, 0, 1, 0.3);
	makeDestroyTrigger("Pink_Button.png", 10, 10, 30, -10, 2, 84, 0, 0, 50, -290, TRIGGER, HEPTAGON, 0, 0, 1, 0.3);
	makeDestroyTrigger("Purple_Button.png", 10, 10, 30, -10, 2, 85, 0, 0, 100, -290, TRIGGER, OCTAGON, 0, 0, 1, 0.3);


	//dfkjgnskfjgnkjsfgnkdfefdgasdgjfadsfadfasfgzdfhbhxdfxdggbxdf
	makeImage("Back Ground.png", 1300, 620, 1, 0, 0, -4);
	makeImage("Picture_frame.png", 180, 120, 1, 500, 170, -3);
	makeImage("Book_Shelf.png", 100, 80, 1, -385, 218, -3);
	makeImage("Sloom.png", 120, 100, 1, 385, -255, 4);
	


	//CIRCLE
	{
		auto entity = ECS::CreateEntity();
		ball = entity;
		ECS::SetIsMainBall(entity, true);
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "BeachBall.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(24.f), float32(70.f));
		tempBody = m_physicsWorld->CreateBody(&tempDef);
		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), false, CIRCLE, GROUND | ENVIRONMENT | PLAYER | TRIGGER | TRIANGLE | SQUARE | PENTAGON | HEXAGON | HEPTAGON | OCTAGON, 0.3f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	

	//TRIANGLE
	{
		auto entity = ECS::CreateEntity();
		triangle = entity;
		ECS::SetIsMainTriangle(entity, true);
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "triangle.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(210.f), float32(0.f));
		tempBody = m_physicsWorld->CreateBody(&tempDef);

		std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(0, tempSpr.GetHeight() / 2.f) };
		tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, TRIANGLE, GROUND | ENVIRONMENT | PLAYER | TRIGGER | CIRCLE  | SQUARE | PENTAGON | HEXAGON | HEPTAGON | OCTAGON, 0.5f, 3.5);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	
	//CUBE
	{
		auto entity = ECS::CreateEntity();
		square = entity;
		ECS::SetIsMainSquare(entity, true);
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "square.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-250.f), float32(80.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, SQUARE, GROUND | ENVIRONMENT | PLAYER | TRIGGER | CIRCLE | TRIANGLE  | PENTAGON | HEXAGON | HEPTAGON | OCTAGON, 0.5f, 3.f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	
	
	//PENTAGON
	{
		auto entity = ECS::CreateEntity();
		pentagon = entity;
		ECS::SetIsMainPentagon(entity, true);
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "pentagon.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-225.f), float32(40.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, 0), b2Vec2(-tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, 0), b2Vec2(0, tempSpr.GetHeight() / 2.f) };
		tempPhsBody = PhysicsBody(entity, BodyType::POLYGON, tempBody, points, vec2(0.f, 0.f), false, PENTAGON, GROUND | ENVIRONMENT | PLAYER | TRIGGER | CIRCLE | TRIANGLE | SQUARE | HEXAGON | HEPTAGON | OCTAGON, 0.5f, 3.5);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	

	
	//Hexagon
	{
		auto entity = ECS::CreateEntity();
		hexagon = entity;
		ECS::SetIsMainHexagon(entity, true);
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "hexagon.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(480.f), float32(260.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, 0), b2Vec2(-tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, 0), b2Vec2(tempSpr.GetWidth() / 4.f, tempSpr.GetHeight() / 2.f),  b2Vec2(-tempSpr.GetWidth() / 4.f, tempSpr.GetHeight() / 2.f)};
		tempPhsBody = PhysicsBody(entity, BodyType::POLYGON, tempBody, points, vec2(0.f, 0.f), false, HEXAGON, GROUND | ENVIRONMENT | PLAYER | TRIGGER | CIRCLE | TRIANGLE | SQUARE | PENTAGON | HEPTAGON | OCTAGON, 0.5f, 3.5);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	

	
	//heptagon
	{
	auto entity = ECS::CreateEntity();
	heptagon = entity;
	ECS::SetIsMainHeptagon(entity, true);
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up the components
	std::string fileName = "heptagon.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(200.f), float32(200.f));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	std::vector<b2Vec2> points = { b2Vec2(0, tempSpr.GetHeight()/2), b2Vec2(-tempSpr.GetWidth() / 2.f, tempSpr.GetHeight()/4.f), b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 4.f),  b2Vec2(-tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f),  b2Vec2(tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 4.f), b2Vec2(tempSpr.GetWidth() / 2.f, tempSpr.GetHeight() / 4.f) };
		tempPhsBody = PhysicsBody(entity, BodyType::POLYGON, tempBody, points, vec2(0.f, 0.f), false, HEPTAGON, GROUND | ENVIRONMENT | PLAYER | TRIGGER | CIRCLE | TRIANGLE | SQUARE | PENTAGON | HEXAGON | OCTAGON, 0.5f, 3.5);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}
	
	
	
	//octagon
	{
		auto entity = ECS::CreateEntity();
		octagon = entity;
		ECS::SetIsMainOctagon(entity, true);
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up the components
		std::string fileName = "octagon.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(60.f), float32(-150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		std::vector<b2Vec2> points = { b2Vec2(tempSpr.GetWidth() / 4.f, tempSpr.GetHeight() / 2), b2Vec2(-tempSpr.GetWidth() / 4.f, tempSpr.GetHeight() / 2), b2Vec2(-tempSpr.GetWidth() / 2.f, tempSpr.GetHeight() / 4.f), b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 4.f),  b2Vec2(-tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f),  b2Vec2(tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 4.f), b2Vec2(tempSpr.GetWidth() / 2.f, tempSpr.GetHeight() / 4.f) };
		tempPhsBody = PhysicsBody(entity, BodyType::POLYGON, tempBody, points, vec2(0.f, 0.f), false, OCTAGON, GROUND | ENVIRONMENT | PLAYER | TRIGGER | CIRCLE | TRIANGLE | SQUARE | PENTAGON | HEXAGON | HEPTAGON, 0.5f, 3.5);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	PlaySound(TEXT("nubbys enjoyable evening.wav"), NULL, SND_ASYNC | SND_LOOP);
	
}

void PhysicsPlayground::Update()
{
	
}


void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& ball = ECS::GetComponent<PhysicsBody>(MainEntities::MainBall());
	auto& ballSprite = ECS::GetComponent<Sprite>(MainEntities::MainBall());
	auto& triangle = ECS::GetComponent<PhysicsBody>(MainEntities::MainTriangle());
	auto& triangleSprite = ECS::GetComponent<Sprite>(MainEntities::MainTriangle());
	auto& square = ECS::GetComponent<PhysicsBody>(MainEntities::MainSquare());
	auto& squareSprite = ECS::GetComponent<Sprite>(MainEntities::MainSquare());
	auto& pentagon = ECS::GetComponent<PhysicsBody>(MainEntities::MainPentagon());
	auto& pentagonSprite = ECS::GetComponent<Sprite>(MainEntities::MainPentagon());
	auto& hexagon = ECS::GetComponent<PhysicsBody>(MainEntities::MainHexagon());
	auto& hexagonSprite = ECS::GetComponent<Sprite>(MainEntities::MainHexagon());
	auto& heptagon = ECS::GetComponent<PhysicsBody>(MainEntities::MainHeptagon());
	auto& heptagonSprite = ECS::GetComponent<Sprite>(MainEntities::MainHeptagon());
	auto& octagon = ECS::GetComponent<PhysicsBody>(MainEntities::MainOctagon());
	auto& octagonSprite = ECS::GetComponent<Sprite>(MainEntities::MainOctagon());


	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
	}

	//Change physics body size for circle
	if (Input::GetKey(Key::NumPad0) && Input::GetKey(Key::Add))
	{
		ball.ScaleBody(0.01, 0);
		ballSprite.SetHeight(ball.GetHeight());
		ballSprite.SetWidth(ball.GetWidth());
	}
	else if (Input::GetKey(Key::NumPad0) && Input::GetKey(Key::Subtract))
	{
		ball.ScaleBody(-0.01, 0);
		ballSprite.SetHeight(ball.GetHeight());
		ballSprite.SetWidth(ball.GetWidth());
	}

	if (Input::GetKey(Key::NumPad3) && Input::GetKey(Key::Add))
	{
		triangle.ScaleBody(0.02, 0);
		triangleSprite.SetHeight(triangle.GetHeight());
		triangleSprite.SetWidth(triangle.GetWidth());
	}
	else if (Input::GetKey(Key::NumPad3) && Input::GetKey(Key::Subtract))
	{
		triangle.ScaleBody(-0.02, 0);
		triangleSprite.SetHeight(triangle.GetHeight());
		triangleSprite.SetWidth(triangle.GetWidth());
	}

	if (Input::GetKey(Key::NumPad4) && Input::GetKey(Key::Add))
	{
		square.ScaleBody(0.02, 0);
		squareSprite.SetHeight(square.GetHeight());
		squareSprite.SetWidth(square.GetWidth());
	}
	else if (Input::GetKey(Key::NumPad4) && Input::GetKey(Key::Subtract))
	{
		square.ScaleBody(-0.02, 0);
		squareSprite.SetHeight(square.GetHeight());
		squareSprite.SetWidth(square.GetWidth());
	}

	if (Input::GetKey(Key::NumPad5) && Input::GetKey(Key::Add))
	{
		pentagon.ScaleBody(0.02, 0);
		pentagonSprite.SetHeight(pentagon.GetHeight());
		pentagonSprite.SetWidth(pentagon.GetWidth());
	}
	else if (Input::GetKey(Key::NumPad5) && Input::GetKey(Key::Subtract))
	{
		pentagon.ScaleBody(-0.02, 0);
		pentagonSprite.SetHeight(pentagon.GetHeight());
		pentagonSprite.SetWidth(pentagon.GetWidth());
	}
	
	if (Input::GetKey(Key::NumPad6) && Input::GetKey(Key::Add))
	{
		hexagon.ScaleBody(0.02, 0);
		hexagonSprite.SetHeight(hexagon.GetHeight());
		hexagonSprite.SetWidth(hexagon.GetWidth());
	}
	else if (Input::GetKey(Key::NumPad6) && Input::GetKey(Key::Subtract))
	{
		hexagon.ScaleBody(-0.02, 0);
		hexagonSprite.SetHeight(hexagon.GetHeight());
		hexagonSprite.SetWidth(hexagon.GetWidth());
	}

	if (Input::GetKey(Key::NumPad7) && Input::GetKey(Key::Add))
	{
		heptagon.ScaleBody(0.02, 0);
		heptagonSprite.SetHeight(heptagon.GetHeight());
		heptagonSprite.SetWidth(heptagon.GetWidth());
	}
	else if (Input::GetKey(Key::NumPad7) && Input::GetKey(Key::Subtract))
	{
		heptagon.ScaleBody(-0.02, 0);
		heptagonSprite.SetHeight(heptagon.GetHeight());
		heptagonSprite.SetWidth(heptagon.GetWidth());
	}

	if (Input::GetKey(Key::NumPad8) && Input::GetKey(Key::Add))
	{
		octagon.ScaleBody(0.02, 0);
		octagonSprite.SetHeight(octagon.GetHeight());
		octagonSprite.SetWidth(octagon.GetWidth());
	}
	else if (Input::GetKey(Key::NumPad8) && Input::GetKey(Key::Subtract))
	{
		octagon.ScaleBody(-0.02, 0);
		octagonSprite.SetHeight(octagon.GetHeight());
		octagonSprite.SetWidth(octagon.GetWidth());
	}
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<Player>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::Q))
	{
		
	}

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	
	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 16000000.f), true);
			canJump.m_canJump = false;
		}
	}
	
}

void PhysicsPlayground::KeyboardUp()
{
	

}


void PhysicsPlayground::makeImage(std::string filename, int width, int height, float opacity, float x, float y, float z)
{
	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);

	//Set up the components
	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(opacity);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, y, z));
}

void PhysicsPlayground::makeStaticObject(std::string filename, int width, int height, int x, int y, int z, int physx, int physy, float shrinkX, float shrinkY, EntityCategories type, float r, float g, float b, float opacity, int rotate)
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, width, height);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, y, z));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(physx), float32(physy));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, type, PLAYER | CIRCLE | TRIANGLE | SQUARE | PENTAGON | HEXAGON | HEPTAGON | OCTAGON);
	tempPhsBody.SetColor(vec4(r, g, b, opacity));
	tempPhsBody.SetRotationAngleDeg(rotate);
}

void PhysicsPlayground::makeStaticObjectTriangle(std::string filename, int width, int height, int x, int y, int z, int physx, int physy, float shrinkX, float shrinkY, EntityCategories type, float r, float g, float b, float opacity, int rotate)
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components

	ECS::GetComponent<Sprite>(entity).LoadSprite(filename, width, height);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, y, z));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(physx), float32(physy));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(0, tempSpr.GetHeight() / 2.f) };
	tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, type, PLAYER | CIRCLE | TRIANGLE | SQUARE | PENTAGON | HEXAGON | HEPTAGON | OCTAGON);
	tempPhsBody.SetColor(vec4(r, g, b, opacity));
	tempPhsBody.SetRotationAngleDeg(rotate);
}

void PhysicsPlayground::makeDestroyTrigger(std::string fileName, int length, int width, int x, int y, int z, int target, float shrinkX, float shrinkY, int physX, int physY, EntityCategories type, EntityCategories canActivate, float r, float g, float b, float opacity)
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger*>(entity);

	//Sets up components
	//space
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, length, width);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, y, z));
	ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger();
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(target);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);


	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(physX), float32(physY));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, type, canActivate);
	tempPhsBody.SetColor(vec4(r, g, b, opacity));
}


