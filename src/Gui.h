#include <iostream>
#include <vector>
#include <map>
#include <initializer_list>
#include <functional>
#include <string>
#include "raylib.h"
#include "raymath.h"

// foward declarations...
struct GUI_Element;


enum GUI_ELEMENT_TYPE { NONE, PANEL, LABEL, BUTTON, TOGGLE, TEXTINPUT, BAR, IMAGE};
class GUI
{
public:
	std::map<std::string, GUI_Element*> elements;
	std::map<std::string, std::string> data;
	#if defined(PLATFORM_WEB)
		Font font = LoadFont("resources/ConsolaMono-Bold.ttf");
	#else
		Font font = LoadFont("./src/resources/ConsolaMono-Bold.ttf");
	#endif

	GUI();
	~GUI();
	void update();
	void draw();
	void add_element(GUI_ELEMENT_TYPE type, std::string name, std::string parent, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list);
	void del_element(std::string name);
};

struct GUI_Element {
	std::string name = "";
	GUI_ELEMENT_TYPE type = GUI_ELEMENT_TYPE::NONE;
	GUI* gui = nullptr;
	GUI_Element* parent = nullptr;
	std::map<std::string, GUI_Element*> children;
	std::map<std::string, std::string> data;
	int X = 0;
	int Y = 0;
	int W = 0;
	int H = 0;
	Rectangle rec{ 0.0f, 0.0f, 0.0f, 0.0f };
	bool mouse_inside_prev = false;
	bool mouse_inside_now = false;

	GUI_Element();
	~GUI_Element();
	virtual void update() = 0;
	virtual void draw() = 0;
	void add_element(GUI_Element* e);
	void del_element(std::string n);
};

struct GUI_Panel : GUI_Element {
	GUI_Panel(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h);
	void update();
	void draw();
};

struct GUI_Label : GUI_Element {
	GUI_Label(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list);
	void update();
	void draw();
};

struct GUI_Button : GUI_Element {
	bool mouse_inside_prev = false;
	bool mouse_inside_now = false;
	std::function<void()> func;
	GUI_Button(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list);
	void update();
	void draw();
};

struct GUI_Toggle : GUI_Element {
	bool mouse_inside_prev = false;
	bool mouse_inside_now = false;
	std::function<void()> func;
	GUI_Toggle(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list);
	void update();
	void draw();
};

struct GUI_TextInput : GUI_Element {
	bool mouse_inside_prev = false;
	bool mouse_inside_now = false;
	bool input_active = false;
	std::function<void()> func;
	GUI_TextInput(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list);
	void update();
	void draw();
};

struct GUI_Bar : GUI_Element {
	float portion_full = 0.5f;
	Color color = RED;
	GUI_Bar(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list);
	void update();
	void draw();
};

struct GUI_Image : GUI_Element {
	Texture2D tex;
	std::string file;
	Rectangle src_rec;
	GUI_Image(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list);
	~GUI_Image();
	void update();
	void draw();
};
