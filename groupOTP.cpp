// groupOTP - One-time pad for unlimited file encryption                        Run it: "apt install g++ geany". Open the .cpp in Geany. Hit F9 once. F5 to run.
//            between n people. No key is rejected upon
//            decryption--giving false plausible output.


/* Version 5.0.0   +   rolling-code 3.1.1
Send in the order encrypted, and decrypt in the order received! Although
there are no limits, you should get new keys occasionally. How a group works:
 ______________________________________________________________________________
/                                                                              \
|        user 1 makes keys:       1       2       3       4    ...n            |
|        user 2  gets copy:       1       2       3       4    ...n            |
|        user 3  gets copy:       1       2       3       4    ...n            |
|        user 4  gets copy:       1       2       3       4    ...n            |
|     ...user n  gets copy:       1       2       3       4    ...n            |
|                                                                              |
|                                 ^                               ^            |
|                            with which                      with which        |
|                         user 1 encrypts,                user n encrypts,     |
|                          others decrypt                  others decrypt      |
\_____________________________________________________________________________*/

#include <fstream>
#include <iostream>
using namespace std;
int main()
{	ifstream in_stream;
	ofstream out_stream;
	
	cout << "\n(k) Get keys"
	     << "\n(e) Encrypt outgoing file"
	     << "\n(d) Decrypt received file"
	
	     << "\n\nOption: ";
	
	char user_option; cin >> user_option;
	if((user_option != 'k') && (user_option != 'e') && (user_option != 'd')) {cout << "\nInvalid.\n"; return 0;}
	
	
	
	
	
	//______________________________________________________Get_keys__________________________________________________//
	if(user_option == 'k')
	{	//Checks if keys already exist.
		in_stream.open("keys");
		if(in_stream.fail() == false) {cout << "\nkeys folder already exists! \n\nRun a copy of groupOTP.cpp in a different folder.\n"; in_stream.close(); return 0;}
		in_stream.close();
		
		//Gets number of users.
		long long users = 0; cout << "Enter number of users (2 to 10^16): "; cin >> users;
		if((users < 2) || (users > 10000000000000000)) {cout << "Out of bounds.\n\n"; return 0;}
		if(users == 2) {cout << "\nYou are user 1. The other party must be user 2."                                                                   ;}
		else           {cout << "\nYou are user 1. The Other parties must be assigned \na unique user number between 2 and " << users << " incluseve.";}
		
		//Creates folder "keys".
		system("mkdir keys");
		
		//Prepares key file naming.
		char key_file_name[50]   = {"keys/00000000000000001"};
		long long renaming_arithmetic = 1000000000000000001  ;
		
		//The following is verbatim from rollingcode.cpp, except for code_length_in_thousands, write to keys, no cout.
		{	long long code_length_in_thousands = users;
			bool Unix_time_supplement = false;       //Set to true for codes of unique randomness, even with the same seeds file. DEFAULT = false.
			bool absurd_protection_against_cryptanalysis = false; //Slow, code_length_in_thousands becomes "actual code length."  DEFAULT = false.
			
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
					cout << "\nJust once, drop/enter any file"
					     << "\nof 1,000+ random first bytes:\n";
					
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
				
				//..........Supplements all actual_seeds[] with randomness based on Unix time.
				if(Unix_time_supplement == true)
				{	srand(time(0));
					for(int a = 0; a < 100; a++)
					{	temp_overflow_for_randomness = (rand() % 4294967296);
						temp_overflow_for_randomness += actual_seeds[a];
						actual_seeds[a] = (temp_overflow_for_randomness % 4294967296);
					}
				}
				
				
				
				
				
				//..........Generator house.
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
					
					if(absurd_protection_against_cryptanalysis == true) {out_stream << char((randomness[500] % 94) + 33);} //..........Writes only 1 code item per round.
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
						
						//..........Creates new name for a key file.
						long long temp_renaming_arithmetic = renaming_arithmetic;
						for(int b = 21; b > 4; b--)
						{	key_file_name[b] = (temp_renaming_arithmetic % 10);
							key_file_name[b] += 48;
							temp_renaming_arithmetic /= 10;
						}
						
						//..........Creates key file.
						out_stream.open(key_file_name);
						for(int b = 0; b < 1000; b++) {out_stream << char((randomness[b] % 10) + 48);}
						out_stream << "\n\nThese are 100 10-digit integer seeds strung together."
						           << "\nThey're always rolling, and in symmetry with other parties."
						           << "\nSome integers are smaller; prepended zeros make them \"10-digit.\""
						
						           << "\n\nRandomness sprouts from these seeds."
						           << "\nIf you are user " << (renaming_arithmetic % 1000000000000000000)
						           << "\nthen you're using that randomness to encrypt,"
						           << "\nwhile others are using it to decrypt."
						           
						           << "\n\nAll 100 seeds are used then rolled, per action."
						           << "\nIn fact, per 1,000 bytes of the input file, and more.\n";
						out_stream.close();
						
						//.........Updates key file name arithmetic for next name.
						renaming_arithmetic++;
					}
				}
				
				
				
				
				
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
		}
		
		//Overwrites & removes RC_seeds just in case. (Again, this is only effective on spinning-disk drives.)
		out_stream.open("RC_seeds"); for(int a = 0; a < 1000000; a++) {out_stream.put(  0);} out_stream.close();
		out_stream.open("RC_seeds"); for(int a = 0; a < 1000000; a++) {out_stream.put(255);} out_stream.close();
		remove("RC_seeds");
		
		cout << "\nDone! Share the keys folder securely in person."
		     << "\nOr they can get the same keys using the same"
		     << "\nfile of 1,000+ random first bytes.\n\n";
		
		return 0;
	}
	
	
	
	
	
	//Prepares for encrypt/decrypt....................
	//Exits if keys not present.
	in_stream.open("keys"); if(in_stream.fail() == true) {cout << "\nGet keys first.\n"; return 0;} in_stream.close();
	
	//Creates file "user" if not present.
	in_stream.open("user");
	if(in_stream.fail() == true)
	{	long long num = 0; cout << "Just once, enter your user number: "; cin >> num;
		out_stream.open("user"); out_stream << "user number    : " << num << "\n" << "files encrypted: 0\n"; out_stream.close();
	}
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
	
	//Checks if file exists.
	char file_byte;
	in_stream.open(path_to_file);
	if(in_stream.fail() == true) {in_stream.close(); cout << "\n\nNo such file or directory.\n";             return 0;}
	in_stream.get(file_byte);
	if(in_stream.eof() == true)  {in_stream.close(); cout << "\n\nNothing to process, the file is empty.\n"; return 0;}
	in_stream.close();
	
	//Gets file size in MB for displaying "x of y MB...".
	in_stream.open(path_to_file);
	long long total_bytes = 0;
	in_stream.get(file_byte);
	for(; in_stream.eof() == false;)
	{	in_stream.get(file_byte);
		total_bytes++;
	}
	in_stream.close();
	
	long long total_MB = (total_bytes - (total_bytes % 1000000));
	total_MB /= 1000000;
	cout << "\n";
	
	//Which key number.
	long long which_key = 0;
	if(user_option == 'e') //Key number based on file "user".
	{	in_stream.open("user");
		for(int a = 0; a < 18; a++) {in_stream.get(file_byte);}
		for(; file_byte != '\n';)
		{	which_key *= 10;
			which_key += (file_byte - 48);
			in_stream.get(file_byte);
		}
		in_stream.close();
	}
	else                   //Key number based on name of file to decrypt.
	{	int path_to_file_digit_one_bookmark;
		for(int a = 9999; path_to_file[a] != '/'; a--) {path_to_file_digit_one_bookmark = a;}
		path_to_file_digit_one_bookmark += 5;
		for(; path_to_file[path_to_file_digit_one_bookmark] != '_'; path_to_file_digit_one_bookmark++)
		{	which_key *= 10;
			which_key += (path_to_file[path_to_file_digit_one_bookmark] - 48);
		}
	}
	
	//Creates key file name.
	char key_file_name[50] =    {"keys/00000000000000000"};
	long long renaming_arithmetic = (1000000000000000000 + which_key);
	for(int a = 21; a > 4; a--)
	{	key_file_name[a] = (renaming_arithmetic % 10);
		key_file_name[a] += 48;
		renaming_arithmetic /= 10;
	}
	
	//Gets number of files encrypted.
	long long files_encrypted = 0;
	if(user_option == 'e')
	{	in_stream.open("user");
		in_stream.get(file_byte);
		for(; file_byte != '\n';) {in_stream.get(file_byte);}
		for(int a = 0; a < 18; a++) {in_stream.get(file_byte);}
		for(; file_byte != '\n';)
		{	files_encrypted *= 10;
			files_encrypted += (file_byte - 48);
			in_stream.get(file_byte);
		}
		in_stream.close();
	}
	
	//Loads seeds[] with key file. (From rolling-code.)
	in_stream.open(key_file_name);
	if(in_stream.fail() == true) {cout << "\n\nNo key file by the name " << key_file_name << "\n\n"; in_stream.close(); return 0;}
	unsigned int seeds[1000];
	for(int a = 0; a < 1000; a++)
	{	in_stream.get(file_byte);
		int normal_byte = file_byte;
		if(normal_byte < 0) {normal_byte += 256;}
		seeds[a] = normal_byte;
		
		seeds[a] -= 48;
		if((in_stream.eof() == true) || (seeds[a] > 9))
		{	for(int b = 0; b < 1000; b++) {seeds[b] = 0; seeds[b] = 4294967295;} //..........Overwrites RAM of array unsigned int seeds[1000].
			cout << "\n\nBad seeds file.\n\n"; in_stream.close(); return 0;
		}
	}
	in_stream.close();
	
	//Makes 100 10-digit actual seeds based on seeds[]. (Strings together 10 contiguous digits, 100 times.) (From rolling-code.)
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
	
	
	
	
	
	//______________________________________________________Encrypt___________________________________________________//
	if(user_option == 'e')
	{	//Updates file "user".
		out_stream.open("user");
		out_stream << "user number    : " << which_key << "\nfiles encrypted: " << (files_encrypted + 1) << "\n";
		out_stream.close();
		
		//Creates out file name.
		in_stream.open("user");
		char path_to_out_file[50] = {"user_"};
		int path_to_out_file_bookmark = 5;
		for(int a = 0; a < 18; a++) {in_stream.get(file_byte);}
		for(; file_byte != '\n'; path_to_out_file_bookmark++) {path_to_out_file[path_to_out_file_bookmark] = file_byte; in_stream.get(file_byte);}
		path_to_out_file[path_to_out_file_bookmark    ] = '_';
		path_to_out_file[path_to_out_file_bookmark + 1] = 'm';
		path_to_out_file[path_to_out_file_bookmark + 2] = 's';
		path_to_out_file[path_to_out_file_bookmark + 3] = 'g';
		path_to_out_file[path_to_out_file_bookmark + 4] = '_';
		path_to_out_file_bookmark += 5;
		
		for(int a = 0; a < 18; a++) {in_stream.get(file_byte);}
		for(; file_byte != '\n'; path_to_out_file_bookmark++) {path_to_out_file[path_to_out_file_bookmark] = file_byte; in_stream.get(file_byte);}
		path_to_out_file[path_to_out_file_bookmark     ] = '_';
		path_to_out_file[path_to_out_file_bookmark +  1] = '(';
		path_to_out_file[path_to_out_file_bookmark +  2] = 'e';
		path_to_out_file[path_to_out_file_bookmark +  3] = 'n';
		path_to_out_file[path_to_out_file_bookmark +  4] = 'c';
		path_to_out_file[path_to_out_file_bookmark +  5] = 'r';
		path_to_out_file[path_to_out_file_bookmark +  6] = 'y';
		path_to_out_file[path_to_out_file_bookmark +  7] = 'p';
		path_to_out_file[path_to_out_file_bookmark +  8] = 't';
		path_to_out_file[path_to_out_file_bookmark +  9] = 'e';
		path_to_out_file[path_to_out_file_bookmark + 10] = 'd';
		path_to_out_file[path_to_out_file_bookmark + 11] = ')';
		in_stream.close();
		
		//Prepares 2 file streams.
		in_stream.open (path_to_file    );
		out_stream.open(path_to_out_file);
		
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
			
			one_thousand_counter++;
			one_million_counter++;
			if(one_thousand_counter ==    1000) {one_thousand_counter = 0                                                                ;}
			if(one_million_counter  == 1000000) {one_million_counter  = 0; MB_done++; cout << MB_done << " of " << total_MB << " MB...\n";}
			in_stream.get(garbage_byte);
		}
		in_stream.close();
		out_stream.close();
		
		cout << "\nCreated file: " << path_to_out_file << "\nSend it to ALL other parties.\n\n";
	}
	
	
	
	
	
	//______________________________________________________Decrypt___________________________________________________//
	if(user_option == 'd')
	{	//Creates out file name.
		char path_to_out_file[50] = {"user_"};
		int path_to_out_file_bookmark = 5;
		int path_to_file_digit_one_bookmark;
		for(int a = 9999; path_to_file[a] != '/'; a--) {path_to_file_digit_one_bookmark = a;}
		path_to_file_digit_one_bookmark += 5;
		for(; path_to_file[path_to_file_digit_one_bookmark] != '(';)
		{	path_to_out_file[path_to_out_file_bookmark] = path_to_file[path_to_file_digit_one_bookmark];
			path_to_out_file_bookmark++;
			path_to_file_digit_one_bookmark++;
		}
		path_to_out_file[path_to_out_file_bookmark     ] = '(';
		path_to_out_file[path_to_out_file_bookmark +  1] = 'd';
		path_to_out_file[path_to_out_file_bookmark +  2] = 'e';
		path_to_out_file[path_to_out_file_bookmark +  3] = 'c';
		path_to_out_file[path_to_out_file_bookmark +  4] = 'r';
		path_to_out_file[path_to_out_file_bookmark +  5] = 'y';
		path_to_out_file[path_to_out_file_bookmark +  6] = 'p';
		path_to_out_file[path_to_out_file_bookmark +  7] = 't';
		path_to_out_file[path_to_out_file_bookmark +  8] = 'e';
		path_to_out_file[path_to_out_file_bookmark +  9] = 'd';
		path_to_out_file[path_to_out_file_bookmark + 10] = ')';
		
		//Prepares 2 file streams.
		in_stream.open (path_to_file    );
		out_stream.open(path_to_out_file);
		
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
			else                          {out_stream.put(garbage_byte_normal - 256);}
			
			one_thousand_counter++;
			one_million_counter++;
			if(one_thousand_counter ==    1000) {one_thousand_counter = 0                                                                ;}
			if(one_million_counter  == 1000000) {one_million_counter  = 0; MB_done++; cout << MB_done << " of " << total_MB << " MB...\n";}
			in_stream.get(garbage_byte);
		}
		in_stream.close();
		out_stream.close();
		
		cout << "\nCreated file: " << path_to_out_file << "\n\n";
	}
	
	
	
	
	
	//Updates key file with new seeds.
	out_stream.open(key_file_name);
	for(int a = 0; a < 100; a++)
	{	unsigned int place = 10;
		for(int b = 0; b < 9; b++)
		{	if(actual_seeds[a] < place) {out_stream << "0";}
			place *= 10;
		}
		out_stream << actual_seeds[a];
	}
	out_stream.close();
}
