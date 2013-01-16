#include "glcommon.h"
#include "utils.h"

#include <vector>
#include "vnclib.h"
#include "filter.h"


using namespace std;


#define INIT_WIDTH  640
#define INIT_HEIGHT 480
int WIDTH=INIT_WIDTH; 
int HEIGHT=INIT_HEIGHT;
int pre_width;
int pre_height;

int mycount = 0;
int Max=0;
int vsync = 1;
string str="";
int fullscreen=0;
int g_LeaveGameMode=0;

// float picw=0.0f;
// float pich=0.0f;
float posx=0.0f;
float posy=0.0f;
float picx=0;
float picy=0;
float scale=1.0f;
float angle=0.0f;
int   small=0;
int   mode=0;

float org_posx=0.0f;
float org_posy=0.0f;
float org_scale;

MouseDat mousedat;
MouseDat passive_mousedat;


VNC_Client vnc;

Texture tex;
Texture info_tex;



void timer(int value) {
  Max=mycount;
  mycount=0;
  glutTimerFunc(1000 , timer , 0);
  // glutPostRedisplay();
  //vnc.set_display(1);
}

char buf[255],buf2[255];

void gen_picxy(){
  int x,y;
  int mx,my;
  double ang=-angle/180.0*M_PI;


  x=(mousedat.x-posx)/scale;
  y=(mousedat.y-posy)/scale;

  picx=cos(ang)*x-sin(ang)*y;
  picy=sin(ang)*x+cos(ang)*y;
  //  printf("%f %f\n",(double)picx,(double)picy);
  
}

void gen_posxy(){
  int x;
  int y;
  double ang=angle/180.0*M_PI;
  x=cos(ang)*picx-sin(ang)*picy;
  y=sin(ang)*picx+cos(ang)*picy;
  
  posx=mousedat.x-x*scale;
  posy=mousedat.y-y*scale;
}

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

  // Square(mousedat.x-1,mousedat.y-10,
  // 	 mousedat.x+1,mousedat.y+10);

  
  glPushMatrix();
  {

    // {
    //   Lock(vnc.img_mutex);
    //   picw=tex.sizeX;
    //   pich=tex.sizeY;
    // }
    
    //    glTranslatef(picw/2,pich/2,0.0f);
    //    glTranslatef(-picw/2,-pich/2,0.0f);

    glTranslatef(posx,posy,0.0f);
    glScalef(scale,scale,scale);
    
    glColor3ub(255,255,255);
    {
      Lock lock(vnc.img_mutex);
      tex.set(vnc.img2);
      info_tex.set(vnc.info_img2);
    }
    glRotatef(angle,0.0f,0.0f,1.0f);
    tex.display();
    info_tex.display();
  }
  glPopMatrix();
  

  if(small){
    glPushMatrix();
    {
      glScalef(0.2,0.2,0.2);
      glColor3ub(255,255,255);
      vnc.img_mutex.lock();
      tex.set(vnc.img2);
      vnc.img_mutex.unlock();
      tex.display();
    }
    glPopMatrix();
  }
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
      vnc.set_key(0xffff,updown);
      break;
    default:
      vnc.set_key(key,updown);
    }
  }
}

void 
vncskey(unsigned char key,int updown){
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
  case 0x78://zenkaku_hanaku
    vnc.set_key(0xff28,updown);break;
  case 0x76://super
    //    vnc.set_key(0xffe9,updown);break;
    break;
  default:
    vnc.set_key(0xffbd+key,updown);
  }
}

void
window_fit(double tex_w,double tex_h){
  if(angle==90.0f){
    posx=WIDTH;
    posy=0.0f;
    scale=WIDTH/tex_h;
    glutReshapeWindow(WIDTH,tex_w*scale);
  }else{
    posx=0.0f;
    posy=0.0f;
    scale=WIDTH/tex_w;
    glutReshapeWindow(WIDTH,tex_h*scale);
  }
}

void
keydown(unsigned char key, int x, int y){

  //  printf("key down:%d\n",(int)key);
  if(super_toggle){
    vnckey(key,1);
  }else{
    switch(key){
    case '1':
      posx=0.0f;
      posy=0.0f;
      scale=1.0f;
      break;
    case '2':
      {
	vnc.img_mutex.lock();
	double texx=tex.sizeX;
	double texy=tex.sizeY;
	vnc.img_mutex.unlock();
	window_fit(texx,texy);
	
      }
      break;
    case 'r':
      angle+=30.0f;
      break;
    case 'R':
      angle-=30.0f;
      break;
    case 's':
      small=small?0:1;
      break;
    case 'm':
      mode=(mode+1)%3;
      vnc.set_mode(mode);
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
  // printf("skey down:%d\n",(int)key);
  //  if(key==0x76)//windows key
  if(key==0x73)//right contrl key
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
  
  gen_picxy();

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
    gen_posxy();
  }
}


void
motion(int x , int y) {
  mousedat.x=x;
  mousedat.y=y;
  if(super_toggle){
    gen_picxy();
    vnc.set_point(picx,picy,mouse_vnc2glut(mousedat.button,mousedat.state));
  }else{
    if(mousedat.ctrl){
      scale=org_scale*(1.0+(org_posy-y)*0.005);
      if(scale < 0.1)
	scale=0.1;
      else if(scale>100)
	scale=100;
    }
    gen_posxy();
  }
}
void
passive_motion(int x , int y) {
  mousedat.x=x;
  mousedat.y=y;
  gen_picxy();
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
  WIDTH=glutGet(GLUT_WINDOW_WIDTH);
  HEIGHT=glutGet(GLUT_WINDOW_HEIGHT);
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  //  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
reshape(int w,int h){
  //  printf("reshape\n");
  WIDTH=glutGet(GLUT_WINDOW_WIDTH);
  HEIGHT=glutGet(GLUT_WINDOW_HEIGHT);
  vnc.set_key(0xffe9,0);
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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA );
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
  glutReshapeFunc(reshape);

  //  glutSetCursor(GLUT_CURSOR_NONE);
  vnc.init(argv[1],atoi(argv[2]),argv[3]);
  
  
  glutMainLoop();
  vnc.close();
  return 0;
}
