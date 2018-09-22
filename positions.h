void random_move(pos)
int *pos[];
{
	int possible[4] = {TRUE, TRUE, TRUE, TRUE};
	int to;

	static int last = -1;

	if (dungeon.map[*pos[Y]+1][*pos[X]] != ' ')
		possible[DOWN] = FALSE;

	if (dungeon.map[*pos[Y]-1][*pos[X]] != ' ')
		possible[UP] = FALSE;

	if (dungeon.map[*pos[Y]][*pos[X]+1] != ' ')
		possible[RIGHT] = FALSE;

	if (dungeon.map[*pos[Y]][*pos[X]-1] != ' ')
		possible[LEFT] = FALSE;

	while (1)
	{
		int dir = mrand(0, 3);

		if (possible[dir] && dir != last)
		{
			to = dir;
			break;
		}
	}

	switch (to)
	{
		case UP:
		{
			--pos[Y];
			break;
		}
		case DOWN:
		{
			++pos[Y];
			break;
		}
		case LEFT:
		{
			--pos[X];
			break;
		}
		case RIGHT:
		{
			++pos[X];
			break;
		}
	}
}

int dist(pos1, pos2)
int pos1[];
int pos2[];
{
	int distance = 0;

	pos2[Y] -= pos1[Y];
	pos2[X] -= pos1[X];

	for (int i = 0; pos2[Y] != 0 || pos2[X] != 0; ++i)
	{
		if (pos2[Y] > 0 && pos2[X] > 0) // y > 0 and x > 0
		{
			--pos2[Y];
			--pos2[X];
			++distance;
		}
		else if (pos2[Y] < 0 && pos2[X] < 0) //y < 0 and x < 0
		{
			++pos2[Y];
			++pos2[X];
			++distance;
		}
		else if (pos2[Y] > 0 && pos2[X] < 0) // y > 0 and x < 0
		{
			--pos2[Y];
			++pos2[X];
			++distance;
		}
		else if (pos2[Y] < 0 && pos2[X] > 0) // y < 0 and x > 0
		{
			++pos2[Y];
			--pos2[X];
			++distance;
		}
		else if (pos2[Y] == 0) // y == 0
		{
			if (pos2[X] > 0) // x > 0
			{
				--pos2[X];
				++distance;
			}
			else if (pos2[X] < 0) // x < 0
			{
				++pos2[X];
				++distance;
			}
		}
		else if (pos2[X] == 0) // x == 0
		{
			if (pos2[Y] > 0) // y > 0
			{
				--pos2[Y];
				++distance;
			}
			else if (pos2[Y] < 0) // y < 0
			{
				++pos2[Y];
				++distance;
			}
		}
		else
		{
			int x = setsign(pos2[X], '+');
			int y = setsign(pos2[Y], '+');

			if (x > y)
			{	
				if (pos2[X] > 0)
					--pos2[X];
				else
					++pos2[X];

				++distance;
			}
			else if (x < y)
			{
				if (pos2[Y] > 0)
					--pos2[Y];
				else
					++pos2[Y];
				
				++distance;
			}
		}
	}

	return(distance);
}

void getroute(pos1, pos2)
int *pos1[], pos2[];
{
					 /*Top, Bottom, Right, Left*/
	int possible[4] = {TRUE, TRUE, TRUE, TRUE};

	if (dungeon.map[*pos1[Y]+1][*pos1[X]] != ' ')
		possible[DOWN] = FALSE;

	if (dungeon.map[*pos1[Y]-1][*pos1[X]] != ' ')
		possible[UP] = FALSE;

	if (dungeon.map[*pos1[Y]][*pos1[X]+1] != ' ')
		possible[RIGHT] = FALSE;

	if (dungeon.map[*pos1[Y]][*pos1[X]-1] != ' ')
		possible[LEFT] = FALSE;

	int x = setsign(pos1[X]-pos2[X], '+');
	int y = setsign(pos1[Y]-pos2[Y], '+');

	if ((!x && !y) || pos_equal(pos1, pos2))
		return;

	if (x > y)
	{	
		if (pos1[X]-pos2[X] > 0 && possible[LEFT])
		{
			--pos1[X];
			return;
		}
		else if (possible[RIGHT])
		{
			++pos1[X];
			return;
		}
	}
	else if (x < y)
	{
		if (pos1[Y]-pos2[Y] > 0 && possible[DOWN])
		{
			--pos1[Y];
			return;
		}
		else if (possible[UP])
		{
			++pos1[Y];
			return;
		}
	}
	else if (x == 0)
	{
		if (pos1[Y]-pos2[Y] > 0 && possible[DOWN])
		{
			--pos1[Y];
			return;
		}
		else if (possible[UP])
		{
			++pos1[Y];
			return;
		}
	}
	else if (y == 0)
	{	
		if (pos1[X]-pos2[X] > 0 && possible[LEFT])
		{
			--pos1[X];
			return;
		}
		else if (possible[RIGHT])
		{
			++pos1[X];
			return;
		}
	}
}