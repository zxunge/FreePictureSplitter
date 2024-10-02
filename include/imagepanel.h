/* wxImagePanel -- A wxPanel which shows images. */

#ifndef WXIMAGEPANEL_H_
#define WXIMAGEPANEL_H_

#include <cstdint>
#include <wx/wx.h>

class wxImagePanel : public wxPanel
{
private:
	int canvw, canvh;
	float imgh, imgw, scale, aspRatio, imageOffsetX, imageOffsetY;
	int imagePosX, imagePosY;
	float mouseX, mouseY, panAmountX, panAmountY;
	bool isPanning, isLoading, showLoadingScreen;
	wxCursor panCursor;
	wxImage loadingScreen;
private:
	void onScroll(wxMouseEvent& evt);
	void scaleToRatio(int neww, int newh);
	void panHandler(wxMouseEvent& evt);
	void leftDown(wxMouseEvent& evt);
	void leftUp(wxMouseEvent& evt);
	void rightDown(wxMouseEvent& evt);
	void render(wxDC& dc);
	void renderLoading(wxDC& dc);
	void bgErase(wxEraseEvent& evt);
	void onSize(wxSizeEvent& evt);
	void initImageProps();
	void initBitmapProps(wxBitmap bitmap);
	void resetTransforms();
	wxBitmap RGBAtoBitmap(uint8_t* rgba, int w, int h);
	uint8_t* loadImageDataRGBA(const char* path, int *w, int *h, int* channels);

public:
	wxBitmap renderedBitmap;

	wxImagePanel(wxWindow* parent);

	void paintEvent(wxPaintEvent& evt);
	void paintNow();
	void loadBitmap(wxString path);
	void loadImage(wxString path);
	void loadImage(uint8_t* imageData, int x, int y, int channels);
	void loadImage(uint8_t* imageData, int x, int y);
	void loadImage(uint8_t* imageData, uint8_t* alphaData, int x, int y);
	void setLoading(bool isLoading);
	wxDECLARE_EVENT_TABLE();
};

#endif
