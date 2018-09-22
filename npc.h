struct npc rat = {
	"Rat", //Name
	'R', //Symbol
	5, //HP
	5, //MAX_HP
	{}, //Pos
	1, //dmg
	7, //View range
	{ITEM_GOLD, 7} //item to drop on death
};

void move_npcs()
{
	for (int i = 0; i < 10; ++i)
	{
		dungeon.npcs[i] = get_npc_by_name(npc(i).name);

		if (!npc(i).hp)
			continue;

		if (dist(player.pos, npc(i).pos) <= npc(i).view_range) //Need to add line of sight check
		{
			getroute(npc(i).pos, player.pos);
		}
		else
		{
			random_move(npc(i).pos);
		}
	}
}

struct npc get_npc_by_name(name)
char *name;
{
	struct npc none = EMPTY;
	if (name == "Rat")
		return(rat);

	return(none);
}