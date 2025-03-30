using namespace std;
#include <3ds.h>
#include <stdio.h>
#include <citro2d.h>
#include <citro3d.h>
#include <string>
#include <cmath>
#include "3ds-libs\include\audio\OggAudioPlayer.h"
#include "3ds-libs/include/Console.h"
//#include "3ds-libs/include/renderable/Sprite.h"

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

// YIPPIEE SPRITE RENDERING TIME!!! 🤯🤯🧪
#define MAX_SPRITES 768


typedef struct
{
	C2D_Sprite spr;
} Sprite;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[MAX_SPRITES];
static size_t numSprites = MAX_SPRITES/2;

static void initSprite(int spriteNum){
	size_t numImages = C2D_SpriteSheetCount(spriteSheet);
	Sprite* sprite = &sprites[spriteNum];
	//load sprite
	C2D_SpriteFromSheet(&sprite->spr,spriteSheet,spriteNum);
	C2D_SpriteSetCenter(&sprite->spr, 0.5f, 0.5f);
	C2D_SpriteSetPos(&sprite->spr,SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

}



class Ball{
public:

u32 clrWhite = C2D_Color32f(1,1,1,1);
u32 clrShadow = C2D_Color32f(0.38,0.03,0.45,0.1);
//const char* phsPath = "romfs:/paddle_hit.ogg";
//const char* whsPath = "romfs:/wall_hit.ogg";
//OggAudioPlayer* paddleHitSound = new OggAudioPlayer(const_cast<char*>(phsPath));
//OggAudioPlayer* wallHitSound = new OggAudioPlayer(const_cast<char*>(whsPath));

float x,y;
int radius;
float speedX,speedY;
float boost = 1;
bool scored = false;
bool enemyHit = false;
bool ballHit = false;

bool CheckCollision(int colX1,int colX2,int colY1,int colY2){
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
			boost = 1.5;
			ballHit = true;
			UpdatePosition();
			//paddleHitSound->play();
			return true;
	}
	return false;

}

void Draw(){
	float sizeX = radius*2+abs(((x - (SCREEN_WIDTH / 2)) / 20));
	float sizeY = radius*2+abs(((y - (SCREEN_HEIGHT / 2)) / 30));
	C2D_DrawEllipseSolid(x-(sizeX / 2) - ((x - (SCREEN_WIDTH / 2)) / 10),y-(sizeY / 2)-((y - (SCREEN_HEIGHT / 2)) / 10),0,sizeX,sizeY,clrShadow);
	C2D_DrawCircleSolid(x,y,0,radius,clrWhite);
}

void UpdatePosition(){
	scored = false;
	x += speedX * boost;
	y += speedY * boost;
	if(y + radius >= SCREEN_HEIGHT || y  - radius<= 0) {
		speedY *= -1;
		//wallHitSound->play();
	}
	if(x + radius >= SCREEN_WIDTH || x - radius <= 0) {
		//wallHitSound->play();
		speedX *= -1;
		scored = true;
		}
	
		 if(boost > 1){
		 	boost -= 0.015;
		 }
		 if(boost < 1){
			boost = 1;
		 }

		 clrWhite = C2D_Color32f(1,1  - (boost - 1),1,1);
	}

	int sign(float num){
		if(num >= 0) return 1;
		else return -1;
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
		if(x - (SCREEN_WIDTH / 2) <= 0) colX1 = x - 15;
		else colX1 = x;
		
		colX2 = x + width;
		colY1 = y;
		colY2 = y + height;
	}

public:
	u32 clrWhite = C2D_Color32f(1,1,1,1);
	u32 clrShadow = C2D_Color32f(0.38,0.03,0.45,0.1);
	int colX1 = 0,colX2 = 0,colY1 = 0,colY2 = 0;
	int x,y;
	float speedY = 0;
	int height = 50;
	int width = 5;


	void Draw(){
		C2D_DrawRectangle(x+3*sign(x - (SCREEN_WIDTH / 2)),y+3,0,width+2,height+2,clrShadow,clrShadow,clrShadow,clrShadow);
		C2D_DrawRectangle(x,y,0,width,height,clrWhite,clrWhite,clrWhite,clrWhite);

	}

	void UpdatePosition(){
		y += speedY;
		CheckPosition();
		getCollisionPoints();
	}

	int sign(float num){
		if(num >= 0) return 1;
		else return -1;
	}
};

class CpuPaddle: public Paddle{
	public:
	float speedDegredation = 0;

	enum IdleStates{
		STAY,
		MOVE_UP,
		MOVE_DOWN
	};
	
	IdleStates state = STAY;
	int stateTimer = 15;

	void UpdatePosition(int ballPosition,float ballSpeed){
		if (ballSpeed >0){
		if (ballPosition < y + (height / 2)) {// if ball is higher
			speedY = -3.6 + speedDegredation;
		}
		else speedY = 3.6 - speedDegredation;
		y += speedY;
		CheckPosition();
		getCollisionPoints();
	}
	else IdleMovement();
		


}
void IdleMovement(){
	//printf("\x1b[3;1 \n %i",state);
if(stateTimer > 0){
	if (state == MOVE_UP){
		speedY = -3.6 + speedDegredation;
		stateTimer -= 2;
	}
	if (state == MOVE_DOWN){
		speedY = 3.6 - speedDegredation;
		stateTimer -= 2;
	}
	y += speedY;
	CheckPosition();
	getCollisionPoints();
}
else{
	stateTimer = rand() % 30;
	if(rand() % 2 == 0) state = MOVE_UP;
	else if(rand() % 2 == 0) state = MOVE_DOWN;
	else state = STAY;
}
stateTimer -= 1;
}


};

class GameManager{
	public:


	// you know what bro pointers are actually not that bad 😋😋
	void Reset(Paddle* player,CpuPaddle* cpu,Ball* ball,int playerScore, int opScore){
		ball->x = SCREEN_WIDTH / 2;
		ball->y = SCREEN_HEIGHT / 2;
		if (rand() % 10 >= 5){
			ball->speedX = 3;
			ball->speedY = 3;
		}
		else{
			ball->speedX = -3;
		ball->speedY = -3;
		}
		
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
	ndspInit(); // loads audio
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM,NULL);


	// Create Screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP,GFX_LEFT);
	//C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM,GFX_LEFT);

	//init sprites
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfxx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	// colors
	u32 clrClear = C2D_Color32f(0.83,0.45,0.35,1);

	int yourScore = 0;
	int opScore = 0;
	bool gameStarted = false;

	//initSprite(1);


	//Console* console = new Console(GFX_TOP);
	
	//Create Objects
	Ball ball;
	Paddle player;
	CpuPaddle cpu;
	GameManager gameManager;
	player.clrWhite = C2D_Color32f(0.35,0.55,0.89,1);

	u32 clrWhite = C2D_Color32f(0.94,0.9,0.89,1);

	
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
		//printf("\x1b[3;1 \nis playing = %i, is loaded = %i",soundTest->isPlaying(),soundTest->isLoaded());

		// Render Scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top,clrClear);
		C2D_SceneBegin(top);

		//C2D_DrawSprite(&sprites[1].spr);

		// create middle line
		C2D_DrawLine(SCREEN_WIDTH / 2,0,clrWhite,SCREEN_WIDTH/2,SCREEN_HEIGHT,clrWhite,3,0);


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

		

		cpu.UpdatePosition(ball.y,ball.speedX);
		cpu.Draw();

		ball.UpdatePosition();
		ball.CheckCollision(player.colX1,player.colX2,player.colY1,player.colY2);
		ball.CheckCollision(cpu.colX1,cpu.colX2,cpu.colY1,cpu.colY2);
		ball.Draw();
		
		// slow down the enemy ball so the player can actually win 😟
		if(ball.enemyHit){
			cpu.speedDegredation += 0.025 + (0.025 * (rand() % 6));
			ball.enemyHit = false;
		}


		// aaaaand hi text!
		renderText();

		C3D_FrameEnd(0);


	}



	// Deinit libs
	//C2D_SpriteSheetFree(spriteSheet); // delete sprites
	freeText(); // kill text
	C2D_Fini();
	C3D_Fini();
	ndspExit(); // unload audio
	romfsExit(); // unload the filesystem
	cfguExit(); // i think kills text
	gfxExit();

	return 0;
}


