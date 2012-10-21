#include "glcommon.h"
#include "utils.h"

#include <vector>
#include "vnclib.h"
#include "filter.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;


#define INIT_WIDTH  640
#define INIT_HEIGHT 480
#define WIDTH  glutGet(GLUT_WINDOW_WIDTH)
#define HEIGHT glutGet(GLUT_WINDOW_HEIGHT)
int pre_width;
int pre_height;

int mycount = 0;
int Max=0;
int vsync = 1;
string str="";
int fullscreen=0;
int g_LeaveGameMode=0;

float posx=0.0f;
float posy=0.0f;
float picx=0;
float picy=0;
float scale=1.0f;

float org_posx=0.0f;
float org_posy=0.0f;
float org_scale;

MouseDat mousedat;
MouseDat passive_mousedat;


VNC_Client vnc;

Texture tex;



void timer(int value) {
  Max=mycount;
  mycount=0;
  glutTimerFunc(1000 , timer , 0);
  // glutPostRedisplay();
  //vnc.set_display(1);
}

char buf[255],buf2[255];

void
display_fps(){
  mycount++;
  sprintf(buf,"%03d",mycount);
  sprintf(buf2,"%03d",Max);
  str="space on/off FPS[";
  str+=buf2;
  str+="] FRAME[";
  str+=buf;
  str+="]";
  glutSetWindowTitle(str.c_str());
}
/*
|-------------------------|
|                         |
|      (px,py)--|         |
|       |  (picx,pixy)|   |
|       |-------|         |
|                         |
|                         |
|-------------------------|
  picx=(x-px)/scale;
  picy=(y-py)/scale;

 */


void
display(void){
  mycount++;
  //  posx+=4.0f;
  //  printf("display\n");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, WIDTH, HEIGHT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);

  Square(mousedat.x-1,mousedat.y-10,
	 mousedat.x+1,mousedat.y+10);

  glPushMatrix();
  {
    glTranslatef(posx,posy,0.0f);
    glScalef(scale,scale,scale);
    glColor3ub(255,255,255);
    vnc.img_mutex.lock();
    tex.set(vnc.img2);
    vnc.img_mutex.unlock();
    tex.display();
    //vnc.set_display(1);
  }
  glPopMatrix();
  
  //  display_fps();

  glutSwapBuffers();

  if( g_LeaveGameMode == 1 ){
    glutLeaveMainLoop();
    g_LeaveGameMode = 0;
  }

}

void
idle(){
  glutPostRedisplay();
}

/*

Key name        Keysym value	Key name        Keysym value  
BackSpace	0xff08		F1		0xffbe	      
Tab		0xff09		F2		0xffbf	      
Return or Enter	0xff0d		F3		0xffc0	      
Escape		0xff1b		F4		0xffc1	      
Insert		0xff63		...		...	      
Delete		0xffff		F12		0xffc9	      
Home		0xff50		Shift (left)	0xffe1	      
End		0xff57		Shift (right)	0xffe2	      
Page Up		0xff55		Control (left)	0xffe3	      
Page Down	0xff56		Control (right)	0xffe4	      
Left		0xff51		Meta (left)	0xffe7	      
Up		0xff52		Meta (right)	0xffe8	      
Right		0xff53		Alt (left)	0xffe9	      
Down		0xff54          Alt (right)	0xffea        
*/

int ctrl_key =0;
int alt_key  =0;
int shift_key=0;
int super_toggle=0;

void 
vnckey(unsigned char key,int updown){
  if(ctrl_key){
    if(updown){
      vnc.set_key(0xffe3,1);
      vnc.set_key(0x60+key,1);
    }else{
      vnc.set_key(0x60+key,0);
      vnc.set_key(0xffe3,0);
    }
  }else{
    switch(key){
    case 0x8: //backspace
    case 0x9: //tab
    case 0xd: //Enter
    case 0x1b://esc
      vnc.set_key(key|0xff00,updown);
      break;
    case 0x7f://del
      vnc.set_key(0xff7f,updown);
      break;
    default:
      vnc.set_key(key,updown);
    }
  }
}

void 
vncskey(unsigned char key,int updown){
  //  printf("skey %x\n",(int)key);
  switch(key){
  case 0x01://f1
  case 0x02:
  case 0x03:
  case 0x04:
  case 0x05:
  case 0x06:
  case 0x07:
  case 0x08:
  case 0x09:
  case 0x0a:
  case 0x0b:
  case 0x0c://f12
    vnc.set_key(0xffbd+key,updown);break;
  case 0x64://left
    vnc.set_key(0xff51,updown);break;
  case 0x65://up
    vnc.set_key(0xff52,updown);break;
  case 0x66://right
    vnc.set_key(0xff53,updown);break;
  case 0x67://down
    vnc.set_key(0xff54,updown);break;
  case 0x68://pgup
    vnc.set_key(0xff55,updown);break;
  case 0x69://pgdown
    vnc.set_key(0xff56,updown);break;
  case 0x6a://home
    vnc.set_key(0xff50,updown);break;
  case 0x6c://insert
    vnc.set_key(0xff63,updown);break;
  case 0x70://shift
    shift_key=updown;
    vnc.set_key(0xffe1,updown);break;
  case 0x72://ctrl
    ctrl_key=updown;
    vnc.set_key(0xffe3,updown);break;
  case 0x74://alt
    alt_key=updown;
    vnc.set_key(0xffe9,updown);break;
  case 0x76://super
    //    vnc.set_key(0xffe9,updown);break;
    break;
  default:
    vnc.set_key(0xffbd+key,updown);
  }
}


void
keydown(unsigned char key, int x, int y){

  if(super_toggle){
    vnckey(key,1);
  }else{
    switch(key){
    case '1':
      posx=0.0f;
      posy=0.0f;
      scale=1.0f;
      break;
    case 's':
      printf("write:snap.png\n");
      vnc.img.write("snap.png");
      break;
    case 'f':
      fullscreen = !fullscreen;
      if (fullscreen){
	glutFullScreen();                /* Go to full screen */
      }else{
	glutReshapeWindow(INIT_WIDTH,INIT_HEIGHT);
	glutPositionWindow(0,0);
      }
      break;
    case 'q':
    case 27://ESC key
      g_LeaveGameMode = 1;
      break;
    case ' ':
      vsync = vsync==0 ? 1:0;
      set_vsync(vsync);
      break;
    }
  }
}

void
keyup(unsigned char key, int x, int y){
  if(super_toggle){
    vnckey(key,0);
  }
}

void
skeydown(int key, int x, int y){
  if(key==0x76)
     super_toggle = super_toggle ? 0:1;
  // //  printf("super_togle %d\n",super_toggle);
  if(super_toggle){
    vncskey(key,1);
  }
}

void
skeyup(int key, int x, int y){
  if(super_toggle){
    vncskey(key,0);
  }
}


int
mouse_vnc2glut(int button,int state){
  return
    ( (button == GLUT_LEFT_BUTTON   &&  state==GLUT_DOWN)  ? 1	: 0)|
    ( (button == GLUT_MIDDLE_BUTTON &&  state==GLUT_DOWN)  ? 2	: 0)|
    ( (button == GLUT_RIGHT_BUTTON  &&  state==GLUT_DOWN)  ? 4	: 0)|
    ( (button == GLUT_UP_BUTTON     &&  state==GLUT_DOWN)  ? 8	: 0)|
    ( (button == GLUT_DOWN_BUTTON   &&  state==GLUT_DOWN)  ? 16	: 0)
    ;

}

void
mouse(int button , int state , int x , int y) {
  int shift=glutGetModifiers() & GLUT_ACTIVE_SHIFT;
  int ctrl =glutGetModifiers() & GLUT_ACTIVE_CTRL;
  int alt  =glutGetModifiers() & GLUT_ACTIVE_ALT;
  mousedat.button=button&0x7;
  mousedat.state=state;
  mousedat.x=x;
  mousedat.y=y;
  mousedat.shift=shift;
  mousedat.ctrl=ctrl;
  mousedat.alt=alt;

  org_posx=x;
  org_posy=y;
  org_scale=scale;
  //  printf("%d\n",button);
  picx=(x-posx)/scale;
  picy=(y-posy)/scale;

  if(super_toggle){
    vnc.set_point(picx,picy,mouse_vnc2glut(mousedat.button,mousedat.state));
  }else{

    if(button==3){
      if(scale <= 100)
	scale*=1.2;
    }
    if(button==4){
      if(scale >= 0.1)
	scale*=0.8;
    }
    if(button==GLUT_DOWN&&button == GLUT_LEFT_BUTTON ){

    }
    posx=x-picx*scale;
    posy=y-picy*scale;
  }
}


void
motion(int x , int y) {
  mousedat.x=x;
  mousedat.y=y;
  if(super_toggle){
    picx=(mousedat.x-posx)/scale;
    picy=(mousedat.y-posy)/scale;
    vnc.set_point(picx,picy,mouse_vnc2glut(mousedat.button,mousedat.state));
  }else{
    if(mousedat.ctrl){
      scale=org_scale*(1.0+(org_posy-y)*0.005);
      if(scale < 0.1)
	scale=0.1;
      else if(scale>100)
	scale=100;
    }
    posx=x-picx*scale;
    posy=y-picy*scale;
  }
}
void
passive_motion(int x , int y) {
  mousedat.x=x;
  mousedat.y=y;
  picx=(mousedat.x-posx)/scale;
  picy=(mousedat.y-posy)/scale;
  if(super_toggle){
    vnc.set_point(picx,picy,mouse_vnc2glut(mousedat.button,mousedat.state));
  }
}


void
dropfile(const char* file){
  printf("dropfile:'%s'\n",file);
}

void
Init(){
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void
reshape(int w,int h){
}


int
main(int argc, char *argv[]){
  // Camera cam;
  // cam.open(1280,720);
  // cam.get().write("a.png");
  // {
  //   int num=get_num_cams();
  //   printf("number of cameras:%d\n",num);
  //   for(int i=0;i<num;i++)
  //     getcamera(640,480,i).write(strf("a%d.png",i));
  // }
  // return 0;

  char *temp[2]={"vnclient",NULL};
  int num=1;
  glutInit(&num,temp);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
  glutInitWindowSize(INIT_WIDTH, INIT_HEIGHT);
  glutCreateWindow("vncclient");
  glutKeyboardFunc(keydown);
  glutKeyboardUpFunc(keyup);
  glutSpecialFunc(skeydown);
  glutSpecialUpFunc(skeyup);

  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(passive_motion);

  glutDisplayFunc(display);
  Init();
  set_vsync(1);
  //glutTimerFunc(1000 , timer , 0);
  glutIdleFunc(idle);
  glutDropFileFunc(dropfile);
  //  glutReshapeFunc(reshape);

  // for(int i=1;i<argc;i++){
  //   //    if(file_exist(argv[i])){
  //     texidx=0;
  //     tex.push_back(Texture(argv[i]));
  //     //    }
  // }

  glutSetCursor(GLUT_CURSOR_NONE);
  vnc.init(argv[1],atoi(argv[2]),argv[3]);
  // texidx=0;
  // BMPb b=vnc.get_display();
  // b.write("snap.png");
  // //  tex=Texture(vnc.get_display());
  // tex.set(b);
  //  vnc.set_display(1);
  // vnc.get_display();
  // tex.set(vnc.img);
  
  glutMainLoop();
  vnc.close();
  return 0;
}
