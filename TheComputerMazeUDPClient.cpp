// TheComputerMazeUDPClient.cpp : Defines the entry point for the console application.
//

/*
	Author: Josiah Murray
	Date Started: 02/03/2020
	Revision date: 
	Functional description: A bot to complete the computer maze
	User Advice: 
*/
#include "stdafx.h"
#include <winsock2.h>
#include <time.h>
#include <conio.h> 

#pragma comment(lib, "wsock32.lib")

#define STUDENT_NUMBER		"19018590"
#define STUDENT_FIRSTNAME	"Josiah"
#define STUDENT_FAMILYNAME	"Murray"

#define IP_ADDRESS_SERVER	"127.0.0.1"
//#define IP_ADDRESS_SERVER "164.11.80.69"


#define PORT_SERVER 0x1984 // We define a port that we are going to use.
#define PORT_CLIENT 0x1985 // We define a port that we are going to use.

#define MAX_FILENAME_SIZE 500

#define MAX_BUFFER_SIZE   5000
#define MAX_STRING_SIZE   200
#define MAX_NO_TOKENS     50

#define MAX_ITEMS_IN_ROOM		20
#define MAX_ITEMS_IN_BACKPACK	50

#define OPTION_MOVE_NORTH 1
#define OPTION_MOVE_SOUTH 2
#define OPTION_MOVE_EAST 3
#define OPTION_MOVE_WEST 4
#define OPTION_MOVE_UP 5
#define OPTION_MOVE_DOWN 6

#define OPTION_UNLOCK_NORTH 7
#define OPTION_UNLOCK_SOUTH 8
#define OPTION_UNLOCK_EAST 9 
#define OPTION_UNLOCK_WEST 10
#define OPTION_UNLOCK_UP 11
#define OPTION_UNLOCK_DOWN 12

#define OPTION_BASE_FOR_READS 200
#define OPTION_BASE_FOR_PICKUPS 500
#define OPTION_BASE_FOR_DROPS 800
#define OPTION_BASE_FOR_DOS 1100
#define OPTION_BASE_FOR_EVENTS 1300

 /*-------------------------------------
	Enumerator name: directions
	Description: defining a list of new data 
				 type for possible 
				 directions to take
  ---------------------------------------*/   
enum directions
{
	DIRECTION_NORTH = 0,
	DIRECTION_SOUTH = 1,
	DIRECTION_EAST = 2,
	DIRECTION_WEST = 3,
	DIRECTION_UP = 4,
	DIRECTION_DOWN = 5
};

/*-------------------------------------
	Enumerator name: direction_possible
	Description: defining a list of new data
				 type for checking if its 
				 possibe to go in a certain 
				 direction
  ---------------------------------------*/
enum direction_possible
{
	DIRECTION_NOT_PRESENT = -1,
	DIRECTION_LOCKED = 0,
	DIRECTION_OPEN = 1
};
/*-------------------------------------
	Enumerator name: item_types
	Description: defining a list of new 
				data types for different 
				item types
  ---------------------------------------*/
enum item_types
{
	ITEM_NONE = 0,
	ITEM_BOOK = 1,
	ITEM_JUNK = 2,
	ITEM_EQUIPMENT = 3,
	ITEM_MANUSCRIPT = 4,
	ITEM_TEST = 5,
	ITEM_OTHER = 10
};

/*-------------------------------------
	Enumerator name: typeofroom
	Description: defining a new data
				 type for the different
				 types of room
  ---------------------------------------*/
enum typeofroom {

	ROOM_NONE = 0,
	ROOM_LECTURE_SMALL = 1,
	ROOM_LECTURE_MEDIUM = 2,
	ROOM_LECTURE_LARGE = 3,
	ROOM_CORRIDOR = 4,
	ROOM_LAB_SMALL = 5,
	ROOM_LAB_MEDIUM = 6,
	ROOM_LAB_LARGE = 7,
	ROOM_MEETING_ROOM = 8,
	ROOM_SEMINAR = 9,
	ROOM_HIVE = 10, //one per floor
	ROOM_COFFEESHOP = 11, //one
	ROOM_LIBRARY = 12, //only one
	ROOM_SHOP_SELL = 13,
	ROOM_SHOP_BUY = 14,
	ROOM_SHOP_BUYSELL = 15,
	ROOM_OFFICE = 16, //maybe only one door
	ROOM_LOBBY = 17, //Only one
	ROOM_EXIT = 18, //only one
	ROOM_STAIRS = 19,
	ROOM_ENTRANCE = 20 //only one
};

/*-------------------------------------
	Structure name: Item
	Description: defining a new data type
				 for grouping an item's behaviour
  ---------------------------------------*/
struct Item
{
	int  number;
	int  value;
	int  volume;
};

/*-------------------------------------
	Structure name: Student
	Description: defining a new data type
				 for grouping a student's behaviour
  ---------------------------------------*/
struct Student
{
	int level;
	int rooms_visited;
	int doors_openned;
	int number_of_moves;
	int score;
};

/*-------------------------------------
	Structure name: Room
	Description: defining a new data type
				 for grouping a room's behaviour
  ---------------------------------------*/
struct Room
{
	char name[5];
	int  type;
	int  direction[6];
	int  number_of_keys;
	int  keys[4];
	int  number_of_items;
	Item items[MAX_ITEMS_IN_ROOM];
};

/*-------------------------------------
	Structure name: Backpack
	Description: defining a new data type
				 for grouping a backpack's behaviour
  ---------------------------------------*/
struct Backpack
{
	int number_of_items;
	Item items[MAX_ITEMS_IN_BACKPACK];
};




#define MAX_OPTIONS	50

int number_of_options;
int options[MAX_OPTIONS];

Student student;
Room room;
Backpack backpack;



SOCKADDR_IN server_addr;
SOCKADDR_IN client_addr;

SOCKET sock;  // This is our socket, it is the handle to the IO address to read/write packets

WSADATA data;




char InputBuffer[MAX_BUFFER_SIZE];

char Tokens[MAX_NO_TOKENS][MAX_STRING_SIZE];

char text_student[1000];
char text_backpack[1000];
char text_room[1000];
char text_keys[1000];
char text_items[1000];
char text_options[1000];



void sentOption(int option, int key)
{
	char buffer[100];

	sprintf(buffer, "Option %d, %x", option, key);
	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR*)&server_addr, sizeof(SOCKADDR));
}


/*************************************************************/
/********* Your tactics code starts here *********************/
/*************************************************************/
#define KNOWN_KEYS 17

int move = 0;

void yourMove()
{

	switch (move)
	{
		//floor 0
		/*-------------------------------------
			Current Room: 0Y06
			Description: Bot is moving south toward 0Y16
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 0:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;

		/*-------------------------------------
			Current Room: 0Y16
			Description: The bot is trying to unlock 
						 the door to the east
			Key using: 0x12FC
			Items: None
			Event: None
		---------------------------------------*/
	case 1:
		sentOption(OPTION_UNLOCK_EAST, 0x12FC);
		break;
		/*-------------------------------------
			Current Room: 0Y16
			Description: The bot is moving east toward 0Y17
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 2:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y17
			Description: The bot is moving south toward 0Y27
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 3:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y27
			Description: The bot is trying to pickup 
						 the gold processor
			Key using: None
			Items: The gold processor - £5.00, 2 litres
			Event: None
		---------------------------------------*/
	case 4:
		sentOption(OPTION_BASE_FOR_PICKUPS + 80, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y27
			Description: The bot is moving east toward 0Y28
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 5:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y28
			Description: The bot is trying to attend the 
						 digital principles event
			Key using: None
			Items: None
			Event: Digital principles
		---------------------------------------*/
	case 6:
		sentOption(OPTION_BASE_FOR_EVENTS + 43, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y28
			Description: The bot is trying to unlock 
						 the door to the south
			Key using: 0xAC7A
			Items: None
			Event: None
		---------------------------------------*/
	case 7:
		sentOption(OPTION_UNLOCK_SOUTH, 0xAC7A);
		break;
		/*-------------------------------------
			Current Room: 0Y28
			Description: The bot is moving south towards 0Y38
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 8:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y38
			Description: The bot is trying to unlock
						 the door to the east
			Key using: 0xBD70
			Items: None
			Event: None
		---------------------------------------*/
	case 9:
		sentOption(OPTION_UNLOCK_EAST, 0xBD70);
		break;
		/*-------------------------------------
			Current Room: 0Y38
			Description: The bot is moving east
						 towards 0Y39
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 10:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y39
			Description: The bot is trying to pickup
						 a book called Fundermentals
						 of calculus
			Key using: None
			Items: Book called Fundermentals of calculus
				   - £2.00, 1 litre
			Event: None
		---------------------------------------*/
	case 11:
		sentOption(OPTION_BASE_FOR_PICKUPS + 53, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y39
			Description: The bot is trying to read
						 a book called Fundermentals
						 of calculus
			Key using: None
			Items: Book called Fundermentals of calculus
				   - £2.00, 1 litre
			Event: None
		---------------------------------------*/
	case 12:
		sentOption(OPTION_BASE_FOR_READS + 53, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y39
			Description: The bot is trying to pickup
						 the CANS self-assessment test 6
			Key using: None
			Items: CANS self-assessment test 6 - £1.00,
				   1 litre
			Event: None
		---------------------------------------*/
	case 13:
		sentOption(OPTION_BASE_FOR_PICKUPS + 5, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y39
			Description: The bot is trying to do
						 the CANS self-assessment test 6
			Key using: None
			Items: CANS self-assessment test 6 - £1.00,
				   1 litre
			Event: None
		---------------------------------------*/
	case 14:
		sentOption(OPTION_BASE_FOR_DOS + 5, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y39
			Description: The bot is moving south
						 towards 0Y49
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 15:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y49
			Description: The bot is moving west
						 towards 0Y48
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 16:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y48
			Description: The bot is moving west
						 towards 0Y47
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 17:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y47
			Description: The bot is moving north
						 towards 0Y37
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 18:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y37
			Description: The bot is trying to attend the
						 Web development PAL event
			Key using: None
			Items: None
			Event: Web development PAL
		---------------------------------------*/
	case 19:
		sentOption(OPTION_BASE_FOR_EVENTS + 49, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y37
			Description: The bot is moving west
						 towards 0Y36
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 20:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y36
			Description: The bot is trying to
						 pickup a silicon sachet
			Key using: None
			Items: A silicon sachet - £3.00, 2 litres
			Event: None
		---------------------------------------*/
	case 21:
		sentOption(OPTION_BASE_FOR_PICKUPS + 39, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y36
			Description: The bot is trying to
						 pickup the 3D glasses
			Key using: None
			Items: A pair of 3D glasses - £10.00, 4 litres
			Event: None
		---------------------------------------*/
	case 22:
		sentOption(OPTION_BASE_FOR_PICKUPS + 138, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y36
			Description: The bot is trying to attend the
						Autonomous cars practical event
			Key using: None
			Items: None
			Event: Autonomous cars practical
		---------------------------------------*/
	case 23: 
		sentOption(OPTION_BASE_FOR_EVENTS + 38, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y36
			Description: The bot is trying to unlock
						 the door to the west
			Key using: 0x1696
			Items: None
			Event: None
		---------------------------------------*/
	case 24:
		sentOption(OPTION_UNLOCK_WEST, 0x1696);
		break;
		/*-------------------------------------
			Current Room: 0Y36
			Description: The bot is moving west
						 towards 0Y35
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 25:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y35
			Description: The bot is moving west
						 towards 0Y34
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 26:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y34
			Description: The bot is moving north
						 towards 0Y24
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 27:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y24
			Description: The bot is trying to unlock 
					     the door to the north
			Key using: 0xE4A8
			Items: None
			Event: None
		---------------------------------------*/
	case 28:
		sentOption(OPTION_UNLOCK_NORTH, 0xE4A8);
		break;
		/*-------------------------------------
			Current Room: 0Y24
			Description: The bot is moving north
						 towards 0Y14
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 29:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y14
			Description: The bot is trying to pickup
						 A golden ticket
			Key using: None
			Items: Golden Ticket - £10.00 , 4 litres
			Event: None
		---------------------------------------*/
	case 30:
		sentOption(OPTION_BASE_FOR_PICKUPS + 62, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y14
			Description: The bot is moving west
						 towards 0Y13
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 31:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y13
			Description: The bot is trying to pickup
						 a book called Test Driven 
						 development by example
			Key using: None
			Items: Test Driven development by example
			- £4.00, 2 litres
			Event: None
		---------------------------------------*/
	case 32:
		sentOption(OPTION_BASE_FOR_PICKUPS + 129, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y13
			Description: The bot is trying to read
						 a book called Test Driven
						 development by example
			Key using: None
			Items: Test Driven development by example
			- £4.00, 2 litres
			Event: None
		---------------------------------------*/
	case 33:
		sentOption(OPTION_BASE_FOR_READS + 129, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y13
			Description: The bot is moving south
						 towards 0Y23 (A corridor)
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 34:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y23
			Description: The bot is moving south
						 towards 0Y33 (A corridor)
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 35:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y33
			Description: The bot is moving west
						 towards 0Y32 (A corridor)
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 36:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y32
			Description: The bot is moving west
						 towards 0Y42
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 37:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y42
			Description: The bot is moving west
						 towards 0Y43
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 38:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y43
			Description: The bot is trying to pickup
						 a plastic crown
			Key using: None
			Items: A plastic crown - £3.00, 2 litres
			Event: None
		---------------------------------------*/
	case 39:
		sentOption(OPTION_BASE_FOR_PICKUPS + 125, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y43
			Description: The bot is trying to pickup
						 a book called tackling complexity 
						 in the heart of software
			Key using: None
			Items: tackling complexity in the heart 
				   of software - £2.00, 2 litres
			Event: None
		---------------------------------------*/
	case 40:
		sentOption(OPTION_BASE_FOR_PICKUPS + 133, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y43
			Description: The bot is trying to read
						 a book called tackling complexity
						 in the heart of software
			Key using: None
			Items: tackling complexity in the heart
				   of software - £2.00, 2 litres
			Event: None
		---------------------------------------*/
	case 41:
		sentOption(OPTION_BASE_FOR_READS + 133, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y43
			Description: The bot is trying to unlock
						 the door to the east
			Key using: 0x43AB
			Items: None
			Event: None
		---------------------------------------*/
	case 42:
		sentOption(OPTION_UNLOCK_EAST, 0x43AB);
		break;
		/*-------------------------------------
			Current Room: 0Y43
			Description: The bot is moving west
						 towards 0Y44
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 43:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y44
			Description: The bot is trying to pickup
						 a book called 2000 leagues 
						 under the sea
			Key using: None
			Items: 2000 leagues under the sea -
			£3.00, 2 litres
			Event: None
		---------------------------------------*/
	case 44:
		sentOption(OPTION_BASE_FOR_PICKUPS + 22, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y44
			Description: The bot is trying to read
						 a book called 2000 leagues
						 under the sea
			Key using: None
			Items: 2000 leagues under the sea -
			£3.00, 2 litres
			Event: None
		---------------------------------------*/
	case 45:
		sentOption(OPTION_BASE_FOR_READS + 22, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y44
			Description: The bot is trying to pickup
						 a USB stick
			Key using: None
			Items: USB stick - £2.00, 1 litre
			Event: None
		---------------------------------------*/
	case 46:
		sentOption(OPTION_BASE_FOR_PICKUPS + 86, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y44
			Description: The bot is moving south 
						 towards 0Y54
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 47:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y54
			Description: The bot is moving west
						 towards 0Y53
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 48:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y53
			Description: The bot is trying to pickup
						 a book called refactoring:
						 Improving the design of 
						 existing code
			Key using: None
			Items: refactoring:Improving the design
			of existing code - £1.00, 2 litre
			Event: None
		---------------------------------------*/
	case 49:
		sentOption(OPTION_BASE_FOR_PICKUPS + 16, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y53
			Description: The bot is trying to read
						 a book called refactoring:
						 Improving the design of
						 existing code
			Key using: None
			Items: refactoring:Improving the design
			of existing code - £1.00, 2 litre
			Event: None
		---------------------------------------*/
	case 50:
		sentOption(OPTION_BASE_FOR_READS + 16, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y53
			Description: The bot is trying to pickup
						 a Measuring tape
			Key using: None
			Items: Measuring tape - £2.00, 1 litre
			Event: None
		---------------------------------------*/
	case 51:
		sentOption(OPTION_BASE_FOR_PICKUPS + 51, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y53
			Description: The bot is trying to unlock
						 the door to the west
			Key using: 0x37EB
			Items: None
			Event: None
		---------------------------------------*/
	case 52:
		sentOption(OPTION_UNLOCK_WEST, 0x37EB);
		break;
		/*-------------------------------------
			Current Room: 0Y53
			Description: The bot is moving west
						 towards 0Y52
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 53:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y52
			Description: The bot is moving south
						 towards 0Y62
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 54:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y62
			Description: The bot is trying to pickup 
						 a book called Life of Pi
			Key using: None
			Items: life of pi - £1.00, 2 litre
			Event: None
		---------------------------------------*/
	case 55:
		sentOption(OPTION_BASE_FOR_PICKUPS + 59, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y62
			Description: The bot is trying to read
						 a book called Life of Pi
			Key using: None
			Items: life of pi - £1.00, 2 litre
			Event: None
		---------------------------------------*/
	case 56:
		sentOption(OPTION_BASE_FOR_READS + 59, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y62
			Description: The bot is moving west
						 towards 0Y61
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 57:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y61
			Description: The bot is moving north
						 towards 0Y51
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 58:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y51
			Description: The bot is trying to pickup
						 a vaccum cleaner
			Key using: None
			Items: Vacuum cleaner - £1.00, 1 litre
			Event: None
		---------------------------------------*/
	case 59:
		sentOption(OPTION_BASE_FOR_PICKUPS + 64, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y51
			Description: The bot is trying to pickup
						 a book called Working effectivly
						 with legacy code
			Key using: None
			Items: Working effectivly with legacy code
			- £2.00, 2 litres
			Event: None
		---------------------------------------*/
	case 60:
		sentOption(OPTION_BASE_FOR_PICKUPS + 136, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y51
			Description: The bot is trying to read
						 a book called Working effectivly
						 with legacy code
			Key using: None
			Items: Working effectivly with legacy code
			- £2.00, 2 litres
			Event: None
		---------------------------------------*/
	case 61:
		sentOption(OPTION_BASE_FOR_READS + 136, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y51
			Description: The bot is trying to unlock
						 the door to the north
			Key using: 0xD2F7
			Items: None
			Event: None
		---------------------------------------*/
	case 62:
		sentOption(OPTION_UNLOCK_NORTH, 0xD2F7);
		break;
		/*-------------------------------------
			Current Room: 0Y51
			Description: The bot is moving north
						 towards 0Y41
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 63:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y41
			Description: The bot is trying to attend 
						 a Network security PAL event
			Key using: None
			Items: None
			Event: Network security PAL
		---------------------------------------*/
	case 64:
		sentOption(OPTION_BASE_FOR_EVENTS + 27, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y41
			Description: The bot is moving north
						 towards 0Y31
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 65:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y31
			Description: The bot is trying to unlock
						 the door to the west
			Key using: 0xE28F
			Items: None
			Event: None
		---------------------------------------*/
	case 66:
		sentOption(OPTION_UNLOCK_WEST, 0xE28F);
		break;
		/*-------------------------------------
			Current Room: 0Y31
			Description: The bot is moving west
						 toward 0Y30 
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 67:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y30
			Description: The bot is trying to unlock
						 the door to the north
			Key using: 0x90F8
			Items: None
			Event: None
		---------------------------------------*/
	case 68:
		sentOption(OPTION_UNLOCK_NORTH, 0x90F8);
		break;
		/*-------------------------------------
			Current Room: 0Y30
			Description: The bot is moving north
						 towards 0Y20
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 69:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
	case 70:
		/*-------------------------------------
			Current Room: 0Y20
			Description: The bot is trying to do
						 the CANS self assessment 
						 test 1
			Key using: None
			Items: CANS self assessment test 1
			- £1.00, 1 litres
			Event: None
		---------------------------------------*/
	case 71:
		sentOption(OPTION_BASE_FOR_DOS + 88, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y20
			Description: The bot is moving south
						 towards 0Y30
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 72:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y30
			Description: The bot is moving south
						 towards 0Y40
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 73:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y40
			Description: The bot is moving south
						 towards 0Y50
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 74:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y50
			Description: The bot is trying to 
						 unlock the door to the 
						 south
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 75:
		sentOption(OPTION_UNLOCK_SOUTH, 0x0666);
		break;
		/*-------------------------------------
			Current Room: 0Y50
			Description: The bot is moving south
						 towards 0Y60
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 76:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y60
			Description: The bot is moving south
						 towards 0Y70
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 77:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y70
			Description: The bot is trying to 
						 pickup a Dragon plush toy
			Key using: None
			Items: Dragon plush toy - £5.00, 2 litre
			Event: None
		---------------------------------------*/
	case 78:
		sentOption(OPTION_BASE_FOR_PICKUPS + 127, 0); 
		break;
		/*-------------------------------------
			Current Room: 0x70
			Description: The bot is moving east
						 towards 0Y71
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 79:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y71
			Description: The bot is moving south
						 towards 0Y81
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 80:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y81
			Description: The bot is moving south
						 towards 0Y91
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 81:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y91
			Description: The bot is moving east
						 towards 0Y92
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 82:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y92
			Description: The bot is trying to unlock
						 the door to the east
			Key using: 0x7A0B
			Items: None
			Event: None
		---------------------------------------*/
	case 83:
		sentOption(OPTION_UNLOCK_EAST, 0x7A0B);
		break;
		/*-------------------------------------
			Current Room: 0Y92
			Description: The bot is moving east
						 towards 0Y93
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 84:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y93
			Description: The bot is moving east
						 towards 0Y94
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 85:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
	case 86:
		/*-------------------------------------
			Current Room: 0Y94
			Description: The bot is trying to 
						 read Test cheat sheet 
						 book
			Key using: None
			Items: Test cheat sheet book- £5.00, 2 litres
			Event: None
		---------------------------------------*/
	case 87:
		sentOption(OPTION_BASE_FOR_READS + 135, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y94
			Description: The bot is trying to unlock
						 the door to the east 
			Key using: 0xD2F7
			Items: None
			Event: None
		---------------------------------------*/
	case 88:
		sentOption(OPTION_UNLOCK_EAST, 0xD2F7);
		break;
		/*-------------------------------------
			Current Room: 0Y94
			Description: The bot is moving east
						 towards 0Y95
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 89:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y95
			Description: The bot is moving east
						 towards 0Y96
			Key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 90:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y96
			Description: The bot is moving north
						 towards 0Y86
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 91:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y86
			Description: The bot is trying to pickup 
						 a vinyl disc
			Key using: None
			Items: A vinyl disc - £10.00, 4 litres
			Event: None
		---------------------------------------*/
	case 92:
		sentOption(OPTION_BASE_FOR_PICKUPS + 78, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y86
			Description: The bot is moving north
						 towards 0Y76
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 93:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y76
			Description: The bot is trying to 
						 unlock the door to the
						 west
			key using: 0x1AE1
			Items: None
			Event: None
		---------------------------------------*/
	case 94:
		sentOption(OPTION_UNLOCK_WEST, 0x1AE1);
		break;
		/*-------------------------------------
			Current Room: 0Y76
			Description: The bot is moving west
						 towards 0Y75
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 95:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y75
			Description: The bot is moving west
						 towards 0Y74
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 96:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y74
			Description: The bot The bot is trying to 
						 pickup Martin's AI Research
			key using: None
			Items: Martin's AI Research - £10.00, 4 litres
			Event: None
		---------------------------------------*/
	case 97:
		sentOption(OPTION_BASE_FOR_PICKUPS + 1, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y74
			Description: The bot is trying to 
						 attend the Software 
						 development practical event
			key using: None
			Items: None
			Event: Software development practical
		---------------------------------------*/
	case 98:
		sentOption(OPTION_BASE_FOR_EVENTS + 12, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y74
			Description: The bot is moving east
						 towards 0Y75
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 99:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y75
			Description: The bot is moving north
						 towards 0Y65
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 100:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y65
			Description: The bot is moving east
						 towards 0Y66
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 101:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y66
			Description: The bot is moving east
						 towards 0Y67
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 102:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
	case 103:
		/*-------------------------------------
			Current Room: 0Y67
			Description: The bot is trying to read
						 Anonymous's guide to hacks
			key using: None
			Items: Anonymous's guide to hacks - £5.00, 2 litres
			Event: None
		---------------------------------------*/
	case 104:
		sentOption(OPTION_BASE_FOR_READS + 9, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y67
			Description: The bot is moving north
						 towards 0Y57
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 105:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y57
			Description: The bot is moving east
						 towards 0Y58
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 106:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y58
			Description: The bot is moving east
						 towards 0Y59
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 107:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y59
			Description: The bot is moving south
						 towards 0Y69
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 108:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y69
			Description: The bot is trying to attend
						 a Mobile application 
						 development practical
			key using: None
			Items: None
			Event: Mobile application development practical
		---------------------------------------*/
	case 109:
		sentOption(OPTION_BASE_FOR_EVENTS + 9, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y69
			Description: The bot is trying to unlock
						 the door to the west
			key using: 0x8FD4
			Items: None
			Event: None
		---------------------------------------*/
	case 110:
		sentOption(OPTION_UNLOCK_WEST, 0x8FD4);
		break;
		/*-------------------------------------
			Current Room: 0Y69
			Description: The bot is moving west
						 towards 0Y68
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 111:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y68
			Description: The bot is moving south
						 towards 0Y78
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 112:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y78
			Description: The bot is moving east
						 towards 0Y79
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 113:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y79
			Description: The bot is trying to pickup
						 Last year's programming notes
			key using: None
			Items: Last year's programming notes, £0.00, 1 litre
			Event: None
		---------------------------------------*/
	case 114:
		sentOption(OPTION_BASE_FOR_PICKUPS + 93, 0); 
		break;
		/*-------------------------------------
			Current Room: 0Y79
			Description: The bot is moving south
						 towards 0Y89
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 115:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y89
			Description: The bot is moving south
						 towards 0Y99
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 116:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 0Y99
			Description: The bot is moving upstairs
						 towards 1Y99
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 117:
		sentOption(OPTION_MOVE_UP, 0);
		break;

		//floor 1
		/*-------------------------------------
			Current Room: 1Y99
			Description: The bot is trying to unlock
						 the door to the west
			key using: 0x49EA
			Items: None
			Event: None
		---------------------------------------*/
	case 118:
		sentOption(OPTION_UNLOCK_WEST, 0x49EA);
		break;
		/*-------------------------------------
			Current Room: 1Y99
			Description: The bot is moving west 
						 towards 1Y98
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 119:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y98
			Description: The bot is moving north
						 towards 1Y88
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 120:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y88
			Description: The bot is moving west
						 towards 1Y78
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 121:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y78
			Description: The bot is trying to unlock
						 the door to the north
			key using: 0x43AB
			Items: None
			Event: None
		---------------------------------------*/
	case 122:
		sentOption(OPTION_UNLOCK_NORTH, 0x43AB);
		break;
		/*-------------------------------------
			Current Room: 1Y78
			Description: The bot is moving north
						 towards 1Y68
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 123:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y68
			Description: The bot is trying to read
						 Harry potter and the 
						 philosopher's stone
			key using: None
			Items: Harry potter and the philosopher's 
				   stone - £1.00, 2 litres
			Event: None
		---------------------------------------*/
	case 124:
		sentOption(OPTION_BASE_FOR_READS + 38, 0);
		break;   
		/*-------------------------------------
			Current Room: 1Y68
			Description: The bot is trying to attend a
						 Network Engineering practical
						 event
			key using: None
			Items: None
			Event: Network Engineering practical
		---------------------------------------*/
	case 125:
		sentOption(OPTION_BASE_FOR_EVENTS + 44, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y68
			Description: The bot is trying to unlock
						 the door towards the west
			key using: 0xC430
			Items: None
			Event: None
		---------------------------------------*/
	case 126:
		sentOption(OPTION_UNLOCK_WEST, 0xC430);
		break;
		/*-------------------------------------
			Current Room: 1Y68
			Description: The bot is moving west
						 towards 1Y67
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 127:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y67
			Description: The bot is moving north
						 towards 1Y57
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 128:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
	case 129:
		/*-------------------------------------
			Current Room: 1Y57
			Description: The bot is trying to read
						 Introduction to algorithms
			key using: None
			Items: Introduction to Algorithms
				   - £3.00, 2 litres
			Event: None
		---------------------------------------*/
	case 130:
		sentOption(OPTION_BASE_FOR_READS + 119, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y57
			Description: The bot is trying to unlock 
						 the door to the west
			key using: 0x8F32
			Items: None
			Event: None
		---------------------------------------*/
	case 131:
		sentOption(OPTION_UNLOCK_WEST, 0x8F32);
		break;
		/*-------------------------------------
			Current Room: 1Y57
			Description: The bot is moving west
						 towards 1Y56
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 132:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y56
			Description: The bot is moving west
						 towards 1Y55
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 133:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y55
			Description: The bot is trying to unlock
						 the door to the west
			key using: 0x1AE1
			Items: None
			Event: None
		---------------------------------------*/
	case 134:
		sentOption(OPTION_UNLOCK_WEST, 0x1AE1);
		break;
		/*-------------------------------------
			Current Room: 1Y55
			Description: The bot is moving west
						 towards 1Y54
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 135:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y54
			Description: The bot is trying to unlock
						 the door to the south
			key using: 0x8F32
			Items: None
			Event: None
		---------------------------------------*/
	case 136:
		sentOption(OPTION_UNLOCK_SOUTH, 0x8F32);
		break; 
		/*-------------------------------------
			Current Room: 1Y54
			Description: The bot is moving south
						 towards 1Y64
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 137:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y64
			Description: The bot is moving south
						 towards 1Y74
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 138:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y74
			Description: The bot is trying to drop
						 the book life of pi
			key using: None
			Items: life of pi - £1.00, 2 litres 
			Event: None
		---------------------------------------*/
	case 139:
		sentOption(OPTION_BASE_FOR_DROPS + 59, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y74
			Description: The bot is trying to pickup
						 C programming for dummies
			key using: None
			Items: C programming for dummies - £4.00,
				   2 litres 
			Event: None
		---------------------------------------*/
	case 140:
		sentOption(OPTION_BASE_FOR_PICKUPS + 134, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y74
			Description: The bot is trying to read
						 C programming for dummies
			key using: None
			Items: C programming for dummies - £4.00,
				   2 litres
			Event: None
		---------------------------------------*/
	case 141:
		sentOption(OPTION_BASE_FOR_READS + 134, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y74
			Description: The bot is trying to unlock 
						 the door to the south
			key using: 0x200F
			Items: None
			Event: None
		---------------------------------------*/
	case 142:
		sentOption(OPTION_UNLOCK_SOUTH, 0x200F);
		break;
		/*-------------------------------------
			Current Room: 1Y74
			Description: The bot is moving south
						 towards 1Y84
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 143:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y84
			Description: The bot is trying to attend
						 a Safety critical systems 
						 practical Event
			key using: None
			Items: None
			Event: Safety critical systems practical
		---------------------------------------*/
	case 144:
		sentOption(OPTION_BASE_FOR_EVENTS + 56, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y84
			Description: The bot is moving west
						 towards 1Y83
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 145:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y83
			Description: The bot is trying to read
						  a book called Pro Grit
			key using: None
			Items: Pro Grit - £5.00, 2 litres
			Event: None
		---------------------------------------*/
	case 146:
		sentOption(OPTION_BASE_FOR_READS + 94, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y83
			Description: The bot is moving south
						 towards 1Y93
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 147:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y93
			Description: The bot is trying to unlock
						 the door to the west
			key using: 0x22A0
			Items: None
			Event: None
		---------------------------------------*/
	case 148:
		sentOption(OPTION_UNLOCK_WEST, 0x22A0);
		break;
		/*-------------------------------------
			Current Room: 1Y93
			Description: The bot is moving west
						 towards 1Y92
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 149:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y92
			Description: The bot is moving west
						 towards 1Y91
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 150:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y91
			Description: The bot is trying to unlock 
						 the door to the north 
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 151:
		sentOption(OPTION_UNLOCK_NORTH, 0xA6D8);
		break;
		/*-------------------------------------
			Current Room: 1Y91
			Description: The bot is moving north
						 towards 1Y81
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 152:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y81
			Description: The bot is trying to read 
						 a book called Easy make, giant
						 killer robots
			key using: None
			Items: Easy make, giant killer robots - £4.00,
				   2 litres
			Event: None
		---------------------------------------*/
	case 153:
		sentOption(OPTION_BASE_FOR_READS + 74, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y81
			Description: The bot is trying to unlock the 
						 door to the north
			key using: 0x58B8
			Items: None
			Event: None
		---------------------------------------*/
	case 154:
		sentOption(OPTION_UNLOCK_NORTH, 0x58B8);
		break;
		/*-------------------------------------
			Current Room: 1Y81
			Description: The bot is moving north
						 towards 1Y71
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 155:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y71
			Description: The bot is moving north
						 towards 1Y61
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 156:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y61
			Description: The bot is moving east
						 towards 1Y62
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 157:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y62
			Description: The bot is trying to do
						 the CaNS Self-assessment
						 Test 7
			key using: None
			Items: CaNS Self-assessment Test 7 - £1.00, 1 litres
			Event: None
		---------------------------------------*/
	case 158:
		sentOption(OPTION_BASE_FOR_DOS + 132, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y62
			Description: The bot is trying to unlock
					     the door to the east
			key using: 0x7EB9
			Items: None
			Event: None
		---------------------------------------*/
	case 159:
		sentOption(OPTION_UNLOCK_EAST, 0x7EB9);
		break;
		/*-------------------------------------
			Current Room: 1Y62
			Description: The bot is moving east
						 towards 1Y63
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 160:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y63
			Description: The bot is moving east
						 towards 1Y53
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 161:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y53
			Description: The bot is trying to unlock
						 the door to the north
			key using: 0x9F4A
			Items: None
			Event: None
		---------------------------------------*/
	case 162:
		sentOption(OPTION_UNLOCK_NORTH, 0x9F4A);
		break;
		/*-------------------------------------
			Current Room: 1Y53
			Description: The bot is moving north
						 towards 1Y43
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 163:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y43
			Description: The bot is trying to read
						 JavaScript: the Good Parts
			key using: None
			Items: JavaScript: the Good Parts - £2.00, 
				   2 litres
			Event: None
		---------------------------------------*/
	case 164:
		sentOption(OPTION_BASE_FOR_READS + 108, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y43
			Description: The bot is trying to attend
						 Fault Tolerant Systems practical
			key using: None
			Items: None
			Event: Fault Tolerant Systems practical
		---------------------------------------*/
	case 165:
		sentOption(OPTION_BASE_FOR_EVENTS + 57, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y43
			Description: The bot is moving west
						 towards 1Y42
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 166:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y42
			Description: The bot is trying to read
						 Hackers stole my car
			key using: None
			Items: Hackers stole my car - £4.00,
				   2 litres
			Event: None
		---------------------------------------*/
	case 167:
		sentOption(OPTION_BASE_FOR_READS + 77, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y42
			Description: The bot is moving North
						 towards 1Y32
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 168:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y32
			Description: The bot is trying to unlock
						 the door to the west
			key using: 0x2B8D
			Items: None
			Event: None
		---------------------------------------*/
	case 169:
		sentOption(OPTION_UNLOCK_WEST, 0x2B8D);
		break;
		/*-------------------------------------
			Current Room: 1Y32
			Description: The bot is moving west
						 towards 1Y31
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 170:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y31
			Description: The bot is trying to unlock 
						 the door to the south
			key using: 0xD2F7
			Items: None
			Event: None
		---------------------------------------*/
	case 171:
		sentOption(OPTION_UNLOCK_SOUTH, 0xD2F7);
		break;
		/*-------------------------------------
			Current Room: 1Y31
			Description: The bot is moving south
						 towards 1Y41
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 172:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y41
			Description: The bot is moving west
						 towards 1Y40
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 173:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y40
			Description: The bot is trying to read
						 A code of conduct for 
						 professional Programmers
			key using: None
			Items:  A code of conduct for professional
					Programmers - £1.00,  2 litres
			Event: None
		---------------------------------------*/
	case 174:
		sentOption(OPTION_BASE_FOR_READS + 67, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y40
			Description: The bot is moving north
						 towards 1Y30
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 175:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y30
			Description: The bot is trying to read
						 Peopleware: Productive Projects
						 and Teams
			key using: None
			Items:  Peopleware: Productive Projects and 
					Teams - £2.00,  2 litres
			Event: None
		---------------------------------------*/
	case 176:
		sentOption(OPTION_BASE_FOR_READS + 79, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y30
			Description: The bot is trying to unlock 
						 the door to the north
			key using: 0x2C86
			Items: None
			Event: None
		---------------------------------------*/
	case 177:
		sentOption(OPTION_UNLOCK_NORTH, 0x2C86);
		break;
		/*-------------------------------------
			Current Room: 1Y30
			Description: The bot is moving north
						 towards 1Y20
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 178:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y20
			Description: The bot is trying to unlock
						 the door to the east
			key using: 0x03FB
			Items: None
			Event: None
		---------------------------------------*/
	case 179:
		sentOption(OPTION_UNLOCK_EAST, 0x03FB);
		break;
		/*-------------------------------------
			Current Room: 1Y20
			Description: The bot is moving east
						 towards 1Y21
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 180:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y21
			Description: The bot is moving east
						 towards 1Y22
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 181:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y22
			Description: The bot is moving east
						 towards 1Y23
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 182:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y23
			Description: The bot is trying to read
						 The Lion, the Witch and the 
						 Wardrobe
			key using: None
			Items: The Lion, the Witch and the Wardrobe
					- £5.00, 0 litres
			Event: None
		---------------------------------------*/
	case 183:
		sentOption(OPTION_BASE_FOR_READS + 41, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y23
			Description: The bot is trying to pickup
						 The Lion, the Witch and the
						 Wardrobe
			key using: None
			Items: The Lion, the Witch and the Wardrobe
					- £5.00, 0 litres
			Event: None
		---------------------------------------*/
	case 184:
		sentOption(OPTION_BASE_FOR_PICKUPS + 41, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y23
			Description: The bot is trying to unlock
						 the door to the east
			key using: 0xF02E
			Items: None
			Event: None
		---------------------------------------*/
	case 185:
		sentOption(OPTION_UNLOCK_EAST, 0xF02E);
		break;
		/*-------------------------------------
			Current Room: 1Y23
			Description: The bot is moving east
						 towards 1Y24
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 186:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y24
			Description: The bot is moving east
						 towards 1Y25
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 187:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y25
			Description: The bot is trying to read
						 how to hack the pentagon
			key using: None
			Items: how to hack the pentagon
					- £5.00, 2 litres
			Event: None
		---------------------------------------*/
	case 188:
		sentOption(OPTION_BASE_FOR_READS + 95, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y25
			Description: The bot is trying to attend
						 Database Administration 
						 seminar event
			key using: None
			Items: None
			Event: Database Administration seminar
		---------------------------------------*/
	case 189:
		sentOption(OPTION_BASE_FOR_EVENTS + 26, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y25
			Description: The bot is moving south
						 towards 1Y35
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 190:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y35
			Description: The bot is trying to unlock
						 the door to the east
			key using: 0x130E
			Items: None
			Event: None
		---------------------------------------*/
	case 191:
		sentOption(OPTION_UNLOCK_EAST, 0x130E);
		break;
		/*-------------------------------------
			Current Room: 1Y35
			Description: The bot is moving east
						 towards 1Y36
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 192:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y36
			Description: The bot is moving north
						 towards 1Y26
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 193:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y26
			Description: The bot is trying to read
						 C programming, Tony Royce
			key using: None
			Items: C programming, Tony Royce
					- £4.00, 2 litres
			Event: None
		---------------------------------------*/
	case 194:
		sentOption(OPTION_BASE_FOR_READS + 96, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y26
			Description: The bot is moving east
						 towards 1Y27
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 195:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y27
			Description: The bot is moving north
						 towards 1Y17
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 196:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y17
			Description: The bot is moving east
						 towards 1Y18
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 197:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y18
			Description: The bot is moving east
						 towards 1Y19
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 198:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y19
			Description: The bot is moving north
						 towards 1Y09
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 199:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 1Y09
			Description: The bot is moving upstairs
						 towards 2Y09
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 200: 
		sentOption(OPTION_MOVE_UP, 0);
		break;

		//Floor 2
		/*-------------------------------------
			Current Room: 2Y09
			Description: The bot is moving west
						 towards 2Y08
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 201:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y08
			Description: The bot is moving west
						 towards 2Y07
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 202:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y07
			Description: The bot is moving south
						 towards 2Y17
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 203:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y17
			Description: The bot is moving east
						 towards 2Y18
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 204:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y18
			Description: The bot is moving south
						 towards 2Y28
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 205:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y28
			Description: The bot is moving west
						 towards 2Y27
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 206:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y27
			Description: The bot is trying to read
						 the hobbit
			key using: None
			Items: The Hobbit - £1.00, 2 litre
			Event: None
		---------------------------------------*/
	case 207:
		sentOption(OPTION_BASE_FOR_READS + 37, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y27
			Description: The bot is trying to attend
						 Information Technology 
						 Management practical event
			key using: None
			Items: None
			Event: Information Technology Management
				   practical
		---------------------------------------*/
	case 208:
		sentOption(OPTION_BASE_FOR_EVENTS + 50, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y27
			Description: The bot is moving south
						 towards 2Y37
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 209:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y37
			Description: The bot is trying to attend
						 Project Management PAL session 
			key using: None
			Items: None
			Event: Project Management PAL session 
		---------------------------------------*/
	case 210: 
		sentOption(OPTION_BASE_FOR_EVENTS + 24, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y37
			Description: The bot is moving west
						 towards 2Y36
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 211:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y36
			Description: The bot is trying to unlock
						 the door to the west
			key using: 0x12FC
			Items: None
			Event: None
		---------------------------------------*/
	case 212:
		sentOption(OPTION_UNLOCK_WEST, 0x12FC);
		break;
		/*-------------------------------------
			Current Room: 2Y36
			Description: The bot is moving west
						 towards 2Y35
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 213:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y35
			Description: The bot is moving west
						 towards 2Y34
			key using: None
			Items: None
			Event: None
		---------------------------------------*/ 
	case 214:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y34
			Description: The bot is trying to attend
						CaNS practical event
			key using: None
			Items: None
			Event: CaNS practical
		---------------------------------------*/
	case 215:
		sentOption(OPTION_BASE_FOR_EVENTS + 3, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y34
			Description: The bot is moving north
						 towards 2Y24
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 216:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y24
			Description: The bot is trying to unlock
						 the door to the north
			key using: 0xD000
			Items: None
			Event: None
		---------------------------------------*/
	case 217:
		sentOption(OPTION_UNLOCK_NORTH, 0xD000);
		break;
		/*-------------------------------------
			Current Room: 2Y24
			Description: The bot is moving north
						 towards 2Y14
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 218:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y14
			Description: The bot is trying to attend
						 Programming in C++ practical
			key using: None
			Items: None
			Event: Programming in C++ practical
		---------------------------------------*/
	case 219:
		sentOption(OPTION_BASE_FOR_EVENTS + 6, 0);
		break;
	case 220:
		/*-------------------------------------
			Current Room: 2Y14
			Description: The bot is moving north
						 towards 2Y04
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 221:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y04
			Description: The bot is moving west
						 towards 2Y03
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 222:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y03
			Description: The bot is moving west
						 towards 2Y02
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 223:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y02
			Description: The bot is moving south
						 towards 2Y12
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 224:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y12
			Description: The bot is trying to unlock
						 the door to the south
			key using: 0xBE27
			Items: None
			Event: None
		---------------------------------------*/
	case 225:
		sentOption(OPTION_UNLOCK_SOUTH, 0xBE27);
		break;
		/*-------------------------------------
			Current Room: 2Y12
			Description: The bot is moving south
						 towards 2Y22
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 226:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y22
			Description: The bot is trying to read
						 skynet instuctions manual
			key using: None
			Items: skynet instuctions manual - £3.00, 2 litre
			Event: None
		---------------------------------------*/
	case 227:
		sentOption(OPTION_BASE_FOR_READS + 23, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y22
			Description: The bot is trying to do
						 the AI self assessment test
						 3
			key using: None
			Items: AI self assessment test 3 - £1.00, 1 litre
			Event: None
		---------------------------------------*/
	case 228:
		sentOption(OPTION_BASE_FOR_DOS + 33, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y22
			Description: The bot is moving west
						 towards 2Y21
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 229:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y22
			Description: The bot is trying to unlock
						 the door to the south
			key using: 0xBE27
			Items: None
			Event: None
		---------------------------------------*/
	case 230:
		sentOption(OPTION_UNLOCK_SOUTH, 0xBE27);
		break;
		/*-------------------------------------
			Current Room: 2Y21
			Description: The bot is moving west
						 towards 2Y31
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 231:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y31
			Description: The bot is trying to do
						 CaNS Self - assessment 
						 Test 2
			key using: None
			Items: CaNS Self-assessment Test 2 - £1.00, 1 litre
			Event: None
		---------------------------------------*/
		
	case 232:
		sentOption(OPTION_BASE_FOR_DOS + 91, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y22
			Description: The bot is trying to attend
						 Software Systems Engineering
						 practical 
			key using: None
			Items: None
			Event: Software Systems Engineering practical 
		---------------------------------------*/
	case 233:
		sentOption(OPTION_BASE_FOR_EVENTS + 7, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y31
			Description: The bot is moving south
						 towards 2Y41
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 234:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y41
			Description: The bot is moving west
						 towards 2Y40
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 235:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y40
			Description: The bot is moving west
						 towards 2Y50
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 236:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y50
			Description: The bot is moving east
						 towards 2Y51
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 237:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y51
			Description: The bot is moving east
						 towards 2Y52
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 238:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y52
			Description: The bot is moving north
						 towards 2Y42
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 239:
		sentOption(OPTION_MOVE_NORTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y42
			Description: The bot is trying to attend
						 Data warehousing practical 
			key using: None
			Items: None
			Event: Data warehousing practical 
		---------------------------------------*/
	case 240:
		sentOption(OPTION_BASE_FOR_EVENTS + 11, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y42
			Description: The bot is moving east
						 towards 2Y43
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 241:
		sentOption(OPTION_MOVE_EAST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y43
			Description: The bot is moving south
						 towards 2Y53
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 242:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y53
			Description: The bot is trying to unlock
						 the door to the south
			key using: 0x6C61
			Items: None
			Event: None
		---------------------------------------*/
	case 243:
		sentOption(OPTION_UNLOCK_SOUTH, 0x6C61);
		break;
		/*-------------------------------------
			Current Room: 2Y53
			Description: The bot is moving south
						 towards 2Y63
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 244:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break; 
		/*-------------------------------------
			Current Room: 2Y63
			Description: The bot is trying to attend
						 Database Management practical
			key using: None
			Items: None
			Event: Database Management practical
		---------------------------------------*/
	case 245:
		sentOption(OPTION_BASE_FOR_EVENTS + 52, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y63
			Description: The bot is moving south
						 towards 2Y73
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 246:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y73
			Description: The bot is moving west
						 towards 2Y72
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 247:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y72
			Description: The bot is moving south
						 towards 2Y82
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 248:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y82
			Description: The bot is trying to unlock
						 the door to the south
			key using: 0x69F4
			Items: None
			Event: None
		---------------------------------------*/
	case 249:
		sentOption(OPTION_UNLOCK_WEST, 0x69F4);
		break;
		/*-------------------------------------
			Current Room: 2Y82
			Description: The bot is moving west
						 towards 2Y81
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 250:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y81
			Description: The bot is moving west
						 towards 2Y80
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 251:
		sentOption(OPTION_MOVE_WEST, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y80
			Description: The bot is moving south
						 towards 2Y90
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 252:
		sentOption(OPTION_MOVE_SOUTH, 0);
		break;
		/*-------------------------------------
			Current Room: 2Y90
			Description: The bot is moving upstairs
						 towards 3Y90
			key using: None
			Items: None
			Event: None
		---------------------------------------*/
	case 253:
		sentOption(OPTION_MOVE_UP, 0);
		break;

	default: // This statement runs if the bot has no more instructions
		sentOption(OPTION_MOVE_WEST, 0);
		break;
	}
	move++;
	
}


/*************************************************************/
/********* Your tactics code ends here ***********************/
/*************************************************************/



int getTokens(char* instring, char seperator)
{
	int  number_of_tokens;
	char chr;
	int  state;
	int  i;
	int  j;


	for (i = 0; i < MAX_NO_TOKENS; i++)
	{
		for (j = 0; j < MAX_STRING_SIZE; j++)
		{
			Tokens[i][j] = '\0';
		}
	}

	number_of_tokens = -1;
	chr = instring[0];
	state = 0;
	i = 0;

	while (chr != '\0')
	{
		switch (state)
		{
		case 0:  // Initial state
			if (chr == seperator)
			{
				number_of_tokens++;
				state = 1;
			}
			else if ((chr == ' ') || (chr == '\t') || (chr == '\n'))
			{
				state = 1;
			}
			else
			{
				number_of_tokens++;
				j = 0;
				Tokens[number_of_tokens][j] = chr;
				Tokens[number_of_tokens][j + 1] = '\0';
				state = 2;
			}
			break;

		case 1:  // Leading white space
			if (chr == seperator)
			{
				number_of_tokens++;
				state = 1;
			}
			else if ((chr == ' ') || (chr == '\t') || (chr == '\n'))
			{
				state = 1;
			}
			else
			{
				number_of_tokens++;
				j = 0;
				Tokens[number_of_tokens][j] = chr;
				Tokens[number_of_tokens][j + 1] = '\0';
				state = 2;
			}
			break;

		case 2:  // Collecting chars
			if (chr == seperator)
			{
				//number_of_tokens++;
				state = 1;
			}
			else
			{
				j++;
				Tokens[number_of_tokens][j] = chr;
				Tokens[number_of_tokens][j + 1] = '\0';
				state = 2;
			}
			break;

		default:
			break;
		}

		i++;
		chr = instring[i];
	}

	return (number_of_tokens + 1);
}



bool getline(FILE *fp, char *buffer)
{
	bool rc;
	bool collect;
	char c;
	int  i;

	rc = false;
	collect = true;

	i = 0;
	while (collect)
	{
		c = getc(fp);

		switch (c)
		{
		case EOF:
			if (i > 0)
			{
				rc = true;
			}
			collect = false;
			break;

		case '\n':
			if (i > 0)
			{
				rc = true;
				collect = false;
				buffer[i] = '\0';
			}
			break;

		default:
			buffer[i] = c;
			i++;
			break;
		}
	}

	return (rc);
}



void printRoom()
{
	int i;

	printf("Room\n");
	printf("Room = %s, Room type = %d\n", room.name, room.type);

	printf("Directions = ");
	for (i = 0; i < 6; i++)
	{
		printf("%d  ", room.direction[i]);
	}
	printf("\n");

	if (room.number_of_keys > 0)
	{
		printf("Keys = ");
		for (i = 0; i < room.number_of_keys; i++)
		{
			printf("0x%X  ", room.keys[i]);
		}
		printf("\n");
	}
	else
	{
		printf("No keys in this room\n");
	}

	if (room.number_of_items > 0)
	{
		for (i = 0; i < room.number_of_items; i++)
		{
			printf("Item=%d, Value=%d, Volume=%d\n", room.items[i].number, room.items[i].value, room.items[i].volume);
		}
	}
	else
	{
		printf("No items in this room\n");
	}

	printf("\n");
}


void printStudent()
{
	printf("Student\n");
	printf("Level=%d,  Number of rooms visited = %d,  Number of doors openned = %d,  Number of moves = %d,  Score = %d\n", student.level, student.rooms_visited, student.doors_openned, student.number_of_moves, student.score);
	printf("\n");
}


void printBackpack()
{
	int i;

	printf("Backpack\n");

	if (backpack.number_of_items > 0)
	{
		for (i = 0; i < backpack.number_of_items; i++)
		{
			printf("Item=%d, Value=%d, Volume=%d\n", backpack.items[i].number, backpack.items[i].value, backpack.items[i].volume);
		}
	}
	else
	{
		printf("Your backpack is empty\n");
	}
	printf("\n");
}


void printOptions()
{
	int i;

	printf("Options\n");
	printf("Options = ");
	for (i = 0; i < number_of_options; i++)
	{
		printf("%d  ", options[i]);
	}
	printf("\n");
	printf("\n");
}




void communicate_with_server()
{
	char buffer[4096];
	int  len = sizeof(SOCKADDR);
	int  i;
	char* p;
	int	 number_of_tokens;


	sprintf_s(buffer, "Register  %s %s %s", STUDENT_NUMBER, STUDENT_FIRSTNAME, STUDENT_FAMILYNAME);
	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));

	while (true)
	{
		memset(buffer, '\0', sizeof(buffer));

		if (recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (SOCKADDR *)&client_addr, &len) != SOCKET_ERROR)
		{
			p = ::inet_ntoa(client_addr.sin_addr);

			if ((strcmp(IP_ADDRESS_SERVER, "127.0.0.1") == 0) || (strcmp(IP_ADDRESS_SERVER, p) == 0))
			{
				printf("%s\n\n", buffer);

				number_of_tokens = getTokens(buffer, '|');

				if (number_of_tokens == 6)
				{
					strcpy(text_student, Tokens[0]);
					strcpy(text_backpack, Tokens[1]);
					strcpy(text_room, Tokens[2]);
					strcpy(text_keys, Tokens[3]);
					strcpy(text_items, Tokens[4]);
					strcpy(text_options, Tokens[5]);

					printf("Student  = '%s'\n", text_student);
					printf("Backpack = '%s'\n", text_backpack);
					printf("Room     = '%s'\n", text_room);
					printf("Keys     = '%s'\n", text_keys);
					printf("Items    = '%s'\n", text_items);
					printf("Options  = '%s'\n", text_options);

					student.level = -1;
					student.rooms_visited = -1;
					student.doors_openned = -1;
					student.number_of_moves = -1;

					if (sscanf(text_student, "%d,%d,%d,%d,%d", &student.level, &student.rooms_visited, &student.doors_openned, &student.number_of_moves, &student.score) == 5)
					{
					}

					if (strlen(text_backpack) > 0)
					{
						backpack.number_of_items = getTokens(text_backpack, '&');

						if (backpack.number_of_items > 0)
						{
							for (i = 0; i < backpack.number_of_items; i++)
							{
								if (i < MAX_ITEMS_IN_BACKPACK)
								{
									backpack.items[i].number = -1;

									if (sscanf(Tokens[i], "%d, %d, %d", &backpack.items[i].number, &backpack.items[i].value, &backpack.items[i].volume) == 3)
									{
									}
								}
							}
						}
					}
					else
					{
						backpack.number_of_items = 0;
					}

					sscanf(text_room, "%s ,%d, %d, %d, %d, %d, %d, %d", &room.name, &room.type, &room.direction[DIRECTION_NORTH], &room.direction[DIRECTION_SOUTH], &room.direction[DIRECTION_EAST], &room.direction[DIRECTION_WEST], &room.direction[DIRECTION_UP], &room.direction[DIRECTION_DOWN]);

					if (strlen(text_keys) > 0)
					{
						room.number_of_keys = getTokens(text_keys, '&');

						if (room.number_of_keys > 0)
						{
							for (i = 0; i < room.number_of_keys; i++)
							{
								if (i < 4)
								{
									room.keys[i] = -1;

									if (sscanf(Tokens[i], "%x", &room.keys[i]) == 1)
									{
									}
								}
							}
						}
					}
					else
					{
						room.number_of_keys = 0;
					}

					if (strlen(text_items) > 0)
					{
						room.number_of_items = getTokens(text_items, '&');

						if (room.number_of_items > 0)
						{
							for (i = 0; i < room.number_of_items; i++)
							{
								if (i < MAX_ITEMS_IN_ROOM)
								{
									room.items[i].number = -1;

									if (sscanf(Tokens[i], "%d, %d, %d", &room.items[i].number, &room.items[i].value, &room.items[i].volume) == 3)
									{
									}
								}
							}
						}
					}
					else
					{
						room.number_of_items = 0;
					}

					if (strlen(text_options) > 0)
					{
						number_of_options = getTokens(text_options, ',');

						if (number_of_options > 0)
						{
							for (i = 0; i < number_of_options; i++)
							{
								if (i < MAX_OPTIONS)
								{
									options[i] = -1;

									if (sscanf(Tokens[i], "%d", &options[i]) == 1)
									{
									}
								}
							}
						}
					}
					else
					{
						number_of_options = 0;
					}
				}

				printStudent();
				printBackpack();
				printRoom();
				printOptions();

				system("timeout /t 60");

				yourMove();
			}
		}
		else
		{
			printf_s("recvfrom error = %d\n", WSAGetLastError());
		}
	}

	printf_s("Student %s\n", STUDENT_NUMBER);
}




int main()
{
	char chr = '\0';

	printf("\n");
	printf("The Computer Maze Student Program\n");
	printf("UWE Computer and Network Systems Assignment 2 \n");
	printf("\n");

	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return(0);

	//sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	//if (!sock)
	//{	
	//	printf("Socket creation failed!\n"); 
	//}

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	if (!sock) 
	{
		// Creation failed! 
	}

	memset(&server_addr, 0, sizeof(SOCKADDR_IN));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS_SERVER);
	server_addr.sin_port = htons(PORT_SERVER);

	memset(&client_addr, 0, sizeof(SOCKADDR_IN));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_addr.sin_port = htons(PORT_CLIENT);

	//int ret = bind(sock_recv, (SOCKADDR *)&receive_addr, sizeof(SOCKADDR));
	////	int ret = bind(sock_send, (SOCKADDR *)&receive_addr, sizeof(SOCKADDR));
	//if (ret)
	//{
	//	printf("Bind failed! %d\n", WSAGetLastError());
	//}

	communicate_with_server();

	closesocket(sock);
	WSACleanup();

	while (chr != '\n')
	{
		chr = getchar();
	}

	return 0;
}

