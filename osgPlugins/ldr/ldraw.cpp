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
	
	S32 transform_stack_ptr = 0;
	S32 include_stack_ptr = 0;
	osg::Matrixf *current_transform[MAX_INCLUDE_DEPTH];
	
	void checkLDrawDirectory(){
		bool error = false;
		std::cout << "Initializing LDraw Subsystem..." << std::endl;
		
		char * FullPath = dStrdup(Con::getVariable("$pref::ResourceManager::LDrawDirectory"));
		// Platform::isDirectory expects no trailing / so make sure we conform
		//if( FullPath[ dStrlen( FullPath ) - 1 ] == '/' )	//We don't need to worry about this, the console does it MAGIC for us.
		//	FullPath[ dStrlen( FullPath ) - 1 ] = 0x00;
		
		// A bad path!?  For shame...
		if( !Platform::isDirectory( FullPath )){
			error = true;
		}
		else if (!Platform::isSubDirectory(FullPath, "P") || !Platform::isSubDirectory(FullPath, "PARTS") || !Platform::isSubDirectory(FullPath, "MODELS")){
			error = true;
		}
		
		delete [] FullPath;
		
		if(error)
			std::cerr << "Please point us (see supported options) at a valid LDraw installation" << std::endl;
		
		return;// !error;
	}
	
	char * bufferFile(std::string filename){
		
		FileStream *s = new FileStream;
		char* script;
			if( !s->open (filename, FileStream::Read) )
			{
				delete s;
				return NULL;
			}
			U32 fsize = s->getStreamSize ();
			
		if(s)
		{
			script = new char [fsize+2];
			s->read(fsize, script);
			s->close();
			delete s;
			script[fsize+1] = 0;
			script[fsize] = 0;
		}
		
		if (!fsize || !script)
		{
			delete [] script;
			std::cerr << "\tcouldn't buffer LDraw file " << filename << std::endl;
			return NULL;
		}
		return script;
	}
	
	
	std::string findLDrawFile(std::string file/*, Vector<StringTableEntry> mpdSubfileList*/)
	{
		/*if(mpdSubfileList.size()){
			for(Vector<StringTableEntry>::iterator i = mpdSubfileList.begin(); i != mpdSubfileList.end(); i++){
				if(file == (*i)){
					return StringTable->insert(".mpd.");	//Flag value
				}
			}
		}*/
		
		std::string path = StringTable->insert(Con::getVariable("$pref::ResourceManager::LdrawDirectory"));
		std::vector<Platform::FileInfo> fileVector;
		
		// construct the full file path. we need this to get the file size and to recurse
		U32 len = strlen(path) + 8;
		char* pathbuf = new char[len];
		sprintf( pathbuf, "%s/P", path);
		pathbuf[len] = '\0';
		
		Platform::dumpPath( pathbuf, fileVector, 0 ); 

		sprintf(pathbuf, "%s/PARTS", path);
		Platform::dumpPath( pathbuf, fileVector, 3 ); 

		sprintf(pathbuf, "%s/MODELS", path);
		Platform::dumpPath( pathbuf, fileVector, 500 ); 
		
		delete [] pathbuf;
		
		for(std::vector<Platform::FileInfo>::iterator j =  fileVector.begin(); j != fileVector.end(); j++){
			if(osgDB::equalCaseInsensitive(j->pFileName, file)){
				char pReturn[1024]; 
				sprintf(pReturn, "%s/%s", j->pFullPath, j->pFileName);
				pReturn[strlen(pReturn)] = '\0';
				return std::string(pReturn);
			}
		}
		//const char* fullpath = buildPath(path, file);
		return NULL;
	}
	
	
};

/*
ConsoleFunctionGroupBegin(LDrawFuncs, "These are things from LDraw.cc");

ConsoleFunction(LDrawInit, void, 1, 1, "() Initializes the LDraw system, and checks for a valid LDraw directory")
{
	LDParse::checkLDrawDirectory();
}

ConsoleFunction(testLDrawFindFile, const char *, 2, /*3*//*2, "(filename [, fname2]) looks for a file"){
	Vector<StringTableEntry> tsubfiles(5);
	StringTableEntry ste = StringTable->insert(argv[1]);
	if (argc > 2){
		StringTableEntry ste2 = StringTable->insert(argv[2]);
		tsubfiles.push_back(ste2);
	}
	return LDParse::findLDrawFile(ste/*, tsubfiles*//*);
}

ConsoleFunction(testLDrawParseFile, S32, 2, 2, "(filename)"){
	return LDParse::initParse(argv[1]);
}

ConsoleFunctionGroupEnd(LDrawFuncs);
*/

