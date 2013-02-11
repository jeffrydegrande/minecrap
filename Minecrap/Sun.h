#ifndef MINECRAP_SUN_H
#define MINECRAP_SUN_H

#include "minecrap.h"

class Sun
{
	GLuint sun;

public:
	Sun(void);
	~Sun(void);

	int render();
private:

	void draw();
	void drawBlock();
};

#endif
