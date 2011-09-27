#include "Analyzers/Quest.h"

#include <sstream>
#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Monster.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Move.h"
#include "Data/Monster.h"
#include "Events/WantItems.h"
#include "Events/QuestStatus.h"

using namespace analyzer;
using namespace event;
using namespace std;

// Quest's main responsibility is to keep Explore informed about when and how to explore the quest dungeons.
// It also handles chatting with the leader, and picking up our rewards.

/* constructors/destructor */
Quest::Quest() : Analyzer("Quest"), _status(QUEST_STATUS_NOT_READY), _portal_level(-1), _seen_bell(false), _seen_arti(false) {
	/* register events */
	EventBus::registerEvent(WantItems::ID, this);

	artifacts[ARCHEOLOGIST] = "Orb of Detection";
	artifacts[BARBARIAN] = "Heart of Ahriman";
	artifacts[CAVEMAN] = "Sceptre of Might";
	artifacts[HEALER] = "Staff of Aesculapius";
	artifacts[KNIGHT] = "Magic Mirror of Merlin";
	artifacts[MONK] = "Eyes of the Overworld";
	artifacts[PRIEST] = "Mitre of Holiness";
	artifacts[RANGER] = "Longbow of Diana";
	artifacts[ROGUE] = "Master Key of Thievery";
	artifacts[SAMURAI] = "Tsurugi of Muramasa";
	artifacts[TOURIST] = "Platinum Yendorian Express Card";
	artifacts[VALKYRIE] = "Orb of Fate";
	artifacts[WIZARD] = "Eye of the Aethiopica";

	accepted_messages[ARCHEOLOGIST] = "Youmustthendefeat";
	accepted_messages[BARBARIAN] = "defeathim,andreturn";
	accepted_messages[CAVEMAN] = "indeedreadynow";
	accepted_messages[HEALER] = "yousenseasmile";
	accepted_messages[KNIGHT] = "Thouarttrulyready";
	accepted_messages[MONK] = "trulyreadynow";
	accepted_messages[PRIEST] = "trulyreadynow";
	accepted_messages[ROGUE] = "onestepbackward";
	accepted_messages[SAMURAI] = "indeedyouareready";
	accepted_messages[TOURIST] = "provenyourselfaworthy";
	accepted_messages[VALKYRIE] = "thatyoucandefeat";
	accepted_messages[WIZARD] = "youtrulyareready";
}

/* methods */
void Quest::parseMessages(const string& messages) {
	if (_status == QUEST_STATUS_READY && World::level().branch() == BRANCH_QUEST && World::level().depth() == 1) {
		// unfortunately, nethack's quest pager inserts spaces unpredictably
		string menu;
		for (string::const_iterator ch = messages.begin(); ch != messages.end(); ++ch) {
			if (*ch != ' ')
				menu.push_back(*ch);
		}

		if (menu.find(accepted_messages[Saiph::role()]) != string::npos)
			setStatus(_portal_level, QUEST_STATUS_GIVEN);
	}

	if (Saiph::position().level() != _portal_level && (messages.find("pleading for help.  ") != string::npos || messages.find("demanding your attendance.  ") != string::npos || messages.find("  Look for a ...ic transporter.  ") != string::npos)) {
		setStatus(Saiph::position().level(), _status);
	}
}

void Quest::analyze() {
	if (Saiph::experience() >= 14 && _status == QUEST_STATUS_NOT_READY)
		setStatus(_portal_level, QUEST_STATUS_READY);
	else if (Saiph::experience() < 14 && _status == QUEST_STATUS_READY)
		setStatus(_portal_level, QUEST_STATUS_NOT_READY);

	// our only active job is to chat with the leader
	if (World::level().depth() != 1 || World::level().branch() != BRANCH_QUEST || _status != QUEST_STATUS_READY)
		return;

	for (map<Point, Monster*>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
		if (!m->second->visible() || !m->second->data() || m->second->data()->sounds() != MS_LEADER)
			continue;
		_leader_pos = m->first;
	}

	if (_leader_pos.row() < 0)
		return;

	Tile& go = World::shortestPath(_leader_pos);

	if (go.distance() > 1 && go.cost() < UNPASSABLE)
		World::setAction(new action::Move(this, go, PRIORITY_QUEST_GET_QUEST, go.cost()));
}

void Quest::onEvent(event::Event * const event) {
	if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.name() == "silver bell" || i->second.name() == "Bell of Opening") {
				_seen_bell = true;
				i->second.want(i->second.count());
			} else if (i->second.name() == artifacts[Saiph::role()]) {
				_seen_arti = true;
				i->second.want(i->second.count());
			}

			if (_status != QUEST_STATUS_COMPLETED && _seen_bell && _seen_arti)
				setStatus(_portal_level, QUEST_STATUS_COMPLETED);
		}
	}
}

/* private methods */
void Quest::setStatus(int plevel, int status) {
	QuestStatus qs(status, plevel);
	_status = status;
	_portal_level = plevel;
	EventBus::broadcast(&qs);
	Debug::custom(name()) << "Setting quest status to " << status << " (portal at " << plevel << ")" << endl;
}
