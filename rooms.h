struct room rooms[ROOM_COUNT];

void generate_rooms()
{
	int y = 0;

	rooms[ROOM_START].door = 5;
	rooms[ROOM_START].x_size = 22;
	rooms[ROOM_START].y_size = 11;
	rooms[ROOM_START].npc = rat;
	set_pos(rooms[ROOM_START].npc.pos, 3, 9);
	
	rooms[ROOM_START].room[y++] = "+--------------------+";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    #";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "+--------------------+";
}