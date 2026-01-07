#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <cstdlib>

using namespace sf;
using namespace std;

const int SCREEN_WIDTH=1136;
const int SCREEN_HEIGHT=896;







int getEnemySpeed(int enemyType){
    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;

    const int GHOST_SPEED=2;
    const int SKELETON_SPEED=2;
    const int INVISIBLE_SPEED=2;
    const int CHELNOV_SPEED=2;

    if(enemyType==TYPE_GHOST)return GHOST_SPEED;
    if(enemyType==TYPE_SKELETON)return SKELETON_SPEED;
    if(enemyType==TYPE_INVISIBLE)return INVISIBLE_SPEED;
    if(enemyType==TYPE_CHELNOV)return CHELNOV_SPEED;
    return 2;
}

int getEnemyWidth(int enemyType){
    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;

    const int GHOST_WIDTH =144;
    const int SKELETON_WIDTH =96;
    const int INVISIBLE_WIDTH =96;
    const int CHELNOV_WIDTH =96;
    const int MINION_WIDTH =96;

    if(enemyType==TYPE_GHOST)return GHOST_WIDTH;
    if(enemyType==TYPE_SKELETON)return SKELETON_WIDTH;
    if(enemyType==TYPE_INVISIBLE)return INVISIBLE_WIDTH;
    if(enemyType==TYPE_CHELNOV)return CHELNOV_WIDTH;
    if(enemyType==TYPE_MINION)return MINION_WIDTH;  
    return 96;  
}

int getEnemyHeight(int enemyType){
    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;

    const int GHOST_HEIGHT=128;
    const int SKELETON_HEIGHT =144;
    const int INVISIBLE_HEIGHT =144;
    const int CHELNOV_HEIGHT =111;
    const int MINION_HEIGHT =128;

    if(enemyType==TYPE_GHOST)return GHOST_HEIGHT;
    if(enemyType==TYPE_SKELETON)return SKELETON_HEIGHT;
    if(enemyType==TYPE_INVISIBLE)return INVISIBLE_HEIGHT;
    if(enemyType==TYPE_CHELNOV)return CHELNOV_HEIGHT;
    if(enemyType==TYPE_MINION)return MINION_HEIGHT;  
    return 144; 
}

int getEnemyCapturePoints(int enemyType){
    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;

    const int GHOST_CAPTURE_POINTS=50;
    const int SKELETON_CAPTURE_POINTS=75;
    const int INVISIBLE_CAPTURE_POINTS=150;
    const int CHELNOV_CAPTURE_POINTS=200;
    const int PROJECTILE_DEFEAT_MULTIPLIER=2;

    if(enemyType==TYPE_GHOST)return GHOST_CAPTURE_POINTS;
    if(enemyType==TYPE_SKELETON)return SKELETON_CAPTURE_POINTS;
    if(enemyType==TYPE_INVISIBLE)return INVISIBLE_CAPTURE_POINTS;
    if(enemyType==TYPE_CHELNOV)return CHELNOV_CAPTURE_POINTS;
    return 50;
}



void initialize2DArray(int**& array, int& capacity, int cols){
    //gets pointer and assign it pointer of capacity rows and cols columns
    array=new int*[capacity];
    for(int i=0;i<capacity;i++){
        array[i]=new int[cols];
    }
}

void increaseSize(int**& array, int& capacity, int cols){
    //increase 1 size(row+1)
    int newCapacity=capacity+1;
    int** newArray=new int*[newCapacity];
    
    //copy previous data
    for(int i=0;i<capacity;i++){
        newArray[i]=array[i];
    }
    
    //add row
    newArray[capacity]=new int[cols];    
    //delete previous allocation
    delete[] array;
     
    //asign new pointers
    array=newArray;
    capacity=newCapacity;
}

void decreaseSize(int**& array, int& capacity, int cols){
    //decrease 1 row in array
    
    capacity=capacity-1;
    int** newArray=new int*[capacity];
    
    for(int i=0;i<capacity;i++){
        newArray[i]=array[i];
    }
    
    delete[] array[capacity];    
    delete[] array;
    
    array=newArray;
}

void cleanup2DArray(int**& array, int capacity){
    for(int i=0;i<capacity;i++){
        delete[] array[i];
    }
    delete[] array;
    array=nullptr;
}


void clearLevel(char** lvl){
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    for(int i=0;i<LEVEL_HEIGHT;i++){
        for(int j=0;j<LEVEL_WIDTH;j++){
            lvl[i][j]=' ';
        }
    }
}

void createLevel1Layout(char** lvl){
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    clearLevel(lvl);
    
    //left, right, bottom
    for(int j=0;j<LEVEL_WIDTH;j++){
        lvl[LEVEL_HEIGHT-1][j]='#';
    }
    for(int i=0;i<LEVEL_HEIGHT;i++){
        lvl[i][0]='#';
        lvl[i][LEVEL_WIDTH-1]='#';
    }
    
    //platforms
    for(int j=0;j<LEVEL_WIDTH;j++){
        if(j<=3||j>=LEVEL_WIDTH-4||(j>=7&&j<=LEVEL_WIDTH-8)){
            lvl[10][j]='#';
        }
    }    
    for(int j=0;j<LEVEL_WIDTH;j++){
        if(j<=5||j>=LEVEL_WIDTH-6){
            lvl[7][j]='#';
        }
    }    
    for(int j=0;j<LEVEL_WIDTH;j++){
        if((j>=3&&j<=7)||(j>=10&&j<=LEVEL_WIDTH-4)){
            lvl[4][j]='#';
        }
    }
}

bool isNearSlantedPlatform(char** lvl, int x, int y, int range){
    //check that if in square of range around block is slatntes
    for(int dy=-range;dy<=range;dy++){
        for(int dx=-range;dx<=range;dx++){
            int checkY=y+dy;
            int checkX=x+dx;
            
            if(checkY>=0&&checkY<14&&checkX>=0&&checkX<18){
                if(lvl[checkY][checkX]=='/'||lvl[checkY][checkX]=='\\'){
                    return true;
                }
            }
        }
    }
    return false;
}


void createLevel2Layout(char** lvl){
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    clearLevel(lvl);
    
    //create down, left, right
    for(int j=0;j<LEVEL_WIDTH;j++){
        lvl[LEVEL_HEIGHT-1][j]='#';
    }
    for(int i=0;i<LEVEL_HEIGHT;i++){
        lvl[i][0]='#';
        lvl[i][LEVEL_WIDTH-1]='#';
    }
    
    //start from 4 to 7 above
    int startY=4+(rand()%3);
    
    //either go from left to right down or oopposite
    bool startFromLeft=(rand()%2==0);
    int startX;    
    //select starting point on x cord
    if(startFromLeft){
        startX=3+(rand()%5); 
    }
    else {
        startX=(LEVEL_WIDTH-4)- rand()% 5; 
    }
    
    
    //make the slanted platform
    int currentX=startX;
    int currentY=startY;    
    for(int i=0; 1;i++){
        if(currentY>=LEVEL_HEIGHT-3)break; //hit bottom
        if(currentX<=2||currentX>=LEVEL_WIDTH-3)break; //hit side border
        
        lvl[currentY][currentX]=startFromLeft ?'\\':'/';
        lvl[currentY][currentX-1]=startFromLeft ?'\\':'/';
        
        //move diagonally
        currentY++; 
        currentX+=startFromLeft ?1:-1; 
    }
    

    //generate other platform (horizontal)
    for(int y=4; y<LEVEL_HEIGHT-3; y+=3){
        int x=1;
        
        while(x<LEVEL_WIDTH-1){
            //check if current position is near a slanted platform
            if(isNearSlantedPlatform(lvl, x, y, 2)){
                //if near slant skipp the iteration
                x++;
                continue;
            }
            
            int platformStart=x;
            int platformWidth=0;
            int maxWidth=3+(rand()%4);
            while(x<LEVEL_WIDTH-1&&!isNearSlantedPlatform(lvl, x, y, 2)){
                platformWidth++;
                x++;
                if(platformWidth>=maxWidth){
                    x+=2;
                    break;
                }
            }
            
            //only place platform if it's at least 3 blocks wide
            if(platformWidth>=3){
                for(int k=platformStart;k<platformStart+platformWidth;k++){
                    lvl[y][k]='#';
                }
            }
        }
    }
}


void createLevel3Layout(char** lvl){
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    clearLevel(lvl);
    
    //down, r and l
    for(int j=0;j<LEVEL_WIDTH;j++){
        lvl[LEVEL_HEIGHT-1][j]='#';
    }
    for(int i=0;i<LEVEL_HEIGHT;i++){
        lvl[i][0]='#';
        lvl[i][LEVEL_WIDTH-1]='#';
    }
    
    //platform layout 
    for(int j=0;j<LEVEL_WIDTH;j++){
        if(j<=6||j>=LEVEL_WIDTH-7 ){
            lvl[4][j]='#';
            lvl[7][j]='#';
            lvl[10][j]='#';
        }
    }
}

void initializeBossLevel(int& bossX, int& bossY, int& bossHealth, bool& bossAngry, int**& tentacles, int& tentacleCount, int& tentacleCapacity, int**& minions, int& minionCount, int& minionCapacity, int& bossHDirection, int& bossVDirection, int& bossAtTopTimer){
    const int BOSS_WIDTH=384;
    const int BOSS_HEIGHT=384;
    const int BOSS_TOP_BORDER=SCREEN_HEIGHT/2;
    const int BOSS_BOTTOM_BORDER=SCREEN_HEIGHT-BOSS_HEIGHT/3 ;
    const int BOSS_MOVING_RIGHT=1;
    const int BOSS_MOVING_UP=2;

    //boss starts at bottom-center
    bossX=SCREEN_WIDTH/2-BOSS_WIDTH/2;
    bossY=BOSS_BOTTOM_BORDER; //start at bottom
    bossHealth=5;
    bossAngry=false;    
    //initialize movement state
    bossHDirection=BOSS_MOVING_RIGHT;
    bossVDirection=BOSS_MOVING_UP;
    bossAtTopTimer=0;    
}


void initializePlayer(float& x, float& y, float& vx, float& vy,  bool& onGround, bool& isJumping, int& health,int character, float& playerSpeed, float& vacuumRange, int& score){
    x=200;
    y=600;
    vx=0;
    vy=0;
    onGround=false;
    isJumping=false;
    health=3;
    score=0;    
}


void playerDie(float& playerX, float& playerY, int& playerHealth, bool& invulnerable, int& invulnerableTimer, int& score, RenderWindow& window, int currentLevel, Font& font){

    playerHealth--;
    invulnerable=true;
    invulnerableTimer=0;
    
    playerX=200;
    playerY=600;
    score-=50;
    
    if(playerHealth<=0){
        score-=200;
    }
}


bool isGroundBelow(float x, float y, char** lvl){
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;

    int checkY=((int)(y+PLAYER_HEIGHT+1))/CELL_SIZE;
    //int leftX=((int)x)/CELL_SIZE;
    int midX=((int)(x+PLAYER_WIDTH/2))/CELL_SIZE;
    //int rightX=((int)(x+PLAYER_WIDTH)-1)/CELL_SIZE;
    
    if(checkY>=LEVEL_HEIGHT)return true;
    
    return (lvl[checkY][midX]=='#'||lvl[checkY][midX]=='/'|| lvl[checkY][midX]=='\\');
}

void applyGravity(float& y, float& vy, bool& onGround, char** lvl, float x){
    const float GRAVITY=1;
    const float TERMINAL_VELOCITY=20;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const int CELL_SIZE=64;

    if(!onGround){
        vy+=GRAVITY;
        if(vy>TERMINAL_VELOCITY){
            vy=TERMINAL_VELOCITY;
        }
        y+=vy;
        
        if(isGroundBelow(x, y, lvl)){
            onGround=true;
            vy=0;

            int platformY=((int)(y+PLAYER_HEIGHT))/CELL_SIZE;
            y=platformY*CELL_SIZE-PLAYER_HEIGHT;
        }
    }
    else{
        if(!isGroundBelow(x, y, lvl)){
            onGround=false;
        }
    }
}

void handleJump(float& vy, bool& isJumping, bool& onGround, float& y){
    const float JUMP_STRENGTH=-20;
    const float GRAVITY=1;
    if(Keyboard::isKeyPressed(Keyboard::Up)&&onGround&&!isJumping){
        vy=JUMP_STRENGTH;
        isJumping=true;
        onGround=false;
    }
    
    if(isJumping){
        vy+=GRAVITY;
        y+=vy;
        
        if(vy>=0){
            isJumping=false;
        }
    }
}

void handleSlantedPlatformSlide(float& playerX, float& playerY, char** lvl, float& slideVelocity){
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;

    int playerCellX=(playerX+PLAYER_WIDTH/2)/CELL_SIZE;
    int playerCellY=(playerY+PLAYER_HEIGHT+1)/CELL_SIZE;
    char platformType=lvl[playerCellY][playerCellX];
    
    if(platformType=='/'){
        //add x and y both for diagnoal movement
        playerX-=slideVelocity;
        playerY+=slideVelocity; 
        
    }
    else if(platformType=='\\'){

        playerX+=slideVelocity;
        playerY+=slideVelocity;
        
    }
    
}

bool canMoveRight(float x, float y, char** lvl){
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const float INITIAL_PLAYER_SPEED=5;

    int checkX=((int)(x+PLAYER_WIDTH+INITIAL_PLAYER_SPEED))/CELL_SIZE;
    //int topY=((int)y)/CELL_SIZE;
    int midY=((int)(y+PLAYER_HEIGHT/2))/CELL_SIZE;
    //int botY=((int)(y+PLAYER_HEIGHT))/CELL_SIZE;
    
    //if(topY<0)topY=0;
    if(midY>=LEVEL_HEIGHT)midY=LEVEL_HEIGHT-1;
    //if(botY>=LEVEL_HEIGHT)botY=LEVEL_HEIGHT-1;
    if(checkX>=LEVEL_WIDTH)return false;
    
    return (/*//lvl[topY][checkX] != '#'&&lvl[botY][checkX] != '#' &&*/ lvl[midY][checkX] != '#' &&lvl[midY][checkX] != '\\' &&lvl[midY][checkX] != '/' );
}

bool canMoveLeft(float x, float y, char** lvl){
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int PLAYER_HEIGHT=144;
    const float INITIAL_PLAYER_SPEED=5;

    int checkX=((int)(x-INITIAL_PLAYER_SPEED))/CELL_SIZE;
    //int topY=((int)y)/CELL_SIZE;
    int midY=((int)(y+PLAYER_HEIGHT/2))/CELL_SIZE;
    //int botY=((int)(y+PLAYER_HEIGHT))/CELL_SIZE;
    
    //if(topY<0)topY=0;
    if(midY>=LEVEL_HEIGHT)midY=LEVEL_HEIGHT-1;
    //if(botY>=LEVEL_HEIGHT)botY=LEVEL_HEIGHT-1;
    if(checkX<0)return false;
    
    return (/*lvl[topY][checkX] != '#'&&lvl[botY][checkX] != '#'&&*/lvl[midY][checkX] != '#' &&lvl[midY][checkX] != '\\' &&lvl[midY][checkX] != '/' );
}



float getPlayerSpeed(float baseSpeed, int activePowerups[3], int character){
    const float GREEN_PLAYER_SPEED_MULTI=1.5f;
    float speed=baseSpeed;
    
    if(character==0){ //green
        speed*=1.5;
    }
    
    if(activePowerups[0]>0){
        speed*=2.0f;
    }
    
    return speed;
}

float getVacuumRange(int activePowerups[3], int selectedCharacter){
    const float YELLOW_PLAYER_VACCUM_MULTI=1.2f;
    const float INITITAL_VACUUM_RANGE=100;
    float range=INITITAL_VACUUM_RANGE;
    
    if(selectedCharacter==1){ 
        range*=YELLOW_PLAYER_VACCUM_MULTI;
    }
    
    if(activePowerups[2]>0){
        range*=1.5f;
    }
    
    return range;
}

float getVacuumAngle(int activePowerups[3], int selectedCharacter){

    const float YELLOW_PLAYER_VACCUM_MULTI=1.2f;
    const float INITITAL_VACUUM_ANGLE=26.57*3.14/180;
    float angle=INITITAL_VACUUM_ANGLE;

    if(selectedCharacter==1){
        angle*=YELLOW_PLAYER_VACCUM_MULTI;
    }
    
    if(activePowerups[1]>0){
        angle*=1.5f;
    }
    
    return angle;
}

void updatePlayerMovement(float& x, float& y, float playerSpeed, char** lvl, bool& movingLeft, bool& movingRight, int selectedCharacter, int* activePowerups){
    const float INITIAL_PLAYER_SPEED=5;
    movingLeft=false;
    movingRight=false;
    
    if(Keyboard::isKeyPressed(Keyboard::Right)){
        movingRight=true;
        if(canMoveRight(x, y, lvl)){
            x+=getPlayerSpeed(INITIAL_PLAYER_SPEED, activePowerups, selectedCharacter);;
        }
    }
    
    if(Keyboard::isKeyPressed(Keyboard::Left)){
        movingLeft=true;
        if(canMoveLeft(x, y, lvl)){
            x-=getPlayerSpeed(INITIAL_PLAYER_SPEED, activePowerups, selectedCharacter);;
        }
    }
}



void updatePlayerAnimation(Sprite& playerSprite, Texture& texRight, Texture& texLeft, bool movingRight, bool movingLeft, bool isJumping, float vy, bool onGround, int offHeight){
    static bool facingRight=false;
    static int walkFrame=0;
    static int frameCounter=0;
    
    frameCounter++;
    
    //walk animation
    if(frameCounter>9){
        frameCounter=0;
        walkFrame++;
        if(walkFrame>3)walkFrame=0;
    }
    
    if(movingRight)facingRight=true;
    if(movingLeft)facingRight=false;
    
    //jumping animation
    if(!onGround){
        if(facingRight){
            playerSprite.setTexture(texRight);
            if(vy>=18){
                playerSprite.setTextureRect(IntRect(733, 33+offHeight, 32, 48));
            }
            else {
                playerSprite.setTextureRect(IntRect(766, 33+offHeight, 32, 48));
            }
        }
        else {
            playerSprite.setTexture(texLeft);
            if(vy>=18){
                playerSprite.setTextureRect(IntRect(354, 33+offHeight, 32, 48));
            }
            else {
                playerSprite.setTextureRect(IntRect(387, 33+offHeight, 32, 48));
            }
        }
    }
    //walking animation
    else if(movingRight||movingLeft){
        if(movingRight){
            playerSprite.setTexture(texRight);
            playerSprite.setTextureRect(IntRect(1069-walkFrame*33, 33+offHeight, 32, 48));
        }
        else {
            playerSprite.setTexture(texLeft);
            playerSprite.setTextureRect(IntRect(walkFrame*33+51, 33+offHeight, 32, 48));
        }
    }
    //idle animation
    else {
        if(facingRight){
            playerSprite.setTexture(texRight);
            playerSprite.setTextureRect(IntRect(1104, 33+offHeight, 32, 48));
        }
        else {
            playerSprite.setTexture(texLeft);
            playerSprite.setTextureRect(IntRect(16, 33+offHeight, 32, 48));
        }
    }
}





void updateEnemyAnimation(int i, int**& enemies, int enemyCount, Sprite& enemySprite, Texture enemyTextures[]){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;


    const int TYPE_GHOSTL=0;
    const int TYPE_GHOSTR=4;
    const int TYPE_SKELETONL=1;
    const int TYPE_SKELETONR=5;
    const int TYPE_INVISIBLEL= 2;
    const int TYPE_INVISIBLER= 6;
    const int TYPE_CHELNOVL= 3;
    const int TYPE_CHELNOVR= 7;


    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;

    const int STATE_ACTIVE=0;
    const int STATE_STUNNED=2;
    const int STATE_INACTIVE=3;



    if(enemies[i][ENEMIES_STATE]==STATE_ACTIVE||enemies[i][ENEMIES_STATE]==STATE_STUNNED){
        if(enemies[i][ENEMIES_TYPE]==TYPE_INVISIBLE&&enemies[i][ENEMIES_VISIBLE]==0){
            //skip walking frame of invis man
            if(enemies[i][ENEMIES_FRAME]==1)return;  //skip walking when invisible
        }

        
        int enemyType=enemies[i][ENEMIES_TYPE];
        
        //increment current enemyies frame counter
        enemies[i][ENEMIES_TOTALFRAMES]++;
        
        //gHOST ANIMATION
        //left
        if(enemyType==TYPE_GHOST){
            if(enemies[i][ENEMIES_DIR_X]==-1){
                enemySprite.setTexture(enemyTextures[TYPE_GHOSTL]);
            
                if(enemies[i][ENEMIES_FRAME]==1){ //walking
                    //update frame every 10 ticks
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]--;  
                        if(enemies[i][ENEMIES_CURRENT]<0)enemies[i][ENEMIES_CURRENT]=3;
                    }
                    enemySprite.setTextureRect(IntRect(148-enemies[i][ENEMIES_CURRENT]*49, 8, 48, 32));
                } 
                else if(enemies[i][ENEMIES_FRAME]==2){ //waiting
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;  
                        if(enemies[i][ENEMIES_CURRENT]>1)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    enemySprite.setTextureRect(IntRect(214+enemies[i][ENEMIES_CURRENT]*49, 8, 48, 32));
                }
            } else {
                enemySprite.setTexture(enemyTextures[TYPE_GHOSTR]);
            
                if(enemies[i][ENEMIES_FRAME]==1){ //walking
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]--; 
                        if(enemies[i][ENEMIES_CURRENT]<0)enemies[i][ENEMIES_CURRENT]=3;
                    }
                    enemySprite.setTextureRect(IntRect(1158-enemies[i][ENEMIES_CURRENT]*49, 8, 48, 32));
                } 
                else if(enemies[i][ENEMIES_FRAME]==2){ //waiting
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;  
                        if(enemies[i][ENEMIES_CURRENT]>1)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    enemySprite.setTextureRect(IntRect(902+enemies[i][ENEMIES_CURRENT]*49, 8, 48, 32));
                }
            }
        }
        
        //sKELETON ANIMATION
        else if(enemyType==TYPE_SKELETON){
            enemies[i][ENEMIES_TOTALFRAMES]++;
            if(enemies[i][ENEMIES_DIR_X]==-1){
                enemySprite.setTexture(enemyTextures[TYPE_SKELETONL]);
            
                if(enemies[i][ENEMIES_FRAME]==1){ //walking
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>3)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    enemySprite.setTextureRect(IntRect(290-enemies[i][ENEMIES_CURRENT]*33, 24, 32, 48));
                }
                else if(enemies[i][ENEMIES_FRAME]==2){ //jumping
                    int jumpFrameL=0;
                    
                    if(enemies[i][ENEMIES_DIR_Y]==1){
                        if(enemies[i][ENEMIES_Y]>enemies[i][ENEMIES_TARGET_Y]+50){
                            jumpFrameL=0;
                        }
                        else if(enemies[i][ENEMIES_Y]>enemies[i][ENEMIES_TARGET_Y]+10){
                            jumpFrameL=2;
                        }
                        else {
                            jumpFrameL=1;
                        }
                    }
                    else {
                        jumpFrameL=2;
                    }
                    
                    if(jumpFrameL==0){
                        enemySprite.setTextureRect(IntRect(536, 24, 32, 48));
                    }
                    else if(jumpFrameL==1){
                        enemySprite.setTextureRect(IntRect(536+36, 24, 32, 48));
                    }
                    else {
                        enemySprite.setTextureRect(IntRect(536+72, 24, 32, 48));
                    }
                }
                else if(enemies[i][ENEMIES_FRAME]==3){ //waiting/Idle
                    //simple back and forth: 2->1->0->1->2
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        
                        //use ENEMIES_FLAG5 as direction flag
                        if(enemies[i][ENEMIES_FLAG5]==0){ //going down
                            enemies[i][ENEMIES_CURRENT]--;
                            if(enemies[i][ENEMIES_CURRENT]<=0){
                                enemies[i][ENEMIES_CURRENT]=0;
                                enemies[i][ENEMIES_FLAG5]=1; //start going up
                            }
                        } else { //going up
                            enemies[i][ENEMIES_CURRENT]++;
                            if(enemies[i][ENEMIES_CURRENT]>=2){
                                enemies[i][ENEMIES_CURRENT]=2;
                                enemies[i][ENEMIES_FLAG5]=0; //start going down
                            }
                        }
                    }
                    
                    if(enemies[i][ENEMIES_CURRENT]==0)
                        enemySprite.setTextureRect(IntRect(148, 24, 32, 48));
                    else if(enemies[i][ENEMIES_CURRENT]==1)
                        enemySprite.setTextureRect(IntRect(98, 24, 48, 48));
                    else
                        enemySprite.setTextureRect(IntRect(48, 24, 48, 48));
                }
                else if(enemies[i][ENEMIES_FRAME]==4){ //drop climb animation
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=20){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>10)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    
                    if(enemies[i][ENEMIES_CURRENT]==0){
                        enemySprite.setTextureRect(IntRect(644, 24, 48, 48));
                    }
                    else if(enemies[i][ENEMIES_CURRENT]==1){
                        enemySprite.setTextureRect(IntRect(694, 24, 32, 48));
                    }
                    else {
                        enemySprite.setTextureRect(IntRect(728, 8, 32, 64));
                    }
                }
            } else {
                enemySprite.setTexture(enemyTextures[TYPE_SKELETONR]);
            
                if(enemies[i][ENEMIES_FRAME]==1){ //walking
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>3)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    enemySprite.setTextureRect(IntRect(1138-enemies[i][ENEMIES_CURRENT]*33, 24, 32, 48));
                }
                else if(enemies[i][ENEMIES_FRAME]==2){ //jumping
                    int jumpFrameR=0;
                    
                    if(enemies[i][ENEMIES_DIR_Y]==1){
                        if(enemies[i][ENEMIES_Y]>enemies[i][ENEMIES_TARGET_Y]+50){
                            jumpFrameR=0;
                        }
                        else if(enemies[i][ENEMIES_Y]>enemies[i][ENEMIES_TARGET_Y]+10){
                            jumpFrameR=2;
                        }
                        else {
                            jumpFrameR=1;
                        }
                    }
                    else {
                        jumpFrameR=2;
                    }
                    
                    if(jumpFrameR==0){
                        enemySprite.setTextureRect(IntRect(790, 24, 32, 48));
                    }
                    else if(jumpFrameR==1){
                        enemySprite.setTextureRect(IntRect(756, 24, 32, 48));
                    }
                    else {
                        enemySprite.setTextureRect(IntRect(722, 24, 32, 48));
                    }
                }
                else if(enemies[i][ENEMIES_FRAME]==3){ //waiting/Idle
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        
                        if(enemies[i][ENEMIES_FLAG5]==0){
                            enemies[i][ENEMIES_CURRENT]--;
                            if(enemies[i][ENEMIES_CURRENT]<=0){
                                enemies[i][ENEMIES_CURRENT]=0;
                                enemies[i][ENEMIES_FLAG5]=1;
                            }
                        } else {
                            enemies[i][ENEMIES_CURRENT]++;
                            if(enemies[i][ENEMIES_CURRENT]>=2){
                                enemies[i][ENEMIES_CURRENT]=2;
                                enemies[i][ENEMIES_FLAG5]=0;
                            }
                        }
                    }
                    
                    if(enemies[i][ENEMIES_CURRENT]==0)
                        enemySprite.setTextureRect(IntRect(1178, 24, 32, 48));
                    else if(enemies[i][ENEMIES_CURRENT]==1)
                        enemySprite.setTextureRect(IntRect(1212, 24, 48, 48));
                    else
                        enemySprite.setTextureRect(IntRect(1262, 24, 48, 48));
                }
                else if(enemies[i][ENEMIES_FRAME]==4){ //drop/Climb
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=6){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>2)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    
                    if(enemies[i][ENEMIES_CURRENT]==0){
                        enemySprite.setTextureRect(IntRect(666, 24, 48, 48));
                    }
                    else if(enemies[i][ENEMIES_CURRENT]==1){
                        enemySprite.setTextureRect(IntRect(632, 24, 32, 48));
                    }
                    else {
                        enemySprite.setTextureRect(IntRect(598, 8, 32, 64));
                    }
                }
                else if(enemies[i][ENEMIES_FRAME]==5){ //fire
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=6){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>2)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    
                    if(enemies[i][ENEMIES_CURRENT]==0){
                        enemySprite.setTextureRect(IntRect(666, 24, 48, 48));
                    }
                    else if(enemies[i][ENEMIES_CURRENT]==1){
                        enemySprite.setTextureRect(IntRect(632, 24, 32, 48));
                    }
                    else {
                        enemySprite.setTextureRect(IntRect(598, 8, 32, 64));
                    }
                }
            }
        }
        
        //invisman animation
        else if(enemyType==TYPE_INVISIBLE){
            if(enemies[i][ENEMIES_DIR_X]==-1){
                enemySprite.setTexture(enemyTextures[TYPE_INVISIBLEL]);
            
                if(enemies[i][ENEMIES_FRAME]==1){ //walking
                    //update frame every 10 ticks
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]--;  
                        if(enemies[i][ENEMIES_CURRENT]<0)enemies[i][ENEMIES_CURRENT]=3;
                    }
                    enemySprite.setTextureRect(IntRect(258-enemies[i][ENEMIES_CURRENT]*34, 8, 32, 48));
                } 
                else if(enemies[i][ENEMIES_FRAME]==2){ //disappear
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=20){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;  
                        if(enemies[i][ENEMIES_CURRENT]>5)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    enemySprite.setTextureRect(IntRect(298+enemies[i][ENEMIES_CURRENT]* 34, 8, 32, 48));
                }
                else if(enemies[i][ENEMIES_FRAME]==3){ //appear
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=20){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]--;  
                        if(enemies[i][ENEMIES_CURRENT]<0)enemies[i][ENEMIES_CURRENT]=5;
                    }
                    enemySprite.setTextureRect(IntRect(468-enemies[i][ENEMIES_CURRENT]* 34, 8, 32, 48));
                }
            } else {
                enemySprite.setTexture(enemyTextures[TYPE_INVISIBLER]);
            
                if(enemies[i][ENEMIES_FRAME]==1){ //walking
                    //update frame every 10 ticks
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;  
                        if(enemies[i][ENEMIES_CURRENT]>3)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    enemySprite.setTextureRect(IntRect(800+enemies[i][ENEMIES_CURRENT]*34, 8, 32, 48));
                } 
                else if(enemies[i][ENEMIES_FRAME]==2){ //disappear
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=20){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]--;  
                        if(enemies[i][ENEMIES_CURRENT] <0)enemies[i][ENEMIES_CURRENT]=5;
                    }
                    enemySprite.setTextureRect(IntRect(760-enemies[i][ENEMIES_CURRENT]* 34, 8, 32, 48));
                }
                else if(enemies[i][ENEMIES_FRAME]==3){ //appear
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=20){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;  
                        if(enemies[i][ENEMIES_CURRENT] >5)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    enemySprite.setTextureRect(IntRect(590+enemies[i][ENEMIES_CURRENT]* 34, 8, 32, 48));
                }
            }
        }
        
        //cHELNOV ANIMATION
        else if(enemyType==TYPE_CHELNOV){
            enemies[i][ENEMIES_TOTALFRAMES]++;
            if(enemies[i][ENEMIES_DIR_X]==-1){
                enemySprite.setTexture(enemyTextures[TYPE_CHELNOVL]);
            
                if(enemies[i][ENEMIES_FRAME]==1){ //walking
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>4)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    enemySprite.setTextureRect(IntRect(6+ enemies[i][ENEMIES_CURRENT]*33, 0, 32, 42));
                }
                else if(enemies[i][ENEMIES_FRAME]==2){ //jumping
                    int jumpFrameL=0;
                    
                    if(enemies[i][ENEMIES_DIR_Y]==1){
                        if(enemies[i][ENEMIES_Y]>enemies[i][ENEMIES_TARGET_Y]+50){
                            jumpFrameL=0;
                        }
                        else if(enemies[i][ENEMIES_Y]>enemies[i][ENEMIES_TARGET_Y]+10){
                            jumpFrameL=2;
                        }
                        else {
                            jumpFrameL=1;
                        }
                    }
                    else {
                        jumpFrameL=2;
                    }
                    
                    if(jumpFrameL==0){
                        enemySprite.setTextureRect(IntRect(201, 13, 28, 29));
                    }
                    else if(jumpFrameL==1){
                        enemySprite.setTextureRect(IntRect(44, 56, 27, 34));
                    }
                    else {
                        enemySprite.setTextureRect(IntRect(13, 46, 24, 44));
                    }
                }
                else if(enemies[i][ENEMIES_FRAME]==3){ //waiting/Idle
                    //simple back and forth: 2->1->0->1->2
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        
                        //use ENEMIES_FLAG5 as direction flag
                        if(enemies[i][ENEMIES_FLAG5]==0){ //going down
                            enemies[i][ENEMIES_CURRENT]--;
                            if(enemies[i][ENEMIES_CURRENT]<=0){
                                enemies[i][ENEMIES_CURRENT]=0;
                                enemies[i][ENEMIES_FLAG5]=1; //start going up
                            }
                        } else { //going up
                            enemies[i][ENEMIES_CURRENT]++;
                            if(enemies[i][ENEMIES_CURRENT]>=2){
                                enemies[i][ENEMIES_CURRENT]=2;
                                enemies[i][ENEMIES_FLAG5]=0; //start going down
                            }
                        }
                    }
                    
                    if(enemies[i][ENEMIES_CURRENT]==0)
                        enemySprite.setTextureRect(IntRect(201, 13, 28, 29));
                    else if(enemies[i][ENEMIES_CURRENT]==1)
                        enemySprite.setTextureRect(IntRect(44, 56, 27, 34));
                    else
                        enemySprite.setTextureRect(IntRect(13, 46, 24, 44));
                }
                else if(enemies[i][ENEMIES_FRAME]==4){ //drop/Climb animation
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=20){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>10)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    
                    enemySprite.setTextureRect(IntRect(77, 42, 23, 48));
                }
                else if(enemies[i][ENEMIES_FRAME]==5){ //fire
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=20){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>8)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    
                    if(enemies[i][ENEMIES_CURRENT] <=2){
                        enemySprite.setTextureRect(IntRect(144, 94, 27, 36));
                    }
                    else if(enemies[i][ENEMIES_CURRENT] >2&&enemies[i][ENEMIES_CURRENT] <=4 ){
                        enemySprite.setTextureRect(IntRect(174, 95, 44, 35));
                    }
                    else {
                        enemySprite.setTextureRect(IntRect(223, 95, 43, 35));
                    }
                }
            } else {
                enemySprite.setTexture(enemyTextures[TYPE_CHELNOVR]);
            
                if(enemies[i][ENEMIES_FRAME]==1){ //walking
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>3)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    enemySprite.setTextureRect(IntRect(118+enemies[i][ENEMIES_CURRENT]*33, 0, 32, 42));
                }
                else if(enemies[i][ENEMIES_FRAME]==2){ //jumping
                    int jumpFrameR=0;
                    
                    if(enemies[i][ENEMIES_DIR_Y]==1){
                        if(enemies[i][ENEMIES_Y]>enemies[i][ENEMIES_TARGET_Y]+50){
                            jumpFrameR=0;
                        }
                        else if(enemies[i][ENEMIES_Y]>enemies[i][ENEMIES_TARGET_Y]+10){
                            jumpFrameR=2;
                        }
                        else {
                            jumpFrameR=1;
                        }
                    }
                    else {
                        jumpFrameR=2;
                    }
                    
                    if(jumpFrameR==0){
                        enemySprite.setTextureRect(IntRect(58, 13, 28, 29));
                    }
                    else if(jumpFrameR==1){
                        enemySprite.setTextureRect(IntRect(217, 56, 27, 34));
                    }
                    else {
                        enemySprite.setTextureRect(IntRect(188, 46, 23, 44));
                    }
                }
                else if(enemies[i][ENEMIES_FRAME]==3){ //waiting/Idle
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=10){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        
                        if(enemies[i][ENEMIES_FLAG5]==0){
                            enemies[i][ENEMIES_CURRENT]--;
                            if(enemies[i][ENEMIES_CURRENT]<=0){
                                enemies[i][ENEMIES_CURRENT]=0;
                                enemies[i][ENEMIES_FLAG5]=1;
                            }
                        } else {
                            enemies[i][ENEMIES_CURRENT]++;
                            if(enemies[i][ENEMIES_CURRENT]>=2){
                                enemies[i][ENEMIES_CURRENT]=2;
                                enemies[i][ENEMIES_FLAG5]=0;
                            }
                        }
                    }
                    
                    if(enemies[i][ENEMIES_CURRENT]==0)
                        enemySprite.setTextureRect(IntRect(58, 13, 28, 29));
                    else if(enemies[i][ENEMIES_CURRENT]==1)
                        enemySprite.setTextureRect(IntRect(217, 56, 27, 34));
                    else
                        enemySprite.setTextureRect(IntRect(188, 24, 23, 44));
                }
                else if(enemies[i][ENEMIES_FRAME]==4){ //drop/Climb
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=6){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>2)enemies[i][ENEMIES_CURRENT]=0;
                    }
                        enemySprite.setTextureRect(IntRect(188, 46, 23, 48));
                }

                else if(enemies[i][ENEMIES_FRAME]==5){ //fire
                    if(enemies[i][ENEMIES_TOTALFRAMES]>=20){
                        enemies[i][ENEMIES_TOTALFRAMES]=0;
                        enemies[i][ENEMIES_CURRENT]++;
                        if(enemies[i][ENEMIES_CURRENT]>8)enemies[i][ENEMIES_CURRENT]=0;
                    }
                    
                    if(enemies[i][ENEMIES_CURRENT] <=2){
                        enemySprite.setTextureRect(IntRect(117, 94, 27, 36));
                    }
                    else if(enemies[i][ENEMIES_CURRENT] >2&&enemies[i][ENEMIES_CURRENT] <=4 ){
                        enemySprite.setTextureRect(IntRect(69, 95, 44, 35));
                    }
                    else {
                        enemySprite.setTextureRect(IntRect(22, 95, 43, 35));
                    }
                } 
            }
        }
    }
}



void updateProjectileAnimation(int i, int**& projectiles, Sprite& projSprite, Texture enemyTextures[], Texture& minRevTex){
    const int MAX_PROJECTILES=100;
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_VX=2;
    const int PROJECT_VY=3;
    const int PROJECT_DIR=4; 
    const int PROJECT_TYPE=5;
    const int PROJECT_STATE=6;
    const int PROJECT_TIMER=7;
    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;

    const int TYPE_GHOSTL=0;
    const int TYPE_GHOSTR=4;
    const int TYPE_SKELETONL=1;
    const int TYPE_SKELETONR=5;
    const int TYPE_INVISIBLEL= 2;
    const int TYPE_INVISIBLER= 6;
    const int TYPE_CHELNOVL= 3;
    const int TYPE_CHELNOVR= 7;


    int projType=projectiles[i][PROJECT_TYPE];
    
    if(projType==-1){
        //enemy projectile 
        return;
    }
    
    //player projectile
    static int animFrames[MAX_PROJECTILES]={0};
    static int animTimers[MAX_PROJECTILES]={0}; 
    
    animTimers[i]++;
    
    //ipdate frame every 5 frame
    if(animTimers[i]>=5){
        animTimers[i]=0;
        animFrames[i]++;
        
        if(projType==TYPE_CHELNOV){
            if(animFrames[i]>8)animFrames[i]=1; //loop frames 1-8 for Chelnov
        } else {
            if(animFrames[i]>8)animFrames[i]=1; //loop frames 1-8 for others
        }
    }
    
    int currentFrame=animFrames[i];
    
    if(projType==TYPE_GHOST){
         projSprite.setTexture(enemyTextures[TYPE_GHOSTL]);
        projSprite.setTextureRect(IntRect(936+currentFrame*34, 8, 32, 32));
    }
    else if(projType==TYPE_SKELETON){
        projSprite.setTexture(enemyTextures[TYPE_SKELETONL]);
        projSprite.setTextureRect(IntRect(1040+currentFrame*34, 40, 32, 32));
    }
    else if(projType==TYPE_INVISIBLE){
        projSprite.setTexture(enemyTextures[TYPE_INVISIBLEL]);
        projSprite.setTextureRect(IntRect(812+currentFrame*34, 24, 32, 32));
    }
    else if(projType==TYPE_CHELNOV){
         projSprite.setTexture(enemyTextures[TYPE_CHELNOVL]);
        projSprite.setTextureRect(IntRect(7+currentFrame*34, 135, 32, 32));
    }
    else if(projType==TYPE_MINION){
            projSprite.setTexture(minRevTex);
            projSprite.setTextureRect(IntRect(0, 0, 64, 64));
            projSprite.setScale(2, 2);
    }
}




void initializeEnemiesLevel1(int**& enemies, int& enemyCount, int& enemyCapacity){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;
    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;
    
    const int STATE_INACTIVE=3;
    const int ENEMY_TOTAL_PROPERTIES=22;


    enemyCount=0;
    
    //8 ghosts
    int ghostPositions[8][2]={
        {128, 128}, {704, 128}, {128, 320}, {704, 320},
        {128, 512}, {512, 512}, {704, 512}, {896, 512}
    };
    
    for(int i=0;i<8;i++){
        increaseSize(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
        enemies[enemyCount][ENEMIES_X]=ghostPositions[i][0];
        enemies[enemyCount][ENEMIES_Y]=ghostPositions[i][1];
        enemies[enemyCount][ENEMIES_DIR_X]=1;
        enemies[enemyCount][ENEMIES_DIR_Y]=0;
        enemies[enemyCount][ENEMIES_TYPE]=TYPE_GHOST;
        enemies[enemyCount][ENEMIES_STATE]=STATE_ACTIVE;
        enemies[enemyCount][ENEMIES_TIMER]=0;
        enemies[enemyCount][ENEMIES_FLAG1]=0;
        enemies[enemyCount][ENEMIES_TOTALFRAMES]=0;
        enemies[enemyCount][ENEMIES_CURRENT]=0;
        enemies[enemyCount][ENEMIES_FLAG5]=1;
        enemyCount++;
    }
    
    //4 skeletons
    int skeletonPositions[4][2]={
        {128, 116}, {128, 308}, {128, 500}, {704, 500}
    };
    
    for(int i=0;i<4;i++){
        increaseSize(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
        enemies[enemyCount][ENEMIES_X]=skeletonPositions[i][0];
        enemies[enemyCount][ENEMIES_Y]=skeletonPositions[i][1];
        enemies[enemyCount][ENEMIES_DIR_X]=1;
        enemies[enemyCount][ENEMIES_DIR_Y]=1;
        enemies[enemyCount][ENEMIES_TYPE]=TYPE_SKELETON;
        enemies[enemyCount][ENEMIES_STATE]=STATE_ACTIVE;
        enemies[enemyCount][ENEMIES_TIMER]=0;
        enemies[enemyCount][ENEMIES_JUMP]=0;
        enemies[enemyCount][ENEMIES_FLAG1]=0;
        enemies[enemyCount][ENEMIES_FLAG2]=0;
        enemies[enemyCount][ENEMIES_FLAG3]=0;
        enemies[enemyCount][ENEMIES_FLAG4]=0;
        enemies[enemyCount][ENEMIES_TOTALFRAMES]=0;
        enemies[enemyCount][ENEMIES_CURRENT]=0;
        enemies[enemyCount][ENEMIES_FLAG5]=0;
        enemyCount++;
    }
}
void initializeEnemiesLevel2(int**& enemies, int& enemyCount, int& enemyCapacity){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;

    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;

    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;
    const int STATE_INACTIVE=3;
    const int ENEMY_TOTAL_PROPERTIES=22;

    enemyCount=20; //total enemies to spawn
    int spawnIndex=0;
    
    //4 ghosts
    for(int i=0;i<4;i++){
        increaseSize(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
        enemies[spawnIndex][ENEMIES_X]=150+i*200;
        enemies[spawnIndex][ENEMIES_Y]=200;
        enemies[spawnIndex][ENEMIES_TYPE]=TYPE_GHOST;
        enemies[spawnIndex][ENEMIES_STATE]=STATE_INACTIVE; //start inactive
        enemies[spawnIndex][ENEMIES_DIR_X]=1;
        enemies[spawnIndex][ENEMIES_SPAWN_TIMER]=i*180; //spawn delay (3 seconds apart)
        enemies[spawnIndex][ENEMIES_TIMER]=0;
        enemies[spawnIndex][ENEMIES_FLAG1]=0;
        enemies[spawnIndex][ENEMIES_TOTALFRAMES]=0;
        enemies[spawnIndex][ENEMIES_CURRENT]=0;
        enemies[spawnIndex][ENEMIES_FLAG5]=1;
        spawnIndex++;
    }
    //9 skeletons
    for(int i=0;i<9;i++){
        increaseSize(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
        enemies[spawnIndex][ENEMIES_X]=100+i*100;
        enemies[spawnIndex][ENEMIES_Y]=300+(i%3)*128;
        enemies[spawnIndex][ENEMIES_DIR_X]=1;
        enemies[spawnIndex][ENEMIES_DIR_Y]=1;
        enemies[spawnIndex][ENEMIES_TYPE]=TYPE_SKELETON;
        enemies[spawnIndex][ENEMIES_STATE]=STATE_INACTIVE; 
        enemies[spawnIndex][ENEMIES_TIMER]=0;
        enemies[spawnIndex][ENEMIES_JUMP]=0;
        enemies[spawnIndex][ENEMIES_FLAG1]=0;
        enemies[spawnIndex][ENEMIES_FLAG2]=0;
        enemies[spawnIndex][ENEMIES_FLAG3]=0;
        enemies[spawnIndex][ENEMIES_FLAG4]=0;
        enemies[spawnIndex][ENEMIES_TOTALFRAMES]=0;
        enemies[spawnIndex][ENEMIES_CURRENT]=0;
        enemies[spawnIndex][ENEMIES_FLAG5]=0;
        enemies[spawnIndex][ENEMIES_SPAWN_TIMER]=(4+i)*180; //after ghosts
        spawnIndex++;
    }
    
    //3 invisible men
    for(int i=0;i<3;i++){
        increaseSize(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
        enemies[spawnIndex][ENEMIES_X]=200+i*250;
        enemies[spawnIndex][ENEMIES_Y]=400;
        enemies[spawnIndex][ENEMIES_TYPE]=TYPE_INVISIBLE;
        enemies[spawnIndex][ENEMIES_STATE]=STATE_INACTIVE;
        enemies[spawnIndex][ENEMIES_DIR_X]=1;
        enemies[spawnIndex][ENEMIES_VISIBLE]=1;
        enemies[spawnIndex][ENEMIES_SPAWN_TIMER]=(13+i)*180; //after skeletons
        spawnIndex++;
    }
    
    //4 chelnov
    for(int i=0;i<4;i++){
        increaseSize(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
        enemies[spawnIndex][ENEMIES_X]=180+i*220;
        enemies[spawnIndex][ENEMIES_Y]=250;
        enemies[spawnIndex][ENEMIES_TYPE]=TYPE_CHELNOV;
        enemies[spawnIndex][ENEMIES_STATE]=STATE_INACTIVE; 
        enemies[spawnIndex][ENEMIES_DIR_X]=1;
        enemies[spawnIndex][ENEMIES_DIR_Y]=1;
        enemies[spawnIndex][ENEMIES_SHOOT_TIMER]=0;
        enemies[spawnIndex][ENEMIES_SPAWN_TIMER]=(16+i)*180; //after invisible men
        enemies[spawnIndex][ENEMIES_TIMER]=0;
        enemies[spawnIndex][ENEMIES_JUMP]=0;
        enemies[spawnIndex][ENEMIES_FLAG1]=0;
        enemies[spawnIndex][ENEMIES_FLAG2]=0;
        enemies[spawnIndex][ENEMIES_FLAG3]=0;
        enemies[spawnIndex][ENEMIES_FLAG4]=0;
        enemies[spawnIndex][ENEMIES_TOTALFRAMES]=0;
        enemies[spawnIndex][ENEMIES_CURRENT]=0;
        enemies[spawnIndex][ENEMIES_FLAG5]=0;
        spawnIndex++;
    }
}
void snapEnemyToPlatform(int enemy[], char** lvl){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_TYPE=4;

    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;

    int enemyType=enemy[ENEMIES_TYPE];
    int enemyHeight=getEnemyHeight(enemyType);
    int enemyWidth=getEnemyWidth(enemyType);
    
    int enemyCellX=(enemy[ENEMIES_X]+enemyWidth/2)/CELL_SIZE;
    int startY=enemy[ENEMIES_Y]/CELL_SIZE;
    
    //search for platform (down)
    for(int y=startY; y<LEVEL_HEIGHT; y++){
        if(enemyCellX>=0&&enemyCellX<LEVEL_WIDTH){
            if(lvl[y][enemyCellX]=='#' ){
                //if found platform place on top of it
                enemy[ENEMIES_Y]=(y)*CELL_SIZE-(enemyHeight);
                return;
            }
        }
    }
    
}

void updateEnemySpawning(int**& enemies, int enemyCount, char** lvl){
    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;
    
    const int STATE_INACTIVE=3;
    const int ENEMIES_STATE=5;
    const int ENEMIES_SPAWN_TIMER=19;
    for(int i=0;i<enemyCount;i++){
        if(enemies[i][ENEMIES_STATE]==STATE_INACTIVE&&enemies[i][ENEMIES_SPAWN_TIMER]>=0){
            enemies[i][ENEMIES_SPAWN_TIMER]--;
            
            if(enemies[i][ENEMIES_SPAWN_TIMER]<=0){
                //snap to nearest platform before activating
                snapEnemyToPlatform(enemies[i], lvl);
                
                //activate this enemy
                enemies[i][ENEMIES_STATE]=STATE_ACTIVE;
            }
        }
    }
}

void updateGhost(int enemy[], char** lvl){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    const int TYPE_GHOST=0;
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    //enemy Types
    int enemySpeed=getEnemySpeed(TYPE_GHOST);
    int enemyWidth=getEnemyWidth(TYPE_GHOST);
    int enemyHeight=getEnemyHeight(TYPE_GHOST);
    
    //move in current direction
    enemy[ENEMIES_X]+=enemy[ENEMIES_DIR_X]*enemySpeed;
    enemy[ENEMIES_FRAME]=1;
    
    //check wall collision ahead
    int checkX=(enemy[ENEMIES_DIR_X]>0)?(enemy[ENEMIES_X]+enemyWidth)/CELL_SIZE:enemy[ENEMIES_X]/CELL_SIZE;
    int checkY=(enemy[ENEMIES_Y]+enemyHeight/2)/CELL_SIZE;
    
    if(checkY>=0&&checkY<LEVEL_HEIGHT&&checkX>=0&&checkX<LEVEL_WIDTH){
        if(lvl[checkY][checkX]=='#'){
            enemy[ENEMIES_DIR_X]*=-1;
        }
    }
    
    //check for edge 
    int groundCheckX=(enemy[ENEMIES_DIR_X]>0)?(enemy[ENEMIES_X]+enemyWidth+enemySpeed)/CELL_SIZE:(enemy[ENEMIES_X]-enemySpeed)/CELL_SIZE;
    int groundCheckY=(enemy[ENEMIES_Y]+enemyHeight+2)/CELL_SIZE;
    
    if(groundCheckY>=0&&groundCheckY<LEVEL_HEIGHT&&
       groundCheckX>=0&&groundCheckX<LEVEL_WIDTH){
        if(lvl[groundCheckY][groundCheckX] != '#'){
            enemy[ENEMIES_DIR_X]*=-1;
        }
    }
    
    //random direction change 
    if(rand()%600==7){
        enemy[ENEMIES_DIR_X]*=-1;
        enemy[ENEMIES_FRAME]=2;
    }
    
    //random pause
    if(rand()%500==9){
        enemy[ENEMIES_FLAG1]=1;
        enemy[ENEMIES_TIMER]=0;
        enemy[ENEMIES_FRAME]=2;
    }
    
    //handle pause
    if(enemy[ENEMIES_FLAG1]==1){
        enemy[ENEMIES_FRAME]=2;
        enemy[ENEMIES_TIMER]++;
        enemy[ENEMIES_X]-=enemy[ENEMIES_DIR_X]*enemySpeed; //stop movement
        if(enemy[ENEMIES_TIMER]>=60){
            enemy[ENEMIES_FLAG1]=0;
            enemy[ENEMIES_TIMER]=0;
        }
    }
}



void updateSkeleton(int enemy[], char** lvl){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;

    const int TYPE_SKELETON=1;
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    int enemySpeed=getEnemySpeed(TYPE_SKELETON);
    int enemyWidth=getEnemyWidth(TYPE_SKELETON);
    int enemyHeight=getEnemyHeight(TYPE_SKELETON);
    
    //not jumping 
    enemy[ENEMIES_FRAME]=1;
    if(enemy[ENEMIES_JUMP]==0){
        int random=rand()%100;
        
        //random wait behavior
        if(random==42||enemy[ENEMIES_FLAG1]==1){
            if(enemy[ENEMIES_TIMER]==1)enemy[ENEMIES_FLAG4]=random;
            enemy[ENEMIES_TIMER]++;
            enemy[ENEMIES_FLAG1]=1;
            enemy[ENEMIES_FRAME]=3;
            
            if(enemy[ENEMIES_TIMER]>=60){
                //decide next action after waiting
                int action=enemy[ENEMIES_FLAG4]%4;
                
                if(action==0 ){
                    enemy[ENEMIES_DIR_X]=1; //move right
                    enemy[ENEMIES_TIMER]=0;
                    enemy[ENEMIES_FLAG1]=0;
                }
                else if(action==1){
                    enemy[ENEMIES_DIR_X]=-1; //move left
                    enemy[ENEMIES_TIMER]=0;
                    enemy[ENEMIES_FLAG1]=0;
                }
                else if(action==2&&enemy[ENEMIES_FLAG2]==0){
                    //move up to platform above
                    //enemy[ENEMIES_FRAME]=4;
                    int upX=(enemy[ENEMIES_X]+enemyWidth/2)/CELL_SIZE;
                    int upY=((enemy[ENEMIES_Y]+20)/CELL_SIZE)-1;
                    if(upY>=0&&lvl[upY][upX]=='#'){
                        enemy[ENEMIES_FLAG2]=1;
                        enemy[ENEMIES_TARGET_Y]=enemy[ENEMIES_Y]-(3*CELL_SIZE);
                    }
                    else {
                        enemy[ENEMIES_TIMER]=0;
                        enemy[ENEMIES_FLAG1]=0;
                    }
                }
                else if(action==3&&enemy[ENEMIES_FLAG3]==0){
                    //move down to platform below
                    //enemy[ENEMIES_FRAME]=4;
                    int downX=(enemy[ENEMIES_X]+enemyWidth/2)/CELL_SIZE;
                    int downY=((enemy[ENEMIES_Y]+enemyHeight-10)/CELL_SIZE)+4;
                    if(downY<LEVEL_HEIGHT&&lvl[downY][downX]=='#'){
                        enemy[ENEMIES_FLAG3]=1;
                        enemy[ENEMIES_TARGET_Y]=enemy[ENEMIES_Y]+(3*CELL_SIZE);
                    }
                    else {
                        enemy[ENEMIES_TIMER]=0;
                        enemy[ENEMIES_FLAG1]=0;
                    }
                }
                //moving up
                else if(enemy[ENEMIES_FLAG2]==1){
                    enemy[ENEMIES_FRAME]=4;
                    enemy[ENEMIES_Y]-=2;
                    if(enemy[ENEMIES_Y]<=enemy[ENEMIES_TARGET_Y]){
                        //snap to the platform
                        int platformY=((int)(enemy[ENEMIES_Y]+enemyHeight))/CELL_SIZE;
                        enemy[ENEMIES_Y]=platformY*CELL_SIZE-enemyHeight;
                        enemy[ENEMIES_TIMER]=0;
                        enemy[ENEMIES_FLAG1]=0;
                        enemy[ENEMIES_FLAG2]=0;
                    }
                }
                //moving down
                else if(enemy[ENEMIES_FLAG3]==1){
                    enemy[ENEMIES_FRAME]=4;
                    enemy[ENEMIES_Y]+=2;
                    if(enemy[ENEMIES_Y]>=enemy[ENEMIES_TARGET_Y]){
                        //snap to the platform
                        int platformY=((int)(enemy[ENEMIES_Y]+enemyHeight))/CELL_SIZE;
                        enemy[ENEMIES_Y]=platformY*CELL_SIZE-enemyHeight;
                        enemy[ENEMIES_TIMER]=0;
                        enemy[ENEMIES_FLAG1]=0;
                        enemy[ENEMIES_FLAG3]=0;
                    }
                }
            }
        }
        //normal horizontal movement
        else {
            enemy[ENEMIES_FRAME]=1;
            enemy[ENEMIES_X]+=enemy[ENEMIES_DIR_X]*enemySpeed;
            
            //check wall collision
            int checkX=(enemy[ENEMIES_DIR_X]>0)?(enemy[ENEMIES_X]+enemyWidth+enemySpeed)/CELL_SIZE:(enemy[ENEMIES_X]-enemySpeed)/CELL_SIZE;
            int checkY=(enemy[ENEMIES_Y]+enemyHeight/2)/CELL_SIZE;
            
            if(checkY>=0&&checkY<LEVEL_HEIGHT&&checkX>=0&&checkX<LEVEL_WIDTH){
                if(lvl[checkY][checkX]=='#'){
                    enemy[ENEMIES_DIR_X]*=-1;
                }
            }
            
            //check edge
            int edgeX=(enemy[ENEMIES_DIR_X]>0)?(enemy[ENEMIES_X]+enemyWidth+enemySpeed)/CELL_SIZE:(enemy[ENEMIES_X]-enemySpeed)/CELL_SIZE;
            int edgeY=(enemy[ENEMIES_Y]+enemyHeight+2)/CELL_SIZE;
            
            if(edgeY>=0&&edgeY<LEVEL_HEIGHT&&edgeX>0&&edgeX<LEVEL_WIDTH){
                if(lvl[edgeY][edgeX] != '#'){
                    //either reverse or jump
                    //bool jumpPossible;
                    //if((enemy[ENEMIES_DIR_X]>0)&&(edgeX+1)<(LEVEL_WIDTH-1)&&(edgeX+2)<(LEVEL_WIDTH-1))jumpPossible=1;
                    // else if((enemy[ENEMIES_DIR_X]<0)&&(edgeX-1)>0&&(edgeX-2)>0)jumpPossible=1;
                    // else jumpPossible=0;1
                    if((rand()%2==0)){
                        enemy[ENEMIES_DIR_X]*=-1;
                    }
                    else {
                        //setup jump
                        for(int dist=2; dist<5; dist++){
                            int targetX=edgeX+(enemy[ENEMIES_DIR_X]*dist);
                            if(targetX>0&&targetX<LEVEL_WIDTH-1&&lvl[edgeY][targetX]=='#'){
                                enemy[ENEMIES_JUMP]=1;
                                enemy[ENEMIES_DIR_Y]=1;
                                enemy[ENEMIES_TARGET_Y]=enemy[ENEMIES_Y]-(dist*CELL_SIZE)/1.5;
                                break;
                            }
                        }
                    }
                }
            }
            
            //random direction change
            if(rand()%500==5){
                enemy[ENEMIES_DIR_X]*=-1;
            }
        }
    }
    //jumping
    else {
        enemy[ENEMIES_X]+=enemy[ENEMIES_DIR_X]*enemySpeed*1;
        
        if(enemy[ENEMIES_DIR_Y]==1){
            enemy[ENEMIES_Y]-=enemySpeed;
        }
        else {
            enemy[ENEMIES_Y]+=enemySpeed;
        }
        
        //reached peak
        if(enemy[ENEMIES_Y]<=enemy[ENEMIES_TARGET_Y]){
            enemy[ENEMIES_DIR_Y]=-1;
        }
        
        //check landing
        int landX=(enemy[ENEMIES_X]+(enemy[ENEMIES_DIR_X]>0 ?enemyWidth:0))/CELL_SIZE;
        int landY=(enemy[ENEMIES_Y]+enemyHeight+1)/CELL_SIZE;
        
        if(landY>=0&&landY<LEVEL_HEIGHT&&landX>=0&&landX<LEVEL_WIDTH){
            if(lvl[landY][landX]=='#'){
                enemy[ENEMIES_JUMP]=0;
                enemy[ENEMIES_DIR_Y]=1;
            }
        }
        enemy[ENEMIES_FRAME]=2;
    }
}






void teleportInvisibleManVertically(int enemy[], char** lvl){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    
    const int TYPE_INVISIBLE=2;
    const int CELL_SIZE=64;
    int enemyCellX=enemy[ENEMIES_X]/CELL_SIZE;
    int enemyCellY=enemy[ENEMIES_Y]/CELL_SIZE;
    
    //platform rows are at y=4, 7, 10, 13
    int platforms[4]={4, 7, 10, 13};
    int validPlatforms[4];
    int validCount=0;
    
    //find which platforms have blocks at enemy's X position
    for(int i=0;i<4;i++){
        int y=platforms[i];
        if(y != enemyCellY&&(lvl[y][enemyCellX]=='#')){
            validPlatforms[validCount]=y;
            validCount++;
        }
    }
    
    //randomly pick one valid platform
    if(validCount>0){
        int chosen=validPlatforms[rand()%validCount];
        enemy[ENEMIES_Y]=chosen*CELL_SIZE-getEnemyHeight(TYPE_INVISIBLE);
    }
}

void updateInvisibleMan(int enemy[], char** lvl){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    
    const int TYPE_INVISIBLE=2;
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    int enemySpeed=getEnemySpeed(TYPE_INVISIBLE);
    int enemyWidth=getEnemyWidth(TYPE_INVISIBLE);
    int enemyHeight=getEnemyHeight(TYPE_INVISIBLE);
    
    //normal movement
    
    
    //wall collision
    int checkX=(enemy[ENEMIES_DIR_X]>0)?(enemy[ENEMIES_X]+enemyWidth)/CELL_SIZE:enemy[ENEMIES_X]/CELL_SIZE;
    int checkY=(enemy[ENEMIES_Y]+enemyHeight/2)/CELL_SIZE;
    
    if(checkY>=0&&checkY<LEVEL_HEIGHT&&checkX>=0&&checkX<LEVEL_WIDTH){
        if(lvl[checkY][checkX]=='#'){
            enemy[ENEMIES_DIR_X]*=-1;
        }
    }
    
    //edge detection
    int edgeX=(enemy[ENEMIES_DIR_X]>0)?(enemy[ENEMIES_X]+enemyWidth+enemySpeed)/CELL_SIZE:(enemy[ENEMIES_X]-enemySpeed)/CELL_SIZE;
    int edgeY=(enemy[ENEMIES_Y]+enemyHeight+2)/CELL_SIZE;
    
    if(edgeY>=0&&edgeY<LEVEL_HEIGHT&&edgeX>=0&&edgeX<LEVEL_WIDTH){
        if(lvl[edgeY][edgeX] != '#'){
            enemy[ENEMIES_DIR_X]*=-1;
        }
    }
    
    //random invisibility
    if(rand()%1000==0 &&enemy[ENEMIES_VISIBLE]==1){
        enemy[ENEMIES_VISIBLE]=0;
        enemy[ENEMIES_TIMER]=0;
    }
    
    //handle invisibility duration
    if(enemy[ENEMIES_VISIBLE]==0){
        enemy[ENEMIES_TIMER]++;
        //cout<<212;
        enemy[ENEMIES_FRAME]=2;
        if(enemy[ENEMIES_TIMER]>=60){  
            if(enemy[ENEMIES_TIMER]>=120){    
                if(enemy[ENEMIES_TIMER]==120)teleportInvisibleManVertically(enemy, lvl);
                enemy[ENEMIES_FRAME]=3;
                //cout<<323;
                if(enemy[ENEMIES_TIMER]>=180){
                    enemy[ENEMIES_VISIBLE]=1;
                    enemy[ENEMIES_TIMER]=0;
                    enemy[ENEMIES_FRAME]=1;
                }
            }
        }
    }
    else {

    enemy[ENEMIES_X]+=enemy[ENEMIES_DIR_X]*enemySpeed;
        enemy[ENEMIES_FRAME]=1;

    }
    
    //random direction change
    if(rand()%600==0){
        enemy[ENEMIES_DIR_X]*=-1;
    }
}



//1 walk
//2 jump
//3 wait
//4 up

void updateChelnov(int enemy[], char** lvl, int**& projectiles, int& projCount, int& projCapacity){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    
    const int TYPE_CHELNOV=3;
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    const int ENEMY_TOTAL_PROPERTIES=22;
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_VX=2;
    const int PROJECT_VY=3;
    const int PROJECT_DIR=4; 
    const int PROJECT_TYPE=5;
    const int PROJECT_STATE=6;
    const int PROJECT_TIMER=7;
    const int ENEMY_PROJECT_HEIGHT=20; 
    const int ENEMY_PROJECT_WIDTH=64;


    int enemySpeed=getEnemySpeed(TYPE_CHELNOV);
    int enemyWidth=getEnemyWidth(TYPE_CHELNOV);
    int enemyHeight=getEnemyHeight(TYPE_CHELNOV);
    
    //shooting behavior
    enemy[ENEMIES_SHOOT_TIMER]++;
    if(enemy[ENEMIES_SHOOT_TIMER]>=240){ //every 4 seconds shoot gole
        enemy[ENEMIES_FRAME]=5;

        if(enemy[ENEMIES_SHOOT_TIMER]>=300){
            for(int i=0;i<3;i++){
                increaseSize(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES);
                projectiles[projCount][PROJECT_X]=enemy[ENEMIES_X]+i*ENEMY_PROJECT_WIDTH+(enemy[ENEMIES_DIR_X]>0?enemyWidth:0);
                projectiles[projCount][PROJECT_Y]=enemy[ENEMIES_Y]+enemyHeight/2;
                projectiles[projCount][PROJECT_VX]=enemy[ENEMIES_DIR_X]*8;
                projectiles[projCount][PROJECT_VY]=0;
                projectiles[projCount][PROJECT_TYPE]=-1; //enemy projectile
                projectiles[projCount][PROJECT_STATE]=0;
                projCount++;
                enemy[ENEMIES_SHOOT_TIMER]=0;
                enemy[ENEMIES_FRAME]=1;

            }
        }
    }
    else{
        //not jumping
        enemy[ENEMIES_FRAME]=1;
        if(enemy[ENEMIES_JUMP]==0){
            int random=rand()%100;
            
            //random wait behavior
            if(random==42||enemy[ENEMIES_FLAG1]==1){
                if(enemy[ENEMIES_TIMER]==1)enemy[ENEMIES_FLAG4]=random;
                enemy[ENEMIES_TIMER]++;
                enemy[ENEMIES_FLAG1]=1;
                enemy[ENEMIES_FRAME]=3;
                //decide next action after waiting
                int action=enemy[ENEMIES_FLAG4]%4;
                if(enemy[ENEMIES_TIMER]==1)action=rand()%2;
                if(action==0&&enemy[ENEMIES_FLAG2]==0){
                    //move up to platform above
                    enemy[ENEMIES_FRAME]=4;
                    int upX=(enemy[ENEMIES_X]+enemyWidth/2)/CELL_SIZE;
                    int upY=((enemy[ENEMIES_Y]+30)/CELL_SIZE)-1;
                    if(upY>=0&&lvl[upY][upX]=='#'){
                        enemy[ENEMIES_FLAG2]=1;
                        enemy[ENEMIES_TARGET_Y]=enemy[ENEMIES_Y]-(3*CELL_SIZE);
                        //snap to the platform
                        int platformY=((int)(enemy[ENEMIES_Y]+enemyHeight))/CELL_SIZE;
                        enemy[ENEMIES_Y]=platformY*CELL_SIZE-enemyHeight;
                    }
                    else {
                        enemy[ENEMIES_TIMER]=0;
                        enemy[ENEMIES_FLAG1]=0;
                    }
                }
                else if(action==1&&enemy[ENEMIES_FLAG3]==0){
                    //move down to platform below
                    enemy[ENEMIES_FRAME]=4;
                    int downX=(enemy[ENEMIES_X]+enemyWidth/2)/CELL_SIZE;
                    int downY=((enemy[ENEMIES_Y]+enemyHeight-10)/CELL_SIZE)+4;
                    if(downY<LEVEL_HEIGHT&&lvl[downY][downX]=='#'){
                        enemy[ENEMIES_FLAG3]=1;
                        enemy[ENEMIES_TARGET_Y]=enemy[ENEMIES_Y]+(3* CELL_SIZE);
                    }
                    else {
                        enemy[ENEMIES_TIMER]=0;
                        enemy[ENEMIES_FLAG1]=0;
                    }
                }
                //moving up
                else if(enemy[ENEMIES_FLAG2]==1){
                    enemy[ENEMIES_FRAME]=4;
                    enemy[ENEMIES_Y]-=2;
                    enemy[ENEMIES_SHOOT_TIMER]--;
                    if(enemy[ENEMIES_Y]<=enemy[ENEMIES_TARGET_Y]){
                        enemy[ENEMIES_TIMER]=0;
                        enemy[ENEMIES_FLAG1]=0;
                        enemy[ENEMIES_FLAG2]=0;
                        int platformY=((int)(enemy[ENEMIES_Y]+enemyHeight))/CELL_SIZE;
                        enemy[ENEMIES_Y]=platformY*CELL_SIZE-enemyHeight;
                    }
                }
                //moving down
                else if(enemy[ENEMIES_FLAG3]==1){
                    enemy[ENEMIES_FRAME]=4;
                    enemy[ENEMIES_Y]+=2;
                    enemy[ENEMIES_SHOOT_TIMER]--;
                    if(enemy[ENEMIES_Y]>=enemy[ENEMIES_TARGET_Y]){
                        enemy[ENEMIES_TIMER]=0;
                        enemy[ENEMIES_FLAG1]=0;
                        enemy[ENEMIES_FLAG3]=0;
                        int platformY=((int)(enemy[ENEMIES_Y]+enemyHeight))/CELL_SIZE;
                        enemy[ENEMIES_Y]=platformY*CELL_SIZE-enemyHeight;
                    }
                }
            
            }
            //normal horizontal movement
            else {
                enemy[ENEMIES_FRAME]=1;
                enemy[ENEMIES_X]+=enemy[ENEMIES_DIR_X]*enemySpeed;
                
                //check wall collision
                int checkX=(enemy[ENEMIES_DIR_X]>0)?(enemy[ENEMIES_X]+enemyWidth+enemySpeed)/CELL_SIZE:(enemy[ENEMIES_X]-enemySpeed)/CELL_SIZE;
                int checkY=(enemy[ENEMIES_Y]+enemyHeight/2)/CELL_SIZE;
                
                if(checkY>=0&&checkY<LEVEL_HEIGHT&&checkX>=0&&checkX<LEVEL_WIDTH){
                    if(lvl[checkY][checkX]=='#'){
                        enemy[ENEMIES_DIR_X]*=-1;
                    }
                }
                
                //check edge
                int edgeX=(enemy[ENEMIES_DIR_X]>0)?(enemy[ENEMIES_X]+enemyWidth+enemySpeed)/CELL_SIZE:(enemy[ENEMIES_X]-enemySpeed)/CELL_SIZE;
                int edgeY=(enemy[ENEMIES_Y]+enemyHeight+2)/CELL_SIZE;
                
                if(edgeY>=0&&edgeY<LEVEL_HEIGHT&&edgeX>0&&edgeX<LEVEL_WIDTH){
                    if(lvl[edgeY][edgeX] != '#'&&lvl[edgeY][edgeX] != '\\'&&lvl[edgeY][edgeX] != '/'){
                        //either reverse or jump
                        bool jumpPossible;
                        if((enemy[ENEMIES_DIR_X]>0)&&(edgeX+1)<(LEVEL_WIDTH-1)&&(edgeX+2)<(LEVEL_WIDTH-1))jumpPossible=1;
                        else if((enemy[ENEMIES_DIR_X]<0)&&(edgeX-1)>0&&(edgeX-2)>0)jumpPossible=1;
                        else jumpPossible=0;
                        if(rand()%5==0&&jumpPossible){
                            //setup jump
                            enemy[ENEMIES_JUMP]=1;
                            enemy[ENEMIES_DIR_Y]=1;
                            enemy[ENEMIES_TARGET_Y]=enemy[ENEMIES_Y]-(2*CELL_SIZE);
                        }
                        else {
                            enemy[ENEMIES_DIR_X]*=-1;
                        }
                    }
                }
            }
        }
        //jumping
        else {

            enemy[ENEMIES_SHOOT_TIMER]--;
            enemy[ENEMIES_X]+=enemy[ENEMIES_DIR_X]*enemySpeed*1;
            
            if(enemy[ENEMIES_DIR_Y]==1){
                enemy[ENEMIES_Y]-=enemySpeed*1;
            }
            else {
                enemy[ENEMIES_Y]+=enemySpeed*1;
            }
            
            //reached peak
            if(enemy[ENEMIES_Y]<=enemy[ENEMIES_TARGET_Y]){
                enemy[ENEMIES_DIR_Y]=-1;
            }
            
            //check landing
            int landX=(enemy[ENEMIES_X]+(enemy[ENEMIES_DIR_X]>0 ?enemyWidth:0))/CELL_SIZE;
            int landY=(enemy[ENEMIES_Y]+enemyHeight+1)/CELL_SIZE;
            
            if(landY>=0&&landY<LEVEL_HEIGHT&&landX>=0&&landX<LEVEL_WIDTH){
                if(lvl[landY][landX]=='#'||lvl[landY][landX]=='\\'||lvl[landY][landX]=='/'){
                    enemy[ENEMIES_JUMP]=0;
                    enemy[ENEMIES_DIR_Y]=1;
                    int platformY=((int)(enemy[ENEMIES_Y]+enemyHeight))/CELL_SIZE;
                    enemy[ENEMIES_Y]=platformY*CELL_SIZE-enemyHeight;
                }
            }
            enemy[ENEMIES_FRAME]=2;
        }
    }

}




void updateBossMinion(int* minion, char** lvl, float playerX, float playerY, bool bossAngry){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    
    const int TYPE_MINION=4;
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;

    //spinning state
    if(minion[ENEMIES_FLAG1]==1){
        minion[ENEMIES_FRAME]=4; //spinning/climbing animation
        
        //move up to target pos
        minion[ENEMIES_Y]-=2;        
        //if target reached
        if(minion[ENEMIES_Y]<=minion[ENEMIES_TARGET_Y]){
            minion[ENEMIES_Y]=minion[ENEMIES_TARGET_Y];
            minion[ENEMIES_FLAG1]=0; 
            minion[ENEMIES_TIMER]=60; 
        }
        return;
    }
    
    
    int enemySpeed=2;
    int enemyWidth=128;
    int enemyHeight=128;
    //follow player if angry
    if(bossAngry){
        float dx=playerX-minion[ENEMIES_X];
        
        
        if(abs(dx)>10){
            minion[ENEMIES_X]+=(dx>0)?enemySpeed:-enemySpeed;
            minion[ENEMIES_DIR_X]=(dx>0)?1:-1;
            minion[ENEMIES_FRAME]=1; //walking
        }
        else {
            minion[ENEMIES_FRAME]=3; //idle
        }
        
    }
    else {
        
        //handle vertical movement (up/down platforms)
        if(minion[ENEMIES_FLAG2]==1){ //moving up
            minion[ENEMIES_FRAME]=4;
            minion[ENEMIES_Y]-=2;
            if(minion[ENEMIES_Y]<=minion[ENEMIES_TARGET_Y]){
                int platformY=((int)(minion[ENEMIES_Y]+enemyHeight))/CELL_SIZE;
                minion[ENEMIES_Y]=platformY*CELL_SIZE-enemyHeight;
                minion[ENEMIES_TIMER]=0;
                minion[ENEMIES_FLAG1]=0;
                minion[ENEMIES_FLAG2]=0;
            }
            return;
        }
        
        if(minion[ENEMIES_FLAG3]==1){ //moving down
            minion[ENEMIES_FRAME]=4;
            minion[ENEMIES_Y]+=2;
            if(minion[ENEMIES_Y]>=minion[ENEMIES_TARGET_Y]){
                int platformY=((int)(minion[ENEMIES_Y]+enemyHeight))/CELL_SIZE;
                minion[ENEMIES_Y]=platformY*CELL_SIZE-enemyHeight;
                minion[ENEMIES_TIMER]=0;
                minion[ENEMIES_FLAG1]=0;
                minion[ENEMIES_FLAG3]=0;
            }
            return;
        }
        
        int random=rand()%100;
        
        //random wait/decision
        if(random==42||minion[ENEMIES_FLAG1]==2){ 
            minion[ENEMIES_TIMER]++;
            minion[ENEMIES_FLAG1]=2;
            minion[ENEMIES_FRAME]=3;
            
            if(minion[ENEMIES_TIMER]>=60){
                int action=rand()%4;
                
                if(action==0){
                    minion[ENEMIES_DIR_X]=1;
                    minion[ENEMIES_TIMER]=0;
                    minion[ENEMIES_FLAG1]=0;
                }
                else if(action==1){
                    minion[ENEMIES_DIR_X]=-1;
                    minion[ENEMIES_TIMER]=0;
                    minion[ENEMIES_FLAG1]=0;
                }
                else if(action==2){ //try move up
                    int upX=(minion[ENEMIES_X]+enemyWidth/2)/CELL_SIZE;
                    int upY=((minion[ENEMIES_Y]+20)/CELL_SIZE)-1;
                    if(upY>=0&&lvl[upY][upX]=='#'){
                        minion[ENEMIES_FLAG2]=1;
                        minion[ENEMIES_TARGET_Y]=minion[ENEMIES_Y]-(3*CELL_SIZE);
                    }
                    else {
                        minion[ENEMIES_TIMER]=0;
                        minion[ENEMIES_FLAG1]=0;
                    }
                }
                else if(action==3){ //try move down
                    int downX=(minion[ENEMIES_X]+enemyWidth/2)/CELL_SIZE;
                    int downY=((minion[ENEMIES_Y]+enemyHeight-10)/CELL_SIZE)+4;
                    if(downY<LEVEL_HEIGHT&&lvl[downY][downX]=='#'){
                        minion[ENEMIES_FLAG3]=1;
                        minion[ENEMIES_TARGET_Y]=minion[ENEMIES_Y]+(3*CELL_SIZE);
                    }
                    else {
                        minion[ENEMIES_TIMER]=0;
                        minion[ENEMIES_FLAG1]=0;
                    }
                }
            }
        }
        else {
            //normal horizontal movement
            minion[ENEMIES_FRAME]=1;
            minion[ENEMIES_X]+=minion[ENEMIES_DIR_X]*enemySpeed;
            
            //wall collision
            int checkX=(minion[ENEMIES_DIR_X]>0)?(minion[ENEMIES_X]+enemyWidth+enemySpeed)/CELL_SIZE:(minion[ENEMIES_X]-enemySpeed)/CELL_SIZE;
            int checkY=(minion[ENEMIES_Y]+enemyHeight/2)/CELL_SIZE;
            
            if(checkY>=0&&checkY<LEVEL_HEIGHT&&checkX>=0&&checkX<LEVEL_WIDTH){
                if(lvl[checkY][checkX]=='#'){
                    minion[ENEMIES_DIR_X]*=-1;
                }
            }
            
            //edge detection
            int edgeX=(minion[ENEMIES_DIR_X]>0)?(minion[ENEMIES_X]+enemyWidth+enemySpeed)/CELL_SIZE:(minion[ENEMIES_X]-enemySpeed)/CELL_SIZE;
            int edgeY=(minion[ENEMIES_Y]+enemyHeight+2)/CELL_SIZE;
            
            if(edgeY>=0&&edgeY<LEVEL_HEIGHT&&edgeX>0&&edgeX<LEVEL_WIDTH){
                if(lvl[edgeY][edgeX] != '#'){
                    minion[ENEMIES_DIR_X]*=-1; // Just reverse, no jumping
                }
            }
            
            //random direction change
            if(rand()%500==5){
                minion[ENEMIES_DIR_X]*=-1;
            }
        }
    }
}




void updateAllEnemies(int**& enemies, int enemyCount, char** lvl, int**& projectiles, int& projCount, int& projCapacity, float playerX, float playerY, bool bossAngry){

    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;
    const int ENEMIES_TYPE=4;
    const int STATE_ACTIVE=0;
    const int ENEMIES_STATE=5;


    for(int i=0;i<enemyCount;i++){
        if(enemies[i][ENEMIES_STATE]==STATE_ACTIVE){
            int type=enemies[i][ENEMIES_TYPE];
            
            if(type==TYPE_GHOST){
                updateGhost(enemies[i], lvl);
            }
            else if(type==TYPE_SKELETON){
                updateSkeleton(enemies[i], lvl);
            }
            else if(type==TYPE_INVISIBLE){
                updateInvisibleMan(enemies[i], lvl);
            }
            else if(type==TYPE_CHELNOV){
                updateChelnov(enemies[i], lvl, projectiles, projCount, projCapacity);
            }
            else if(type==TYPE_MINION){  
                updateBossMinion(enemies[i], lvl, playerX, playerY, bossAngry);
            }
        }
    }
}


bool isInVacuumRange(int enemyX, int enemyY, float playerX, float playerY, int vacuumDir, int selectedCharacter, int enemyType,  int* activePowerups, float vacuumAngle){
    const int YELLOW_TUMBLEPOPER=0;
    const int GREEN_TUMBLEPOPER=1;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const float INITITAL_VACUUM_ANGLE=26.57*3.14/180;

    int enemyHeight=getEnemyHeight(enemyType);
    int enemyWidth=getEnemyWidth(enemyType);
    int vacuumRange= getVacuumRange(activePowerups,selectedCharacter);
    
    float dist=0;
    float crossDist=0;
    float vaccumHeight=vacuumRange*tan(getVacuumAngle(activePowerups, selectedCharacter));
    
    if(selectedCharacter==GREEN_TUMBLEPOPER){
        float dx=enemyX-playerX;
        float dy=enemyY-playerY;
        float dist=sqrt(dx*dx+dy*dy);

        if(dist>vacuumRange+150)return false;

        //calculate angle to enemy
        float enemyAngle=atan2((-dy), dx);
        if (enemyAngle<0)enemyAngle+=2*3.14159f;

        //check if it is within cone
        float angleDiff=abs(vacuumAngle-enemyAngle);
        if(angleDiff>3.14159f)angleDiff=2*3.14159f-angleDiff;

        return angleDiff<INITITAL_VACUUM_ANGLE;

    }else{
        if(vacuumDir==0){ //up
            //distance from player top to enemy bottom
            dist=(playerY)-(enemyY+enemyHeight);
            //x dis check
            crossDist=abs((enemyX+enemyWidth/2)-(playerX+PLAYER_WIDTH/2));
            return (dist<=vacuumRange&&dist>=0&&crossDist<vaccumHeight );
        }
        else if(vacuumDir==1){ //down
            //distance from player bottom to enemy top
            dist=enemyY-(playerY+PLAYER_HEIGHT);
            crossDist=abs((enemyX+enemyWidth/2)-(playerX+PLAYER_WIDTH/2));
            return (dist<=vacuumRange&&dist>=0&&crossDist<vaccumHeight);
        }
        else if(vacuumDir==2){ //left
            //distance from player left to enemy right
            dist=playerX-(enemyX+enemyWidth);
            crossDist=abs((enemyY+enemyHeight/2)-(playerY+PLAYER_HEIGHT/2));
            return (dist<=vacuumRange&&dist>=0&&crossDist<vaccumHeight);
        }
        else if(vacuumDir==3){ //right
            //distance from player right to enemy left
            dist=enemyX-(playerX+PLAYER_WIDTH);
            crossDist=abs((enemyY+enemyHeight/2)-(playerY+PLAYER_HEIGHT/2));
            return (dist<=vacuumRange&&dist>=0&&crossDist<vaccumHeight);
        }
    }
    
    return false;
}



void updateVacuumAnimation(float playerX, float playerY, int vacuumDir,   float vacuumRange, bool vacuumActive, Sprite& vacuumSprite, Texture& vacuumTex, bool movingLeft, bool movingRight, Texture& vaccumUp, int* activePowerups){
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const int POWER_ACTIVE_SPEED=0;
    const int POWER_ACTIVE_RANGE=1;
    const int POWER_ACTIVE_POWER=2;

    if(!vacuumActive){
        vacuumSprite.setPosition(-1000, -1000);
        return;
    } 
    
    //vaccum animation frames for left/right (x, y, width, height)
    int framesHorizontal[12][4]={
        {552, 172, 32, 32},
        {519, 172, 32, 32},
        {502, 172, 16, 32},
        {485, 172, 16, 32},
        {466, 172, 16, 32},
        {433, 172, 32, 32},
        {400, 172, 32, 32},
        {349, 172, 48, 32},
        {300, 172, 48, 32},
        {251, 172, 48, 32},
        {218, 172, 32, 32},
        {201, 172, 16, 32}
    };
    
    //vaccum animation frames for up/down
    int framesVertical[12][4]={
        {0, 320+12, 32, 32},
        {0, 288+11, 32, 32},
        {0, 272+10, 32, 16},
        {0, 256+9, 32, 16},
        {0, 240+8, 32, 16},
        {0, 208+7, 32, 32},
        {0, 176+6, 32, 32},
        {0, 144+4, 32, 48},
        {0, 96+3, 32, 48},
        {0, 48+2, 32, 48},
        {0, 16+1, 32, 32},
        {0, 0, 32, 16}
    };

    int framesHorizontalRange[5][4]={
        {16, 336, 32, 32},
        {49, 336, 48, 32},
        {98, 336, 64, 32},
        {163, 336, 80, 32},
        {46226, 336, 80, 32},
    };

    int framesHorizontalPower[7][4]={
        {493, 336, 32, 32},
        {526, 336, 48, 32},
        {575, 336, 80, 32},
        {656, 336, 96, 32},
        {753, 336, 96, 32},
        {850, 336, 96, 32},
        {947, 336, 96, 32},
    };
    
    static int frameCounter=0;
    static int currentFrame=7;
    static bool facingRight=true;
    
    //update facing direction
    if(movingRight)facingRight=true;
    if(movingLeft)facingRight=false;
    
    frameCounter++;
    
    //frames wrt to powerups
    bool usingPowerFrames=(activePowerups[POWER_ACTIVE_POWER]>0);
    bool usingRangeFrames=(activePowerups[POWER_ACTIVE_RANGE]>0);
    
    int maxFrames=12;
    int minFrame=4;
    
    if(usingPowerFrames){
        maxFrames=7;
        minFrame=0;
        if(currentFrame>=maxFrames)currentFrame=0;
    }
    else if(usingRangeFrames){
        maxFrames=4;
        minFrame=0;
        if(currentFrame>=maxFrames)currentFrame=0;
    }
    
    if(frameCounter>=4){ //change frame every 4 ticks
        frameCounter=0;
        currentFrame++;
        if(currentFrame>=maxFrames)currentFrame=minFrame;
    }
    
    float vacX=0;
    float vacY=0;
    
    //facing left
    if(facingRight==false){
        
        if(vacuumDir==0){ //up W
            vacuumSprite.setTexture(vaccumUp);
            vacuumSprite.setTextureRect(IntRect(framesVertical[currentFrame][0], framesVertical[currentFrame][1], framesVertical[currentFrame][2], framesVertical[currentFrame][3]));
            vacuumSprite.setScale(3, 3);
            vacX=playerX;
            vacY=playerY-(framesVertical[currentFrame][3]*3);
        }
        else if(vacuumDir==1){ //down S
            vacuumSprite.setTexture(vaccumUp);
            vacuumSprite.setTextureRect(IntRect(framesVertical[currentFrame][0], framesVertical[currentFrame][1], framesVertical[currentFrame][2], framesVertical[currentFrame][3]));
            vacuumSprite.setScale(3, -3);
            vacX=playerX;
            vacY=playerY+PLAYER_HEIGHT+(framesVertical[currentFrame][3]*3);
        }
        else if(vacuumDir==2){ //left A
            vacuumSprite.setTexture(vacuumTex);
            
            if(usingPowerFrames){
                vacuumSprite.setTextureRect(IntRect(framesHorizontalPower[currentFrame][0], framesHorizontalPower[currentFrame][1], framesHorizontalPower[currentFrame][2], framesHorizontalPower[currentFrame][3]));
                vacuumSprite.setScale(3, 3);
                vacX=playerX-(framesHorizontalPower[currentFrame][2]*3);
            }
            else if(usingRangeFrames){
                vacuumSprite.setTextureRect(IntRect(framesHorizontalRange[currentFrame][0], framesHorizontalRange[currentFrame][1], framesHorizontalRange[currentFrame][2], framesHorizontalRange[currentFrame][3]));
                vacuumSprite.setScale(3, 3);
                vacX=playerX-(framesHorizontalRange[currentFrame][2]*3);
            }
            else {
                vacuumSprite.setTextureRect(IntRect(framesHorizontal[currentFrame][0], framesHorizontal[currentFrame][1], framesHorizontal[currentFrame][2], framesHorizontal[currentFrame][3]));
                vacuumSprite.setScale(3, 3);
                vacX=playerX-(framesHorizontal[currentFrame][2]*3);
            }
            vacY=playerY+PLAYER_HEIGHT/3;
        }
        else if(vacuumDir==3){ //right D
            vacuumSprite.setTexture(vacuumTex);
            
            if(usingPowerFrames){
                vacuumSprite.setTextureRect(IntRect(framesHorizontalPower[currentFrame][0], framesHorizontalPower[currentFrame][1], framesHorizontalPower[currentFrame][2], framesHorizontalPower[currentFrame][3]));
                vacuumSprite.setScale(-3, 3);
                vacX=playerX+PLAYER_WIDTH+(framesHorizontalPower[currentFrame][2]*3);
            }
            else if(usingRangeFrames){
                vacuumSprite.setTextureRect(IntRect(framesHorizontalRange[currentFrame][0], framesHorizontalRange[currentFrame][1], framesHorizontalRange[currentFrame][2], framesHorizontalRange[currentFrame][3]));
                vacuumSprite.setScale(-3, 3);
                vacX=playerX+PLAYER_WIDTH+(framesHorizontalRange[currentFrame][2]*3);
            }
            else {
                vacuumSprite.setTextureRect(IntRect(framesHorizontal[currentFrame][0], framesHorizontal[currentFrame][1], framesHorizontal[currentFrame][2], framesHorizontal[currentFrame][3]));
                vacuumSprite.setScale(-3, 3);
                vacX=playerX+PLAYER_WIDTH+(framesHorizontal[currentFrame][2]*3);
            }
            vacY=playerY+PLAYER_HEIGHT/3;
        }
    }
    //fACING RIGHT
    else {
        
        if(vacuumDir==0){ //up W
            vacuumSprite.setTexture(vaccumUp);
            vacuumSprite.setTextureRect(IntRect(framesVertical[currentFrame][0], framesVertical[currentFrame][1],framesVertical[currentFrame][2], framesVertical[currentFrame][3]));
            vacuumSprite.setScale(3, 3);
            vacX=playerX;
            vacY=playerY-(framesVertical[currentFrame][3]*3);
        }
        else if(vacuumDir==1){ //down S
            vacuumSprite.setTexture(vaccumUp);
            vacuumSprite.setTextureRect(IntRect(framesVertical[currentFrame][0], framesVertical[currentFrame][1], framesVertical[currentFrame][2], framesVertical[currentFrame][3]));
            vacuumSprite.setScale(3, -3);
            vacX=playerX;
            vacY=playerY+PLAYER_HEIGHT+(framesVertical[currentFrame][3]*3);

        }
        else if(vacuumDir==2){ //left A
            vacuumSprite.setTexture(vacuumTex);
            
            if(usingPowerFrames){
                vacuumSprite.setTextureRect(IntRect(framesHorizontalPower[currentFrame][0], framesHorizontalPower[currentFrame][1], framesHorizontalPower[currentFrame][2], framesHorizontalPower[currentFrame][3]));
                vacuumSprite.setScale(3, 3);
                vacX=playerX-(framesHorizontalPower[currentFrame][2]*3);
            }
            else if(usingRangeFrames){
                vacuumSprite.setTextureRect(IntRect(framesHorizontalRange[currentFrame][0], framesHorizontalRange[currentFrame][1],framesHorizontalRange[currentFrame][2], framesHorizontalRange[currentFrame][3]));
                vacuumSprite.setScale(3, 3);
                vacX=playerX-(framesHorizontalRange[currentFrame][2]*3);
            }
            else {
                vacuumSprite.setTextureRect(IntRect(framesHorizontal[currentFrame][0], framesHorizontal[currentFrame][1], framesHorizontal[currentFrame][2], framesHorizontal[currentFrame][3]));
                vacuumSprite.setScale(3, 3);
                vacX=playerX-(framesHorizontal[currentFrame][2]*3);
            }
            vacY=playerY+PLAYER_HEIGHT/3;
        }
        else if(vacuumDir==3){ //right D
            vacuumSprite.setTexture(vacuumTex);
            
            if(usingPowerFrames){
                vacuumSprite.setTextureRect(IntRect(framesHorizontalPower[currentFrame][0],  framesHorizontalPower[currentFrame][1],   framesHorizontalPower[currentFrame][2],  framesHorizontalPower[currentFrame][3]));
                vacuumSprite.setScale(-3, 3);
                vacX=playerX+PLAYER_WIDTH+(framesHorizontalPower[currentFrame][2]*3);
            }
            else if(usingRangeFrames){
                vacuumSprite.setTextureRect(IntRect(framesHorizontalRange[currentFrame][0],  framesHorizontalRange[currentFrame][1],   framesHorizontalRange[currentFrame][2],  framesHorizontalRange[currentFrame][3]));
                vacuumSprite.setScale(-3, 3);
                vacX=playerX+PLAYER_WIDTH+(framesHorizontalRange[currentFrame][2]*3);
            }
            else {
                vacuumSprite.setTextureRect(IntRect(framesHorizontal[currentFrame][0],  framesHorizontal[currentFrame][1],   framesHorizontal[currentFrame][2],  framesHorizontal[currentFrame][3]));
                vacuumSprite.setScale(-3, 3);
                vacX=playerX+PLAYER_WIDTH+(framesHorizontal[currentFrame][2]*3);
            }
            vacY=playerY+PLAYER_HEIGHT/3;
        }
    }
    
    vacuumSprite.setPosition(vacX, vacY);
}

void spawnPowerup(int**& powerups, int& powerupCount, int x, int y, int type, int& powerupCapacity){
    const int MAX_POWERUPS=10;
    const int POWERUP_TOTAL_PROPERTIES=6;

    const int POWER_X=0;
    const int POWER_Y=1;
    const int POWER_TYPE=2;
    const int POWER_STATE=3;
    const int POWER_TIMER=4;
    const int POWER_VY=5;

    if(powerupCount>=MAX_POWERUPS)return;
    increaseSize(powerups, powerupCapacity, POWERUP_TOTAL_PROPERTIES);
    
    powerups[powerupCount][POWER_X]=x;
    powerups[powerupCount][POWER_Y]=y;
    powerups[powerupCount][POWER_TYPE]=type;
    powerups[powerupCount][POWER_STATE]=0;
    powerups[powerupCount][POWER_TIMER]=0;
    powerups[powerupCount][POWER_VY]=0;
    powerupCount++;
}

void updateVacuumDirection(int& vacuumDir, float& vacuumAngle, int selectedCharacter){   

    const float VACCUM_ROTATE_SPEED=0.025f;
    const int YELLOW_TUMBLEPOPER=0;
    const int GREEN_TUMBLEPOPER=1;

    if(selectedCharacter==GREEN_TUMBLEPOPER){ //yellow 360° rotate
        if(Keyboard::isKeyPressed(Keyboard::W))vacuumAngle+=VACCUM_ROTATE_SPEED;
        if(Keyboard::isKeyPressed(Keyboard::S))vacuumAngle-=VACCUM_ROTATE_SPEED;
        if(Keyboard::isKeyPressed(Keyboard::A))vacuumAngle+=VACCUM_ROTATE_SPEED;
        if(Keyboard::isKeyPressed(Keyboard::D))vacuumAngle-=VACCUM_ROTATE_SPEED;
    
        // Keep angle in range 0 to 2 pi
        while(vacuumAngle<0)vacuumAngle+=2*3.14159f;
        while(vacuumAngle>2*3.14159f)vacuumAngle-=2*3.14159f;
        vacuumAngle=(vacuumAngle<0)?0:(vacuumAngle>2*3.14159f)?2*3.14159f:vacuumAngle;
        
        //convert angle to up, down, left, right 
        if(vacuumAngle<3.14159f/4||vacuumAngle>=7*3.14159f/4)vacuumDir=3; //right
        else if(vacuumAngle<3*3.14159f/4)vacuumDir=0; //up
        else if(vacuumAngle<5*3.14159f/4)vacuumDir=2; //left
        else vacuumDir=1; //down
    }else{
        if(Keyboard::isKeyPressed(Keyboard::W))vacuumDir=0;
        else if(Keyboard::isKeyPressed(Keyboard::S))vacuumDir=1;
        else if(Keyboard::isKeyPressed(Keyboard::A))vacuumDir=2;
        else if(Keyboard::isKeyPressed(Keyboard::D))vacuumDir=3;
    }

}



void updateStunnedEnemies(int**& enemies, int enemyCount){
    const int ENEMIES_STATE=5;
    const int ENEMIES_STUN_TIMER=20;
    const int STATE_ACTIVE=0;
    const int STATE_STUNNED=2;

    for(int i=0;i<enemyCount;i++){
        if(enemies[i][ENEMIES_STATE]==STATE_STUNNED){
            enemies[i][ENEMIES_STUN_TIMER]--;
            
            if(enemies[i][ENEMIES_STUN_TIMER]<=0){
                //stun reactivate enemy
                enemies[i][ENEMIES_STATE]=STATE_ACTIVE;
            }
        }
    }
}

void addCapturedEnemy(int*& capturedQueue, int& captureCount){
    int* newcapturedQueue=new int[captureCount+1];
    
    //copy old 
    for(int i=0;i<captureCount;i++){
        newcapturedQueue[i]=capturedQueue[i];
    }    
    //delete old array
    delete[] capturedQueue;    
    //update pointer
    capturedQueue=newcapturedQueue;
}

void removeCapturedEnemy(int*& capturedQueue, int& captureCount){

    int* newcapturedQueue=new int[captureCount];
    for(int i=0;i<captureCount;i++){
        newcapturedQueue[i]=capturedQueue[i];
    }
    delete[] capturedQueue;
    capturedQueue=newcapturedQueue;
    

}
void removeEnemy(int**& enemies, int& enemyCount, int& enemyCapacity, int index){
    const int ENEMY_TOTAL_PROPERTIES=22;
    //swap the enemy with the enemy
    for(int j=0;j<ENEMY_TOTAL_PROPERTIES;j++){
        enemies[index][j]=enemies[enemyCount-1][j];
    }
    
    enemyCount--;
    decreaseSize(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
}

void captureEnemies(int**& enemies, int& enemyCount, int& enemyCapacity, float playerX, float playerY, int vacuumDir, int selectedCharacter, int*& capturedQueue, int& captureCount, int maxCapacity, int& score, int* activePowerups, int**& powerups, int& powerupCount, int& powerupCapacity, float vacuumAngle){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    const int TYPE_MINION=4;
    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;

    if(Keyboard::isKeyPressed(Keyboard::Space)){
        for(int i=enemyCount-1;i>=0;i--){
            if(enemies[i][ENEMIES_TYPE]== TYPE_MINION&&(enemies[i][ENEMIES_FLAG2]==1||enemies[i][ENEMIES_FLAG3]==1))return;
            if(enemies[i][ENEMIES_STATE]==STATE_ACTIVE||enemies[i][ENEMIES_STATE]==STATE_STUNNED ){

                if(isInVacuumRange(enemies[i][ENEMIES_X], enemies[i][ENEMIES_Y], playerX, playerY, vacuumDir, selectedCharacter, enemies[i][ENEMIES_TYPE], activePowerups, vacuumAngle)){
                    
                    //vaccum timer
                    enemies[i][ENEMIES_FLAG6]++;
                    
                    //stun it on first frame
                    if(enemies[i][ENEMIES_FLAG6]==1&&enemies[i][ENEMIES_STATE]==STATE_ACTIVE){
                        enemies[i][ENEMIES_STATE]=STATE_STUNNED;
                        enemies[i][ENEMIES_STUN_TIMER]=60; // 1 second stun
                        score+=getEnemyCapturePoints(enemies[i][ENEMIES_TYPE]);
                    }
                    
                    //after 10 frames capture
                    if(enemies[i][ENEMIES_FLAG6]>=10&&enemies[i][ENEMIES_STATE]==STATE_STUNNED){
                        if(captureCount<maxCapacity){

                            addCapturedEnemy(capturedQueue, captureCount);
                            capturedQueue[captureCount]=enemies[i][ENEMIES_TYPE];
                            captureCount++;
                            
                            if((rand()%10)==1){
                                spawnPowerup(powerups, powerupCount, enemies[i][ENEMIES_X], enemies[i][ENEMIES_Y], rand()% 4, powerupCapacity);
                            }
                            
                            removeEnemy(enemies, enemyCount, enemyCapacity, i);
                        }
                    }
                }
                else {
                    //not in range so reset vacuum timer
                    enemies[i][ENEMIES_FLAG6]=0;
                }
            }
        }
    }
    else {
        //space released so we reset all enemys vacuum timers
        for(int i=0;i<enemyCount;i++){
            enemies[i][ENEMIES_FLAG6]=0;
        }
    }
}


void shootSingleEnemy(int*& capturedQueue, int& captureCount, int**& projectiles, int& projCount, float playerX, float playerY, int vacuumDir, int& projCapacity, float vacuumAngle, int selectedCharacter){
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_VX=2;
    const int PROJECT_VY=3;
    const int PROJECT_DIR=4; 
    const int PROJECT_TYPE=5;
    const int PROJECT_STATE=6;
    const int PROJECT_TIMER=7;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;

    if(captureCount<=0 )return;
    
        //get last captured enemy
        int enemyType=capturedQueue[captureCount-1];
        captureCount--;
    removeCapturedEnemy(capturedQueue, captureCount);

    increaseSize(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES);
    if(selectedCharacter==1){
        projectiles[projCount][PROJECT_X]=playerX+PLAYER_WIDTH/2;
        projectiles[projCount][PROJECT_Y]=playerY+PLAYER_HEIGHT/2;
        projectiles[projCount][PROJECT_TYPE]=enemyType;
        projectiles[projCount][PROJECT_STATE]=0;
        projectiles[projCount][PROJECT_DIR]=0;
    
        //shoot in the angle direction
        float speed=10.0f;
        projectiles[projCount][PROJECT_VX]=speed*cos(vacuumAngle);
        projectiles[projCount][PROJECT_VY]=-speed*sin(vacuumAngle);     
    }
    else{
        //create projectile
        projectiles[projCount][PROJECT_X]=playerX+PLAYER_WIDTH/2;
        projectiles[projCount][PROJECT_Y]=playerY +PLAYER_HEIGHT-96;
        projectiles[projCount][PROJECT_TYPE]=enemyType;
        projectiles[projCount][PROJECT_STATE]=0; //flying state
        projectiles[projCount][PROJECT_DIR]=vacuumDir;
        
        //set velocity depending on direction
        if(vacuumDir==0){ //up
            projectiles[projCount][PROJECT_VX]=0;
            projectiles[projCount][PROJECT_VY]=-15;
        }
        else if(vacuumDir==1){ //down
            projectiles[projCount][PROJECT_VX]=0;
            projectiles[projCount][PROJECT_VY]=15;
        }
        else if(vacuumDir==2){ //left
         projectiles[projCount][PROJECT_STATE]=1; 
            projectiles[projCount][PROJECT_VX]=-8;
            projectiles[projCount][PROJECT_VY]=0;
        }
        else { //right
            projectiles[projCount][PROJECT_STATE]=1; 
            projectiles[projCount][PROJECT_VX]=8;
            projectiles[projCount][PROJECT_VY]=0;
        }
    }
        
        projCount++;
}


void shootAllEnemies(int*& capturedQueue, int& captureCount, int**& projectiles, int& projCount, float playerX, float playerY, int vacuumDir, int& projCapacity, float& vacuumAngle, int& selectedCharacter){
    while(captureCount>0){
        shootSingleEnemy(capturedQueue, captureCount, projectiles, projCount, playerX+rand()%32, playerY, vacuumDir, projCapacity, vacuumAngle, selectedCharacter);
    }
}

void updateProjectiles(int**& projectiles, int& projCount, char** lvl, int& projCapacity){
    const int MAX_PROJECTILES=100;
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_VX=2;
    const int PROJECT_VY=3;
    const int PROJECT_DIR=4; 
    const int PROJECT_TYPE=5;
    const int PROJECT_STATE=6;
    const int PROJECT_TIMER=7;

    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;


    const float GRAVITY=1;
    const float TERMINAL_VELOCITY=20;

    for(int i=0;i<projCount;i++){
        if(projectiles[i][PROJECT_STATE]==0){ //flying
            projectiles[i][PROJECT_X]+=projectiles[i][PROJECT_VX];
            projectiles[i][PROJECT_Y]+=projectiles[i][PROJECT_VY];
            
            //check collision with walls
            int checkX=projectiles[i][PROJECT_X]/CELL_SIZE;
            int checkY=projectiles[i][PROJECT_Y]/CELL_SIZE;
            
            if(checkX<0||checkX>=LEVEL_WIDTH||
               checkY<0||checkY>=LEVEL_HEIGHT ||
               lvl[checkY][checkX]=='#'){
                //start rolling
                if(projectiles[i][PROJECT_TYPE] != -1){
                    projectiles[i][PROJECT_STATE]=1;
                    projectiles[i][PROJECT_VY]=0;
                    projectiles[i][PROJECT_VX]=(projectiles[i][PROJECT_VX]>0)?8:-8;
                }
            }
        }
        else if(projectiles[i][PROJECT_STATE]==1){ //rolling
            projectiles[i][PROJECT_X]+=projectiles[i][PROJECT_VX];
            
            //apply gravity
            projectiles[i][PROJECT_VY]+=GRAVITY;
            if(projectiles[i][PROJECT_VY]>TERMINAL_VELOCITY){
                projectiles[i][PROJECT_VY]=TERMINAL_VELOCITY;
            }
            projectiles[i][PROJECT_Y]+=projectiles[i][PROJECT_VY];
            
            //check ground
            int groundX=projectiles[i][PROJECT_X]/CELL_SIZE;
            int groundY=(projectiles[i][PROJECT_Y]+96)/CELL_SIZE;
            
            if(groundY>=0&&groundY<LEVEL_HEIGHT&&
               groundX>=0&&groundX<LEVEL_WIDTH){
                if(lvl[groundY][groundX]=='#'){
                    projectiles[i][PROJECT_VY]=0;
                    projectiles[i][PROJECT_Y]=groundY*CELL_SIZE-96;
                }
            }
            
            //remove if out of bounds
            if(projectiles[i][PROJECT_X]<0||projectiles[i][PROJECT_X]>SCREEN_WIDTH){
                //swapping with last and remove
                projectiles[i][PROJECT_X]=projectiles[projCount-1][PROJECT_X];
                projectiles[i][PROJECT_Y]=projectiles[projCount-1][PROJECT_Y];
                projectiles[i][PROJECT_VX]=projectiles[projCount-1][PROJECT_VX];
                projectiles[i][PROJECT_VY]=projectiles[projCount-1][PROJECT_VY];
                projectiles[i][PROJECT_STATE]=projectiles[projCount-1][PROJECT_STATE];
                projectiles[i][PROJECT_TYPE]=projectiles[projCount-1][PROJECT_TYPE];
                projCount--;
                i--;

                decreaseSize(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES);   
            }
        }
    }
}


bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
    return ((x1<x2+w2&&x1+w1>x2&&y1<y2+h2&&y1+h1>y2));
}


void updatePowerups(int**& powerups, int& powerupCount, char** lvl){
    // Powerup array indices
    const int POWER_X=0;
    const int POWER_Y=1;
    const int POWER_TYPE=2;
    const int POWER_STATE=3;
    const int POWER_TIMER=4;
    const int POWER_VY=5;
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;

    for(int i=0;i<powerupCount;i++){
        if(powerups[i][POWER_STATE]==0){
            //apply gravity
            powerups[i][POWER_VY]+=1;
            if(powerups[i][POWER_VY]>10)powerups[i][POWER_VY]=10;
            
            int nextY=powerups[i][POWER_Y]+powerups[i][POWER_VY];
            int checkY=(nextY+32)/CELL_SIZE; // 32=powerup size
            int checkX=(powerups[i][POWER_X]+16)/CELL_SIZE;
            
            //check ground collision
            if(checkY>=0&&checkY<LEVEL_HEIGHT&&checkX>=0&&checkX<LEVEL_WIDTH){
                if(lvl[checkY][checkX]=='#'||lvl[checkY][checkX]=='/'||lvl[checkY][checkX]=='\\'){
                    powerups[i][POWER_VY]=0;
                    powerups[i][POWER_Y]=checkY*CELL_SIZE-32;
                }
                else {
                    powerups[i][POWER_Y]=nextY;
                }
            }
            else {
                powerups[i][POWER_Y]=nextY;
            }
            
        }
    }
}

void handlePowerupCollection(int**& powerups, int& powerupCount, float playerX, float playerY, int& playerHealth, int activePowerups[3], int& powerupCapacity){
    const int POWER_X=0;
    const int POWER_Y=1;
    const int POWER_TYPE=2;
    const int POWER_STATE=3;
    const int POWER_TIMER=4;
    const int POWER_VY=5;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;

    const int POWERUP_SPEED=0;
    const int POWERUP_RANGE=1;
    const int POWERUP_POWER=2;
    const int POWERUP_EXTRA_LIFE=3;

    const int POWERUP_DURATION=600; 

    const int POWER_ACTIVE_SPEED=0;
    const int POWER_ACTIVE_RANGE=1;
    const int POWER_ACTIVE_POWER=2;
    const int POWERUP_TOTAL_PROPERTIES=6;

    for(int i=0;i<powerupCount;i++){
        if(powerups[i][POWER_STATE]==0){
            //check collision with player
            if(checkCollision(powerups[i][POWER_X], powerups[i][POWER_Y], 32, 32, playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT)){
                
                int type=powerups[i][POWER_TYPE];
                
                if(type==POWERUP_SPEED){
                    activePowerups[POWER_ACTIVE_SPEED]=POWERUP_DURATION;
                }
                else if(type==POWERUP_RANGE){
                    activePowerups[POWER_ACTIVE_RANGE]=POWERUP_DURATION;
                }
                else if(type==POWERUP_POWER){
                    activePowerups[POWER_ACTIVE_POWER]=POWERUP_DURATION;
                }
                else if(type==POWERUP_EXTRA_LIFE){
                    playerHealth++;
                }
                
                //remove powerup
                for(int k=0;k<POWERUP_TOTAL_PROPERTIES;k++){
                    powerups[i][k]=powerups[powerupCount-1][k];
                }
                powerupCount--;
                i--;
                decreaseSize(powerups, powerupCapacity, POWERUP_TOTAL_PROPERTIES);
            }
        }
    }
}

void updateActivePowerups(int activePowerups[3]){
    for(int i=0;i<3;i++){
        if(activePowerups[i]>0){
            activePowerups[i]--;
        }
    }
}

void renderPowerups(RenderWindow& window, int**& powerups, int powerupCount, Sprite& powerupSprites, Texture& playerTexRight){

    const int MAX_POWERUPS=10;
    const int POWERUP_TOTAL_PROPERTIES=6;
    const int POWER_X=0;
    const int POWER_Y=1;
    const int POWER_TYPE=2;
    const int POWER_STATE=3;
    const int POWER_TIMER=4;
    const int POWER_VY=5;
    const int POWERUP_SPEED=0;
    const int POWERUP_RANGE=1;
    const int POWERUP_POWER=2;
    const int POWERUP_EXTRA_LIFE=3;
    const int POWERUP_DURATION=600;
    const int POWER_ACTIVE_SPEED=0;
    const int POWER_ACTIVE_RANGE=1;
    const int POWER_ACTIVE_POWER=2;


    static int animTimers[MAX_POWERUPS]={0};
    static int animFrames[MAX_POWERUPS]={0};
    
    for(int i=0;i<powerupCount;i++){
        if(powerups[i][POWER_STATE]==0){
            int type=powerups[i][POWER_TYPE];
            
            animTimers[i]++;
            
            powerupSprites.setTexture(playerTexRight);
            
            if(type==POWERUP_SPEED){
                if(animTimers[i]>=10){
                    animTimers[i]=0;
                    animFrames[i]++;
                    if(animFrames[i] >=4)animFrames[i]=2;
                }
                
                int frameX=1037;
                if(animFrames[i]==0)frameX=1037;
                else if(animFrames[i]==1)frameX=1037+17;
                else if(animFrames[i]==2)frameX=1037+17+17;
                else if(animFrames[i]==3)frameX=1037+17+17+33;
                
                powerupSprites.setTextureRect(IntRect(frameX, 378, (animFrames[i]==1||animFrames[i]==0)?16:32, 32));
            }
            else if(type==POWERUP_RANGE){
                if(animTimers[i]>=10){
                    animTimers[i]=0;
                    animFrames[i]++;
                    if(animFrames[i]>=7)animFrames[i]=2;
                }
                
                int frameX=669;
                if(animFrames[i]==0)frameX=669;
                else if(animFrames[i]==1)frameX=669+17;
                else if(animFrames[i]==2)frameX=669+17+33;
                else if(animFrames[i]==3)frameX=669+17+33+33;
                else if(animFrames[i]==4)frameX=669+17+33+33+33;
                else if(animFrames[i]==5)frameX=669+17+33+33+33+33;
                else if(animFrames[i]==6)frameX=669+17+33+33+33+33+33;
                
                powerupSprites.setTextureRect(IntRect(frameX, 378, (animFrames[i]==0)?16:32, 32));
            }
            else if(type==POWERUP_POWER){
                if(animTimers[i]>=10){
                    animTimers[i]=0;
                    animFrames[i]++;
                    if(animFrames[i]>=5)animFrames[i]=2;
                }
                
                int frameX=886;
                if(animFrames[i]==0)frameX=886;
                else if(animFrames[i]==1)frameX=886+17;
                else if(animFrames[i]==2)frameX=886+17+33;
                else if(animFrames[i]==3)frameX=886+17+33+33;
                else if(animFrames[i]==4)frameX=886+17+33+33+33;
                
                powerupSprites.setTextureRect(IntRect(frameX, 378, (animFrames[i]==0)?16:32, 32));
            }
            else if(type==POWERUP_EXTRA_LIFE){
                if(animTimers[i]>=10){
                    animTimers[i]=0;
                    animFrames[i]++;
                    if(animFrames[i]>=3)animFrames[i]=2;
                }
                
                int frameX=600;
                if(animFrames[i]==0)frameX=600;
                else if(animFrames[i]==1)frameX=600+17;
                else if(animFrames[i]==2)frameX=600+17+17;
                
                powerupSprites.setTextureRect(IntRect(frameX, 378, (animFrames[i]==0||animFrames[i]==1)?16:32, 32));
            }

            powerupSprites.setPosition(powerups[i][POWER_X], powerups[i][POWER_Y]);
            window.draw(powerupSprites);
        }
    }
}




void renderProducer(RenderWindow& window, int*& producer, Sprite& producerSprite, Texture& producerTex){
    const int PRODUCER_X=0;
    const int PRODUCER_Y=1;
    const int PRODUCER_ACTIVE=4;

    if(producer[PRODUCER_ACTIVE]==1){
        producerSprite.setTexture(producerTex);
        producerSprite.setPosition(producer[PRODUCER_X], producer[PRODUCER_Y]);
        window.draw(producerSprite);
    }
}




void initializeProducer(int*& producer, float cloudY){
    const int PRODUCER_X=0;
    const int PRODUCER_Y=1;
    const int PRODUCER_HEALTH=2;
    const int PRODUCER_TIMER=3;
    const int PRODUCER_ACTIVE=4;
    const int PRODUCER_MAX_HEALTH=5;
    const int PRODUCER_SPAWN_INTERVAL=180;
    const int PRODUCER_WIDTH=144;
    const int PRODUCER_HEIGHT=132;
    const int CELL_SIZE=64;
    
    producer[PRODUCER_X]=8*CELL_SIZE-7; //center on cloud
    producer[PRODUCER_Y]=cloudY-144; //on top of cloud
    producer[PRODUCER_HEALTH]=PRODUCER_MAX_HEALTH;
    producer[PRODUCER_TIMER]=0;
    producer[PRODUCER_ACTIVE]=1;
}



void updateProducer(int*& producer, int**& enemies, int& enemyCount, int& enemyCapacity, float cloudY, char** lvl){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int CELL_SIZE=64;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    const int PRODUCER_X=0;
    const int PRODUCER_Y=1;
    const int PRODUCER_HEALTH=2;
    const int PRODUCER_TIMER=3;
    const int PRODUCER_ACTIVE=4;
    const int PRODUCER_MAX_HEALTH=5;
    const int PRODUCER_SPAWN_INTERVAL=180;
    const int PRODUCER_WIDTH=144;
    const int PRODUCER_HEIGHT=132;
    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;
    
    const int STATE_INACTIVE=3;
    const int ENEMY_TOTAL_PROPERTIES=22;
    const int MAX_ENEMIES=20;


    if(producer[PRODUCER_ACTIVE]==0)return;

    //producer on top of cloud
    producer[PRODUCER_Y]=cloudY-132;
    
    //spawn enemies
    producer[PRODUCER_TIMER]++;
    if(producer[PRODUCER_TIMER]>=PRODUCER_SPAWN_INTERVAL){
        producer[PRODUCER_TIMER]=0;
        
        //spawn new enemy dynamically
        if(enemyCount<MAX_ENEMIES){
            increaseSize(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
            enemies[enemyCount][ENEMIES_X]=producer[PRODUCER_X]+ ((rand()%2)?192:(-135));//on right or left of producer
            enemies[enemyCount][ENEMIES_Y]=producer[PRODUCER_Y]+64; 
            enemies[enemyCount][ENEMIES_TYPE]=rand()%4;
            enemies[enemyCount][ENEMIES_STATE]=STATE_ACTIVE;
            enemies[enemyCount][ENEMIES_DIR_X]=(rand()%2)?1:-1;
            enemies[enemyCount][ENEMIES_DIR_Y]=1;
            enemies[enemyCount][ENEMIES_TIMER]=0;
            enemies[enemyCount][ENEMIES_FLAG1]=0;
            enemies[enemyCount][ENEMIES_FLAG2]=0;
            enemies[enemyCount][ENEMIES_FLAG3]=0;
            enemies[enemyCount][ENEMIES_FLAG6]=0;
            enemies[enemyCount][ENEMIES_JUMP]=0;
            enemies[enemyCount][ENEMIES_TOTALFRAMES]=0;
            enemies[enemyCount][ENEMIES_CURRENT]=0;
            enemies[enemyCount][ENEMIES_FLAG5]=0;
            snapEnemyToPlatform(enemies[enemyCount], lvl);
            enemyCount++;
        }
    }
}





void updateCloud(float& cloudY, int& direction, char** lvl, int& prevCloudCellY, float playerX, float& playerY, int*& producer){
    const int cloudCellX=8;
    const float CLOUD_SPEED=2.0f;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    
    //clear previous cloud position
    if(prevCloudCellY>=0&&prevCloudCellY<LEVEL_HEIGHT){
        lvl[prevCloudCellY][cloudCellX]=' ';
        lvl[prevCloudCellY][cloudCellX+1]=' ';
    }
    
    //update position
    cloudY+=direction*CLOUD_SPEED;
    
    //cap
    if(cloudY<300)direction=1;
    if(cloudY>800)direction=-1;
    
    int currentCloudCellY=int(cloudY/64);
    
    //set new cloud position
    if(currentCloudCellY>=0&&currentCloudCellY<LEVEL_HEIGHT){
        lvl[currentCloudCellY][cloudCellX]='#';
        lvl[currentCloudCellY][cloudCellX+1]='#';
        prevCloudCellY=currentCloudCellY;
    }
    
    //if player is standing on the cloud
    int playerCellX=int((playerX+PLAYER_WIDTH/2)/64);
    int playerBottomCellY=int((playerY+PLAYER_HEIGHT+1)/64);
    
    if((playerCellX==cloudCellX||playerCellX==cloudCellX+1)&&
       playerBottomCellY==currentCloudCellY){
        playerY=cloudY-PLAYER_HEIGHT-3;
    }
    
    // producer[PRODUCER_Y]=cloudY-132;
}




void handleProjectileProducerCollision(int**& projectiles, int& projCount, int*& producer, int& score, int&projCapacity){
    const int PRODUCER_X=0;
    const int PRODUCER_Y=1;
    const int PRODUCER_HEALTH=2;
    const int PRODUCER_TIMER=3;
    const int PRODUCER_ACTIVE=4;
    const int PRODUCER_MAX_HEALTH=5;
    const int PRODUCER_SPAWN_INTERVAL=180;
    const int PRODUCER_WIDTH=144;
    const int PRODUCER_HEIGHT=132;
    const int MAX_PROJECTILES=100;
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_VX=2;
    const int PROJECT_VY=3;
    const int PROJECT_DIR=4; 
    const int PROJECT_TYPE=5;
    const int PROJECT_STATE=6;
    const int PROJECT_TIMER=7;

    if(producer[PRODUCER_ACTIVE]==0)return;
    
    for(int i=0;i<projCount;i++){
        if(projectiles[i][PROJECT_TYPE]>=0){ // Player projectile
            
            //check collision with producer
            if(checkCollision(projectiles[i][PROJECT_X], projectiles[i][PROJECT_Y], 96, 96, producer[PRODUCER_X], producer[PRODUCER_Y], PRODUCER_WIDTH, PRODUCER_HEIGHT)){
                
                producer[PRODUCER_HEALTH]--;
                score+=50;
                
                //if destroyed
                if(producer[PRODUCER_HEALTH]<=0){
                    producer[PRODUCER_ACTIVE]=0;
                    score+=500; 
                }
                
                for(int k=0;k<PROJECT_TOTAL_PROPERTIES;k++){
                    projectiles[i][k]=projectiles[projCount-1][k];
                }
                projCount--;
                i--;
                decreaseSize(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES); 
            }
        }
    }
}


void handlePlayerProducerCollision(int*& producer, float& playerX, float& playerY, int& playerHealth, bool& invulnerable, int& invulnerableTimer, int& score, RenderWindow& window, int currentLevel, Font& font){

    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const int PRODUCER_X=0;
    const int PRODUCER_Y=1;
    const int PRODUCER_HEALTH=2;
    const int PRODUCER_TIMER=3;
    const int PRODUCER_ACTIVE=4;

    const int PRODUCER_MAX_HEALTH=5;
    const int PRODUCER_SPAWN_INTERVAL=180;
    const int PRODUCER_WIDTH=144;
    const int PRODUCER_HEIGHT=132;
    if(producer[PRODUCER_ACTIVE]==0)return;     

    if(invulnerable){
        invulnerableTimer++;
        if(invulnerableTimer>=240){
            invulnerable=false;
            invulnerableTimer=0;
        }
        return;
    }


    if(checkCollision(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, producer[PRODUCER_X], producer[PRODUCER_Y],PRODUCER_WIDTH, PRODUCER_HEIGHT)){                
        playerDie(playerX, playerY, playerHealth, invulnerable, invulnerableTimer, score, window, currentLevel, font);
    }
}


void handleProjectileEnemyCollision(int**& projectiles, int& projCount, int& projCapacity, int**& enemies, int& enemyCount, int& enemyCapacity, int& score, int**& powerups, int& powerupCount, int& powerupCapacity){
    const int MAX_PROJECTILES=100;
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_VX=2;
    const int PROJECT_VY=3;
    const int PROJECT_DIR=4; 
    const int PROJECT_TYPE=5;
    const int PROJECT_STATE=6;
    const int PROJECT_TIMER=7;
    const int PROJECTILE_DEFEAT_MULTIPLIER=2;
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;
    
    const int STATE_INACTIVE=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;



    for(int i=projCount-1;i>=0;i--){
        if(projectiles[i][PROJECT_TYPE]>=0){ 
            bool projectileHit=false;
            
            for(int j=enemyCount-1;j>=0;j--){
                int type=enemies[j][ENEMIES_TYPE];
                if(enemies[j][ENEMIES_STATE]==STATE_ACTIVE||enemies[j][ENEMIES_STATE]==STATE_STUNNED){
                    if(checkCollision(projectiles[i][PROJECT_X], projectiles[i][PROJECT_Y], getEnemyWidth(type), getEnemyHeight(type), enemies[j][ENEMIES_X], enemies[j][ENEMIES_Y],getEnemyWidth(type), getEnemyHeight(type))){
                        
                        score+=getEnemyCapturePoints(enemies[j][ENEMIES_TYPE])*PROJECTILE_DEFEAT_MULTIPLIER;
                        
                        if((rand()%15)==1){
                            spawnPowerup(powerups, powerupCount, enemies[j][ENEMIES_X], enemies[j][ENEMIES_Y], rand()% 4, powerupCapacity);
                        }
                        removeEnemy(enemies, enemyCount, enemyCapacity, j);
                        
                        projectileHit=true;
                        break;
                    }
                }
            }
            
            //remove projectile if it hit an enemy
            if(projectileHit){
                //swap with last projectile
                for(int k=0;k<PROJECT_TOTAL_PROPERTIES;k++){
                    projectiles[i][k]=projectiles[projCount-1][k];
                }
                projCount--;
                decreaseSize(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES);
            }
        }
    }
}
void handleProjectilePlayerCollision(int**& projectiles, int& projCount, float& playerX, float& playerY, int& playerHealth, bool& invulnerable, int& invulnerableTimer, int& score, RenderWindow& window, int currentLevel, Font& font, int& projCapacity){
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const int MAX_PROJECTILES=100;
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_VX=2;
    const int PROJECT_VY=3;
    const int PROJECT_DIR=4; 
    const int PROJECT_TYPE=5;
    const int PROJECT_STATE=6;
    const int PROJECT_TIMER=7;
    const int ENEMY_PROJECT_HEIGHT=20; 
    const int ENEMY_PROJECT_WIDTH=64;


    if(invulnerable){
        invulnerableTimer++;
        if(invulnerableTimer>=240){
            invulnerable=false;
            invulnerableTimer=0;
        }
        return;
    }

    for(int i=0;i<projCount;i++){
        if(projectiles[i][PROJECT_TYPE]==-1 ){ //enemy projectile
            if(checkCollision(projectiles[i][PROJECT_X], projectiles[i][PROJECT_Y], ENEMY_PROJECT_WIDTH, ENEMY_PROJECT_HEIGHT, playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT)){

                playerDie(playerX, playerY, playerHealth, invulnerable, invulnerableTimer, score, window, currentLevel, font);
                projectiles[i][PROJECT_X]=projectiles[projCount-1][PROJECT_X];
                projectiles[i][PROJECT_Y]=projectiles[projCount-1][PROJECT_Y];
                projectiles[i][PROJECT_VX]=projectiles[projCount-1][PROJECT_VX];
                projectiles[i][PROJECT_VY]=projectiles[projCount-1][PROJECT_VY];
                projectiles[i][PROJECT_STATE]=projectiles[projCount-1][PROJECT_STATE];
                projectiles[i][PROJECT_TYPE]=projectiles[projCount-1][PROJECT_TYPE];
                projCount--;
                i--;
                decreaseSize(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES); 
            }
        }
    }
}


void handlePlayerEnemyCollision(int**& enemies, int enemyCount,float& playerX, float& playerY, int& playerHealth, bool& invulnerable, int& invulnerableTimer, int& score, RenderWindow& window, int currentLevel, Font& font){
    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;
    
    const int STATE_INACTIVE=3;
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;

    if(invulnerable){
        invulnerableTimer++;
        if(invulnerableTimer>=240){
            invulnerable=false;
            invulnerableTimer=0;
        }
        return;
    }
    
    for(int i=0;i<enemyCount;i++){
        int type=enemies[i][ENEMIES_TYPE];
        if(enemies[i][ENEMIES_STATE]==STATE_ACTIVE|| enemies[i][ENEMIES_STATE]==STATE_STUNNED){
            if(checkCollision(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, enemies[i][ENEMIES_X], enemies[i][ENEMIES_Y], getEnemyWidth(type), getEnemyHeight(type))){                
                playerDie(playerX, playerY, playerHealth, invulnerable, invulnerableTimer, score, window, currentLevel, font);
            }
        }
    }
}




void addTentacle(int**& tentacles, int& count, int& capacity, int targetX, int targetY){

    const int TENTACLE_STATE_RISING=0;
    const int TENTACLE_STATE_ACTIVE=1;
    const int TENTACLE_STATE_SINKING=2;
    const int TENTACLE_TARGET_Y=7;
    const int TENTACLE_STATE=6; 
    const int TOTAL_TENTACLES=3;     
    const int TENTACLES_GAP=192;
    const int TENTACLES_WIDTH=128;
    const int TENTACLES_HEIGHT=256;
    const int TENTACLE_X=0;
    const int TENTACLE_Y=1;
    const int TENTACLE_TIMER=2;
    const int TENTACLE_WIDTH=3;
    const int TENTACLE_HEIGHT=4;

    if(count>=TOTAL_TENTACLES)return;
    
    //tentacle gaps
    for(int i=0;i<count;i++){
        if(abs(tentacles[i][TENTACLE_X]-targetX)<TENTACLES_GAP){
            return; 
        }
    }
    
    increaseSize(tentacles, capacity, 7);
    tentacles[count][TENTACLE_X]=targetX;
    tentacles[count][TENTACLE_Y]=SCREEN_HEIGHT; 
    tentacles[count][TENTACLE_TARGET_Y]=targetY; //where it should rise to
    tentacles[count][TENTACLE_TIMER]=0;
    tentacles[count][TENTACLE_WIDTH]=TENTACLES_WIDTH;
    tentacles[count][TENTACLE_HEIGHT]=0; 
    tentacles[count][TENTACLE_STATE]=TENTACLE_STATE_RISING;
    
    count++;
}

void updateTentacles(int**& tentacles, int& count, int& capacity){

    const int TENTACLE_STATE_RISING=0;
    const int TENTACLE_STATE_ACTIVE=1;
    const int TENTACLE_STATE_SINKING=2;
    const int TENTACLE_TARGET_Y=7;
    const int TENTACLE_STATE=6; 
    const int TOTAL_TENTACLES=3;     
    const int TENTACLES_GAP=192;
    const int TENTACLES_WIDTH=128;
    const int TENTACLES_HEIGHT=256;
    const int TENTACLE_X=0;
    const int TENTACLE_Y=1;
    const int TENTACLE_TIMER=2;
    const int TENTACLE_WIDTH=3;
    const int TENTACLE_HEIGHT=4;

    for(int i=count-1;i>=0;i--){
        int state=tentacles[i][TENTACLE_STATE];
        
        if(state==TENTACLE_STATE_RISING){
            tentacles[i][TENTACLE_Y]-=4; //rise 
            
            int bottomY=SCREEN_HEIGHT;
            int currentTop=tentacles[i][TENTACLE_Y];

            tentacles[i][TENTACLE_HEIGHT]=bottomY-currentTop;
            
            
            //if reached target height
            if(tentacles[i][TENTACLE_Y]<=tentacles[i][TENTACLE_TARGET_Y]){
                tentacles[i][TENTACLE_Y]=tentacles[i][TENTACLE_TARGET_Y];
                tentacles[i][TENTACLE_HEIGHT]=TENTACLES_HEIGHT;
                tentacles[i][TENTACLE_STATE]=TENTACLE_STATE_ACTIVE;
                tentacles[i][TENTACLE_TIMER]=120; 
            }
        }
        else if(state==TENTACLE_STATE_ACTIVE){
            tentacles[i][TENTACLE_TIMER]--;
            
            if(tentacles[i][TENTACLE_TIMER]<=0){
                tentacles[i][TENTACLE_STATE]=TENTACLE_STATE_SINKING;
            }
        }
        else if(state==TENTACLE_STATE_SINKING){
            tentacles[i][TENTACLE_Y]+=4; //sink 
            
            int bottomY=SCREEN_HEIGHT;
            //tentacles[i][TENTACLE_HEIGHT]=bottomY-tentacles[i][TENTACLE_Y];
     
            
            //if full down remove tentacle
            if(tentacles[i][TENTACLE_Y]>=SCREEN_HEIGHT){
                
                for(int j=0;j<7;j++){
                    tentacles[i][j]=tentacles[count-1][j];
                }
                count--;
                decreaseSize(tentacles, capacity, 7);
                i--;
            }
        }
    }
}



void spawnBossMinion(int**& minions, int& minionCount, int& minionCapacity, int bossX, int bossY){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;
    
    const int STATE_INACTIVE=3;
    const int MAX_ENEMIES=20;
    const int ENEMY_TOTAL_PROPERTIES=22;



    increaseSize(minions, minionCapacity, ENEMY_TOTAL_PROPERTIES);

    //initialize minion
    minions[minionCount][ENEMIES_X]=bossX;
    minions[minionCount][ENEMIES_Y]=SCREEN_HEIGHT;
    minions[minionCount][ENEMIES_TYPE]=TYPE_MINION;
    minions[minionCount][ENEMIES_STATE]=STATE_ACTIVE;
    minions[minionCount][ENEMIES_DIR_X]=(rand()%2)? 1 : -1;
    minions[minionCount][ENEMIES_DIR_Y]=-1;
    minions[minionCount][ENEMIES_TIMER]=0;
    minions[minionCount][ENEMIES_FLAG1]=1;
    minions[minionCount][ENEMIES_FLAG2]=0;
    minions[minionCount][ENEMIES_FLAG3]=0;
    minions[minionCount][ENEMIES_FLAG6]=0;
    minions[minionCount][ENEMIES_JUMP]=0;
    minions[minionCount][ENEMIES_TOTALFRAMES]=0;
    minions[minionCount][ENEMIES_CURRENT]=0;
    minions[minionCount][ENEMIES_FLAG5]=0;
    minions[minionCount][ENEMIES_SPAWN_TIMER]=0;
    minions[minionCount][ENEMIES_STUN_TIMER]=0;
    
    //calculate target platform
    int targetPlatform=4+(rand()%3)*3; // 4/7/9
    minions[minionCount][ENEMIES_TARGET_Y]=targetPlatform*CELL_SIZE-128;
    
    minionCount++;
}

void updateBossMovement(int& bossX, int& bossY, int& bossHDirection,  int& bossVDirection, int& bossAtTopTimer){
    const int BOSS_MOVING_LEFT=0;
    const int BOSS_MOVING_RIGHT=1;
    const int BOSS_MOVING_UP=2;
    const int BOSS_MOVING_DOWN=3;
    const int BOSS_MAX_HEALTH=10;
    const int BOSS_WIDTH=384;
    const int BOSS_HEIGHT=384;
    const int MINION_SPAWN_WAIT=180;
    const int BOSS_ANGRY_ATHEALTH=2;
    const int BOSS_LEFT_BORDER=128;
    const int BOSS_RIGHT_BORDER=SCREEN_WIDTH-200;
    const int BOSS_TOP_BORDER=SCREEN_HEIGHT/2;
    const int BOSS_BOTTOM_BORDER=SCREEN_HEIGHT-BOSS_HEIGHT/3 ;




    //left right movment
    if(bossHDirection==BOSS_MOVING_LEFT){
        bossX-=1; //move leftsa
        
        // deflect
        if(bossX<=BOSS_LEFT_BORDER){
            bossX=BOSS_LEFT_BORDER;
            bossHDirection=BOSS_MOVING_RIGHT;
        }
    }
    else if(bossHDirection==BOSS_MOVING_RIGHT){
        bossX+=1; 
        
        if(bossX>=BOSS_RIGHT_BORDER-BOSS_WIDTH){
            bossX=BOSS_RIGHT_BORDER-BOSS_WIDTH;
            bossHDirection=BOSS_MOVING_LEFT;
        }
    }
    
    //up down movement
    if(bossVDirection==BOSS_MOVING_UP){
        bossY-=1; //move up slowly
        
        //reached top border
        if(bossY<=BOSS_TOP_BORDER){
            bossY=BOSS_TOP_BORDER;
            bossVDirection=-1; //special state: "at top, waiting"
            bossAtTopTimer=60+rand()%120; //wait 1-3 seconds
        }
    }
    else if(bossVDirection==-1){
        //wait at top
        bossAtTopTimer--;
        
        //move down start
        if(bossAtTopTimer<=0||rand()%200==0){
            bossVDirection=BOSS_MOVING_DOWN;
        }
    }
    //down
    else if(bossVDirection==BOSS_MOVING_DOWN){
        bossY+=2; 
        
        // deflect from bottom
        if(bossY>=BOSS_BOTTOM_BORDER){
            bossY=BOSS_BOTTOM_BORDER;
            bossVDirection=BOSS_MOVING_UP;
        }
    }
}

void updateBoss(int& bossX, int& bossY, int& bossHealth, bool& bossAngry,int**& tentacles, int& tentacleCount, int& tentacleCapacity,int**& minions, int& minionCount, int& minionCapacity, int& spawnTimer, int& bossHDirection, int& bossVDirection, int& bossAtTopTimer){

    const int TOTAL_TENTACLES=3;     
    const int TENTACLES_GAP=192;
    const int TENTACLES_WIDTH=128;
    const int TENTACLES_HEIGHT=256;
    const int BOSS_MAX_HEALTH=10;
    const int BOSS_WIDTH=384;
    const int BOSS_HEIGHT=384;
    const int MINION_SPAWN_WAIT=180;
    const int BOSS_ANGRY_ATHEALTH=2;


    updateBossMovement(bossX, bossY, bossHDirection, bossVDirection, bossAtTopTimer);
    
    //check angry state
    if(bossHealth<=BOSS_ANGRY_ATHEALTH&&!bossAngry){
        bossAngry=true;
    }
    
    //spawn tentacles with spacing
    int tentacleSpawnChance=bossAngry ? 60 : 120;
    if(tentacleCount<TOTAL_TENTACLES &&rand()% tentacleSpawnChance==0){
        //random X position with dist from edges
        int randX=200+rand()%(SCREEN_WIDTH-400);
        
        // random height between 20 - 60 percent
        int minHeight=SCREEN_HEIGHT*0.2f;
        int maxHeight=SCREEN_HEIGHT*0.6f;
        int randTargetY=minHeight+rand()%(maxHeight-minHeight);
        //int randTargetY=500 ;
        
        addTentacle(tentacles, tentacleCount, tentacleCapacity, randX, randTargetY);
    }
    
    //update ents
    updateTentacles(tentacles, tentacleCount, tentacleCapacity);
    
    //spawn minions
    spawnTimer++;
    
    if(spawnTimer>=600&&minionCount<7){
        spawnTimer=0;
        
        int batchSize=(1+rand()%3);
        for(int i=0;i<batchSize;i++){
            int random=(rand()%(384))+32;
            if(rand()%2==1)spawnBossMinion(minions, minionCount, minionCapacity, random, bossY);
            else spawnBossMinion(minions, minionCount, minionCapacity, random+704, bossY);
        }
    }
}

void handleProjectileBossCollision(int**& projectiles, int& projCount, int& bossHealth, int bossX, int bossY, int& projCapacity){
    const int BOSS_MAX_HEALTH=10;
    const int BOSS_WIDTH=384;
    const int BOSS_HEIGHT=384;
    const int MINION_SPAWN_WAIT=180;
    const int BOSS_ANGRY_ATHEALTH=2;
    const int MAX_PROJECTILES=100;
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_VX=2;
    const int PROJECT_VY=3;
    const int PROJECT_DIR=4; 
    const int PROJECT_TYPE=5;
    const int PROJECT_STATE=6;
    const int PROJECT_TIMER=7;

    for(int i=0;i<projCount;i++){
        if(projectiles[i][PROJECT_TYPE]>=0){ //if player projectile
            
            //check collision with boss head
            if(checkCollision(projectiles[i][PROJECT_X], projectiles[i][PROJECT_Y], 64, 64, bossX, bossY, BOSS_WIDTH, BOSS_HEIGHT)){
                
                //damage boss
                bossHealth--;
                
                //remove projectile
                for(int k=0; k<PROJECT_TOTAL_PROPERTIES; k++){
                    projectiles[i][k]=projectiles[projCount-1][k];
                }
                projCount--;
                i--;
                decreaseSize(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES); 
            }
        }
    }
}

void handlePlayerBossCollision(float& playerX, float& playerY, int& playerHealth,bool& invulnerable, int& invulnerableTimer,int bossX, int bossY, int**& tentacles, int tentacleCount, int**& minions, int minionCount, int& score, RenderWindow& window, int currentLevel, Font& font){
    const int TENTACLE_X=0;
    const int TENTACLE_Y=1;
    const int TENTACLE_TIMER=2;
    const int TENTACLE_WIDTH=3;
    const int TENTACLE_HEIGHT=4;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    const int BOSS_MAX_HEALTH=10;
    const int BOSS_WIDTH=384;
    const int BOSS_HEIGHT=384;
    const int MINION_SPAWN_WAIT=180;
    const int BOSS_ANGRY_ATHEALTH=2;
    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;
    
    const int STATE_INACTIVE=3;



    if(invulnerable){
        invulnerableTimer++;
        if(invulnerableTimer>=240){
            invulnerable=false;
            invulnerableTimer=0;
        }
        return;
    }
   

    //check boss body collision
    if(checkCollision(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT,   bossX, bossY, BOSS_WIDTH, BOSS_HEIGHT)){
        playerDie(playerX, playerY, playerHealth, invulnerable, invulnerableTimer, score, window, currentLevel, font);
        return;
    }
    
    //check boss tentacle collision
    for(int i=0;i<tentacleCount;i++){
        int tentX=tentacles[i][TENTACLE_X];
        int tentY=tentacles[i][TENTACLE_Y];
        int tentW=tentacles[i][TENTACLE_WIDTH];
        int tentH=tentacles[i][TENTACLE_HEIGHT];
        
        if(checkCollision(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT,tentX, tentY, tentW, SCREEN_HEIGHT- tentH)){
            playerDie(playerX, playerY, playerHealth, invulnerable, invulnerableTimer, score, window, currentLevel, font);
            return;
        }
        
    }
    
    //check minion collisions
    for(int i=0;i<minionCount;i++){
        if(minions[i][ENEMIES_STATE]==STATE_ACTIVE){
            int type=minions[i][ENEMIES_TYPE];
            if(checkCollision(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, minions[i][ENEMIES_X], minions[i][ENEMIES_Y], getEnemyWidth(type), getEnemyHeight(type))){
                playerDie(playerX, playerY, playerHealth, invulnerable, invulnerableTimer, score, window, currentLevel, font);
                return;
            }
        }
    }
}

void handleProjectileTentacleRespawn(int**& projectiles, int& projCount,int** tentacles, int tentacleCount, int**& minions, int& minionCount, int& minionCapacity, int& projCapacity, char** lvl){
    const int MAX_PROJECTILES=100;
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_VX=2;
    const int PROJECT_VY=3;
    const int PROJECT_DIR=4; 
    const int PROJECT_TYPE=5;
    const int PROJECT_STATE=6;
    const int PROJECT_TIMER=7;
    const int TENTACLE_X=0;
    const int TENTACLE_Y=1;
    const int TENTACLE_TIMER=2;
    const int TENTACLE_WIDTH=3;
    const int TENTACLE_HEIGHT=4;
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;



    for(int i=0;i<projCount;i++){
        if(projectiles[i][PROJECT_TYPE]>=0&&projectiles[i][PROJECT_STATE]==1){ //rolling projectile
            
            for(int t=0; t<tentacleCount; t++){
                if(checkCollision(projectiles[i][PROJECT_X], projectiles[i][PROJECT_Y],64, 64, tentacles[t][TENTACLE_X], tentacles[t][TENTACLE_Y],tentacles[t][TENTACLE_WIDTH], SCREEN_HEIGHT-tentacles[t][TENTACLE_HEIGHT])){
                    
                    //respawn minion at tentacle location
                    spawnBossMinion(minions, minionCount, minionCapacity, tentacles[t][TENTACLE_X], tentacles[t][TENTACLE_Y]);
                    minions[minionCount-1][ENEMIES_FLAG1]=0;                    
                    minions[minionCount-1][ENEMIES_TARGET_Y]=projectiles[i][PROJECT_Y];
                    minions[minionCount-1][ENEMIES_Y]=projectiles[i][PROJECT_Y];
                    snapEnemyToPlatform(minions[minionCount-1], lvl);

                    
                    //remove projectile
                    for(int k=0; k<PROJECT_TOTAL_PROPERTIES; k++){
                        projectiles[i][k]=projectiles[projCount-1][k];
                    }
                    projCount--;
                    i--;
                    decreaseSize(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES); 
                    break;
                }
            }
        }
    }
}


void renderUI(RenderWindow& window, int score, int health,  int captureCount, Font& font){
    //score text
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: "+to_string(score));
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(80, 20);
    window.draw(scoreText);
    
    // Health text
    Text healthText;
    //convert to string
    healthText.setFont(font);
    char healthStr[20] = "Health: ";
    healthStr[8] = (char)(health + '0');
    healthStr[9] = '\0'; 
    healthText.setString(healthStr);
    healthText.setCharacterSize(30);
    healthText.setFillColor(Color::Red);
    healthText.setPosition(80, 60);
    window.draw(healthText);
    
    //captured enemies count
    Text captureText;
    captureText.setFont(font);
    //convert to string
    char captureStr[20] = "Captured: ";
    captureStr[10] = (char)(captureCount + '0');
    captureStr[11] = '\0'; 
    captureText.setString(captureStr);
    captureText.setCharacterSize(30);
    captureText.setFillColor(Color::Yellow);
    captureText.setPosition(80, 100);
    window.draw(captureText);
}

void renderLevel(RenderWindow& window, char** lvl, Sprite& blockSprite, Texture blockTex[]){
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;

    for(int i=0;i<LEVEL_HEIGHT;i++){
        for(int j=0; j<LEVEL_WIDTH; j++){
            if(lvl[i][j]=='#'||lvl[i][j]=='/'|| lvl[i][j]=='\\'){
                blockSprite.setPosition(j*CELL_SIZE, i*CELL_SIZE);
                //bottom
                if(i==LEVEL_HEIGHT-1)blockSprite.setTexture(blockTex[2]);
                //walls
                else if(j==0||j==LEVEL_WIDTH-1)blockSprite.setTexture(blockTex[1]);
                //other platforms
                else blockSprite.setTexture(blockTex[0]);
                window.draw(blockSprite);
            }
        }
    }
}


void renderEnemies(RenderWindow& window, int**& enemies, int enemyCount, Sprite& enemySprite, Texture enemyTextures[]){
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;
    const int ENEMIES_DIR_X=2;
    const int ENEMIES_DIR_Y=3;
    const int ENEMIES_TYPE=4;
    const int ENEMIES_STATE=5;
    const int ENEMIES_TIMER=6;
    const int ENEMIES_FLAG1=7;
    const int ENEMIES_FLAG2=8;
    const int ENEMIES_FLAG3=9;
    const int ENEMIES_JUMP=10;
    const int ENEMIES_TARGET_Y=11;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_VISIBLE=13;
    const int ENEMIES_SHOOT_TIMER=14;
    const int ENEMIES_FLAG4=15;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;
    const int ENEMIES_FLAG5=18;
    const int ENEMIES_SPAWN_TIMER=19;
    const int ENEMIES_STUN_TIMER=20;
    const int ENEMIES_FLAG6=21;
    const int TYPE_GHOST=0;
    const int TYPE_SKELETON=1;
    const int TYPE_INVISIBLE=2;
    const int TYPE_CHELNOV=3;
    const int TYPE_MINION=4;
    const int STATE_ACTIVE=0;
    const int STATE_CAPTURED=1;
    const int STATE_STUNNED=2;
    
    const int STATE_INACTIVE=3;

    for(int i=0;i<enemyCount;i++){
        if(enemies[i][ENEMIES_STATE]==STATE_ACTIVE||((enemies[i][ENEMIES_STATE]==STATE_STUNNED)&&(enemies[i][ENEMIES_STUN_TIMER]%2==1))){
            //dont render if invisible
            if(((enemies[i][ENEMIES_TYPE]==TYPE_INVISIBLE &&enemies[i][ENEMIES_TIMER] >60&&enemies[i][ENEMIES_TIMER]<120)&&enemies[i][ENEMIES_VISIBLE]==0 )||enemies[i][ENEMIES_TYPE]==TYPE_MINION ){
                continue;
            }
            
            updateEnemyAnimation(i, enemies, enemyCount, enemySprite, enemyTextures);
            enemySprite.setPosition(enemies[i][ENEMIES_X], enemies[i][ENEMIES_Y]);
            window.draw(enemySprite);
        }
    }
}


void renderProjectiles(RenderWindow& window, int**& projectiles, int projCount, Sprite& projSprite, Texture& projTex, Texture enemyTextures[], Texture& minRevTex){
    const int PROJECT_X=0;
    const int PROJECT_Y=1;
    const int PROJECT_TYPE=5;

    static int enemyProjTimer=0;

    enemyProjTimer++;
    
    if(enemyProjTimer>3){
        enemyProjTimer=0;
    }

    for(int i=0;i<projCount;i++){
        if(projectiles[i][PROJECT_TYPE]==-1){
            //enemy projectile 
            projSprite.setTexture(projTex);
            switch(enemyProjTimer){
                case 0:
                    projSprite.setTextureRect(IntRect(233, 61, 13, 13));
                    break;
                case 1:
                    projSprite.setTextureRect(IntRect(249, 61, 13, 13));
                    break;
                case 2:
                    projSprite.setTextureRect(IntRect(233, 77, 13, 13));
                    break;
                case 3:
                    projSprite.setTextureRect(IntRect(249, 77, 13, 13));
                    break;
            }
        } else {
            // Player projectile
            updateProjectileAnimation(i, projectiles, projSprite, enemyTextures, minRevTex);
        }
        projSprite.setPosition(projectiles[i][PROJECT_X], projectiles[i][PROJECT_Y]);
        window.draw(projSprite);
    }
}




void updateBossMinionAnimation(int i, int**& minions, int minionCount, Sprite& minionSprite, Texture& min1LTex, Texture& min1RTex, Texture& min2LTex, Texture& min2RTex, Texture& minRevTex){

    const int ENEMIES_DIR_X=2;
    const int ENEMIES_STATE=5;
    const int ENEMIES_FRAME=12;
    const int ENEMIES_TOTALFRAMES=16;
    const int ENEMIES_CURRENT=17;

    const int STATE_ACTIVE=0;

    if(minions[i][ENEMIES_STATE] != STATE_ACTIVE)return;
    
    minions[i][ENEMIES_TOTALFRAMES]++;
    
    int currentFrame=minions[i][ENEMIES_FRAME];
    int direction=minions[i][ENEMIES_DIR_X];
    
    //climb/spawn
    if(currentFrame==4){
        minionSprite.setTexture(minRevTex);
        
        if(minions[i][ENEMIES_TOTALFRAMES]>=5){
            minions[i][ENEMIES_TOTALFRAMES]=0;
            minions[i][ENEMIES_CURRENT]++;
            if(minions[i][ENEMIES_CURRENT]>7)minions[i][ENEMIES_CURRENT]=0;
        }
        
        int frameX=minions[i][ENEMIES_CURRENT]*32;
        minionSprite.setScale(2, 2);
    }
    //walking
    else if(currentFrame==1){
        if(minions[i][ENEMIES_TOTALFRAMES]>=10){
            minions[i][ENEMIES_TOTALFRAMES]=0;
            minions[i][ENEMIES_CURRENT]++ ;
            if(minions[i][ENEMIES_CURRENT]>1)minions[i][ENEMIES_CURRENT]=0;
        }
        
        if(direction==-1){ //moving left
            if(minions[i][ENEMIES_CURRENT]==0){
                minionSprite.setTexture(min1LTex);
            } else {
                minionSprite.setTexture(min2LTex);
            }
        }
        else { //moving right
            if(minions[i][ENEMIES_CURRENT]==0){
                minionSprite.setTexture(min1RTex);
            } else {
                minionSprite.setTexture(min2RTex);
            }
        }
        
        minionSprite.setScale(2, 2);
    }
    //idle  is it even a thingy lol
    else if(currentFrame==3){
        if(direction==-1){
            minionSprite.setTexture(min1LTex);
        } else {
            minionSprite.setTexture(min1RTex);
        }
        
        minionSprite.setScale(2, 2);
    }
}



void renderBoss(RenderWindow& window, int bossX, int bossY, int bossHealth, bool bossAngry, Sprite& bossSprite, Texture& bossTex){
    bossSprite.setTexture(bossTex);
    bossSprite.setPosition(bossX, bossY);
    bossSprite.setScale(7, 7); //scale up the octopus bossy
    
    //color change when angry
    if(bossAngry){
        bossSprite.setColor(Color(255, 100, 100)); //reddish eyes
    }
    else {
        bossSprite.setColor(Color::White);
    }
    
    window.draw(bossSprite);
    
}


void renderTentacles(RenderWindow& window, int** tentacles, int tentacleCount, Sprite& tentacleSprite, Texture& tentacleTex){
    const int TENTACLE_X=0;
    const int TENTACLE_Y=1;
    for(int i=0;i<tentacleCount;i++){
            tentacleSprite.setTexture(tentacleTex);
            
            tentacleSprite.setPosition(tentacles[i][TENTACLE_X], tentacles[i][TENTACLE_Y]);
            tentacleSprite.setScale(3, 5); //scale up to 3x5 :((((\--/))))): 5 height btw
            window.draw(tentacleSprite);
        // }
    }
}


void renderBossMinions(RenderWindow& window, int** minions, int minionCount, Sprite& minionSprite, Texture& min1LTex, Texture& min1RTex, Texture& min2LTex, Texture& min2RTex, Texture& minRevTex){
    const int ENEMIES_STATE=5;
    const int ENEMIES_STUN_TIMER=20;
    const int STATE_STUNNED=2;
    const int STATE_ACTIVE=0;
    const int ENEMIES_X=0;
    const int ENEMIES_Y=1;

    for(int i=0;i<minionCount;i++){
        if(minions[i][ENEMIES_STATE]==STATE_ACTIVE||((minions[i][ENEMIES_STATE]==STATE_STUNNED)&&(minions[i][ENEMIES_STUN_TIMER]%2==1))){
            //update and render animation of minion kidzz
            updateBossMinionAnimation(i, minions, minionCount, minionSprite, min1LTex, min1RTex, min2LTex, min2RTex, minRevTex);
            minionSprite.setPosition(minions[i][ENEMIES_X], minions[i][ENEMIES_Y]);
            window.draw(minionSprite);
        }
    }
}



void drawFancyButton(RenderWindow& window, Text& text, bool isHovered, bool isDisabled=false) {
    FloatRect bounds = text.getGlobalBounds();
    
    // Button background
    RectangleShape button;
    button.setSize(Vector2f(bounds.width + 40, bounds.height + 30));
    button.setPosition(bounds.left - 20, bounds.top - 10);
    
    if(isDisabled) {
        button.setFillColor(Color(50, 50, 50, 150));
        text.setFillColor(Color(100, 100, 100));
    }
    else if(isHovered) {
        button.setFillColor(Color(255, 215, 0, 200)); // Gold
        button.setOutlineColor(Color::Yellow);
        button.setOutlineThickness(3);
        text.setFillColor(Color::Black);
        text.setScale(1.1f, 1.1f);
    }
    else {
        button.setFillColor(Color(0, 0, 0, 150));
        button.setOutlineColor(Color::White);
        button.setOutlineThickness(2);
        text.setFillColor(Color::White);
        text.setScale(1.0f, 1.0f);
    }
    
    window.draw(button);
}

bool isMouseOverText(Text& text, RenderWindow& window) {
    FloatRect bounds = text.getGlobalBounds();
    Vector2i mousePos = Mouse::getPosition(window);
    return bounds.contains(mousePos.x, mousePos.y);
}


void saveGameState(float playerX, float playerY, int playerHealth, int score, int currentLevel, int captureCount, int* capturedQueue, int enemyCount, int** enemies, int projCount, int** projectiles, int powerupCount, int** powerups){
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int POWERUP_TOTAL_PROPERTIES=6;
    const int ENEMY_TOTAL_PROPERTIES=22;
    ofstream saveFile("savegame.txt");
    if(!saveFile)return;
    
    //save player data
    saveFile<<playerX<<" "<<playerY<<" "<<playerHealth<<" "<<score<<" "<<currentLevel<<"\n";
    
    //save captured enemies
    saveFile<<captureCount<<"\n";
    for(int i=0;i<captureCount;i++){
        saveFile<<capturedQueue[i]<<" ";
    }
    saveFile<<"\n";
    
    //save enemy count
    saveFile<<enemyCount<<"\n";
    
    //save each enemy
    for(int i=0;i<enemyCount;i++){
        for(int j=0; j<ENEMY_TOTAL_PROPERTIES; j++){
            saveFile<<enemies[i][j]<<" ";
        }
        saveFile<<"\n";
    }
    
    //save projectile count
    saveFile<<projCount<<"\n";
    
    //save each projectile
    for(int i=0;i<projCount;i++){
        for(int j=0; j<PROJECT_TOTAL_PROPERTIES; j++){
            saveFile<<projectiles[i][j]<<" ";
        }
        saveFile<<"\n";
    }
    
    //save powerup count
    saveFile<<powerupCount<<"\n";
    
    //save each powerup
    for(int i=0;i<powerupCount;i++){
        for(int j=0; j<POWERUP_TOTAL_PROPERTIES; j++){
            saveFile<<powerups[i][j]<<" ";
        }
        saveFile<<"\n";
    }
    
    saveFile.close();
}

bool loadGameState(float& playerX, float& playerY, int& playerHealth, int& score,int& currentLevel, int& captureCount, int*& capturedQueue,int& enemyCount, int**& enemies, int& enemyCapacity, int& projCount, int**& projectiles, int& projCapacity,int& powerupCount, int**& powerups, int& powerupCapacity,char** lvl){
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int POWERUP_TOTAL_PROPERTIES=6;
    const int ENEMY_TOTAL_PROPERTIES=22;



    ifstream saveFile("savegame.txt");
    if(!saveFile)return true;
    
    //load player data
    saveFile>>playerX>>playerY>>playerHealth>>score>>currentLevel;
    
    //load captured enemies
    saveFile>>captureCount;
    delete[] capturedQueue;
    capturedQueue=new int[captureCount];
    for(int i=0;i<captureCount;i++){
        saveFile>>capturedQueue[i];
    }
    
    //load enemy count
    saveFile>>enemyCount;
    
    //clear existing enemies
    cleanup2DArray(enemies, enemyCapacity);
    enemyCapacity=1;
    initialize2DArray(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
    
    //load each enemy
    for(int i=0;i<enemyCount;i++){
        increaseSize(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);
        for(int j=0; j<ENEMY_TOTAL_PROPERTIES; j++){
            saveFile>>enemies[i][j];
        }
    }
    
    //load projectile count
    saveFile>>projCount;
    
    //clear existing projectiles
    cleanup2DArray(projectiles, projCapacity);
    projCapacity=1;
    initialize2DArray(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES);
    
    //load each projectile
    for(int i=0;i<projCount;i++){
        increaseSize(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES);
        for(int j=0; j<PROJECT_TOTAL_PROPERTIES; j++){
            saveFile>>projectiles[i][j];
        }
    }
    
    //load powerup count
    saveFile>>powerupCount;
    
    //clear existing powerups
    cleanup2DArray(powerups, powerupCapacity);
    powerupCapacity=1;
    initialize2DArray(powerups, powerupCapacity, POWERUP_TOTAL_PROPERTIES);
    
    //load each powerup
    for(int i=0;i<powerupCount;i++){
        increaseSize(powerups, powerupCapacity, POWERUP_TOTAL_PROPERTIES);
        for(int j=0; j<POWERUP_TOTAL_PROPERTIES; j++){
            saveFile>>powerups[i][j];
        }
    }
    
    saveFile.close();
    
    //recreate level based on currentLevel
    if(currentLevel==1){
        createLevel1Layout(lvl);
    }
    else if(currentLevel==2){
        createLevel2Layout(lvl);
    }
    else if(currentLevel==3){
        createLevel3Layout(lvl);
    }
    else if(currentLevel==4){
        createLevel3Layout(lvl);
    }
    
    return true;
}

bool saveFileExists(){
    ifstream file("savegame.txt");
    bool exists=file.good();
    file.close();
    return exists;
}




void loadLeaderboard(int scores[7]){

    const int MAX_LEADERBOARDS=7;
    //initialize all to 0
    for(int i=0;i<MAX_LEADERBOARDS;i++){
        scores[i]=0;
    }
    
    ifstream file("leaderboard.txt");
    if(!file)return;
    
    for(int i=0;i<MAX_LEADERBOARDS;i++){
        file>>scores[i];
    }
    
    file.close();
}

void saveLeaderboard(int scores[7]){

    const int MAX_LEADERBOARDS=7;
    ofstream file("leaderboard.txt");
    if(!file)return;
    
    for(int i=0;i<MAX_LEADERBOARDS;i++){
        file<<scores[i]<<"\n";
    }
    
    file.close();
}

void updateLeaderboard(int newScore){
    const int MAX_LEADERBOARDS=7;
    int scores[MAX_LEADERBOARDS];
    loadLeaderboard(scores);
    
    //find a pos to insert new score
    int insertPos=-1;
    for(int i=0;i<MAX_LEADERBOARDS;i++){
        if(newScore>scores[i]){
            insertPos=i;
            break;
        }
    }
    
    //insert score if it finds though
    if(insertPos != -1){
        //shift scores down
        for(int i=MAX_LEADERBOARDS-1; i>insertPos; i--){
            scores[i]=scores[i-1];
        }
        scores[insertPos]=newScore;
        
        saveLeaderboard(scores);
    }
}





void displayLeaderboard(RenderWindow& window, Font& font, Texture& bgTex) {
    const int MAX_LEADERBOARDS = 7;
    Sprite bgSprite(bgTex);
    bgSprite.setScale(0.4,0.5);
    
    int scores[MAX_LEADERBOARDS];
    // Load leaderboard function goes here
    for(int i = 0; i < MAX_LEADERBOARDS; i++) {
        scores[i] = 0; // Placeholder
    }
    
    Text title, backText;
    
    title.setFont(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(60);
    title.setFillColor(Color::Yellow);
    title.setStyle(Text::Bold);
    title.setPosition(350, 50);
    
    backText.setFont(font);
    backText.setString("Press ESC to return");
    backText.setCharacterSize(30);
    backText.setFillColor(Color::White);
    backText.setPosition(380, 780);
    
    Text scoreTexts[MAX_LEADERBOARDS];
    for(int i = 0; i < MAX_LEADERBOARDS; i++) {
        scoreTexts[i].setFont(font);
        char str[60];
        str[0] = '0' + (i + 1);
        str[1] = '.';
        str[2] = ' ';
        str[3] = '-';
        str[4] = '-';
        str[5] = '-';
        str[6] = '\0';
        scoreTexts[i].setString(str);
        scoreTexts[i].setCharacterSize(40);
        scoreTexts[i].setFillColor(Color::White);
        scoreTexts[i].setPosition(400, 150 + i * 90);
    }
    
    while(window.isOpen()) {
        Event ev;
        while(window.pollEvent(ev)) {
            if(ev.type == Event::Closed) {
                window.close();
                return;
            }
            if(ev.type == Event::KeyPressed) {
                if(Keyboard::isKeyPressed(Keyboard::Escape)) {
                    return;
                }
            }
        }
        
        window.clear();
        window.draw(bgSprite);
        window.draw(title);
        for(int i = 0; i < MAX_LEADERBOARDS; i++) {
            window.draw(scoreTexts[i]);
        }
        window.draw(backText);
        window.display();
    }
}










int displayLevels(RenderWindow& window, Font& font, Texture& bgTex) {
    Sprite bgSprite(bgTex);
    
    Text title, option1, option2, option3, option4;
    
    title.setFont(font);
    title.setString("LEVEL SELECTOR");
    title.setCharacterSize(70);
    title.setFillColor(Color::Yellow);
    title.setStyle(Text::Bold);
    title.setPosition(300, 100);
    
    option1.setFont(font);
    option1.setString("1. Level 1");
    option1.setCharacterSize(40);
    option1.setFillColor(Color::White);
    option1.setPosition(400, 280);
    
    option2.setFont(font);
    option2.setString("2. Level 2");
    option2.setCharacterSize(40);
    option2.setFillColor(Color::White);
    option2.setPosition(400, 380);
    
    option3.setFont(font);
    option3.setString("3. Level 3");
    option3.setCharacterSize(40);
    option3.setFillColor(Color::White);
    option3.setPosition(400, 480);
    
    option4.setFont(font);
    option4.setString("4. Boss Level");
    option4.setCharacterSize(40);
    option4.setFillColor(Color::Red);
    option4.setStyle(Text::Bold);
    option4.setPosition(400, 580);
    
    int hoveredOption = -1;
    
    while(window.isOpen()) {
        Event ev;
        while(window.pollEvent(ev)) {
            if(ev.type == Event::Closed) {
                window.close();
                return 0;
            }
            
            if(ev.type == Event::KeyPressed) {
                if(Keyboard::isKeyPressed(Keyboard::Num1)) return 3;
                if(Keyboard::isKeyPressed(Keyboard::Num2)) return 4;
                if(Keyboard::isKeyPressed(Keyboard::Num3)) return 5;
                if(Keyboard::isKeyPressed(Keyboard::Num4)) return 6;
                if(Keyboard::isKeyPressed(Keyboard::Escape)) return 0;
            }
            
            if(ev.type == Event::MouseButtonPressed) {
                if(ev.mouseButton.button == Mouse::Left) {
                    if(hoveredOption == 0) return 3;
                    if(hoveredOption == 1) return 4;
                    if(hoveredOption == 2) return 5;
                    if(hoveredOption == 3) return 6;
                }
            }
        }
        
        // Check hover
        hoveredOption = -1;
        if(isMouseOverText(option1, window)) hoveredOption = 0;
        else if(isMouseOverText(option2, window)) hoveredOption = 1;
        else if(isMouseOverText(option3, window)) hoveredOption = 2;
        else if(isMouseOverText(option4, window)) hoveredOption = 3;
        
        window.clear();
        window.draw(bgSprite);
        
        drawFancyButton(window, option1, hoveredOption == 0);
        window.draw(option1);
        
        drawFancyButton(window, option2, hoveredOption == 1);
        window.draw(option2);
        
        drawFancyButton(window, option3, hoveredOption == 2);
        window.draw(option3);
        
        drawFancyButton(window, option4, hoveredOption == 3);
        window.draw(option4);
        
        window.draw(title);
        window.display();
    }
    
    return 0;
}
int pauseMenu(RenderWindow& window, Font& font, float playerX, float playerY, int playerHealth, int score, int currentLevel, int captureCount, int* capturedQueue, int enemyCount, int** enemies, int projCount, int** projectiles, int powerupCount, int** powerups) 
{
    Text title, option1, option2, option3;
    
    title.setFont(font);
    title.setString("GAME PAUSED");
    title.setCharacterSize(50);
    title.setFillColor(Color::Yellow);
    title.setPosition(350, 150);
    
    option1.setFont(font);
    option1.setString("1. Resume");
    option1.setCharacterSize(35);
    option1.setFillColor(Color::White);
    option1.setPosition(400, 300);
    
    option2.setFont(font);
    option2.setString("2. Save & Exit");
    option2.setCharacterSize(35);
    option2.setFillColor(Color::White);
    option2.setPosition(400, 400);
    
    option3.setFont(font);
    option3.setString("3. Exit without Saving");
    option3.setCharacterSize(35);
    option3.setFillColor(Color::White);
    option3.setPosition(400, 500);
    
    while(window.isOpen()){
        Event ev;
        while(window.pollEvent(ev)){
            if(ev.type==Event::Closed){
                window.close();
                return 0;
            }
            if(ev.type==Event::KeyPressed){
                if(Keyboard::isKeyPressed(Keyboard::Num1)){
                    return 1; //resume
                }
                if(Keyboard::isKeyPressed(Keyboard::Num2)){
                    saveGameState(playerX, playerY, playerHealth, score, currentLevel, captureCount, capturedQueue, enemyCount, enemies, projCount, projectiles, powerupCount, powerups);
                    return 2; //save & Exit
                }
                if(Keyboard::isKeyPressed(Keyboard::Num3)){
                    return 3; //exit without saving
                }
            }
        }
        
        window.clear(Color::Black);
        window.draw(title);
        window.draw(option1);
        window.draw(option2);
        window.draw(option3);
        window.display();
    }
    
    return 0;
}

int mainMenu(RenderWindow& window, Font& font, Texture& bgTex, Texture& logoTex) {
    Sprite bgSprite(bgTex);
    bgSprite.setScale(0.4,0.5);
    Sprite logoSprite(logoTex);
    
    // Position logo
    logoSprite.setPosition(250, 50);
    logoSprite.setScale(2, 2);
    
    Text title, option1, option2, option3, option4, option5;
    
    // Animated title
    title.setFont(font);
    title.setString("TUMBLE-POP");
    title.setCharacterSize(80);
    title.setFillColor(Color::Yellow);
    title.setStyle(Text::Bold);
    title.setPosition(300, 80);
    
    option1.setFont(font);
    option1.setString("1. New Game");
    option1.setCharacterSize(40);
    option1.setFillColor(Color::White);
    option1.setPosition(420, 280);
    
    option2.setFont(font);
    option2.setString("2. Load Game");
    option2.setCharacterSize(40);
    option2.setFillColor(Color::White);
    option2.setPosition(420, 370);
    
    option3.setFont(font);
    option3.setString("3. Leaderboard");
    option3.setCharacterSize(40);
    option3.setFillColor(Color::White);
    option3.setPosition(420, 460);
    
    option4.setFont(font);
    option4.setString("4. Select Level");
    option4.setCharacterSize(40);
    option4.setFillColor(Color::White);
    option4.setPosition(420, 550);
    
    option5.setFont(font);
    option5.setString("5. Exit");
    option5.setCharacterSize(40);
    option5.setFillColor(Color::White);
    option5.setPosition(420, 640);
    
    bool saveExists = false; // Placeholder - replace with actual check
    int hoveredOption = -1;
    int animTimer = 0;
    
    while(window.isOpen()) {
        Event ev;
        while(window.pollEvent(ev)) {
            if(ev.type == Event::Closed) {
                window.close();
                return 0;
            }
            
            if(ev.type == Event::KeyPressed) {
                if(Keyboard::isKeyPressed(Keyboard::Num1)) return 1;
                if(Keyboard::isKeyPressed(Keyboard::Num2) && saveExists) return 2;
                if(Keyboard::isKeyPressed(Keyboard::Num3)) {
                    displayLeaderboard(window, font, bgTex);
                }
                if(Keyboard::isKeyPressed(Keyboard::Num4)) {
                    return displayLevels(window, font, bgTex);
                }
                if(Keyboard::isKeyPressed(Keyboard::Num5) || 
                   Keyboard::isKeyPressed(Keyboard::Escape)) return 0;
            }
            
            if(ev.type == Event::MouseButtonPressed) {
                if(ev.mouseButton.button == Mouse::Left) {
                    if(hoveredOption == 0) return 1;
                    if(hoveredOption == 1 ) return 2;
                    if(hoveredOption == 2) displayLeaderboard(window, font, bgTex);
                    if(hoveredOption == 3) return displayLevels(window, font, bgTex);
                    if(hoveredOption == 4) return 0;
                }
            }
        }
        
        // Animate title
        animTimer++;
        float titleOffset = sin(animTimer * 0.05f) * 10;
        title.setPosition(300, 80 + titleOffset);
        
        // Check hover
        hoveredOption = -1;
        if(isMouseOverText(option1, window)) hoveredOption = 0;
        else if(isMouseOverText(option2, window)) hoveredOption = 1;
        else if(isMouseOverText(option3, window)) hoveredOption = 2;
        else if(isMouseOverText(option4, window)) hoveredOption = 3;
        else if(isMouseOverText(option5, window)) hoveredOption = 4;
        
        window.clear();
        window.draw(bgSprite);
        
        // Draw title with glow effect
        title.setFillColor(Color(255, 255, 0, 100));
        title.setPosition(title.getPosition().x + 3, title.getPosition().y + 3);
        window.draw(title);
        title.setFillColor(Color::Yellow);
        title.setPosition(title.getPosition().x - 3, title.getPosition().y - 3);
        window.draw(title);
        
        drawFancyButton(window, option1, hoveredOption == 0);
        window.draw(option1);
        
        drawFancyButton(window, option2, hoveredOption == 1, !saveExists);
        window.draw(option2);
        
        drawFancyButton(window, option3, hoveredOption == 2);
        window.draw(option3);
        
        drawFancyButton(window, option4, hoveredOption == 3);
        window.draw(option4);
        
        drawFancyButton(window, option5, hoveredOption == 4);
        window.draw(option5);
        
        window.display();
    }
    
    return 0;
}
int gameOverScreen(RenderWindow& window, int& score, int& currentLevel, Font& font, Texture& bgTex){
    //update leaderboard
    updateLeaderboard(score);
    
    Sprite bgSprite(bgTex);
    bgSprite.setScale(0.4, 0.5);
    
    Text title, scoreText, levelText, option1, option2, congratsText;
    
    // Main title with animation
    title.setFont(font);
    title.setString("GAME OVER");
    title.setCharacterSize(80);
    title.setFillColor(Color::Red);
    title.setStyle(Text::Bold);
    title.setPosition(372, 100);
    
    // Congratulations text (shown if score is good)
    congratsText.setFont(font);
    congratsText.setString("New High Score!");
    congratsText.setCharacterSize(40);
    congratsText.setFillColor(Color::Yellow);
    congratsText.setStyle(Text::Bold);
    congratsText.setPosition(400, 200);
    
    // Score display
    scoreText.setFont(font);
    scoreText.setString("Final Score: " + to_string(score));
    scoreText.setCharacterSize(45);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(380, 300);
    
    // Level reached
    levelText.setFont(font);
    levelText.setString("Level Reached: " + to_string(currentLevel));
    levelText.setCharacterSize(35);
    levelText.setFillColor(Color::Cyan);
    levelText.setPosition(420, 350);
    
    // Menu options
    option1.setFont(font);
    option1.setString("1. Return to Menu");
    option1.setCharacterSize(40);
    option1.setFillColor(Color::White);
    option1.setPosition(400, 520);
    
    option2.setFont(font);
    option2.setString("2. Exit Game");
    option2.setCharacterSize(40);
    option2.setFillColor(Color::White);
    option2.setPosition(442, 620);
    
    // Check if it's a high score (simple check - compare with first score)
    int leaderboardScores[7];
    loadLeaderboard(leaderboardScores);
    bool isHighScore = (score > leaderboardScores[0]);
    
    int hoveredOption = -1;
    int animTimer = 0;
    
    while(window.isOpen()){
        Event ev;
        while(window.pollEvent(ev)){
            if(ev.type == Event::Closed){
                window.close();
                return 0;
            }
            
            if(ev.type == Event::KeyPressed){
                if(Keyboard::isKeyPressed(Keyboard::Num1) || 
                   Keyboard::isKeyPressed(Keyboard::Escape)){
                    return 0; // Return to menu
                }
                if(Keyboard::isKeyPressed(Keyboard::Num2)){
                    window.close();
                    return 0;
                }
            }
            
            if(ev.type == Event::MouseButtonPressed){
                if(ev.mouseButton.button == Mouse::Left){
                    if(hoveredOption == 0) return 0;
                    if(hoveredOption == 1){
                        window.close();
                        return 0;
                    }
                }
            }
        }
        
        // Animate title
        animTimer++;
        float titleOffset = sin(animTimer * 0.05f) * 8;
        title.setPosition(372, 100 + titleOffset);
        
        // Pulse high score text
        if(isHighScore){
            float pulse = sin(animTimer * 0.1f) * 0.2f + 1.0f;
            congratsText.setScale(pulse, pulse);
        }
        
        // Check hover
        hoveredOption = -1;
        if(isMouseOverText(option1, window)) hoveredOption = 0;
        else if(isMouseOverText(option2, window)) hoveredOption = 1;
        
        window.clear();
        window.draw(bgSprite);
        
        // Draw title with glow effect
        title.setFillColor(Color(255, 0, 0, 80));
        title.setPosition(title.getPosition().x + 4, title.getPosition().y + 4);
        window.draw(title);
        title.setFillColor(Color::Red);
        title.setPosition(title.getPosition().x - 4, title.getPosition().y - 4);
        window.draw(title);
        
        // Draw high score notification if applicable
        if(isHighScore){
            window.draw(congratsText);
        }
        
        // Draw score with shadow
        scoreText.setFillColor(Color(0, 0, 0, 150));
        scoreText.setPosition(scoreText.getPosition().x + 3, scoreText.getPosition().y + 3);
        window.draw(scoreText);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(scoreText.getPosition().x - 3, scoreText.getPosition().y - 3);
        window.draw(scoreText);
        
        window.draw(levelText);
        
        // Draw buttons with fancy style
        drawFancyButton(window, option1, hoveredOption == 0);
        window.draw(option1);
        
        drawFancyButton(window, option2, hoveredOption == 1);
        window.draw(option2);
        
        window.display();
    }
    
    return 0;
}


bool checkLevelComplete(int**& enemies, int enemyCount, int*producer, int& currentLevel, int& bossHealth, int& captureCount){
    const int PRODUCER_ACTIVE=4;
    //if any enemies left lvl 1 and 2
    if(currentLevel<3){
        for(int i=0;i<enemyCount;i++){
            if(captureCount>0||enemyCount>0  ){
                return false;
            }
        }
    }      //lvl 3     
    else if(currentLevel==4&&bossHealth>0 )return false;                                             
    else if(enemyCount >0||captureCount>0||(currentLevel==3&&producer[PRODUCER_ACTIVE]==1))return false;
    return true;
}

void levelUp(int& levelStartTime, int& score, char** lvl, int**& enemies,int& enemyCount, int& maxCapacity, float& playerX, float& playerY,  int& playerHealth, int& currentLevel, int& enemyCapacity, int& bossX, int& bossY, int& bossHealth, bool& bossAngry, int**& tentacles, int& tentacleCount, int& tentacleCapacity, int**& minions, int& minionCount, int& minionCapacity,int& bossHDirection, int& bossVDirection, int& bossAtTopTimer, int*& producer, float& cloudY, bool& gameRunning,RenderWindow& window, Font& font, Texture& menuBgTex){
    currentLevel++;

    if(currentLevel==1){
        
        createLevel1Layout(lvl);
        initializeEnemiesLevel1(enemies, enemyCount, enemyCapacity);
    }
    if(currentLevel==2){
        int seconds=levelStartTime/60;
        if(seconds<30)score+=2000;
        else if(seconds<45)score+=1000;
        else if(seconds<60)score+=500;
        
        createLevel2Layout(lvl);
        initializeEnemiesLevel2(enemies, enemyCount, enemyCapacity);
        maxCapacity=5;
        score+=1000;
        if(playerHealth==3)score+=1500;
    }
    else if(currentLevel==3){
        createLevel3Layout(lvl);
        int seconds=levelStartTime/60;
        if(seconds<30)score+=2000;
        else if(seconds<45)score+=1000;
        else if(seconds<60)score+=500;
        if(playerHealth==3)score+=2500;
        score+=2000;
        if(playerHealth==3)score+=1500;
        initializeProducer(producer, cloudY);
    }
    else if(currentLevel==4){
        int seconds=levelStartTime/60;
        if(seconds<30)score+=2000;
        else if(seconds<45)score+=1000;
        else if(seconds<60)score+=500;
        if(playerHealth==3)score+=2500;
        score+=4000;
        if(playerHealth==3)score+=1500;
        
        createLevel3Layout(lvl);
        initializeBossLevel(bossX, bossY, bossHealth, bossAngry,tentacles, tentacleCount, tentacleCapacity,minions, minionCount, minionCapacity,bossHDirection, bossVDirection, bossAtTopTimer);
    }
    else if(currentLevel>=5){
        int seconds=levelStartTime/60;
        if(seconds<30)score+=2000;
        else if(seconds<45)score+=1000;
        else if(seconds<60)score+=500;
        if(playerHealth==3)score+=2500;
        score+=4000;
        if(playerHealth==3)score+=1500;
        
        //victory
        gameRunning=false;
        gameOverScreen(window, score, currentLevel, font, menuBgTex);
    }

        playerHealth=3;
        playerX=500;
        playerY=150;
        levelStartTime=0;
}
int characterSelectionScreen(RenderWindow& window, Font& font, Texture& bgTex, Texture& char1Tex, Texture& char2Tex) {
    Sprite bgSprite(bgTex);
    bgSprite.setScale(0.4,0.5);
    Sprite char1Sprite(char1Tex);
    Sprite char2Sprite(char2Tex);
    
    // Position character sprites
    char1Sprite.setPosition(200, 350);
    char1Sprite.setScale(6, 6);
    char2Sprite.setPosition(700, 350);
    char2Sprite.setScale(6, 6);
    
    Text title, desc1, desc2, backText;
    
    title.setFont(font);
    title.setString("SELECT YOUR CHARACTER");
    title.setCharacterSize(60);
    title.setFillColor(Color::Yellow);
    title.setStyle(Text::Bold);
    title.setPosition(200, 100);
    
    desc1.setFont(font);
    desc1.setString("1.5x Speed\n90 Vacuum");
    desc1.setCharacterSize(30);
    desc1.setFillColor(Color::Green);
    desc1.setStyle(Text::Bold);
    desc1.setPosition(220, 600);
    
    desc2.setFont(font);
    desc2.setString("1.2x Power\n360 Vacuum");
    desc2.setCharacterSize(30);
    desc2.setFillColor(Color::Yellow);
    desc2.setStyle(Text::Bold);
    desc2.setPosition(720, 600);
    
    
    // Selection boxes
    RectangleShape select1(Vector2f(250, 350));
    select1.setPosition(175, 320);
    select1.setFillColor(Color::Transparent);
    select1.setOutlineColor(Color::Green);
    select1.setOutlineThickness(3);
    
    RectangleShape select2(Vector2f(250, 350));
    select2.setPosition(675, 320);
    select2.setFillColor(Color::Transparent);
    select2.setOutlineColor(Color::Yellow);
    select2.setOutlineThickness(3);
    
    int hoveredChar = -1;
    
    while(window.isOpen()) {
        Event ev;
        while(window.pollEvent(ev)) {
            if(ev.type == Event::Closed) {
                window.close();
                return 0;
            }
            
            if(ev.type == Event::KeyPressed) {
                if(Keyboard::isKeyPressed(Keyboard::Num1)) return 0;
                if(Keyboard::isKeyPressed(Keyboard::Num2)) return 1;
            }
            
            if(ev.type == Event::MouseButtonPressed) {
                if(ev.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if(select1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        return 0;
                    }
                    if(select2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        return 1;
                    }
                }
            }
        }
        
        // Check hover
        Vector2i mousePos = Mouse::getPosition(window);
        if(select1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            hoveredChar = 0;
            select1.setOutlineThickness(5);
            select1.setFillColor(Color(0, 255, 0, 30));
        }
        else {
            select1.setOutlineThickness(3);
            select1.setFillColor(Color::Transparent);
        }
        
        if(select2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            hoveredChar = 1;
            select2.setOutlineThickness(5);
            select2.setFillColor(Color(255, 255, 0, 30));
        }
        else {
            select2.setOutlineThickness(3);
            select2.setFillColor(Color::Transparent);
        }
        
        window.clear();
        window.draw(bgSprite);
        window.draw(select1);
        window.draw(select2);
        window.draw(char1Sprite);
        window.draw(char2Sprite);
        window.draw(title);
        window.draw(desc1);
        window.draw(desc2);
        window.draw(backText);
        window.display();
    }
    
    return 0;
}










int main(){
    const int CELL_SIZE=64;
    const int LEVEL_HEIGHT=14;
    const int LEVEL_WIDTH=18;
    const int PLAYER_WIDTH=96;
    const int PLAYER_HEIGHT=144;
    const int LVL1_VACCUM_CAP=3;
    const int PROJECT_TOTAL_PROPERTIES=8;
    const int ENEMY_TOTAL_PROPERTIES=22;
    const int POWERUP_TOTAL_PROPERTIES=6;
    const int BOSS_MOVING_RIGHT=1;
    const int BOSS_MOVING_UP=2;
    const int PRODUCER_ACTIVE=4;
    const int TYPE_GHOSTL=0;
    const int TYPE_GHOSTR=4;
    const int TYPE_SKELETONL=1;
    const int TYPE_SKELETONR=5;
    const int TYPE_INVISIBLEL= 2;
    const int TYPE_INVISIBLER= 6;
    const int TYPE_CHELNOVL= 3;
    const int TYPE_CHELNOVR= 7;

    srand(time(0));
    
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Tumble-POP");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Texture bgTex[3], blockTex[3], playerTexRight, playerTexLeft, enemyTextures[10], vacuumUp, projTex,min1LTex, min1RTex, min2LTex, min2RTex, minRevTex,bossTex, tentacleTex, producerTex, powerupTextures[4],menuBgTex, logoTex, char1Tex, char2Tex;
    Sprite bgSprite, blockSprite, playerSprite, projSprite, enemySprite, bossSprite, tentacleSprite, vacuumSprite, minionSprite,  producerSprite, powerupSprites;


    menuBgTex.loadFromFile("Assets/menubg.png");
    char1Tex.loadFromFile("Assets/green.png");
    char2Tex.loadFromFile("Assets/yellow.png");
    bgTex[0].loadFromFile("Assets/bg1.png");
    bgTex[1].loadFromFile("Assets/bg2.png");
    bgTex[2].loadFromFile("Assets/bg3.png");
    blockTex[0].loadFromFile("Assets/block1.png");
    blockTex[1].loadFromFile("Assets/block2.png");
    blockTex[2].loadFromFile("Assets/block3.png");
    playerTexRight.loadFromFile("Assets/playerright.png");
    playerTexLeft.loadFromFile("Assets/playerleft.png");
    enemyTextures[TYPE_GHOSTL].loadFromFile("Assets/ghostleft.png");
    enemyTextures[TYPE_GHOSTR].loadFromFile("Assets/ghostright.png");
    enemyTextures[TYPE_SKELETONL].loadFromFile("Assets/skeleleft.png");
    enemyTextures[TYPE_SKELETONR].loadFromFile("Assets/skeleright.png");
    enemyTextures[TYPE_INVISIBLEL].loadFromFile("Assets/invisleft.png");
    enemyTextures[TYPE_INVISIBLER].loadFromFile("Assets/invisright.png");
    enemyTextures[TYPE_CHELNOVL].loadFromFile("Assets/chelnovleft.png");
    enemyTextures[TYPE_CHELNOVR].loadFromFile("Assets/chelnovright.png");
    vacuumUp.loadFromFile("Assets/vacuumup.png");
    projTex.loadFromFile("Assets/chelnovleft.png");
    bossTex.loadFromFile("Assets/boss.png");
    tentacleTex.loadFromFile("Assets/tentacles.png");
    min1LTex.loadFromFile("Assets/min1L.png");
    min1RTex.loadFromFile("Assets/min1R.png");
    min2LTex.loadFromFile("Assets/min2L.png");
    min2RTex.loadFromFile("Assets/min2R.png");
    minRevTex.loadFromFile("Assets/revolve.png");
    producerTex.loadFromFile("Assets/producer.png");
    
    playerSprite.setTexture(playerTexLeft);
    projSprite.setTexture(projTex);
    playerSprite.setTextureRect(IntRect(16, 33, 32, 48));
    playerSprite.setScale(3, 3);
    enemySprite.setScale(3, 3);
    projSprite.setScale(3, 3);
    bossSprite.setScale(5, 5);
    tentacleSprite.setScale(3, 3);
    minionSprite.setScale(2, 2);
    producerSprite.setScale(1.5, 1.5);
    


    Font font;
    font.loadFromFile("Assets/font.otf");
    
    Music music;
    music.openFromFile("Assets/mus.ogg");
    music.setVolume(20);
    music.play();
    music.setLoop(true);



    while(window.isOpen()){
        //show main menu
        int menuChoice=mainMenu(window, font, menuBgTex, logoTex);
        
        if(menuChoice==0){
            break; //exit game
        }


        
        //create level
        char** lvl=new char*[LEVEL_HEIGHT];
        for(int i=0;i<LEVEL_HEIGHT;i++){
            lvl[i]=new char[LEVEL_WIDTH];
        }
        
        //game state
        int currentLevel=0;
        int score=0;
        int selectedCharacter ;
        //player state
        float playerX, playerY, playerVX, playerVY;
        bool onGround, isJumping;
        int playerHealth;
        float slideVelocity=1.5f;
        float playerSpeed, vacuumRange;
        bool movingLeft=false, movingRight=false;
        bool invulnerable=true;
        int invulnerableTimer=0;
        int levelStartTime=0;
        
        // Vacuum state
        int vacuumDir=3; // Default right
        bool vacuumActive=false;
        int vacuumHoldTimer=0;
        int* capturedQueue=new int[0];
        int captureCount=0;
        float vacuumAngle=0.0f; //in radians
        int maxCapacity=LVL1_VACCUM_CAP;
        
        int** enemies=nullptr;
        int enemyCount=0;
        int enemyCapacity =1;
        initialize2DArray(enemies, enemyCapacity, ENEMY_TOTAL_PROPERTIES);         
        int projCapacity=1;
        int** projectiles=nullptr;
        int projCount=0;
        initialize2DArray(projectiles, projCapacity, PROJECT_TOTAL_PROPERTIES);
        int powerupCapacity=1;
        int** powerups=nullptr;
        int powerupCount=0;
        int* activePowerups= new int[3]{0, 0, 0}; //speed, range, power timers
        initialize2DArray(powerups, powerupCapacity, POWERUP_TOTAL_PROPERTIES);

        int bossX=0, bossY=0;
        int bossHealth=0;
        bool bossAngry=false;
        int bossSpawnTimer=0;
        int bossHDirection=BOSS_MOVING_RIGHT; 
        int bossVDirection=BOSS_MOVING_UP;   
        int bossAtTopTimer=0;         
        int** bossTentacles=nullptr;
        int tentacleCount=0;
        int tentacleCapacity=0;
        
        float cloudY=300;
        int cloudDirection=1; // 1=down, -1=up
        int prevCloudCellY=-1;

        int* producer= new int[6];

        
        for(int i=0;i<4;i++){
            powerupSprites.setTexture(playerTexLeft);
            powerupSprites.setScale(2, 2);
        }
        // single press vars
        bool spaceWasPressed=false;
        bool qWasPressed=false;
        bool eWasPressed=false;



        if(menuChoice==1){
            //new Game
            selectedCharacter=characterSelectionScreen(window, font, menuBgTex, char1Tex, char2Tex);
            createLevel1Layout(lvl);
            initializePlayer(playerX, playerY, playerVX, playerVY, onGround, isJumping, playerHealth, selectedCharacter, playerSpeed, vacuumRange, score);
        }
        else if(menuChoice==2){
            //load Game
            bool loaded=loadGameState(playerX, playerY, playerHealth, score,currentLevel, captureCount, capturedQueue, enemyCount, enemies, enemyCapacity,  projCount, projectiles, projCapacity,powerupCount, powerups, powerupCapacity,lvl);
           
        }
        else if(menuChoice >2 ){
            currentLevel=menuChoice-3;
        }

        int offHeight= (selectedCharacter)*189;
        producer[PRODUCER_ACTIVE]=0;



        //main game loop
        bool gameRunning=true;
        while(window.isOpen()&&gameRunning){
        
            Event ev;
            while(window.pollEvent(ev)){
                if(ev.type==Event::Closed){
                    window.close();
                }
            }
            
            if(Keyboard::isKeyPressed(Keyboard::Escape)){
                window.close();
            }
                //check for pause (P key)
                if(Keyboard::isKeyPressed(Keyboard::P)){
                    int pauseChoice=pauseMenu(window, font, playerX, playerY,playerHealth, score, currentLevel, captureCount, capturedQueue,  enemyCount, enemies, projCount, projectiles, powerupCount, powerups); 
                    if(pauseChoice==1){
                        //resume continue game loop
                        continue;
                    }
                    else if(pauseChoice==2||pauseChoice==3){
                        //exit to menu
                        gameRunning=false;
                        break;
                    }
                }
            updateVacuumDirection(vacuumDir, vacuumAngle, selectedCharacter);
            vacuumActive=Keyboard::isKeyPressed(Keyboard::Space);
            captureEnemies(enemies, enemyCount, enemyCapacity, playerX, playerY, vacuumDir,selectedCharacter, capturedQueue, captureCount, maxCapacity, score, activePowerups, powerups, powerupCount, powerupCapacity, vacuumAngle);
            updateStunnedEnemies(enemies, enemyCount);
            if(Keyboard::isKeyPressed(Keyboard::Q)){
                if(!qWasPressed){
                    shootSingleEnemy(capturedQueue, captureCount, projectiles, projCount,
                                    playerX, playerY, vacuumDir, projCapacity, vacuumAngle, selectedCharacter);
                    qWasPressed=true;
                }
            }
            else qWasPressed=false;

            if(Keyboard::isKeyPressed(Keyboard::E)){
                if(!eWasPressed){
                    shootAllEnemies(capturedQueue, captureCount, projectiles, projCount,
                                   playerX, playerY, vacuumDir, projCapacity, vacuumAngle, selectedCharacter);
                    eWasPressed=true;
                }
            }
            else eWasPressed=false;

            levelStartTime++;
            
            updatePlayerMovement(playerX, playerY, playerSpeed, lvl, movingLeft, movingRight, selectedCharacter, activePowerups);
            handleSlantedPlatformSlide(playerX, playerY, lvl, slideVelocity); 
            handleJump(playerVY, isJumping, onGround, playerY);
            if(Keyboard::isKeyPressed(Keyboard::Down)&&(playerY/CELL_SIZE)<LEVEL_HEIGHT-4)isJumping=true;
            if(!isJumping)applyGravity(playerY, playerVY, onGround, lvl, playerX);
        
            updateEnemySpawning(enemies, enemyCount, lvl);
            updateAllEnemies(enemies, enemyCount, lvl, projectiles, projCount, projCapacity, playerX, playerY, bossAngry);

            updatePowerups(powerups, powerupCount, lvl);
            handlePowerupCollection(powerups, powerupCount, playerX, playerY, playerHealth, activePowerups, powerupCapacity);
            updateActivePowerups(activePowerups);
            
            updateProjectiles(projectiles, projCount, lvl, projCapacity);
            
            handleProjectileEnemyCollision(projectiles, projCount, projCapacity,enemies, enemyCount, enemyCapacity, score, powerups, powerupCount, powerupCapacity);
            handleProjectilePlayerCollision(projectiles, projCount, playerX, playerY, playerHealth, invulnerable, invulnerableTimer, score, window, currentLevel,font, projCapacity);
            handlePlayerEnemyCollision(enemies, enemyCount, playerX, playerY, playerHealth, invulnerable, invulnerableTimer, score, window, currentLevel, font);
            
            if(currentLevel==3){
                handleProjectileProducerCollision(projectiles, projCount, producer, score, projCapacity);
               if(producer[PRODUCER_ACTIVE]==1) handlePlayerProducerCollision(producer, playerX, playerY, playerHealth, invulnerable, invulnerableTimer, score, window, currentLevel, font);
                updateCloud(cloudY, cloudDirection, lvl, prevCloudCellY, playerX, playerY, producer);
                updateProducer(producer, enemies, enemyCount, enemyCapacity, cloudY, lvl);
            }

            updatePlayerAnimation(playerSprite, playerTexRight, playerTexLeft, movingRight, movingLeft, isJumping, playerVY, onGround, offHeight);
            updateVacuumAnimation(playerX, playerY, vacuumDir, vacuumRange, vacuumActive, vacuumSprite, playerTexLeft, movingLeft, movingRight, vacuumUp, activePowerups);
        
             if(currentLevel==4){ 
                updateBoss(bossX, bossY, bossHealth, bossAngry, bossTentacles, tentacleCount, tentacleCapacity, enemies, enemyCount, enemyCapacity, bossSpawnTimer, bossHDirection, bossVDirection,  bossAtTopTimer);
                handleProjectileBossCollision(projectiles, projCount, bossHealth, bossX, bossY, projCapacity);
                handlePlayerBossCollision(playerX, playerY, playerHealth,invulnerable, invulnerableTimer,bossX, bossY, bossTentacles, tentacleCount,enemies, enemyCount, score, window, currentLevel, font);
                handleProjectileTentacleRespawn(projectiles, projCount,bossTentacles, tentacleCount, enemies, enemyCount, enemyCapacity, projCapacity, lvl);
            }
            
            if(checkLevelComplete(enemies, enemyCount, producer, currentLevel, bossHealth, captureCount)){
                levelUp(levelStartTime, score, lvl, enemies, enemyCount, maxCapacity, playerX, playerY, playerHealth, currentLevel, enemyCapacity, bossX, bossY, bossHealth, bossAngry,bossTentacles, tentacleCount, tentacleCapacity, enemies, enemyCount, enemyCapacity, bossHDirection, bossVDirection, bossAtTopTimer,producer, cloudY, gameRunning, window, font, menuBgTex);
            }
      
            window.clear();
            if(currentLevel==1)bgSprite.setTexture(bgTex[0]);
            else if(currentLevel==2)bgSprite.setTexture(bgTex[1]);
            else bgSprite.setTexture(bgTex[2]);
            window.draw(bgSprite);
            renderLevel(window, lvl, blockSprite, blockTex);
            renderEnemies(window, enemies, enemyCount, enemySprite, enemyTextures);
            if(currentLevel==4){
                renderBoss(window, bossX, bossY, bossHealth, bossAngry, bossSprite, bossTex);
                renderTentacles(window, bossTentacles, tentacleCount, tentacleSprite, tentacleTex);
                renderBossMinions(window, enemies, enemyCount, minionSprite, min1LTex, min1RTex, min2LTex, min2RTex, minRevTex);
            }
            if(currentLevel==3)renderProducer(window, producer, producerSprite, producerTex);
            renderPowerups(window, powerups, powerupCount, powerupSprites, playerTexRight);
            renderProjectiles(window, projectiles, projCount, projSprite, projTex, enemyTextures, minRevTex);
            if(!invulnerable||(invulnerableTimer%10<5)){
                playerSprite.setPosition(playerX, playerY);
                window.draw(playerSprite);
                if(vacuumActive)window.draw(vacuumSprite);
            }
            renderUI(window, score, playerHealth, captureCount, font);
            if(playerHealth<=1){
                gameOverScreen(window, score, currentLevel, font, menuBgTex);
                gameRunning=false;
                break;
            }
            
            window.display();
        }
        music.stop();

        cleanup2DArray(enemies, enemyCapacity);
        cleanup2DArray(projectiles, projCapacity);
        cleanup2DArray(powerups, powerupCapacity);
        cleanup2DArray(bossTentacles, tentacleCount);
        delete[] capturedQueue;
        delete[] producer; 
        delete[] activePowerups;

        for(int i=0;i<LEVEL_HEIGHT;i++){
            delete[] lvl[i];
        }
        delete[] lvl;
    }
    return 0;
}
