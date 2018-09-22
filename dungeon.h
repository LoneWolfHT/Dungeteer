struct dungeon dungeon = {
	{0, 0}, //offset
	ROOM_START, //room id
	{},
	{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}, //npcs
	dungeon_prepare,
	print_room,
	show_dungeon
};

void dungeon_prepare()
{
	generate_rooms();

	print_room(ROOM_START, 0, DUN_Y/2);
}

void print_room(number)
int number;
{
	int y = 0;

	struct room room = rooms[number];

	while (y < room.y_size)
	{
		dungeon.map[y] = room.room[y++];
	}

	set_pos(player.pos, 1, room.door);

	dungeon.npcs[NPC_COUNT++] = room.npc;
}

void show_dungeon()
{
	int x, y;

	clear();

	for (y = 0; y < 25; ++y)
	{
		mvaddstr(y, 0, dungeon.map[y]);

		for (x = 0; y == player.pos[Y] && x < 25; ++x)
		{
			int pos[2] = {y, x};

			if (pos_equal(player.pos, pos))
			{
				attron(COLOR_PAIR(COLOR_GREEN));
				mvaddch(y, x, '@');
				attroff(COLOR_PAIR(COLOR_GREEN));
				refresh();
				break;
			}
		}
	}

	print_debug();

	refresh();
}