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
	struct room room = rooms[number];

	dungeon.room_id = number;

	set_pos(player.pos, 1, room.door);

	dungeon.npc = room.npc;
}

void show_dungeon()
{
	int y;

	clear();

	struct room room = rooms[dungeon.room_id];

	for (y = 0; y < room.y_size; ++y)
	{
		mvprintw(y, 0, "%s", room.room[y]);
	}

	attron(COLOR_PAIR(COLOR_GREEN));
	mvprintw(player.pos.y, player.pos.x, "%c", '@');
	attroff(COLOR_PAIR(COLOR_GREEN));

	if (NPC.id != NO_NPC)
	{
		if (distance(player.pos, NPC.pos) <= NPC.view_range)
		{
			attron(COLOR_PAIR(COLOR_RED));
			mvprintw(NPC.pos.y, NPC.pos.x, "%c", NPC.symbol);
			attroff(COLOR_PAIR(COLOR_RED));
		}
		else
			mvprintw(NPC.pos.y, NPC.pos.x, "%c", NPC.symbol);
	}

	print_debug();

	if (NPC.hp > 0)
		print_debug2();

	refresh();
}

#endif