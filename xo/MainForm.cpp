#include "MainForm.h"

xo::MainForm::MainForm(void): turn(1){
	InitializeComponent();
	Width = 300;
	Height = 300;
	turnlable = gcnew Label();
	turnlable->Location = Point(75, 10);
	turnlable->Text = L"Turn is " + (turn ? "X" : "0");
	
	Controls->Add(turnlable);

	int x = 50, y = 50;
	for (int i = 0; i < 9; ++i) {
		Button^ button = gcnew Button;
		button->Width = 50;
		button->Height = 50;

		button->Location = Point(x, y);
		button->Click += gcnew EventHandler(this, &MainForm::button_click);

		Controls->Add(button);
		buttons[i] = button;
		x += 50;
		if ((i + 1) % 3 == 0) {
			x = 50;
			y += 50;
		}
	}
}

void xo::MainForm::button_click(Object^ sender, EventArgs^ e){
	Button^ button = static_cast<Button^>(sender);
	if (button->Text == L"") {
		if (turn) button->Text = L"X";
		else button->Text = L"0";
		turn = 1 - turn;
		check_win();
	}
}

void xo::MainForm::cleaning_game(){
	for (int i = 0; i < 9; ++i)
		buttons[i]->Text = L"";
	turn = 1;
}

void xo::MainForm::check_win(){
	for (int i = 0; i < 3; ++i) {
		if (buttons[i]->Text != L"" &&
			buttons[i]->Text == buttons[i + 3]->Text &&
			buttons[i]->Text == buttons[i + 6]->Text
			) {
			MessageBox::Show("Player " + buttons[i]->Text + " Winer!");
			cleaning_game();
		}
		else if (
			buttons[i * 3]->Text != L""&&
			buttons[i * 3]->Text == buttons[i * 3 * 1]->Text &&
			buttons[i * 3]->Text == buttons[i * 3 + 2]->Text
			) {
			MessageBox::Show("Player " + buttons[i * 3]->Text + " Winer!");
			cleaning_game();
		}
	}
	if (
		buttons[0]->Text != L"" &&
		buttons[0]->Text == buttons[4]->Text &&
		buttons[0]->Text == buttons[8]->Text
		) {
		MessageBox::Show("Player " + buttons[0]->Text + " Winer!");
		cleaning_game();
	}
	if (
		buttons[2]->Text != L"" &&
		buttons[2]->Text == buttons[4]->Text &&
		buttons[2]->Text == buttons[6]->Text
		) {
		MessageBox::Show("Player " + buttons[2]->Text + " Winer!");
		cleaning_game();
	}
	bool is_draw = true;
	for (int i = 0; i < 9; ++i)
		if (buttons[i]->Text == L"") is_draw = false;
	if (is_draw) {
		MessageBox::Show("Game over draw");
		cleaning_game();
	}
	turnlable->Text = L"Turn of " + (turn ? "X" : "0");
}
