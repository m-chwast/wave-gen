#include <screen/menu/menu_elements.h>
#include <stdlib.h>


static void Callback_WaveSetup_Type();


static MenuElement menuRun;
static MenuElement menuMode;
static MenuElement waveSetup;
static MenuElement waveSetup_type;
static MenuElement waveSetup_frequency;
static MenuElement waveSetup_return;

MenuElement * const MenuElements_initialMenuElement = &menuRun;

static MenuElement menuRun =
{
		.text = "Run",
		.callback = NULL,
		.parent = NULL,
		.submenu = NULL,
		.prev = NULL,
		.next = &menuMode,
};

static MenuElement menuMode =
{
		.text = "Mode Select",
		.callback = NULL,
		.parent = NULL,
		.submenu = NULL,
		.prev = &menuRun,
		.next = &waveSetup,
};

static MenuElement waveSetup =
{
		.text = "Wave Setup...",
		.callback = NULL,
		.parent = NULL,
		.submenu = &waveSetup_type,
		.prev = &menuMode,
		.next = NULL,
};

static MenuElement waveSetup_type =
{
		.text = "Type",
		.properties.valueType = VALUE_TYPE_WAVE_TYPE,
		.callback = Callback_WaveSetup_Type,
		.next = &waveSetup_frequency,
};

static MenuElement waveSetup_frequency =
{
		.text = "Freq",
		.properties.valueType = VALUE_TYPE_UINT,
		.prev = &waveSetup_type,
		.next = &waveSetup_return,
};

static MenuElement waveSetup_return =
{
	.text = "Return...",
	.parent = &menuRun,
	.prev = &waveSetup_frequency,
};


static void Callback_WaveSetup_Type()
{

}
