#ifndef DUNGETEER_NPC
#define DUNGETEER_NPC

#define NO_NPC 0
#define RAT 1
#define T_H 2
#define BIG_RAT 3
#define MAX_NPCS 4

struct npc npcs[] = {
	EMPTY,
	{ //Rat
		"Rat", //Name
		RAT, //npc id
		'R', //Symbol
		5, //HP
		5, //MAX_HP
		{}, //Pos
		1, //dmg
		0, //skill
		7.0, //View range
		{ITEM_GOLD, 7} //item to drop on death
	},
	{ //Treasure hunter
		"Treasure Hunter", //Name
		T_H, //npc id
		'T', //Symbol
		15, //HP
		15, //MAX_HP
		{}, //Pos
		2, //dmg
		2, //skill
		10.0, //View range
		{ITEM_GOLD, 3} //item to drop on death
	},
	{ //big rat
		"Big Rat", //Name
		BIG_RAT, //npc id
		'R', //Symbol
		15, //HP
		15, //MAX_HP
		{}, //Pos
		3, //dmg
		2, //skill
		9.0, //View range
		{ITEM_GOLD, 6} //item to drop on death
	}
};

void move_npc()
{
	if (NPC.id == NO_NPC)
		return;

	if (distance(player.pos, NPC.pos) <= NPC.view_range) //Need to add line of sight check
	{
		fprintf(logfile, "\nNPC can see player. (%d <= %d)", distance(player.pos, NPC.pos), NPC.view_range);
		NPC.pos = getroute(NPC.pos, player.pos);
	}
	else
	{
		fprintf(logfile, "\nNPC can't see player. (%d > %d)", distance(player.pos, NPC.pos), NPC.view_range);
		NPC.pos = random_move(NPC.pos);
	}
}

#endif