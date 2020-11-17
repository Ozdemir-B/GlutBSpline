#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include<stdint.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
	int x;
	int y;
}point;
typedef struct{
	float x;
	float y;
}sPoint; //s stands for scaled

//computational
const char* read_from_txt(char*);
struct point * getPoints(char*);
int start(point* );
int** plane(point*,int);
int getLen(char*);
void scale();
float recBasisFuncX(float,int , float);
float recBasisFuncY(float,int , float);
void sortPoints();

//global
int** pl; //global variable for plane matrix.
int uzaklik;
char* line;
point* sortedPoints;
point* points; // struct point array that notates all the points given as a global variable.
sPoint* sPoints; // scaled version above.
int lenght;   //lenght of the points array above.
float circleRadius;
float circleRadiusScaled;
sPoint circleCenter;
int scaleRate; 
int lenKnotVector;
float * knotVector;
float u;
sPoint * splinePoints;
int lenSplinePoints;
int degreePolynom;

int donguSay=0;

//drawing 
void display();
void render();
void horizontalLine();
void verticalLine();
void glPoint(sPoint);
void drawMec();
void drawSpline();
void Start();
void Circle();
void glPointCircle(sPoint);
void Curve();
void PolygonDraw();
void DrawLine(sPoint,sPoint);

void glPointSpline(sPoint);
void quadPoint(sPoint, char);


int main(int argc, char** argv) {
	
	lenght=getLen("nokta.txt");
	points = (point*)malloc(sizeof(point) * lenght);
	sPoints= (sPoint*)malloc(sizeof(sPoint) * lenght);
	
	
	points=getPoints("nokta.txt");
	scale();
	int b;
	
	
	
	//------------------------------------
	//this part reforms the sPoints array to draw the spline curve. Since i am drawing a non-uniform b-spline.
	splinePoints=(sPoint*)malloc(sizeof(sPoint)*(lenght+5));
	int j;
	for(j=0;j<lenght;j++){
		splinePoints[j+2]=sPoints[j];
	}
	splinePoints[0].x=sPoints[0].x;
	splinePoints[0].y=sPoints[0].y;
	splinePoints[1].x=sPoints[0].x;
	splinePoints[1].y=sPoints[0].y;
	splinePoints[lenght+1].x=sPoints[lenght-1].x;
	splinePoints[lenght+1].y=sPoints[lenght-1].y;
	splinePoints[lenght+2].x=sPoints[lenght-1].x;
	splinePoints[lenght+2].y=sPoints[lenght-1].y;
	splinePoints[lenght+3].y=sPoints[lenght-1].y;
	splinePoints[lenght+3].x=sPoints[lenght-1].x;
	//
	//---------------------------------------------
	
	lenKnotVector= lenght+2+1;
	knotVector=(float*)malloc(sizeof(float)*lenKnotVector);
	
	int k;
	for(k=0;k<lenKnotVector;k++){
		knotVector[k]=(float)(k+1);
	}

	
	printf("\n cemberin merkez noktasi:(%f,%f)\n cembrin yaricapi=%f",circleCenter.x*scaleRate,circleCenter.y*scaleRate,circleRadius);
	
	int a;
	
	
	
	glutInit(&argc, argv);                 // Initialize GLUT
	
    
	glutInitWindowSize(640, 640); 
    glutInitWindowPosition(420, 100);
	glutCreateWindow("Minimum Enclosing Circle"); 
    glutDisplayFunc(drawMec);
    
    glutInitWindowPosition(1120, 100);
    glutInitWindowSize(640, 640);   
    glutCreateWindow("B-Spline"); 
    glutDisplayFunc(drawSpline);

    glutMainLoop();           // Enter the event-processing loop
	
	

	
	return 0;
}

void drawMec(){ 
	//draws all (goes to glutDisplayFunc() in main)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear the color buffer (background)
 
   	// Draw a Red 1x1 Square centered at origin
   	verticalLine(); //void func that draws line
   	horizontalLine(); // kinda same above
   	
   	Circle();
   	int i;
   	for(i=0;i<lenght;i++){
   		glPoint(sPoints[i]);
	   }

   	glFlush();
	
}

void drawSpline(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   	verticalLine(); //void func that draws square as vertical line 
   	horizontalLine(); // kinda same above but horizontal
	PolygonDraw();
	Curve();
   	int i;
   	for(i=0;i<lenght;i++){
   		glPoint(sPoints[i]);
	}	
   	glFlush(); //renders the code above
}

void Start(){
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,0.0f);
	glVertex2f(-0.5f,-0.5f);
	glVertex2f(0.5f,-0.5f);
	glVertex2f(0.5f,0.5f);
	glVertex2f(-0.5f,0.5f);
	
	glEnd();
	glFlush();
	
}

void verticalLine(){
	//glut coordinates are between -1.0f and 1.0f.  (0.0f,0.0f) point is the center. **************
	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(0.0f, 0.0f, 0.0f); // black
      //those coordinates below notates the points of the square
      glVertex2f(-0.003f, -1.0f);    // x, y
      glVertex2f( 0.003f, -1.0f);
      glVertex2f( 0.003f,  1.0f);
      glVertex2f(-0.003f,  1.0f); 
   glEnd();
}

void horizontalLine(){
	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(0.0f, 0.0f, 0.0f); // black
      glVertex2f(-1.0f, -0.003f);    // x, y
      glVertex2f( 1.0f, -0.003f);
      glVertex2f( 1.0f,  0.003f);
      glVertex2f(-1.0f,  0.003f);   
	glEnd();
}

void Circle(){
	glPointCircle(circleCenter);
	//everything that will be drawn to the windows need to be scaled between -1 and 1 (float).
	sPoint temp;
	float i;
	for(i=0;i<360;i+=0.5){
		temp.x=cos(i)*circleRadiusScaled;
		temp.x+=circleCenter.x;
		
		temp.y=sin(i)*circleRadiusScaled;
		temp.y+=circleCenter.y;
		
		quadPoint(temp,'g');
	}
	
}

void Curve(){
	// it draws cubic b-spline. degree is not variable. so i will not implemente the basis function as a recursive function. instead i will code the open form of the basis function for the degree as 4(cubic b-spline)
	sPoint tempSplinePoint;
	tempSplinePoint.x=0;
	tempSplinePoint.y=0;
	int a,b,c;
	float u;
	//i will calculate each segment seprately.
	for(a=1;a<lenght+2;a++){
		for(u=0.0;u<1.0;u+=0.005){
			tempSplinePoint.x=recBasisFuncX(u,a,splinePoints[a].x); //because i will not draw an uniform b-spline
			tempSplinePoint.y=recBasisFuncY(u,a,splinePoints[a].y);
			quadPoint(tempSplinePoint,'b');
		
		}
	}
}

float recBasisFuncX(float u, int a, float coor){
	//open formula for qubic b-spline curve
	float sCoor;
	sCoor=((1-u)*(1-u)*(1-u)*splinePoints[a-1].x + (3*u*u*u - 6*u*u + 4)*splinePoints[a].x + ((-3)*u*u*u+3*u*u+3*u+1)*splinePoints[a+1].x + u*u*u*splinePoints[a+2].x)/6;
	return sCoor;
}
float recBasisFuncY(float u, int a, float coor){
	float sCoor;
	sCoor=((1-u)*(1-u)*(1-u)*splinePoints[a-1].y + (3*u*u*u - 6*u*u + 4)*splinePoints[a].y + ((-3)*u*u*u+3*u*u+3*u+1)*splinePoints[a+1].y + u*u*u*splinePoints[a+2].y)/6;
	return sCoor;
}

void PolygonDraw(){
	
	int a,b;
	for(a=0;a<lenght-1;a++){
		DrawLine(sPoints[a],sPoints[a+1]);
	}
}

void DrawLine(sPoint a, sPoint b) {

	glBegin(GL_LINES);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex2d(a.x, a.y);
	glVertex2d(b.x, b.y);
	glEnd();

	if (1) {
		glPointSize(3.0);
		glBegin(GL_POINTS);
		glVertex2d(a.x, a.y);
		glVertex2d(b.x, b.y);
		glEnd();
	}
}



void glPointSpline(sPoint p){
	glBegin(GL_QUADS);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex2f(p.x-0.005, p.y-0.005);    //x, y
    glVertex2f( p.x+0.005, p.y-0.005);
    glVertex2f( p.x+0.005,  p.y+0.005);
    glVertex2f(p.x-0.005,  p.y+0.005);
    glEnd();
}

void glPoint(sPoint p){
	
	//its basicly verry small square. but locates differantly according to point given as parameter
	glBegin(GL_QUADS);
	glColor3f(1.0f,0.0f,0.0f);//red
	glVertex2f(p.x-0.005, p.y-0.005);    //x, y
    glVertex2f( p.x+0.005, p.y-0.005);
    glVertex2f( p.x+0.005,  p.y+0.005);
    glVertex2f(p.x-0.005,  p.y+0.005);
    glEnd();
	
}

void glPointCircle(sPoint p){
	
	glBegin(GL_QUADS);
	glColor3f(0.0f,1.0f,0.0f); //green
	glVertex2f(p.x-0.005, p.y-0.005);    //x, y
    glVertex2f( p.x+0.005, p.y-0.005);
    glVertex2f( p.x+0.005,  p.y+0.005);
    glVertex2f(p.x-0.005,  p.y+0.005);
    glEnd();
	
}

void quadPoint(sPoint p, char c){
	glBegin(GL_QUADS);
	if(c=='r') glColor3f(1.0f,0.0f,0.0f);
	if(c == 'g') glColor3f(0.0f,1.0f,0.0f);
	if(c=='b') glColor3f(0.0f,0.0f,1.0f);
	glVertex2f(p.x-0.002, p.y-0.002);    //x, y
    glVertex2f( p.x+0.002, p.y-0.002);
    glVertex2f( p.x+0.002,  p.y+0.002);
    glVertex2f(p.x-0.002,  p.y+0.002);
    glEnd();
}


struct point * getPoints(char * location) {

	
	printf("\n\n------------------\n");
	FILE* f = fopen(location, "r");
	line = (char*)malloc(sizeof(char) * 50);
	fgets(line, 50, f);
	fclose(f);
	printf("\ntext:%s\n", line);

	int a=0;
	int lenght = 0;
	while (a<14) {
		if (line[a] == ',') lenght++;
		a++;
	}
	lenght++;
	
	point* points = (point*)malloc(sizeof(point) * lenght);
	int index = 0;
	char* tempx = (char*)malloc(sizeof(char) * 50);
	char* tempy = (char*)malloc(sizeof(char) * 50);
	int state=0;
	int tx = 0, ty = 0;
	int j = 0;
	
	while (line[j-1] != '\0') {
		if (line[j] == '_') { points[index].x = atoi(tempx); tempx[0] = '\0'; state = 1; tx = 0; j++; }
		if (line[j] == ',' || line[j]=='\0') { points[index].y = atoi(tempy); tempy[0] = '\0'; state = 0; ty = 0; j++;  index++; }
		if (state == 0) {
			tempx[tx] = line[j];
			tx++;
		}
		if (state == 1) {
			tempy[ty] = line[j];
			ty++;
		}
		
		j++;	
	}
	
	return points;
}

int getLen(char* location) {
	int len=0;
	FILE* f = fopen(location, "r");
	char* line = (char*)malloc(sizeof(char) * 50);
	fgets(line, 50, f);
	fclose(f);
	int i=0;
	while (line[i] != '\0') { if (line[i] == '_') { len++; } i++; }
	return len;
}

void scale(){
	
	int len=lenght;
	int i,j;
	float uzak1=0,uzaklik;
	int sUzak=0;
	uzak1 = 0; 
	int index1, index2; 
	for(i=0; i<len-1 ;i++){
		for(j=i+1; j<len ;j++){
			uzaklik=pow(pow(points[i].x-points[j].x,2) + pow(points[i].y-points[j].y,2),0.5 );
			if(uzaklik>uzak1){
				uzak1=uzaklik;
				index1=i;
				index2=j;
			}	
		}
	}

	//sssssssssssssssssss
	for(i=0;i<len;i++){
		if(abs(points[i].x)>abs(points[i].y)){
			if(abs(points[i].x)>sUzak){
				sUzak=points[i].x;
			}
		}
		else{
			if(abs(points[i].y)>sUzak){
				sUzak=points[i].y;
			}
		}
	}
	
	scaleRate=sUzak*2; // to scale the point coordinates between 1 and -1, wee need to scale the values by dividing all the points with dynamicly setted value.
	for(i=0;i<len;i++){
		sPoints[i].x=(float)points[i].x/(sUzak*2);
		//printf("\n%f=%d/%d",sPoints[i].x,points[i].x,sUzak*2);
		sPoints[i].y=(float)points[i].y/(sUzak*2);
	}
	
		
	printf("\nen uzak noktalar:(%d,%d)-(%d,%d), mesafe=%f",points[index1].x,points[index1].y,points[index2].x,points[index2].y,uzak1);
	
	circleRadius=uzak1/2;
	circleRadiusScaled=circleRadius/scaleRate;
	
	//if(index2>index1) :::
	float index1M,index2M;
	index1M = pow(points[index1].x,2)+pow(points[index1].y,2);
	index2M = pow(points[index2].x,2)+pow(points[index2].y,2);
	if(index2M>index1M){
		circleCenter.x=(sPoints[index2].x-sPoints[index1].x)/2;
		circleCenter.y=(sPoints[index2].y-sPoints[index1].y)/2;
		circleCenter.x+=sPoints[index1].x;
		circleCenter.y+=sPoints[index1].y;
	}
	if(index1M>index2M){
		circleCenter.x=(sPoints[index1].x-sPoints[index2].x)/2;
		circleCenter.y=(sPoints[index1].y-sPoints[index2].y)/2;
		circleCenter.x+=sPoints[index2].x;
		circleCenter.y+=sPoints[index2].y;
	}
	
	
	
}

void sortPoints(){
	sortedPoints=(point*)malloc(sizeof(point)*lenght);
	float * mList=(float*)malloc(sizeof(float)*lenght);
	int* indexList=(int*)malloc(sizeof(int)*lenght);
	point temp;temp.x=0;temp.y=0;
	
	int a,b,c;
	for(a=0;a<lenght;a++){
		mList[a]=pow(points[a].x,2)+pow(points[a].y,2);
	}
	for(a=0;a<lenght-1;a++){
		for(b=a+1;b<lenght;b++){
			break;
		}
	}
	
}

