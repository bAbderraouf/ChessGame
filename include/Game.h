#pragma once

/******************
* Game Interface
******************/

class Game 
{
public:
	//Game(); no need (interface)
	virtual ~Game();

	virtual void Init() = 0;  // setup()
	virtual void Input() = 0; 
	virtual void Update() = 0; // logic
	virtual void Draw() = 0;	// render
};