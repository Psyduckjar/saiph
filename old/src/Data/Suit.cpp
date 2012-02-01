#include "Data/Suit.h"

#include "Globals.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Suit*> Suit::_suits;

/* protected constructors */
Suit::Suit(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : Armor(name, cost, weight, material, SLOT_SUIT, ac, mc, properties) {
}

/* destructor */
Suit::~Suit() {
}

/* public static methods */
void Suit::init() {
	/* suits */
	create("white dragon scale mail", 900, 40, MATERIAL_DRAGON_HIDE, 9, 0, PROPERTY_COLD | PROPERTY_MAGIC);
	create("gray dragon scales", 700, 40, MATERIAL_DRAGON_HIDE, 3, 0, PROPERTY_MAGICRES);
	create("silver dragon scale mail", 1200, 40, MATERIAL_DRAGON_HIDE, 9, 0, PROPERTY_REFLECTION | PROPERTY_MAGIC);
	create("silver dragon scales", 700, 40, MATERIAL_DRAGON_HIDE, 3, 0, PROPERTY_REFLECTION);
	create("red dragon scales", 500, 40, MATERIAL_DRAGON_HIDE, 3, 0, PROPERTY_FIRE);
	create("scale mail", 45, 250, MATERIAL_IRON, 4, 0, 0);
	create("ring mail", 100, 250, MATERIAL_IRON, 3, 0, 0);
	create("orcish ring mail", 80, 250, MATERIAL_IRON, 2, 1, 0);
	create("leather jacket", 10, 30, MATERIAL_LEATHER, 1, 0, 0);
	create("yellow dragon scale mail", 900, 40, MATERIAL_DRAGON_HIDE, 9, 0, PROPERTY_STONE | PROPERTY_MAGIC | PROPERTY_ACID);
	create("banded mail", 90, 350, MATERIAL_IRON, 6, 0, 0);
	create("white dragon scales", 500, 40, MATERIAL_DRAGON_HIDE, 3, 0, PROPERTY_COLD);
	create("leather armor", 5, 150, MATERIAL_LEATHER, 2, 0, 0);
	create("plate mail", 600, 450, MATERIAL_IRON, 7, 2, 0);
	create("bronze plate mail", 400, 450, MATERIAL_COPPER, 6, 0, 0);
	create("red dragon scale mail", 900, 40, MATERIAL_DRAGON_HIDE, 9, 0, PROPERTY_FIRE | PROPERTY_MAGIC);
	create("orange dragon scale mail", 900, 40, MATERIAL_DRAGON_HIDE, 9, 0, PROPERTY_SLEEP | PROPERTY_MAGIC);
	create("black dragon scale mail", 1200, 40, MATERIAL_DRAGON_HIDE, 9, 0, PROPERTY_DISINT | PROPERTY_MAGIC);
	create("studded leather armor", 15, 200, MATERIAL_LEATHER, 3, 1, 0);
	create("blue dragon scale mail", 900, 40, MATERIAL_DRAGON_HIDE, 9, 0, PROPERTY_SHOCK | PROPERTY_MAGIC);
	create("gray dragon scale mail", 1200, 40, MATERIAL_DRAGON_HIDE, 9, 0, PROPERTY_MAGIC | PROPERTY_MAGICRES);
	create("black dragon scales", 700, 40, MATERIAL_DRAGON_HIDE, 3, 0, PROPERTY_DISINT);
	create("elven mithril-coat", 240, 150, MATERIAL_MITHRIL, 5, 3, 0);
	create("crystal plate mail", 820, 450, MATERIAL_GLASS, 7, 2, 0);
	create("orcish chain mail", 75, 300, MATERIAL_IRON, 4, 1, 0);
	create("blue dragon scales", 500, 40, MATERIAL_DRAGON_HIDE, 3, 0, PROPERTY_SHOCK);
	create("yellow dragon scales", 500, 40, MATERIAL_DRAGON_HIDE, 3, 0, PROPERTY_STONE | PROPERTY_ACID);
	create("splint mail", 80, 400, MATERIAL_IRON, 6, 1, 0);
	create("green dragon scales", 500, 40, MATERIAL_DRAGON_HIDE, 3, 0, PROPERTY_POISON);
	create("orange dragon scales", 500, 40, MATERIAL_DRAGON_HIDE, 3, 0, PROPERTY_SLEEP);
	create("green dragon scale mail", 900, 40, MATERIAL_DRAGON_HIDE, 9, 0, PROPERTY_POISON | PROPERTY_MAGIC);
	create("chain mail", 75, 300, MATERIAL_IRON, 5, 1, 0);
	create("dwarvish mithril-coat", 240, 150, MATERIAL_MITHRIL, 6, 3, 0);

	/* aliases */
	addToMap("crude ring mail", _suits["orcish ring mail"]);
	addToMap("crude chain mail", _suits["orcish chain mail"]);
}

const map<const string, const Suit*>& Suit::suits() {
	return _suits;
}

/* protected static methods */
void Suit::addToMap(const string& name, const Suit* suit) {
	_suits[name] = suit;
	Armor::addToMap(name, suit);
}

/* private static methods */
void Suit::create(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new Suit(name, cost, weight, material, ac, mc, properties));
}
