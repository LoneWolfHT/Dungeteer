#ifndef DUNGETEER_ROOMS
#define DUNGETEER_ROOMS

#define SYMBOL_DOOR 'H'

#define ROOM_START 0
#define ROOM_MAZE 1
#define ROOM_COUNT 2

struct room rooms[ROOM_COUNT];

void generate_rooms()
{
	int y = 0;

	rooms[ROOM_START].door = 5;
	rooms[ROOM_START].x_size = 22;
	rooms[ROOM_START].y_size = 11;
	rooms[ROOM_START].npc = npcs[RAT];
	
	rooms[ROOM_START].room[y++] = "+--------------------+";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    H";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "|                    |";
	rooms[ROOM_START].room[y++] = "+--------------------+";

	y = 0;

	rooms[ROOM_MAZE].door = 5;
	rooms[ROOM_MAZE].x_size = 22;
	rooms[ROOM_MAZE].y_size = 11;
	rooms[ROOM_MAZE].npc = npcs[RAT];
	
	rooms[ROOM_MAZE].room[y++] = "+--------------------+";
	rooms[ROOM_MAZE].room[y++] = "|---#                |";
	rooms[ROOM_MAZE].room[y++] = "|#### ############## |";
	rooms[ROOM_MAZE].room[y++] = "|   # #              |";
	rooms[ROOM_MAZE].room[y++] = "| # # #  ############|";
	rooms[ROOM_MAZE].room[y++] = "| # # #  #           H";
	rooms[ROOM_MAZE].room[y++] = "|## # #  # ##########|";
	rooms[ROOM_MAZE].room[y++] = "|   # #  #           |";
	rooms[ROOM_MAZE].room[y++] = "| ### #  ########### |";
	rooms[ROOM_MAZE].room[y++] = "|     #              |";
	rooms[ROOM_MAZE].room[y++] = "+--------------------+";
}

#endif