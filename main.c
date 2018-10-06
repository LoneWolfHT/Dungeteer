#include <curses.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define Y 0 //Y coordinate
#define X 1 //X coordinate

#define printpos(text, pos) fprintf(logfile, "\n%s{%d, %d}\n", text, pos.x, pos.y)

typedef struct position position;

#define WHITE_BLACK 8
#define WHITE_RED 9
#define WHITE_GREEN 10
#define WHITE_YELLOW 11
#define WHITE_BLUE 12
#define WHITE_MAGENTA 13
#define WHITE_CYAN 14
#define WHITE_WHITE 15

#define NPC dungeon.npc
#define ROOM rooms[dungeon.room_id]

//#define fprintf(logfile, text) def_prog_mode(); endwin(); fprintf(stderr, "%s\n\n", text); reset_prog_mode(); refresh()
//#define fprintf(logfile, text) clear(); dungeon.show(); mvprintw(15, 0, "%s...", text); refresh(); sleep(1)
#define log
#define error(text) clear(); endwin(); printf("\n[ERROR] %s\n\n", text)

int mrand(), getlen(), find(), setsign(), pos_equal(), ftoi(), distance();
float itof();
void game_load(), game_save(), map_save();
char *add(), addstring[100];

FILE *savefile, *fopen();
FILE *logfile;

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

void move_npc(), dungeon_prepare(), generate_rooms(), new_room(), show_dungeon();
int dist();
position random_move(), getroute(), pos_sub(), pos_add(), pos_normalized();

int NPC_COUNT = 0;
int ROOM_COUNT = 0;

#define print_debug() mvprintw(11, 0, "%s: Health: %d/%d | Skill: %d | Weapon: %s: Dmg: %d\n\n>", player.name, player.hp, player.hp_max, player.skill, player_weapon.name, player_weapon.weapon[DMG])
#define print_debug2() mvprintw(13, 0, "%s: Health: %d/%d | Dmg: %d | Skill: %d\n\n>", NPC.name, NPC.hp, NPC.hp_max, NPC.dmg, NPC.skill)
#define set_pos(pos, x, y) pos = (position){x, y}

#define getpos(pos) rooms[dungeon.room_id].room[pos.y][pos.x]
#define getpos_up(pos) rooms[dungeon.room_id].room[pos.y-1][pos.x]
#define getpos_down(pos) rooms[dungeon.room_id].room[pos.y+1][pos.x]
#define getpos_right(pos) rooms[dungeon.room_id].room[pos.y][pos.x+1]
#define getpos_left(pos) rooms[dungeon.room_id].room[pos.y][pos.x-1]

#define pos_up(pos) (position){pos.x-1, pos.y}
#define pos_down(pos) (position){pos.x+1, pos.y}
#define pos_right(pos) (position){pos.x, pos.y+1}
#define pos_left(pos) (position){pos.x, pos.y-1}

#define RAND_ROOM mrand(1, ROOM_COUNT-1)

#define DUN_X 50
#define DUN_Y 50

#define EMPTY {"...", 0, '?', 0, 0, {}, 0, 0, 0, {}}

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

#include "structs.h"
#include "keys.h"

struct dungeon dungeon;
struct room rooms[];

#include "player.h"

#define player_setpos(y, x) player.pos.y = y; player.pos.x = x
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

	logfile = fopen("logs.txt", "w");

	initscr();
 	echo();
 	start_color();
 	curs_set(0);
 	keypad(stdscr, TRUE);
	cbreak();

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

	for (int i = 0; player.name[i] != '\0'; ++i)
		if (player.name[i] == ' ')
			player.name[i] = '_';

	generate_rooms();

	system("mkdir ./players/");
	system(add("mkdir ./players/", player.name, "/"));

	if (fopen(add("players/", player.name, "/save.dt"), "r") == NULL)
	{
		fprintf(logfile, "\nPreparing the dungeon");
		new_room(ROOM_START);
	}

	fprintf(logfile, "\nSearching for save file");

	if (fopen(add("players/", player.name, "/save.dt"), "r") != NULL)
	{
		fprintf(logfile, "\nSave file found");
		game.load();
		dungeon.show();
	}
	else 
	{
		fprintf(logfile, "\nCreating save file");
		game.save();
	}
	
	if (fopen(add("players/", player.name, "/save.dt"), "r") == NULL)
	{
		error("Creation of save file failed");
		return;
	}

	char c;
	int interval = 0;

	dungeon.show();
	timeout(500);

	while ((c = getch()) != 27 && player.hp > 0) //Get pressed key and make sure player isn't dead
	{
		static int interval = 0;
		struct room room = rooms[dungeon.room_id];
 		getmaxyx(stdscr, WIN_H, WIN_W);

		if (player.pos.y > 0 && c == MOVE_UP)
		{
			fprintf(logfile, "\nRequest to move up");
			char sym = getpos_up(player.pos);

			if (sym == ' ')
			{
				fprintf(logfile, "\nArea is empty. Moving");
				--player.pos.y;
			}
			else if (sym == SYMBOL_DOOR)
			{
				fprintf(logfile, "\nDoor found. moving to a new room");
				dungeon.new_room(RAND_ROOM);
			}
			else if (sym == '\0')
			{
				error("Tried to access an empty array value");
				return;
			}
		}

		if (player.pos.y < room.y_size && c == MOVE_DOWN)
		{
			fprintf(logfile, "\nRequest to move down");
			char sym = getpos_down(player.pos);

			if (sym == ' ')
			{
				fprintf(logfile, "\nArea is empty. Moving");
				++player.pos.y;
			}
			else if (sym == SYMBOL_DOOR)
			{
				fprintf(logfile, "\nDoor found. moving to a new room");
				dungeon.new_room(RAND_ROOM);
			}
			else if (sym == '\0')
			{
				error("Tried to access an empty array value");
				return;
			}
		}

		if (player.pos.x > 0 && c == MOVE_LEFT)
		{
			fprintf(logfile, "\nRequest to move left");
			char sym = getpos_left(player.pos);

			if (sym == ' ')
			{
				fprintf(logfile, "\nArea is empty. Moving");
				--player.pos.x;
			}
			else if (sym == SYMBOL_DOOR)
			{
				fprintf(logfile, "\nDoor found. moving to a new room");
				dungeon.new_room(RAND_ROOM);
			}
			else if (sym == '\0')
			{
				error("Tried to access an empty array value");
				return;
			}
		}

		if (player.pos.x < room.x_size && c == MOVE_RIGHT)
		{
			fprintf(logfile, "\nRequest to move right");
			char sym = getpos_right(player.pos);

			if (sym == ' ')
			{
				fprintf(logfile, "\nArea is empty. Moving");
				++player.pos.x;
			}
			else if (sym == SYMBOL_DOOR)
			{
				fprintf(logfile, "\nDoor found. moving to a new room");
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

			getstr(command);

			fprintf(logfile, "Player runs command: \'%s\'", command);

			if (find("save", command) != ERR)
				game.save();

			noecho();
		}

		if (distance(player.pos, NPC.pos) <= 1.0)
		{
			NPC.hp -= mrand(player_weapon.weapon[DMG], player_weapon.weapon[DMG]+player.skill);

			if (NPC.hp > 0)
				player.hp -= mrand(NPC.dmg, NPC.dmg+NPC.skill);
			else
			{
				NPC.hp = 0;

				/*CODE THAT GIVES PLAYER A DROPPED ITEM*/

				if (mrand(1, 30-NPC.skill) == 15)
					++player.skill;

				NPC = npcs[NO_NPC];
			}
		}
		else if (interval++ == 1)
		{
			interval = 0;
			move_npc();
		}

		if (player.hp < player.hp_max && mrand(1, 15) == 3)
			++player.hp;

		dungeon.show();
	}

	nocbreak();

	if (player.hp <= 0)
	{
		char *none;
		clear();
		mvprintw(WIN_H/2, (WIN_W/2)-18, "You died. Press [Enter] to continue");
		refresh();
		getstr(none);
		system(add("rm -r ./players/", player.name, "/"));
	}
	else
		game.save();

	clear();
	endwin();
	system("clear");
}

int mrand(n1, n2)
int n1, n2;
{ //Mrand function contents
	int out = -1;

	++n1;
	++n2;

	while (out < n1)
		out = rand() % n2+1;

	return(out-1);
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

void game_load()
{
	struct room room = rooms[dungeon.room_id];

	fprintf(logfile, "\nLoading player");
	savefile = fopen(add("players/", player.name, "/save.dt"), "r");
	fscanf(savefile, "HP=%d/%d\nW_ID=%d\nPOS={%d, %d}", &player.hp, &player.hp_max, &player.weapon_id, &player.pos.y, &player.pos.x);
	
	fprintf(logfile, "\nLoading map");
	int a = 0;
	fscanf(savefile, "\nNPC_ID=%d\nNPC_POS={%d, %d}\nNPC_HP=%d\nMAP_ID=%d", &NPC.id, &NPC.pos.y, &NPC.pos.x, &NPC.hp, &dungeon.room_id);
	
	if (NPC.id < MAX_NPCS)
	{
		position pos = {NPC.pos.x, NPC.pos.y};
		int hp = NPC.hp;

		NPC = npcs[NPC.id];
		set_pos(NPC.pos, pos.x, pos.y);
		NPC.hp = hp;
	}
	else
		NPC = npcs[NO_NPC];
	
	fclose(savefile);

	fprintf(logfile, "\nDone");
}

void game_save()
{
	struct room room = rooms[dungeon.room_id];
	
	fprintf(logfile, "\nSaving player");
	savefile = fopen(add("players/", player.name, "/save.dt"), "w");
	fprintf(savefile, "HP=%d/%d\nW_ID=%d\nPOS={%d, %d}", player.hp, player.hp_max, player.weapon_id, player.pos.y, player.pos.x);

	fprintf(logfile, "\nSaving map");
	fprintf(savefile, "\nNPC_ID=%d\nNPC_POS={%d, %d}\nNPC_HP=%d\nMAP_ID=%d", NPC.id, NPC.pos.y, NPC.pos.x, NPC.hp, dungeon.room_id);

	fclose(savefile);
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

int ftoi(f)
float f;
{
	f = floor(f);

	return(f);
}

float itof(i)
int i;
{
	return(i);
}