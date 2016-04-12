
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library


#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET 10 // Can alternately just connect to Arduino's reset pin


// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// variables for the position of the ball and paddle
int paddleX = 0;
int paddleY = 300;
int oldPaddleX, oldPaddleY;
int paddleA = 0;
int paddleB = 0;
int oldPaddleA, oldPaddleB;
int ballSpeed = 5; 
int ballDirectionX = ballSpeed;
int ballDirectionY = ballSpeed;
//int ballDirectionA = ballSpeed;
//int ballDirectionB = ballSpeed;
int paddleHeight = 25;
int player1Score = 0;
int player2Score = 0;
int p1C = BLUE;
int p2C = WHITE;



int oldBallX , oldBallY;
int ballY = 160;
int ballX = 120;

void setup(void) {
  
    Serial.begin(9600);
    tft.reset();
    uint16_t identifier = tft.readID();
    tft.begin(identifier);
    tft.fillScreen(BLACK);
    tft.setTextColor(WHITE,BLACK);    
    tft.setTextSize(2);
   // ballY = 7;
    
}

void loop(void) {
    // save the width and height of the screen
  int myWidth = tft.width();
  int myHeight = tft.height();
  
 tft.drawFastHLine(0, myHeight/2, myWidth, MAGENTA);
 tft.setCursor(210, 130);
 tft.setTextColor(p1C,BLACK);
 tft.print(player1Score);
 
 tft.setCursor(30, 170);
 tft.setTextColor(p2C,BLACK);
 tft.print(player2Score);
   

  
   paddleX = map(analogRead(A5), 190, 8, 0, myHeight); 

    if (paddleX > tft.width()-24 ) {
   paddleX = 216;
  }
  if (paddleX < 0 ) {
   paddleX = 0;
  }
  
        Serial.println(paddleX);
      
  if (oldPaddleX != paddleX || oldPaddleY != paddleY) {
    tft.fillRect(oldPaddleX, oldPaddleY, paddleHeight, 10, BLACK);
  }
  
  tft.fillRect(paddleX, paddleY, paddleHeight, 10, p2C);
    oldPaddleX = paddleX;
  oldPaddleY = paddleY;
  
 //   Serial.print(tft.width());
   // Serial.print(tft.height());
  //  Serial.println(paddleX);


    
     //ADDED
    paddleA = map(analogRead(A4), 190, 8, 0, myHeight); 
    
     if (paddleA > tft.width()-24 ) {
   paddleA = 216;
  }
  if (paddleA < 0 ) {
   paddleA = 0;
  }
 // if (paddleA > tft.width()  || paddleA < 0) {
   //ballDirectionX = -ballDirectionX; 
  //}  
  
  if (oldPaddleA != paddleA || oldPaddleB != paddleB) {
    tft.fillRect(oldPaddleA, oldPaddleB, paddleHeight, 10, BLACK);
  }
  
  tft.fillRect(paddleA, paddleB, paddleHeight, 10, p1C);
    oldPaddleA = paddleA;
  oldPaddleB = paddleB;
  
 
     moveBall();
     
  
  
}

void moveBall() {
  
  int radius = 5;
    // if the ball goes offscreen, reverse the direction:
 if (ballX > tft.width()  || ballX < 0) {
   ballDirectionX = -ballDirectionX;
 }
   if (ballY > tft.height() || ballY < 0) {
   ballDirectionY = -ballDirectionY;
 }  
   // check if the ball and the paddle occupy the same space on screen
  if (inPaddle(ballX, ballY, paddleX, paddleY, paddleHeight, 10)) {
    ballDirectionX =  +ballDirectionX;
    ballDirectionY = -ballDirectionY;
  }
    //ADDED
    if (inPaddle(ballX, ballY, paddleA, paddleB, paddleHeight, 10)) {
    ballDirectionX =  +ballDirectionX;
    ballDirectionY = -ballDirectionY;
 }
 if(ballY == 320) { 
   resetBall(0);
   player1Score++;
 }
 
 if(ballY == 0) {
   resetBall(1);
   player2Score++;
 }
   
  // update the ball's position
 ballX += ballDirectionX;
 ballY += ballDirectionY;
 
 //ADDED
 if (player1Score == 10) {
   tft.fillScreen(BLACK);
   tft.setCursor(30, 130);
   tft.setTextColor(p1C);
   tft.print(" Player 1 WINS ");
   delay(3000);
   tft.fillScreen(BLACK);
   player1Score = 0;
   player2Score = 0;
 }
 if (player2Score == 10) {
   tft.fillScreen(BLACK);
   tft.setCursor(30, 130);
   tft.setTextColor(p2C);
   tft.print(" Player 2 WINS ");
   delay(3000);
   tft.fillScreen(BLACK);
   player1Score = 0;
   player2Score = 0;
   
 }
 
  if (oldBallX != ballX || oldBallY != ballY) {
    tft.fillCircle(oldBallX, oldBallY, radius, BLACK);
  }
  
  tft.fillCircle(ballX, ballY, radius, YELLOW);
  oldBallX = ballX;
  oldBallY = ballY;
  
}
void resetBall(int winner) {
 ballX = tft.width()/2;
 ballY = tft.height()/2;
 if (winner == 0){
   //Reset the ball.
   ballDirectionX = ballSpeed;
   ballDirectionY = ballSpeed;
 }
 else {
    //Reset the ball.
   ballDirectionX = -ballSpeed;
   ballDirectionY = -ballSpeed;
 
 }
  
}

// this function checks the position of the ball
// to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
  boolean result = false;
   
  if ((x >= rectX && x <= (rectX + rectWidth)) && 
      (y >= rectY && y <= (rectY + rectHeight))) {
       result = true; 
      }
 
return result;  
}

