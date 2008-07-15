#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : saiph(saiph) {
	memset(check_item, false, sizeof (check_item));
	check_item[WEAPON] = true;
	check_item[ARMOR] = true;
	check_item[RING] = true;
	check_item[AMULET] = true;
	check_item[TOOL] = true;
	check_item[FOOD] = true;
	check_item[POTION] = true;
	check_item[SCROLL] = true;
	check_item[SPELLBOOK] = true;
	check_item[WAND] = true;
	check_item[GOLD] = true;
	check_item[GEM] = true;
	check_item[STATUE] = true;
	// skipping boulder as that's a special item
	check_item[IRON_BALL] = true;
	check_item[CHAINS] = true;
	check_item[VENOM] = true;
}

/* methods */
void Loot::command(string *command) {
	*command = action;
}

int Loot::finish() {
	return 0;
}

void Loot::inspect(const Point &point) {
	unsigned char s = saiph->world->view[point.row][point.col];
	if (check_item[s]) {
		/* we're interested in this symbol */
		if (stashes[saiph->current_branch][saiph->current_level][point.row][point.col].items.size() > 0) {
			/* we know of a stash here already */
			if (stashes[saiph->current_branch][saiph->current_level][point.row][point.col].top_item == s) {
				/* top item is the same as last time we checked.
				 * FIXME: is there any point checking the place again? */
				return;
			} else {
				/* top item changed, we should check this stash again */
			}
		} else {
			/* we've not seen a stash here before, we should check */
		}
	} else if (stashes[saiph->current_branch][saiph->current_level][point.row][point.col].items.size() > 0) {
		/* there used to be a stash here, but now it's gone */
		stashes[saiph->current_branch][saiph->current_level][point.row][point.col].items.clear();
		for (list<Coordinate>::iterator sl = stash_locations.begin(); sl != stash_locations.end(); ++sl) {
			if (sl->branch == saiph->current_branch && sl->level == saiph->current_level && sl->row == point.row && sl->col == point.col) {
				/* found the stash, erase it */
				stash_locations.erase(sl);
				break;
			}
		}
	}
	return;
}

int Loot::parseMessages(string *messages) {
	string::size_type pos = messages->find(MESSAGE_YOU_SEE_HERE, 0);
	if (pos != string::npos) {
		/* one item on the floor */
		pos += sizeof (MESSAGE_YOU_SEE_HERE) - 1;
		string::size_type length = messages->find(".  ", pos);                                                                           
		if (length != string::npos) {                                                                                                          
			length = length - pos;
			int b = saiph->current_branch;
			int l = saiph->current_level;
			int r = saiph->world->player.row;
			int c = saiph->world->player.col;
			vector<Item> *stash = &stashes[b][l][r][c].items;
			if (stash->size() > 0) {
				/* we know about this stash already.
				 * ditch the previous content of this stash */
				stash->clear();
			} else {
				/* new stash, add it to stash_locations */
				stash_locations.push_back(Coordinate(b, l, r, c));
			}
			parseMessageItem(messages->substr(pos, length), stash);
		}
	}
	pos = messages->find(MESSAGE_THINGS_THAT_ARE_HERE, 0);
	if (pos != string::npos) {
		/* multiple items on the floor */
		int b = saiph->current_branch;
		int l = saiph->current_level;
		int r = saiph->world->player.row;
		int c = saiph->world->player.col;
		vector<Item> *stash = &stashes[b][l][r][c].items;
		if (stash->size() > 0) {
			/* we know about this stash already.
			 * ditch the previous content of this stash */
			stash->clear();
		} else {
			/* new stash, add it to stash_locations */
			stash_locations.push_back(Coordinate(b, l, r, c));
		}
		pos = messages->find("  ", pos);
		while (pos != string::npos) {
			pos += 2;
			string::size_type length = messages->find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			parseMessageItem(messages->substr(pos, length), stash);
			pos += length;
		}
	}
	return 0;
}

/* private methods */
void Loot::parseMessageItem(const string &message, vector<Item> *stash) {
	char amount[16];
	char name[512];
	int matched = sscanf(message.c_str(), GET_SINGLE_ITEM, amount, name);
	if (matched != 2) {
		cerr << "ERROR: matched " << matched << ", expected 2" << endl;
		exit(1);
	}
	/* figure out amount of items */
	int count;
	if (amount[0] < '0' || amount[0] > '9')
		count = 1; // "a", "an" or "the" <item>
	else    
		count = atoi(amount);
	/* add item to stash */
	stash->push_back(Item(name, count));
}
