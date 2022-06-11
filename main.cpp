#include "main.h"
#include <cmath>
#include "rc.h"

#define _USE_MATH_DEFINES
#include <math.h>

int number_dialog::idd() const {
	return IDD_NUMBER;
}
bool number_dialog::on_init_dialog() {
	set_int(IDC_EDIT1, brojKrugova);
	return true;
}
bool number_dialog::on_ok() {
	try {
		brojKrugova = get_int(IDC_EDIT1);
	}
	catch (std::exception&) {
		return false;
	}
	return true;
}

void main_window::on_paint(HDC hdc) {
	RECT rc;
	POINT centerRc;

	HBRUSH hBrush = CreateSolidBrush(color);
	HGDIOBJ brush = SelectObject(hdc, hBrush);

	GetClientRect(*this, &rc);

	centerRc.x = rc.right / 2;
	centerRc.y = rc.bottom / 2;

	const int r = rc.bottom / 4;

	SelectObject(hdc, GetStockObject(NULL_PEN));
	SetROP2(hdc, R2_NOTXORPEN);
	for (int i = 0; i < brojkrugova; ++i) {
		const double degStep = 2 * M_PI * i / brojkrugova;
		const int x = centerRc.x + r * sin(degStep);
		const int y = centerRc.y + r * cos(degStep);

		Ellipse(hdc, x - r, y - r, x + r, y + r);
	}
	SelectObject(hdc, brush);
	DeleteObject(hBrush);
}

void main_window::on_command(int id){
	switch(id){
		case ID_NUMBER:
		{
			number_dialog nDlg;
			nDlg.brojKrugova = brojkrugova;
			if (nDlg.do_modal(0, *this)== IDOK) {
				brojkrugova = nDlg.brojKrugova;
				InvalidateRect(*this, nullptr, true);
			}
			break;
		}
		case ID_COLOR: 
		{

			CHOOSECOLOR cc;
			static COLORREF custCols[16] = { 0 };
			cc.hwndOwner = *this;  //glavni prozor mu je parent ali dialog može izaæi izvan njega pa je owner
			ZeroMemory(&cc, sizeof(CHOOSECOLOR));
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			cc.lpCustColors = (LPDWORD) custCols;
			cc.rgbResult = color;
			if (ChooseColor(&cc)) {
				color = cc.rgbResult;
			}
			InvalidateRect(*this, nullptr, true);
			break;
		}
		
		case ID_EXIT: 
			DestroyWindow(*this); 
			break;
	}
}

void main_window::on_destroy(){
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	vsite::nwp::application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
