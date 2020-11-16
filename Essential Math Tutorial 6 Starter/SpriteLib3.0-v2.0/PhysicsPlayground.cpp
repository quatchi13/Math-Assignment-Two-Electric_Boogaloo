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
	//PlaySound(TEXT("music.wav"), NULL, SND_ASYNC);

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
		vec4 temp = vec4(-300.f, 300.f, -300.f, 300.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Player>(entity);
		

		//Sets up the components
		std::string fileName = "Hamster place holder.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
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
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, 15, 50, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 0);//top cubby roof (trigger for cercle)
	//makeStaticObject("wood.png", 60, 10, 30, -10, 2, 70, 15, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//right door (trigger needs to be attached) 7th created 
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, 125, 14, 0, 0, GROUND, 0, 1, 0, 0.3, 30);//ramp to the right 
	makeStaticObject("wood.png", 65, 11, 30, -10, 2, 207, 43, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//triangle cubby roof
	makeStaticObject("wood.png", 70, 10, 30, -10, 2, 180, 10, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//left bubby wall
	makeStaticObject("wood.png", 73, 10, 30, -10, 2, 240, 12, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//right cubby wall
	makeStaticObject("wood.png", 70, 10, 30, -10, 2, 210, -20, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 0);//cubby door (triggers) trinagle 
	makeStaticObject("wood.png", 440, 10, 30, -10, 2, 160, -60, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//beeg lower platfoom 
	makeStaticObject("wood.png", 98, 10, 30, -10, 2, 340, 30, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//wall to the right with jump plat foom on it
	//makeStaticObject("wood.png", 42, 10, 30, -10, 2, 340, -40, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//door thats triggers(c) 15th created
	makeStaticObject("wood.png", 30, 10, 30, -10, 2, 330, 10, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//side jump platform 
	makeStaticObject("wood.png", 190, 10, 30, -10, 2, 455, -5, 0, 0, GROUND, 0, 1, 0, 0.3, 35);//ramp after c door 
	makeStaticObject("wood.png", 40, 11, 30, -10, 2, 550, 48, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//platfoom hold (p) button
	makeStaticObject("wood.png", 35, 10, 30, -10, 2, 420, 35, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//ramp jump thing flat 
	makeStaticObject("wood.png", 160, 10, 30, -10, 2, 340, 80, 0, 0, GROUND, 0, 1, 0, 0.3, -35);//ramp the one angled yes that one
	makeStaticObject("wood.png", 261, 10, 30, -10, 2, 147, 125, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//platform on top 
	makeStaticObject("wood.png", 202, 10, 30, -10, 2, -65, 175, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//the other ramp to the left ish on top
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -200, 225, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat top left between ramps
	makeStaticObject("wood.png", 105, 10, 30, -10, 2, -295, 243, 0, 0, GROUND, 0, 1, 0, 0.3, -20);//the mini ramp
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -392, 261, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat top left 
	makeStaticObject("wood.png", 112, 10, 30, -10, 2, -438, 210, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//wall on the left 
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -540, 220, 0, 0, GROUND, 0, 1, 0, 0.3, -35);//coin showwt thing i cant spell left one
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -480, 150, 0, 0, GROUND, 0, 1, 0, 0.3, 35);//right slide thing
	makeStaticObject("wood.png", 35, 10, 30, -10, 2, -20, 210, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//moving back up right ramp jump thing tiny flat
	//makeStaticObject("wood.png", 95, 10, 30, -10, 2, -80, 210, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//(P) door trigger
	makeStaticObject("wood.png", 173, 10, 30, -10, 2, 80, 225, 0, 0, GROUND, 0, 1, 0, 0.3, 10);//ramp after it
	makeStaticObject("wood.png", 435, 10, 30, -10, 2, 380, 240, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//long platfoom on top right
	makeStaticObject("wood.png", 80, 10, 30, -10, 2, 460, 280, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//door for hexagon (trigger)
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, 180, 215, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//septagon cubby wall left
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, 230, 215, 0, 0, ENVIRONMENT, 0, 1, 0, 0.3, 90);//wall right
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, 205, 190, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//floor septagon (trigger)
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -110, -30, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//ramp on teh left after lower beeg platfoom
	makeStaticObject("wood.png", 120, 11, 30, -10, 2, -219, -1, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat and floor of too cubbys
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, -274, 30, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//pentagon cubby left wall 
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, -205, 30, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//right wall (trigger)
	makeStaticObject("wood.png", 70, 10, 30, -10, 2, -244, 55, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//cubby roof / floor
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, -274, 90, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//Square cubby left wall (trigger
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, -205, 90, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//cubby right wall
	makeStaticObject("wood.png", 70, 10, 30, -10, 2, -244, 115, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//cubby roof
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -335, 100, 0, 0, GROUND, 0, 1, 0, 0.3, 15);//ramp coming down
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, -415, 85, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat thingy
	//makeStaticObject("wood.png", 80, 10, 30, -10, 2, -438, 120, 0, 0, GROUND, 0, 1, 0, 0.3, 90);// (s) door trigger thing
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -560, 65, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//platform thing that is like a z idk im tired
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -469, 40, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//ramp thing
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, -405, 15, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat thing
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -318, 14, 0, 0, GROUND, 0, 1, 0, 0.3, 45);//ramp from the square cubby thing
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, -380, -30, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//ramp flat thing
	makeStaticObject("wood.png", 100, 10, 30, -10, 2, -550, -30, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//Z platform 2 electrijnsdfdkjnksd
	//makeStaticObject("wood.png", 120, 10, 30, -10, 2, -450, -30, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//heptagont 7 (TRIGGER)
	makeStaticObject("wood.png", 121, 10, 30, -10, 2, -450, -60, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//ramp thging
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -350, -90, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//that platfoom lower platform bottom left
	makeStaticObject("wood.png", 200, 10, 30, -10, 2, -195, -116, 0, 0, GROUND, 0, 1, 0, 0.3, -15);//other ramp thing 
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, -75, -142, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//ramp flat thing
	makeStaticObject("wood.png", 80, 10, 30, -10, 2, 50, -100, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//(trigger) 'H' over octogon cubby
	makeStaticObject("wood.png", 101, 10, 30, -10, 2, 80, -142, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//cubby roof 
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, -20, -171, 0, 0, GROUND, 0, 1, 0, 0.3, 30);//left ramp of the octogon cubby 
	makeStaticObject("wood.png", 120, 10, 30, -10, 2, 180, -171, 0, 0, GROUND, 0, 1, 0, 0.3, -30);//right ramp
    makeStaticObject("wood.png", 60, 10, 30, -10, 2, 40, -170, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//hexagon cubby left wall 
	makeStaticObject("wood.png", 60, 10, 30, -10, 2, 120, -170, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//cubby wall right
	makeStaticObject("wood.png", 90, 10, 30, -10, 2, 80, -200, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flor thing (TRIGGER) tri
	makeStaticObject("wood.png", 50, 10, 30, -10, 2, 250, -200, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//right platfoom flat pog
	makeStaticObject("wood.png", 80, 10, 30, -10, 2, -100, -200, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//flat thing in the middle
	makeStaticObject("wood.png", 156, 10, 30, -10, 2, -210, -175, 0, 0, GROUND, 0, 1, 0, 0.3, -20);//ramp on the bottom left
	makeStaticObject("wood.png", 101, 10, 30, -10, 2, -285, -195, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//right wall donky donk thing
	makeStaticObject("wood.png", 80, 10, 30, -10, 2, -285, -250, 0, 0, GROUND, 0, 1, 0, 0.3,90);//TRIGGER (h) door at bottom 
	makeStaticObject("wood.png", 320, 10, 30, -10, 2, -450, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//TRIGGER (p) unleash the shaps
	makeStaticObject("wood.png", 170, 10, 30, -10, 2, 480, -70, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//(8) wall TRIGGER thingnskdfg
	makeStaticObject("wood.png", 200, 10, 30, -10, 2, 540, -150, 0, 0, GROUND, 0, 1, 0, 0.3, 0);//wINNer thing on top SHAPES TRIGGER TO WIN
	makeStaticObject("wood.png", 200, 10, 30, -10, 2, 445, -250, 0, 0, GROUND, 0, 1, 0, 0.3, 90);//wall thing
	makeStaticObject("wood.png", 300, 10, 30, -10, 2, 313, -225, 0, 0, GROUND, 0, 1, 0, 0.3, 30);//long ramp derp bottom right
	makeStaticObject("wood.png", 200, 10, 30, -10, 2, 540, -225, 0, 0, GROUND, 0, 1, 0, 0.3, 0);// HAMSTER TRIGGER to win 





	//Ball
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
		tempDef.position.Set(float32(45.f), float32(-8.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, GROUND | ENVIRONMENT | PLAYER | TRIGGER, 0.3f);

		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	}

	//Setup trigger
	{
		//Creates entity
		/*auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		
		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 40);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(ball);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(300.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));*/
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

	
}

void PhysicsPlayground::Update()
{
	
}


void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& ball = ECS::GetComponent<PhysicsBody>(MainEntities::MainBall());


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
	if (Input::GetKey(Key::O))
	{
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
	}

	if (Input::GetKey(Key::NumPad0))
	{
		ball.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::NumPad9))
	{
		ball.ScaleBody(-1.3 * Timer::deltaTime, 0);
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
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, type, PLAYER | ENEMY);
	tempPhsBody.SetColor(vec4(r, g, b, opacity));
	tempPhsBody.SetRotationAngleDeg(rotate);
}


