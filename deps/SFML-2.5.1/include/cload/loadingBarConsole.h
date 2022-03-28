#ifdef LOADINGBARCONSOLE_EXPORTS
#define LOADINGBARCONSOLE_API __declspec(dllexport)
#else
#define LOADINGBARCONSOLE_API __declspec(dllimport)
#endif

enum colorcode {
	OK,
	SUSPEND,
	CANCEL
};

class LOADINGBARCONSOLE_API CloadingBarConsole {
private:
	void* hConsole;
	int* pcent;
	colorcode cc = colorcode::OK;
public:
	CloadingBarConsole(void);
	void render();
	void setPercent(int p);
	~CloadingBarConsole();
	void setColorCode(colorcode colorcode = colorcode::OK);
};