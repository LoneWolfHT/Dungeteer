struct dungeon dungeon = {
	ROOM_START, //room id
	{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}, //npcs
	dungeon_prepare,
	new_room,
	show_dungeon
};

void dungeon_prepare()
{
	generate_rooms();

	new_room(ROOM_START, 0, DUN_Y/2);
}

void new_room(number)
int number;
{
	int y = 0;

	struct room room = rooms[number];

	dungeon.room_id = number;

	set_pos(player.pos, 1, room.door);

	dungeon.npcs[NPC_COUNT++] = room.npc;
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

	print_debug();

	refresh();
}