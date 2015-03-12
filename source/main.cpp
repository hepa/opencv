//\====================================================================================
//\ Author: Peter.Chui
//\ Date  : 24 July 2012
//\ About : main.cpp - Defines the entry point for our Simple SDL Project
//\====================================================================================

#include <opencv\cvaux.h>
#include <opencv\highgui.h>
#include <opencv\cxcore.h>

#include <Windows.h>
#include "GL_Functions.h"
#include "pctimer.h"
#include <string>
#include <cmath>
#include <crtdbg.h>
#include <time.h>

//#include <iostream>
//#include <fstream>

using namespace std;

// don't move bat within this value change
float threshold = 7.0f;

enum State {
	STARTUP,
	MAIN_MENU,
	GAME,
	SCORE,
	HIGHSCORE,
	HOWTOPLAY,
	EXIT = -1
};

enum SubState{
	INIT,
	UPDATE,
	DRAW,
	CLEANUP
};

State g_GameState = STARTUP;
State g_NextGameState = MAIN_MENU;
//State g_NextGameState = GAME;

//global variables

// multi-dimension array for storing which texture to draw on the background
int g_aBackground[24][32];

//ifstream fin;
//fin.open("./images/bkgTile.txt");

// constant screen dimension
const float SCREEN_WIDTH = 1024;
const float SCREEN_HEIGHT = 700;

//declare variables for all tiled images for background, bats and ball
int top;
int topLeft;
int topRight;
int bottom;
int bottomLeft;
int bottomRight;
int leftT;
int rightT;
int center;
int bat;
int ball;

//position for player1 bat
float player1_x = 900;
float player1_y = 500;
//float prev_player1_y = 500;

//position for player2 bat
float player2_x = 50;
float player2_y = 100;
//float prev_player2_y = 100;

//Constant dimension for bat
const float BAT_WIDTH = 30;
const float BAT_HEIGHT = 100;

//Constant dimension for ball
const float BALL_WIDTH = 30;
const float BALL_HEIGHT = 30;

//position for ball
float ball_x;
float ball_y;

float ballspeed_x = 17.0f;
float ballspeed_y = 2.0f;

int player1Score = 0;
int player2Score = 0;

int TimeScore = 0;
int highTimeScore = 0;

bool isHighTimeScore = false;

int a_highTimeScore[5] = {0,0,0,0,0};

//float Rot = 0;

void runGameState(SubState a_sstate);
void initialisePlayersBallPosition();
void cleanUp();
void initTexture();

//Variables for calculating speed of bat swing during hitting the ball
clock_t timer;
clock_t startTimer;
float startPositionY;
clock_t endTimer;
float endPositionY;
int col, row, z, y, maxx = 0, maxy = 0, minx = 700, miny = 700;
int maxx2 = 0, maxy2 = 0, minx2 = 700, miny2 = 700;


/****************************************************************************/
// Main Function Main 
// parameters: sub state
// return type: void
// expected use: to initialize the game program
/****************************************************************************/
int main(int argc, char* argv[])
{
	//Lets open the window and initialize openGL
	InitGL(SCREEN_WIDTH,SCREEN_HEIGHT);

	CvSize size1280x720 = cvSize(640, 480);

	CvCapture* p_capWebcam;

	IplImage* p_imgOriginal;
	IplImage* p_imgProcessed;
	IplImage* p_imgProcessed_hsv;
	IplImage* p_imgProcessed_blue_cut;
	IplImage* p_imgProcessed_yellow_cut;



	CvMemStorage* p_strStorage;

	CvSeq* p_seqCircles;

	
	bool volt = false;
	uchar b, g, r;
	uchar b2, g2, r2;

	float* p_fltXYRadius = NULL;
	int i;
	char charCheckForEscKey;

	p_capWebcam = cvCreateCameraCapture(-1);//cvCaptureFromAVI("D:\2015-02-26-1711-234.avi");
	if (p_capWebcam == NULL) {
		printf("ERROR: capture is null. \n");
		getchar();
		return(-1);
	}

	assert(p_capWebcam);

	cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Processed_yellow", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Processed_blue", CV_WINDOW_AUTOSIZE);
	
	p_imgProcessed = cvCreateImage(size1280x720, IPL_DEPTH_8U, 1);
	p_imgProcessed_hsv = cvCreateImage(size1280x720, IPL_DEPTH_8U, 3);
	p_imgProcessed_blue_cut = cvCreateImage(size1280x720, IPL_DEPTH_8U, 1);
	p_imgProcessed_yellow_cut = cvCreateImage(size1280x720, IPL_DEPTH_8U, 1);
	
	int db = 0;

	while (FrameworkUpdate() && g_GameState != EXIT) //Do some secret stuff, 
	{
		if (g_NextGameState != g_GameState)
		{
			//cleanup the old state
			runGameState(CLEANUP);
			g_GameState = g_NextGameState;

			//initialize the new state
			runGameState(INIT);
		}

		//--------------------

		p_imgOriginal = cvRetrieveFrame(p_capWebcam);
		if (p_imgOriginal == NULL) {
			printf("ERROR: image capture is null.");
			getchar();
			break;
		}		
		
		//cvInRangeS(p_imgOriginal, CV_RGB(160, 0, 0), CV_RGB(255, 60, 60), p_imgProcessed);

		//convert rgb to hsv picture
		cvCvtColor(p_imgOriginal, p_imgProcessed_hsv, CV_BGR2HSV);

		//yellow color range
		CvScalar hsv_min_yellow = cvScalar(160, 200, 100 );
		CvScalar hsv_max_yellow = cvScalar(180,255,255 );
		
		//blue color range
		CvScalar hsv_min_blue = cvScalar(100, 200,120 );
		CvScalar hsv_max_blue = cvScalar(150,255,255 );

		//cut picture for two color
		cvInRangeS(p_imgProcessed_hsv, hsv_min_blue, hsv_max_blue, p_imgProcessed_blue_cut);
		cvInRangeS(p_imgProcessed_hsv, hsv_min_yellow, hsv_max_yellow, p_imgProcessed_yellow_cut);
		//cvInRangeS(p_imgOriginal, CV_RGB(160, 0, 0), CV_RGB(255, 60, 60), p_imgProcessed);
		maxx = 0; maxy = 0; minx = 700; miny = 700;
		maxx2 = 0; maxy2 = 0; minx2 = 700; miny2 = 700;
		for (y = 0; y < p_imgProcessed->height; y+=2)
		{
			for (col = 0; col < p_imgProcessed->width; col+=2)
			{
				//for( z = 0; z < img->nChannels; z++ )
				//{
				//   c = img->imageData[img->widthStep * row + col * img->nChannels + z];
				//}
				b = p_imgProcessed->imageData[p_imgProcessed->widthStep * y + col * 3];
				g = p_imgProcessed->imageData[p_imgProcessed->widthStep * y + col * 3 + 1];
				r = p_imgProcessed->imageData[p_imgProcessed->widthStep * y + col * 3 + 2];

				b2 = p_imgProcessed_blue_cut->imageData[p_imgProcessed_blue_cut->widthStep * y + col * 3];
				g2 = p_imgProcessed_blue_cut->imageData[p_imgProcessed_blue_cut->widthStep * y + col * 3 + 1];
				r2 = p_imgProcessed_blue_cut->imageData[p_imgProcessed_blue_cut->widthStep * y + col * 3 + 2];
				

				if (b == 255 && r == 255 && g == 255) {
					if (y>maxy) maxy = y;
					if (y < miny) miny = y;
					
				}

				if (b2 == 255 && r2 == 255 && g2 == 255) {
					if (y>maxy2) maxy2 = y;
					if (y < miny2) miny2 = y;
					volt = true;
				}
			}
		}

		//if (volt)	
		//printf("%d %d %d %d\n", minx2, miny2, maxx2, maxy2);
		

		//cvShowImage("Original", p_imgOriginal);
		cvShowImage("Original", p_imgOriginal);
		cvShowImage("Processed_blue", p_imgProcessed_blue_cut);
		cvShowImage("Processed_yellow", p_imgProcessed_yellow_cut);

			

		//--------------------
		runGameState(UPDATE);
		runGameState(DRAW);

		//Stop it from running too fast! Sleep ZZzzz
		//Sleep(1);		
	} 

	//Before you exit, clean up after yourself
	runGameState(CLEANUP);

	//Close down
	CloseDown();

	_CrtDumpMemoryLeaks();

	cvReleaseCapture(&p_capWebcam);
	cvDestroyWindow("Original");
	cvDestroyWindow("Processed");

	//Quit!
	return 0;
}


/****************************************************************************/
// Function mainMenu Sub-state
// parameters: sub state
// return type: void
// expected use:  to display the Main Menu 
/****************************************************************************/
void mainMenu(SubState a_subState)
{
	switch (a_subState)
	{
		case INIT:
			initTexture();

			ball_x = 800;
			ball_y = 450;
			player1Score = 0;
			player2Score = 0;

			TimeScore = 0;

			break;

		case UPDATE:
			//Player press ENTER to start the game
			if (IsKeyDown(KEY_F1))
				g_NextGameState = GAME;  //Change to GAME State

			if (IsKeyDown(KEY_F2))
				g_NextGameState = HIGHSCORE;  //Change Game SubState to HighScore

			if (IsKeyDown(KEY_ESCAPE))
				g_NextGameState = EXIT;  //Change Game SubState to HighScore


			break;

		case DRAW:
			//Clear the screen, so previous frames don't build up
			ClearScreen();

			//Draw background
			for (int y=0; y < 24; y++)
			{
				for (int x=0; x < 32; x++)
				{
					DrawSprite(g_aBackground[y][x], 32.0f*x, 32.0f*y,32, 32);
				}
			}

			//Draw player1's bat
			DrawSprite(bat, player1_x, player1_y, BAT_WIDTH, BAT_HEIGHT);

			//Draw player2's bat
			DrawSprite(bat, player2_x, player2_y, BAT_WIDTH, BAT_HEIGHT);
		
			//Draw ball
			DrawSprite(ball, ball_x, ball_y, BALL_WIDTH, BALL_HEIGHT);

			//Draw Game Intro Text
			DrawString("Ping Pong", 250, 250, 6.0f);
			DrawString("Start Game > Press F1", 300, 350, 2.0f);
			DrawString("High Score > Press F2", 300, 400, 2.0f);

			break;

		case CLEANUP:
			cleanUp();
			break;

	}
}


/****************************************************************************/
// Function gameStart Sub-state
// parameters: sub state
// return type: void
// expected use: to begin the game play
/****************************************************************************/
void gameStart(SubState a_subState)
{
	timer = clock();

	isHighTimeScore = false;
	float posy = ((miny + maxy) / 2)*1.45f;	
	float posy2 = ((miny2 + maxy2) / 2)*1.45f;

	switch (a_subState)
	{
		case INIT:
			initTexture();

			//initialize players and ball position
			initialisePlayersBallPosition();

			break;

		case UPDATE:

			//startTimer = clock();

			//Left pressed move left
			//if (IsKeyDown(KEY_LEFT))
			//	Rot -= 10.0f;

			//Right pressed move right
			//if (IsKeyDown(KEY_RIGHT))
			//	Rot += 10.0f;
			
			if (miny!=700 && maxy!=0) {
				if (abs(player1_y - posy) > threshold) {
					player1_y = posy;
				}
			
				if(player1_y<32) {
					player1_y=32;
				} else if(player1_y> SCREEN_HEIGHT-BAT_HEIGHT-32) {
					player1_y= SCREEN_HEIGHT-BAT_HEIGHT-32;
				}
			}

			if (miny2!=700 && maxy2!=0) {
				if (abs(player2_y - posy2) > threshold) {
					player2_y = posy2;
				}
			
				if(player2_y<32) {
					player2_y=32;
				} else if(player2_y> SCREEN_HEIGHT-BAT_HEIGHT-32) {
					player2_y= SCREEN_HEIGHT-BAT_HEIGHT-32;
				}
			}
			//player2_y = ((miny2 + maxy2) / 2)*1.45f;
			//Player1 Key UP
			if (IsKeyDown(KEY_UP))
			{
				/*if (player1_y - 16 <= 0)
					player1_y = 0 + 16;
				else
					player1_y -= 5;*/
				
			}
		
			//Player1 Key Down
			if (IsKeyDown(KEY_DOWN))
			{
				/*if (player1_y + BAT_HEIGHT + 16 >= SCREEN_HEIGHT)
					player1_y = SCREEN_HEIGHT - BAT_HEIGHT - 16;
				else
					player1_y += 5;*/


				//collect the current time
				startTimer = timer;
				startPositionY = player1_y;
				//save current time to previous time for last time of keypressed
				//collect the player bat position
			}
			else
			{
				//collect the current time
				endTimer = timer;
				endPositionY = player1_y;

				//collect the player bat position
				//calculate velocity of bat -> (New position of bat - Previous position of bat) / (current time - last time)
			}

			//Player2 Key LShift (UP)
			/*if (IsKeyDown(KEY_F2))
			{
				//startTimer = clock();

				if (player2_y - 32 <= 0){
					player2_y = 0 + 32;}
				else{
					player2_y -= 5;}
			}*/


				//player2_y -= 5;

			//Player2 Key LCtrl (DOWN)
			/*if (IsKeyDown(KEY_F1))
			{
				//startTimer = clock();

				if (player2_y + BAT_HEIGHT + 16 >= SCREEN_HEIGHT){
					player2_y = SCREEN_HEIGHT - BAT_HEIGHT - 16;}
				else{
					player2_y += 5;}
			}*/


				//player2_y += 5;


			//Check if ball has moved and collided with the bat
			if ((ball_x >= player1_x + BAT_WIDTH) || 
				(ball_x + BALL_WIDTH <= player1_x) ||
				(ball_y >= player1_y + BAT_HEIGHT) ||
				(ball_y + BALL_HEIGHT <= player1_y))
			{
				// Do Nothing as Ball is not colliding with bat
			}
			else
			{
				ballspeed_x *= -1;		//Change direction x
				TimeScore ++;		//Increment Timer score by 1
			}

			if ((ball_x >= player2_x + BAT_WIDTH) || 
				(ball_x + BALL_WIDTH <= player2_x) ||
				(ball_y >= player2_y + BAT_HEIGHT) ||
				(ball_y + BALL_HEIGHT <= player2_y))
			{
				// Do Nothing as Ball is not colliding with bat
			}
			else
			{
				ballspeed_x *= -1;		//Change direction x
				TimeScore ++;		//Increment Timer score by 1
			}

			//	//float timeOfTravel = (endTimer - startTimer) / CLOCKS_PER_SEC;
			//	////float timeOfTravel = endTimer - startTimer;
			//	//ballspeed_y += (endPositionY - startPositionY) / timeOfTravel;
			//	//ballspeed_y = ballspeed_y / (ballspeed_y + 0.5f);


			//Check if ball has bounced on the wall. If so, bounce back in opposite y direction
			if ((ball_y + BALL_HEIGHT >= SCREEN_HEIGHT-16) || (ball_y < 0+16))
				ballspeed_y *= -1;		//Change direction


			//Check if ball has hit on Player 1's wall. If yes, Player2 gets ONE score
			if (ball_x + BALL_WIDTH >= SCREEN_WIDTH-16)
			{
				player2Score += 1;
				initialisePlayersBallPosition();
				g_NextGameState = SCORE;  //Change Game State to SCORE
			}

			//Check if ball has hit on Player 2's wall. If yes, Player1 gets ONE score
			if (ball_x <= 0+16)
			{
				player1Score += 1;
				initialisePlayersBallPosition();
				g_NextGameState = SCORE;  //Change Game State to SCORE
			}


			ball_x += ballspeed_x;		//Update ball position x
			ball_y += ballspeed_y;		//Update ball position y		
		
			//Game Loop Exit
			if (IsKeyDown(KEY_ESCAPE))
				g_NextGameState = MAIN_MENU;

			break;

		case DRAW:

			//Clear the screen, so previous frames don't build up
			ClearScreen();

			//Draw background
			for (int y=0; y < 24; y++)
			{
				for (int x=0; x < 32; x++)
				{
					DrawSprite(g_aBackground[y][x], 32.0f*x, 32.0f*y,32, 32);
				}
			}

			//Draw player1's bat
			DrawSprite(bat, player1_x, player1_y, BAT_WIDTH, BAT_HEIGHT);

			//Draw player2's bat
			DrawSprite(bat, player2_x, player2_y, BAT_WIDTH, BAT_HEIGHT);
		
			//Draw ball
			DrawSprite(ball, ball_x, ball_y, BALL_WIDTH, BALL_HEIGHT);

			//Draw player1 score
			char score[32];
			//DrawString("Player 1", 1024-200, 15, 2.0f);
			sprintf(score,"Player 1: %i", player1Score);
			DrawString(score, 1024-220, 35, 2.0f);

			//Draw player2 score
			sprintf(score,"Player 2: %i", player2Score);
			DrawString(score, 15, 35, 2.0f);

			//Draw Round Number
			sprintf(score,"Round %i", player1Score + player2Score + 1);
			DrawString(score, 475, 35, 2.0f);

			break;

		case CLEANUP:
			cleanUp();
			break;
	}
}


/****************************************************************************/
// Function scoreMenu sub-state
// parameters: sub state
// return type: void
// expected use: display the score status and determine if one of players 
// has won the game 
/****************************************************************************/
void scoreMenu(SubState a_subState)
{
	int scoreDisplayCount = 0;

	int a_tmpHighTimeScore[5];

	switch (a_subState)
	{
		case INIT:
			initTexture();

			//initialize players and ball position
			initialisePlayersBallPosition();

			break;

		case UPDATE:

			//Player1 Key UP
			if (IsKeyDown(KEY_SPACE) )
			{
				// If 6 rounds of the game has completed
				if ((player1Score + player2Score) >= 6)
				{
					//Set Point
					if (player1Score == player2Score)
					{
						//Set next state back to the GAME
						g_NextGameState = GAME;
					} 
					else
					{
						if (TimeScore > highTimeScore)
						{
							highTimeScore = TimeScore;
						}

						//Set next state to MAIN MENU
						g_NextGameState = MAIN_MENU;
					}
				} 
				else
				{
					//Increment Timer score by 1
					TimeScore ++;

					//Set next state back to the GAME
					g_NextGameState = GAME;
				}
			}

			break;

		case DRAW:

			//Clear the screen, so previous frames don't build up
			ClearScreen();

			//Draw background
			for (int y=0; y < 24; y++)
			{
				for (int x=0; x < 32; x++)
				{
					DrawSprite(g_aBackground[y][x], 32.0f*x, 32.0f*y,32, 32);
				}
			}

			//Draw player1's bat
			DrawSprite(bat, player1_x, player1_y, BAT_WIDTH, BAT_HEIGHT);

			//Draw player2's bat
			DrawSprite(bat, player2_x, player2_y, BAT_WIDTH, BAT_HEIGHT);
		
			//Draw ball
			DrawSprite(ball, ball_x, ball_y, BALL_WIDTH, BALL_HEIGHT);

			//Draw player1 score
			char score[32];
			//DrawString("Player 2", 1024-200, 15, 2.0f);
			sprintf(score,"Player 2: %i", player2Score);
			DrawString(score, 15, 35, 2.0f);

			//Draw player2 score
			//DrawString("Player 1: ", 15, 15, 2.0f);
			sprintf(score,"Player 1: %i", player1Score);
			DrawString(score, 1024-220, 35, 2.0f);

			for (int i=0; i<3; i++)
			{
				a_tmpHighTimeScore[i] = a_highTimeScore[i];
			}

			// if player 1 score add player 2 score is equal or more than 6
			if ((player1Score + player2Score) >= 6)
			{

				if (player1Score > player2Score)
				{
					//Display Winner
					DrawString("Player 1 WIN!!! ", 100, 100, 4.0f);

					sprintf(score,"Final Score: %i", TimeScore);
					DrawString(score, 200, 400, 4.0f);

					//Check the winner's score with the array of High Scores\

					for (int i=0; i<5; i++)
					{
						if (TimeScore > a_highTimeScore[i] && isHighTimeScore == false)
						{
							a_highTimeScore[i] = TimeScore;
							if (i + 1 < 5)
								a_highTimeScore[i+1] = a_tmpHighTimeScore[i];
							isHighTimeScore = true;							
						}
					}

					if (isHighTimeScore == true)
						DrawString("You beat the High score!! ", 150, 500, 3.0f);

					//Display Continue Message
					DrawString("Press SPACEBAR to Exit", 275, 600, 2.0f);

				 }
				 else if (player2Score > player1Score)
				 {
						DrawString("Player 2 WIN!!! ", 200, 300, 4.0f);

						sprintf(score,"Final Score: %i", TimeScore);
						DrawString(score, 200, 400, 4.0f);



						for (int i=0; i<5; i++)
						{
							if (TimeScore > a_highTimeScore[i] && isHighTimeScore == false)
							{
								a_highTimeScore[i] = TimeScore;
								if (i + 1 < 5)
									a_highTimeScore[i+1] = a_tmpHighTimeScore[i];
								isHighTimeScore = true;							
							}
						}

						if (isHighTimeScore == true)
							DrawString("You beat the High score!! ", 150, 500, 3.0f);

						//Display Continue Message
						DrawString("Press SPACEBAR to Exit", 275, 600, 2.0f);

					}
					else
					{
						//Display Round number
						sprintf(score,"Round %i", player1Score + player2Score);
						DrawString(score, 300, 75, 7.0f);

						//Display the score status for both players
						sprintf(score,"%i  :  %i", player2Score, player1Score);
						DrawString(score, 300, 200, 7.0f);

						//Display whoever win the 7th round wins
						DrawString("SET Point!", 300, 300, 4.0f);

						//Display Continue Message
						DrawString("Press SPACEBAR to Continue", 275, 600, 2.0f);
					 }
			}
			else
			{
				//Display Round number
				sprintf(score,"Round %i", player1Score + player2Score);
				DrawString(score, 300, 75, 7.0f);

				//Display the score status for both players
				sprintf(score,"%i  :  %i", player2Score, player1Score);
				DrawString(score, 300, 200, 7.0f);

			//Display Continue Message
			DrawString("Press SPACEBAR to Continue", 275, 600, 2.0f);
			}

			break;

		case CLEANUP:
			cleanUp();
			break;
	}
}

/****************************************************************************/
// Function scoreMenu sub-state
// parameters: sub state
// return type: void
// expected use: to display the high score
/****************************************************************************/
void highScore(SubState a_subState)
{
	int scoreDisplayCount = 0;



	switch (a_subState)
	{
		case INIT:
			initTexture();

			//initialize players and ball position
			initialisePlayersBallPosition();

			break;

		case UPDATE:

			//If Escape Key is pressed, Exit to Main Menu
			if (IsKeyDown(KEY_SPACE))
				//Set next state to MAIN MENU
				g_NextGameState = MAIN_MENU;
				
			break;

		case DRAW:

			//Clear the screen, so previous frames don't build up
			ClearScreen();

			//Draw background
			for (int y=0; y < 24; y++)
			{
				for (int x=0; x < 32; x++)
				{
					DrawSprite(g_aBackground[y][x], 32.0f*x, 32.0f*y,32, 32);
				}
			}

			//Draw player1's bat
			DrawSprite(bat, player1_x, player1_y, BAT_WIDTH, BAT_HEIGHT);

			//Draw player2's bat
			DrawSprite(bat, player2_x, player2_y, BAT_WIDTH, BAT_HEIGHT);

			//Draw ball
			DrawSprite(ball, ball_x, ball_y, BALL_WIDTH, BALL_HEIGHT);

			//Draw High score
			char highscore[32];
			//sprintf(highscore,"High Score: %i", highTimeScore);
			//DrawString(highscore, 300, 300, 4.0f);
			DrawString("Pong High Score", 275, 300, 4.0f);




			//for (int i=0; i<5; i++)
			//{
			//	sprintf(highscore,"%i", a_highTimeScore[i]);
			//	if (a_highTimeScore[i] > 0)
			//		DrawString(highscore, 350, 350+50*i, 4.0f);
			//	else
			//		DrawString("- N/A -", 350, 350+50*i, 4.0f);
			//}


			//Draw array of High Scores
			for (int i=0; i<5; i++)
			{
				sprintf(highscore,"%i", a_highTimeScore[i]);
				if (a_highTimeScore[i] > 0)
					DrawString(highscore, 500, 350+50*i, 4.0f);
				else
					DrawString(" ", 500, 350+50*i, 4.0f);
			}

			//for (int i=0; i<5; i++)
			//{
			//	if (TimeScore > a_highTimeScore[i])
			//		a_highTimeScore[i] = TimeScore;

			//	break;
			//}

			//Display EXIT Message
			DrawString("Press SPACEBAR to EXIT", 275, 600, 2.0f);

			break;


		case CLEANUP:
			cleanUp();
			break;
	}
}

/*****************************************************************/
// Function runGameState sub-state
// parameters: sub state
// return type: void
// expected use: to manage the flow of the game state
/*****************************************************************/
void runGameState(SubState a_sstate)
{
	switch (g_GameState)
	{
		case STARTUP:
			break;

		case MAIN_MENU:
			mainMenu(a_sstate);
			break;

		case GAME:
			gameStart(a_sstate);
			break;

		case SCORE:
			scoreMenu(a_sstate);
			break;

		case HIGHSCORE:
			highScore(a_sstate);
			break;

		case HOWTOPLAY:
		//howToPlay(a_sstate);
			break;

		case EXIT:
			break;

	}
}

/***************************************************************************/
// Function initialisePlayersBallPosition
// parameters: n/a
// return type: void
// expected use: to manage initialize Players position and Ball Position
/***************************************************************************/
void initialisePlayersBallPosition()
{
	//initialize position for player1 bat
	player1_x = 900;
	player1_y = 350;

	//initialize position for player2 bat
	player2_x = 50;
	player2_y = 350;

	// initialize ball position
	ball_x = 512;
	ball_y = 384;
}

void initTexture() {
	//Loads all Images, we store them in the integer variables
			top = LoadTexture("./images/Top.png");
			topLeft = LoadTexture("./images/TopLeft.png");
			topRight = LoadTexture("./images/TopRight.png");
			bottom = LoadTexture("./images/Bottom.png");
			bottomLeft = LoadTexture("./images/BottomLeft.png");
			bottomRight = LoadTexture("./images/BottomRight.png");
			leftT = LoadTexture("./images/Left.png");
			rightT = LoadTexture("./images/Right.png");
			center = LoadTexture("./images/Center.png");
			bat = LoadTexture("./images/grass.png");
			ball = LoadTexture("./images/ball.png");

			int width=32, height=21;

			for (int y=0; y <= height; y++)
			{
				for (int x=0; x < 32; x++)
				{
					if (y==0 && x==0)
						g_aBackground[y][x] = topLeft;

					else if (y==0 && x==31)
						g_aBackground[y][x] = topRight;

					else if (y==height && x==0)
						g_aBackground[y][x] = bottomLeft;

					else if (y==height && x==31)
						g_aBackground[y][x] = bottomRight;

					else if (y==0 && x>0 && x<31)
						g_aBackground[y][x] = top;

					else if (y==height && x>0 && x<31)
						g_aBackground[y][x] = bottom;

					else if (y>0 && y<height && x==0)
						g_aBackground[y][x] = leftT;

					else if (y>0 && y<height && x==31)
						g_aBackground[y][x] = rightT;

					else
						g_aBackground[y][x] = center;
				}
			}
}

void cleanUp() {
	FreeTexture(top);
	FreeTexture(topLeft);
	FreeTexture(topRight);
	FreeTexture(bottom);
	FreeTexture(bottomLeft);
	FreeTexture(bottomRight);
	FreeTexture(leftT);
	FreeTexture(rightT);
	FreeTexture(center);
	FreeTexture(ball);
	FreeTexture(bat);
}