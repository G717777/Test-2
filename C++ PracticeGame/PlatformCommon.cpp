struct ButtonState
{
	bool isDown;
	bool changed;
};

enum
{
	Button_Up,
	Button_Down,

	Button_W,
	Button_S,

	Button_Count,

};

struct Input
{
	ButtonState buttons[Button_Count];

};