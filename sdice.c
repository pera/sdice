#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Size of the dictionary */
#define DSIZE 1024
#define BITMASK (DSIZE - 1)

/* Maximum number of words */
#define MAXPHRASE 100

/* Size of the read buffer */
#define BSIZE (MAXPHRASE * 2)

char *words[DSIZE] = {
	"acid", "acorn", "acre", "acts", "afar", "affix", "aged", "agent",
	"agile", "aging", "agony", "ahead", "aide", "ajar", "alarm", "alias",
	"alibi", "alien", "alike", "aloe", "aloft", "aloha", "alone", "alpha",
	"alto", "amend", "amino", "ample", "amuse", "angel", "angle", "apple",
	"april", "apron", "aqua", "area", "arena", "argue", "arise", "armed",
	"armor", "army", "aroma", "array", "arson", "ashen", "atlas", "atom",
	"attic", "audio", "avert", "avoid", "awake", "award", "awoke", "axis",
	"bacon", "badge", "bagel", "baggy", "baked", "balmy", "banjo", "barge",
	"barn", "bash", "basil", "batch", "baton", "bats", "blade", "blank",
	"blast", "blaze", "bleak", "blend", "bless", "blimp", "blink", "bloat",
	"blob", "blunt", "blush", "boast", "boat", "body", "boil", "bolt",
	"boned", "bonus", "bony", "book", "booth", "boss", "botch", "both",
	"boxer", "breed", "bribe", "brick", "bride", "brim", "bring", "brisk",
	"broad", "broil", "broke", "brook", "brush", "buck", "buggy", "bulge",
	"bully", "bunch", "bunny", "bunt", "buzz", "cable", "cache", "cadet",
	"calm", "cameo", "canal", "candy", "cane", "canon", "card", "cargo",
	"carol", "carry", "carve", "case", "cash", "cause", "cedar", "chain",
	"chant", "chaos", "charm", "chase", "cheek", "chef", "chess", "chief",
	"chili", "chip", "chomp", "chow", "chuck", "chump", "chunk", "churn",
	"chute", "cider", "cinch", "city", "civic", "clad", "claim", "clamp",
	"clash", "clean", "cleft", "clerk", "click", "cling", "clip", "cloak",
	"clone", "cloth", "cloud", "clump", "coach", "coast", "coat", "cocoa",
	"coil", "coke", "cola", "come", "comic", "comma", "copy", "coral",
	"cork", "cost", "couch", "cover", "cozy", "craft", "cramp", "crane",
	"crawl", "crazy", "creme", "crept", "crib", "cried", "crisp", "crook",
	"crop", "cross", "crowd", "crumb", "crush", "cult", "cupid", "cure",
	"curry", "curse", "curvy", "cushy", "cycle", "daily", "dance", "dandy",
	"darn", "dash", "data", "dawn", "deaf", "debit", "debt", "debug",
	"decaf", "deck", "decor", "deed", "delay", "delta", "denim", "dense",
	"dent", "depth", "derby", "desk", "dial", "diary", "dice", "dill",
	"dime", "dimly", "diner", "dingy", "disco", "dish", "ditch", "ditzy",
	"dock", "dodge", "doing", "doll", "dome", "donor", "donut", "down",
	"dowry", "doze", "drab", "drama", "drank", "dress", "dried", "drift",
	"drill", "drive", "drone", "droop", "drove", "drown", "drum", "duck",
	"dude", "dusk", "duty", "dwarf", "dwell", "eagle", "early", "earth",
	"easel", "east", "eaten", "eats", "ebay", "ebony", "ebook", "echo",
	"edge", "eject", "elbow", "elder", "elope", "elude", "elves", "email",
	"emit", "empty", "enter", "entry", "envoy", "envy", "equal", "erase",
	"error", "erupt", "essay", "etch", "evade", "even", "evict", "evil",
	"evoke", "exact", "exit", "fable", "faced", "fact", "fade", "fall",
	"false", "fancy", "fang", "feast", "feed", "femur", "fence", "ferry",
	"fetal", "fetch", "fever", "fiber", "fifth", "film", "final", "finch",
	"five", "flag", "flaky", "flame", "flask", "fled", "flick", "fling",
	"flip", "flirt", "float", "flock", "floss", "flyer", "foam", "foil",
	"folic", "folk", "food", "found", "foyer", "frail", "frame", "fray",
	"fresh", "fried", "frill", "frisk", "from", "front", "froth", "frown",
	"froze", "fruit", "gains", "gala", "game", "gear", "gecko", "geek",
	"genre", "gift", "gills", "given", "glad", "glass", "glide", "gloss",
	"glow", "glue", "goal", "going", "golf", "gong", "good", "gooey",
	"gore", "gown", "grab", "grain", "grant", "grape", "grasp", "grave",
	"green", "grew", "grid", "grief", "grill", "groom", "grope", "growl",
	"grub", "grunt", "guide", "gulf", "gummy", "guru", "gush", "habit",
	"half", "happy", "harm", "hash", "hasty", "hatch", "hate", "haven",
	"hazel", "hazy", "heap", "heave", "hedge", "hefty", "herbs", "hers",
	"hula", "human", "humid", "hump", "hung", "hurry", "hurt", "hush",
	"icing", "icon", "igloo", "image", "iron", "islam", "issue", "item",
	"ivory", "jaws", "jazz", "jeep", "jelly", "jiffy", "jolly", "jolt",
	"judge", "juice", "july", "jumbo", "jump", "junky", "juror", "jury",
	"keep", "kept", "kick", "kilt", "king", "kite", "kitty", "kiwi",
	"knee", "knelt", "koala", "kung", "ladle", "lady", "lain", "lake",
	"lance", "land", "lapel", "large", "lash", "lasso", "latch", "late",
	"lazy", "left", "legal", "lemon", "lend", "level", "life", "lilac",
	"lilly", "lily", "limb", "limes", "line", "lion", "list", "lived",
	"love", "lunar", "lunch", "lung", "lure", "lying", "lyric", "mace",
	"maker", "malt", "mama", "mango", "manor", "many", "march", "mardi",
	"marry", "mash", "match", "mate", "moan", "mocha", "moist", "mold",
	"moody", "morse", "most", "motor", "motto", "mount", "mouse", "mouth",
	"move", "movie", "mower", "mulch", "mule", "mumbo", "mummy", "mural",
	"music", "musky", "nacho", "nail", "name", "nanny", "navy", "near",
	"neon", "nerd", "nest", "niece", "ninja", "ninth", "nutty", "oasis",
	"oboe", "ocean", "olive", "omega", "omen", "onion", "only", "ooze",
	"opal", "open", "opera", "otter", "ouch", "ounce", "outer", "oval",
	"oven", "ozone", "pace", "pagan", "pager", "palm", "panda", "panic",
	"pants", "paper", "park", "party", "pasta", "patch", "path", "patio",
	"payer", "pecan", "penny", "perch", "perky", "perm", "pest", "petal",
	"petri", "petty", "photo", "plant", "plaza", "plead", "plot", "pluck",
	"plug", "poach", "poem", "pogo", "point", "poise", "poker", "polar",
	"polio", "polka", "pond", "poppy", "pork", "poser", "pouch", "pound",
	"pout", "power", "prank", "press", "print", "prior", "prism", "prize",
	"probe", "prong", "proof", "props", "prude", "pull", "pulse", "puma",
	"punch", "punk", "pupil", "puppy", "purr", "purse", "push", "putt",
	"quack", "quake", "query", "quiet", "quill", "quit", "quota", "rabid",
	"race", "radar", "radio", "raft", "rage", "raid", "rake", "rally",
	"ramp", "ranch", "range", "rank", "rash", "raven", "reach", "ream",
	"rebel", "recap", "relax", "relic", "remix", "repay", "repel", "reply",
	"rerun", "reset", "rhyme", "rice", "rigid", "rigor", "rinse", "riot",
	"ripen", "rise", "ritzy", "rival", "river", "roast", "robe", "robin",
	"rock", "rogue", "roman", "romp", "rope", "rover", "royal", "ruby",
	"ruin", "rule", "rumor", "runny", "rush", "sadly", "sage", "said",
	"saint", "salad", "salon", "salsa", "salt", "same", "sandy", "santa",
	"satin", "sauna", "saved", "savor", "scale", "scam", "scarf", "scoff",
	"scold", "scoop", "scope", "scorn", "scout", "scowl", "scrap", "scrub",
	"scuba", "scuff", "sect", "sedan", "self", "send", "sepia", "serve",
	"seven", "shack", "shade", "shaft", "shaky", "sham", "shape", "sharp",
	"shed", "sheep", "shelf", "shine", "ship", "shirt", "shock", "shore",
	"shout", "shove", "shown", "shred", "shrug", "shun", "shush", "sift",
	"silk", "silly", "siren", "sixth", "size", "skate", "skew", "skid",
	"skier", "skirt", "slab", "slack", "slang", "slash", "slate", "sled",
	"sleek", "slept", "slice", "slimy", "sling", "slip", "slob", "slug",
	"slurp", "slush", "small", "smash", "smell", "smile", "smirk", "smog",
	"snack", "snap", "snare", "sneak", "sneer", "sniff", "snore", "snout",
	"snowy", "snub", "snuff", "speak", "speed", "spent", "spew", "spied",
	"spill", "spiny", "spoil", "spoke", "spoof", "sport", "spot", "spray",
	"spree", "spur", "squad", "stack", "staff", "stage", "stain", "stall",
	"stamp", "stand", "stark", "stash", "state", "stays", "steam", "steep",
	"stem", "stick", "sting", "stir", "stock", "stole", "stomp", "stony",
	"stood", "stop", "storm", "stout", "stove", "straw", "strut", "stuck",
	"stud", "stuff", "stump", "stung", "suds", "sugar", "sulk", "surf",
	"sushi", "swab", "swarm", "swear", "sweep", "swell", "swept", "swim",
	"swing", "swipe", "swirl", "swoop", "swore", "syrup", "tacky", "taco",
	"take", "tall", "talon", "tamer", "tank", "taps", "tarot", "tart",
	"task", "taste", "taunt", "thank", "thaw", "theft", "theme", "thigh",
	"thing", "thorn", "those", "throb", "thud", "thumb", "tiara", "tidal",
	"tidy", "tiger", "tile", "tint", "trace", "train", "trap", "trash",
	"treat", "tree", "trend", "trial", "tribe", "trick", "trio", "trout",
	"truce", "trunk", "tulip", "tummy", "turf", "tusk", "tutor", "tutu",
	"tweak", "tweet", "twice", "twins", "twirl", "twist", "uncle", "uncut",
	"undo", "unify", "union", "unit", "untie", "upon", "upper", "urban",
	"used", "usher", "utter", "value", "vapor", "vegan", "venue", "verse",
	"vest", "veto", "vibes", "vice", "video", "view", "viral", "virus",
	"visa", "visor", "vixen", "vocal", "voice", "void", "volt", "voter",
	"vowel", "wafer", "wages", "wagon", "wake", "walk", "wand", "wasp",
	"watch", "water", "wavy", "wheat", "whiff", "whole", "whoop", "wick",
	"widen", "widow", "width", "wife", "wilt", "wimp", "wind", "wipe",
	"wired", "wiry", "wise", "wispy", "wolf", "womb", "wool", "woozy",
	"word", "worry", "wound", "woven", "wrath", "wreck", "wrist", "xerox",
	"yahoo", "yard", "year", "yeast", "yield", "yodel", "yoga", "yoyo",
	"yummy", "zebra", "zero", "zesty", "zeta", "zippy", "zone", "zoom"
};

int main(int argc, char **argv) {
	int fd;
	uint8_t buf[BSIZE];
	unsigned short len;

	if (argc == 1) {
		len = 8;
	} else {
		char *endptr;
		long int n = strtol(argv[1], &endptr, 10);
		if ((!*endptr) && (n > 0) && (n <= MAXPHRASE))
			len = n;
		else
			return EXIT_FAILURE;
	}

	fd = open("/dev/urandom", O_RDONLY);
	if ((fd == -1) || (read(fd, buf, 2*len) == -1))
		return EXIT_FAILURE;

	for (size_t i = 0; i < 2*len; i += 2)
		printf("%s ", words[*(uint16_t*)&buf[i] & BITMASK]);

	putchar(0x0A);

	close(fd);
	memset(buf, 0, sizeof(buf));

	return EXIT_SUCCESS;
}

