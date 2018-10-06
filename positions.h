#ifndef DUNGETEER_POS
#define DUNGETEER_POS

position random_move(pos)
position pos;
{
	position newpos = {0, 0};
	int limit = 0;

	while (getpos(newpos) != ' ')
	{
		int random = mrand(1, 4);

		fprintf(logfile, "\n[Random_Move] value of random is %d", random);

		switch (random)
		{
			case 1:
				newpos = pos_up(pos); break;
			case 2:
				newpos = pos_down(pos); break;
			case 3:
				newpos = pos_left(pos); break;
			case 4:
				newpos = pos_right(pos); break;
		}

		if (limit++ >= 100)
		{
			newpos = pos;
			break;
		}
	}

	return(newpos);
}

position pos_add(pos1, pos2)
position pos1, pos2;
{
	position pos;

	pos.y = pos1.y + pos2.y;
	pos.x = pos1.x + pos2.x;

	return(pos);
}

position pos_sub(pos1, pos2)
position pos1, pos2;
{
	position pos;

	pos.y = pos1.y - pos2.y;
	pos.x = pos1.x - pos2.x;

	return(pos);
}

position getroute(pos1, pos2)
position pos1, pos2;
{
	position dir = pos_normalized(pos_sub(pos2, pos1));
	position pos = pos_add(pos1, dir);

	if (ROOM.room[pos.y][pos.x] == ' ' && !pos_equal(pos, player.pos))
	{
		return(pos);
	}
	else
	{
		return(random_move(pos1));
	}
}

int distance(pos1, pos2)
position pos1, pos2;
{
	position pos = pos_sub(pos2, pos1);

	return(ftoi(sqrt(pow(itof(pos.x), 2.0)+pow(itof(pos.y), 2.0))));
}

position pos_normalized(pos)
position pos;
{
	if (pos.x >= 1)
		pos.x = 1;
	else if (pos.x <= -1)
		pos.x = -1;
	else
		pos.x = 0;

	if (pos.y >= 1)
		pos.y = 1;
	else if (pos.y <= -1)
		pos.y = -1;
	else
		pos.y = 0;

	return(pos);
}

int pos_equal(pos1, pos2)
position pos1, pos2;
{
	if (pos1.y == pos2.y && pos1.x == pos2.x)
		return(TRUE);

	return(FALSE);
}

#endif