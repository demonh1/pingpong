#include "common.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>


//============================================
//	Init
//============================================
bool Init(int w, int h, int pal, int fullScreen) {
	if (SDL_Init(SDL_INIT_VIDEO)!= 0) {
		std::cerr << "Unable to initialize SDL:\n " << SDL_GetError() << std::endl;
		return false;
	}
	else {
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
		if (!fullScreen) { surface = SDL_SetVideoMode(w,h,pal,SDL_OPENGL|SDL_RESIZABLE); }
		else surface = SDL_SetVideoMode(w,h, pal,SDL_OPENGL|SDL_FULLSCREEN|SDL_RESIZABLE);
		return true;
	}
	if(!surface) { 
		std::cerr << "Unable to initialize SDL:\n " << SDL_GetError() << std::endl;
		return false;
		
	}
	return true;
}

//============================================================
//	void ResizeWindow
//	=========================================================
void ResizeWindow(int w, int h) { 

	if ( h == 0) h = 1;

	GLfloat ratio = (GLfloat)w/(GLfloat)h;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,ratio, 0.1,10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
//===================================
//	Mode2d
//==================================
void Mode2d (int w, int h) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool LoadTexture (const std::string& fileName, GLuint& tex) {
	SDL_Surface* tmp = nullptr;
	GLint maxTexSize;
	GLuint glFormat = GL_RGBA; //use alpha chanal

	//jpg -- no  alpha chanal
	if(fileName.substr(fileName.length()-3,3).compare("jpg") == 0) glFormat = GL_RGB;
	tmp = IMG_Load(fileName.c_str());

	if (tmp == nullptr) 
	{
		std::cout << "Error textures manager: " << fileName << " : "<< SDL_GetError() << std::endl;
		SDL_FreeSurface(tmp);
		return false;
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
	if(tmp->w > maxTexSize)
	{
		std::cout << "Error textures manager: " << fileName << " texture is big size." << std::endl;
		SDL_FreeSurface(tmp);
		return false;
	}
	// generate textures
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	//transmit image from tmp texture tex
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexImage2D(GL_TEXTURE_2D, 0, glFormat, tmp->w, tmp->h, 0, glFormat, GL_UNSIGNED_BYTE, tmp->pixels);

	SDL_FreeSurface(tmp);

	return true;
}

bool LoadTexNearest (const std::string& fileName, GLuint& tex) {
	SDL_Surface* tmp = nullptr;
	GLint maxTexSize;
	GLuint glFormat = GL_RGBA; //use alpha chanal

	//jpg -- no  alpha chanal
	if(fileName.substr(fileName.length()-3,3).compare("jpg") == 0) glFormat = GL_RGB;
	tmp = IMG_Load(fileName.data());

	if (tmp == nullptr) 
	{
		std::cout << "Error textures manager: " << fileName << " : "<< SDL_GetError() << std::endl;
		SDL_FreeSurface(tmp);
		return false;
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
	if(tmp->w > maxTexSize)
	{
		std::cout << "Error textures manager: " << fileName << " texture is big size." << std::endl;
		SDL_FreeSurface(tmp);
		return false;
	}
	// generate textures
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_NEAREST (текстура загружается без фильтрации)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexImage2D(GL_TEXTURE_2D, 0, glFormat, tmp->w, tmp->h, 0, glFormat, GL_UNSIGNED_BYTE, tmp->pixels);

	SDL_FreeSurface(tmp);

	return true;
}

//=======================================================
// void Draw
//отрисовка затекстурированного полигона в координатах x,y; 
//размером dX, dY; повернутом  на delta градусов 
// относительно верхней левой точки, либо относительно центра 
//======================================================
void Draw (float x, float y, float dX, float dY, float delta, int center)
{

	glEnable(GL_TEXTURE_2D); 
	glLoadIdentity();
	glTranslatef(x,y,0.0f); // задаем местоположение
	glRotatef(delta,0.0f,0.0f,-1.0f);

	// if rotate in center then smeshenie na 0.5
	if (center == 1) glTranslatef(-dX/2,-dY/2,0);

	//draw poligon
	glBegin(GL_QUADS);
	glTexCoord2i(0,0); glVertex2f(0, 0 ); // Top left angle
	glTexCoord2i(0,1); glVertex2f(0, dY); // Bottom left angle
	glTexCoord2i(1,1); glVertex2f(dX,dY); // Bottom right angle
	glTexCoord2i(1,0); glVertex2f(dX,0 ); // Top right angle
	glEnd();

	glLoadIdentity();
}

//=====================================================
void DrawPart (float w,// ширина текстуры (в пикселях)
	float h,// высота
	float t_x,// координаты верхнего левого угла "куска" 
	float t_y,// в текстуре (в пикселях)
	float t_dx,// размер вырезаемого куска
	float t_dy,

	float x,// 
	float y,
	float dX,
	float dY,
	float delta,
	int center)
{
	glEnable(GL_TEXTURE_2D); 
	glLoadIdentity();
	glTranslatef(x,y,0);
	glRotatef(delta,0,0,-1);

	if (center==1) glTranslatef(-dX/2,-dY/2,0); // смещение по центру

	glBegin(GL_QUADS);
	glTexCoord2f((t_x/w),(t_y/h));               glVertex2f(0, 0); // top left angle
	glTexCoord2f((t_x/w),((t_y+t_dy)/h));        glVertex2f(0, dY);// bottom left angle
	glTexCoord2f(((t_x+t_dx)/w),((t_y+t_dy)/h)); glVertex2f(dX,dY);// bottom right angle
	glTexCoord2f(((t_x+t_dx)/w),(t_y/h));        glVertex2f(dX, 0);// top right angle
	glEnd();

	glLoadIdentity();
}

void RenderTxt (int sdl_dx, int sdl_dy, TTF_Font *font, SDL_Color textColor, const char text[], GLuint texture)
{
	SDL_Surface *temp = nullptr,*tempb = nullptr;
	int w,h;
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	if(font == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		SDL_FreeSurface(temp);
		SDL_FreeSurface(tempb);
	}

	temp = TTF_RenderText_Blended(font, text, textColor);
	SDL_SetAlpha(temp, 0, 0);

	tempb = SDL_CreateRGBSurface(0, sdl_dx, sdl_dy, 32, rmask,gmask,bmask,amask);

	TTF_SizeUTF8(font,text, &w,&h);

	SDL_Rect src,dst;
	src.x=0;
	src.y=0;
	src.w=sdl_dx; //w
	src.h=sdl_dy; //h

	dst.x=0;
	dst.y=0;
	dst.w=sdl_dx;//w;
	dst.h=sdl_dy;//h;

	SDL_BlitSurface(temp, &src, tempb, &dst);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 
		GL_RGBA, 
		tempb->w, 
		tempb->h, 
		GL_RGBA, 
		GL_UNSIGNED_BYTE, 
		tempb->pixels);

	SDL_FreeSurface(temp);
	SDL_FreeSurface(tempb);
}

void DrawTxt(float x, float y, float dX, float dY, TTF_Font *font, const std::string& text, float delta, int center)
{
	SDL_Color txtColor = {255,255,255}; // white

	GLuint t_txt; // temp text texture	 
	glGenTextures(1, &t_txt);
	RenderTxt(dX, dY,font, txtColor, text.c_str(), t_txt);

	// рисуем полигон с текстовой текстурой
	glBindTexture( GL_TEXTURE_2D, t_txt);
	Draw(x,y, dX, dY, delta, center);

	glDeleteTextures(1, &t_txt);
}



int main () 
{
	
	// create window
	Init(width, height, pal,fullscreen);
	//ResizeWindow(width, height);

	// title window
	SDL_WM_SetCaption("PingPong",NULL);

	//SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"),NULL);

	//init 2d mode
	Mode2d(width, height);

	//color
	glClearColor(0,0,0,0);

	//enable texture
	glEnable(GL_TEXTURE_2D);

	// enable alpha chanal
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// load font
	TTF_Init();  
	TTF_Font *fonts;
	fonts = TTF_OpenFont("font1.ttf", height/16);

	srand(time(NULL));

	//load texture
	LoadTexture("back.jpg",back);
	LoadTexture("spr.png",sprite);

	Player::Instance() -> Start();
	Enemy::Instance() -> Start();

	Ball::Instance() -> StartBall();

	while(!done) {
		while (SDL_PollEvent(&event)){
			if (event.type==SDL_QUIT) done = true;

			// если единичный случай нажатия
			if (event.type==SDL_KEYDOWN)
			{
				// выход по ESC   
				if (event.key.keysym.sym==SDLK_ESCAPE) done = true;
			}      
		}

		keys = SDL_GetKeyState(NULL);

		if (keys[SDLK_LEFT]) Player::Instance()->getPlayer()->x = Player::Instance()->getPlayer()->x - Player::Instance()->getPlayer()->step; 
		if (keys[SDLK_RIGHT]) Player::Instance()->getPlayer()->x = Player::Instance()->getPlayer()->x + Player::Instance()->getPlayer()->step; 

		if (Player::Instance()->getPlayer()->x >= width-0.7*Player::Instance()->getPlayer()->dx) Player::Instance()->getPlayer()->x = width-0.7*Player::Instance()->getPlayer()->dx;
		if (Player::Instance()->getPlayer()->x < 0.7*Player::Instance()->getPlayer()->dx) Player::Instance()->getPlayer()->x = 0.7*Player::Instance()->getPlayer()->dx;

		if (Enemy::Instance()->getEnemy()->x >= width-0.7*Enemy::Instance()->getEnemy()->dx) Enemy::Instance()->getEnemy()->x=width-0.7*Enemy::Instance()->getEnemy()->dx;
		if (Enemy::Instance()->getEnemy()->x < 0.7*Enemy::Instance()->getEnemy()->dx) Enemy::Instance()->getEnemy()->x = 0.7*Enemy::Instance()->getEnemy()->dx;

		if (Ball::Instance()->getBallPos()->ballX>Enemy::Instance()->getEnemy()->x) Enemy::Instance()->getEnemy()->x=Enemy::Instance()->getEnemy()->x+Enemy::Instance()->getEnemy()->step;
		if (Ball::Instance()->getBallPos()->ballX<Enemy::Instance()->getEnemy()->x) Enemy::Instance()->getEnemy()->x=Enemy::Instance()->getEnemy()->x-Enemy::Instance()->getEnemy()->step;

		Ball::Instance()->getBallPos()->ballX += Ball::Instance()->getBallPos()->ballStepx;
		Ball::Instance()->getBallPos()->ballY += Ball::Instance()->getBallPos()->ballStepy;

		// мяч крутится вокруг своей оси
		Ball::Instance()->getBallPos()->ballDelta += 5;
		if (Ball::Instance()->getBallPos()->ballDelta>=360) Ball::Instance()->getBallPos()->ballDelta = 0;

		if (Ball::Instance()->getBallPos()->ballX > width-0.5*Ball::Instance()->getBallPos()->ballDy ||
			Ball::Instance()->getBallPos()->ballX < 0.5*Ball::Instance()->getBallPos()->ballDy)

			Ball::Instance()->getBallPos()->ballStepx = -Ball::Instance()->getBallPos()->ballStepx;

		// если мяч вылетает за экран, кто-то забил
		if (Ball::Instance()->getBallPos()->ballY > height-0.5*Ball::Instance()->getBallPos()->ballDy ||
			Ball::Instance()->getBallPos()->ballY < 0.5*Ball::Instance()->getBallPos()->ballDy) 
		{

			if (Ball::Instance()->getBallPos()->ballY > height-0.5*Ball::Instance()->getBallPos()->ballDy) Enemy::Instance()->getEnemy()->score++;
			else if (Ball::Instance()->getBallPos()->ballY < 0.5*Ball::Instance()->getBallPos()->ballDy) Player::Instance()->getPlayer()->score++;
			Ball::Instance()->StartBall();


		}

		// отражение от верхней центральной части ракетки
		if (Player::Instance()->getPlayer()->x - 0.4*Player::Instance()->getPlayer()->dx < Ball::Instance()->getBallPos()->ballX + 0.5*Ball::Instance()->getBallPos()->ballDx &&
			Player::Instance()->getPlayer()->x + 0.4*Player::Instance()->getPlayer()->dx > Ball::Instance()->getBallPos()->ballX - 0.5*Ball::Instance()->getBallPos()->ballDx &&
			Player::Instance()->getPlayer()->y - 0.5*Player::Instance()->getPlayer()->dy < Ball::Instance()->getBallPos()->ballY + 0.5*Ball::Instance()->getBallPos()->ballDy &&
			Player::Instance()->getPlayer()->y + 0.5*Player::Instance()->getPlayer()->dy > Ball::Instance()->getBallPos()->ballY-0.5*Ball::Instance()->getBallPos()->ballDy    )
		{
			Ball::Instance()->getBallPos()->ballStepy=-Ball::Instance()->getBallPos()->ballStepy;
			// симуляция передачи импульса мячу
			if (keys[SDLK_LEFT]) Ball::Instance()->getBallPos()->ballStepx = Ball::Instance()->getBallPos()->ballStepx-0.5*Player::Instance()->getPlayer()->step;
			if (keys[SDLK_RIGHT]) Ball::Instance()->getBallPos()->ballStepx = Ball::Instance()->getBallPos()->ballStepx+0.5*Player::Instance()->getPlayer()->step;   
		}  

		// отражение от левого бока ракетки
		if ( Ball::Instance()->getBallPos()->ballStepx > 0 &&
			Ball::Instance()->getBallPos()->ballStepx+0.5*Ball::Instance()->getBallPos()->ballDx > Player::Instance()->getPlayer()->x-0.6*Player::Instance()->getPlayer()->dx &&
			Ball::Instance()->getBallPos()->ballX+0.5*Ball::Instance()->getBallPos()->ballDx < Player::Instance()->getPlayer()->x-0.4*Player::Instance()->getPlayer()->dx &&
			Ball::Instance()->getBallPos()->ballY+0.5*Ball::Instance()->getBallPos()->ballDy > Player::Instance()->getPlayer()->y-0.5*Player::Instance()->getPlayer()->dy &&
			Ball::Instance()->getBallPos()->ballY-0.5*Ball::Instance()->getBallPos()->ballDy < Player::Instance()->getPlayer()->y+0.5*Player::Instance()->getPlayer()->dy  )

			Ball::Instance()->getBallPos()->ballStepx=-Ball::Instance()->getBallPos()->ballStepx;

		// от правого бока
		if ( Ball::Instance()->getBallPos()->ballStepx < 0 &&
			Ball::Instance()->getBallPos()->ballX - 0.5*Ball::Instance()->getBallPos()->ballDx < Player::Instance()->getPlayer()->x + 0.6*Player::Instance()->getPlayer()->dx &&
			Ball::Instance()->getBallPos()->ballX - 0.5*Ball::Instance()->getBallPos()->ballDx > Player::Instance()->getPlayer()->x + 0.4*Player::Instance()->getPlayer()->dx &&
			Ball::Instance()->getBallPos()->ballY + 0.5*Ball::Instance()->getBallPos()->ballDy > Player::Instance()->getPlayer()->y - 0.5*Player::Instance()->getPlayer()->dy &&
			Ball::Instance()->getBallPos()->ballY- 0.5*Ball::Instance()->getBallPos()->ballDy < Player::Instance()->getPlayer()->y +0.5*Player::Instance()->getPlayer()->dy    )

			Ball::Instance()->getBallPos()->ballStepx=-Ball::Instance()->getBallPos()->ballStepx;

		//взаимодействие мяча с ракеткой противника

		if (Enemy::Instance()->getEnemy()->x - 0.4*Enemy::Instance()->getEnemy()->dx < Ball::Instance()->getBallPos()->ballX + 0.5*Ball::Instance()->getBallPos()->ballDx &&
			Enemy::Instance()->getEnemy()->x + 0.4*Enemy::Instance()->getEnemy()->dx > Ball::Instance()->getBallPos()->ballX - 0.5*Ball::Instance()->getBallPos()->ballDx &&
			Enemy::Instance()->getEnemy()->y - 0.5*Enemy::Instance()->getEnemy()->dy < Ball::Instance()->getBallPos()->ballY + 0.5*Ball::Instance()->getBallPos()->ballDy &&
			Enemy::Instance()->getEnemy()->y + 0.5*Enemy::Instance()->getEnemy()->dy > Ball::Instance()->getBallPos()->ballY - 0.5*Ball::Instance()->getBallPos()->ballDy  )

			Ball::Instance()->getBallPos()->ballStepy=-Ball::Instance()->getBallPos()->ballStepy;

		// левый бок
		if ( Ball::Instance()->getBallPos()->ballStepx > 0 &&
			Ball::Instance()->getBallPos()->ballStepx+0.5*Ball::Instance()->getBallPos()->ballDx > Enemy::Instance()->getEnemy()->x-0.6*Player::Instance()->getPlayer()->dx &&
			Ball::Instance()->getBallPos()->ballX+0.5*Ball::Instance()->getBallPos()->ballDx < Enemy::Instance()->getEnemy()->x-0.4*Player::Instance()->getPlayer()->dx &&
			Ball::Instance()->getBallPos()->ballY+0.5*Ball::Instance()->getBallPos()->ballDy > Enemy::Instance()->getEnemy()->y-0.5*Player::Instance()->getPlayer()->dy &&
			Ball::Instance()->getBallPos()->ballY-0.5*Ball::Instance()->getBallPos()->ballDy < Enemy::Instance()->getEnemy()->y+0.5*Player::Instance()->getPlayer()->dy  )

			Ball::Instance()->getBallPos()->ballStepx=-Ball::Instance()->getBallPos()->ballStepx;
		// правый бок
		if ( Ball::Instance()->getBallPos()->ballStepx < 0 &&
			Ball::Instance()->getBallPos()->ballX - 0.5*Ball::Instance()->getBallPos()->ballDx < Enemy::Instance()->getEnemy()->x + 0.6*Enemy::Instance()->getEnemy()->dx &&
			Ball::Instance()->getBallPos()->ballX - 0.5*Ball::Instance()->getBallPos()->ballDx > Enemy::Instance()->getEnemy()->x + 0.4*Enemy::Instance()->getEnemy()->dx &&
			Ball::Instance()->getBallPos()->ballY + 0.5*Ball::Instance()->getBallPos()->ballDy > Enemy::Instance()->getEnemy()->y - 0.5*Enemy::Instance()->getEnemy()->dy &&
			Ball::Instance()->getBallPos()->ballY- 0.5*Ball::Instance()->getBallPos()->ballDy < Enemy::Instance()->getEnemy()->y +0.5*Enemy::Instance()->getEnemy()->dy    )

			Ball::Instance()->getBallPos()->ballStepx=-Ball::Instance()->getBallPos()->ballStepx;


		// рисуем фон
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,back);
		Draw(0,0,width,height,0,0);

		std::string s = std::to_string(Player::Instance()->getPlayer()->score) +":"+ std::to_string(Enemy::Instance()->getEnemy()->score);
		// вывод очков
		glColor4f(0,0,1,1);
		DrawTxt(0,//x
			0,//y
			width/8,
			height/16,
			fonts,//font
			s,
			0,//delta
			0);//center 
		glColor4f(1,1,1,1);	         

		// render ball                  
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,sprite);
		DrawPart(256,
			256,
			1,
			1,
			100,
			100,
			Ball::Instance()->getBallPos()->ballX,
			Ball::Instance()->getBallPos()->ballY,
			Ball::Instance()->getBallPos()->ballDx,
			Ball::Instance()->getBallPos()->ballDy,
			Ball::Instance()->getBallPos()->ballDelta,
			1);

		glColor4f(1,1,1,1);

		// render rocket player
		glColor4f(0,1,1,1);
		glBindTexture(GL_TEXTURE_2D,sprite);
		DrawPart(256,
			256,
			0,
			103,
			150,
			30,
			Player::Instance()->getPlayer()->x,
			Player::Instance()->getPlayer()->y,
			Player::Instance()->getPlayer()->dx,
			Player::Instance()->getPlayer()->dy,
			0,1);             
		glColor4f(1,1,1,1);

		// render rocket enemy
		glColor4f(1,0,1,1);
		glBindTexture(GL_TEXTURE_2D,sprite);
		DrawPart(256,
			256,
			0,
			103,
			150,
			30,
			Enemy::Instance()->getEnemy()->x,
			Enemy::Instance()->getEnemy()->y,
			Enemy::Instance()->getEnemy()->dx,
			Enemy::Instance()->getEnemy()->dy,
			0,1);             
		glColor4f(1,1,1,1);

		SDL_GL_SwapBuffers();


	}
	SDL_Quit();

	return 0;
}
