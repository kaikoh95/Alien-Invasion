#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <fstream>
#include <math.h>
#include "loadTGA.h"

using namespace std;

#define TOTAL_DROPLETS 1000000
#define TOTAL_SNOW 500
#define GRAVITY -0.1

GLUquadric *q;
GLuint txId[9];

int check = 0;
int robots_move = 0;

int loop;
int is_snowing = 1;
float decrease_speed = 2.0;
float velocity = 0.0;
float z_change = -40.0;

int water_drops = 0;
int create_curtain = 0;

int loaded = 1;
int bullet_fire = 0;
int bullets_move = -8;

int launch = 0;
int spaceship_move_y = 0;
int spaceship_move = 0;
int restart_launch = 0;

int toggle = 0;
int cam_height = 300;
float theta = 0.0;
float angle_left_right = 0.0;
float angle_up_down = 1.0;

float light_position[4] = {-30,50,-20,1};
float light_position_1[4] = {30,50,20,0};

/**
 * @brief randomize - generates a randomized number
 * @return double
 */
double randomize(void) {
    return (rand() % 100000) / 100000.0;
}

/**
 * @brief loadTexture - initializes textures to be used
 */
void loadTexture(void) {
    glGenTextures(9, txId);

    glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadTGA("./textures/sky_top.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("./textures/sky_bottom.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("./textures/sky_left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("./textures/sky_right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[4]);
    loadTGA("./textures/sky_back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadTGA("./textures/sky_front.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[6]);
    loadTGA("./textures/castle_top_wall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[7]);
    loadTGA("./textures/castle_top_stubs.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[8]);
    loadTGA("./textures/brick.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

/**
 * @brief skybox - sets up skybox texture
 */
void skybox() {
    glEnable(GL_TEXTURE_2D);

    // left
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000,  0, 1000);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1000, 0., -1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1000, 1000., -1000);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1000, 1000, 1000);
    glEnd();

    // front
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000,  0, -1000);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1000, 0., -1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000, 1000, -1000);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1000,  1000, -1000);
    glEnd();

    // right
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(1000,  0, -1000);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1000, 0, 1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000, 1000,  1000);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(1000,  1000,  -1000);
    glEnd();

    // rear
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex3f( 1000, 0, 1000);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1000, 0,  1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1000, 1000,  1000);
    glTexCoord2f(0.0, 1.0);
    glVertex3f( 1000, 1000, 1000);
    glEnd();

    // top
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1000, 1000, -1000);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1000, 1000,  -1000);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1000, 1000,  1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1000, 1000, 1000);
    glEnd();

    // floor
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000, 0., 1000);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1000, 0.,  1000);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000, 0., -1000);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1000, 0., -1000);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

/**
 * @brief idle - post redisplay for glut
 */
void idle(void) {
    glutPostRedisplay();
}

/**
 * @brief bullet_timer - timer to keep track of bullet animation
 * @param time - for glutTimerFunc
 */
void bullet_timer(int time) {
	glutPostRedisplay();

    if (bullet_fire == 1) {
        bullets_move += 20;
    }
    if (bullets_move > 200) {
        bullet_fire = 0;
        bullets_move = -8;
    } else {
        glutTimerFunc(60, bullet_timer, 0);
    }
}

/**
 * @brief spaceship_timer - timer for spaceship animation
 * @param time - for glutTimerFunc
 */
void spaceship_timer(int time) {
    glutPostRedisplay();

    if (spaceship_move == 1 && spaceship_move_y < 1000) {
        spaceship_move_y++;
    }
    glutTimerFunc(60, spaceship_timer, 0);

}

/**
 * @brief robot_timer - timer for robot animation
 * @param time - for glutTimerFunc
 */
void robot_timer(int time) {
	glutPostRedisplay();
    if (theta < 30 && check == 0) {
        theta++;
    } else if (theta == 30) {
		check = 1;
    } else if (theta == -30) {
        check = 0;
    }
    if (check == 1) {
        theta--;
	}
    robots_move++;
	glutTimerFunc(20, robot_timer, 0);
}

/// Waterfall Sprinkler Structure (Particle System)
typedef struct {
    int sprinkle;
    float x_coord, y_coord;
    float x_change, y_change;
    float mass;
} Particle;
Particle sprinkler[TOTAL_DROPLETS];

/// Snowfall (Particle System)
typedef struct {
    bool alive;
    float lifespan;
    float decay;

    float x_pos;
    float y_pos;
    float z_pos;

    float velocity;
    float gravity;
} Snow;
Snow snowfall[TOTAL_SNOW];

/**
 * @brief init_snowfall - Initialize snowfall
 * @param i
 */
void init_snowfall(int i) {
    snowfall[i].alive = true;
    snowfall[i].lifespan = float(1);
    snowfall[i].decay = float(rand() % 100) / 1000;

    snowfall[i].x_pos = float(rand() % 1000) - 10;
    snowfall[i].y_pos = float(rand() % 1000);
    snowfall[i].z_pos = float(rand() % 1000) - 10;
}

/**
 * @brief draw_snow - Displays snowfall
 */
void draw_snow() {
    float x, y, z;
    for (loop = 0; loop < TOTAL_SNOW; loop=loop+2) {
        if (snowfall[loop].alive == true && is_snowing == 1) {
            // snow at castle front
            glPushMatrix();
                glColor3f(1.0, 1.0, 1.0);
                glTranslatef(snowfall[loop].x_pos-500, snowfall[loop].y_pos, snowfall[loop].z_pos+z_change);
                glutSolidSphere(1, 16, 16);
            glPopMatrix();

            // snow at castle back
            glPushMatrix();
                glColor3f(1.0, 1.0, 1.0);
                glTranslatef(snowfall[loop].x_pos-500, snowfall[loop].y_pos, -(snowfall[loop].z_pos+z_change));
                glutSolidSphere(1, 16, 16);
            glPopMatrix();

            // animate snow using physics of velocity and gravity
            if ((snowfall[loop].y_pos + (velocity / (decrease_speed * 100))) > 0) {
                snowfall[loop].y_pos += velocity / (decrease_speed * 100);
                velocity += float(GRAVITY);
                snowfall[loop].lifespan -= snowfall[loop].decay;
            } else {
                snowfall[loop].lifespan = 0;
            }
            // restart snowfall if lifespan is up
            if (snowfall[loop].lifespan <= float(0)) {
                init_snowfall(loop);
            }
        }
    }
}

/**
 * @brief sprinkle_time - function to animate sprinkler
 */
void sprinkle_time(void) {
    int i;
    for (i=0; i<water_drops; i++) {
        if (sprinkler[i].sprinkle) {
            // physics of gravity
            if (sprinkler[i].y_coord + float(GRAVITY) * sprinkler[i].mass < float(-0.75) && create_curtain == 1) {
                // create waterfall curtain
                sprinkler[i].y_change = -sprinkler[i].y_change;
            }
            else {
                // just sprinkle on to ground
                sprinkler[i].y_change += float(GRAVITY) * sprinkler[i].mass;
            }
            sprinkler[i].x_coord += sprinkler[i].x_change;
            sprinkler[i].y_coord += sprinkler[i].y_change;
            if (sprinkler[i].y_coord < float(-1) || sprinkler[i].x_coord > float(1)) {
                sprinkler[i].sprinkle = 0;
            }
        }
    }
}

/**
 * @brief drop_generator - generates water droplets
 */
void drop_generator(void) {
    int i;
    float new_droplets = float(randomize() * 100);
    if (water_drops + new_droplets >= TOTAL_DROPLETS) {
        new_droplets = TOTAL_DROPLETS - water_drops;
    }
    for (i=water_drops; i<water_drops+new_droplets; i++) {
        sprinkler[i].sprinkle = 1;
        sprinkler[i].x_coord = float(-0.8 + 0.01 * randomize());
        sprinkler[i].y_coord = float(0.8 + 0.01 * randomize());
        sprinkler[i].x_change = float(0.0075 + 0.0025 * randomize());
        sprinkler[i].y_change = 0.0;
        sprinkler[i].mass = float(0.5 + 0.5 * randomize());
    }
    i = water_drops;
    water_drops += new_droplets;
}

/**
 * @brief draw_sprinkler - displays sprinkling of water droplets
 */
void draw_sprinkler(void) {
    int i;
    glColor3f(0, float(0.4), 1);
    glBegin(GL_POINTS);
    for (i=0; i<water_drops; i++) {
        if (sprinkler[i].sprinkle) {
            glVertex2f(sprinkler[i].x_coord, sprinkler[i].y_coord);
        }
    }
    glEnd();
    glFlush();
}

/**
 * @brief hydrant_base - water hydrant pillars
 */
void hydrant_base(void) {
    // left body
    glPushMatrix();
        glColor4f(float(0.4), 0, float(0.1), 0);
        glTranslatef(-82, 42, -20);
        glTranslatef(0,-13,0);
        glScalef(5,25,5);
        glutSolidCube(1.5);
    glPopMatrix();

    // right body
    glPushMatrix();
        glColor4f(float(0.4), 0, float(0.1), 0);
        glTranslatef(23, 42, -20);
        glTranslatef(0,-13,0);
        glScalef(5,25,5);
        glutSolidCube(1.5);
    glPopMatrix();

    // left spout
    glPushMatrix();
        glTranslatef(-80.0, 39.0, -20.0);
        glRotatef(30.0, 0., 0., 1.);
        glRotatef(90.0, 0., 1., 0.);
        gluCylinder(q, 2.5, 2.5, 14.0, 20, 5);
        glTranslatef(0.0, 0.0, 14.0);
        gluDisk(q, 0.0, 2.5, 20.0, 3);
    glPopMatrix();

    // right spout
    glPushMatrix();
        glTranslatef(6, 46.0, -20.0);
        glRotatef(-28.0, 0., 0., 1.);
        glRotatef(90.0, 0., 1., 0.);
        gluCylinder(q, 2.5, 2.5, 14.0, 20, 5);
        glTranslatef(0.0, 0.0, 14.0);
        gluDisk(q, 0.0, 2.5, 20.0, 3);
    glPopMatrix();

    // left sprinkler
    glPushMatrix();
        glTranslatef(-51, 30.5, -20);
        glScalef(20, 20, 20);
        drop_generator();
        draw_sprinkler();
        sprinkle_time();
    glPopMatrix();

    // right sprinkler
    glPushMatrix();
        glTranslatef(-11, 30.5, -20);
        glScalef(20, 20, 20);
        glRotatef(-180.0, 0., 1., 0.);
        drop_generator();
        draw_sprinkler();
        sprinkle_time();
    glPopMatrix();
}

/**
 * @brief top_wall - wall blocks to be placed on top of castle walls
 */
void top_wall(void) {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[7]);
    glBegin(GL_QUADS);

        // front
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0, 5.0, 0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(5.0, 5.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(5.0, 0.0, 0.0);

         // back
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 10.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0, 5.0, 10.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(5.0, 5.0, 10.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(5.0, 0.0, 10.0);

        // left
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(0.0, 0.0, 10.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(0.0,5.0, 10.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0, 5.0, 0.0);

         // right
        glTexCoord2f(0.0,0.0);
        glVertex3f(5.0, 0.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(5.0, 0.0, 10.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(5.0, 5.0, 10.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(5.0, 5.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[7]);
    glBegin(GL_QUADS);

        // top
        glTexCoord2f(0.0,0.0);
        glVertex3i(0.0, 5.0, 0.0);
        glTexCoord2f(0.0,1.0);
        glVertex3i(0.0, 5.0, 10.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(5.0, 5.0, 10.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(5.0, 5.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/**
 * @brief wall - castle walls
 */
void wall(void) {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);

        // front
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0, 50.0, 0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 0.0);
            
        // back
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 80.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0, 50.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 80.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 80.0);
             
        // left
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(0.0, 0.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(0.0,50.0, 80.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0,50.0, 0.0);
        
        // right
        glTexCoord2f(0.0,0.0);
        glVertex3f(20.0, 0.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0,50.0, 80.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(20.0,50.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[8]);
	glBegin(GL_QUADS);

        // top
        glTexCoord2f(0.0,0.0);
        glVertex3i(0.0, 50.0, 0.0);
        glTexCoord2f(0.0,1.0);
        glVertex3i(0.0, 50.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 80.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 50.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(9,50,20);
        top_wall();
        glTranslatef(0,0,20);
        top_wall();
        glTranslatef(0,0,20);
        top_wall();

    glPopMatrix();
}

/**
 * @brief short_wall - castle top wall blocks
 */
void short_wall(void) {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);

        // front
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0, 50.0, 0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 0.0);

        // back
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 61.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0, 50.0, 61.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 61.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 61.0);

        // left
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(0.0, 0.0, 61.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(0.0,50.0, 61.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0,50.0, 0.0);

        // right
        glTexCoord2f(0.0,0.0);
        glVertex3f(20.0, 0.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 61.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0,50.0, 61.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(20.0,50.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[8]);
    glBegin(GL_QUADS);

        // top
        glTexCoord2f(0.0,0.0);
        glVertex3i(0.0, 50.0, 0.0);
        glTexCoord2f(0.0,1.0);
        glVertex3i(0.0, 50.0, 61.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 61.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 50.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(9,50,20);
        top_wall();
        glTranslatef(0,0,20);
        top_wall();
        glTranslatef(0,0,20);
        top_wall();
    glPopMatrix();
}

/**
 * @brief castle_back - back pillars of castle
 */
void castle_back(void) {
    glPushMatrix();
        glColor3f(0., 0., 0.);
        glTranslatef(100,50,-95);
        glRotatef(90,1,0,0);
        GLUquadric *q = gluNewQuadric();
        gluCylinder(q,13,13,50.,30.,30);
        glColor3f(1., 0., 0.);
        glRotatef(180,1,0,0);
        glScalef(5, 5, 20);
        glutSolidCone(2.7,1,50,5);
        gluQuadricDrawStyle(q, GLU_FILL);
    glPopMatrix();
}

/**
 * @brief castle - front pillars and gate of castle
 */
void castle(void) {
    glPushMatrix();
        glColor3f(float(0.2), float(0.2), float(0.2));
        glTranslatef(-30, 50, 1.5);
        glRotatef(90, 90, 0, 0);
        GLUquadric *q = gluNewQuadric();
        gluCylinder(q, 12.0, 12.0, 50, 25, 5);
    glPopMatrix();

    glPushMatrix();
        glColor3f(float(0.2), float(0.2), float(0.2));
        glTranslatef(30, 50, float(2.7));
        glRotatef(90, 90, 0, 0);
        gluCylinder(q, 12.0, 12.0, 50, 25, 5);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(-29.5, 50, 2);
        glRotatef(-90,1,0,0);
        glScalef(5, 5, 20);
        glutSolidCone(2.4,1,50,5);
    glPopMatrix();

	glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(29.5, 50, 3);
        glRotatef(-90,1,0,0);
        glScalef(5, 5, 20);
        glutSolidCone(2.4,1,50,5);
	glPopMatrix();

    glPushMatrix();
        glColor3f(float(0.5), float(0.5), float(0.5));
        glTranslatef(0,38,0);
        glRotatef(0,1,0,0);
        glScalef(60, 15, 20);
        glutSolidCube(1);    
    glPopMatrix();

    // set up wall faces
    glPushMatrix();
        glTranslatef(30,0,10.5);
        glRotatef(90,0,1,0);
        wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(110,0,-9);
        glRotatef(180,0,1,0);
        wall();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(30, 0, -105);
        glRotatef(90,0,1,0);
        wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(30, 0, -125);
        glRotatef(270,0,1,0);
        short_wall();
    glPopMatrix();
    
    glPushMatrix();
        castle_back();
        glTranslatef(-200,0,-1);
        castle_back();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(-30,0,-10);
        glRotatef(-90,0,1,0);
        wall();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-110, 0, -105);
        glRotatef(-270,0,1,0);
        wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(-90,0,-9);
        glRotatef(180,0,1,0);
        wall();
    glPopMatrix();
}

/**
 * @brief spaceship - Spaceship object
 */
void spaceship(void) {
    // top - surface shape generated using an equation
    GLdouble eqn [4]={0.0,1.0,0.0,0.0}; 
    glPushMatrix();
        glColor3f(0, 0, float(0.4));
        glTranslatef(0, 60, -30);
        glClipPlane(GL_CLIP_PLANE0, eqn);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(q, double(5.3), 32, 32);
        glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
    
    // middle
    glPushMatrix();
        glColor4f(float(0.6), float(0.6), float(0.6), 1.0);
        glTranslatef(0,60,-30);
        glRotatef(90,1,0,0);
        GLUquadric *b = gluNewQuadric();
        gluCylinder(b,5.3,5.3,50,30,30);
        gluQuadricDrawStyle(q, GLU_FILL);
    glPopMatrix();

    // bottom
    glPushMatrix();
        glColor4f(1., 0., 0., 0.);
        glTranslatef(0,20,-30);
        glRotatef(90,1,0,0);
        GLUquadric *q = gluNewQuadric();
        gluCylinder(q,2.5,7.5,25,30,30);
        gluQuadricDrawStyle(q, GLU_FILL);
        glColor4f(0, 255, 0, 1.0);
        glTranslatef(0, 7, -30);
        glRotatef(-robots_move-float(10.5),0,1,0);
        glutSolidCube(4);
        glTranslatef(10, 0, -10);
        glRotatef(-robots_move-20,0,1,0);
        glutSolidCube(4);
    glPopMatrix();
}

/**
 * @brief cannon - Cannon object
 */
void cannon(void) {
    // base of cannon
    glPushMatrix();
        glColor4f(float(0.2), float(0.3), float(0.3), 0);
        glTranslatef(-25, 3.0, 20);
        glScalef(10.0, -5.0, 10.0);
        glutSolidCube(1.0);
    glPopMatrix();

    // torso of cannon
    glPushMatrix();
        glColor4f(float(0.4), float(0.26), float(0.12), 0);
        glTranslatef(-15, 10, 20.0);
        glRotatef(-90.0, 0, 1, 0);
        gluCylinder(q, 7.0, 5.0, 30.0, 25, 5);
        glColor4f(0, 0, 0, 0);
        glTranslatef(0.0, 0.0, 30.0);
        gluDisk(q, 0.0, 5, 25, 3);
    glPopMatrix();
}

/**
 * @brief bullet - Bullet object
 */
void bullet(void) {
    glPushMatrix();
        glColor4f(255, 0, 0, 0.);
        glTranslatef(20, 0, 50);
        gluCylinder(q, 3, 2, 10, 25, 10);
        glTranslatef(0.0, 0.0, 10);
        gluDisk(q, 0.0, 2, 25, 10);
    glPopMatrix();
}

/**
 * @brief robot - Creates robot object
 * @param regular - 0 for shadow, 1 for regular robot
 */
void robot(int regular) {
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);

    // set up head
    if (regular == 1) {
        glColor3f(255, 0, 0);
    } else {
        glColor4f(float(0.2), float(0.2), float(0.2), float(1.0));
    }
    glPushMatrix();
        glTranslatef(0, float(6.7), 0);
        glutSolidCube(1.4);
    glPopMatrix();

    // set up face
    if (regular == 1) {
        glColor3f(0, 50, 100);
    }
    glPushMatrix();
        glTranslatef(float(-0.3), 7, float(0.7));
        glutSolidCube(.2);
        glTranslatef(float(0.6), 0, 0);
        glutSolidCube(.2);
        glTranslatef(float(-0.3), float(-0.3), 0);
        glutSolidCube(.2);
        glTranslatef(0, float(-0.3), 0);
        glScalef(3, .5, 1);
        glutSolidCube(.2);
    glPopMatrix();

    // set up body
    if (regular == 1) {
        glColor3f(0, 0, 0);
    }
    glPushMatrix();
        glTranslatef(0, 4.5, 0);
        glScalef(3, 3, float(1.2));
        glutSolidCube(1);
    glPopMatrix();

    // set up legs
    if (regular == 1) {
        glColor3f(0, 0, 1);
    }
    glPushMatrix();
        glTranslatef(0, 4.5, 0);
        glRotatef(-theta, 1, 0, 0);
        glTranslatef(float(-0.8), -3, 0);
        glScalef(1, 4, 1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 4.5, 0);
        glRotatef(theta, 1, 0, 0);
        glTranslatef(float(0.8), -3, 0);
        glScalef(1, 4, 1);
        glutSolidCube(1);
    glPopMatrix();

    // set up skis
    if (regular == 1) {
        glColor3f(1, 1, 1);
    }
    glPushMatrix();
        glTranslatef(0, 2, 0);
        glTranslatef(float(-0.8), float(-2.5), 0);
        glRotatef(90, 1, 0, 0);
        glScalef(1, 4, 1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 2, 0);
        glTranslatef(0.5, -2.5, 0);
        glRotatef(90, 1, 0, 0);
        glScalef(1, 4, 1);
        glutSolidCube(1);
    glPopMatrix();

    // set up arms
    if (regular == 1) {
        glColor3f(0, 0, 0.5);
    }
    glPushMatrix();
        glTranslatef(-2, 5, 0);
        glRotatef(theta, 1, 0, 0);
        glScalef(1, 3, 1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(2, 5, 0);
        glRotatef(-theta, 1, 0, 0);
        glScalef(1, 3, 1);
        glutSolidCube(1);
    glPopMatrix();
    glFlush();
}

/**
 * @brief robot_shadow - creates shadow of robots
 */
void robot_shadow(void) {
    float shadowMatrix[16] =
    {
        light_position[1], 0 ,0, 0,
        -light_position[0], 0,
        -light_position[2], -1, 0, 0,
        light_position[1], 0, 0, 0, 0,
        light_position[1]
    };
    glDisable(GL_LIGHTING);

    glPushMatrix();
        glMultMatrixf(shadowMatrix);
        glColor4f(float(0.6), float(0.6), float(0.6), float(1.0));
        glTranslatef(0, 10, 20);
        glRotatef(90, 0, 1, 0);
        glScalef(2, 2, 2);
        robot(0);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glPushMatrix();
        glTranslatef(0, 0, 20);
        glRotatef(90 ,0, 1 ,0);
        glScalef(2, 2, 2);
        robot(1);
    glPopMatrix();
}

/**
 * @brief robot_walking - animates robots
 * @param check - 0 for stop animation, 1 for animate
 */
void robot_walking(int check) {

    if (check == 1) {
        glPushMatrix();
            glTranslatef(0,3,-50);
            glRotatef(-robots_move-float(10.5), 0, 1, 0);
            glTranslatef(0,0,-160);
            robot_shadow();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,3,-50);
            glRotatef(-robots_move-float(10.5*5), 0, 1, 0);
            glTranslatef(0,0,-160);
            robot_shadow();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,3,-50);
            glRotatef(-robots_move-float(10.5*10), 0, 1, 0);
            glTranslatef(0,0,-160);
            robot_shadow();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,3,-50);
            glRotatef(-robots_move-float(10.5*15), 0, 1, 0);
            glTranslatef(0,0,-160);
            robot_shadow();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,3,-50);
            glRotatef(-robots_move-float(10.5*20), 0, 1, 0);
            glTranslatef(0,0,-160);
            robot_shadow();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,3,-50);
            glRotatef(-robots_move-float(10.5*25), 0, 1, 0);
            glTranslatef(0,0,-160);
            robot_shadow();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,3,-50);
            glRotatef(-robots_move-float(10.5*30), 0, 1, 0);
            glTranslatef(0,0,-160);
            robot_shadow();
        glPopMatrix();
    }
}

/**
 * @brief glut_key_pressed - special function to recognize keys pressed
 * @param key - the key that was pressed
 * @param x - for glutSpecialFunc
 * @param y - for glutSpecialFunc
 */
void glut_key_pressed(int key, int x, int y) {
    if(key == GLUT_KEY_UP) { // pan camera in
        cam_height--;
    } else if(key == GLUT_KEY_DOWN) { // pan camera out
        cam_height++;
    } else if(key == GLUT_KEY_RIGHT) { // turn camera right
        angle_left_right--;
    } else if(key == GLUT_KEY_LEFT) { // turn camera left
        angle_left_right++;
    } else if (key == GLUT_KEY_HOME) { // toggle camera view
        if (toggle == 0) {
            if (cam_height <= 500 && spaceship_move_y < 100) {
                cam_height += spaceship_move_y;
            } else {
                cam_height = 500;
            }
            angle_left_right = 0;
            angle_up_down = 80;
            toggle = 1;
        } else if (toggle == 1) {
            cam_height = 300;
            angle_left_right = 0;
            angle_up_down = 1.0;
            toggle = 0;
        }
    }
    glutPostRedisplay();
}

/**
 * @brief key_pressed - recognized ASCII keys pressed
 * @param key - key that was pressed
 * @param x - for glutKeyboardFunc
 * @param y - for glutKeyboardFunc
 */
void key_pressed(unsigned char key, int x, int y) {
    if (key == 'c' || key == 'C') { // fire cannon
        if (bullet_fire == 0) {
            bullet_fire = 1;
            glutTimerFunc(20, bullet_timer, 0);
        }
    } else if (key == 'l' || key == 'L') { // launch spaceship
        if (launch == 0 && spaceship_move == 0) {
            launch = 1;
            spaceship_move = 1;
            if (restart_launch == 0) {
                glutTimerFunc(20, spaceship_timer, 0);
            }
        }
    } else if (key == 'x' || key == 'X') { // angle camera up
        angle_up_down++;
    } else if (key == 'z' || key == 'Z') { // angle camera down
        angle_up_down--;
    } else if (key == 'w' || key == 'W') { // toggle waterfall curtain
        water_drops = 0;
        if (create_curtain == 0) {
            create_curtain = 1;
        } else {
            create_curtain = 0;
        }
    } else if (key == 'r' || key == 'R') { // repeat spaceship launch
        if (launch == 1 && spaceship_move == 1) {
            launch = 0;
            spaceship_move_y = 0;
            spaceship_move = 0;
            restart_launch = 1;
        }
    } else if (key == 's' || key == 'S') { // toggle snowfall
        velocity = 0;
        if (is_snowing == 0) {
            is_snowing = 1;
        } else {
            is_snowing = 0;
        }
    } else if (key == 27) { // quit
        exit(0);
    }
    glutPostRedisplay();
}

/**
 * @brief initialize - Init method for project
 */
void initialize(void) {
    float grey[4] = {float(0.4), float(0.4), float(0.4), 1};
    float white[4] = {0, 0, 0, 1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);

    q = gluNewQuadric();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

    gluQuadricDrawStyle(q, GLU_FILL);
	glClearColor (0.0, 0.0, 0.0, 0.0);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 5000.0);     //Perspective projection

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    loadTexture();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(0., 0., 1., 1.);

    for (loop=0; loop<TOTAL_SNOW; loop++) {
        init_snowfall(loop);
    }
    loop = 0;
}

/**
 * @brief display - Display method for project
 */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0, 20, cam_height, .0, 1.0, 0.0, 0.0, 1.0, 0.0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);

    glRotatef(angle_left_right, 0.0, 1.0, 0.0);
    glRotatef(angle_up_down, 1.0, 0.0, 0.0);

    skybox();
    castle();
    draw_snow();

    int checker = 1;
    int r_timer = 0;
    robot_walking(checker);
    
    glPushMatrix();
        glTranslatef(10, 0, 5);
        glRotatef(90,0,1,0);
        cannon();
    glPopMatrix();
    
    if (bullets_move == robots_move) {
        checker = 0;
        robot_walking(checker);
    }
    if (checker == 0) {
        r_timer++;
        if (r_timer == 5) {
            checker = 1;
            robot_walking(checker);
            r_timer = 0;
        }
    }
    if (loaded == 1) {
        glPushMatrix();
            glTranslatef(10.0, 10.0, bullets_move);
            bullet();
        glPopMatrix();
    }
    glPushMatrix();
        glTranslatef(0, 5, -20);
        if (launch == 1) {
            glTranslatef(0, spaceship_move_y, 0);
            if (spaceship_move_y < 100 && toggle == 1 && cam_height <= 500) {
                cam_height++;
            }
        }
        glTranslatef(0, spaceship_move_y, 0);
        spaceship();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(30,-10,-60);
        hydrant_base();
    glPopMatrix();
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);
    glutSwapBuffers();
}

/**
 * @brief main - Main function to launch project
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
    glutInitWindowSize (1080, 960);
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("Kai's Alien Invasion! - Winter Edition");
    initialize ();
    glutTimerFunc(20, robot_timer, 0);
    glutSpecialFunc(glut_key_pressed);
    glutKeyboardFunc(key_pressed);
    glutDisplayFunc(display); 
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
