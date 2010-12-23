/*
 *  ReaderWriterLDR.cpp
 *  OpenSceneGraph
 *
 *  Created by Thomas Dickerson on 10/28/10.
 *  Copyright 2010 Thomas Dickerson. All rights reserved.
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


// We *will* want to implement this in FreeBuild
// http://www.openscenegraph.org/projects/osg/wiki/Support/Tutorials/VFS
ReaderWriterLDR::ReaderWriterLDR(){
	supportsExtension("dat", "LDraw Parts and Primitives");
	supportsExtension("ldr", "LDraw Model");
	supportsExtension("mpd", "LDraw Multi-Part Documents");
	//supportsOption("ldrDir", "The path to a valid LDraw installation");
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
	//const FilePathList fpl = options->getDatabasePathList();
	std::string dir = options->getOptionString();
	// See if we handle this kind of file
	if (!acceptsExtension(ext))
		return ReadResult::FILE_NOT_HANDLED;
	
	// See if we can find the requested file
	U32 i;
	std::deque<std::string> ppmPaths;
	//for(i = 0; i < fpl.size(); i++){
	//	std::string dir = fpl[i];
		ppmPaths = LDParse::checkLDrawDirectory(dir);
	//	if(ppmPaths.size() == 3){
	//		ldpath = dir;
	//		i++; // Flag that we did it
	//		break;
	//	}
	//}
	if (ppmPaths.size() != 3){
		osg::notify(osg::FATAL) << "A valid LDraw installation was not provided in the options to ReaderWriterLDR::readNode()" << std::endl;
		return ReadResult::FILE_NOT_FOUND;
	}
	LDParse::LDrawInstallation = new std::deque<std::string>;
	LDParse::LDrawInstallation->resize(3);
	for(i = 0; i < 3; i++){
		std::string tmp = ppmPaths[i];
		(*LDParse::LDrawInstallation)[i] = tmp;
	}
	ReadResult retVal = LDParse::initParse(file);
	// Okay, we're done. reset. This is NOT thread safe.
	// At all, but nothing to do with flex/bison is, so just be smart
	delete LDParse::LDrawInstallation;
	LDParse::LDrawInstallation = NULL;
	
	
	if(osgDB::equalCaseInsensitive(ext, "dat")){
		// Cache a primitive
		;
		
	} else {
		//handle mpd and ldr the same for now
		;
	}
	
	ReadResult retval = ReadResult::ERROR_IN_READING_FILE;
	return retval;
	
}

REGISTER_OSGPLUGIN(ldr, ReaderWriterLDR)
