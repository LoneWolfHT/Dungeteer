#include <curses.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define Y 0 //Y coordinate
#define X 1 //X coordinate

#define WHITE_BLACK 8
#define WHITE_RED 9
#define WHITE_GREEN 10
#define WHITE_YELLOW 11
#define WHITE_BLUE 12
#define WHITE_MAGENTA 13
#define WHITE_CYAN 14
#define WHITE_WHITE 15

#define NPC dungeon.npc
#define log(text) def_prog_mode(); endwin(); fprintf(stderr, "%s\n\n", text); reset_prog_mode(); refresh()
//#define log(text) clear(); dungeon.show(); mvprintw(15, 0, "%s...", text); refresh(); sleep(1)
#define error(text) clear(); endwin(); printf("\n[ERROR] %s\n\n", text)

int mrand(), getlen(), find(), setsign(), pos_equal();
void game_load(), game_save(), map_save();
char *add(), addstring[100];

FILE *playerfile, *mapfile, *fopen();

int WIN_H = 50; //Window height
int WIN_W = 50; //Window width

struct text_val
{
	char *text;
	int val;
};

struct game
{
	void (*load)();
	void (*save)();
};

struct game game = {
	game_load,
	game_save
};

void move_npc(), dungeon_prepare(), generate_rooms(), new_room(), show_dungeon(), random_move(), getroute();
int dist();

int NPC_COUNT = 0;

#define print_debug() mvprintw(11, 0, "%s Pos: {%d, %d} | Health: %d/%d\n\n> ", player.name, player.pos[X], player.pos[Y], player.hp, player.hp_max)
#define set_pos(pos, x, y) pos[Y] = y; pos[X] = x

#define getpos(pos) room.room[pos[Y]][pos[X]]
#define getpos_up(pos) room.room[pos[Y]-1][pos[X]]
#define getpos_down(pos) room.room[pos[Y]+1][pos[X]]
#define getpos_right(pos) room.room[pos[Y]][pos[X]+1]
#define getpos_left(pos) room.room[pos[Y]][pos[X]-1]

#define pos_up(pos) {pos[Y]-1, pos[X]}
#define pos_down(pos) {pos[Y+1], pos[X]}
#define pos_right(pos) {pos[Y], pos[X]+1}
#define pos_left(pos) {pos[Y], pos[X]-1}

#define RAND_ROOM mrand(0, ROOM_COUNT-1)

#define DUN_X 50
#define DUN_Y 50

#define ITEM_COUNT 2
#define ITEM_GOLD 0
#define ITEM_SWORD 1

#define EMPTY {"...", -1, '?', 0, 0, {}, 0, 0, {}}

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

#include "structs.h"
#include "keys.h"

struct dungeon dungeon;
struct room rooms[];

#include "player.h"

#define player_setpos(y, x) player.pos[Y] = y; player.pos[X] = x
#define player_weapon items[player.weapon_id]

#include "items.h"

#include "npc.h"

#include "rooms.h"
#include "dungeon.h"
#include "positions.h"

void main()
{
	time_t t;
	srand((unsigned) time(&t));

	initscr();
 	echo();
 	start_color();
 	curs_set(FALSE);
 	keypad(stdscr, TRUE);
	halfdelay(15);
 	nodelay(stdscr, FALSE);

	for (int i = 0; i < 8; ++i)
		init_pair(i, i, 0);

 	for (int i = 8; i < 16; ++i)
		init_pair(i, i-8, COLOR_WHITE);

 	getmaxyx(stdscr, WIN_H, WIN_W);

	mvprintw(WIN_H/2, WIN_W/2-18, "Please type in your character's name");
	refresh();
	move((WIN_H/2)+3, (WIN_W/2)-18);
	getstr(player.name);
	noecho();

	generate_rooms();

	system("mkdir ./players/");
	system(add("mkdir ./players/", player.name, "/"));

	if (fopen(add("players/", player.name, "/map.dtm"), "r") == NULL)
	{
		log("Preparing the dungeon");
		new_room(ROOM_START);
	}

	log("Searching for save file");

	if (fopen(add("players/", player.name, "/player.dtp"), "r") != NULL)
	{
		log("Save file found");
		game.load();
		dungeon.show();
	}
	else 
	{
		log("Creating save file");
		game.save();
	}
	
	if (fopen(add("players/", player.name, "/player.dtp"), "r") == NULL)
	{
		error("Creation of save file failed");
		return;
	}

	char c;
	int interval = 0;

	dungeon.show();

	while ((c = getch()) != 27) //Get pressed key
	{
		struct room room = rooms[dungeon.room_id];
 		getmaxyx(stdscr, WIN_H, WIN_W);

		if (player.pos[Y] > 0 && c == MOVE_UP)
		{
			log("Request to move up");
			char sym = getpos_up(player.pos);

			if (sym == ' ')
			{
				log("Are is empty. Moving");
				--player.pos[Y];
			}
			else if (sym == SYMBOL_DOOR)
			{
				log("Door found. moving to a new room");
				dungeon.new_room(RAND_ROOM);
			}
			else if (sym == '\0')
			{
				error("Tried to access an empty array value");
				return;
			}
		}

		if (player.pos[Y] < room.y_size && c == MOVE_DOWN)
		{
			log("Request to move down");
			char sym = getpos_down(player.pos);

			if (sym == ' ')
			{
				log("Are is empty. Moving");
				++player.pos[Y];
			}
			else if (sym == SYMBOL_DOOR)
			{
				log("Door found. moving to a new room");
				dungeon.new_room(RAND_ROOM);
			}
			else if (sym == '\0')
			{
				error("Tried to access an empty array value");
				return;
			}
		}

		if (player.pos[X] > 0 && c == MOVE_LEFT)
		{
			log("Request to move left");
			char sym = getpos_left(player.pos);

			if (sym == ' ')
			{
				log("Are is empty. Moving");
				--player.pos[X];
			}
			else if (sym == SYMBOL_DOOR)
			{
				log("Door found. moving to a new room");
				dungeon.new_room(RAND_ROOM);
			}
			else if (sym == '\0')
			{
				error("Tried to access an empty array value");
				return;
			}
		}

		if (player.pos[X] < room.x_size && c == MOVE_RIGHT)
		{
			log("Request to move right");
			char sym = getpos_right(player.pos);

			if (sym == ' ')
			{
				log("Are is empty. Moving");
				++player.pos[X];
			}
			else if (sym == SYMBOL_DOOR)
			{
				log("Door found. moving to a new room");
				dungeon.new_room(RAND_ROOM);
			}
			else if (sym == '\0')
			{
				error("Tried to access an empty array value");
				return;
			}
		}

		if (c == COMMAND)
		{
			echo();
			addch('/');
			char command[25];
			int param[2] = {0, 0};

			getstr(command);
			sscanf(command, "%s %d %d", command, &param[0], &param[1]);

			log(add("Player runs command: \'", command, "\'"));

			if (find("save", command) != ERR)
				game.save();

			if (find("tp", command) != ERR && param[0] > 0 && param[1] > 0)
			{
				log("Teleporting player");
				set_pos(player.pos, param[0], param[1]);
			}

			noecho();
		}

		move_npc();

		dungeon.show();
	}

	game.save();
	clear();
	endwin();
	system("clear");
}

int mrand(n1, n2)
int n1, n2;
{ //Mrand function contents
	int out = -1;

	if (n1 == 0)
	{
		++n1;
		++n2;

		while (out < n1)
			out = rand() % n2+1;

		return(out-1);
	}
	else
	{
		while (out < n1)
			out = rand() % n2+1;

		return(out);
	}
}

int find(s1, s2)
char *s1, *s2;
{
	int i;

	for (i=0; i <= getlen(s1); ++i)
	{
		if (s1[i] == s2[0])
		{
			for (int a = 1; a < getlen(s2); ++a)
			{
				if (s1[i+a] != s2[i+a])
					break;
			}
			return(i);
		}
	}

	return(-1);
}

int getlen(string)
char string[];
{
	int i;

	for (i = 0; string[i] != '\0'; ++i);

	return(i);
}

int setsign(num, sign)
int num;
char sign;
{
	if (sign == '-')
	{
		if (num > 0)
			return(num * -1);
	}
	else if (sign == '+')
	{
		if (num < 0)
			return(num * -1);
	}

	return(num);
}

int pos_equal(pos1, pos2)
int pos1[], pos2[];
{
	if (pos1[Y] == pos2[Y] && pos1[X] == pos2[X])
		return(TRUE);

	return(FALSE);
}

void game_load()
{
	struct room room = rooms[dungeon.room_id];

	log("Loading player save file");
	playerfile = fopen(add("players/", player.name, "/player.dtp"), "r");
	fscanf(playerfile, "HP=%d\nHP_MAX=%d\nW_ID=%d\nPOS={%d, %d}", &player.hp, &player.hp_max, &player.weapon_id, &player.pos[Y], &player.pos[X]);
	fclose(playerfile);
	
	log("Loading map save file");
	mapfile = fopen(add("players/", player.name, "/map.dtm"), "r");
	int a = 0;
	fscanf(mapfile, "NPC_ID=%d\nMAP_ID=%d", &NPC.id, &dungeon.room_id);
	
	if (NPC.id < MAX_NPCS)
		NPC = npcs[NPC.id];
	else
		NPC = npcs[NO_NPC];
	
	fclose(mapfile);

	log("Done");
}

void game_save()
{
	struct room room = rooms[dungeon.room_id];
	
	log("Saving player");
	playerfile = fopen(add("players/", player.name, "/player.dtp"), "w");
	fprintf(playerfile, "HP=%d\nHP_MAX=%d\nW_ID=%d\nPOS={%d, %d}", player.hp, player.hp_max, player.weapon_id, player.pos[Y], player.pos[X]);
	fclose(playerfile);

	log("Saving map");
	mapfile = fopen(add("players/", player.name, "/map.dtm"), "w");
	int a = 0;
	fprintf(mapfile, "NPC_ID=%d\nMAP_ID=%d", NPC.id, dungeon.room_id);

	fclose(mapfile);
}

char *add(before, string, after)
char before[], string[], after[];
{
	int a = 0;
	int b = 0;

	for (a = 0; a <= getlen(before); ++a)
	{
		if (before[a] != '\0')
			addstring[b++] = before[a];
	}

	for (a = 0; a <= getlen(string); ++a)
	{
		if (string[a] != '\0')
			addstring[b++] = string[a];
	}

	for (a = 0; a <= getlen(after); ++a)
	{
		if (after[a] != '\0')
			addstring[b++] = after[a];
	}

	addstring[b] = '\0';

	return(addstring);
}