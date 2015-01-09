// game settings and configuration
//---------------------------------------------
int gamePoints = 21;

// Olympic style games
int maxDelayOfBall = 75; // probably don't want to go higher than 200
int minDelayOfBall = 10; // probably don't want to go lower than 10
int minVolleys = 1;
int maxVolleys = 30;
int burnerThreshold = 20; // if the player hits that fast, give a good chance to win the point


// The rest of us
/*int maxDelayOfBall = 200; // probably don't want to go higher than 200*/
/*int minDelayOfBall = 10; // probably don't want to go lower than 10*/
/*int minVolleys = 1;*/
/*int maxVolleys = 15;*/
/*int burnerThreshold = 35; // if the player hits that fast, give a good chance to win the point*/


//---------------------------------------------




int ledPins[] = {2,3,4,5,6,7,8,9};
int playerOne = ledPins[0];
int playerTwo = ledPins[7];
int servingPlayer = 0;
long pointScoredAt = 0;
int currentVolleyNumber = 0;
int playerOnePoints = 0;
int playerTwoPoints = 0;
int gameOver = 0;


void setup() {
  for(int index = 0; index <= 7; index++) {
    pinMode(ledPins[index],OUTPUT);
  }
  randomSeed(analogRead(0));
  pointScoredAt = randPointScoredAt();
  servingPlayer = playerOne;
}

int randPointScoredAt() {
  return random(minVolleys, maxVolleys);
}

void loop() {
  if (gameOver == 0) {
    pingPong();           
  }
}

void pingPong() {

  // turn both players on
  digitalWrite(playerOne, HIGH);
  digitalWrite(playerTwo, HIGH);

  // keep track of serves and who served last
  int totalScore = playerOnePoints + playerTwoPoints;
  int remainder = totalScore % 5;
  if (totalScore > 0 && remainder == 0) {
    servingPlayer = otherPlayer(servingPlayer);
  }

  int winningPlayer = startPoint(servingPlayer);
  recordPoint(winningPlayer);
   
}

int startPoint(int serverPlayerForThisPoint) {
  while (currentVolleyNumber != pointScoredAt) {
    long playerOneSpeed = random(minDelayOfBall, maxDelayOfBall); 
    long playerTwoSpeed = random(minDelayOfBall, maxDelayOfBall); 
    int receivingPlayer = otherPlayer(serverPlayerForThisPoint);

    if (serverPlayerForThisPoint == playerOne) {
      playerOneHitsToPlayerTwo(playerOneSpeed);  
      if (didPlayerWinOnBurnerHit(playerOne, playerOneSpeed)) return playerOne;
    } else {
      playerTwoHitsToPlayerOne(playerTwoSpeed);
      if (didPlayerWinOnBurnerHit(playerTwo, playerTwoSpeed)) return playerTwo;
    }
    serverPlayerForThisPoint = otherPlayer(serverPlayerForThisPoint);
  }
  return serverPlayerForThisPoint;
}

bool didPlayerWinOnBurnerHit(int playerThatHitBall, int ballSpeed) {
  if (ballSpeed <= burnerThreshold) {
    int randChanceOfWinningPoint = random(1, 10);
    if (randChanceOfWinningPoint > 5) return true;
  }
  return false;
}

void recordPoint(int winningPlayer) {
  int losingPlayer = otherPlayer(winningPlayer);
  digitalWrite(losingPlayer, LOW);
  blinkPlayer(winningPlayer, 3);
  delay(1000);
  currentVolleyNumber = 0;
  pointScoredAt = randPointScoredAt();

  winningPlayer == playerOne ? playerOnePoints++ : playerTwoPoints++;
  checkIfPlayerHasWon(winningPlayer);
}

void checkIfPlayerHasWon(int winningPlayer) {
  int winningPlayerPoints = (winningPlayer == playerOne ? playerOnePoints : playerTwoPoints);
  int losingPlayerPoints = (winningPlayer == playerOne ? playerTwoPoints : playerOnePoints);
  bool isPointDifferentialGreaterThanOne = abs(winningPlayerPoints - losingPlayerPoints) > 1; 
  if (winningPlayerPoints >= gamePoints && isPointDifferentialGreaterThanOne) {
    playerWins(winningPlayer); 
  }
}

int otherPlayer(int aPlayer) {
  return aPlayer == playerOne ? playerTwo : playerOne;
}

void playerOneHitsToPlayerTwo(long hitSpeed) {
  for(int index = 1; index <= 6; index++) {
    digitalWrite(ledPins[index], HIGH);  // turn LED on
    delay(hitSpeed);                    // pause to slow down
    digitalWrite(ledPins[index], LOW);   // turn LED off
  }
  currentVolleyNumber++;
}

void playerTwoHitsToPlayerOne(long hitSpeed) {
  for(int index = 6; index >= 1; index--) {
    digitalWrite(ledPins[index], HIGH);  // turn LED on
    delay(hitSpeed);                    // pause to slow down
    digitalWrite(ledPins[index], LOW);   // turn LED off
  }
  currentVolleyNumber++;
}


void playerWins(int winningPlayer) {
  int losingPlayer = otherPlayer(winningPlayer); 
  digitalWrite(losingPlayer, LOW);
  digitalWrite(winningPlayer, HIGH);
  
  blinkPingPongBalls(10); 
  blinkPlayer(winningPlayer, 5);
  digitalWrite(winningPlayer, LOW);
  gameOver = 1;
}

void blinkPingPongBalls(int numberOfTimes) {
  for (int index = 0; index < numberOfTimes; index++) {
    turnOnPingPongBalls();
    delay(250);
    turnOffPingPongBalls();
    delay(250);
  }
}

void blinkPlayer(int thePlayer, int numberOfTimes) {
  for (int index = 0; index < numberOfTimes; index++) {
    digitalWrite(thePlayer, HIGH);
    delay(250);
    digitalWrite(thePlayer, LOW);
    delay(250);
  }
}

void turnOnPingPongBalls() {
  for (int index = 1; index <= 6; index++) {
    digitalWrite(ledPins[index], HIGH);
  } 
}

void turnOffPingPongBalls() {
  for (int index = 1; index <= 6; index++) {
    digitalWrite(ledPins[index], LOW);
  } 
}



