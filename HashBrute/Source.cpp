/*
*
*
*
Program Created by Miles Engelbrecht.
**NOTE**  
This program is not intended for malicious use.
This is a project that I made to further my own knowledge of threading, hash codes, and brute force techniques.
For best performance, I recommend setting the project in Release.
And please Note that only x64 works at this time.  (x32 can work with the proper libraries but that seemed redundant for this program).
Also, if you want the output screen to be cleaner, comment out all the couts within the functions.  I kept them there for testing purposes.

...Plus it's also kind of satisfying to see all the threads working.  
*
*
*
*
*/


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "openssl/sha.h"

#include <thread>





using namespace std;

string genHash(const string str)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);
	stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		ss << hex << setw(2) << setfill('0') << (int)hash[i];
	}
	return ss.str();
}





//Lowercase = 0-26; Uppercase = 26-52;  Numbers/Special = 52-64;

const char Alphabet[64] =
{
	'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u',
	'v', 'w', 'x', 'y', 'z', 'A', 'B',
	'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T', 'U',
	'V', 'W', 'X', 'Y', 'Z', '1', '2', 
	'3', '4', '5', '6', '7', '8', '9',
	'0', '!', '$'
};





/*
I tested whether making multiple functions was more efficent than passing in more values to this function
Test results were too similar, and fell within margin of error.  
*/





int GenerateSpecial(unsigned int length, string s, string code)
{


	if (length == 0)
	{
		cout << s << "\n";
		s = genHash(s);
		cout << s << "\n";
		if (s == code)
		{
			cout << "\n\n\n\n\n\n\n" << "Hash Found!" << "\n\n\n\n\n\n\n";

			cout << "\n\n\n\n\n\n\n" << s << " Is the string you are looking for" << "\n\n\n\n\n\n\n";
			exit(0);

		}
		return 0;
	}

	for (unsigned int i = 52; i < 64; i++) // iterate through Numbers/Special
	{
		// Create new string with next character
		// Call generate again until string has reached it's length
		string nxtStr = s + Alphabet[i];

		GenerateSpecial(length - 1, nxtStr, code);
	}
}

/*
This is the Full function, it iterates through every character in the array.
*/

int GenerateFull(unsigned int length, string s, string code)
{


	if (length == 0) // when length has been reached
	{
		cout << s << "\n";
		string s1 = genHash(s);
		cout << s1 << "\n";
		if (s1 == code)
		{
			//Was going to clear the console, however found many warnings against using system("cls");

			cout << "\n\n\n\n\n\n\n" << "Hash Found!" << "\n\n\n\n\n\n\n";

			cout << "\n\n\n\n\n\n\n" << s << " Is the string you are looking for" << "\n\n\n\n\n\n\n";

			exit(0);

		}
		return 0;
	}

	for (unsigned int i = 0; i < 64; i++) // iterate through entire Alphabet
	{
		

		string nxtStr = s + Alphabet[i];

		GenerateFull(length - 1, nxtStr, code);

	}
}



int Generate(unsigned int length, string s, string code, unsigned int i, unsigned int end)
{


	if (length == 0) // when length has been reached
	{
		cout << s << "\n";
		s = genHash(s);
		cout << s << "\n";
		if (s == code)
		{
			cout << "\n\n\n\n\n\n\n" << "Hash Found!" << "\n\n\n\n\n\n\n";

			cout << "\n\n\n\n\n\n\n" << s << " Is the string you are looking for" << "\n\n\n\n\n\n\n";
			exit(0);

		}
		return 0;
	}

	for (i; i < end; i++) //Iterates through 0-16/16-32/32-48/48-64.
	{
		/*
		While it only goes to 0-16, it calls the GenerateFull, which is what iterates through all letters in the Alphabet.
		But it only goes to 16.  If you were to just recursively call this function instead of GenerateFull, nxtStr would suffer from 
		Not getting 17-64 characters in the Alphabet.  
		*/
		
		string nxtStr = s + Alphabet[i];

		GenerateFull(length - 1, nxtStr, code);  
		
		
	}
}






int main() {


	//I hope you're using a Windows Machine.  
	system("color a");  //Make fun of me all you want, I wanted to get into that 'hacker' vibe.  Lol. 

	string code;
	cout << "Please enter the Hash you wish to crack... " << "\n";
	cin >> code;

	/*
	Please note that this system only goes up to 7 characters, you can make it go up to however many you want, I just
	felt it a little redundant to keep going for my purposes.   
	*/


	thread t0(Generate, 1, "", code, 0, 16);
	thread t1(Generate, 1, "", code, 16, 32);
	thread t2(Generate, 1, "", code, 32, 48);
	thread t3(Generate, 1, "", code, 48, 64);

	thread t0s(GenerateSpecial, 1, "", code);

	t0.join();
	t1.join();
	t2.join();
	t3.join();

	t0s.join();

	thread t4(Generate, 2, "", code, 0, 16);
	thread t5(Generate, 2, "", code, 16, 32);
	thread t6(Generate, 2, "", code, 32, 48);
	thread t7(Generate, 2, "", code, 48, 64);

	thread t1s(GenerateSpecial, 2, "", code);

	t4.join();
	t5.join();
	t6.join();
	t7.join();

	t1s.join();

	thread t8(Generate, 3, "", code, 0, 16);
	thread t9(Generate, 3, "", code, 16, 32);
	thread t10(Generate, 3, "", code, 32, 48);
	thread t11(Generate, 3, "", code, 48, 64);

	thread t2s(GenerateSpecial, 3, "", code);

	t8.join();
	t9.join();
	t10.join();
	t11.join();

	t2s.join();

	thread t12(Generate, 4, "", code, 0, 16);
	thread t13(Generate, 4, "", code, 16, 32);
	thread t14(Generate, 4, "", code, 32, 48);
	thread t15(Generate, 4, "", code, 48, 64);

	thread t3s(GenerateSpecial, 4, "", code);

	t12.join();
	t13.join();
	t14.join();
	t15.join();

	t3s.join();

	thread t16(Generate, 5, "", code, 0, 16);
	thread t17(Generate, 5, "", code, 16, 32);
	thread t18(Generate, 5, "", code, 32, 48);
	thread t19(Generate, 5, "", code, 48, 64);

	thread t4s(GenerateSpecial, 5, "", code);

	t16.join();
	t17.join();
	t18.join();
	t19.join();

	t4s.join();

	thread t20(Generate, 6, "", code, 0, 16);
	thread t21(Generate, 6, "", code, 16, 32);
	thread t22(Generate, 6, "", code, 32, 48);
	thread t23(Generate, 6, "", code, 48, 64);

	thread t5s(GenerateSpecial, 6, "", code);

	t20.join();
	t21.join();
	t22.join();
	t23.join();

	t5s.join();

	thread t24(Generate, 7, "", code, 0, 16);
	thread t25(Generate, 7, "", code, 16, 32);
	thread t26(Generate, 7, "", code, 32, 48);
	thread t27(Generate, 7, "", code, 48, 64);

	thread t6s(GenerateSpecial, 7, "", code);

	t24.join();
	t25.join();
	t26.join();
	t27.join();

	t6s.join();





  
	





	/*
	**OLD**
	Outdated system, after further testing, realized this was not a good way to go, because while the lower, upper, and special
	threads would run well, the threads would get caught waiting for full and fullend.  Meaning only 2 threads would be running 
	for an extended period of time.  Was not a fan so made a better system.

	thread t0(GenerateLower, 1, "", code);
	thread t1(GenerateUpper, 1, "", code);
	thread t2(GenerateSpecial, 1, "", code);
	thread t3(GenerateFull, 1, "", code);
	thread t4(GenerateFullEnd, 1, "", code);


	t0.join();
	thread t5(GenerateFull, 2, "", code);

	t1.join();
	thread t6(GenerateFullEnd, 2, "", code);

	t2.join();
	thread t7(GenerateSpecial, 2, "", code);

	t3.join();
	thread t8(GenerateLower, 2, "", code);

	t4.join();
	thread t9(GenerateUpper, 2, "", code);





	t5.join();
	thread t10(GenerateFull, 3, "", code);


	t6.join();
	thread t11(GenerateFullEnd, 3, "", code);


	t7.join();
	thread t12(GenerateSpecial, 3, "", code);


	t8.join();
	thread t13(GenerateLower, 3, "", code);

	t9.join();
	thread t14(GenerateUpper, 3, "", code);






	t10.join();
	thread t15(GenerateLower, 4, "", code);


	t11.join();
	thread t16(GenerateUpper, 4, "", code);


	t12.join();
	thread t17(GenerateSpecial, 4, "", code);


	t13.join();
	thread t18(GenerateFull, 4, "", code);


	t14.join();
	thread t19(GenerateFullEnd, 4, "", code);






	t15.join();
	thread t20(GenerateFull, 5, "", code);


	t16.join();
	thread t21(GenerateFullEnd, 5, "", code);


	t17.join();
	thread t22(GenerateSpecial, 5, "", code);


	t18.join();
	thread t23(GenerateLower, 5, "", code);


	t19.join();
	thread t24(GenerateUpper, 5, "", code);




	t20.join();
	thread t25(GenerateUpper, 6, "", code);


	t21.join();
	thread t26(GenerateLower, 6, "", code);


	t22.join();
	thread t27(GenerateSpecial, 6, "", code);


	t23.join();
	thread t28(GenerateFull, 6, "", code);


	t24.join();
	thread t29(GenerateFullEnd, 6, "", code);



	t25.join();
	thread t30(GenerateUpper, 7, "", code);


	t26.join();
	thread t31(GenerateLower, 7, "", code);


	t27.join();
	thread t32(GenerateSpecial, 7, "", code);


	t28.join();
	thread t33(GenerateFull, 7, "", code);


	t29.join();
	thread t34(GenerateFullEnd, 7, "", code);




	t30.join();
	thread t35(GenerateUpper, 8, "", code);


	t31.join();
	thread t36(GenerateLower, 8, "", code);


	t32.join();
	thread t37(GenerateSpecial, 8, "", code);


	t33.join();
	thread t38(GenerateFull, 8, "", code);


	t34.join();
	thread t39(GenerateFullEnd, 8, "", code);





	t35.join();
	thread t40(GenerateUpper, 9, "", code);


	t36.join();
	thread t41(GenerateLower, 9, "", code);


	t37.join();
	thread t42(GenerateSpecial, 9, "", code);


	t38.join();
	thread t43(GenerateFull, 9, "", code);


	t39.join();
	thread t44(GenerateFullEnd, 9, "", code);



	t40.join();
	t41.join();
	t42.join();
	t43.join();
	t44.join();

	*/

	
	cout << "\n\n\n\n\n\n\n\n" << "Could not be found!" << endl;

	//Cheers!

	return 0;
}