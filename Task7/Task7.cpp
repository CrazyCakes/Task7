#include<gl/GLUT.H>
#include<iostream>
#include <iostream>
#include<fstream>
#include<sstream>
#include <vector>
#include <ctime>
#include <map>
#include<algorithm>
#include <set>
#include <ctime>
#include <iostream>
#include<list>
using namespace std;
GLfloat modelview_matrix[16];
GLfloat default_matrix[16];
GLfloat modelview_z_dis;
const int win_w = 700, win_h = 700;
GLfloat ex[3] = { 0.0, 0.0, 0.0 }; //�۲���λ��
GLfloat ey[3] = { 1.0, 0.0, 0.0 };
GLfloat ez[3] = { 4.0, 0.0, 0.0 };
GLfloat fx[3] = { 0.0, 0.0, 0.0 }; //�۲����λ��
GLfloat fy[3] = { 0.0, 0.0, 0.0 };
GLfloat fz[3] = { 0.0, 0.0, 0.0 };
GLfloat x_min = .0, x_max = 20.0, y_min = .0, y_max = 20.0, z_min = .0, z_max = 20.0;
GLint view = 0;
GLfloat angle = 0.0f;
void reshape_func(int w, int h);
void display_func();
void drawBunny();
void Initial(void);
// ����¼�
void onMouse(int button, int state, int x, int y);
void RotateRect();
void absolute_translate(GLfloat x, GLfloat y, GLfloat z);
void absolute_rotate(GLfloat dgree, GLfloat vecx, GLfloat vecy, GLfloat vecz);
void absolute_scale(GLfloat factor);
void mouse_click_func(int button, int state, int x, int y);
void mouse_move_func(int x, int y);
class Point
{
public:
	int flag;
	int index;
	float _x, _y, _z;
	vector<int>listfacet;
	void addFacet(int i)
	{
		listfacet.push_back(i);
	}
	void getIndex(int index)
	{
		this->index = index;
	}
	Point(float _x, float _y, float _z)
	{
		this->_x = _x;
		this->_y = _y;
		this->_z = _z;
	}
	Point(int index, float _x, float _y, float _z)
	{
		this->index = index;
		this->_x = _x;
		this->_y = _y;
		this->_z = _z;
	}
	Point()
	{}
	// ����
	bool operator<(const Point& b)const
	{
		//return false;
		return (_x < b._x) || (_x == b._x) && (_y < b._y) || (_x == b._x && _y == b._y && _z < b._z);
		//return true;
		// ��ȱȽ� 
	}
	bool isEque(Point point)
	{
		if (point._x == _x && point._y == _y && point._z == _z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
class Facet
{
	// ������Ƭ
public:int index;
	   // ���
	   double volume;
	   // ���
	   double s;
	   // ��Ƭ�е������
	   Point _vertex_1;
	   Point _vertex_2;
	   Point _vertex_3;
	   double sum;
	   vector<double>list;
	   Facet()
	   {}
	   bool operator<(const Facet& _b) const
	   {
		   if (_b.index < index)
		   {
			   return true;
		   }
		   else
		   {
			   return false;
		   }
	   }
	   void toString()
	   {
		   cout << "x= " << _vertex_1.index << " y= " << _vertex_2.index << " z= " << _vertex_3.index << endl;
	   }
};

void ReadPly();
map<int, Point>vecPoint;
vector<Facet>vecFacet;
set<Point>_listPoint;
Point getPoint;
void GetFieldcirculation(int index, int nField);
void onMouse(int button, int state, int x, int y);
void RotateRect();
int main(int argc, char*argv[])
{
	std::cout << "Hello World!\n" << endl;;
	clock_t startTime, endTime;
	startTime = clock();//��ʱ��ʼ
	ReadPly();
	endTime = clock();//��ʱ����
	GetFieldcirculation(0, 5);
	cout << " " << getPoint._x << " " << getPoint._y << " " << getPoint._z << endl;
	cout << "�ܹ����ٸ���: " << _listPoint.size() << endl;
	cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	// ��ͼ
	glutInit(&argc, argv);
	// ���ó�ʼ��ʾģʽ��ָ��RGB��ɫģʽ�Լ�ָ��˫���洰��
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH);
	glutInitWindowSize(win_w, win_h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("bunny");
	// �ı䴰�ڴ�Сʱ����ͼ�����
	glutReshapeFunc(reshape_func);
	// ��ʾͼ��
	glutDisplayFunc(display_func);
	// ��������¼�
	//glutMouseFunc(onMouse);
	glutMouseFunc(mouse_click_func);
	glutMotionFunc(mouse_move_func);
	Initial();
	glutMainLoop();
}
void RotateRect()
{
	angle += 1;
	if (angle > 360)
	{
		angle = 0.0f;
	}
	Sleep(30);
	display_func();
}
// ����¼�
void onMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(RotateRect);
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(NULL);
	}
}
/* ���������������������ƽ�ơ���ת�����ţ����Ǿ����Ӿ�(����)�����½���
 * �������� glTranslate,glRotate,glScale �����ھֲ������½���(������)
 * Ϊ�˴ﵽ���Ӿ������²�����Ч������Ҫ��������˵���ǰ����
 */
void absolute_translate(GLfloat x, GLfloat y, GLfloat z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, z);
	glMultMatrixf(modelview_matrix); // ʹ�任������˵���ǰ�����������ʺϾ�������Ŀ���
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPopMatrix();
}
void absolute_rotate(GLfloat dgree, GLfloat vecx, GLfloat vecy, GLfloat vecz)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(.0, .0, -modelview_z_dis);		// ƽ�ƻ�ȥ��ע��þ�ͺ�����Ҫ��������
	glRotatef(dgree, vecx, vecy, vecz);// ������ת��
	glTranslatef(.0, .0, modelview_z_dis);		// ��ƽ�Ƶ�ԭ��
	glMultMatrixf(modelview_matrix); // ʹ�任������˵���ǰ�����������ʺϾ�������Ŀ���
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPopMatrix();
}
void absolute_scale(GLfloat factor)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(.0, .0, -modelview_z_dis);		// ƽ�ƻ�ȥ��ע��þ�ͺ�����Ҫ��������
	glScalef(factor, factor, factor);
	glTranslatef(.0, .0, modelview_z_dis);		// ��ƽ�Ƶ�ԭ��
	glMultMatrixf(modelview_matrix); // ʹ�任������˵���ǰ�����������ʺϾ�������Ŀ���
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPopMatrix();
}
// ���ô���
void reshape_func(int w, int h)
{
	/*if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.0, 100.0);*/
	glViewport(0, h - win_w, win_w, win_h);

}
// ��ͼ����
void drawBunny()
{
	// ��ת����
	glRotatef(angle, 0, 1, 0);
	// ����һ����ɫ����
	GLfloat color[] = { .4f,.4f,.4f,1.0f };
	// ����
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);

	// ����������߽�����Ҫ��ʲô������������������ģʽ��
	// GL_PROJECTION ͶӰ, GL_MODELVIEW ģ����ͼ, GL_TEXTURE ����
	glMatrixMode(GL_MODELVIEW);
	// ����͸������
	for (auto it = vecFacet.begin();it != vecFacet.end();it++)
	{
			glPushMatrix();
			glBegin(GL_TRIANGLES);
		    glColor3f(0.0, 0.0, 1.0);
			glVertex3f(it->_vertex_1._x , it->_vertex_1._y , it->_vertex_1._z );
			glVertex3f(it->_vertex_3._x, it->_vertex_3._y, it->_vertex_3._z);
			glVertex3f(it->_vertex_2._x , it->_vertex_2._y , it->_vertex_2._z);
			glEnd(); //������
			glPopMatrix();
			
	}
	glPointSize(9.0);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(getPoint._x, getPoint._y, getPoint._z);
	glEnd();
	for (auto it = _listPoint.begin();it != _listPoint.end();it++)
	{
		glBegin(GL_POINTS);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(it->_x, it->_y, it->_z);
		glEnd();
	}
	cout <<"getPointSize" <<getPoint._x << " " << getPoint._y << " " << getPoint._z << endl;
}
// ��ʼ������
void Initial(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//��ɫ������ǰ3����RGB�������Alphaֵ����������͸����1.0��ʾ��ȫ��͸��
	//glShadeModel(GL_FLAT);
	//glMatrixMode(GL_PROJECTION);//OpenGL������ά��ʽ������ͼ��������Ҫһ��ͶӰ�任����άͼ��ͶӰ����ʾ���Ķ�ά�ռ���
	//gluOrtho2D(0.0, 200, 0.0, 150.0);//ָ��ʹ����ͶӰ��һ��x������0~200��y����0~150��Χ�ڵľ�����������ͶӰ����ʾ������
		// ͶӰ�����ʼ��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20.0, 1.0, 10.0, 300.0);
	// Ĭ�Ͼ����ʼ��
	glMatrixMode(GL_MODELVIEW);
	// �Ե�ǰ������г�ʼ��
	glLoadIdentity();
	/*����ԭ��
		void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz);
	�ú�������һ����ͼ���󣬲��뵱ǰ������ˡ�
		��һ��eyex, eyey, eyez ��������������λ��
		�ڶ���centerx, centery, centerz �����ͷ��׼�����������������λ��
		������upx, upy, upz ������ϵķ��������������еķ���
		�����������Ϊ���Լ����Դ���
		��һ�����ݾ����Դ���λ��
		�ڶ������ݾ����۾����������λ��
		���������ͷ������ķ�����Ϊ���������ͷ��ͬһ�����壩*/
	gluLookAt(100.0 + (x_min + x_max)*.5, 100.0 + (y_min + y_max)*.5, 100.0 + (z_min + z_max)*.5,
		(x_min + x_max)*.5, (y_min + y_max)*.5, (z_min + z_max)*.5, .0, .0, 1.0);
	modelview_z_dis = 100.0f * sqrt(3.0f);
	//���������������ȡ��GL_MODELVIEW_MATRIX��Ȼ��洢��mat��������У�������任��
	glGetFloatv(GL_MODELVIEW_MATRIX, default_matrix);
	// ��default������modelview�У�sizeof��ʾ������С
	memcpy(modelview_matrix, default_matrix, sizeof(default_matrix));
	glLoadIdentity();

}
// ��ʾ
void display_func()
{
	static int z = 0;
	std::cout << "display:" << z++ << std::endl;
	// ��ʾҪ�����ɫ�����Լ���Ȼ��壬����ʹ�����±�־λ
	// GL_COLOR_BUFFER_BIT:    ��ǰ��д����ɫ����
	// GL_DEPTH_BUFFER_BIT:    ��Ȼ���
	// GL_ACCUM_BUFFER_BIT : �ۻ�����
	// GL_STENCIL_BUFFER_BIT : ģ�建��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	// ���õ�ǰ����
	glLoadMatrixf(modelview_matrix);

	drawBunny();
	// ����
	// ����������������ָ�룬��������ɣ��������ʾ����Ļ�ϣ��Ӷ����Ƶ��ˢ�µ��µĻ�����˸����
	glutSwapBuffers();
}
// ����n������
void GetFieldcirculation(int index, int nField)
{
	// ����index�ҵ���,ע���ظ����⣬�Ѿ��жϵĵ㲻���ٴμ���
	list<int>_listPointTemp;
	_listPointTemp.push_back(index);
	list<int>_listPointTemp1;
	_listPointTemp1.push_back(index);
	while (nField > 0)
	{
		_listPointTemp = _listPointTemp1;
		list<int>_listPointTemp1;
		for (auto it = _listPointTemp.begin(); it != _listPointTemp.end(); it++)
		{

			// ��ȡindex���ĸ���	
			index = *it;
			for (auto iter = vecPoint.begin(); iter != vecPoint.end(); iter++)
			{
				// �ҵ���Ӧ�ĵ�	

				if (iter->second.index == index)
				{
					Point	getPoint1(iter->second._x, iter->second._y, iter->second._z);
					getPoint = getPoint1;
					// �ҵ���Ӧ����Ƭ��Ϣ
					for (auto it = vecFacet.begin(); it != vecFacet.end(); it++)
					{
						if (it->_vertex_1.index == index)
						{
							it->_vertex_1.flag = 1;
							_listPoint.insert(it->_vertex_1);

							if (it->_vertex_2.flag == 0)
							{
								_listPointTemp1.push_back(it->_vertex_2.index);
								_listPoint.insert(it->_vertex_2);
							}
							if (it->_vertex_3.flag == 0)
							{
								_listPointTemp1.push_back(it->_vertex_3.index);
								_listPoint.insert(it->_vertex_3);

							}
						}
						else if (it->_vertex_2.index == index)
						{
							it->_vertex_2.flag = 1;
							_listPoint.insert(it->_vertex_2);

							if (it->_vertex_1.flag == 0)
							{
								_listPointTemp1.push_back(it->_vertex_1.index);
								_listPoint.insert(it->_vertex_1);

							}
							if (it->_vertex_3.flag == 0)
							{
								_listPointTemp1.push_back(it->_vertex_3.index);
								_listPoint.insert(it->_vertex_3);

							}
						}
						else if (it->_vertex_3.index == index)
						{
							it->_vertex_3.flag = 1;
							_listPoint.insert(it->_vertex_3);
							if (it->_vertex_1.flag == 0)
							{
								_listPointTemp1.push_back(it->_vertex_1.index);
								_listPoint.insert(it->_vertex_1);
							}
							if (it->_vertex_2.flag == 0)
							{
								_listPointTemp1.push_back(it->_vertex_2.index);
								_listPoint.insert(it->_vertex_2);

							}
						}
					}
				}
			}
		}
		nField--;
	}
	_listPoint.erase(getPoint);
}
void ReadPly()
{
	ifstream fin("D:\\work\\����\\Task7\\bunny(1).ply");
	string s;
	int countPoint = 0;
	if (fin.fail())
	{
		cout << "���ļ�����!" << endl;
		exit(0);
	}
	while (getline(fin, s))
	{
		if (strstr(s.c_str(), "end_header") != NULL)
		{
			while (getline(fin, s))
			{
				//cout << "s:" << s << endl;
				istringstream iss(s);
				float x1, y1, z1;
				iss >> x1 >> y1 >> z1;
				Point point(countPoint, x1, y1, z1);
				cout << point._x <<" "<< point._y << endl; 
				point.flag = 0;
				vecPoint.insert(pair<int, Point>(countPoint, point));
				countPoint++;
				if (strstr(s.c_str(), "3 0 1 2") != NULL)
				{
					break;
				}
			}
			break;
		}
	}
	ifstream fin1("D:\\work\\����\\Task7\\bunny(1).ply");
	string s1;
	double Ssum = 0, TotalVolume = 0;
	while (getline(fin1, s1))
	{
		if (strstr(s1.c_str(), "3 0 1 2") != NULL)
		{
			//cout << "s1: " << s1 << endl;
			Facet facet;
			auto it = vecPoint.find(0);
			if (it != vecPoint.end())
			{
				facet._vertex_1 = it->second;
			}
			auto it1 = vecPoint.find(1);
			if (it1 != vecPoint.end())
			{
				facet._vertex_2 = it1->second;
			}
			auto it2 = vecPoint.find(2);
			if (it2 != vecPoint.end())
			{
				facet._vertex_3 = it2->second;
			}
			vecFacet.push_back(facet);
			while (getline(fin, s))
			{
				//cout << "s11: " << s << endl;
				istringstream isss(s);
				Facet facet;
				int a, b, c, d;
				isss >> a >> b >> c >> d;

				auto it = vecPoint.find(b);
				if (it != vecPoint.end())
				{
					facet._vertex_1 = it->second;
				}
				auto it1 = vecPoint.find(c);
				if (it1 != vecPoint.end())
				{
					facet._vertex_2 = it1->second;
				}
				auto it2 = vecPoint.find(d);
				if (it2 != vecPoint.end())
				{
					facet._vertex_3 = it2->second;
				}
				vecFacet.push_back(facet);
			}
			break;
		}
	}
}

void absolute_default()
{
	memcpy(modelview_matrix, default_matrix, sizeof(default_matrix));
}
// ���״̬������������������¼����϶��¼�֮��ͨ��
static bool l_button_down = false, r_button_down = false, mid_button_down = false;
static int last_x = -1, last_y = -1;
#define  GLUT_WHEEL_UP		3 // ���ֲ���  
#define  GLUT_WHEEL_DOWN	4
void mouse_click_func(int button, int state, int x, int y)
{
	y = win_h - y;
	switch (button) {
		// ��ť
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			l_button_down = true;
			last_x = x; last_y = y;
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		}
		else {
			l_button_down = false;
			last_x = -1; last_y = -1;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		break;
		// �м�
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			mid_button_down = true;
			last_x = x; last_y = y;
			glutSetCursor(GLUT_CURSOR_CYCLE);

		}
		else {
			mid_button_down = false;
			last_x = -1; last_y = -1;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		break;
		// �ұ�
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			r_button_down = true;
			absolute_default();
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			glutPostRedisplay();
		}
		else {
			r_button_down = false;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		break;
		// ת��
	case GLUT_WHEEL_UP:
		if (state == GLUT_UP) {
			absolute_scale(.9f);
			glutPostRedisplay();
		}
		break;
		// ����ת��
	case GLUT_WHEEL_DOWN:
		if (state == GLUT_UP) {
			absolute_scale(1.1f);
			glutPostRedisplay();
		}
		break;
	}

}
// ����ƶ�
void mouse_move_func(int x, int y)
{
	y = win_h - y;
	if (last_x >= 0 && last_y >= 0 && (last_x != x || last_y != y)) {
		GLfloat deltax = GLfloat(x - last_x), deltay = GLfloat(y - last_y);
		if (mid_button_down) {
			absolute_translate(deltax * .1f, deltay * .1f, .0f);
			glutPostRedisplay();
		}
		else if (l_button_down) {
			GLfloat dis = sqrt(deltax*deltax + deltay * deltay);
			absolute_rotate(dis, -deltay / dis, deltax / dis, .0);
			glutPostRedisplay();
		}
	}
	last_x = x; last_y = y;
}