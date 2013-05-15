#ifndef INPUT_EVENT
#define INPUT_EVENT

#include "Vector3D.h"

enum type_entity {NONE,MAP,HERO,INVENTORY,OPTIONS};
enum type_key {UP, DOWN, LEFT, RIGHT, ESC, ML,MR, MM, MSU, MSD};
enum entity_id {ONMAP = -1};
enum key_state {RELEASED, PRESSED};

//structure for encoding all the inputs of the game
//this will server as a post office and distribute the specific events to the specific queues, thanks to susma madam//
 typedef struct
{
	int entity_type;
	int key;
	bool k_state;
	int selected_entity_id;//id of the entity clicked on
	Vector3D position;

} InputEvents;


#endif