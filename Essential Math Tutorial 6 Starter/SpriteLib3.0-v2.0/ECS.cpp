#include "ECS.h"

entt::registry* ECS::m_reg = nullptr;

void ECS::AttachRegister(entt::registry * reg)
{
	//Attach reference to the registry
	m_reg = reg;
}

void ECS::UnattachRegister()
{
	//Unattaches the register
	m_reg = nullptr;
}

unsigned ECS::CreateEntity()
{
	//create the entity
	auto entity = m_reg->create();

	//Attach Entity Identifier to the entity
	//m_reg->assign<EntityIdentifier>(entity);

	//Return entity ID
	return entity;
}

void ECS::DestroyEntity(unsigned entity)
{
	//Destroys the entity
	m_reg->destroy(entity);
}

void ECS::SetIsMainCamera(unsigned entity, bool mainCamera)
{
	if (mainCamera)
	{
		MainEntities::MainCamera(entity);
	}
}

void ECS::SetIsMainPlayer(unsigned entity, bool mainPlayer)
{
	if (mainPlayer)
	{
		MainEntities::MainPlayer(entity);
	}
}

void ECS::SetIsMainBall(unsigned entity, bool mainBall)
{
	if (mainBall)
	{
		MainEntities::MainBall(entity);
	}
}

void ECS::SetIsMainTriangle(unsigned entity, bool mainTriangle)
{
	if (mainTriangle)
	{
		MainEntities::MainTriangle(entity);
	}
}

void ECS::SetIsMainSquare(unsigned entity, bool mainSquare)
{
	if (mainSquare)
	{
		MainEntities::MainSquare(entity);
	}
}

void ECS::SetIsMainPentagon(unsigned entity, bool mainPentagon)
{
	if (mainPentagon)
	{
		MainEntities::MainPentagon(entity);
	}
}

void ECS::SetIsMainHexagon(unsigned entity, bool mainHexagon)
{
	if (mainHexagon)
	{
		MainEntities::MainHexagon(entity);
	}
}

void ECS::SetIsMainHeptagon(unsigned entity, bool mainHeptagon)
{
	if (mainHeptagon)
	{
		MainEntities::MainHeptagon(entity);
	}
}

void ECS::SetIsMainOctagon(unsigned entity, bool mainOctagon)
{
	if (mainOctagon)
	{
		MainEntities::MainOctagon(entity);
	}
}
