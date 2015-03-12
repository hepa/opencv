#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \property	union SColour
///
/// \brief	 the colour. 
///
/// \return	The s colour. 
////////////////////////////////////////////////////////////////////////////////////////////////////

struct SColour
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	SColour()
	///
	/// \brief	Default constructor.
	///
	/// \author	Caswal
	/// \date	18/05/2011
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SColour() {}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	SColour(unsigned int a_iColour)
	///
	/// \brief	Constructor.
	///
	/// \author	Caswal
	/// \date	18/05/2011
	///
	/// \param	a_iColour	colour in the form ARGB
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SColour(unsigned int a_iColour) : colour(a_iColour) { }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	SColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	///
	/// \brief	Constructor.
	///
	/// \author	Caswal
	/// \date	18/05/2011
	///
	/// \param	r	The red value (0-255)
	/// \param	g	The geen value (0-255)
	/// \param	b	The blue value (0-255) 
	/// \param	a	The alpha value (0-255)
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		colours.r = r;
		colours.g = g;
		colours.b = b;
		colours.a = a;
	}

	union
	{
		unsigned int colour;
		struct SColours
		{

			///< colours values between 0 and 255
			unsigned char a, r, g, b;
		} colours;
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int InitGL(int Width, int Height, bool fullscreen = false)
///
/// \brief	Initialises OpenGL. 
///
/// \author	Caswal
/// \date	7/09/2010
///
/// \param	Width		The width. 
/// \param	Height		The height. 
/// \param	fullscreen	true to fullscreen. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int InitGL(int Width, int Height, bool fullscreen = false);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int LoadTexture(const char* File)
///
/// \brief	Loads a texture. 
///
/// \author	Caswal
/// \date	7/09/2010
///
/// \param	File	The file. 
///
/// \return	The texture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int LoadTexture(const char* File);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void DrawSprite( int Sprite, int x, int y, int width, int height)
///
/// \brief	Draw a sprite. The coordinates are the centre of the sprite
///
/// \author	Caswal
/// \date	7/09/2010
///
/// \param	Sprite	The texture to Draw. 
/// \param	x		The x coordinate. 
/// \param	y		The y coordinate. 
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawSprite( int Sprite, float x, float y, float width, float height);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void DrawPoint (float x, float y, float size, const SColour& colour )
///
/// \brief	Draw a point. 
///
/// \author	Caswal
/// \date	7/09/2010
///
/// \param	x		The x coordinate. 
/// \param	y		The y coordinate. 
/// \param	size	The size. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawPoint (float x, float y, float size );


////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void FreeTexture( int Texture )
///
/// \brief	Free texture OpenGL Texture. 
///
/// \author	Caswal
/// \date	7/09/2010
///
/// \param	Texture	The texture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void FreeTexture( int Texture );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void DrawText( const char *Text, int x, int y, float Scale = 1.f)
///
/// \brief	Draw text to the screen. 
///
/// \author	Caswal
/// \date	7/09/2010
///
/// \param	TExt	The extent. 
/// \param	x		The x coordinate. 
/// \param	y		The y coordinate. 
/// \param	Scale	The scale. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawString( const char *Text, float X, float Y, float Scale );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void DrawSpriteRotated(int Sprite, int x, int y, int with, int height, float Rad )
///
/// \brief	Draw sprite rotated. 
///
/// \author	Caswal
/// \date	7/09/2010
///
/// \param	Sprite	The sprite. 
/// \param	x		The x coordinate. 
/// \param	y		The y coordinate. 
/// \param	with	The with. 
/// \param	height	The height. 
/// \param	Rad		The radians to rotate the sprite by (Rotated Around sprites centre). 
////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawSpriteRotated(int Sprite, float x, float y, float width, float height, float Rad );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void DrawLine(float a_fStartX, float a_fStartY, float a_fEndX, float a_fEndY);
///
/// \brief	Draw line.
///
/// \author	Caswal
/// \date	18/05/2011
///
/// \param	a_fStartX	start x coordinate. 
/// \param	a_fStartY	start y coordinate. 
/// \param	a_fEndX  	end x coordinate. 
/// \param	a_fEndY  	end y coordinate. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawLine(float a_fStartX, float a_fStartY, float a_fEndX, float a_fEndY);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn
/// void DrawLine(float a_fStartX, float a_fStartY, float a_fEndX, float a_fEndY,
/// SColour a_sColour);
///
/// \brief	Draw line.
///
/// \author	Caswal
/// \date	18/05/2011
///
/// \param	a_fStartX	start x coordinate. 
/// \param	a_fStartY	start y coordinate. 
/// \param	a_fEndX  	end x coordinate. 
/// \param	a_fEndY  	end y coordinate. 
/// \param	a_sColour	Line colour. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawLine(float a_fStartX, float a_fStartY, float a_fEndX, float a_fEndY, SColour a_sColour);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn
/// void DrawLine(float a_fStartX, float a_fStartY, float a_fEndX, float a_fEndY,
/// SColour a_sStartColour, SColour a_sEndColour);
///
/// \brief	Draw line.
///
/// \author	Caswal
/// \date	18/05/2011
///
/// \param	a_fStartX	  	start x coordinate. 
/// \param	a_fStartY	  	start y coordinate. 
/// \param	a_fEndX		  	end x coordinate. 
/// \param	a_fEndY		  	end y coordinate. 
/// \param	a_sStartColour	start colour. 
/// \param	a_sEndColour  	end colour. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawLine(float a_fStartX, float a_fStartY, float a_fEndX, float a_fEndY, SColour a_sStartColour, SColour a_sEndColour);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CloseDown( )
///
/// \brief	Closes down SDL, Cleans up Textures
///
/// \author	Caswal
/// \date	24/01/2011
///
////////////////////////////////////////////////////////////////////////////////////////////////////

void CloseDown();


////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool ClearScreen( )
///
/// \brief	Clears the Screen
///
/// \author	Caswal
/// \date	24/01/2011
///
///
/// \return	Returns nothing
////////////////////////////////////////////////////////////////////////////////////////////////////

void ClearScreen();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int ArrayToTexture(void* pixels, int width, int height)
///
/// \brief	Converts a array of pixels, preferably unsigned integers into an OpenGL texture
///			The dimensions should preferably be powers of 2, e.g. 512x256, 128x128 etc			
///
/// \author	Caswal
/// \date	09/02/2011
///
/// \param	pixels	The pixels/array you wish to convert into an OpenGL texture
/// \param	width	The width of the image, should preferably be a power of 2 
/// \param	height	The height of the image, should preferably be a power of 2
////////////////////////////////////////////////////////////////////////////////////////////////////
int ArrayToTexture(void* pixels, int width, int height);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void TextureToArray(int textureHandle, void* pixels, int width, int height)
///
/// \brief	Converts an OpenGL texture back into an array of pixel data. 
//
///
/// \author	Caswal
/// \date	09/02/2011
///
///	\param  textureHandle	The OpenGL texture handle to convert into a pixel array
/// \param	pixels			The pixels/array you wish to be filled with pixels from the OpenGL texture
/// \param	width			The width of the image, should preferably be a power of 2 
/// \param	height			The height of the image, should preferably be a power of 2
////////////////////////////////////////////////////////////////////////////////////////////////////
void TextureToArray(int textureHandle, void* pixels);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReadScreenPixels(void* pixels)
///
/// \brief	Reads the pixels off the screen and returns them
//
///
/// \author	Caswal
/// \date	09/02/2011
///
/// \param	pixels			The pixels/array you wish fill with the pixels from the screen
////////////////////////////////////////////////////////////////////////////////////////////////////
void ReadScreenPixels(void* pixels);


////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SetTintColour(float r, float g, float b, float a = 1.0f);
///
/// \brief	Draws everything from the function call in the tint colour set
///
///
/// \author	Caswal
/// \date	09/02/2011
///
///	\param  r	How much Red, values between 0, 1
/// \param	g	How much Green, values between 0, 1
/// \param	b	How much Blue, values between 0, 1
/// \param	a 	How much Alpha, values between 0, 1
////////////////////////////////////////////////////////////////////////////////////////////////////

void SetTintColour(float r, float g, float b, float a = 1.0f);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool FrameworkUpdate( )
///
/// \brief	Updates the Framework, must be called as the last thing in a loop. 
///
/// \author	Caswal
/// \date	24/01/2011
///
///
/// \return	Returns false if the program should exit
////////////////////////////////////////////////////////////////////////////////////////////////////

bool FrameworkUpdate();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int GetFPS( )
///
/// \brief	Gets the FPS, calculated from FrameworkUpdate
///
/// \author	Caswal
/// \date	24/01/2011
///
///
/// \return	Returns false if the program should exit
////////////////////////////////////////////////////////////////////////////////////////////////////

int GetFPS();


////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void DrawFPS( )
///
/// \brief	Draws the FPS, calculated from FrameworkUpdate
///
/// \author	Caswal
/// \date	24/01/2011
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawFPS();

const int KEY_BACKSPACE	= 8;
const int KEY_TAB		= 9;
const int KEY_CLEAR		= 12;
const int KEY_RETURN	= 13;
const int KEY_PAUSE		= 19;
const int KEY_ESCAPE	= 27;

const int KEY_UP		= 273;
const int KEY_DOWN		= 274;
const int KEY_RIGHT		= 275;
const int KEY_LEFT		= 276;
const int KEY_INSERT	= 277;
const int KEY_HOME		= 278;
const int KEY_END		= 279;
const int KEY_PAGEUP	= 280;
const int KEY_PAGEDOWN	= 281;

const int KEY_F1		= 282;
const int KEY_F2		= 283;
const int KEY_F3		= 284;
const int KEY_F4		= 285;
const int KEY_F5		= 286;
const int KEY_F6		= 287;
const int KEY_F7		= 288;
const int KEY_F8		= 289;
const int KEY_F9		= 290;
const int KEY_F10		= 291;
const int KEY_F11		= 292;
const int KEY_F12		= 293;
const int KEY_F13		= 294;
const int KEY_F14		= 295;
const int KEY_F15		= 296;

const int KEY_NUMLOCK	= 300;
const int KEY_CAPSLOCK	= 301;
const int KEY_SCROLLOCK	= 302;
const int KEY_RSHIFT	= 303;
const int KEY_LSHIFT	= 304;
const int KEY_RCTRL		= 305;
const int KEY_LCTRL		= 306;
const int KEY_RALT		= 307;
const int KEY_LALT		= 308;
const int KEY_SPACE		= 32;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool IsKeyDown( int key );
///
/// \brief Gets if a key is down, either use letters in single quotes, e.g. 'a' or the key code above
///
/// \author	Caswal
/// \date	24/01/2011
///
/// \param	key	The key to see if it is pressed. 
///
/// \return Whether the key is down or not
////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsKeyDown(int key);


////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int GetMouseX();
///
/// \brief Gets the mouses's X coordinate
///
/// \author	Caswal
/// \date	9/02/2011
///
///
/// \return The mouse's X coordinate
////////////////////////////////////////////////////////////////////////////////////////////////////
int GetMouseX();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int GetMouseY();
///
/// \brief Gets the mouses's Y coordinate
///
/// \author	Caswal
/// \date	9/02/2011
///
///
/// \return The mouse's X coordinate
////////////////////////////////////////////////////////////////////////////////////////////////////
int GetMouseY();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool IsKeyDown( int key );
///
/// \brief Gets if a key is down, either use letters in single quotes, e.g. 'a' or the key code above
///
/// \author	Caswal
/// \date	24/01/2011
///
/// \param	key	The mouse button that is pressed, 1 for left mouse, 2 for middle, 3 for right
///
/// \return Whether The mouse button is down or not.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GetMouseButtonDown(int button);