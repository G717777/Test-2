#define IsDown(b) input->buttons[b].isDown
#define Pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define Released(b) !(input->buttons[b].isDown && input->buttons[b].changed)

// Player
float posX = 74.f;
float playerHSX = 2, playerHSY = 10;
float pos = 0.f, vel, pos1, vel1;

int player1Score = 0, player2Score = 0;

// Arena
float arenaHSX = 77, arenaHSY = 45;

//Ball
float ballX, ballY, velX = 100.f, velY, ballAccX = 0.f, ballAccY = 0.f;
float ballHSX = 1.5f, ballHSY = 1.5f;

void PlayerCollision(float& pos, float halfSizeY, float& vel ) 
{
	if (pos + halfSizeY > arenaHSY) 
	{
		pos = arenaHSY - playerHSY;
		vel *= -1;
	}

	if (pos - halfSizeY < -arenaHSY)
	{
		pos = -arenaHSY + playerHSY;
		vel *= -1;
	}
}

void BallReset() 
{
	ballAccX = 0.f;
	ballAccY = 0.f;

	ballX = 0.f;
	ballY = 0.f;

	//velX = 0.f;
	velY = 0.f;

}
void BallArenaCollision() 
{
	if (ballX + ballHSX > arenaHSX)// || ballX - ballHSX < -arenaHSX)
	{
		velX *= -1;
		BallReset();
		player2Score++;
	}
	else if(ballX - ballHSX < -arenaHSX)
	{
		velX *= -1;
		BallReset();
		player1Score++;
	}

	if (ballY + ballHSY > arenaHSY)
	{
		ballY = arenaHSY - ballHSY;
		velY *= -1;
	}

	else if (ballY - ballHSY < -arenaHSY)
	{
		ballY = -arenaHSY + ballHSY;
		velY *= -1;
	}
}

void BallCollision() 
{
	if (ballX + ballHSX > posX - playerHSX &&
		ballX - ballHSX < posX + playerHSX &&
		ballY + ballHSY > pos - playerHSY &&
		ballY - ballHSY < pos + playerHSY)
	{
		ballX = posX - playerHSX - ballHSX;
		velX *= -1;
		velY = (velY - pos) + vel * .75f;
	}
	else if (ballX + ballHSX > -posX - playerHSX &&
		ballX - ballHSX < -posX + playerHSX &&
		ballY + ballHSY > pos1 - playerHSY &&
		ballY - ballHSY < pos1 + playerHSY)
	{
		ballX = -posX + playerHSX + ballHSX;
		velX *= -1;
		velY = (velY - pos1) +  vel1 * .75f;
	}
}
void PlayerMovement(float& pos, float& vel, float acc, float dt) 
{
	//Velocity needs its own variation in time - picks up speed over time - derivative of velocity is acceleration
	//Acceleration is rate of change of velocity in time

	//Friction is change in acceleration based off of vel
	acc -= vel * 5.f;


	// Vel(2) = vel(1) + acc* dt
	vel = vel + acc * dt;
	// New pos pos(2) --> pos(1) + vel(2) * dt + (acc*dt^2)/ 2
	pos = pos + vel * dt + acc * dt * dt * .5f;
}
void BallMovement(float& vel, float& pos, float dt, float acc) 
{
	vel += acc * dt;
	pos += vel * dt + acc * dt * dt * .5f;

	
}

internal void
SimulateGame(Input* input, float dt, float speed)
{
	ClearScreen(0xff785670);

	float acc = 0.f, acc1 = 0.f;

	// (posY * speed) is units per frame - if the window is bigger the movement is slower
	// (posY * speed * dt) is units per sec 
	// posY += speed * dt --> is the velocity - rate of change of the pos in time (derivative of the pos) 

	if (IsDown(Button_Up)) //Player 2
	{
		acc += speed; 
	}
	if (IsDown(Button_Down))
	{
		acc -= speed;
	}

	

	if (IsDown(Button_W)) //Player 1
	{
		acc1 += speed;
	}
	if (IsDown(Button_S))
	{
		acc1 -= speed;
	}


	PlayerCollision(pos, playerHSY, vel);
	PlayerCollision(pos1, playerHSY, vel1);
	PlayerMovement(pos, vel, acc, dt);
	PlayerMovement(pos1, vel1, acc1, dt);


	//Ball Movement
	BallMovement(velX, ballX, dt, ballAccX);
	BallMovement(velY, ballY, dt, ballAccY);
	
	// Ball Collision
	BallCollision();
	BallArenaCollision();
	
	///..Drawing..///

	// x, y, scale, color
	DrawRec(0, 0, arenaHSX, arenaHSY, 0x1111111); // border

	DrawRec(posX, pos, playerHSX, playerHSY, 0xff76007); //paddle 1
	DrawRec(-posX, pos1, playerHSX, playerHSY, 0xff9684); //paddle 2

	DrawRec(ballX, ballY, ballHSX, ballHSY, 0x00FFFFFF); // ball

	// Tester changes
}	
