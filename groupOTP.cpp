/// groupOTP - fast & plentiful One-time pad for groups of any size.
/// Nikolay Valentinovich Repnitskiy - License: WTFPLv2+ (wtfpl.net)


/* Version 2.1, plausibility ratio = 1:95, plausible messages per ciphertext =
95^79. This is communication with ciphertext of unlimited plausible deniability.
Any key tried is not rejected and successfully decrypts any ciphertext into some
random or plausible message. Get 20,000 or 1,600,000 outgoing messages per user.
Generate and share multiple folders to have spare keys.  Keys are truncated upon
use & written back, after overwriting their locations. Plaintext is 79 char max.
80th char holds message length. Reserved randomness within key files is appended
to all plaintext then both that randomness & plaintext are encrypted, along with
message length information--preventing key fragment exposure and size-analytics.
All ciphertext is 80 char. User IDs are appended to automate decryption. You get
n-channel symmetry. All users may encrypt in simultaneity without interference.
 ______________________________________________________________________________
/                                                                              \
|    * you generate keys:         1  2  3  4  5  6  7  8  9  10  11 ... n      |
|                                                                              |
|     user 2 gets copies:         1  2  3  4  5  6  7  8  9  10  11 ... n      |
|          .                      .  .  .  .  .  .  .  .  .   .   .     .      |
|          .                      .  .  .  .  .  .  .  .  .   .   .     .      |
|          .                      .  .  .  .  .  .  .  .  .   .   .     .      |
|     user n gets copies:         1  2  3  4  5  6  7  8  9  10  11 ... n      |
|                                                                              |
|                                                                              |
|                                 ^                                            |
|                                 you encrypt, others decrypt...               |
|                                                                       ^      |
|                                ...user n encrypts, you & others decrypt      |
\______________________________________________________________________________/

If you happen to encrypt n messages in a row and send them disordered or did not
send them at all, send the other parties a new message  where they decrypt until
the output makes sense. If you send encrypted messages with seeds for generating
new symmetric keys, perfect secrecy is then reduced to decryption difficulty and
plausible messages per cipher narrows. Proof:  github.com/compromise-evident/OTP
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
If there's a hitch, remove files remaining.encrypt.txt and number.assignment.txt
and option 1 will ask for your number assignment again. Enter only the ID that's
not assigned, then send an encrypted message to announce your reassignment. This
is only possible if you had generated more keys than there are OTP participants.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Invitation after key usage  -  Currently, this feature is not automated, but you
can invite anyone by sharing current versions of your keys, and assigning unused
IDs to the new users.  They must pick up decryption where you left off--as would
be indicated by the timestamp of the last unread message within the history-chat
room automated by a different services where this program sits compute-isolated!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
How to run the program  -  Software package repositories for GNU+Linux operating
systems have all the tools you can imagine. Open a terminal and use this command
as root to install Geany and g++ on your computer: apt install geany g++   Geany
is a fast & lightweight text editor and Integrated Development Environment where
you can write and run code. g++ is the GNU compiler for C++ which allows written
code to run. The compiler operates in the background and displays errors in your
code as you will see in the lower Geany box. Make a new folder somewhere on your
machine. Paste this code into Geany. For clarity in auditing, enable indentation
guides: go to View >> Show Indentation Guides. Save the document as anything.cpp
within the newly-created folder. Use these shortcuts to run the program: F9, F5.
You may paste over this code with other  .cpp files, or open a new tab & repeat.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
How to make an executable with g++  -  Save this program as anything.cpp, open a
terminal, and type g++ then space. Drag & drop this saved file into the terminal
and its directory will be  appended to your pending entry. Click on the terminal
and press enter.   a.out now resides in the user directory, you may rename it to
anything.  To run that executable, simply drag and drop it into a terminal, then
click on the terminal and press enter.  Reminder:  executable's effect-directory
is the user directory on your machine, for example:  /home/nikolay    Enjoy. */

#include <fstream>
#include <iostream>
#include <sys/stat.h> //For mkdir() (creating folders.)
using namespace std;

int main()
{	cout << "\n(group One-time pad)\n\n"
	
	     << "(1) Encrypt\n"
	     << "(2) Decrypt\n"
	     << "(3) Get keys with 20,000 msg/user.      (Generates 3.2MB in 8 seconds/user.)\n"
	     << "(4) Get keys with 1.6 million msg/user. (Generates 256MB in 8 minutes/user.)\n\n"
	
	     << "Enter option: ";
	
	int user_option;
	cin >> user_option;
	if((user_option != 1) && (user_option != 2) && (user_option != 3) && (user_option != 4)) {cout << "\nInvalid, program ended.\n"; return 0;}
	//(You can run each of the four ifs holding options 1, 2, and 3, and 4 in isolation--they are self-sustained.)
	
	
	
	
	
	//______________________________________________________Encrypt___________________________________________________//
	if(user_option == 1)
	{	ifstream in_stream;
		ofstream out_stream;
		
		//Checks if keys are present.
		bool existence_of_file_option_3 = false;
		bool existence_of_file_option_4 = false;
		in_stream.open("./keys/option_3"); if(in_stream.fail() == false) {existence_of_file_option_3 = true;} in_stream.close();
		in_stream.open("./keys/option_4"); if(in_stream.fail() == false) {existence_of_file_option_4 = true;} in_stream.close();
		if((existence_of_file_option_3 == false)
		&& (existence_of_file_option_4 == false))
		{cout << "\nCan't encrypt without keys.\n"; return 0;}
		
		//Initialize user ID if fresh start.
		in_stream.open("remaining.encrypt.txt");
		if(in_stream.fail() == true)
		{	//Gets user ID.
			in_stream.close();
			cout << "\nEnter your assigned number: ";
			long long user_ID;
			cin >> user_ID;
			
			//Applies user ID to batch numbering.
			char file_name_key_outgoing[30] = "./keys/00000000000000000/0000";
			long long temp_file_name_key_batch_numbering = user_ID;
			for(int a = 23; a > 6; a--)
			{	file_name_key_outgoing[a] = ((temp_file_name_key_batch_numbering % 10) + 48);
				temp_file_name_key_batch_numbering /= 10;
			}
			
			//Applies first available key file name. Quits if no key file in batch folder.
			int file_numbering = 1000000; //Only the last 4 digits are used, size makes mod operations easier.
			for(int a = 0; a < 10000; a++)
			{	int temp_file_numbering = file_numbering;//                                       (Batch #)      (File name)
				file_name_key_outgoing[28] = (temp_file_numbering % 10) + 48; //        ./keys/00000000000000000/0  0  0  0
				temp_file_numbering /= 10; //                                                                    ^  ^  ^  ^
				file_name_key_outgoing[27] = (temp_file_numbering % 10) + 48; //                                25 26 27 28    (element layout)
				temp_file_numbering /= 10;
				file_name_key_outgoing[26] = (temp_file_numbering % 10) + 48;
				temp_file_numbering /= 10;
				file_name_key_outgoing[25] = (temp_file_numbering % 10) + 48;
				
				in_stream.open(file_name_key_outgoing);
				if(in_stream.fail() == false) {in_stream.close(); break;}
				in_stream.close();
				
				file_numbering++;
				
				if(a == 9999) {cout << "\nNo key with that ID. (Your number assignment is your encryption key batch.)\n"; return 0;}
			}
			
			//Creates user ID file.
			out_stream.open("number.assignment.txt");
			long long truncator = 10000000000000000;
			for(int a = 0; a < 16; a++)
			{	if(user_ID < truncator) {out_stream << 0;}
				truncator /= 10;
			}
			out_stream << user_ID;
			out_stream.close();
			
			//Creates file remaining.encrypt.txt based on key type.
			if(existence_of_file_option_3 == true)
			{	out_stream.open("remaining.encrypt.txt");
				out_stream << "0020000 messages left to encrypt.\n\n"
						   << "Do not modify this file.\n"
						   << "This number must consist\n"
						   << "of digits 0000000 - 1600000.";
				out_stream.close();
			}
			else
			{	out_stream.open("remaining.encrypt.txt");
				out_stream << "1600000 messages left to encrypt.\n\n"
						   << "Do not modify this file.\n"
						   << "This number must consist\n"
						   << "of digits 0000000 - 1600000.";
				out_stream.close();
			}
		}
		in_stream.close();
		
		//Checks that messages left to encrypt is not zero.
		char remaining_encrypt[7];
		in_stream.open("remaining.encrypt.txt");
		if(in_stream.fail() == true) {in_stream.close(); cout << "\nremaining.encrypt.txt is missing.\n"; return 0;}
		for(int a = 0; a < 7; a++) {in_stream >> remaining_encrypt[a];}
		in_stream.close();
		
		for(int a = 0; a < 7; a++) {remaining_encrypt[a] -= 48;} //Fixes input.
		int remaining_encrypt_decimal = 0;
		if(remaining_encrypt[0] > 0) {remaining_encrypt_decimal += (remaining_encrypt[0] * 1000000);}
		if(remaining_encrypt[1] > 0) {remaining_encrypt_decimal += (remaining_encrypt[1] *  100000);}
		if(remaining_encrypt[2] > 0) {remaining_encrypt_decimal += (remaining_encrypt[2] *   10000);}
		if(remaining_encrypt[3] > 0) {remaining_encrypt_decimal += (remaining_encrypt[3] *    1000);}
		if(remaining_encrypt[4] > 0) {remaining_encrypt_decimal += (remaining_encrypt[4] *     100);}
		if(remaining_encrypt[5] > 0) {remaining_encrypt_decimal += (remaining_encrypt[5] *      10);}
		if(remaining_encrypt[6] > 0) {remaining_encrypt_decimal +=  remaining_encrypt[6]           ;}
		if(remaining_encrypt_decimal == 0)     {cout << "\nEncryption keys depleted.\n"; return 0;}
		
		cout << "\n79 characters max (width of terminal -1.) Enter message:\n\n";
		
		char nothing_to_see_here[1]; //This is here to catch the cached new line.
		cin.getline(nothing_to_see_here, 1);
		
		char plaintext[80] = {'\0'}; //(Init fills remaining with null.)
		cin.getline(plaintext, 80);
		plaintext[79] = '\0'; //Message is cut down to 79 characters.
		
		if(plaintext[0] == '\0') {cout << "No input given.\n"; return 0;}
		
		int ciphertext[80];
		for(int a = 0; a < 80; a++) //Prepares initial ciphertext[]. Pressing arrow/other keys WILL TRIGGER THE FOLLOWING ELSE!
		{	     if(plaintext[a] == ' ') {ciphertext[a] =  0;}	else if(plaintext[a] == 'P') {ciphertext[a] = 48;}
			else if(plaintext[a] == '!') {ciphertext[a] =  1;}	else if(plaintext[a] == 'Q') {ciphertext[a] = 49;}
			else if(plaintext[a] == '"') {ciphertext[a] =  2;}	else if(plaintext[a] == 'R') {ciphertext[a] = 50;}
			else if(plaintext[a] == '#') {ciphertext[a] =  3;}	else if(plaintext[a] == 'S') {ciphertext[a] = 51;}
			else if(plaintext[a] == '$') {ciphertext[a] =  4;}	else if(plaintext[a] == 'T') {ciphertext[a] = 52;}
			else if(plaintext[a] == '%') {ciphertext[a] =  5;}	else if(plaintext[a] == 'U') {ciphertext[a] = 53;}
			else if(plaintext[a] == '&') {ciphertext[a] =  6;}	else if(plaintext[a] == 'V') {ciphertext[a] = 54;}
			else if(plaintext[a] =='\'') {ciphertext[a] =  7;}	else if(plaintext[a] == 'W') {ciphertext[a] = 55;}
			else if(plaintext[a] == '(') {ciphertext[a] =  8;}	else if(plaintext[a] == 'X') {ciphertext[a] = 56;}
			else if(plaintext[a] == ')') {ciphertext[a] =  9;}	else if(plaintext[a] == 'Y') {ciphertext[a] = 57;}
			else if(plaintext[a] == '*') {ciphertext[a] = 10;}	else if(plaintext[a] == 'Z') {ciphertext[a] = 58;}
			else if(plaintext[a] == '+') {ciphertext[a] = 11;}	else if(plaintext[a] == '[') {ciphertext[a] = 59;}
			else if(plaintext[a] == ',') {ciphertext[a] = 12;}	else if(plaintext[a] =='\\') {ciphertext[a] = 60;}
			else if(plaintext[a] == '-') {ciphertext[a] = 13;}	else if(plaintext[a] == ']') {ciphertext[a] = 61;}
			else if(plaintext[a] == '.') {ciphertext[a] = 14;}	else if(plaintext[a] == '^') {ciphertext[a] = 62;}
			else if(plaintext[a] == '/') {ciphertext[a] = 15;}	else if(plaintext[a] == '_') {ciphertext[a] = 63;}
			else if(plaintext[a] == '0') {ciphertext[a] = 16;}	else if(plaintext[a] == '`') {ciphertext[a] = 64;}
			else if(plaintext[a] == '1') {ciphertext[a] = 17;}	else if(plaintext[a] == 'a') {ciphertext[a] = 65;}
			else if(plaintext[a] == '2') {ciphertext[a] = 18;}	else if(plaintext[a] == 'b') {ciphertext[a] = 66;}
			else if(plaintext[a] == '3') {ciphertext[a] = 19;}	else if(plaintext[a] == 'c') {ciphertext[a] = 67;}
			else if(plaintext[a] == '4') {ciphertext[a] = 20;}	else if(plaintext[a] == 'd') {ciphertext[a] = 68;}
			else if(plaintext[a] == '5') {ciphertext[a] = 21;}	else if(plaintext[a] == 'e') {ciphertext[a] = 69;}
			else if(plaintext[a] == '6') {ciphertext[a] = 22;}	else if(plaintext[a] == 'f') {ciphertext[a] = 70;}
			else if(plaintext[a] == '7') {ciphertext[a] = 23;}	else if(plaintext[a] == 'g') {ciphertext[a] = 71;}
			else if(plaintext[a] == '8') {ciphertext[a] = 24;}	else if(plaintext[a] == 'h') {ciphertext[a] = 72;}
			else if(plaintext[a] == '9') {ciphertext[a] = 25;}	else if(plaintext[a] == 'i') {ciphertext[a] = 73;}
			else if(plaintext[a] == ':') {ciphertext[a] = 25;}	else if(plaintext[a] == 'j') {ciphertext[a] = 74;}
			else if(plaintext[a] == ';') {ciphertext[a] = 27;}	else if(plaintext[a] == 'k') {ciphertext[a] = 75;}
			else if(plaintext[a] == '<') {ciphertext[a] = 28;}	else if(plaintext[a] == 'l') {ciphertext[a] = 76;}
			else if(plaintext[a] == '=') {ciphertext[a] = 29;}	else if(plaintext[a] == 'm') {ciphertext[a] = 77;}
			else if(plaintext[a] == '>') {ciphertext[a] = 30;}	else if(plaintext[a] == 'n') {ciphertext[a] = 78;}
			else if(plaintext[a] == '?') {ciphertext[a] = 31;}	else if(plaintext[a] == 'o') {ciphertext[a] = 79;}
			else if(plaintext[a] == '@') {ciphertext[a] = 32;}	else if(plaintext[a] == 'p') {ciphertext[a] = 80;}
			else if(plaintext[a] == 'A') {ciphertext[a] = 33;}	else if(plaintext[a] == 'q') {ciphertext[a] = 81;}
			else if(plaintext[a] == 'B') {ciphertext[a] = 34;}	else if(plaintext[a] == 'r') {ciphertext[a] = 82;}
			else if(plaintext[a] == 'C') {ciphertext[a] = 35;}	else if(plaintext[a] == 's') {ciphertext[a] = 83;}
			else if(plaintext[a] == 'D') {ciphertext[a] = 36;}	else if(plaintext[a] == 't') {ciphertext[a] = 84;}
			else if(plaintext[a] == 'E') {ciphertext[a] = 37;}	else if(plaintext[a] == 'u') {ciphertext[a] = 85;}
			else if(plaintext[a] == 'F') {ciphertext[a] = 38;}	else if(plaintext[a] == 'v') {ciphertext[a] = 86;}
			else if(plaintext[a] == 'G') {ciphertext[a] = 39;}	else if(plaintext[a] == 'w') {ciphertext[a] = 87;}
			else if(plaintext[a] == 'H') {ciphertext[a] = 40;}	else if(plaintext[a] == 'x') {ciphertext[a] = 88;}
			else if(plaintext[a] == 'I') {ciphertext[a] = 41;}	else if(plaintext[a] == 'y') {ciphertext[a] = 89;}
			else if(plaintext[a] == 'J') {ciphertext[a] = 42;}	else if(plaintext[a] == 'z') {ciphertext[a] = 90;}
			else if(plaintext[a] == 'K') {ciphertext[a] = 43;}	else if(plaintext[a] == '{') {ciphertext[a] = 91;}
			else if(plaintext[a] == 'L') {ciphertext[a] = 44;}	else if(plaintext[a] == '|') {ciphertext[a] = 92;}
			else if(plaintext[a] == 'M') {ciphertext[a] = 45;}	else if(plaintext[a] == '}') {ciphertext[a] = 93;}
			else if(plaintext[a] == 'N') {ciphertext[a] = 46;}	else if(plaintext[a] == '~') {ciphertext[a] = 94;}
			else if(plaintext[a] == 'O') {ciphertext[a] = 47;}	else if(plaintext[a] =='\0') {ciphertext[a] = -1;} //If message < 79 char, this marks all ending unused spaces to which randomness will be applied then encrypted.
			
			else
			{	cout << "\nDefective input, try again.\n";
				
				//Overwrites RAM of arrays plaintext[] and ciphertext[].
				for(int a = 0; a < 80; a++)
				{	for(int b = 0; b < 10; b++)
					{	plaintext[a]  = b;
						ciphertext[a] = b;
					}
				} //Only partially effective, printed/copied text is cloned from RAM to RAM.
				
				return 0;
			}
		}
		
		//Generates key file NAME based on depletion...
		in_stream.open("number.assignment.txt");
		if(in_stream.fail() == true) {in_stream.close(); cout << "\nfile number.assignment.txt is missing.\n"; return 0;}
		char number_assignment_txt[17] = {'\0'};
		in_stream.get(number_assignment_txt[0]);
		if(in_stream.eof() == true) {in_stream.close();  cout << "\nFile number.assignment.txt is empty.\n";   return 0;}
		for(int a = 1; in_stream.eof() == false; a++) {in_stream.get(number_assignment_txt[a]);}
		in_stream.close();
		
		//Applies user ID to batch numbering.
		char file_name_key_outgoing[30] = "./keys/00000000000000000/0000";
		for(int a = 7; a < 24; a++) {file_name_key_outgoing[a] = number_assignment_txt[a - 7];}
		
		//Applies first available key file name. Quits if no key file in batch folder.
		int file_numbering = 1000000; //Only the last 4 digits are used, size makes mod operations easier.
		for(int a = 0; a < 10000; a++)
		{	int temp_file_numbering = file_numbering;//                                       (Batch #)      (File name)
			file_name_key_outgoing[28] = (temp_file_numbering % 10) + 48; //        ./keys/00000000000000000/0  0  0  0
			temp_file_numbering /= 10; //                                                                    ^  ^  ^  ^
			file_name_key_outgoing[27] = (temp_file_numbering % 10) + 48; //                                25 26 27 28    (element layout)
			temp_file_numbering /= 10;
			file_name_key_outgoing[26] = (temp_file_numbering % 10) + 48;
			temp_file_numbering /= 10;
			file_name_key_outgoing[25] = (temp_file_numbering % 10) + 48;
			
			in_stream.open(file_name_key_outgoing);
			if(in_stream.fail() == false) {in_stream.close(); break;}
			in_stream.close();
			
			file_numbering++;
			
			if(a == 9999) {cout << "\nNo keys?\n"; return 0;}
		}
		
		//Gets a copy of the key file. (Only the first 160 characters!)
		char key_outgoing_content[160];
		in_stream.open(file_name_key_outgoing);
		for(int a = 0; a < 160; a++) {in_stream.get(key_outgoing_content[a]);}
		in_stream.close();
		for(int a = 0; a < 160; a++) {key_outgoing_content[a] -= 32;} //Fixes key contents.
		
		//Gets the message length information.
		int message_length = 0;
		for(int a = 0; a < 79; a++)
		{	if(ciphertext[a] != -1) {message_length++;}
		}
		
		//Appends randomness if message is less than 79 characters.
		for(int a = 0; a < 79; a++)
		{	if(ciphertext[a] == -1) {ciphertext[a] = key_outgoing_content[a];}
		}
		
		//Appends message length information (overwrites 80th element.)
		ciphertext[79] = (message_length);
		
		///Encrypts message.
		for(int a = 0; a < 80; a++) {ciphertext[a] += key_outgoing_content[a + 80]; ciphertext[a] %= 95;}
		
		//Prints ciphertext.
		for(int a = 0; a < 40; a++) {cout << "\n";}
		cout << "\nSelect all 97 ciphertext characters including first spaces, right-click & copy:\n\n";
		for(int a = 0; a < 80; a++)
		{	cout << char(ciphertext[a] + 32);
		}
		
		//Appends the user ID.
		for(int a = 7; a < 24; a++) {cout << file_name_key_outgoing[a];}
		
		//Overwriting RAM of plaintext[].
		for(int a = 0; a < 80; a++)
		{	for(int b = 0; b < 10; b++)
			{	plaintext[a] = b;
			}
		} //Only partially effective, printed/copied text is cloned from RAM to RAM.
		
		//Overwriting RAM of key_outgoing_content[].
		for(int a = 0; a < 160; a++)
		{	for(int b = 0; b < 10; b++)
			{	key_outgoing_content[a] = b;
			}
		}
		
		//Copies entire outgoing key into RAM before overwriting it.
		char truncating_table_private[100001] = {'\0'};
		in_stream.open(file_name_key_outgoing);
		in_stream.get(truncating_table_private[0]);
		for(int a = 1; in_stream.eof() == false; a++) {in_stream.get(truncating_table_private[a]);}
		in_stream.close();
		
		//Overwrites the outgoing key file twice with 100,000 characters each round.
		out_stream.open(file_name_key_outgoing); for(int a = 0; a < 100000; a++) {out_stream << '\0';} out_stream.close(); //Binary: 00000000
		out_stream.open(file_name_key_outgoing); for(int a = 0; a < 100000; a++) {out_stream.put(-1);} out_stream.close(); //Binary: 11111111
		
		//Removing or truncating the outgoing key file by the first 160 characters.
		if(truncating_table_private[160] == '\0') {remove(file_name_key_outgoing);}
		else
		{	out_stream.open(file_name_key_outgoing);
			for(int a = 160; truncating_table_private[a] != '\0'; a++) {out_stream.put(truncating_table_private[a]);}
			out_stream.close();
		}
		
		//Overwriting RAM of truncating_table_private[];
		for(int a = 0; a < 100001; a++)
		{	for(int b = 0; b < 10; b++) {truncating_table_private[a] = b;}
		}
		
		//Adjusting file remaining.encrypt.txt.
		remaining_encrypt_decimal--;
		out_stream.open("remaining.encrypt.txt");
		if(remaining_encrypt_decimal < 1000000) {out_stream << "0";}
		if(remaining_encrypt_decimal <  100000) {out_stream << "0";}
		if(remaining_encrypt_decimal <   10000) {out_stream << "0";}
		if(remaining_encrypt_decimal <    1000) {out_stream << "0";}
		if(remaining_encrypt_decimal <     100) {out_stream << "0";}
		if(remaining_encrypt_decimal <      10) {out_stream << "0";}
		out_stream << remaining_encrypt_decimal;
		out_stream << " messages left to encrypt.\n\n"
		
		           << "Do not modify this file.\n"
		           << "This number must consist\n"
		           << "of digits 0000000 - 1600000.";
		out_stream.close();
		
		//Displays # of messages left to generate.
		if     (remaining_encrypt_decimal == 0) {cout << "\n\nEncryption keys depleted.\n"        ;}
		else if(remaining_encrypt_decimal == 1) {cout << "\n\nYou may encrypt one more message.\n";}
		else   {cout << "\n\nYou may encrypt " << remaining_encrypt_decimal << " more messages.\n";}
	}
	
	
	
	
	
	//______________________________________________________Decrypt___________________________________________________//
	if(user_option == 2)
	{	ifstream in_stream;
		ofstream out_stream;
		
		//Checks if keys are present.
		bool existence_of_file_option_3 = false;
		bool existence_of_file_option_4 = false;
		in_stream.open("./keys/option_3"); if(in_stream.fail() == false) {existence_of_file_option_3 = true;} in_stream.close();
		in_stream.open("./keys/option_4"); if(in_stream.fail() == false) {existence_of_file_option_4 = true;} in_stream.close();
		if((existence_of_file_option_3 == false)
		&& (existence_of_file_option_4 == false))
		{cout << "\nCan't decrypt without keys.\n"; return 0;}
		
		cout << "Decrypt in the order received.\n"
		     << "Right-click and paste the ciphertext of 97 characters:\n\n";
		
		char nothing_to_see_here[1]; //This is here to catch the cached new line.
		cin.getline(nothing_to_see_here, 1);
		
		char ciphertext[98] = {'\0'}; //(Init fills remaining with null.)
		cin.getline(ciphertext, 98);
		
		if(ciphertext[0] == '\0') {cout << "No input given.\n"; return 0;}
		for(int a = 0; a < 97; a++) {ciphertext[a] -= 32;} //Fixes ciphertext.
		ciphertext[97] = '\0';
		
		//Generates key file NAME based on depletion...
		char file_name_key_incoming[30] = "./keys/00000000000000000/0000";
		int ciphertext_read_bookmark = 80;
		
		//Applies user ID to batch numbering.
		for(int a = 7; a < 24; a++)
		{	file_name_key_incoming[a] = ciphertext[ciphertext_read_bookmark] + 32;
			ciphertext_read_bookmark++;
		}
		
		//Applies first available key file name. Quits if no key file in batch folder.
		int file_numbering = 1000000; //Only the last 4 digits are used, size makes mod operations easier.
		for(int a = 0; a < 10000; a++)
		{	int temp_file_numbering = file_numbering;//                                       (Batch #)      (File name)
			file_name_key_incoming[28] = (temp_file_numbering % 10) + 48; //        ./keys/00000000000000000/0  0  0  0
			temp_file_numbering /= 10; //                                                                    ^  ^  ^  ^
			file_name_key_incoming[27] = (temp_file_numbering % 10) + 48; //                                25 26 27 28    (element layout)
			temp_file_numbering /= 10;
			file_name_key_incoming[26] = (temp_file_numbering % 10) + 48;
			temp_file_numbering /= 10;
			file_name_key_incoming[25] = (temp_file_numbering % 10) + 48;
			
			in_stream.open(file_name_key_incoming);
			if(in_stream.fail() == false) {in_stream.close(); break;}
			in_stream.close();
			
			file_numbering++;
			
			if(a == 9999) {cout << "\nNo keys?\n"; return 0;}
		}
		
		//Gets a copy of the key file. (Only the first 160 characters!)
		char key_incoming_content[160];
		in_stream.open(file_name_key_incoming);
		for(int a = 0; a < 160; a++) {in_stream.get(key_incoming_content[a]);}
		in_stream.close();
		for(int a = 80; a < 160; a++) {key_incoming_content[a] -= 32;} //Fixes key content portion used for decryption.
		
		/// Decrypts message. The following formula helps extract plaintext quickly.
		//      ______________________________________________ ________________________________________________
		//     |                                              |                                                |
		//     |          if sub-key <= ciphertext            |                     else                       |
		//     |   then plaintext = (ciphertext - sub-key)    |    plaintext = ((95 - sub-key) + ciphertext)   |
		//     |______________________________________________|________________________________________________|
		//
		int plaintext[80];
		for(int a = 0; a < 80; a++)
		{	if(key_incoming_content[a + 80] <= ciphertext[a])
			{	plaintext[a] = (ciphertext[a] - key_incoming_content[a + 80]);
			}
			else
			{	plaintext[a] = ((95 - key_incoming_content[a + 80]) + ciphertext[a]);
			}
		}
		
		//Prints plaintext.
		for(int a = 0; a < 40; a++) {cout << "\n";}
		cout << "\nUser "; for(int a = 7; a < 24; a++) {cout << file_name_key_incoming[a];} cout << " says:\n";
		for(int a = 0; a < (plaintext[79]); a++) {cout << char(plaintext[a] + 32);}
		
		//Overwriting RAM of plaintext[].
		for(int a = 0; a < 80; a++)
		{	for(int b = 0; b < 10; b++)
			{	plaintext[a] = b;
			}
		} //Only partially effective, printed/copied text is cloned from RAM to RAM.
		
		//Overwriting RAM of key_incoming_content[].
		for(int a = 0; a < 160; a++)
		{	for(int b = 0; b < 10; b++)
			{	key_incoming_content[a] = b;
			}
		}
		
		//Copies entire incoming key into RAM before overwriting it.
		char truncating_table_private[100001] = {'\0'};
		in_stream.open(file_name_key_incoming);
		in_stream.get(truncating_table_private[0]);
		for(int a = 1; in_stream.eof() == false; a++) {in_stream.get(truncating_table_private[a]);}
		in_stream.close();
		
		//Overwrites the incoming key file twice with 100,000 characters each round.
		out_stream.open(file_name_key_incoming); for(int a = 0; a < 100000; a++) {out_stream << '\0';} out_stream.close(); //Binary: 00000000
		out_stream.open(file_name_key_incoming); for(int a = 0; a < 100000; a++) {out_stream.put(-1);} out_stream.close(); //Binary: 11111111
		
		//Removing or truncating the incoming key file by the first 160 characters.
		if(truncating_table_private[160] == '\0') {remove(file_name_key_incoming);}
		else
		{	out_stream.open(file_name_key_incoming);
			for(int a = 160; truncating_table_private[a] != '\0'; a++) {out_stream.put(truncating_table_private[a]);}
			out_stream.close();
		}
		
		//Overwriting RAM of truncating_table_private[];
		for(int a = 0; a < 100001; a++)
		{	for(int b = 0; b < 10; b++) {truncating_table_private[a] = b;}
		}
	}
	
	
	
	
	
	//______________________________________________________Get_keys_with_20k_________________________________________//
	if(user_option == 3)
	{	ifstream in_stream;
		ofstream out_stream;
		
		//Testing your fork & need keys fast? Set testing_mode to true (omits input for 90 seeds.)
		//Option 3 & 4 key files are essentially the same, only--here, you can say each key was used
		//and therefore truncated equally--simply giving you less times to use each individual key.
		//CAUTION: DO NOT PRODUCE KEYS IN TESTING MODE FOR ACTUAL USE! UNIX TIME CAN BE NARROWED DOWN!
		bool testing_mode = false; //False is good.
		
		//Quits if keys are present (prevents overwriting existing keys.)
		bool existence_of_file_option_3 = false;
		bool existence_of_file_option_4 = false;
		in_stream.open("./keys/option_3"); if(in_stream.fail() == false) {existence_of_file_option_3 = true;} in_stream.close();
		in_stream.open("./keys/option_4"); if(in_stream.fail() == false) {existence_of_file_option_4 = true;} in_stream.close();
		if((existence_of_file_option_3 == true)
		|| (existence_of_file_option_4 == true))
		{cout << "\n\nKeys already exist, run a new groupOTP.cpp file in a different folder.\n"; return 0;}
		
		//Gets inputs from user.
		cout << "\nEnter the maximum participants for this group: ";
		
		long long group_size;
		cin >> group_size;
		long long temp_group_size = group_size;
		if(group_size < 1) {cout << "\nInvalid, program ended.\n"; return 0;}
		if(testing_mode == true) {cout << "\n";}
		
		//Seeds automation IF TESTING.
		unsigned int user_seeds[90] = {0};
		if(testing_mode == true)
		{	srand(time(0));
			for(int a = 0; a < 90; a++) {user_seeds[a] = (rand() % 1000000000);}
		}
		else
		{	cout << "\nEnter a random nine-digit integer, repeat 90 times.\n\n";
			
			//Gets seeds for RNG.
			for(int a = 0; a < 90; a++)
			{	if(a < 9) {cout << " " << (a + 1) << " of 90: ";} //Prints blank to align input status report (aesthetics.)
				else      {cout <<        (a + 1) << " of 90: ";}
				
				//Gets and checks input.
				cin >> user_seeds[a];
				if((user_seeds[a] > 999999999) || (user_seeds[a] < 100000000)) {cout << "\nOut of bounds, try again.\n"; return 0;}
			}
			
			cout << "\nWait 8 seconds per batch...\n";
		}
		
		//Makes keys.
		mkdir("keys",  0777); //Creates a folder.
		char file_name_key[30]           = "./keys/00000000000000001/0000";
		long long file_name_key_batch_numbering = 100000000000000001; //Only the last non-zero integer-string value is used, size makes mod operations easier.
		unsigned char table_private[3200000] = {0};
		
		for(; group_size > 0; group_size--)
		{	//Fills table_private[] with randomness 0 - 94
			for(int a = 0; a < 90; a++) //Constructively applies random digits to table_private[] based on the 90 seeds provided by the user.
			{	srand(user_seeds[a]);   //WRITES ALTERNATING BETWEEN LEFT TO RIGHT & RIGHT TO LEFT. Alternation is based on the 90 user seeds.
				
				if((user_seeds[a] % 2) == 0)
				{	for(int b = 0; b < 3200000; b++) //WRITES LEFT TO RIGHT.
					{	table_private[b] += (rand() % 95);
						table_private[b] %= 95;
					}
				}
				else
				{	for(int b = 3199999; b >= 0; b--) //WRITES RIGHT TO LEFT.
					{	table_private[b] += (rand() % 95);
						table_private[b] %= 95;
					}
				}
			}
			
			//Adding additional randomness in table_private[].
			unsigned int seeds_sum = 0;
			for(int a = 0; a < 90; a++) {seeds_sum = ((seeds_sum + user_seeds[a]) % 1000000000);}
			srand(seeds_sum); //A new 9-digit seed comes from the sum of ALL user-seeds.
			for(int a = 0; a < 3200000; a++) //WRITES LEFT TO RIGHT.
			{	table_private[a] += (rand() % 95);
				table_private[a] %= 95;
			}
			
			//Again, adding additional randomness in table_private[].
			seeds_sum = 0;
			for(int a = 0; a < 90; a += 2) {seeds_sum = ((seeds_sum + user_seeds[a]) % 1000000000);}
			srand(seeds_sum); //Another new 9-digit seed comes from the sum of EVERY OTHER user-seed.
			for(int a = 3199999; a >= 0; a--) //WRITES RIGHT TO LEFT.
			{	table_private[a] += (rand() % 95);
				table_private[a] %= 95;
			}
			seeds_sum = 123456789; //Overwriting.
			
			//Filling user_seeds[] with a stack of 90 10-digit seeds from nicely-distributed locations in table_private[].
			for(int a = 0; a < 90; a++) {user_seeds[a] = 0;} //Resetting user_seeds[] since filling uses '+' only.
			int table_private_read_bookmark = 0;
			for(int a = 0; a < 90; a++)
			{	long long multiplier = 1000000000;
				for(int b = 0; b < 10; b++)
				{	user_seeds[a] += (table_private[table_private_read_bookmark] * multiplier);
					multiplier /= 10;
					//                                                                                                      _
					table_private_read_bookmark += 3555; //3,200,000 divided by (900 total times to extract digits) = 3,555.5
				}
			}
			
			//Creates a folder for a batch of keys.
			file_name_key[24] = '\0'; //Temporarily truncates dir to omit file.
			mkdir(file_name_key, 0777);
			file_name_key[24] = '/';
			
			//Writes batch of keys.
			table_private_read_bookmark = 0;
			for(int a = 0; a < 100; a++)
			{	//Applies file numbering.
				int temp_file_numbering = (a + 1000000);
				file_name_key[28] = (temp_file_numbering % 10) + 48; //        ./keys/00000000000000001/0  0  0  0
				temp_file_numbering /= 10; //                                                           ^  ^  ^  ^
				file_name_key[27] = (temp_file_numbering % 10) + 48; //                                      27 28    (element layout)
				
				//Creates key.
				out_stream.open(file_name_key);
				for(int b = 0; b < 32000; b++)
				{	out_stream.put(table_private[table_private_read_bookmark] + 32);
					table_private_read_bookmark++;
				}
				out_stream.close();
			}
			
			cout << "Key batch " << (file_name_key_batch_numbering % 100000000000000000) << " of " << temp_group_size << " created...\n";
			
			//Updates and applies key batch numbering.
			file_name_key_batch_numbering++;
			long long temp_file_name_key_batch_numbering = file_name_key_batch_numbering;
			for(int a = 23; a > 6; a--)
			{	file_name_key[a] = ((temp_file_name_key_batch_numbering % 10) + 48);
				temp_file_name_key_batch_numbering /= 10;
			}
		}
		
		//Overwrites RAM of user_seeds[].
		for(int a = 0; a < 90; a++)
		{	user_seeds[a] = 123456789;
			user_seeds[a] = 987604321;
			user_seeds[a] = 0;
		} //Only partially effective, printed/copied text is cloned from RAM to RAM.
		
		//Overwrites RAM of array table_private[].
		for(int a = 0; a < 3200000; a++)
		{	for(int b = 0; b < 10; b++) {table_private[a] = b;}
		}
		
		//Creates file which allows option 1 to create the correct counter of the remaining times to encrypt.
		out_stream.open("./keys/option_3");
		out_stream << "This helps option 1 give you the correct remaining times to encrypt.";
		out_stream.close();
		
		cout << "\nFinished! Share the keys folder with all participants\n"
		     << "in private, THEN assign identification numbers 1 to " << temp_group_size << ".\n"
		     << "Randomize assignment to conceal the number of users.\n\n";
	}
	
	
	
	
	
	//______________________________________________________Get_keys_with_1.6M________________________________________//
	if(user_option == 4)
	{	ifstream in_stream;
		ofstream out_stream;
		
		//Quits if keys are present (prevents overwriting existing keys.)
		bool existence_of_file_option_3 = false;
		bool existence_of_file_option_4 = false;
		in_stream.open("./keys/option_3"); if(in_stream.fail() == false) {existence_of_file_option_3 = true;} in_stream.close();
		in_stream.open("./keys/option_4"); if(in_stream.fail() == false) {existence_of_file_option_4 = true;} in_stream.close();
		if((existence_of_file_option_3 == true)
		|| (existence_of_file_option_4 == true))
		{cout << "\n\nKeys already exist, run a new groupOTP.cpp file in a different folder.\n"; return 0;}
		
		//Gets inputs from user.
		cout << "\nEnter the maximum participants for this group: ";
		
		long long group_size;
		cin >> group_size;
		long long temp_group_size = group_size;
		if(group_size < 1) {cout << "\nInvalid, program ended.\n"; return 0;}
		
		cout << "\nEnter a random nine-digit integer, repeat 90 times.\n\n";
		
		//Gets seeds for RNG.
		unsigned int user_seeds[90] = {0};
		for(int a = 0; a < 90; a++)
		{	if(a < 9) {cout << " " << (a + 1) << " of 90: ";} //Prints blank to align input status report (aesthetics.)
			else      {cout <<        (a + 1) << " of 90: ";}
			
			//Gets and checks input.
			cin >> user_seeds[a];
			if((user_seeds[a] > 999999999) || (user_seeds[a] < 100000000)) {cout << "\nOut of bounds, try again.\n"; return 0;}
		}
		
		cout << "\nWait 8 minutes per batch...\n";
		
		//Makes keys.
		mkdir("keys",  0777); //Creates a folder.
		char file_name_key[30]           = "./keys/00000000000000001/0000";
		long long file_name_key_batch_numbering = 100000000000000001; //Only the last non-zero integer-string value is used, size makes mod operations easier.
		static unsigned char table_private[256000000] = {0};
		
		for(; group_size > 0; group_size--)
		{	//Fills table_private[] with randomness 0 - 94
			for(int a = 0; a < 90; a++) //Constructively applies random digits to table_private[] based on the 90 seeds provided by the user.
			{	srand(user_seeds[a]);   //WRITES ALTERNATING BETWEEN LEFT TO RIGHT & RIGHT TO LEFT. Alternation is based on the 90 user seeds.
				
				if((user_seeds[a] % 2) == 0)
				{	for(int b = 0; b < 256000000; b++) //WRITES LEFT TO RIGHT.
					{	table_private[b] += (rand() % 95);
						table_private[b] %= 95;
					}
				}
				else
				{	for(int b = 255999999; b >= 0; b--) //WRITES RIGHT TO LEFT.
					{	table_private[b] += (rand() % 95);
						table_private[b] %= 95;
					}
				}
			}
			
			//Adding additional randomness in table_private[].
			unsigned int seeds_sum = 0;
			for(int a = 0; a < 90; a++) {seeds_sum = ((seeds_sum + user_seeds[a]) % 1000000000);}
			srand(seeds_sum); //A new 9-digit seed comes from the sum of ALL user-seeds.
			for(int a = 0; a < 256000000; a++) //WRITES LEFT TO RIGHT.
			{	table_private[a] += (rand() % 95);
				table_private[a] %= 95;
			}
			
			//Again, adding additional randomness in table_private[].
			seeds_sum = 0;
			for(int a = 0; a < 90; a += 2) {seeds_sum = ((seeds_sum + user_seeds[a]) % 1000000000);}
			srand(seeds_sum); //Another new 9-digit seed comes from the sum of EVERY OTHER user-seed.
			for(int a = 255999999; a >= 0; a--) //WRITES RIGHT TO LEFT.
			{	table_private[a] += (rand() % 95);
				table_private[a] %= 95;
			}
			seeds_sum = 123456789; //Overwriting.
			
			//Filling user_seeds[] with a stack of 90 10-digit seeds from nicely-distributed locations in table_private[].
			for(int a = 0; a < 90; a++) {user_seeds[a] = 0;} //Resetting user_seeds[] since filling uses '+' only.
			int table_private_read_bookmark = 0;
			for(int a = 0; a < 90; a++)
			{	long long multiplier = 1000000000;
				for(int b = 0; b < 10; b++)
				{	user_seeds[a] += (table_private[table_private_read_bookmark] * multiplier);
					multiplier /= 10;
					//                                                                                                            _
					table_private_read_bookmark += 284444; //256,000,000 divided by (900 total times to extract digits) = 284,444.4
				}
			}
			
			//Creates a folder for a batch of keys.
			file_name_key[24] = '\0'; //Temporarily truncates dir to omit file.
			mkdir(file_name_key, 0777);
			file_name_key[24] = '/';
			
			//Writes batch of keys.
			table_private_read_bookmark = 0;
			for(int a = 0; a < 2560; a++)
			{	//Applies file numbering.
				int temp_file_numbering = (a + 1000000);
				file_name_key[28] = (temp_file_numbering % 10) + 48; //        ./keys/00000000000000000/0  0  0  0
				temp_file_numbering /= 10; //                                                           ^  ^  ^  ^
				file_name_key[27] = (temp_file_numbering % 10) + 48; //                                25 26 27 28    (element layout)
				temp_file_numbering /= 10;
				file_name_key[26] = (temp_file_numbering % 10) + 48;
				temp_file_numbering /= 10;
				file_name_key[25] = (temp_file_numbering % 10) + 48;
				
				//Creates key.
				out_stream.open(file_name_key);
				for(int b = 0; b < 100000; b++)
				{	out_stream.put(table_private[table_private_read_bookmark] + 32);
					table_private_read_bookmark++;
				}
				out_stream.close();
			}
			
			cout << "Key batch " << (file_name_key_batch_numbering % 100000000000000000) << " of " << temp_group_size << " created...\n";
			
			//Updates and applies key batch numbering.
			file_name_key_batch_numbering++;
			long long temp_file_name_key_batch_numbering = file_name_key_batch_numbering;
			for(int a = 23; a > 6; a--)
			{	file_name_key[a] = ((temp_file_name_key_batch_numbering % 10) + 48);
				temp_file_name_key_batch_numbering /= 10;
			}
		}
		
		//Overwrites RAM of user_seeds[].
		for(int a = 0; a < 90; a++)
		{	user_seeds[a] = 123456789;
			user_seeds[a] = 987604321;
			user_seeds[a] = 0;
		} //Only partially effective, printed/copied text is cloned from RAM to RAM.
		
		//Overwrites RAM of array table_private[].
		for(int a = 0; a < 256000000; a++)
		{	for(int b = 0; b < 10; b++) {table_private[a] = b;}
		}
		
		//Creates file which allows option 1 to create the correct counter of the remaining times to encrypt.
		out_stream.open("./keys/option_4");
		out_stream << "This helps option 1 give you the correct remaining times to encrypt.";
		out_stream.close();
		
		cout << "\nFinished! Share the keys folder with all participants\n"
		     << "in private, THEN assign identification numbers 1 to " << temp_group_size << ".\n"
		     << "Randomize assignment to conceal the number of users.\n\n";
	}
}
