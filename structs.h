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
	char symbol;
	int hp;
	int hp_max;
	int pos[2];
	int dmg;
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
	int hp;
	int hp_max;
	int weapon_id;
	int pos[2];
};

//DUNGEON

struct dungeon
{
	int room_id;
	struct npc npcs[10];
	void (*prepare)();
	void (*new_room)();
	void (*show)();
};