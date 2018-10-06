#ifndef DUNGETEER_ROOMS
#define DUNGETEER_ROOMS

#define SYMBOL_DOOR 'H'

#define ROOM_START 0

void register_room();

struct room rooms[15];

void generate_rooms()
{
	register_room(7, NO_NPC, (position){0, 0}, (char *[25]){
		"+--------------------+",
		"|     DUNGETEER      |",
		"+--------------------+",
		"|                    |",
		"|                    |",
		"|Walk into the door> H",
		"|                    |",
		"|                    |",
		"|                    |",
		"|                    |",
		"+--------------------+"
	});

	register_room(5, RAT, (position){9, 5}, (char *[25]){
		"+--------------------+",
		"|                    |",
		"|                    |",
		"|                    |",
		"|                    |",
		"|                    H",
		"|                    |",
		"|                    |",
		"|                    |",
		"|                    |",
		"+--------------------+"
	});

	register_room(5, RAT, (position){16, 5}, (char *[25]){
		"+--------------------+",
		"|---#                |",
		"|#### ############## |",
		"|   # #              |",
		"| # # #  ############|",
		"| # # #  #           H",
		"|## # #  # ##########|",
		"|   # #  #           |",
		"| ### #  ########### |",
		"|     #              |",
		"+--------------------+"
	});

	register_room(9, BIG_RAT, (position){15, 2}, (char *[25]){
		"+--------------------+",
		"|                    |",
		"|########            H",
		"|                    |",
		"|########  ##########|",
		"|                    |",
		"| ###################|",
		"|                    |",
		"|################### |",
		"|                    |",
		"+--------------------+"
	});

	register_room(9, T_H, (position){16, 2}, (char *[25]){
		"+--------------------+",
		"|                    |",
		"|        #           |",
		"|        #           |",
		"|        #     ######|",
		"|        #     #     H",
		"|        #     #     |",
		"|        #     #     |",
		"|        #     #     |",
		"[        #           |",
		"+--------------------+"
	});
}

void register_room(door, npc, npc_pos, room)
int door, npc;
position npc_pos;
char *room[25];
{
	static int number = 0;
	int y;

	rooms[number].door = door;
	rooms[number].npc = npcs[npc];
	set_pos(rooms[number].npc.pos, npc_pos.x, npc_pos.y);

	for (y = 0; y < 25 && room[y] != '\0'; ++y)
	{
		rooms[number].room[y] = room[y];
	}

	rooms[number].y_size = y+1;
	rooms[number].x_size = getlen(room[1]);

	++ROOM_COUNT;
	++number;
}

#endif