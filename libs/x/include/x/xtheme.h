#ifndef XTHEME_H
#define XTHEME_H

#include <ewoksys/ewokdef.h>
#include <font/font.h>

#ifdef __cplusplus
extern "C" {
#endif

#define X_THEME_ROOT        "/usr/x/themes"
#define THEME_NAME_MAX 64 
#define X_DEFAULT_XTHEME    "opencde"

typedef struct {
	uint32_t uuid;
	char     name[THEME_NAME_MAX];

	char     fontName[FONT_NAME_MAX];
	uint32_t fontSize;
	int32_t  charSpace;
	int32_t  lineSpace;

	uint32_t bgColor;
	uint32_t fgColor;

	uint32_t docBGColor;
	uint32_t docFGColor;

	uint32_t bgDisableColor;
	uint32_t fgDisableColor;

	uint32_t selectColor;
	uint32_t selectBGColor;

	uint32_t titleColor;
	uint32_t titleBGColor;
} x_theme_t;

#ifdef __cplusplus
}
#endif

#endif
