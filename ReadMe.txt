This is the version of ECS I create
most of it follows the same principle of xecs but the difference of it is that 
this variation of ECS thats into account when users do not have to understand the low level
stuff such as Archetype and Pools as ECS Manager acts as the interface of it.

in xecs, there is a bug where the bits were checked wrongly and hence creating more archetypes instead of getting 
existing archeypes that matches

There were thoughts to replace the array in the Pool to dynamically expand the pool size based on the archetype needs(some archetypes do not
require 32 components or even it might require more than 32) to save space but after much consideration, I decide speed is what I 
want for this ECS and uses a array so that the data can be more packed to each other and reduced the chance of cache misses, as vector
contains a pointer to pointer to the exact location and array just have the data at that place.

EntityManager was created to manage the entity and its archetypes instead so the creation process is different,where xecs requrie the user to
getArchetype to create entities in the archetype,my ecs only require the user to create entity either by passing in the components they
want or through a function (which they can also initalize the entity component data on the data)

attempted to do a cheap and faster way to do a search for archetypes where users can choose to either add entity into the search 

