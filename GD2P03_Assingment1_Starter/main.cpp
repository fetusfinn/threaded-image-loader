//
//	Threaded image downloader
//
#define NOMINMAX

#include <SFML/Graphics.hpp>

#include "globals.h"

// To keep track of all the tasks
int CTask::s_iTaskID = 0;

//
// Downloads the list of URLs
//
bool DownloadUrls(std::string& _strData, const std::string& _strUrl)
{
	if (Global::rDownloader.Download(_strUrl.c_str(), _strData))
	{
		std::cout << _strData << "\n";
		printf("data length: %zd\n", _strData.length());
		return true;
	}

	printf("Data failed to download!");

	return false;
}

//
// Called when a page is changed
//
void OnPageChange(int _iPage, const int _kiNumPages, short _iDir, sf::RenderWindow& _rWindow, sf::Sprite* _arrSprites)
{
	Global::rImageHandler.DownloadAndUpdate(_iPage, _kiNumPages, _iDir, _rWindow, _arrSprites);

	return;

	Global::rImageHandler.ChangePage(_iDir, _rWindow, _arrSprites);

	if (_iDir == 1)
	{
		// If we moved forward a page then download the images for the next page
		Global::rImageHandler.DownloadNextPage(_iPage, _kiNumPages);
	}
	else if (_iDir == 2)
	{
		// If we moved back a page then download the images for the previous page
		Global::rImageHandler.DownloadPreviousPage(_iPage, _kiNumPages);
	}

}

//
// Main
//
int main()
{
	sf::RenderWindow rWindow(sf::VideoMode(800, 600), "GD2P03 Assignment 1");

	// Used to download list of urls
	std::string strData = "";

	// Both of urls to download our image lists from 
	std::string arrUrls[2] = {
		"https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListSmall.txt",
		"https://raw.githubusercontent.com/MDS-HugoA/TechLev/main/ImgListLarge.txt"
	};

	// Try download the urls
	if (!DownloadUrls(strData, arrUrls[0]))
	{
		// Failed to download
		std::cout << "Failed to download URLs!\n";
		return 1;
	}

	// Split our downloaded urls
	// TODO : use threads
	Global::rImageHandler.SplitUrls(strData);

	// What page of images we're on
	// 4 images to a page
	int iPage = 0;

	// Total number of pages there should be
	const int kiMaxPages = std::ceil(Global::rImageHandler.GetUrls().size() / 4.f);

	// Have to create this in main() otherwise it wont draw, unsure why
	sf::Sprite arrSprites[4];

	// Set position, this will never change so we only
	// need to set it once in the beginning
	for (int i = 0; i < 4; i++)
	{
		arrSprites[i].setPosition(0 + (((i + 1) % 2 == 0) ? (rWindow.getSize().x / 2.f) : 0), 0 + ((i >= 2) ? (rWindow.getSize().y / 2.f) : 0));
	}



	// Download all images for the first page and update our sprites with them
	Global::rImageHandler.DownloadAndUpdate(iPage, kiMaxPages, 0, rWindow, arrSprites);

	// TODO v broken

	// Download all the images for the second page
	// Global::rImageHandler.DownloadNextPage(iPage, kiMaxPages);

	// Download all the images for the last page (previous)
	// Global::rImageHandler.DownloadPreviousPage(iPage, kiMaxPages);





	// Main loop
	while (rWindow.isOpen())
	{
		// Event handling
		sf::Event rEvent;
		while (rWindow.pollEvent(rEvent))
		{
			switch (rEvent.type)
			{
			case sf::Event::Closed:
				rWindow.close();
				break;

			default:
				break;
			}
		}

		// Keypress stuff
		  {
			if (Global::CheckKeypress(sf::Keyboard::Right))
			{
				iPage++;

				// Rollover to the beginning
				if (iPage >= kiMaxPages)
					iPage = 0;

				std::cout << "Page changed : " << iPage << "\n";

				// Update images
				OnPageChange(iPage, kiMaxPages, 1, rWindow, arrSprites);
			}

			if (Global::CheckKeypress(sf::Keyboard::Left))
			{
				iPage--;

				// Roll back over to the end
				if (iPage < 0)
					iPage = kiMaxPages;

				std::cout << "Page changed : " << iPage << "\n";

				// Update images
				OnPageChange(iPage, kiMaxPages, 2, rWindow, arrSprites);

			}
		}


		// Drawing
		rWindow.clear();
		{
			Global::rImageHandler.Draw(rWindow, arrSprites);
		}
		rWindow.display();
	}

	curl_global_cleanup();

	return 0;
}