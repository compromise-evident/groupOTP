/// OTP - One-time pad for unlimited file encryption between 2 people.          Run it: "apt install g++ geany". Open this in Geany. Hit F9 once. F5 to run.


/* Version 4.0.0   +   rolling-code 3.1.0
Send in the order encrypted, and decrypt in the order received!
Although there are no limits, you should get new keys occasionally.
 ______________________________________________________________________________
/                                                                              \
|     Alice makes keys:      * incoming                   * outgoing           |
|                                                                              |
|      Bob gets copies:      * incoming                   * outgoing           |
|                               (copy)                       (copy)            |
|                                                                              |
|                            with which                   with which           |
|                           Bob encrypts,               Alice encrypts,        |
|                          Alice decrypts!               Bob decrypts!         |
\_____________________________________________________________________________*/

#include <fstream>
#include <iostream>
using namespace std;

int main()
{	ifstream in_stream;
	ofstream out_stream;
	
	cout << "\n(1) Encrypt outgoing"
	     << "\n(2) Decrypt incoming"
	     << "\n(3) Get keys"
	
	     << "\n\nOption: ";
	
	int user_option; cin >> user_option;
	if((user_option != 1) && (user_option != 2) && (user_option != 3)) {cout << "\nInvalid.\n"; return 0;}
	
	
	
	
	
	//______________________________________________________Get_keys__________________________________________________//
	if(user_option == 3)
	{	//Checks if keys already exist.
		in_stream.open("keys");
		if(in_stream.fail() == false) {cout << "\nKeys already exist, run OTP.cpp in a different folder.\n"; in_stream.close(); return 0;}
		in_stream.close();
		
		
		
		
		
		//The following is verbatim from rollingcode.cpp, except for cout, extraction, & user knobs 1 & 2.
		long long code_length_in_thousands = 2000; //Must be equal with whom codes are to be symmetric. DEFAULT = 2000.
		bool RAM_Unix_time_supplement = false;   //Set to true for codes of unique randomness, even with the same seeds file. DEFAULT = false.
		bool absurd_protection_against_cryptanalysis = true; //Slow, code_length_in_thousands becomes "actual code length."  DEFAULT = true.
		
		//Creates seeds file if missing.
		const char seeds_file_name[50] = {"RC_seeds"};
		char garbage_byte_for_seeds_file;
		{	//..........Checks if seeds file exists.
			in_stream.open(seeds_file_name);
			bool existence_of_seeds_file = true;
			if(in_stream.fail() == true) {existence_of_seeds_file = false;}
			in_stream.close();
			
			if(existence_of_seeds_file == false)
			{	//..........Gets path to file from user.
				cout << "Drop/enter any file of 1,000+ random first bytes:\n";
				
				//..........Gets path then fixes it if drag-n-dropped, regardless of single-quote presence and "enter"
				//..........not being cleared, meaning you can have options before this, where the user presses enter.
				char path_to_file_for_seeds[10000] = {'\0'};
				{	for(int a = 0; a < 10000; a++) {path_to_file_for_seeds[a] = '\0';}
					cin.getline(path_to_file_for_seeds, 10000);
					if(path_to_file_for_seeds[0] == '\0')
					{	for(int a = 0; a < 10000; a++) {path_to_file_for_seeds[a] = '\0';}
						cin.getline(path_to_file_for_seeds, 10000);
					}
					if(path_to_file_for_seeds[0] == '\0') {cout << "\nNo path given.\n"; return 0;}
					
					//..........Removes last space in path_to_file_for_seeds[].
					bool existence_of_last_space = false;
					for(int a = 9999; a > 0; a--)
					{	if(path_to_file_for_seeds[a] != '\0')
						{	if(path_to_file_for_seeds[a] == ' ') {path_to_file_for_seeds[a] = '\0'; existence_of_last_space = true;}
							break;
						}
					}
					
					//..........Removes encapsulating single-quotes in path_to_file_for_seeds[].
					bool existence_of_encapsulating_single_quotes = false;
					if(path_to_file_for_seeds[0] == '\'')
					{	for(int a = 0; a < 9999; a++)
						{	path_to_file_for_seeds[a] = path_to_file_for_seeds[a + 1];
							if(path_to_file_for_seeds[a] == '\0') 
							{	if(path_to_file_for_seeds[a - 1] != '\'') {cout << "\nBad path.\n"; return 0;}
								path_to_file_for_seeds[a - 1] = '\0';
								existence_of_encapsulating_single_quotes = true;
								break;
							}
						}
					}
					
					//..........Replaces all "'\''" with "'" in path_to_file_for_seeds[].
					int single_quote_quantity = 0;
					for(int a = 0; a < 10000; a++)
					{	if(path_to_file_for_seeds[a] == '\'') {single_quote_quantity++;}
					}
					
					if((single_quote_quantity                     >    0)
					&& (existence_of_last_space                  == true)
					&& (existence_of_encapsulating_single_quotes == true))
					{	if((single_quote_quantity % 3) != 0) {cout << "\nBad path.\n"; return 0;}
						
						for(int a = 0; a < 9997; a++)
						{	if(path_to_file_for_seeds[a] == '\'')
							{	int temp = (a + 1);
								for(; temp < 9997; temp++)
								{	path_to_file_for_seeds[temp] = path_to_file_for_seeds[temp + 3];
								}
							}
						}
					}
				}
				
				//..........Loads seeds[] with given file's 1,000 Bytes.
				in_stream.open(path_to_file_for_seeds);
				if(in_stream.fail() == true) {cout << "\nNo such file.\n"; in_stream.close(); return 0;}
				unsigned int seeds[1000];
				for(int a = 0; a < 1000; a++)
				{	in_stream.get(garbage_byte_for_seeds_file);
					if(in_stream.eof() == true)
					{	for(int b = 0; b < 1000; b++) {seeds[b] = 0; seeds[b] = 4294967295;} //..........Overwrites RAM of array unsigned int seeds[1000].
						cout << "\n\nFile too small.\n\n"; in_stream.close(); return 0;
					}
					
					int normal_byte = garbage_byte_for_seeds_file;
					if(normal_byte < 0) {normal_byte += 256;}
					seeds[a] = normal_byte;
				}
				in_stream.close();
				
				//..........Creates seeds file (digits, and better distributed.)
				out_stream.open(seeds_file_name);
				srand(seeds[0]);
				for(int a = 0; a < 1000; a++)
				{	long long temp = ((seeds[a] + rand()) % 10);
					out_stream << char(temp + 48);
				}
				out_stream << "\n\n\n\nPRIVATE! DO NOT SHARE!\n";
				out_stream.close();
				
				//..........Overwrites RAM of array unsigned int seeds[1000].
				for(int a = 0; a < 1000; a++) {seeds[a] = 0; seeds[a] = 4294967295;}
			}
		}
		
		
		
		
		
		//Generates randomness.
		{	long long temp_overflow_for_randomness;
			cout << "Wait...\n";
			
			//..........Loads seeds[] with seeds file.
			in_stream.open(seeds_file_name);
			unsigned int seeds[1000];
			for(int a = 0; a < 1000; a++)
			{	in_stream.get(garbage_byte_for_seeds_file);
				int normal_byte = garbage_byte_for_seeds_file;
				if(normal_byte < 0) {normal_byte += 256;}
				seeds[a] = normal_byte;
				
				seeds[a] -= 48;
				if((in_stream.eof() == true) || (seeds[a] > 9))
				{	for(int b = 0; b < 1000; b++) {seeds[b] = 0; seeds[b] = 4294967295;} //..........Overwrites RAM of array unsigned int seeds[1000].
					cout << "\n\nBad seeds file.\n\n"; in_stream.close(); return 0;
				}
			}
			in_stream.close();
			
			//..........Makes 100 10-digit actual seeds based on seeds[]. (Strings together 10 contiguous digits, 100 times.)
			unsigned int actual_seeds[100] = {0};
			int seeds_read_bookmark = 0;
			for(int a = 0; a < 100; a++)
			{	temp_overflow_for_randomness = 0;
				for(int b = 0; b < 10; b++)
				{	temp_overflow_for_randomness += seeds[seeds_read_bookmark];
					if(b < 9) {temp_overflow_for_randomness *= 10;}
					seeds_read_bookmark++;
				}
				
				actual_seeds[a] = (temp_overflow_for_randomness % 4294967296);
			}
			
			/*..........Supplements actual_seeds[] for unique randomness. (100 10-digit values
			            created from garbage RAM are added to the 100 10-digit actual_seeds[].)
			            Even if all zeros as supplement, actual_seeds[] take the weight (seeds file.)
			            Declare 100k or 1M unsigned int array; there will be ~628 garbage items at end.*/
			if(RAM_Unix_time_supplement == true)
			{	unsigned int RAM_garbage[100000];
				temp_overflow_for_randomness = (time(0) % 4294967296); //..........Adds Unix time to actual_seeds[0]. (temp_overflow_for_randomness is never reset; each actual_seed[] is supplemented with incremental, and unique.)
				
				for(int a = 0; a < 100; a++) //..........Adds sum of every RAM_garbage[] to actual_seeds[0], then sum of every other to actual_seeds[1], then sum of every third to actual_seeds[2], and so on.
				{	int skip = (a + 1);
					for(int b = 0; b < 100000; b += skip) {temp_overflow_for_randomness += RAM_garbage[b]; temp_overflow_for_randomness %= 4294967296;}
					
					temp_overflow_for_randomness += actual_seeds[a];
					actual_seeds[a] = (temp_overflow_for_randomness % 4294967296);
				}
				
				for(int a = 0; a < 100000; a++) {RAM_garbage[a] = 0; RAM_garbage[a] = 4294967295;} //..........Overwrites RAM of array unsigned int RAM_garbage[100000].
			}
			
			
			
			
			
			//..........Generator house.
			out_stream.open("Code", ios::app);
			unsigned int randomness[1000] = {0};
			for(long long a = 0; a < code_length_in_thousands; a++)
			{	
				//..........Generator powerhouse.
				for(int b = 0; b < 100; b++)
				{	srand(actual_seeds[b]);
					/*#######*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
					#####'`                                                                  `'#####
					###'             You may freely replace the following few items.            '###
					##            Here, randomness is generated  1,000 bytes per round.           ##
					#,         What happens here, must happen 100 times.  For each time,          ,#
					#'        continue scrambling the unsigned int randomness[1000] array.        '#
					##        srand() will use a different seed for you, each time  (100.)        ##
					###,       Leave randomness[] with at least values  0 to 9  when done.      ,###
					#####,.                                                                  .,#####
					##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#######*/
					for(int c = 0; c < 1000; c++) {randomness[c] += rand(); randomness[c] %= 256;} //..........Fills randomness[] (100x per 1,000-char code.)
					temp_overflow_for_randomness = (actual_seeds[99 - b] + rand()); //..........Modifies inverse actual_seeds[].
					actual_seeds[99 - b] = (temp_overflow_for_randomness % 4294967296);
					
					srand(actual_seeds[99 - b]);  //..........Now using that inverse seed.
					for(int c = 0; c < 1000; c++) //..........Swaps EACH & EVERY element in randomness[] with randomly chosen (100,000x per 1,000-char code.)
					{	int random_element = (rand() % 1000);
						for(; random_element == c;) {random_element = (rand() % 1000);}
						
						unsigned int temp_element = randomness[random_element];
						randomness[random_element] = randomness[c];
						randomness[c] = temp_element;
					}
					temp_overflow_for_randomness = (actual_seeds[b] + rand()); //..........Modifies current actual_seeds[].
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				//..........Makes 100 10-digit new actual seeds based on randomness[]. (!!! Adds to current actual_seeds. !!! The generated Code is NOT alone responsible for new actual_seeds. !!!) (Strings together 10 contiguous digits, 100 times.)
				int randomness_read_bookmark = 0;
				for(int b = 0; b < 100; b++)
				{	temp_overflow_for_randomness = 0;
					for(int c = 0; c < 10; c++)
					{	temp_overflow_for_randomness += (randomness[randomness_read_bookmark] % 10);
						if(c < 9) {temp_overflow_for_randomness *= 10;}
						randomness_read_bookmark++;
					}
					
					temp_overflow_for_randomness += actual_seeds[b];
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				if(absurd_protection_against_cryptanalysis == true) {out_stream << char((randomness[500] % 10) + 48);} //..........Writes only 1 code item per round.
				else
				{	/*#######*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
					#####'`                                                                  `'#####
					###'               Append-writes randomness[] to file "Code".               '###
					##                                                                            ##
					#,      Extract rand here!  unsigned int randomness[1000]  here-contains      ,#
					#'      1,000 random values 0 to 255. This array gets all new randomness      '#
					##       every time the program is here. And how many times is it here?       ##
					###,      That's equal to the direct value of code_length_in_thousands.     ,###
					#####,.                                                                  .,#####
					##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#######*/
					for(int b = 0; b < 1000; b++) {out_stream << char((randomness[b] % 94) + 33);}
				}
			}
			out_stream << "\n";
			out_stream.close();
			
			
			
			
			
			//..........Overwrites seeds file.
			out_stream.open(seeds_file_name);
			for(int a = 0; a < 100; a++)
			{	unsigned int place = 10;
				for(int b = 0; b < 9; b++)
				{	if(actual_seeds[a] < place) {out_stream << "0";}
					place *= 10;
				}
				out_stream << actual_seeds[a];
			}
			out_stream << "\n\n\n\nPRIVATE! DO NOT SHARE!\n";
			out_stream.close();
			
			//..........Overwrites RAM of variable long long temp_overflow_for_randomness.
			temp_overflow_for_randomness = 0; temp_overflow_for_randomness = -9223372036854775807; temp_overflow_for_randomness = 9223372036854775807;
			
			//..........Overwrites RAM of arrays unsigned int seeds[1000] and unsigned int randomness[1000].
			for(int a = 0; a < 1000; a++) {seeds[a] = 0; seeds[a] = 4294967295; randomness[a] = 0; randomness[a] = 4294967295;}
			
			//..........Overwrites RAM of array unsigned int actual_seeds[100].
			for(int a = 0; a < 100; a++) {actual_seeds[a] = 0; actual_seeds[a] = 4294967295;}
		}
		
		
		
		
		
		//Makes files "seeds_for_incoming" and "seeds_for_outgoing" in folder "keys" (copies randomness from generated file "Code".)
		system("mkdir keys");
		
		in_stream.open("Code");
		char garbage_byte;
		out_stream.open("keys/seeds_for_incoming");
		for(int a = 0; a < 1000; a++) {in_stream.get(garbage_byte); out_stream.put(garbage_byte);}
		out_stream << "\n\nSeeds are always rolling and in symmetry with the other party."
		           << "\nThe one with file \"2\" uses incoming for outgoing, and outgoing for incoming.\n";
		out_stream.close();
		
		out_stream.open("keys/seeds_for_outgoing");
		for(int a = 0; a < 1000; a++) {in_stream.get(garbage_byte); out_stream.put(garbage_byte);}
		out_stream << "\n\nSeeds are always rolling and in symmetry with the other party."
		           << "\nThe one with file \"2\" uses incoming for outgoing, and outgoing for incoming.\n";
		out_stream.close();
		remove("RC_seeds");
		remove("Code");
		in_stream.close();
		
		out_stream.open("keys/2");
		out_stream << "\nOnly one of you must have this file (key maker had already been asked to remove it.)\n";
		out_stream.close();
		
		cout << "\nDone. Share the \"keys\" folder in person"
		     << "\nunless the other party uses option 3 with"
		     << "\nthe same file of 1,000+ random first bytes."
		
		     << "\n\nIf you're that other party--all done."
		     << "\nElse remove file \"2\" in \"keys\" on your end.\n\n\n";
		
		return 0;
	}
	
	
	
	
	
	//Checks if keys are present.
	in_stream.open("keys");
	if(in_stream.fail() == true) {cout << "\nGet keys first.\n"; in_stream.close(); return 0;}
	in_stream.close();
	
	//Gets path to file from user.
	cout << "Drop/enter file: ";
	
	//..........Gets path then fixes it if drag-n-dropped, regardless of single-quote presence and "enter"
	//..........not being cleared, meaning you can have options before this, where the user presses enter.
	char path_to_file[10000] = {'\0'};
	{	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';}
		cin.getline(path_to_file, 10000);
		if(path_to_file[0] == '\0')
		{	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';}
			cin.getline(path_to_file, 10000);
		}
		if(path_to_file[0] == '\0') {cout << "\nNo path given.\n"; return 0;}
		
		//..........Removes last space in path_to_file[].
		bool existence_of_last_space = false;
		for(int a = 9999; a > 0; a--)
		{	if(path_to_file[a] != '\0')
			{	if(path_to_file[a] == ' ') {path_to_file[a] = '\0'; existence_of_last_space = true;}
				break;
			}
		}
		
		//..........Removes encapsulating single-quotes in path_to_file[].
		bool existence_of_encapsulating_single_quotes = false;
		if(path_to_file[0] == '\'')
		{	for(int a = 0; a < 9999; a++)
			{	path_to_file[a] = path_to_file[a + 1];
				if(path_to_file[a] == '\0') 
				{	if(path_to_file[a - 1] != '\'') {cout << "\nBad path.\n"; return 0;}
					path_to_file[a - 1] = '\0';
					existence_of_encapsulating_single_quotes = true;
					break;
				}
			}
		}
		
		//..........Replaces all "'\''" with "'" in path_to_file[].
		int single_quote_quantity = 0;
		for(int a = 0; a < 10000; a++)
		{	if(path_to_file[a] == '\'') {single_quote_quantity++;}
		}
		
		if((single_quote_quantity                     >    0)
		&& (existence_of_last_space                  == true)
		&& (existence_of_encapsulating_single_quotes == true))
		{	if((single_quote_quantity % 3) != 0) {cout << "\nBad path.\n"; return 0;}
			
			for(int a = 0; a < 9997; a++)
			{	if(path_to_file[a] == '\'')
				{	int temp = (a + 1);
					for(; temp < 9997; temp++)
					{	path_to_file[temp] = path_to_file[temp + 3];
					}
				}
			}
		}
	}
	
	//Checks if file exists (failure can be bad path info as well.)
	in_stream.open(path_to_file);
	if(in_stream.fail() == true) {in_stream.close(); cout << "\n\nNo such file or directory.\n";             return 0;}
	char sniffed_one_file_character;
	in_stream.get(sniffed_one_file_character);
	if(in_stream.eof() == true) {in_stream.close();  cout << "\n\nNothing to process, the file is empty.\n"; return 0;}
	in_stream.close();
	
	//Gets location of the first encountered end-null coming from the left in path_to_file[].
	int path_to_file_null_bookmark;
	for(int a = 0; a < 10000; a++) {if(path_to_file[a] == '\0') {path_to_file_null_bookmark = a; break;}}
	
	//Gets a copy of path_to_file[] in case overwriting raw file after encryption.
	char path_to_file_copy_if_overwriting_raw[10000];
	for(int a = 0; a < 10000; a++) {path_to_file_copy_if_overwriting_raw[a] = path_to_file[a];}
	long long file_size_for_overwriting = 0;
	
	//..........Loads seeds[] with seeds file in folder "keys".
	bool existence_of_file_2 = true;
	in_stream.open("keys/2");
	if(in_stream.fail() == true) {existence_of_file_2 = false;}
	in_stream.close();
	
	if(user_option == 1)
	{	if(existence_of_file_2 == true) {in_stream.open("keys/seeds_for_incoming")           ;}
		else                            {in_stream.open("keys/seeds_for_outgoing")           ;}
		if(in_stream.fail()    == true) {cout << "\nNo seeds?\n"; in_stream.close(); return 0;}
	}
	else
	{	if(existence_of_file_2 == true) {in_stream.open("keys/seeds_for_outgoing")           ;}
		else                            {in_stream.open("keys/seeds_for_incoming")           ;}
		if(in_stream.fail()    == true) {cout << "\nNo seeds?\n"; in_stream.close(); return 0;}
	}
	
	unsigned int seeds[1000]; //..........(From rolling-code.)
	char garbage_byte;
	for(int a = 0; a < 1000; a++)
	{	in_stream.get(garbage_byte);
		int normal_byte = garbage_byte;
		if(normal_byte < 0) {normal_byte += 256;}
		seeds[a] = normal_byte;
		
		seeds[a] -= 48;
		if((in_stream.eof() == true) || (seeds[a] > 9))
		{	for(int b = 0; b < 1000; b++) {seeds[b] = 0; seeds[b] = 4294967295;} //..........Overwrites RAM of array unsigned int seeds[1000].
			cout << "\n\nBad seeds file.\n\n"; in_stream.close(); return 0;
		}
	}
	in_stream.close();
	
	//..........Makes 100 10-digit actual seeds based on seeds[]. (Strings together 10 contiguous digits, 100 times.) (From rolling-code.)
	unsigned int actual_seeds[100] = {0};
	long long temp_overflow_for_randomness;
	int seeds_read_bookmark = 0;
	for(int a = 0; a < 100; a++)
	{	temp_overflow_for_randomness = 0;
		for(int b = 0; b < 10; b++)
		{	temp_overflow_for_randomness += seeds[seeds_read_bookmark];
			if(b < 9) {temp_overflow_for_randomness *= 10;}
			seeds_read_bookmark++;
		}
		
		actual_seeds[a] = (temp_overflow_for_randomness % 4294967296);
	}
	
	unsigned int randomness[1000] = {0};
	int          actual_key[1000] = {0}; //..........Later filled with randomness 0 - 255 based on randomness[], renewed for each kB of the file.
	
	//Gets file size in MB for displaying "x of y MB...".
	in_stream.open(path_to_file);
	long long total_bytes = 0;
	in_stream.get(garbage_byte);
	for(; in_stream.eof() == false;)
	{	in_stream.get(garbage_byte);
		total_bytes++;
	}
	in_stream.close();
	
	long long total_MB = (total_bytes - (total_bytes % 1000000));
	total_MB /= 1000000;
	cout << "\n";
	
	
	
	
	
	//______________________________________________________Encrypt___________________________________________________//
	if(user_option == 1)
	{	//Prepares 2 file streams.
		in_stream.open(path_to_file);
		path_to_file[path_to_file_null_bookmark    ] = '.'; //..........Appends ".1time" to file name.
		path_to_file[path_to_file_null_bookmark + 1] = '1';
		path_to_file[path_to_file_null_bookmark + 2] = 't';
		path_to_file[path_to_file_null_bookmark + 3] = 'i';
		path_to_file[path_to_file_null_bookmark + 4] = 'm';
		path_to_file[path_to_file_null_bookmark + 5] = 'e';
		out_stream.open(path_to_file);
		
		//Encrypts.
		char garbage_byte;
		int  garbage_byte_normal;
		int  one_thousand_counter = 0;
		int  one_million_counter  = 0;
		in_stream.get(garbage_byte);
		for(long long MB_done = 0; in_stream.eof() == false;)
		{	//..........Makes new actual key.
			if(one_thousand_counter == 0)
			{	
				
				
				
				
				//..........Fills int actual_key[1000] with new randomness (0 to 255.)
				//..........(Exactly the same for encrypt & decrypt.) Each kB of file consumes entire actual_key[].
				//..........The following block verbatim from rolling-code.cpp, except for absurd_protection_against_cryptanalysis & extracting rand.
				
				//..........Generator powerhouse.
				for(int b = 0; b < 100; b++)
				{	srand(actual_seeds[b]);
					for(int c = 0; c < 1000; c++) {randomness[c] += rand(); randomness[c] %= 256;} //..........Fills randomness[] (100x per 1,000-char code.)
					temp_overflow_for_randomness = (actual_seeds[99 - b] + rand()); //..........Modifies inverse actual_seeds[].
					actual_seeds[99 - b] = (temp_overflow_for_randomness % 4294967296);
					
					srand(actual_seeds[99 - b]);  //..........Now using that inverse seed.
					for(int c = 0; c < 1000; c++) //..........Swaps EACH & EVERY element in randomness[] with randomly chosen (100,000x per 1,000-char code.)
					{	int random_element = (rand() % 1000);
						for(; random_element == c;) {random_element = (rand() % 1000);}
						
						unsigned int temp_element = randomness[random_element];
						randomness[random_element] = randomness[c];
						randomness[c] = temp_element;
					}
					temp_overflow_for_randomness = (actual_seeds[b] + rand()); //..........Modifies current actual_seeds[].
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				//..........Makes 100 10-digit new actual seeds based on randomness[]. (!!! Adds to current actual_seeds. !!! The generated Code is NOT alone responsible for new actual_seeds. !!!) (Strings together 10 contiguous digits, 100 times.)
				int randomness_read_bookmark = 0;
				for(int b = 0; b < 100; b++)
				{	temp_overflow_for_randomness = 0;
					for(int c = 0; c < 10; c++)
					{	temp_overflow_for_randomness += (randomness[randomness_read_bookmark] % 10);
						if(c < 9) {temp_overflow_for_randomness *= 10;}
						randomness_read_bookmark++;
					}
					
					temp_overflow_for_randomness += actual_seeds[b];
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				for(int b = 0; b < 1000; b++) {actual_key[b] = randomness[b];}
				
				
				
				
				
			}
			
			//..........Makes byte normal.
			garbage_byte_normal = garbage_byte;
			if(garbage_byte_normal < 0) {garbage_byte_normal += 256;}
			
			//..........Encrypts byte.
			garbage_byte_normal += actual_key[one_thousand_counter];
			garbage_byte_normal %= 256;
			
			//..........Writes byte to file.
			if(garbage_byte_normal < 128) {out_stream.put(garbage_byte_normal      );}
			else                          {out_stream.put(garbage_byte_normal - 256);}
			
			file_size_for_overwriting++;
			one_thousand_counter++;
			one_million_counter++;
			if(one_thousand_counter ==    1000) {one_thousand_counter = 0                                                                ;}
			if(one_million_counter  == 1000000) {one_million_counter  = 0; MB_done++; cout << MB_done << " of " << total_MB << " MB...\n";}
			in_stream.get(garbage_byte);
		}
		in_stream.close();
		out_stream.close();
	}
	
	
	
	
	
	//______________________________________________________Decrypt___________________________________________________//
	if(user_option == 2)
	{	//Prepares two file streams.
		in_stream.open(path_to_file);
		path_to_file[path_to_file_null_bookmark     ] = ' '; //..........Appends " (decrypted)" to file name.
		path_to_file[path_to_file_null_bookmark +  1] = '(';
		path_to_file[path_to_file_null_bookmark +  2] = 'd';
		path_to_file[path_to_file_null_bookmark +  3] = 'e';
		path_to_file[path_to_file_null_bookmark +  4] = 'c';
		path_to_file[path_to_file_null_bookmark +  5] = 'r';
		path_to_file[path_to_file_null_bookmark +  6] = 'y';
		path_to_file[path_to_file_null_bookmark +  7] = 'p';
		path_to_file[path_to_file_null_bookmark +  8] = 't';
		path_to_file[path_to_file_null_bookmark +  9] = 'e';
		path_to_file[path_to_file_null_bookmark + 10] = 'd';
		path_to_file[path_to_file_null_bookmark + 11] = ')';
		out_stream.open(path_to_file);
		
		//Decrypts.
		char garbage_byte;
		int  garbage_byte_normal;
		int  one_thousand_counter = 0;
		int  one_million_counter  = 0;
		in_stream.get(garbage_byte);
		for(long long MB_done = 0; in_stream.eof() == false;)
		{	//..........Makes new actual key.
			if(one_thousand_counter == 0)
			{	
				
				
				
				
				//..........Fills int actual_key[1000] with new randomness (0 to 255.)
				//..........(Exactly the same for encrypt & decrypt.) Each kB of file consumes entire actual_key[].
				//..........The following block verbatim from rolling-code.cpp, except for absurd_protection_against_cryptanalysis & extracting rand.
				
				//..........Generator powerhouse.
				for(int b = 0; b < 100; b++)
				{	srand(actual_seeds[b]);
					for(int c = 0; c < 1000; c++) {randomness[c] += rand(); randomness[c] %= 256;} //..........Fills randomness[] (100x per 1,000-char code.)
					temp_overflow_for_randomness = (actual_seeds[99 - b] + rand()); //..........Modifies inverse actual_seeds[].
					actual_seeds[99 - b] = (temp_overflow_for_randomness % 4294967296);
					
					srand(actual_seeds[99 - b]);  //..........Now using that inverse seed.
					for(int c = 0; c < 1000; c++) //..........Swaps EACH & EVERY element in randomness[] with randomly chosen (100,000x per 1,000-char code.)
					{	int random_element = (rand() % 1000);
						for(; random_element == c;) {random_element = (rand() % 1000);}
						
						unsigned int temp_element = randomness[random_element];
						randomness[random_element] = randomness[c];
						randomness[c] = temp_element;
					}
					temp_overflow_for_randomness = (actual_seeds[b] + rand()); //..........Modifies current actual_seeds[].
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				//..........Makes 100 10-digit new actual seeds based on randomness[]. (!!! Adds to current actual_seeds. !!! The generated Code is NOT alone responsible for new actual_seeds. !!!) (Strings together 10 contiguous digits, 100 times.)
				int randomness_read_bookmark = 0;
				for(int b = 0; b < 100; b++)
				{	temp_overflow_for_randomness = 0;
					for(int c = 0; c < 10; c++)
					{	temp_overflow_for_randomness += (randomness[randomness_read_bookmark] % 10);
						if(c < 9) {temp_overflow_for_randomness *= 10;}
						randomness_read_bookmark++;
					}
					
					temp_overflow_for_randomness += actual_seeds[b];
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				for(int b = 0; b < 1000; b++) {actual_key[b] = randomness[b];}
				
				
				
				
				
			}
			
			//..........Makes byte normal.
			garbage_byte_normal = garbage_byte;
			if(garbage_byte_normal < 0) {garbage_byte_normal += 256;}
			
			//..........Decrypts byte.
			/*   ______________________________________________ ________________________________________________
			    |                                              |                                                |
			    |          if sub-key <= cipherfile            |                     else                       |
			    |   then plainfile = (cipherfile - sub-key)    |    plainfile = ((256 - sub-key) + cipherfile)  |
			    |______________________________________________|________________________________________________|
			*/
			if(actual_key[one_thousand_counter] <= garbage_byte_normal)
			{	garbage_byte_normal = (garbage_byte_normal - actual_key[one_thousand_counter]);
			}
			else
			{	garbage_byte_normal = ((256 - actual_key[one_thousand_counter]) + garbage_byte_normal);
			}
			
			//..........Writes byte to file.
			if(garbage_byte_normal < 128) {out_stream.put(garbage_byte_normal      );}
			else                            {out_stream.put(garbage_byte_normal - 256);}
			
			file_size_for_overwriting++;
			one_thousand_counter++;
			one_million_counter++;
			if(one_thousand_counter ==    1000) {one_thousand_counter = 0                                                                ;}
			if(one_million_counter  == 1000000) {one_million_counter  = 0; MB_done++; cout << MB_done << " of " << total_MB << " MB...\n";}
			in_stream.get(garbage_byte);
		}
		in_stream.close();
		out_stream.close();
	}
	
	
	
	
	
	//..........Overwrites seeds file with new seeds.
	if(user_option == 1)
	{	if(existence_of_file_2 == true) {out_stream.open("keys/seeds_for_incoming");}
		else                            {out_stream.open("keys/seeds_for_outgoing");}
	}
	else
	{	if(existence_of_file_2 == true) {out_stream.open("keys/seeds_for_outgoing");}
		else                            {out_stream.open("keys/seeds_for_incoming");}
	}
	
	for(int a = 0; a < 100; a++)
	{	unsigned int place = 10;
		for(int b = 0; b < 9; b++)
		{	if(actual_seeds[a] < place) {out_stream << "0";}
			place *= 10;
		}
		out_stream << actual_seeds[a];
	}
	out_stream << "\n\nSeeds are always rolling and in symmetry with the other party."
	           << "\nThe one with file \"2\" uses incoming for outgoing, and outgoing for incoming.\n";
	out_stream.close();
	
	//Overwrites things (some are partially effective--printed/copied text is cloned from RAM to RAM, flash storage cannot be easily overwritten.)
	//..........Overwrites RAM of variable char sniffed_one_file_character.
	sniffed_one_file_character = '\0'; sniffed_one_file_character = -1;   //Binary: 00000000, 111111111.
	
	//..........Overwrites RAM of variable int path_to_file_null_bookmark.
	path_to_file_null_bookmark = 0; path_to_file_null_bookmark = -2147483648; path_to_file_null_bookmark = 2147483647;
	
	//..........Overwrites RAM of array unsigned int seeds[1000].
	for(int a = 0; a < 1000; a++) {seeds[a] = 0; seeds[a] = 4294967295;}
	
	//..........Overwrites RAM of variable long long temp_overflow_for_randomness.
	temp_overflow_for_randomness = 0; temp_overflow_for_randomness = -9223372036854775807; temp_overflow_for_randomness = 9223372036854775807;
	
	//..........Overwrites RAM of array unsigned int actual_seeds[100].
	for(int a = 0; a < 100; a++) {actual_seeds[a] = 0; actual_seeds[a] = 4294967295;}
	
	//..........Overwrites RAM of array int actual_key[1000].
	for(int a = 0; a < 1000; a++) {actual_key[a] = 0; actual_key[a] = -2147483648; actual_key[a] = 2147483647;}
	
	//..........Overwrites RAM of array unsigned int randomness[1000].
	for(int a = 0; a < 1000; a++) {randomness[a] = 0; randomness[a] = 4294967295;}
	
	//..........Asks to overwrite the plainfile file after encryption.
	if(user_option == 1)
	{	cout << "Done! Overwrite plainfile? y/n: ";
		char wait; cin >> wait;
		
		if(wait == 'y')
		{	out_stream.open(path_to_file_copy_if_overwriting_raw);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream << '\0';} out_stream.close(); //Binary: 00000000.
			out_stream.open(path_to_file_copy_if_overwriting_raw);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream.put(-1);} out_stream.close(); //Binary: 11111111.
			
			cout << "Overwrite finished.\n";
		}
		else {cout << "File unharmed.\n";}
	}
	
	//..........Asks to overwrite the plainfile file after decryption, giving the user a moment to observe or transfer it first.
	if(user_option == 2)
	{	cout << "Done! Now is your chance to observe or copy the plainfile\n"
		     << "onto external devices so that it can be heavily\n"
		     << "overwritten here. Continue? y/n: ";
		char wait; cin >> wait;
		
		if(wait == 'y') //..........Note the difference here--original (non-copy) path_to_file[] that was appended to in decrypt.
		{	out_stream.open(path_to_file);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream << '\0';} out_stream.close(); //Binary: 00000000.
			out_stream.open(path_to_file);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream.put(-1);} out_stream.close(); //Binary: 11111111.
			
			cout << "Overwrite finished.\n";
		}
		else {cout << "File unharmed.\n";}
	}
	
	//..........Overwrites RAM of variable long long file_size_for_overwriting.
	file_size_for_overwriting = 0; file_size_for_overwriting = -9223372036854775807; file_size_for_overwriting = 9223372036854775807;
	
	//..........Overwrites RAM of array char path_to_file[10000]
	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0'; path_to_file[a] = -1;}           //Binary: 00000000, 111111111.
	
	//..........Overwrites RAM of array char path_to_file_copy_if_overwriting_raw[10000];    //Binary: 00000000, 111111111.
	for(int a = 0; a < 10000; a++) {path_to_file_copy_if_overwriting_raw[a] = '\0'; path_to_file_copy_if_overwriting_raw[a] = -1;}
}
