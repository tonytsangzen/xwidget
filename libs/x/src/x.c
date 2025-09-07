#include <x/xwin.h>
#include <x/x.h>
#include <tinyjson/tinyjson.h>
#include <ewoksys/basic_math.h>
#include <font/font.h>
#include <ewoksys/proc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <path.h>

#define DEBUG 0
#if DEBUG
#define DBG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#define FS_FULL_NAME_MAX 128
	static int x_get_event(x_t *x, xevent_t *ev, bool block)
	{
		static int _mouse_drag = 0;
		static int _mouse_double_click = 0;
		static long _mouse_click_ts = 0;

	 	SDL_Event e;
		if(_mouse_double_click){
			_mouse_double_click = 0;
			DBG("Mouse double click\n");	
			ev->type = XEVT_MOUSE;
			ev->state = MOUSE_STATE_CLICK;
			return 1;
		}
		memset(ev, 0, sizeof(xevent_t));
		int ret = SDL_PollEvent(&e);
		if (ret)
		{
			sdl_window_t *win = sdl_get_window(x->sdl, e.window.windowID);
			if(win == NULL)
				return 0;
			ev->win = win->data;
			DBG("xwin %p\n", ev->win);
			switch (e.type)
			{
			case SDL_WINDOWEVENT:
				DBG("window event: %d\n", e.window.event);
				if(e.window.event == SDL_WINDOWEVENT_CLOSE) {
					ev->type = XEVT_WIN;
					ev->state = XEVT_WIN_CLOSE;
				}
				break;
			case SDL_QUIT:
				DBG("Quit\n");
				ev->type = XEVT_WIN;
				ev->state = XEVT_WIN_CLOSE;
				break;
			case SDL_MOUSEMOTION:
				DBG("Mouse move: %d %d\n", e.motion.x, e.motion.y);
				ev->type = XEVT_MOUSE;
				if(_mouse_drag)
					ev->state = MOUSE_STATE_DRAG;
				else
					ev->state = MOUSE_STATE_MOVE;
				ev->value.mouse.x = e.motion.x;
				ev->value.mouse.y = e.motion.y;
				ev->value.mouse.rx = e.motion.xrel;
				ev->value.mouse.ry = e.motion.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
				DBG("Mouse down: %d %d %d\n", e.button.x, e.button.y, e.button.button);
				ev->type = XEVT_MOUSE;
				ev->state = MOUSE_STATE_DOWN;
				ev->value.mouse.x = e.button.x;
				ev->value.mouse.y = e.button.y;	
				if(kernel_tic_ms() - _mouse_click_ts < 300)
					_mouse_double_click = 1;
				_mouse_click_ts = kernel_tic_ms();
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					_mouse_drag = 1;
					ev->value.mouse.button = MOUSE_BUTTON_LEFT;
					break;
				case SDL_BUTTON_RIGHT:
					ev->value.mouse.button = MOUSE_BUTTON_RIGHT;
					break;
				case SDL_BUTTON_MIDDLE:
					ev->value.mouse.button = MOUSE_BUTTON_MID;
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				DBG("Mouse up: %d %d %d\n", e.button.x, e.button.y, e.button.button);
				ev->type = XEVT_MOUSE;
				ev->state = MOUSE_STATE_UP;
				ev->value.mouse.x = e.button.x;
				ev->value.mouse.y = e.button.y;	
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					_mouse_drag = 0;
					ev->value.mouse.button = MOUSE_BUTTON_LEFT;
					break;
				case SDL_BUTTON_RIGHT:
					ev->value.mouse.button = MOUSE_BUTTON_RIGHT;
					break;
				case SDL_BUTTON_MIDDLE:
					ev->value.mouse.button = MOUSE_BUTTON_MID;
					break;
				default:
					break;
				}
				break;
			case SDL_KEYDOWN:
				DBG("Key down: %d\n", e.key.keysym.sym);
				ev->type = XEVT_IM;
				ev->state = XIM_STATE_PRESS;
				ev->value.im.value = e.key.keysym.sym;
				break;
			case SDL_KEYUP:
				DBG("Key up: %d\n", e.key.keysym.sym);
				break;
			}
		}
		return ret;
	}

	int x_screen_info(xscreen_info_t *scr, uint32_t index)
	{
		scr->size.w = 640;
		scr->size.h = 480;
		return 0;
	}

	typedef struct
	{
		xscreen_info_t screen;
		graph_t g;
	} xscreen_t;

#define SCREEN_MAX 8
	static xscreen_t _x_screens[SCREEN_MAX];

	int x_fetch_screen_graph(uint32_t index, graph_t *g)
	{
		printf("Todo: %s %s\n", __FILE__, __func__);
		return 0;
	}

	int x_get_display_num(void)
	{
		return 0;
	}

	/*
	static void sig_stop(int sig_no, void* p) {
		(void)sig_no;
		x_t* x = (x_t*)p;
		x->terminated = true;
	}
	*/

	void x_terminate(x_t *x)
	{
		x->terminated = true;
	}

	const char *x_get_work_dir(void)
	{
		return get_executable_path();
	}

	static x_theme_t _x_theme;

	static int x_update_theme(void)
	{
		printf("Todo: %s %s\n", __FILE__, __func__);
		return 0;
	}

	int x_get_theme(x_theme_t *theme)
	{
		// memcpy(theme, &_x_theme, sizeof(x_theme_t));
		theme->uuid = 0;
		strcpy(theme->name, "default");
		strcpy(theme->fontName, "font.ttf");
		theme->fontSize = 16;
		theme->charSpace = 0;
		theme->lineSpace = 0;

		theme->bgColor = 0xffffffff;
		theme->fgColor = 0xff000000;
		theme->docBGColor = 0xffffffff;
		theme->docFGColor = 0xff000000;

		theme->bgDisableColor = 0xfff0f0f0;
		theme->fgDisableColor = 0xff000000;

		theme->selectColor = 0xff000000;
		theme->selectBGColor = 0xfff0f0f0;

		theme->titleColor = 0xff000000;
		theme->titleBGColor = 0xfff0f0f0;

		return 0;
	}

	void x_init(x_t *x, void *data)
	{
		memset(&_x_theme, 0, sizeof(x_theme_t));
		x_get_theme(&_x_theme);

		memset(_x_screens, 0, sizeof(xscreen_info_t) * SCREEN_MAX);
		memset(x, 0, sizeof(x_t));
		x->data = data;
		// sys_signal(SYS_SIG_STOP, sig_stop, x);
	}

	int x_get_desktop_space(int disp_index, grect_t *r)
	{
		printf("Todo: %s %s\n", __FILE__, __func__);
		r->x = 0;
		r->y = 0;
		r->w = 640;
		r->h = 480;
		return 0;
	}

	int x_set_desktop_space(int disp_index, const grect_t *r)
	{
		printf("Todo: %s %s\n", __FILE__, __func__);
		return 0;
	}

	int x_set_top_app(const char *fname)
	{
		printf("Todo: %s %s\n", __FILE__, __func__);
		return 0;
	}

	int x_set_app_name(x_t *x, const char *fname)
	{
		printf("Todo: %s %s\n", __FILE__, __func__);
		return 0;
	}

	int x_exec(const char *fname)
	{
		printf("Todo: %s %s\n", __FILE__, __func__);
		return 0;
	}

	const char *x_get_theme_fname(const char *prefix, const char *app_name, const char *fname)
	{
		static char ret[256] = {0};
		if (fname[0] == '/')
		{
			strncpy(ret, fname, 255);
			return ret;
		}

		x_theme_t theme;
		x_get_theme(&theme);

		if (app_name == NULL || app_name[0] == 0)
			snprintf(ret, 255, "%s/%s/%s", prefix, theme.name, fname);
		else
			snprintf(ret, 255, "%s/%s/%s/%s", prefix, theme.name, app_name, fname);
		return ret;
	}

	const char *x_get_res_name(const char *name)
	{
		static char ret[FS_FULL_NAME_MAX] = "";
		if (name == NULL || name[0] == 0)
			return ret;

		if (name[0] == '/')
		{
			strncpy(ret, name, FS_FULL_NAME_MAX - 1);
			return ret;
		}

		const char *wkdir = x_get_work_dir();
		if (wkdir[1] == 0 && wkdir[0] == '/')
			snprintf(ret, FS_FULL_NAME_MAX - 1, "/res/%s", name);
		else
			snprintf(ret, FS_FULL_NAME_MAX - 1, "%s/res/%s", wkdir, name);
		return ret;
	}

	int x_run(x_t *x, void *loop_data)
	{
		bool block = x->on_loop == NULL ? true : false;
		xevent_t xev;
		while (!x->terminated)
		{
			sdl_render(x->sdl);
			int res = x_get_event(x, &xev, block);
			if (res)
			{
				xwin_t *xwin = xev.win;
				DBG("xwin %p, %d %d\n", xwin, xev.type, xev.state);
				if (xev.type == XEVT_WIN)
				{
					xwin_event_handle(xwin, &xev);
				}
				else if (xwin->on_event != NULL)
				{
					if (xwin->x->prompt_win == NULL ||
						xwin->x->prompt_win == xwin) // has prompt win, can't response
						xwin->on_event(xwin, &xev);
				}
			}
			else if (x->on_loop != NULL)
			{
				x->on_loop(loop_data);
			}
		}
		return 0;
	}

#ifdef __cplusplus
}
#endif
