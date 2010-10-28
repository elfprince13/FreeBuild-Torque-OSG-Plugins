/*
 *  ReaderWriterLDR.cpp
 *  OpenSceneGraph
 *
 *  Created by Thomas Dickerson on 10/28/10.
 *  Copyright 2010 Vermont Sustainable Heating Initiative. All rights reserved.
 *
 */


#include <osg/Node>
#include <osg/Notify>
#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

#include <osgDB/Registry>
#include <osgDB/FileNameUtils>



#include "ReaderWriterLDR.h"

#include <iostream>

using namespace ldr;
using namespace osg;
using namespace osgDB;



ReaderWriterLDR::ReaderWriterLDR(){
	supportsExtension("dat", "LDraw Parts and Primitives");
	supportsExtension("ldr", "LDraw Model");
	supportsExtension("mpd", "LDraw Multi-Part Documents");
}

ReaderWriterLDR::~ReaderWriterLDR(){}



bool ReaderWriterLDR::acceptsExtension(const std::string& extension) const
{
	// If the extension is empty or "bsp", we accept it
	return osgDB::equalCaseInsensitive(extension, "dat") || osgDB::equalCaseInsensitive(extension, "ldr") || osgDB::equalCaseInsensitive(extension, "mpd");
}


ReaderWriter::ReadResult ReaderWriterLDR::readNode(
												   const std::string& file,
												   const ReaderWriter::Options* options) const
{
	ref_ptr<Node>              result;
	osgDB::ifstream            stream;
	std::string ext = osgDB::getFileExtension(file);
	
	// See if we handle this kind of file
	if (!acceptsExtension(ext))
		return ReadResult::FILE_NOT_HANDLED;
	
	// See if we can find the requested file
	std::string fileName = osgDB::findDataFile(file, options);
	if (fileName.empty()) 
		return ReadResult::FILE_NOT_FOUND;
	
	if(osgDB::equalCaseInsensitive(ext, "dat")){
		//We have a Dynamix Interior File - Probably Torque Game Engine or similar
		// Cache a primitive
		;
		
	} else {
		//handle mpd and ldr the same for now
	}
	
	ReadResult retval = ReadResult::ERROR_IN_READING_FILE;
	return retval;
	
}

REGISTER_OSGPLUGIN(ldr, ReaderWriterLDR)
