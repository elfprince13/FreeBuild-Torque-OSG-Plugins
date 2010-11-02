#include <osg/Node>
#include <osg/Notify>
#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

#include <osgDB/Registry>
#include <osgDB/FileNameUtils>



#include "BSPReader.h"
#include "VBSPReader.h"
#include "DIFReader.h"
#include "Q3BSPReader.h"

#include <iostream>

using namespace bsp;
using namespace osg;
using namespace osgDB;


// "VBSP" for Valve BSP files
const int VBSP_MAGIC_NUMBER  = (('P'<<24)+('S'<<16)+('B'<<8)+'V');

// "IBSP" for id (Quake 3) BSP files
const int IBSP_MAGIC_NUMBER = (('P'<<24)+('S'<<16)+('B'<<8)+'I');

namespace bsp{
	
	class ReaderWriterBSP : public osgDB::ReaderWriter
	{
	public:
		ReaderWriterBSP(){
			supportsExtension("dif", "Dynamix Interior File (Torque)");
			supportsExtension("bsp", "Quake and Valve Binary Space Partitions");
		}
		
		const char* className() const
		{
			// Return a description of this class
			return "BSP File Reader";
		}
		
		
		bool acceptsExtension(const std::string& extension) const
		{
			// If the extension is empty or "bsp", we accept it
			return osgDB::equalCaseInsensitive(extension, "bsp") || osgDB::equalCaseInsensitive(extension, "dif") || extension.empty();
		}
		
		
		ReaderWriter::ReadResult readNode(
										  const std::string& file,
										  const ReaderWriter::Options* options) const
		{
			BSPReader *               bspReader;
			ref_ptr<Node>              result;
			osgDB::ifstream            stream;
			int                        magicNumber;
			int                        version;
			std::string ext = osgDB::getFileExtension(file);
			
			// See if we handle this kind of file
			if (!acceptsExtension(ext))
				return ReadResult::FILE_NOT_HANDLED;
			
			// See if we can find the requested file
			std::string fileName = osgDB::findDataFile(file, options);
			if (fileName.empty()) 
				return ReadResult::FILE_NOT_FOUND;
			
			bspReader = NULL;
			if(osgDB::equalCaseInsensitive(ext, "dif")){
				//We have a Dynamix Interior File - Probably Torque Game Engine or similar
				bspReader = new DIFReader();
				
			} else if (osgDB::equalCaseInsensitive(ext, "bsp") || ext.empty()){
				
				
				// Open the file and read the magic number and version
				stream.open(fileName.c_str(), std::ios::binary);
				stream.read((char *) &magicNumber, sizeof(int));
				stream.read((char *) &version, sizeof(int));
				stream.close();
				
				// See which kind of BSP file this is
				if ((magicNumber == VBSP_MAGIC_NUMBER) &&
					(version >= 19) && (version <= 20))
				{
					// Read the Valve file
					bspReader = new VBSPReader();
				}
				else if ((magicNumber == IBSP_MAGIC_NUMBER) && (version == 0x2E))
				{
					// Read the Quake 3 file
					bspReader = new Q3BSPReader();
				}
				
			}
			if(bspReader){
				osg::setNotifyLevel(DEBUG_INFO);
				ReadResult retval = (bspReader->readFile(fileName, options)) ? ReadResult(bspReader->getRootNode().get()) : ReadResult::ERROR_IN_READING_FILE;
				delete bspReader; // I would think this is a memory leak, but it errors if I don't leave it there?
				osg::setNotifyLevel(NOTICE);
				return retval;
			} else{
				return ReadResult::FILE_NOT_HANDLED;
				
			}
			
		}
	};
}


REGISTER_OSGPLUGIN(bsp, ReaderWriterBSP)
