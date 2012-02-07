#include "shooterentity.h"
#include "shooterworld.h"

ShooterEntity::ShooterEntity() : Entity(), shooterWorld(0) 
{
  
};
void ShooterEntity::setWorld(ShooterWorld* const newWorld) 
{ 
  shooterWorld = newWorld; 
  
};
ShooterWorld* ShooterEntity::getWorld() const 
{ 
  return shooterWorld; 
  
};
