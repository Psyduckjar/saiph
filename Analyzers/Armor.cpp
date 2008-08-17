#include "Armor.h"

/* constructors */
Armor::Armor(Saiph *saiph) : Analyzer("Armor"), saiph(saiph), wear_more(false) {
}

/* methods */
void Armor::analyze() {
	if (saiph->inventory_changed || wear_more) {
		wear_more = true;
		wearArmor();
	}
}

void Armor::parseMessages(const string &messages) {
	if (saiph->world->question && !command2.empty() && (messages.find(ARMOR_WHAT_TO_WEAR, 0) != string::npos || messages.find(ARMOR_WHAT_TO_TAKE_OFF, 0) != string::npos)) {
		/* take off or wear something */
		command = command2;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (!saiph->world->question && !command2.empty() && messages.find(ARMOR_YOU_WERE_WEARING, 0) != string::npos) {
		/* took off last piece of armor (no "what do you want to take off?" question then) */
		command2.clear();
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (command == WEAR) {
		/* in case we didn't get to wear the armor */
		priority = ARMOR_WEAR_PRIORITY;
	}
}

bool Armor::request(const Request &request) {
	if (request.request == REQUEST_ARMOR_WEAR) {
		/* player wish to wear this armor */
		if (request.value < 0 || request.value >= ARMOR_SLOTS)
			return false;
		WearArmor wa;
		wa.beatitude = request.status;
		wa.name = request.data;
		armor[request.value].push_back(wa);
		return true;
	}
	return false;
}

/* private methods */
void Armor::wearArmor() {
	/* put on or change armor (which means we'll have to take something off) */
	/* check that we're (still) wearing our preferred armor */
	unsigned char worn[ARMOR_SLOTS];
	memset(worn, 0, sizeof (worn));
	unsigned char best_key[ARMOR_SLOTS];
	memset(best_key, 0, sizeof (best_key));
	int best_armor[ARMOR_SLOTS];
	bool is_cursed[ARMOR_SLOTS];
	for (int s = 0; s < ARMOR_SLOTS; ++s) {
		best_armor[s] = INT_MAX;
		is_cursed[s] = false;
	}
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		for (int s = 0; s < ARMOR_SLOTS; ++s) {
			for (vector<WearArmor>::size_type a = 0; a < armor[s].size(); ++a) {
				if (armor[s][a].name != i->second.name)
					continue;
				if (i->second.additional == "being worn") {
					worn[s] = i->first;
					is_cursed[s] = (i->second.beatitude == CURSED);
				}
				if ((int) a >= best_armor[s])
					continue;
				if ((armor[s][a].beatitude & i->second.beatitude) == 0)
					continue;
				best_key[s] = i->first;
				best_armor[s] = a;
			}
		}
	}
	for (int s = 0; s < ARMOR_SLOTS; ++s) {
		if (best_key[s] == 0 || (worn[s] != 0 && saiph->inventory[worn[s]].name == saiph->inventory[best_key[s]].name))
			continue; // wearing best armor or got no armor to wield
		if (is_cursed[s])
			continue; // the item we're wearing in this slot it cursed and cannot be taken off
		if (s == ARMOR_SUIT || s == ARMOR_SHIRT) {
			/* wish to put on shirt or suit.
			 * if cloak or suit is cursed, this can't be done */
			if (is_cursed[ARMOR_CLOAK] || (s == ARMOR_SHIRT && is_cursed[ARMOR_SUIT])) {
				/* cloak is cursed, or suit is cursed and we're putting on a shirt */
				continue;
			}
			/* are we wearing a cloak? */
			if (worn[ARMOR_CLOAK] != 0) {
				/* yes, we must take it off first */
				command = TAKEOFF;
				command2 = worn[ARMOR_CLOAK];
				priority = ARMOR_WEAR_PRIORITY;
				return;
			}
			if (s == ARMOR_SHIRT) {
				/* are we wearing a suit? */
				if (worn[ARMOR_SUIT] != 0) {
					/* yes, we must take it off first */
					command = TAKEOFF;
					command2 = worn[ARMOR_SUIT];
					priority = ARMOR_WEAR_PRIORITY;
					return;
				}
			}
		}
		if (worn[s] != 0) {
			/* we'll have to take this armor off first */
			command = TAKEOFF;
			command2 = worn[s];
			priority = ARMOR_WEAR_PRIORITY;
			return;
		}
		/* we should put on this piece of armor */
		command = WEAR;
		command2 = best_key[s];
		priority = ARMOR_WEAR_PRIORITY;
		return;
	}
	/* nothing to wear */
	wear_more = false;
	command.clear();
}
