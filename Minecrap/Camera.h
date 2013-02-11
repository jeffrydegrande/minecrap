//
//  Camera.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//



class Camera
{

	// camera positions
	float xpos;
	float ypos;
	float zpos;
	float xrot;
	float yrot; // face North

	int lastx;
	int lasty;


public:
	Camera(void);
	Camera(float xpos, float ypos, float zpos);
	Camera(Point3D point);

	~Camera(void);

	void moveLeft();
	void moveRight();
	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();

	
	void render();

	void look(int x, int y);
};



