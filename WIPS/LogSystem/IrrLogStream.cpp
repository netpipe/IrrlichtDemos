/*!
	@file		IrrLogStream.cpp
	@author		manguonden
	@date		11/2009
	@module
*//*
	This file is part of Irrlicht Engine.
	
	Irrlicht Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	Irrlicht Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with Irrlicht Engine.  If not, see <http://www.gnu.org/licenses/>.
*/

//#define _HAS_EXCEPTIONS 0 //This is a test. I need fix this problem

//#include "IrrLogStream.h"
#include "IrrLogManager.h"
//#include <iomanip>
#include <time.h>

namespace irr
{

	LogStream::LogStream() {}
	LogStream::~LogStream()
	{
		if (mStream.is_open()) {
			mStream.close();
			release();
		}
	}

	LogStream::LogStream(const std::string& _file) : mFileName(_file)
	{
		lock();

		struct tm *current_time;
		time_t ctTime; time(&ctTime);
		current_time = localtime( &ctTime );

		mStream.open(mFileName.c_str(), std::ios_base::out);
		if (mStream.is_open()) {
			// create log header
			mStream << "============================================================================================================ " << std::endl;			
			mStream << ",@H ;@@@@A. :M@@@9s  X@2    3@,  :#@@i  ,@h  r@S:B@@@@@#    :@@@@@A.h@@r  @@:   &@@#,  &@r @@B: :@A s@@@@@2. " << std::endl;
			mStream << ",@@ r@@@@@@::@@@@@@i B@M    @@r s@@@@@A ;@@  X@H;@@@@@@@    r@@@@@@.@@@@  @@r :@@@@@@, @@5.@@@B r@@.2@@@@@M. " << std::endl;
			mStream << ",@@ :@2 ,@@; @@  H@@ 3@3    9@; @@s i@@ :@B  s@&   B@  .    :@H     B@@@  @@: &@A  #@i H@r @@@# ,@# r@X      " << std::endl;
			mStream << ",@@ ,@2  ;@; #@  ,@@ 5@9    X@: @@   @@,.@A  ;@&   #@       ,@A     B@2@; @@,,@@   ;@s 9@r @@3@ .@M ;@i      " << std::endl;
			mStream << ",@@ .@G  M@, @@  2@i 2@3    9@,.@@      .@#: i@G   #@       ,@B     M@.@M @@..@X       A@; @9.@s @M ;@3      " << std::endl;
			mStream << ",@@ .@@B@@:  @@##@;  2@3    9@.;@@      ,@@@@@@&   #@.      ,@@@@@r B@ #@ #@ .@3 .9HAr h@r @A @h @B ;@@@@@:  " << std::endl;
			mStream << ",@@ ,@@@@@; .@@@@@G  X@3    3@.i@@      ,@@9hB@A   @@.      ,@@@@@s B@ .@,A@ .@G ,@@@A h@r @# i#,@B ;@@@@@:  " << std::endl;
			mStream << ",@@ ,@9 :@@: @@  @@; 5@3    9@,.@@      ,@S  ,@A   @@.      ,@H. .  B@  @XX@..@A  .h@2 G@; @@ .@iAM ;@2  .   " << std::endl;
			mStream << ",@@ .@5  B@: #@  M@2 2@X    h@. #@   BH..@3  ;@A   #@.      ,@9     M@  @@A# .@@r  ;@2 3@; @@ .@@MH ;@i      " << std::endl;
			mStream << ",@@ .@h  A@, @@  5@@ S@H:;. 2@, #@i i@@ .@A  ;@H   @@.      :@M;,:, B@  i@@@. &@@r X@2 9@r @@  &@@H ;@A::;,  " << std::endl;
			mStream << ",@@ ,@M  3@; @@  r@@ 2@@@@@ S@; ,@@@@@9 ,@#  s@#   @@,      :@@@@@@ M@,  @@@: .@@@@@@3 M@s @@  ;@@M s@@@@@@. " << std::endl;
			mStream << ".#A .@X  r@: G#  ,Mh ;BBM#9 ;@,   B@@5  .@2  ;@X   3@.      .HBM##X X@.  &AA,  .#@##X. iB; #A   HH2 :A#MM#S  " << std::endl;
			mStream << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+ " << std::endl;
			mStream << "+                     Irrlicht Engine System - Event log                                                   + " << std::endl;
			mStream << "+                          (http://irrlicht.sourceforge.net/)                                              + " << std::endl;
			mStream << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+ " << std::endl;
			mStream << "                      Event loging report for : "
				<< std::setw(2) << std::setfill('0') << current_time->tm_mon + 1 << "/"
				<< std::setw(2) << std::setfill('0') << current_time->tm_mday << "/"
				<< std::setw(4) << std::setfill('0') << current_time->tm_year + 1900 << "   "
				<< std::setw(2) << std::setfill('0') << current_time->tm_hour << ":"
				<< std::setw(2) << std::setfill('0') << current_time->tm_min << ":"
				<< std::setw(2) << std::setfill('0') << current_time->tm_sec << std::endl;
			mStream << "============================================================================================================ " << std::endl << std::endl;
			mStream.close();
		}

		release();
	}

	void LogStream::start(const std::string& _section, const std::string& _level)
	{
		if (mStream.is_open()) {
			mStream.close();
			release();
		}

		lock();

		struct tm *current_time;
		time_t ctTime; time(&ctTime);
		current_time = localtime( &ctTime );

		if (false == mFileName.empty()) {
			mStream.open(mFileName.c_str(), std::ios_base::app);
			if (mStream.is_open()) {
				mStream << std::setw(2) << std::setfill('0') << current_time->tm_hour << ":"
					<< std::setw(2) << std::setfill('0') << current_time->tm_min << ":"
					<< std::setw(2) << std::setfill('0') << current_time->tm_sec << LogManager::separator
					<< _section << LogManager::separator << _level << LogManager::separator;
			}
		}
	}

	bool LogStream::getSTDOutputEnabled()
	{
		return LogManager::getSTDOutputEnabled();
	}

} // End of  irr namespace section
