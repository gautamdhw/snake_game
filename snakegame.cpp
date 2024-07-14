#include<bits/stdc++.h> //for random function and other basic c++ functions
#include<conio.h> //for keyboard hit function
#include<windows.h> //co ordinate system control

using namespace std;

//max snake length defined is 1000
#define SNAKE_LENGTH 1000 

//directions char assign
//const used to establish them READ ONLY variables
const char UP ='U';
const char DOWN ='D';
const char RIGHT ='R';
const char LEFT ='L';

//global variables for defining console height & width
int console_height;
int console_width;

//bool variables repersents if border gets touched or not
bool border_touch=0;

//for intialising SCREEN -- how the game will start
void initScreen()
{
	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE); //this will give the control of console in hconsole variable
   	CONSOLE_SCREEN_BUFFER_INFO csbi;    //another predifened class used for getting console info..
   	GetConsoleScreenBufferInfo(hconsole, &csbi); //this func will put all the info in csbi
   	console_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
   	console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1; 	
}

//used structure for defining REFFERENCE POINT 
//no need for security in this POINT so used struct instead of class
struct Point
{
	int Xcoord;
	int Ycoord;
	
	Point(){  //empty cons bcoz need to make parametirezed
	}
	
	Point(int x,int y)  //kisi bhi point ka x,y value set krne ke liye
	{
		Xcoord = x;  
		Ycoord = y;
	}
};

//SNAKE CLASS 
class Snake
{
	int length; //length of snake
	char direction; //current direction of snake
	
	public:
		//making an array for snake body which contain points in it
		Point body[SNAKE_LENGTH]; //we need (x,y) POINT for each box of snake like if length is 5 we need 5 diff coordinates 1 for each box
		
		Snake(int x,int y)
		{
			length = 1; //starting me snake ki length 1 hogi
			body[0] = Point(x,y); //length 1 h to 0th index of body pe koi coordinate to hone chaiye vo hum isme pass krdenge
			direction = RIGHT; //by default jab game start hogi to snake right move krega
		}
		
		//function to get length of snake at anywhere as length is private
		int getlength()
		{
			return length;
		}
		
        //function for changing direction
        void changedirection(char newdirection)
        {
        	if(newdirection == UP && direction != DOWN) //jab down jaraha to up nhi jayega 180 degree not allowed
        	{
        		direction = newdirection;
			}
			
			else if(newdirection == DOWN && direction != UP) //jab up jaraha to down nhi jayega 180 degree not allowed
        	{
        		direction = newdirection;
			}
			
			else if(newdirection == LEFT && direction != RIGHT) //jab RIGHT jaraha to LEFT nhi jayega 180 degree not allowed
        	{
        		direction = newdirection;
			}
			
			else if(newdirection == RIGHT && direction != LEFT) //jab LEFT jaraha to RIGHT nhi jayega 180 degree not allowed
        	{
        		direction = newdirection;
			}
		}
		
		//func for movement of snake
		//made of boolean type because we need to check a condition if snake is eaten by itself 
		bool move(Point food) //food ki pos ayegi tohi check hoga food eat kiya h length badhado
		{
			//yaha pe snake ki length jab inc hogi to piche se copy hona shuru honge jo last tha ab vo last-1 bn jayega
			for(int i =length-1;i>0;i--) //loop ==0 ke liye nhi chlega kyuki copy sirf 1 st tak honge new head bnega acc to key pressed
			{
				body[i]=body[i-1];
			}
			
			//acc to new direction head kaha point krega-->
			switch(direction) //direction update hogi hogi upar wale func se
			{
				int val; //store purpose
				
				case UP:      //upar jayega to y coord- hoga
					val = body[0].Ycoord;
					body[0].Ycoord = val-1;
					break;
					
				case DOWN:   //niche ayega to y coord+ hoga
					val = body[0].Ycoord;
					body[0].Ycoord = val+1;
					break;
					
				case RIGHT:  //right jayega to x coord+ hoga
					val = body[0].Xcoord;
					body[0].Xcoord = val+1;
					break;
				
				case LEFT:  //left jayega to x coord- hoga
					val = body[0].Xcoord;
					body[0].Xcoord = val-1;
					break;		 	
			}
			
			//snake bites itself
			for(int i=1;i<length;i++)
			{
				//in this for loop we check that if head has same coord as of its body part then its crashed hence returns false
				if(body[0].Xcoord==body[i].Xcoord && body[0].Ycoord==body[i].Ycoord)
				{
					return false;
				}
			}
			
			 // Check if the snake hits the border
            if (body[0].Xcoord < 0 || body[0].Xcoord >= console_width || 
                 body[0].Ycoord < 0 || body[0].Ycoord >= console_height) 
			{
				    border_touch=1; //for printing in main 
                    return false; // Snake hits the border, game over
            }
			
			//snake eats food
			if(food.Xcoord==body[0].Xcoord && food.Ycoord==body[0].Ycoord) //head food pe agya to food eat
			{   
			    //piche wale me last-1 wala daldiya coords
				body[length] = Point(body[length-1].Xcoord,body[length-1].Ycoord);
				length++;
				//length bdha di
			}
			
			return true;   //sab shi h apne ap ko nhi khaya h
		}
};   

class Board
{
	Snake *snake; //dynamically created
	Point food;   //food will be at a point (x,y)
	int score;   //score means the no of times food eaten
	const char FOOD ='o'; //food will be donated by o
	const char SNAKE_BODY = 'O'; //snake will be donated by O
	
    public:
	
	Board(){
		spawnfood(); //when board will be shown food should be spawn on it
		snake = new Snake(10,10); //intially snake will start from 0,10
		score=0;
	}
	
	~Board()
	{
		delete snake;    //dynamically created object ko dlete krwane ke liye
	}
	
	//get score func which will return real time score
	int getscore()
	{
		return score;
	}
	
	//spawn food function
	void spawnfood()
	{
		int x = rand() % console_width; //random x coord for food
		int y = rand() % console_height; //random y coord for food % done to be in range
		food = Point(x,y);
	}  
	
	//to diplsay score
	void  displayscore()
	{
		gotoxy(console_width/2,0);    //score upar display krwan h to y=0 and centre me to width/2
		cout<<"Current Score : "<<score;
	}
	
	//setup current cursor
	void gotoxy(int x,int y)
	{
		COORD coord;
		coord.X=x;
		coord.Y=y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord); //it will setup cursor on that coords
	}
	
	//need to draw something on screen so make a draw finc
	void draw()
	{
		system("cls");  //need to clear screen before drawing
		
		//for loop is used for displaying snake on screen
		for(int i=0;i<snake->getlength();i++)
		{
			//each element's of snake body coord will be passed in goto func so that cursor goes there
			gotoxy(snake->body[i].Xcoord,snake->body[i].Ycoord);
			//and print snake body charachter
			cout<<SNAKE_BODY;
		}
		
		//for printing food on screen
		gotoxy(food.Xcoord,food.Ycoord); //set cursor where the food needs to be displayed
	    //and print food charchter
	    cout<<FOOD;
	    
	    displayscore();
	} 
	
	//update func btayega ki game age chlani h ya nhi
	bool update()
	{
		bool isalive = snake->move(food);
		if(!isalive)
		{
			return false;
		}
		
		//snake eats food
			if(food.Xcoord == snake->body[0].Xcoord && food.Ycoord == snake->body[0].Ycoord) //head food pe agya to food eat
			{   
			    score++; //score badh jayega 
			    spawnfood(); //nya food bna do
			}
			
		//ye func true return krega jab tak snake alive h
		return true;
	}
	
	//func jo ki btayega keyboard se kya press kre to konsi dir jaye
	void getinput()
	{
		if(kbhit()) //agar koi dbai h to true ayega
		{
			int key = getch();  //key ke ch ko int me badakl diya
			 
			 if(key == 'w' || key=='W')
			 {
			 	snake->changedirection(UP);
			 }
			 
			 else if(key == 'a' || key=='A')
			 {
			 	snake->changedirection(LEFT);
			 }
			 
			 else if(key == 's' || key=='S')
			 {
			 	snake->changedirection(DOWN);
			 }
			 
			 else if(key == 'd' || key=='D')
			 {
			 	snake->changedirection(RIGHT);
			 }
		}
	}
};   


int main()
{
	srand(time(0));
	initScreen(); //screen intialize hogi
	Board *board = new Board(); //board class ka new object dynamically create kr diya
	while(board->update()) //jab tak over na ho
	{
		board->getinput(); //new input lo
		board->draw();  //draw hojayega snake
		Sleep(150); //frame rate decide krega
	}
	system("cls");
	if(border_touch)  //to check if there is border touch then mention
	{
		cout<<"BORDER TOUCH"<<endl;
	}
	cout<<"GAME OVER"<<endl;
	cout<<"Final Score is :"<<board->getscore();
	
    return 0;
}