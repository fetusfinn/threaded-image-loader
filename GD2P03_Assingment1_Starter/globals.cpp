//
//
//
#include "globals.h"

//
// Global functions and variables that will be used
// all throughout the project
//
namespace Global
{
	// Our curl downloader
	CDownloader rDownloader;

	// Our thread pool
	CThreadPool rPool;

	// Image handler
	CImageHandler rImageHandler;

	//
	// Returns true if the given key was pressed this tick
	//
	bool CheckKeypress(const sf::Keyboard::Key& _eKey)
	{
		static bool bKeyPressed[sf::Keyboard::KeyCount];

		if (sf::Keyboard::isKeyPressed(_eKey))
		{
			if (!bKeyPressed[_eKey])
			{
				bKeyPressed[_eKey] = true;

				return true;
			}
		}
		else
		{
			if (bKeyPressed[_eKey])
				bKeyPressed[_eKey] = false;
		}

		return false;
	}
}