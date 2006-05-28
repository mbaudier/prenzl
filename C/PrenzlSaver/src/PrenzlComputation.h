#pragma once
#include "stdafx.h"
#include <fstream>

#include "PrenzlConfig.h"
#include "Model/Topology.h"
#include "Model/Rule.h"
#include "Model/Util.h"
#include "FileFinder.h"
#include "ImageLoader.h"

namespace Prenzl {

	class PrenzlComputation {
	public:

		PrenzlComputation(HWND hwnd)
			: timeCounter(0)
			, topology(0)
			, diplayBmp(0)
			, screenBMP(0)
		{
			// read the configuration in the registries
			config.readFromRegistry();

			// create the rule
			rule = config.createRule();

			// get the client rectangle
			GetClientRect(hwnd, &screen);

			// store the initial screen in screenBMP
			HDC screenHDC = GetDC(0);

			bmpHDC = CreateCompatibleDC(screenHDC);

			// create a transit HDC
			transitHDC = CreateCompatibleDC(screenHDC);

			screenBMP = CreateCompatibleBitmap(screenHDC, screen.right, screen.bottom);   
			// initialise it with the content of the screen
			SelectObject(bmpHDC, screenBMP);			
			BitBlt(bmpHDC, 0, 0, screen.right, screen.bottom, screenHDC, 0, 0, SRCCOPY);

			// TODO : should be releaseDC ?
			ReleaseDC(0, screenHDC);
//			DeleteDC(screenHDC);

			// set timer
			timer = (UINT)SetTimer(hwnd, 1, config.delay, NULL);

			// get all the pictures files if config.useFileAsInput is true 
			if(config.useFileAsInput) {
				addPicturesInDirectory(config.inputDirectory.c_str(), pictureFiles);
				if(pictureFiles.size() == 0) {
					// no file found, use screen input
					config.useFileAsInput = false;
				}
			}

			// initialise random generator
			srand((unsigned)time(0));

			// init the first computation
			initComputation();
		}

		~PrenzlComputation() {
			if(config.saveToClipBoard) {
				saveBmpToClipBoard();
			}

			DeleteDC(bmpHDC);
			DeleteDC(transitHDC);


			delete rule;
			delete topology; 
		}

		void paint(HDC hDC) {
			computeGeneration();
			drawGeneration(hDC);
			timeCounter++;

			// if a restart after a certain number of iterations is configured
			// look if we reached this number
			if(    config.restartActivated 
				&& config.nbGenBeforeRestart
				&& (timeCounter >= (unsigned int)config.nbGenBeforeRestart) ) 
			{
				FillRect(hDC, &screen, (HBRUSH)GetStockObject(BLACK_BRUSH));
				initComputation();
				timeCounter = 0;
			}
		}


	private:
		void computeGeneration() {
			topology->nextGeneration();
			rule->computeNext(*topology);
		}

		void initBITMAPINFO(int width, int height) {
			bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmInfo.bmiHeader.biWidth = width;
			bmInfo.bmiHeader.biHeight = height;
			bmInfo.bmiHeader.biPlanes = 1;
			bmInfo.bmiHeader.biBitCount = 24;
			bmInfo.bmiHeader.biCompression = BI_RGB;
			bmInfo.bmiHeader.biSizeImage = 0;
			bmInfo.bmiHeader.biXPelsPerMeter = 0;
			bmInfo.bmiHeader.biYPelsPerMeter = 0;
			bmInfo.bmiHeader.biClrUsed = 0;
			bmInfo.bmiHeader.biClrImportant = 0; 
		}

		void drawGeneration(HDC hDC)
		{
			int width = topology->getWidth();
			int height = topology->getHeight();

			// copy the topology content to the displayBmp
			SetDIBits(hDC, diplayBmp, 0, height ,(LPVOID)topology->getCurrent(),
				&(bmInfo), DIB_RGB_COLORS); 

			SelectObject(transitHDC, diplayBmp);			

			if(config.position == PrenzlConfig::STRETCH) {
				int displayWith = displayZone.right - displayZone.left;
				int displayHeight = displayZone.bottom - displayZone.top;
				StretchBlt(hDC, displayZone.left, displayZone.top, displayWith, displayHeight, 
						transitHDC, 0, 0, width, height, SRCCOPY); 
			}
			else {
				for(int x = displayZone.left; x < displayZone.right; x+= width) {
					for(int y = displayZone.top; y < displayZone.bottom; y += height) {
						BitBlt(hDC, x, y, width, height, transitHDC, 0, 0, SRCCOPY);
					}
				}
			}
		}

		HBITMAP findBitMapInPictureFiles() {
			unsigned int nbPictureFiles = (unsigned int) pictureFiles.size();

			// choose randomly a file among pictureFiles
			unsigned int randomIndex = rand() % nbPictureFiles;

			for(unsigned int i = 0; i < nbPictureFiles; i++) {
				unsigned int index = randomIndex + i;
				if(index >= nbPictureFiles) {
					index -= nbPictureFiles;
				}
				std::string pictureFile = pictureFiles.at(index);

				// try to load the file.
#if 0
				HBITMAP tmpBMP = (HBITMAP)::LoadImage(NULL, 
					pictureFile.c_str(),
					IMAGE_BITMAP,
					0, 0,
					LR_LOADFROMFILE |
					LR_CREATEDIBSECTION |
					LR_DEFAULTSIZE |
					LR_VGACOLOR);	
#else
				HBITMAP tmpBMP = LoadAnImage(pictureFile.c_str());
#endif

				// if we could, return the bitmap
				if(tmpBMP) {
					return tmpBMP;
				}
				// if not, go to the next one ...
			}
			// if no bmp could be read, return 0
			return 0;
		}

		void initComputation() {

			int width = 0;
			int height = 0;

			// get the width and the height
			if(config.useFileAsInput) {

				HBITMAP tmpBMP = findBitMapInPictureFiles();	

				if(tmpBMP) {
					if(screenBMP) {
						DeleteObject(screenBMP);
						screenBMP = 0;
					}
					screenBMP = tmpBMP;

					SelectObject(bmpHDC, screenBMP);			

					// get the size of the bitmap
					BITMAP bm;
					::GetObject( screenBMP, sizeof( bm ), &bm );
					width = bm.bmWidth;
					height = bm.bmHeight ;
				}
				else {
					// we could read absolutetly no bmp
					// switch to screen mode
					config.useFileAsInput = false;
				}
			}

			if(!config.useFileAsInput) {
				// we take initial screen as input
				// just apply the divisions
				width  = screen.right >> config.hdiv;
				height = screen.bottom >> config.vdiv;
			}

			width = saturate(width, 40, screen.right);
			height = saturate(height, 40, screen.bottom);

			// for an unknown reason, width must be a multiple of 4
			// this is probably related to BMP internal rules
			width &= 0xFFFFFFFC;

			// free diplayBmp if it was already defined
			if(diplayBmp) {
				DeleteObject(diplayBmp);
				diplayBmp = 0;
			}
			diplayBmp = CreateCompatibleBitmap(GetDC(NULL), width, height);    // Sets up the bitmap.

			initBITMAPINFO(width, height);

			// create the topology
			if(topology) {
				delete topology;
			}
			topology = new Topology(width, height);

			if(config.position == PrenzlConfig::CENTER) {
				displayZone.left = (screen.right - width)/2;
				displayZone.top = (screen.bottom - height)/2;
				displayZone.right = screen.right - displayZone.left;
				displayZone.bottom = screen.bottom - displayZone.top;
			}
			else if(config.position == PrenzlConfig::TILE) {
				displayZone = screen;
			}
			else if(config.position == PrenzlConfig::RANDOM) {
				// displayZone.left can be between 0 and screen.right-width
				int spectrumX = screen.right-width;
				displayZone.left = spectrumX ? (rand() % spectrumX) : 0; 

				// displayZone.top can be between 0 and screen.bottom-height
				int spectrumY = screen.bottom-height;
				displayZone.top = spectrumY ? (rand() % spectrumY) : 0; 
				displayZone.right = displayZone.left + width;
				displayZone.bottom = displayZone.top + height;
			}
			else if(config.position == PrenzlConfig::STRETCH) {
				// we stretch as much as we can without deforming the image
				// we center the stretched image

				float xZoom = (float) screen.right / (float) width;
				float yZoom = (float) screen.bottom / (float) height;

				// take the min zoom
				float zoom = (xZoom < yZoom) ? xZoom : yZoom;

				int displayWidth  = (int) (width * zoom);
				int displayHeight = (int) (height * zoom);

				// compute the left and top positions to center
				displayZone.left = (screen.right - displayWidth)/2;
				displayZone.top  = (screen.bottom - displayHeight)/2;

				// compute the right and bottom positions from the size of the display
				displayZone.right  = displayZone.left + displayWidth;
				displayZone.bottom = displayZone.top  + displayHeight;
			}

			SelectObject(transitHDC, diplayBmp);			

			if(config.useFileAsInput) {
				BitBlt(transitHDC,0,0,width, height,bmpHDC,0,0,SRCCOPY);
			}
			else {
				// copy in a bmp a portion of the screen bmp
				BitBlt(transitHDC,0,0,width, height,bmpHDC,displayZone.left,displayZone.top,SRCCOPY);
			}

			// copy the value of the bitmap in the topology (current and previous)
			GetDIBits(transitHDC, diplayBmp, 0, height, topology->getCurrent(), &bmInfo, DIB_RGB_COLORS);
			GetDIBits(transitHDC, diplayBmp, 0, height, topology->getPrevious(), &bmInfo, DIB_RGB_COLORS);

		}

		void saveBmpToClipBoard() {
			// put the bmp in the clipboard
			OpenClipboard(0);
			EmptyClipboard();
			SetClipboardData(CF_BITMAP, diplayBmp);
			CloseClipboard();
		}


	protected:
		//! Screen rectangle
		RECT screen;
		HDC bmpHDC;
		HBITMAP screenBMP;

		//! Display Rectangle
		RECT displayZone;

		//! timer handler
		UINT timer;

		//! The BitMap used to transfer the information from the topology to the HDC
		HBITMAP diplayBmp;

		HDC transitHDC;

		//! BITMAPINFO describing diplayBmp
		BITMAPINFO bmInfo;

		//! The configuration (stored in Windows Registers)
		PrenzlConfig config;

		//! The Topology
		Prenzl::Topology * topology;

		//! The Rule
		Prenzl::Rule * rule;

		//! Time (Generation) counter
		unsigned int timeCounter;

		//! list of all files that can be used as input 
		/*!
			Used only when config.useFileAsInput is true
		*/
		std::vector<std::string> pictureFiles;

	};

}