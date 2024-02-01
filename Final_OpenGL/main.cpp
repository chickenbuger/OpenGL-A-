#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Vertex.h"
#include "Player.h"
#include "Enemy.h"
#include "Tile.h"
#include "Wall.h"

using namespace std;

Player	_player;
int		_playerid = 1069; // 출발지
Enemy	_enemy;
int		_enemyid = 767; // 도착지

vector<Wall> _wall;

Vertex scenePos[41][41];
TileNode _Node;

vector<Tile*> Path;

float deltaTime = 0.0;

float MoveTo[2];

bool Fin = false;
bool Update = false;
bool First = true;

char PorE = 'p';

bool Astar(int start, int end) // f=g+h
{
	//cout << "start : " << start << "\nend : " << end << endl;
	Tile* _start = _Node.Edge[start];
	Tile* _end = _Node.Edge[end];

	vector<Tile*> openSet;
	Tile* _current = _start;
	_current->InClosed = true;
	
	int count = 0;
	
	//cout << "Astar" << endl;
	do
	{
		for (Tile* neighbor : _current->To->Edge)
		{
			if (neighbor->Block)
			{
				continue;
			}

			if (!neighbor->InClosed) //안 닫혔다면
			{
				if (!neighbor->InOpen) //안 열려있다면 
				{
					neighbor->Parent = _current;
					float Hdis = sqrt(pow(neighbor->x - _end->x, 2) + pow(neighbor->y - _end->y, 2) + pow(neighbor->z - _end->z, 2));
					neighbor->h = Hdis; // 거리
					float Gdis = sqrt(pow(neighbor->x - _current->x, 2) + pow(neighbor->y - _current->y, 2) + pow(neighbor->z - _current->z, 2));
					neighbor->g = Gdis;
					neighbor->f = neighbor->h + neighbor->g;
					openSet.emplace_back(neighbor);
					neighbor->InOpen = true;
				}
				else
				{
					float newG = sqrt(pow(neighbor->x - _current->x, 2) + pow(neighbor->y - _current->y, 2) + pow(neighbor->z - _current->z, 2));
					if (newG < neighbor->g)
					{
						neighbor->Parent = _current;
						neighbor->g = newG;
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		if (openSet.empty())
		{
			break;
		}

		//가장 작은 가격의 노드 찾기
		auto iter = min_element(openSet.begin(), openSet.end(),
			[](Tile* a, Tile* b) {
				return a->f < b->f;
			});
		
		_current = *iter;
		openSet.erase(iter);
		_current->InOpen = false;
		_current->InClosed = true;
		
		Path.push_back(_current);
	} while (_current != _end);

	//cout << "Astar" << endl;

	return (_current == _end) ? true : false;
}

void PathLine()
{
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for (Tile* path : Path)
	{
		glVertex3f(path->x, path->y, path->z);
	}
	glEnd();

	glutPostRedisplay();
}

void UpdatePath()
{
	Fin = false;
	cout << "업데이트";
	if (!First)
	{
		for (int i = 0; i < 1600; i++)
		{
			_Node.Edge[i]->reset();
		}
		Path.clear();
		for (int i = 0; i < _wall.size(); i++)
		{
			_Node.Edge[_wall[i].id]->Block = true;
		}
	}
	Astar(_playerid, _enemyid);
	First = false;
	Fin = true;
}

void idsetting()
{
	//_playerid = 1500;
	_player.set(_Node.Edge[_playerid]->x, _Node.Edge[_playerid]->y, _Node.Edge[_playerid]->z);
	//_enemyid = 45;
	_enemy.set(_Node.Edge[_enemyid]->x, _Node.Edge[_enemyid]->y, _Node.Edge[_enemyid]->z);

	UpdatePath();
}

void Init()
{
	glEnable(GL_DEPTH_TEST);
	
	for (int i = 0; i < 41; i++)
	{
		for (int j = 0; j < 41; j++)
		{
			scenePos[i][j].set(j, i, 0.0);	
			//scenePos[i][j].print();
		}
	}
	
	int index = 0;
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			Tile* _tile = new Tile;
			_tile->x = (scenePos[i][j].x + scenePos[i][j + 1].x + scenePos[i + 1][j].x + scenePos[i + 1][j + 1].x) / 4.0f;
			_tile->y = (scenePos[i][j].y + scenePos[i][j + 1].y + scenePos[i + 1][j].y + scenePos[i + 1][j + 1].y) / 4.0f;			
			_tile->id = index++;
			_Node.Edge.push_back(_tile);
		}
	}
	//인접 노드 찾기
	/*Tile* test = new Tile;
	_Node.Edge[0]->To->Edge.push_back(test);
	_Node.Edge[0]->To->Edge[0]->x = 10;
	_Node.Edge[0]->To->Edge[0]->y = 20;
	_Node.Edge[0]->To->Edge[0]->print();
	_Node.Edge[0]->To->Edge.push_back(_Node.Edge[500]);
	_Node.Edge[0]->To->Edge[1]->print();

	cout << "확인\n";
	cout << _Node.Edge[0]->To->Edge.size() << endl;*/

	/*cout << "저장" << endl;
	_Node.Edge[0]->To->Edge.push_back(_Node.Edge[0]);
	cout << "프린트" << endl;
	_Node.Edge[0]->To->Edge[0]->print();
	cout << "프린트 끝" << endl;*/

	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			//_Node.Edge[i * 40 + j]->print();
		
			int id = i * 40 + j;
			bool active_bottom = false;
			bool active_top = false;
			bool active_left = false;
			bool active_right = false;

			if (id >= 40) //아래
			{
				_Node.Edge[id]->To->Edge.push_back(_Node.Edge[id - 40]);
				active_bottom = true;
			}
			if (id < 1560) //위
			{
				_Node.Edge[id]->To->Edge.push_back(_Node.Edge[id + 40]);
				active_top = true;
			}
			if ((id % 40) != 0) //왼쪽
			{
				_Node.Edge[id]->To->Edge.push_back(_Node.Edge[id - 1]);
				active_left = true;
			}
			if ((id % 40) != 39) //오른쪽
			{
				_Node.Edge[id]->To->Edge.push_back(_Node.Edge[id + 1]);
				active_right = true;
			}

			if (active_bottom)
			{
				if (active_left)
				{
					_Node.Edge[id]->To->Edge.push_back(_Node.Edge[id - 41]);
				}
				if (active_right)
				{
					_Node.Edge[id]->To->Edge.push_back(_Node.Edge[id - 39]);
				}
			}
			if (active_top)
			{
				if (active_left)
				{
					_Node.Edge[id]->To->Edge.push_back(_Node.Edge[id + 39]);
				}
				if (active_right)
				{
					_Node.Edge[id]->To->Edge.push_back(_Node.Edge[id + 41]);
				}
			}

			_Node.Edge[id]->g = 0.0f;
			//cout << "id : " << _Node.Edge[id]->id << " size : " << _Node.Edge[id]->To->Edge.size() << endl;
		}
	}

	idsetting();

	//UpdatePath();
	//Astar(_enemyid,_playerid);
}

void CameraSetting()
{
	glLoadIdentity();
}

void Lighting(void)
{
	glEnable(GL_LIGHTING);
	GLfloat lightPos[] = { 0.5,0.5,0.5,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
}

void draw_floor(void)
{
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			glColor3f(0.0f, 0.8f, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(scenePos[i][j].x, scenePos[i][j].y, scenePos[i][j].z);
				glVertex3f(scenePos[i][j + 1].x, scenePos[i][j + 1].y, scenePos[i][j + 1].z);
				glVertex3f(scenePos[i + 1][j].x, scenePos[i + 1][j].y, scenePos[i + 1][j].z);
			glEnd();

			/*glLineWidth(2.0);
			glColor3f(1.0f, 0.0f, 1.0f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(scenePos[i][j].x, scenePos[i][j].y, scenePos[i][j].z);
				glVertex3f(scenePos[i][j + 1].x, scenePos[i][j + 1].y, scenePos[i][j + 1].z);
				glVertex3f(scenePos[i + 1][j].x, scenePos[i + 1][j].y, scenePos[i + 1][j].z);
			glEnd();*/

			glColor3f(0.0f, 0.8f, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
				glVertex3f(scenePos[i][j + 1].x, scenePos[i][j + 1].y, scenePos[i][j + 1].z);
				glVertex3f(scenePos[i + 1][j + 1].x, scenePos[i + 1][j + 1].y, scenePos[i + 1][j + 1].z);
				glVertex3f(scenePos[i + 1][j].x, scenePos[i + 1][j].y, scenePos[i + 1][j].z);
			glEnd();

			glLineWidth(2.0);
			glColor3f(1.0f, 0.0f, 1.0f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(scenePos[i][j + 1].x, scenePos[i][j + 1].y, scenePos[i][j + 1].z);
				glVertex3f(scenePos[i + 1][j + 1].x, scenePos[i + 1][j + 1].y, scenePos[i + 1][j + 1].z);
				glVertex3f(scenePos[i + 1][j].x, scenePos[i + 1][j].y, scenePos[i + 1][j].z);
			glEnd();
		}
	}
}

void check_XYZ()
{
	//x축
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(40.0f, 0.0, 0.0f);
	glEnd();

	//y축
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0, 0.0f);
	glVertex3f(0.0f, 40.0, 0.0f);
	glEnd();

	//z축
	//glColor3f(0.0, 1.0, 0.0);
	//glLineWidth(3.0f);
	//glBegin(GL_LINES);
	//glVertex3f(0.0f, 0.0, 0.0f);
	//glVertex3f(0.0f, 0.0, 40.0f);
	//glEnd();
}

void draw(void)
{
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	Lighting();

	glDisable(GL_LIGHTING);
	
	//check_XYZ();

	_player.draw();
	_enemy.draw();

	for (int i = 0; i < _wall.size(); i++)
	{
		_wall[i].draw();
	}

	if (Fin == true)
	{
		PathLine(); //AI가 와야하는 최적의 경로
	}

	draw_floor();
}

void Display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	CameraSetting();
	draw();

	glutSwapBuffers();
}

void Reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	printf("w %d, h %d\n", w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 40, 0, 40, 0, 10);
	//gluPerspective(90,static_cast<GLdouble>(w/h),1.0f,50.0);
	glMatrixMode(GL_MODELVIEW);
}

void Mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			if (PorE == 'p')
			{
				printf("viewport x %d y %d \n", x, y);
				MoveTo[0] = x / 25;
				MoveTo[1] = y / 25;

				printf("전역 좌표계 x %f y %f \n", MoveTo[0], 39 - MoveTo[1]);

				float valueid = MoveTo[0] + 40 * (39 - MoveTo[1]);
				if (valueid != _enemyid)
				{
					_playerid = valueid;
					idsetting();
				}
				//UpdatePath();
				break;
			}
			if (PorE == 'e')
			{
				MoveTo[0] = x / 25;
				MoveTo[1] = y / 25;
				float valueid = MoveTo[0] + 40 * (39 - MoveTo[1]);
				if (valueid != _playerid)
				{
					_enemyid = valueid;
					idsetting();
				}
				break;
			}
		}
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			cout << "벽 설치\n";
			MoveTo[0] = x / 25;
			MoveTo[1] = y / 25;
			cout << "x : " << MoveTo[0] << " y : " << 39-MoveTo[1] << endl;
			float valueid = MoveTo[0] + 40 * (39 - MoveTo[1]);
			if (valueid != _playerid)
			{
				if (valueid != _enemyid)
				{
					int count = 0;
					for (int i = 0; i < _wall.size(); i++)
					{
						float id = _wall[i].x + _wall[i].y * 40;
						if (valueid == id)
						{
							count++;
							break;
						}
					}
					if (count == 0)
					{
						Wall wall;
						wall.set(
							_Node.Edge[valueid]->x,
							_Node.Edge[valueid]->y,
							0.0f);
						_Node.Edge[valueid]->Block = true;
						wall.id = valueid;

						_wall.push_back(wall);
					}
				}
			}
			idsetting();
			cout << "벽 개수 : " << _wall.size() << endl;
		}
	}
}

void Idle()
{
	//_player.x+=0.01;
	glutPostRedisplay();
}

void Keyboard(unsigned char KeyPressed, int x, int y)
{
	switch (KeyPressed)
	{
		case 'P':
			PorE = 'p';
			break;
		case 'p':
			PorE = 'p';
			break;
		case 'E':
			PorE = 'e';
			break;
		case 'e':
			PorE = 'e';
			break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(500, 300);
	glutCreateWindow("Final");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	//glutIdleFunc(Idle);
	Init();
	glutMainLoop();
	return 0;
}

/*
#pragma comment(linker,"/SUBSYSTEM:WINDOWS")
#pragma comment(linker,"/ENTRY:mainCRTStartup")

//제어점 가중치
static GLfloat w[4][4] = {
	{1.0,1.0,1.0,1.0},{1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0},{1.0,1.0,1.0,1.0}
};

static GLfloat ctlpointsCoordi[4][4][4]; //제어점 화면상 위치
static GLfloat ctlpoints[4][4][4];		//제어점 배열
GLfloat knots[8] = { 0.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0 }; //나트벡터
static GLUnurbsObj* theNurbs; // 너브스 객체
GLint width = 300, height = 300;

GLfloat point_mat_diffuse[] = { 1.0,0.5,1.0,1.0 };
GLfloat point_mat_specular[] = { 0.8,0.0,0.8,0.0 };
GLfloat point_mat_emission[] = { 0.0,0.0,0.0,1.0 };
GLfloat point_mat_shininess[] = { 30.0 };

GLfloat surface_mat_diffuse[] = { 0.8,0.8,0.2,1.0 };
GLfloat surface_mat_specular[] = { 0.8,0.0,0.8,0.0 };
GLfloat surface_mat_emission[] = { 0.0,0.0,0.0,1.0 };
GLfloat surface_mat_shininess[] = { 30.0 };

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -15.0f);
	glPushMatrix();
	glRotatef(45.0, 1.0, 0.0, 0.0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, point_mat_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, point_mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, point_mat_emission);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, point_mat_shininess);

			glPointSize(10.0f);
			glBegin(GL_POINTS);
			glVertex3f(ctlpointsCoordi[i][j][0], ctlpointsCoordi[i][j][1], ctlpointsCoordi[i][j][2]);
			glEnd();
			glPopMatrix();
		}
	}
	glPushMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			glBegin(GL_LINES);
			glVertex3f(ctlpointsCoordi[i][j][0], ctlpointsCoordi[i][j][1], ctlpointsCoordi[i][j][2]);
			glVertex3f(ctlpointsCoordi[i][j + 1][0], ctlpointsCoordi[i][j + 1][1], ctlpointsCoordi[i][j + 1][2]);
			glEnd();
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glBegin(GL_LINES);
			glVertex3f(ctlpointsCoordi[i][j][0], ctlpointsCoordi[i][j][1], ctlpointsCoordi[i][j][2]);
			glVertex3f(ctlpointsCoordi[i + 1][j][0], ctlpointsCoordi[i + 1][j][1], ctlpointsCoordi[i + 1][j][2]);
			glEnd();
		}
	}
	glPopMatrix();
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, surface_mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, surface_mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, surface_mat_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, surface_mat_shininess);
	gluBeginSurface(theNurbs);
	gluNurbsSurface(theNurbs, 8, knots, 8, knots, 4 * 4, 4, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_4);
	gluEndSurface(theNurbs);
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

void InitSurface()
{
	int s, t;
	for (s = 0; s < 4; s++)
	{
		for (t = 0; t < 4; t++)
		{
			ctlpointsCoordi[s][t][0] = 2.0 * ((GLfloat)s - 1.5f);
			ctlpointsCoordi[s][t][2] = 2.0 * ((GLfloat)t - 1.5f);
			if ((s == 1 || s == 2) && (t == 1 || t == 2))
				ctlpointsCoordi[s][t][1] = 3.0f;
			else
				ctlpointsCoordi[s][t][1] = -3.0f;
			for (int k = 0; k < 3; k++)
				ctlpoints[s][t][k] = w[s][t] * ctlpointsCoordi[s][t][k];
			ctlpoints[s][t][3] = w[s][t];
		}
	}
}

void Reshape(int NewWidth, int NewHeight)
{
	width = NewWidth; height = NewHeight;
	glViewport(0, 0, (GLsizei)NewWidth, (GLsizei)NewHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(33.0f, (GLfloat)NewWidth / (GLfloat)NewHeight, 3.0f, 20.0f);
}

void init(void)
{
	GLfloat light0_position[] = { 3.0,1.0,1.0,1.0 };
	GLfloat light1_position[] = { -3.0,1.0,1.0,1.0 };
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_AUTO_NORMAL);
	InitSurface();
	theNurbs = gluNewNurbsRenderer();
}

int main(int argc,char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("Final");
	init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return 0;
}
*/