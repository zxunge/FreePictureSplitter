/* wxImagePanel -- A wxPanel which shows images. */

#include "imagepanel.h"
#include "wx/timer.h"
#include <wx/rawbmp.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//TODO CONST
wxBEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
    EVT_PAINT(wxImagePanel::paintEvent)
    EVT_SIZE(wxImagePanel::onSize)
    EVT_MOUSEWHEEL(wxImagePanel::onScroll)
    EVT_MOTION(wxImagePanel::panHandler)
    EVT_LEFT_DOWN(wxImagePanel::leftDown)
    EVT_LEFT_UP(wxImagePanel::leftUp)
wxEND_EVENT_TABLE()

wxImagePanel::wxImagePanel(wxWindow* parent) :
	wxPanel(parent), canvw(0), canvh(0), imgh(0), imgw(0), scale(1), aspRatio(0), imageOffsetX(0), imageOffsetY(0),
	imagePosX(0), imagePosY(0), mouseX(0), mouseY(0), panAmountX(0), panAmountY(0), isPanning(false), isLoading(false), showLoadingScreen(false)
{
	//If you want to show a loading screen when loading images, set the showLoadingScreen to true
	// and load a valid image to show for the loading screen. Example is below.
	if (showLoadingScreen) {
		//loadingScreen.LoadFile("loadingScreen.png");
	}
	SetDoubleBuffered(true);
}

void wxImagePanel::loadBitmap(wxString path) {
	isLoading = true;
	Refresh();
	Update();
	int w, h, channels;
	uint8_t* data = loadImageDataRGBA(path.c_str(), &w, &h, &channels);
	wxBitmap bmp = RGBAtoBitmap(data, w, h);
	if (bmp.IsOk()) {
		renderedBitmap = bmp;
		resetTransforms();
		initBitmapProps(renderedBitmap);
		scaleToRatio(canvw, canvh);
		Refresh();
	}
}

//Load image using its path.
void wxImagePanel::loadImage(wxString path) {
	isLoading = true;
	Refresh();
	Update();
	wxImage image;
	image.LoadFile(path, wxBITMAP_TYPE_ANY);
	if (image.IsOk()) {
		renderedBitmap = (wxBitmap)image;
		resetTransforms();
		initBitmapProps(renderedBitmap);
		scaleToRatio(canvw, canvh);
		Refresh();
	}
}


//Load RGB/RGBA image using raw data. Converts to bitmap before render.
void wxImagePanel::loadImage(uint8_t* imageData, int x, int y, int channels) {
	isLoading = true;
	Refresh();
	Update();
	//Convert the raw data to bitmap first.
	wxBitmap bitmapImage = RGBAtoBitmap(imageData, x, y);
	if (bitmapImage.IsOk()) {
		renderedBitmap = bitmapImage;
		resetTransforms();
		initBitmapProps(bitmapImage);
		scaleToRatio(canvw, canvh);
		Refresh();
	}
}

//Load RGB image using raw data. Converts to wxImage then to bitmap.
void wxImagePanel::loadImage(uint8_t* imageData, int x, int y){
	isLoading = true;
	Refresh();
	Update();
	wxImage image = wxImage(x, y, imageData, true);
	if (image.IsOk()) {
		renderedBitmap = (wxBitmap)image;
		resetTransforms();
		initImageProps();
		scaleToRatio(canvw, canvh);
		Refresh();
	}
}

//Load RGBA image using raw data. Converts to wxImage then to bitmap.
void wxImagePanel::loadImage(uint8_t* imageData, uint8_t* imageAlpha, int x, int y){
	isLoading = true;
	Refresh();
	Update();
	wxImage image = wxImage(x, y, imageData, imageAlpha, true);
	if (image.IsOk()) {
		renderedBitmap = (wxBitmap)image;
		resetTransforms();
		initImageProps();
		scaleToRatio(canvw, canvh);
		Refresh();
	}
}

//Loads RGBA raw Data using STBI.
uint8_t* wxImagePanel::loadImageDataRGBA(const char* path, int* w, int* h, int* channels) {
	int channelsRequired = 4;
	uint8_t* imgData = stbi_load(path, w, h, channels, channelsRequired);
	if (imgData != NULL) {
		return imgData;
	}
	return NULL;
}

//Converts raw RGB/RGBA data to a bitmap.
wxBitmap wxImagePanel::RGBAtoBitmap(uint8_t* rgba, int w, int h)
{
	wxBitmap bitmap = wxBitmap(w, h, 32);
	if (!bitmap.Ok()) {
		//delete bitmap;
		return wxNullBitmap;
	}

	wxAlphaPixelData bmdata(bitmap);
	if (bmdata == false) {
		wxLogDebug(wxT("getBitmap() failed"));
		//delete bitmap;
		return wxNullBitmap;
	}

	wxAlphaPixelData::Iterator dst(bmdata);

	for (int y = 0; y < h; y++) {
		dst.MoveTo(bmdata, 0, y);
		for (int x = 0; x < w; x++) {
			// wxBitmap contains rgb values pre-multiplied with alpha
			unsigned char a = rgba[3];
			dst.Red() = rgba[0] * a / 255;
			dst.Green() = rgba[1] * a / 255;
			dst.Blue() = rgba[2] * a / 255;
			dst.Alpha() = a;
			dst++;
			rgba += 4;
		}
	}
	return bitmap;
}

void wxImagePanel::setLoading(bool isLoading){
	wxImagePanel::isLoading = isLoading;
}

void wxImagePanel::initImageProps(){
	imgw = renderedBitmap.GetWidth();
	imgh = renderedBitmap.GetHeight();
	aspRatio = imgw / imgh;
}

void wxImagePanel::initBitmapProps(wxBitmap bitmap) {
	imgw = bitmap.GetWidth();
	imgh = bitmap.GetHeight();
	aspRatio = imgw / imgh;
}

void wxImagePanel::resetTransforms() {
	scale = 1;
	imageOffsetX = 0;
	imageOffsetY = 0;
}

void wxImagePanel::scaleToRatio(int contw, int conth) {

	float newAspRatio = (float)contw / conth;
	float newImgw, newImgh;
	if (imgh < imgw && imgh > conth) {
		newImgh = contw / aspRatio;
		newImgw = contw;
		scale = newImgh / imgh;
	}
	else {
		newImgh = conth;
		newImgw = conth * aspRatio;
		scale = newImgh / imgh;
	}
}

//Does not render, but prepares to render.
void wxImagePanel::paintEvent(wxPaintEvent& evt) {
	wxPaintDC dc(this);
	dc.GetSize(&canvw, &canvh);
	//Calculates the images offset based on scale, centers it.
	imagePosX = (canvw / scale - imgw) / 2 + (panAmountX + imageOffsetX) / scale;
	imagePosY = (canvh / scale - imgh) / 2 + (panAmountY + imageOffsetY) / scale;

	if (renderedBitmap.IsOk()) {
		render(dc);
	}
	if (showLoadingScreen) {
		if (isLoading) {
			dc.Clear();
			renderLoading(dc);
			isLoading = false;
		}
	}
}

void wxImagePanel::paintNow() {
	wxClientDC dc(this);
	render(dc);
}

//The render function which does the actual rendering to the screen.
void wxImagePanel::render(wxDC& dc) {
	dc.SetUserScale(scale, scale);
	dc.DrawBitmap(renderedBitmap, imagePosX, imagePosY, false);
}

void wxImagePanel::renderLoading(wxDC& dc) {
	    dc.SetUserScale(1, 1);
		dc.DrawBitmap((wxBitmap)loadingScreen, (canvw - loadingScreen.GetWidth()) /2, (canvh -loadingScreen.GetHeight()) /2);
}

//Event Handlers
void wxImagePanel::onScroll(wxMouseEvent& evt){
	if (evt.GetWheelRotation() > 0) {
		if(scale < 10)
		   scale+=0.07f * scale;
	}
	else if(scale >= 0.1){
		scale-=0.07f * scale;
	}
	Refresh();
	evt.Skip();
}

void wxImagePanel::panHandler(wxMouseEvent& evt) {
	if (isPanning) {
		float newMouseX = evt.GetX();
		float newMouseY = evt.GetY();
		panAmountX = newMouseX - mouseX;
		panAmountY = newMouseY - mouseY;
		Refresh();
		evt.Skip();
	}
	evt.Skip();
}

void wxImagePanel::leftDown(wxMouseEvent& evt) {
	isPanning = true;
	mouseX = evt.GetX();
	mouseY = evt.GetY();
}

void wxImagePanel::leftUp(wxMouseEvent& evt) {
	isPanning = false;
	imageOffsetX += panAmountX;
	imageOffsetY += panAmountY;
	panAmountX = 0;
	panAmountY = 0;
	Refresh();
}

void wxImagePanel::rightDown(wxMouseEvent& evt) {
	wxPoint pixelLoc = evt.GetPosition();
	std::cout << pixelLoc.x << "\n" << pixelLoc.y << "\n";
}

void wxImagePanel::onSize(wxSizeEvent& evt) {
	Refresh();
	evt.Skip();
}

void wxImagePanel::bgErase(wxEraseEvent& evt) {

}

