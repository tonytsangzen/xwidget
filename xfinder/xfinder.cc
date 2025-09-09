#include <Widget/WidgetWin.h>
#include <Widget/WidgetX.h>
#include <WidgetEx/FileWidget.h>
#include <x++/X.h>
#include <unistd.h>
#include <ewoksys/proc.h>
#include <graph/graph_png.h>
#include <dirent.h>

#include <string>
using namespace std;
using namespace Ewok;

class FileManager: public FileWidget {


protected:
	void onFile(const string& fname, const string& open_with) {
	}

	void onPath(const string& pathname) {
	}
};

int main(int argc, char** argv) {
	printf("%s\n", __func__);
	X x;
	WidgetWin win;
	RootWidget* root = new RootWidget();
	win.setRoot(root);
	root->setType(Container::VERTICLE);
	

	FileManager* fm = new FileManager();
	root->add(fm);
	root->focus(fm);

	win.open(&x, 0, 0, 0, 640, 480, "xfinder", XWIN_STYLE_NORMAL);
	win.busy(true);
	fm->loadDir("/");
	win.repaint();
	win.busy(false);
	widgetXRun(&x, &win);
	return 0;
}
