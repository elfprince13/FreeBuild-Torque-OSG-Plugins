/*
 *  LDraw.cc
 *  torque_xcode_2_2_UB
 *
 *  Created by Thomas Dickerson on 1/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ldraw.h"
//#include "core/resManager.h"

using namespace LDParse;


namespace LDParse{
	
	std::deque<std::string>* LDrawInstallation = NULL;
	
	S32 transform_stack_ptr = 0;
	S32 include_stack_ptr = 0;
	osg::Matrixf *current_transform[MAX_INCLUDE_DEPTH];
	
	CACHED_STREAM *cached_file_stack[MAX_INCLUDE_DEPTH];
	S32 cached_file_stack_index=0;  // points to unused entry
	LDLITE_PROFILE ldlite_profile;
	S32 current_type[MAX_INCLUDE_DEPTH];
	S32 deferred_flag[MAX_INCLUDE_DEPTH];
	
	
	std::deque<std::string> checkLDrawDirectory(std::string dirname){
		std::deque<std::string> ppmPaths;
		ppmPaths.clear();
		if( osgDB::fileType(dirname) == osgDB::DIRECTORY){
			std::string primsPath = osgDB::concatPaths(dirname, std::string("P"));
			std::string partsPath = osgDB::concatPaths(dirname, std::string("PARTS"));
			std::string modelPath = osgDB::concatPaths(dirname, std::string("MODELS"));
			if( osgDB::fileType(primsPath) == osgDB::DIRECTORY &&
				osgDB::fileType(partsPath) == osgDB::DIRECTORY && 
				osgDB::fileType(modelPath) == osgDB::DIRECTORY){
				ppmPaths.resize(3);
				ppmPaths[0] = primsPath;
				ppmPaths[1] = partsPath;
				ppmPaths[2] = modelPath;
			}
		}
		
		if(!ppmPaths.size())
			osg::notify(osg::WARN) << "Path '" << dirname << "' does not reference a valid LDraw installation" << std::endl;
		
		return ppmPaths;
	}
	
	char * bufferFile(std::string filename){
		
		osgDB::ifstream  stream;
		stream.open(filename.c_str(), std::ios::binary);
		if(!stream.good()){
			return NULL;
		}
		stream.seekg (0, std::ios::end);
		U32 fsize = stream.tellg();
		stream.seekg (0, std::ios::beg);
		
		char* script;

			script = new char [fsize+2];
			stream.read(script, fsize);
			stream.close();
			script[fsize+1] = 0;
			script[fsize] = 0;
		
		if (!fsize || !script)
		{
			delete [] script;
			osg::notify(osg::WARN) << "\tcouldn't buffer LDraw file " << filename << std::endl;
			return NULL;
		}
		return script;
	}
	
	
	std::string findLDrawFile(std::string filename/*, Vector<StringTableEntry> mpdSubfileList*/)
	{
		/*if(mpdSubfileList.size()){
			for(Vector<StringTableEntry>::iterator i = mpdSubfileList.begin(); i != mpdSubfileList.end(); i++){
				if(file == (*i)){
					return StringTable->insert(".mpd.");	//Flag value
				}
			}
		}*/
		
		//Assumes a beginning->end traversal to ensure proper precedence.
		if(!LDrawInstallation){
			osg::notify(osg::FATAL) << "findLDrawFile called before LDParse::LDrawInstallation was initialized" << std::endl;
			exit(-1);
		}
		return osgDB::findFileInPath(filename, *LDrawInstallation);
	}
	
	
};