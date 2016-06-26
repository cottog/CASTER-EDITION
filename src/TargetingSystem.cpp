#include "main.hpp"

CreatureTargetingSystem::CreatureTargetingSystem(TargetType target)
{
	expectedTarget = TargetingSystemBase::CREATURE;
	targetType = target;
	
}

TileTargetingSystem::TileTargetingSystem()
{
	expectedTarget = TargetingSystemBase::TILE;
}

