#include "Actions/Drop.h"

#include <sstream>
#include "EventBus.h"
#include "Inventory.h"
#include "World.h"
#include "Events/WantItems.h"

using namespace action;

/* constructors/destructor */
Drop::Drop(analyzer::Analyzer* analyzer, int priority, bool safe_stash) : Action(analyzer), _drop("D", priority), _close_page(CLOSE_PAGE, PRIORITY_CONTINUE_ACTION), _look(":", PRIORITY_LOOK), _safe_stash(safe_stash), _keys() {
}

Drop::~Drop() {
}

/* methods */
int Drop::id() {
	return ID;
}

const Command& Drop::command() {
	switch (_sequence) {
	case 0:
		return _drop;

	case 1:
		return _drop_item;

	case 2:
		return _close_page;

	case 3:
		return _look;

	default:
		return Action::NOOP;
	}
}

void Drop::update(const std::string& messages) {
	std::string::size_type pos = std::string::npos;
	if (_sequence == 0) {
		if ((pos = messages.find(MESSAGE_DROP_WHICH_ITEMS)) != std::string::npos) {
			/* should always happen, which items do we want to drop? */
			_sequence = 1;
		} else {
			/* shouldn't really happen, maybe we don't have anything to drop? */
			Inventory::update();
			_sequence = 4;
		}
	} else if (_sequence == 1 && _keys.size() <= 0) {
		/* all the items we don't want on this page has been selected for dropping, close page */
		_sequence = 2;
	} else if (_sequence == 2) {
		if (World::menu()) {
			/* more than 1 page */
			_sequence = 1;
			pos = 0;
		} else {
			/* all pages shown */
			Inventory::update();
			_sequence = 3;
		}
	} else if (_sequence == 3) {
		/* looked at ground */
		_sequence = 4;
	}
	if (_sequence == 1 && _keys.size() <= 0) {
		/* figure out which items we would like to drop */
		event::WantItems wi(true, _safe_stash);
		while (pos != std::string::npos && messages.size() > pos + 6) {
			pos += 6;
			std::string::size_type length = messages.find("  ", pos);
			if (length == std::string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-') {
				std::map<unsigned char, Item>::iterator i = Inventory::items().find(messages[pos - 4]);
				if (i != Inventory::items().end()) {
					if (Inventory::slotForKey(i->first) != ILLEGAL_SLOT) {
						/* don't drop items in slots */
					} else if (i->second.name() == "gold piece") {
						/* don't drop money */
					} else {
						i->second.want(0);
						wi.addItem(messages[pos - 4], i->second);
					}
				} else {
					/* this isn't supposed to happen (inventory not updated?) */
					/* it will happen for gold, though, but we won't allow dropping gold like this */
					if (messages.substr(pos, length).find("gold piece") == std::string::npos)
						wi.addItem(messages[pos - 4], Item(messages.substr(pos, length), 0));
				}
			}
			pos += length;
		}
		/* broadcast event */
		EventBus::broadcast(&wi);
		/* drop stuff no analyzer wanted */
		std::ostringstream tmp;
		for (std::map<unsigned char, Item>::const_iterator i = wi.items().begin(); i != wi.items().end(); ++i) {
			if (i->second.count() <= 0 || i->second.want() <= 0) {
				/* drop for beatitude or we don't want the item */
				_keys.push(std::string(1, i->first));
				continue;
			} else if (i->second.count() <= i->second.want()) {
				/* we want all of these items, don't drop them */
				continue;
			}
			/* drop some of these items */
			tmp.str("");
			tmp << (i->second.count() - i->second.want()) << i->first;
			_keys.push(tmp.str());
		}
	}
	if (_sequence == 1) {
		if (_keys.size() > 0) {
			/* make _drop_item the first item in our list of stuff to drop */
			_drop_item = Command(_keys.front(), PRIORITY_SELECT_ITEM);
			_keys.pop();
		} else {
			/* we shouldn't get here, but apparently we did. close page */
			_sequence = 2;
		}
	}
}
