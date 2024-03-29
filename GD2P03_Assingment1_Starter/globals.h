//
// globals.h
//
#pragma once

#include <SFML/Graphics.hpp>

#include "downloader.h"
#include "thread_pool.h"
#include "image_handler.h"

namespace Global
{
	extern CDownloader rDownloader;
	extern CThreadPool rPool;
	extern CImageHandler rImageHandler;

	bool CheckKeypress(const sf::Keyboard::Key& _eKey);
}