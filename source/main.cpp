using namespace std;
#include <3ds.h>
#include <stdio.h>
#include <citro2d.h>
#include <citro3d.h>
#include <string>
#include <cmath>
#include "3ds-libs\include\audio\OggAudioPlayer.h"
#include "3ds-libs/include/Console.h"
//#include <cmath.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

// C:/Users/Wiz/Documents/CodingProjects/20GamesChallenge/Pong

// time for text rendering!! 😋

C2D_TextBuf StaticTextBuffer;
C2D_Text StaticText[2];
C2D_Font font;

static void textInit(){

	StaticTextBuffer = C2D_TextBufNew(4096);
	font = C2D_FontLoadSystem(CFG_REGION_USA);
}


static void textParse(int score1,int score2){

	 // convert the numbers to text 😋
	string str1 = to_string(score1);
	string str2 = to_string(score2);
	// convert the c++ string into c strings.. 🤒
	const char* text1 = str1.c_str();
	const char* text2 = str2.c_str();

	C2D_TextFontParse(&StaticText[0],font,StaticTextBuffer,text1);
	C2D_TextFontParse(&StaticText[1],font,StaticTextBuffer,text2);
	

	// optimize
	C2D_TextOptimize(&StaticText[0]);
	C2D_TextOptimize(&StaticText[1]);

}

static void renderText(){
	C2D_DrawText(&StaticText[0],0,(SCREEN_WIDTH / 2.0f) - (SCREEN_WIDTH / 4.0f),8.0f,0,1.5,1.5);
	C2D_DrawText(&StaticText[1],0,(SCREEN_WIDTH / 2.0f) + (SCREEN_WIDTH / 4.0f),8.0f,0,1.5,1.5);
}

static void freeText(){
	C2D_TextBufDelete(StaticTextBuffer);
	C2D_FontFree(font);
}

class Ball{
private:
u32 clrWhite = C2D_Color32f(1,1,1,1);
public:

int x,y;
int radius;
float speedX,speedY;
bool scored = false;
bool enemyHit = false;

void CheckCollision(int colX1,int colX2,int colY1,int colY2){
	// if ball gets hit by a paddle
	if( (x >= colX1) && (x <= colX2) && (y >= colY1) && (y <= colY2) ){

		// player paddle
		if (x < SCREEN_WIDTH / 2){
			speedX = abs(speedX);
		}
		else {
			speedX *= -1;
			enemyHit = true;
			}
		
		// if (speedX > 0) speedX += 0.03;
		// else speedX -= 0.03;
		// if (speedY > 0) speedY += 0.03;
		// else speedY -= 0.03;
		
		
	}
	//if(y <= colY1 && y >= colY2) speedY *= -1;

}

void Draw(){
	C2D_DrawCircleSolid(x,y,0,radius,clrWhite);
}

void UpdatePosition(){
	scored = false;
	enemyHit = false;
	x += speedX;
	y += speedY;
	if(y + radius >= SCREEN_HEIGHT || y  - radius<= 0) speedY *= -1;
	if(x + radius >= SCREEN_WIDTH || x - radius <= 0) {
		speedX *= -1;
		scored = true;
		}
	}

};

class Paddle{
protected:

	void CheckPosition(){
		if(y + height> SCREEN_HEIGHT || y < 0){
			y -= speedY;
		}
	}

	void getCollisionPoints() {
		colX1 = x - 15;
		colX2 = x + width;
		colY1 = y;
		colY2 = y + height;
	}

public:
	u32 clrWhite = C2D_Color32f(1,1,1,1);
	int colX1 = 0,colX2 = 0,colY1 = 0,colY2 = 0;
	int x,y;
	float speedY = 0;
	int height = 50;
	int width = 5;


	void Draw(){
		C2D_DrawRectangle(x,y,0,width,height,clrWhite,clrWhite,clrWhite,clrWhite);

	}

	void UpdatePosition(){
		y += speedY;
		CheckPosition();
		getCollisionPoints();
	}

};

class CpuPaddle: public Paddle{
	public:
	float speedDegredation = 0;
	void UpdatePosition(int ballPosition){
		if (ballPosition < y) {// if ball is higher
			speedY = -3.3 + speedDegredation;
		}
		else speedY = 3.3 - speedDegredation;
		y += speedY;
		CheckPosition();
		getCollisionPoints();
	}

};

class GameManager{
	public:


	// you know what bro pointers are actually not that bad 😋😋
	void Reset(Paddle* player,CpuPaddle* cpu,Ball* ball,int playerScore, int opScore){
		ball->x = SCREEN_WIDTH / 2;
		ball->y = SCREEN_HEIGHT / 2;
		ball->speedX = 3;
		ball->speedY = 3;
		ball->radius = 10;
				// create Player Paddle
		player->x = 15;
		player->y = (SCREEN_HEIGHT / 2) - (player->height / 2);
			// create CPU Paddle
		cpu->x = SCREEN_WIDTH - 15;
		cpu->y = (SCREEN_HEIGHT / 2) - cpu->height;
		cpu->speedDegredation = 0;
		textParse(playerScore,opScore);
	}


};



int main(int argc, char* argv[])
{
	romfsInit(); // loads the filesystem
	cfguInit(); // Load Text Font Loading System
	gfxInitDefault(); // loads graphics
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM,NULL);


	// Create Screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP,GFX_LEFT);

	// colors
	u32 clrClear = C2D_Color32f(0.83,0.45,0.35,1);

	int yourScore = 0;
	int opScore = 0;

	Console* console = new Console(GFX_TOP);
	
	//OggAudioPlayer *soundTest = new OggAudioPlayer("romfs:/sample.ogg");
	//soundTest->play();

	//Create Objects
	Ball ball;
	Paddle player;
	CpuPaddle cpu;
	GameManager gameManager;
	player.clrWhite = C2D_Color32f(0.35,0.55,0.89,1);

	u32 clrWhite = C2D_Color32f(0.94,0.9,0.89,1);
	u32 clrGreen = C2D_Color32f(0.54,0.65,0.35,1);
	u32 clrRed = C2D_Color32f(0.45,0.14,0.03,1);

	
	// init the text
	textInit();

	gameManager.Reset(&player,&cpu,&ball,yourScore,opScore);






	// Main loop
	while(aptMainLoop()){
		hidScanInput();
		u32 keyJustPressed = hidKeysDown();
		u32 keyPressed = hidKeysHeld();
		if (keyJustPressed & KEY_START) break;
		// printf("\x1b[1;1HYour Score = %i",yourScore);
		// printf("\x1b[2;1 \nOpponent Score = %i",opScore);

		// printf("\x1b[3;1 \nBall Speed = %f",ball.speedY);
		// printf("\x1b[3;1 \nOpponent Speed = %f",cpu.speedDegredation);



		// Render Scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top,clrClear);
		C2D_SceneBegin(top);

		// create middle line
		C2D_DrawLine(SCREEN_WIDTH / 2,0,clrWhite,SCREEN_WIDTH/2,SCREEN_HEIGHT,clrWhite,3,0);

		ball.UpdatePosition();
		ball.Draw();
		ball.CheckCollision(player.colX1,player.colX2,player.colY1,player.colY2);
		ball.CheckCollision(cpu.colX1,cpu.colX2,cpu.colY1,cpu.colY2);
		if (ball.scored){
			if (ball.x + ball.radius >= SCREEN_WIDTH ){
				yourScore += 1;
			}
			else opScore += 1;
			gameManager.Reset(&player,&cpu,&ball,yourScore,opScore);
		}

		if (keyPressed & KEY_UP){
			player.speedY = -3;
		}
		if (keyPressed & KEY_DOWN){
			player.speedY = 3;
		}
		player.UpdatePosition();
		player.Draw();
		player.speedY = 0;

		// slow down the enemy ball so the player can actually win 😟
		if(ball.enemyHit){
			cpu.speedDegredation += 0.025 + (0.025 * (rand() % 6));
		}

		cpu.UpdatePosition(ball.y);
		cpu.Draw();

		// aaaaand hi text!
		renderText();

		C3D_FrameEnd(0);
	}



	// Deinit libs
	freeText(); // kill text
	C2D_Fini();
	C3D_Fini();
	romfsExit(); // unload the filesystem
	cfguExit(); // i think kills text
	gfxExit();

	return 0;
}


