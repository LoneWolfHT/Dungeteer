#ifndef DUNGETEER_POS
#define DUNGETEER_POS

void random_move(pos)
int *pos;
{
	int possible[4] = {TRUE, TRUE, TRUE, TRUE};
	struct room room = rooms[dungeon.room_id];
	int to;

	static int last = -1;

	if (pos[Y] >= room.y_size-1 || room.room[pos[Y]+1][pos[X]] != ' ')
		possible[DOWN] = FALSE;

	if (pos[Y] <= 1 || room.room[pos[Y]-1][pos[X]] != ' ')
		possible[UP] = FALSE;

	if (pos[X] >= room.x_size-1 || room.room[pos[Y]][pos[X]+1] != ' ')
		possible[RIGHT] = FALSE;

	if (pos[X] <= 1 || room.room[pos[Y]][pos[X]-1] != ' ')
		possible[LEFT] = FALSE;

	for (int i = 0;; ++i)
	{
		int dir = mrand(0, 3);

		if (possible[dir] == TRUE && dir != last)
		{
			to = dir;
			log("direction found");
			break;
		}

		if (i == 100)
		{
			log("limit reached");
			return;
		}
	}

	switch (to)
	{
		case UP:
		{
			--pos[Y];
			log("Moving up");
			return;
		}
		case DOWN:
		{
			++pos[Y];
			log("Moving down");
			return;
		}
		case LEFT:
		{
			--pos[X];
			log("Moving left");
			return;
		}
		case RIGHT:
		{
			++pos[X];
			log("Moving right");
			return;
		}
	}
}

int dist(pos1, pos2)
int pos1[];
int pos2[];
{
	int distance = 0;
	int pos[2] = {pos2[Y], pos2[X]};

	pos[Y] -= pos1[Y];
	pos[X] -= pos1[X];

	for (int i = 0; pos[Y] != 0 || pos[X] != 0; ++i)
	{
		if (pos[Y] > 0 && pos[X] > 0) // y > 0 and x > 0
		{
			--pos[Y];
			--pos[X];
			++distance;
		}
		else if (pos[Y] < 0 && pos[X] < 0) //y < 0 and x < 0
		{
			++pos[Y];
			++pos[X];
			++distance;
		}
		else if (pos[Y] > 0 && pos[X] < 0) // y > 0 and x < 0
		{
			--pos[Y];
			++pos[X];
			++distance;
		}
		else if (pos[Y] < 0 && pos[X] > 0) // y < 0 and x > 0
		{
			++pos[Y];
			--pos[X];
			++distance;
		}
		else if (pos[Y] == 0) // y == 0
		{
			if (pos[X] > 0) // x > 0
			{
				--pos[X];
				++distance;
			}
			else if (pos[X] < 0) // x < 0
			{
				++pos[X];
				++distance;
			}
		}
		else if (pos[X] == 0) // x == 0
		{
			if (pos[Y] > 0) // y > 0
			{
				--pos[Y];
				++distance;
			}
			else if (pos[Y] < 0) // y < 0
			{
				++pos[Y];
				++distance;
			}
		}
		else
		{
			int x = setsign(pos[X], '+');
			int y = setsign(pos[Y], '+');

			if (x > y)
			{	
				if (pos[X] > 0)
					--pos[X];
				else
					++pos[X];

				++distance;
			}
			else if (x < y)
			{
				if (pos[Y] > 0)
					--pos[Y];
				else
					++pos[Y];
				
				++distance;
			}
		}
	}

	return(distance);
}

void getroute(pos1, pos2)
int *pos1, pos2[];
{
					 /*Top, Bottom, Right, Left*/
	int possible[4] = {TRUE, TRUE, TRUE, TRUE};
	struct room room = rooms[dungeon.room_id];

	if (pos1[Y] >= room.y_size || room.room[pos1[Y]+1][pos1[X]] != ' ')
		possible[DOWN] = FALSE;

	if (pos1[Y] <= 0 || room.room[pos1[Y]-1][pos1[X]] != ' ')
		possible[UP] = FALSE;

	if (pos1[X] >= room.x_size || room.room[pos1[Y]][pos1[X]+1] != ' ')
		possible[RIGHT] = FALSE;

	if (pos1[X] <= 0 || room.room[pos1[Y]][pos1[X]-1] != ' ')
		possible[LEFT] = FALSE;

	int x = setsign(pos1[X]-pos2[X], '+');
	int y = setsign(pos1[Y]-pos2[Y], '+');

	if ((x == 0 && y == 0) || pos_equal(pos1, pos2))
	{
		log("Positions are equal");
		return;
	}

	if (x > y)
	{	
		if (pos1[X]-pos2[X] > 0 && possible[LEFT])
		{
			--pos1[X];
			log("Moving left");
			return;
		}
		else if (possible[RIGHT])
		{
			++pos1[X];
			log("Moving right");
			return;
		}
		else
		{
			random_move(&NPC.pos);
		}
	}
	
	if (x < y)
	{
		if (pos1[Y]-pos2[Y] > 0 && possible[UP])
		{
			--pos1[Y];
			log("Moving up");
			return;
		}
		else if (possible[DOWN])
		{
			++pos1[Y];
			log("Moving down");
			return;
		}
		else
		{
			random_move(&NPC.pos);
		}
	}
	
	if (x == 0)
	{
		if (pos1[Y]-pos2[Y] > 0 && possible[UP])
		{
			--pos1[Y];
			log("Moving up");
			return;
		}
		else if (possible[DOWN])
		{
			++pos1[Y];
			log("Moving down");
			return;
		}
		else
		{
			random_move(&NPC.pos);
		}
	}
	
	if (y == 0)
	{	
		if (pos1[X]-pos2[X] > 0 && possible[LEFT])
		{
			--pos1[X];
			log("Moving left");
			return;
		}
		else if (possible[RIGHT])
		{
			++pos1[X];
			log("Moving right");
			return;
		}
		else
		{
			random_move(&NPC.pos);
		}
	}

	log("No direction picked. Picking random one");
	random_move(&NPC.pos);
}

#endif