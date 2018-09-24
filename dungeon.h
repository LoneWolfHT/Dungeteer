#ifndef DUNGETEER_DUNGEON
#define DUNGETEER_DUNGEON

struct dungeon dungeon = {
	ROOM_START, //room id
	EMPTY, //npcs
	new_room,
	show_dungeon
};

void new_room(number)
int number;
{
	int y = 0;

	struct room room = rooms[number];

	dungeon.room_id = number;

	set_pos(player.pos, 1, room.door);

	dungeon.npc = room.npc;
}

void show_dungeon()
{
	int x, y;

	clear();

	struct room room = rooms[dungeon.room_id];

	for (y = 0; y < room.y_size; ++y)
	{
		mvprintw(y, 0, "%s", room.room[y]);
		refresh();
	}

	attron(COLOR_PAIR(COLOR_GREEN));
	mvprintw(player.pos[Y], player.pos[X], "%c", '@');
	attroff(COLOR_PAIR(COLOR_GREEN));
	refresh();

	if (NPC.id != NO_NPC)
		mvprintw(NPC.pos[Y], NPC.pos[X], "%c", NPC.symbol);

	print_debug();

	refresh();
}

#endif