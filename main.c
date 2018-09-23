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

#define npc(num) dungeon.npcs[num]
//#define log(text) def_prog_mode(); endwin(); fprintf(stderr, "%s\n\n", text); reset_prog_mode(); refresh()
#define log(text) clear(); mvprintw(1, 0, "%s...", text); refresh()
#define error(text) clear(); endwin(); printf("\n[ERROR] %s\n\n", text)

int mrand(), getlen(), find(), setsign(), pos_equal();
void game_load(), game_save(), map_save();
char *add(), addstring[100];

FILE *playerfile, *mapfile;

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

void move_npcs(), dungeon_prepare(), generate_rooms(), new_room(), show_dungeon(), getroute(), random_move();
int dist();

int NPC_COUNT = 0;

#define print_debug() mvprintw(11, 0, "%s Pos: {%d, %d} | Health: %d/%d", player.name, player.pos[X], player.pos[Y], player.hp, player.hp_max)
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

#define EMPTY {"...", '?', 0, 0, {}, 0, 0, {}}

#define ROOM_COUNT 1
#define ROOM_START 0

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
struct npc get_npc_by_name();

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

	system("mkdir ./players/");
	system(add("mkdir ./players/", player.name, "/"));

	if (fopen(add("players/", player.name, "/map.dtm"), "r") == NULL)
	{
		log("Preparing the dungeon");
		dungeon.prepare();
	}

	log("Searching for save file");

	if (fopen(add("players/", player.name, "/player.dtp"), "r") != NULL)
	{
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

		if (player.pos[Y] >= 0 && c == MOVE_UP)
		{
			log("Request to move up");
			char sym = getpos_up(player.pos);

			if (sym == ' ')
			{
				log("Are is empty. Moving");
				--player.pos[Y];
			}
			else if (sym == '#')
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
			else if (sym == '#')
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
			else if (sym == '#')
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
			else if (sym == '#')
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
			char *command;
			getstr(command);

			log("Player runs command:");

			log(command);

			if (find(command, "save") != ERR)
				game.save();
		}

		//move_npcs();

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
{ //find function contents
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
{ //Getlen function contents
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
	fscanf(playerfile, "HP-%d HP_MAX-%d W_ID-%d POS_Y-%d, POS_X-%d", &player.hp, &player.hp_max, &player.weapon_id, &player.pos[Y], &player.pos[X]);
	fclose(playerfile);
	
	log("Loading map save file 0%");
	mapfile = fopen(add("players/", player.name, "/map.dtm"), "r");
	int a = 0;
	fscanf(mapfile, "NPCS-{%s, %s, %s, %s, %s, %s, %s, %s, %s, %s}\n\nMAP-{", &npc(a++).name, &npc(a++).name, &npc(a++).name, &npc(a++).name, &npc(a++).name, &npc(a++).name, &npc(a++).name, &npc(a++).name, &npc(a++).name, &npc(a).name);
	
	log("Loading map save file 50%");

	int i = 0;
	fscanf(mapfile, "0-{%s}", room.room[i++]);
	fscanf(mapfile, "1-{%s}", room.room[i++]);
	fscanf(mapfile, "2-{%s}", room.room[i++]);
	fscanf(mapfile, "3-{%s}", room.room[i++]);
	fscanf(mapfile, "4-{%s}", room.room[i++]);
	fscanf(mapfile, "5-{%s}", room.room[i++]);
	fscanf(mapfile, "6-{%s}", room.room[i++]);
	fscanf(mapfile, "7-{%s}", room.room[i++]);
	fscanf(mapfile, "8-{%s}", room.room[i++]);
	fscanf(mapfile, "9-{%s}", room.room[i++]);
	fscanf(mapfile, "10-{%s}", room.room[i++]);
	fscanf(mapfile, "11-{%s}", room.room[i++]);
	fscanf(mapfile, "12-{%s}", room.room[i++]);
	fscanf(mapfile, "13-{%s}", room.room[i++]);
	fscanf(mapfile, "14-{%s}", room.room[i++]);
	fscanf(mapfile, "15-{%s}", room.room[i++]);
	fscanf(mapfile, "16-{%s}", room.room[i++]);
	fscanf(mapfile, "17-{%s}", room.room[i++]);
	fscanf(mapfile, "18-{%s}", room.room[i++]);
	fscanf(mapfile, "19-{%s}", room.room[i++]);
	fscanf(mapfile, "20-{%s}", room.room[i++]);
	fscanf(mapfile, "21-{%s}", room.room[i++]);
	fscanf(mapfile, "22-{%s}", room.room[i++]);
	fscanf(mapfile, "23-{%s}", room.room[i++]);
	fscanf(mapfile, "24-{%s}", room.room[i++]);
	
	fclose(mapfile);

	log("Done");
}

void game_save()
{
	struct room room = rooms[dungeon.room_id];
	
	log("Saving player");
	playerfile = fopen(add("players/", player.name, "/player.dtp"), "w");
	fprintf(playerfile, "HP-%d\nHP_MAX-%d\nW_ID-%d\nPOS-{%d, %d}", player.hp, player.hp_max, player.weapon_id, player.pos[Y], player.pos[X]);
	fclose(playerfile);

	log("Saving map");
	mapfile = fopen(add("players/", player.name, "/map.dtm"), "w");
	int a = 0;
	fprintf(mapfile, "NPCS-{%s, %s, %s, %s, %s, %s, %s, %s, %s, %s}\n\nMAP-{", npc(a++).name, npc(a++).name, npc(a++).name, npc(a++).name, npc(a++).name, npc(a++).name, npc(a++).name, npc(a++).name, npc(a++).name, npc(a).name);
	
	for (int i = 0; i < 25; ++i)
		fprintf(mapfile, "\n%d-{%s}", i, room.room[i]);

	fprintf(mapfile, "\n}");

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