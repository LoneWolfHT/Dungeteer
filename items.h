#ifndef DUNGETEER_ITEMS
#define DUNGETEER_ITEMS

struct item items[] = {
	{/*Gold*/
		"Gold",
		"Used for buying things",
		{"You would rather use the gold to buy digestible food", FALSE},
		{FALSE, 0}
	},
	{/*Sword*/
		"Sword",
		"A regular sword",
		{"A sword is pretty hard to chew on", FALSE},
		{TRUE, 3}
	},
};

#endif