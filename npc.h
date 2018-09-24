#ifndef DUNGETEER_NPC
#define DUNGETEER_NPC

#define NO_NPC 0
#define RAT 1
#define MAX_NPCS 2

struct npc npcs[] = {
	EMPTY,
	{
		"Rat", //Name
		RAT,
		'R', //Symbol
		5, //HP
		5, //MAX_HP
		{}, //Pos
		1, //dmg
		7, //View range
		{ITEM_GOLD, 7} //item to drop on death
	}
};

void move_npc()
{
	if (NPC.id == NO_NPC)
		return;

	if (dist(player.pos, NPC.pos) <= NPC.view_range) //Need to add line of sight check
	{
		getroute(NPC.pos, player.pos);
	}
	else
	{
		random_move(&NPC.pos);
	}
}

#endif