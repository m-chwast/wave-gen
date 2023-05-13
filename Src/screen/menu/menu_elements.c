#include <screen/menu/menu_elements.h>
#include <stdlib.h>


static void Callback_WaveSetup_Type();


static const MenuElement menuRun;
static const MenuElement menuMode;
static const MenuElement waveSetup;
static const MenuElement waveSetup_type;
static const MenuElement waveSetup_return;

const MenuElement * const MenuElements_initialMenuElement = &menuRun;

static const MenuElement menuRun =
{
		.text = "Run",
		.callback = NULL,
		.parent = NULL,
		.submenu = NULL,
		.prev = NULL,
		.next = &menuMode,
};

static const MenuElement menuMode =
{
		.text = "Mode Select",
		.callback = NULL,
		.parent = NULL,
		.submenu = NULL,
		.prev = &menuRun,
		.next = &waveSetup,
};

static const MenuElement waveSetup =
{
		.text = "Wave Setup...",
		.callback = NULL,
		.parent = NULL,
		.submenu = &waveSetup_type,
		.prev = &menuMode,
		.next = NULL,
};

static const MenuElement waveSetup_type =
{
		.text = "Type",
		.properties.valueType = VALUE_TYPE_WAVE_TYPE,
		.callback = Callback_WaveSetup_Type,
		.next = &waveSetup_return,
};

static const MenuElement waveSetup_return =
{
	.text = "Return...",
	.parent = &menuRun,
	.prev = &waveSetup_type,
};


static void Callback_WaveSetup_Type()
{

}
