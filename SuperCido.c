#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <GL/glut.h>

#define PI 3.1415926

GLint windowHeight=600, windowWidth=500;
int pause=0, sideCross=0,score=0,gameOver,left_button=0;;
char scoreTable[100];

typedef struct {
    float x,y;
} Position;

typedef struct {
    int type; //1 truck, 0 car.
    float speed;
    Position pos;
    int exist; //0 not exists, 1 exist.
} Vehicle;

typedef struct {
    Position pos;
    int direction; //1 for up, -1 for down
} Agent;

typedef struct { 
    int exists;
    Position pos;
} Coin;

float lane[20];
Vehicle vehicle[20];
Agent agent;
Coin coins[20];

void myInit(){
    glClearColor(1.0,1.0,1.0,1.0);
    glShadeModel(GL_SMOOTH);
    srand(time(NULL));

    float j=-1.0;
    for(int i=0;i<5;i++) {
        j+=0.09;
        for(int z=0;z<4;z++) {
            j=j+0.073;
            lane[i*4+z] = j-(0.073/2); 
        }
    }

    float x=1.0;
    for(int i=0;i<20;i++) {
        if(i%4==0)
            x=x*-1.0;
        vehicle[i].type=rand()%2;
        vehicle[i].speed=((rand() % 40) / 200.0+0.5)*-x;
        vehicle[i].pos.x=x;
        vehicle[i].pos.y=lane[i];
        vehicle[i].exist=0;
    }

    for(int i=0;i<20;i++) {
        coins[i].exists=0;
        coins[i].pos.y=lane[i];
    }

    agent.pos.x=0.0;
    agent.pos.y=-0.955;
    agent.direction=1;

}

void drawAgent() {
    glColor3f(0.941,0.384,0.573);
    
        glBegin(GL_TRIANGLES);
        glVertex2f(agent.pos.x+0.03,agent.pos.y-(0.03*agent.direction));
        glVertex2f(agent.pos.x-0.03,agent.pos.y-(0.03*agent.direction));
        glVertex2f(agent.pos.x,agent.pos.y+(0.03*agent.direction));
        glEnd();
        
}

void drawCoin() {
    glColor3f(1.0,1.0,0.0);
    for(int i=0;i<20;i++) {
        if(coins[i].exists!=0) {
            glBegin(GL_POLYGON);
            
            for(int j=0;j<60;j++) {
                float angle=(float) j*PI/30;
                glVertex2f(coins[i].pos.x+(0.03*cos(angle)),lane[i]+(0.03*sin(angle)));
                
            }
                coins[i].exists+=1;
            glEnd();

            if(agent.pos.y+0.03>=lane[i]&&agent.pos.y-0.03<=lane[i]) {
                if(coins[i].pos.x+0.03>=agent.pos.x-0.03&&coins[i].pos.x-0.03<=agent.pos.x+0.03) {
                    coins[i].exists=0;
                    score+=5;
                }
            }
            
            if(coins[i].exists>10000) {
                coins[i].exists=0;
            }
        }
        else {
            int random=rand()%3000;
            if(random==3){
                float randomX=((rand()%19)/10.0)-0.9;
                coins[i].exists=1;
                coins[i].pos.x=randomX;
            }
        }
    }
}

void mySpecialKeyboard(int c, int x, int y)
{
    if(gameOver!=1&&left_button!=1) {
   
        switch (c)
        {
                case GLUT_KEY_UP  :
                    if(agent.direction!=1) 
                        gameOver=1;
                    else{
                        if(sideCross%5==4||sideCross%5==0){
                            agent.pos.y+=0.0815;
                            
                        }
                        else
                        {
                            
                            agent.pos.y+=0.073;
                        }
                        if(agent.pos.y>0.955000) 
                            agent.direction*=-1;

                        sideCross++;
                        score++;
                    }
                break;
            case GLUT_KEY_DOWN:
                if(agent.direction!=-1) 
                        gameOver=1;
                    else{
                        if(sideCross%5==1||sideCross%5==0){
                            agent.pos.y-=0.0815;
                            sideCross--;
                        }
                        else
                        {
                            sideCross--;
                            agent.pos.y-=0.073;
                        }
                        if(agent.pos.y<-0.873500) 
                            agent.direction*=-1;
                        score++;
                    }        
                break;
            case GLUT_KEY_LEFT :
                if(agent.pos.x-0.06>-1)
                    agent.pos.x-=0.03;
                break;
            case GLUT_KEY_RIGHT :
                if(agent.pos.x+0.06<1)
                    agent.pos.x+=0.03;
                break;
        }

        glutPostRedisplay();		// request redisplay
    }
}

void myKeyboard(unsigned char c, int x, int y) {
   if(c=='q')
    exit(0);
    if(gameOver!=1&&left_button!=1) {
        switch(c) {
            case 'w'  :
                    if(agent.direction!=1) 
                        gameOver=1;
                    else{
                        if(sideCross%5==4||sideCross%5==0){
                            agent.pos.y+=0.0815;
                            
                        }
                        else
                        {
                            
                            agent.pos.y+=0.073;
                        }
                        if(agent.pos.y>0.955000) 
                            agent.direction*=-1;

                        sideCross++;
                        score++;
                    }
                break;
            case 's':
                if(agent.direction!=-1) 
                        gameOver=1;
                    else{
                        if(sideCross%5==1||sideCross%5==0){
                            agent.pos.y-=0.0815;
                            sideCross--;
                        }
                        else
                        {
                            sideCross--;
                            agent.pos.y-=0.073;
                        }
                        if(agent.pos.y<-0.873500) 
                            agent.direction*=-1;
                        score++;
                    }        
                break;
            case 'a' :
                if(agent.pos.x-0.06>-1)
                    agent.pos.x-=0.03;
                break;
            case 'd' :
                if(agent.pos.x+0.06<1)
                    agent.pos.x+=0.03;
                break;
        }
        glutPostRedisplay();
    }
}

void drawVehicle() {
    for(int i=0;i<20;i++) {
        if(vehicle[i].exist==1) {
            if(vehicle[i].type==0) {
                glColor3f(0.29,0.078,0.549);
                glBegin(GL_POLYGON);
                glVertex2f(vehicle[i].pos.x-0.03,vehicle[i].pos.y-0.03);
                glVertex2f(vehicle[i].pos.x-0.03,vehicle[i].pos.y+0.03);
                glVertex2f(vehicle[i].pos.x+0.03,vehicle[i].pos.y+0.03);
                glVertex2f(vehicle[i].pos.x+0.03,vehicle[i].pos.y-0.03);
                glEnd();
                if(vehicle[i].pos.x+0.03<-1.0 || vehicle[i].pos.x-0.03>1.0)
                    vehicle[i].exist=0;

                if(agent.pos.y+0.03>=lane[i]&&agent.pos.y-0.03<=lane[i]) {
                    if(vehicle[i].pos.x+0.03>=agent.pos.x-0.03&&vehicle[i].pos.x-0.03<=agent.pos.x+0.03) {
                        gameOver=1;     
                    }
                }
            }

            else{
                glColor3f(0.376,0.49,0.545);
                glBegin(GL_POLYGON);
                glVertex2f(vehicle[i].pos.x-0.05,vehicle[i].pos.y-0.03);
                glVertex2f(vehicle[i].pos.x-0.05,vehicle[i].pos.y+0.03);
                glVertex2f(vehicle[i].pos.x+0.05,vehicle[i].pos.y+0.03);
                glVertex2f(vehicle[i].pos.x+0.05,vehicle[i].pos.y-0.03);
                glEnd();
                if(vehicle[i].pos.x+0.05<-1.0 || vehicle[i].pos.x-0.05>1.0)
                    vehicle[i].exist=0;
                
                if(agent.pos.y+0.03>=lane[i]&&agent.pos.y-0.03<=lane[i]) {
                    if(vehicle[i].pos.x+0.05>=agent.pos.x-0.03&&vehicle[i].pos.x-0.05<=agent.pos.x+0.03) {
                        gameOver=1;     
                    }
                }
            }
            vehicle[i].pos.x+=vehicle[i].speed/100.0;
        }
        else {
            int random=rand()%500;
            
            if(random==3) {
                vehicle[i].type=rand()%2;
                vehicle[i].speed=((rand() % 40) / 200.0+0.5)*-(vehicle[i].pos.x);
                vehicle[i].exist=1;
            }
        }
    }
}



void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);

    float j=-1.0;
    for(int i=0;i<6;i++) {

        glColor3f(0,0,0);
        glBegin(GL_POLYGON);

            glVertex2f(-1.0,j);
            glVertex2f(-1.0,j+0.09);
            glVertex2f(1.0,j+0.09);
            glVertex2f(1.0,j);
            
            glEnd();

        j+=0.09;
        double x=-1.0;
        for(int z=0;z<3;z++) {
            x=-1.0;
            j=j+0.073;
            for(int k=0;k<10;k++) {
                    
                glColor3f(0,0,0);
                glBegin(GL_LINE_STRIP);

                glVertex2f(x,j);
                glVertex2f(x+0.11,j);
                glEnd();

                x=x+0.21; 
            }     
        }
        j=j+0.073;
    }
    char high[50];
    sprintf(scoreTable,"Score: %d",score);
    
    glColor3f(1.0,1.0,0.0);
    glRasterPos2d(0.65,0.96);
    for(int a=0;a<(int)strlen(scoreTable);a++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13,scoreTable[a]);
    }

  
    drawAgent(); 
    drawVehicle();
    drawCoin();
    
    if(gameOver==1) {
        char over[20];
        strcpy(over,"GAME OVER!");
        glColor3f(1.0,0.0,0.0);
        glRasterPos2d(-0.99,0.95);
        for(int a=0;a<(int)strlen(over);a++) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,over[a]);
        }
    }

    if(pause==1||left_button==1) {
        char paused[20];
        strcpy(paused,"Pause");
        glColor3f(1.0,1.0,1.0);
        glRasterPos2d(0.0,0.95);
        for(int a=0;a<(int)strlen(paused);a++) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,paused[a]);
        }

        printf("Agent x: %f    y: %f\n",agent.pos.x,agent.pos.y);

        for(int i=0;i<20;i++) {
            if(vehicle[i].exist==1)
                printf("Vehicles x: %f  y: %f\n",vehicle[i].pos.x,vehicle[i].pos.y);
        }

        for(int i=0;i<20;i++) {
            if(coins[i].exists!=0)
                printf("Coins x: %f  y: %f\n",coins[i].pos.x,vehicle[i].pos.y);
        }
    }

    glutSwapBuffers();

}

void myMouse(int b, int s, int x, int y) {
    if(gameOver!=1) {
        switch  ( b ) {    // b indicates the button
            case GLUT_LEFT_BUTTON:
                if (s == GLUT_DOWN)  {    // button pressed
                    if(left_button==0) {
                        left_button=1;
                        pause=1;
                    }
                    else
                    {
                        left_button=0;
                        pause=0;
                    }
                }
                break;
            case GLUT_RIGHT_BUTTON:
                if (s == GLUT_DOWN)
                    pause=1;
                break;
            default:
            pause=0;
            break;
        }
        
        glutPostRedisplay();
    }
}

void myTimeOut(int id) {
	// advance the state of animation incrementally
if(gameOver!=1&&pause!=1)
	glutPostRedisplay();			  // request redisplay
    
    
	    glutTimerFunc(16, myTimeOut, 0);  // request next timer event
    
}

int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(500,600);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Super Cido");

    glutDisplayFunc(myDisplay);
    glutTimerFunc(16, myTimeOut, 0);
    glutSpecialFunc(mySpecialKeyboard);
    
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);
    myInit();
    
    glutMainLoop();

    return 0;
}