#include "Gui.h"

GUI::GUI() {
	elements = std::map<std::string, GUI_Element*>();
	data = std::map<std::string, std::string>();

	
}

GUI::~GUI() {
	for (std::pair<std::string, GUI_Element*> e : elements) {
		delete e.second;
	}
	UnloadFont(font);
}

void GUI::add_element(GUI_ELEMENT_TYPE type, std::string name, std::string parent, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list) {
	// make the new element...
	GUI_Element* element = nullptr;
	GUI_Element* p = (parent == "NULLPTR" ? nullptr : elements[parent]);
	switch (type) {
		case NONE: break; // should never be none?
		case PANEL: element = new GUI_Panel(name, this, p, x, y, w, h); break;
		case LABEL: element = new GUI_Label(name, this, p, x, y, w, h, list); break;
		case BUTTON: element = new GUI_Button(name, this, p, x, y, w, h, list); break;
		case TOGGLE: element = new GUI_Toggle(name, this, p, x, y, w, h, list); break;
		case TEXTINPUT: element = new GUI_TextInput(name, this, p, x, y, w, h, list); break;
		case BAR: element = new GUI_Bar(name, this, p, x, y, w, h, list); break;
		case IMAGE: element = new GUI_Image(name, this, p, x, y, w, h, list); break;
		default: break;
	}
	if (element != nullptr) {
		elements.insert({ name, element });
		if (p != nullptr) {
			p->add_element(element);
		}
	}
}

void GUI::update() {
	for (std::pair<std::string, GUI_Element*> e : elements) {
		e.second->update();
	}
}

void GUI::draw() {
	for (std::pair<std::string, GUI_Element*> e : elements) {
		if (e.second->parent == nullptr) { // tell elements with no parents to draw, those will call the draw function on children...
			e.second->draw();
		}
	}
}

GUI_Element::GUI_Element() {
	type = GUI_ELEMENT_TYPE::NONE;
	children = std::map<std::string, GUI_Element*>();
	data = std::map<std::string, std::string>();
}

GUI_Element::~GUI_Element() {
	// do nothing? GUI instance handles deletion through del_element()?
}

void GUI_Element::add_element(GUI_Element* e) {
	children.insert({ e->name, e });
}

void GUI_Element::del_element(std::string n) {
	children[n]->parent = nullptr;
	children.erase(n);
}

GUI_Panel::GUI_Panel(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h) {
	type = GUI_ELEMENT_TYPE::PANEL;
	name = n;
	gui = g;
	parent = p;
	X = (parent != nullptr ? parent->X + x : x);
	Y = (parent != nullptr ? parent->Y + y : y);
	W = w;
	H = h;
	rec = Rectangle{ (float)X, (float)Y, (float)W, (float)H };
}

void GUI_Panel::update() {
	// do nothing?
}

void GUI_Panel::draw() {
	DrawRectangle(X, Y, W, H, WHITE);
	DrawRectangleLinesEx(rec, 4, BLACK);

	// tell children to draw...
	for (std::pair<std::string, GUI_Element*> e : children) {
		e.second->draw();
	}
}

GUI_Label::GUI_Label(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list) {
	type = GUI_ELEMENT_TYPE::LABEL;
	name = n;
	gui = g;
	parent = p;
	X = (parent != nullptr ? parent->X + x : x);
	Y = (parent != nullptr ? parent->Y + y : y);
	W = w;
	H = h;
	rec = Rectangle{ (float)X, (float)Y, (float)W, (float)H };
	for(std::pair<std::string, std::string> p : list) {
		data.insert(p);
	}
}

void GUI_Label::update() {
	// do nothing?
}

void GUI_Label::draw() {
	DrawTextPro(gui->font, data["text"].c_str(), Vector2{(float)(X + 2), (float)(Y)}, Vector2{(float)(0), (float)(0)}, 0.0f, 24.0f, 0.0f, BLACK);
	// tell children to draw...
	for (std::pair<std::string, GUI_Element*> e : children) {
		e.second->draw();
	}
}

GUI_Button::GUI_Button(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list) {
	type = GUI_ELEMENT_TYPE::BUTTON;
	name = n;
	gui = g;
	parent = p;
	X = (parent != nullptr ? parent->X + x : x);
	Y = (parent != nullptr ? parent->Y + y : y);
	W = w;
	H = h;
	rec = Rectangle{ (float)X, (float)Y, (float)W, (float)H };
	for (std::pair<std::string, std::string> p : list) {
		data.insert(p);
	}
	func = []() {};
}

void GUI_Button::update() {
	mouse_inside_now = CheckCollisionPointRec(GetMousePosition(), rec);
	// mouse enter
	if (mouse_inside_now == true && mouse_inside_prev == false) {
		SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
	}
	// mouse leave
	if (mouse_inside_now == false && mouse_inside_prev == true) {
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	}
	mouse_inside_prev = mouse_inside_now;
	// on click
	if (mouse_inside_now == true && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		func();
	}
}

void GUI_Button::draw() {
	DrawRectangleRec(rec, LIGHTGRAY);
	DrawRectangleLinesEx(rec, 1, BLACK);
	DrawTextPro(gui->font, data["text"].c_str(), Vector2{(float)(X + 2), (float)(Y)}, Vector2{(float)(0), (float)(0)}, 0.0f, 24.0f, 0.0f, BLACK);
	// tell children to draw...
	for (std::pair<std::string, GUI_Element*> e : children) {
		e.second->draw();
	}
}

GUI_Toggle::GUI_Toggle(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list) {
	type = GUI_ELEMENT_TYPE::TOGGLE;
	name = n;
	gui = g;
	parent = p;
	X = (parent != nullptr ? parent->X + x : x);
	Y = (parent != nullptr ? parent->Y + y : y);
	W = w;
	H = h;
	rec = Rectangle{ (float)X, (float)Y, (float)W, (float)H };
	for (std::pair<std::string, std::string> p : list) {
		data.insert(p);
	}
	func = []() {};
}

void GUI_Toggle::update() {
	mouse_inside_now = CheckCollisionPointRec(GetMousePosition(), rec);
	// mouse enter
	if (mouse_inside_now == true && mouse_inside_prev == false) {
		SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
	}
	// mouse leave
	if (mouse_inside_now == false && mouse_inside_prev == true) {
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	}
	mouse_inside_prev = mouse_inside_now;
	// on click
	if (mouse_inside_now == true && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (data["state"] == "true") {
			data["state"] = "false";
		}
		else if(data["state"] == "false") {
			data["state"] = "true";
		}
		func();
	}
}

void GUI_Toggle::draw() {
	DrawRectangleRec(rec, LIGHTGRAY);
	DrawRectangleLinesEx(rec, 1, BLACK);
	if (data["state"] == "true") {
		DrawRectangle(X + 2, Y + 2, W - 4, H - 4, BLACK);
	}
	// tell children to draw...
	for (std::pair<std::string, GUI_Element*> e : children) {
		e.second->draw();
	}
}

GUI_TextInput::GUI_TextInput(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list) {
	type = GUI_ELEMENT_TYPE::TEXTINPUT;
	name = n;
	gui = g;
	parent = p;
	X = (parent != nullptr ? parent->X + x : x);
	Y = (parent != nullptr ? parent->Y + y : y);
	W = w;
	H = h;
	rec = Rectangle{ (float)X, (float)Y, (float)W, (float)H };
	for (std::pair<std::string, std::string> p : list) {
		data.insert(p);
	}
	func = []() {};
}

void GUI_TextInput::update() {
	mouse_inside_now = CheckCollisionPointRec(GetMousePosition(), rec);
	// mouse enter
	if (mouse_inside_now == true && mouse_inside_prev == false) {
		SetMouseCursor(MOUSE_CURSOR_IBEAM);
	}
	// mouse leave
	if (mouse_inside_now == false && mouse_inside_prev == true) {
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	}
	mouse_inside_prev = mouse_inside_now;
	// on click
	if (mouse_inside_now == true && input_active == false && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		input_active = true;
	}
	if (mouse_inside_now == false && input_active == true && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		input_active = false;
	}
	// get text...
	if (input_active) {
		int key = GetCharPressed();
		while (key > 0) {
			if ((key >= 32) && (key <= 125) && (data["text"].size() <= std::stoi(data["max_chars"]))){
				data["text"].push_back((char)key);
			}
			key = GetCharPressed();
		}
		if (IsKeyPressed(KEY_BACKSPACE)) {
			data["text"] = data["text"].substr(0, data["text"].size() - 1);
		}
		if (IsKeyPressed(KEY_ENTER)) {
			input_active = false;
			func();
		}
	}
}

void GUI_TextInput::draw() {
	DrawRectangleRec(rec, LIGHTGRAY);
	DrawRectangleLinesEx(rec, 1, BLACK);
	std::string tmp = data["text"];
	if (input_active && (int)(5 * GetTime()) % 2 == 1) {
		tmp.push_back('|');
	}
	DrawTextPro(gui->font, tmp.c_str(), Vector2{ (float)(X + 2), (float)(Y) }, Vector2{ (float)(0), (float)(0) }, 0.0f, 24.0f, 0.0f, BLACK);
	// tell children to draw...
	for (std::pair<std::string, GUI_Element*> e : children) {
		e.second->draw();
	}
}

GUI_Bar::GUI_Bar(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list) {
	type = GUI_ELEMENT_TYPE::BAR;
	name = n;
	gui = g;
	parent = p;
	X = (parent != nullptr ? parent->X + x : x);
	Y = (parent != nullptr ? parent->Y + y : y);
	W = w;
	H = h;
	rec = Rectangle{ (float)X, (float)Y, (float)W, (float)H };
	for (std::pair<std::string, std::string> p : list) {
		data.insert(p);
	}
}

void GUI_Bar::update() {
	portion_full = std::stof(data["current"]) / std::stof(data["max"]);
	color = Color{ (unsigned char)std::stoi(data["red"]), (unsigned char)std::stoi(data["green"]), (unsigned char)std::stoi(data["blue"]), 255 };
}

void GUI_Bar::draw() {
	DrawRectangle(X, Y, W, H, LIGHTGRAY);
	DrawRectangle(X, Y, (int)(W * portion_full), H, color);
	DrawRectangleLines(X, Y, W, H, BLACK);
}

GUI_Image::GUI_Image(std::string n, GUI* g, GUI_Element* p, int x, int y, int w, int h, std::initializer_list<std::pair<std::string, std::string> > list) {
	type = GUI_ELEMENT_TYPE::IMAGE;
	name = n;
	gui = g;
	parent = p;
	file = "";
	X = (parent != nullptr ? parent->X + x : x);
	Y = (parent != nullptr ? parent->Y + y : y);
	W = w;
	H = h;
	rec = Rectangle{ (float)X, (float)Y, (float)W, (float)H };
	for (std::pair<std::string, std::string> p : list) {
		data.insert(p);
	}
	Image tmp = LoadImage(data["file"].c_str());
	tex = LoadTextureFromImage(tmp);
	UnloadImage(tmp);
	file = data["file"];
	src_rec = Rectangle{ 0.0f, 0.0f, (float)tex.width, (float)tex.height };
}

GUI_Image::~GUI_Image() {
	UnloadTexture(tex);
}

void GUI_Image::update() {
	if (file != data["file"]) {
		Image tmp = LoadImage(data["file"].c_str());
		tex = LoadTextureFromImage(tmp);
		UnloadImage(tmp);
		file = data["file"];
		src_rec = Rectangle{ 0.0f, 0.0f, (float)tex.width, (float)tex.height };
	}
}

void GUI_Image::draw() {
	DrawTexturePro(tex, src_rec, rec, Vector2Zeros, 0.0f, WHITE);
	DrawRectangleLines(X, Y, W, H, BLACK);
}
