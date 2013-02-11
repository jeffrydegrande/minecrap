#include "minecrap.h"
#include "Camera.h"

#include <math.h>


Camera::Camera()
{
	this->xpos = 0;
	this->ypos = 140;
	this->zpos = 0;
	this->xrot = 11;
	this->yrot = 0; // face North

	this->lastx = 0;
	this->lasty = 0;
}

Camera::Camera(float xpos, float ypos, float zpos) {
	this->xpos = xpos;
	this->ypos = ypos;
	this->zpos = zpos;
}

Camera::Camera(Point3D point) {
	this->xpos = point.x;
	this->ypos = point.y;
	this->zpos = point.z;
}

Camera::~Camera(void)
{
}

/*
- (int) bound:(int) value {
    if (value < -360) {
        value += 360;
    }
    if (value >= 360) {
        value -= 360;
    }
    
    return value;
}
*/

void Camera::moveLeft() {
	yrot -= 1;
}

void Camera::moveRight() {
	yrot += 1;
}

void Camera::strafeRight() {
    float yrotrad;
    yrotrad = (yrot / 180.0f * PI);
    xpos += (float) cos(yrotrad) * STRAFE_DISTANCE;
    zpos += (float) sin(yrotrad) * STRAFE_DISTANCE;
}

void Camera::strafeLeft() {
    float yrotrad;
    yrotrad = (yrot / 180 * PI);
    xpos -= (float) cos(yrotrad) * STRAFE_DISTANCE;
    zpos -= (float) sin(yrotrad) * STRAFE_DISTANCE;
}

void Camera::moveForward() {
    float xrotrad, yrotrad;
    xrotrad = (xrot / 180 * PI);
    yrotrad = (yrot / 180 * PI);
    
    xpos += (float)sin(yrotrad);
    ypos -= (float)sin(xrotrad);
    zpos -= (float)cos(yrotrad);
}

void Camera::moveBackward() {
    float xrotrad, yrotrad;
    xrotrad = (xrot / 180 * PI);
    yrotrad = (yrot / 180 * PI);
    
    xpos -= (float)sin(yrotrad);
    ypos += (float)sin(xrotrad);
    zpos += (float)cos(yrotrad);
}


/*
- (NSString *) stringFromDirection {    
    int absyrot = yrot;
    
    if (absyrot < 0)
        absyrot += 360;
    
    if (absyrot >= 0 && absyrot < 45) {
        return @"N";
    } else if (absyrot >= 45 && absyrot < 90) {
        return @"NE";
    } else if (absyrot >= 90 && absyrot < 135) {
        return @"E";
    } else if (absyrot >= 135 && absyrot < 180) {
        return @"SE";
    } else if (absyrot >= 180 && absyrot < 225) {
        return @"S";
    } else if (absyrot >= 225 && absyrot < 270) {
        return @"SW";
    } else if (absyrot >= 270 && absyrot < 315) {
        return @"W";
    } else if (absyrot >= 315 && yrot < 360) {
        return @"NW";
    } else {
        return @"Unknown";
    }
}


- (NSString *) stringFromPosition {
    return
    [NSString stringWithFormat:@"COORDS: x=%0.2f, y=%0.2f,z=%0.2f, xrot=%0.3f, yrot=%0.2f, facing %@",
     xpos,
     ypos,
     zpos,
     xrot,
     yrot,
     [self stringFromDirection]];
}
*/

void Camera::render() {
    glRotatef(xrot, 1.0, 0.0, 0.0);  //rotate our camera on the x-axis (left and right)
    glRotatef(yrot, 0.0, 0.0, 1.0);  //rotate our camera on they-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}

void Camera::look(int x, int y) {
	if (lastx == 0 && lasty == 0) {
        lastx = x;
        lasty = y;
    } else {
        int diffx = x - lastx; // check the difference between the current x and the last x position
        int diffy = y - lasty; // check the difference between the current y and the last y position
        lastx = x;             // set lastx to the current x position
        lasty = y;             // set lasty to the current y position
        
        // look up or down by the distance the mouse has travelled        
        xrot -= (float) diffy;
        // look left or right by the distance the mouse has travelled
        yrot += (float) diffx;
    }

}

/*
- (void) updateWithPoint: (NSPoint)point {
    
    if (lastx == 0 && lasty == 0) {
        lastx = point.x;
        lasty = point.y;
    } else {
        int diffx = point.x - lastx; // check the difference between the current x and the last x position
        int diffy = point.y - lasty; // check the difference between the current y and the last y position
        lastx = point.x;             // set lastx to the current x position
        lasty = point.y;             // set lasty to the current y position
        
        // look up or down by the distance the mouse has travelled
        float up = [self bound: xrot - (float) diffy];
        if ( up < 90 && up > -90)
            xrot = up;
        
        // look left or right by the distance the mouse has travelled
        yrot = [self bound: yrot + (float) diffx];
    }
}
*/