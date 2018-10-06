#ifndef DUNGETEER_STRUCTS
#define DUNGETEER_STRUCTS

//POS

struct position
{
	int x;
	int y;
};

//ITEMS

struct drop
{
	int id;
	int chance;
};

struct item
{
	char *name;
	char *desc;
	struct text_val on_eat;
	int weapon[2];
};

//NPCS

struct npc
{
	char *name;
	int id;
	char symbol;
	int hp;
	int hp_max;
	position pos;
	int dmg;
	int skill;
	int view_range;
	struct drop drop;
};

//ROOMS

struct room
{
	int door;
	int x_size;
	int y_size;
	char *room[25];
	struct npc npc;
};

//PLAYER

struct player 
{
	char name[50];
	int skill;
	int hp;
	int hp_max;
	int weapon_id;
	position pos;
};

//DUNGEON

struct dungeon
{
	int room_id;
	struct npc npc;
	void (*new_room)();
	void (*show)();
};

#endif