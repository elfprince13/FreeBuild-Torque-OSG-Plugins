/*
 *  ReaderWriterTER.h
 *  OpenSceneGraph
 *
 *  Created by Thomas Dickerson on 12/22/10.
 *  Copyright 2010 Vermont Sustainable Heating Initiative. All rights reserved.
 *
 */

#include <osg/Endian>

#define TER_VERSION 3
#define TER_BOUNDS 256
#define TER_AREA TER_BOUNDS*TER_BOUNDS

namespace ter{
	
	typedef unsigned int U32;
	typedef unsigned short U16;
	typedef unsigned long U64;
	typedef unsigned char U8;
	typedef int S32;
	typedef short S16;
	typedef long S64;
	typedef char S8;
	typedef float F32;
	typedef double F64;
	
	inline bool isLEndian(){	return osg::getCpuByteOrder()  == osg::LittleEndian;	}
	inline void convertLEndianToHost(U16 * i){ if(!isLEndian()) osg::swapBytes((S8*)i, sizeof(U16)); }
	inline void convertLEndianToHost(U32 * i){ if(!isLEndian()) osg::swapBytes((S8*)i, sizeof(U32)); }
	inline void convertLEndianToHost(U64 * i){ if(!isLEndian()) osg::swapBytes((S8*)i, sizeof(U64)); }
	inline void convertLEndianToHost(F32 * i){ if(!isLEndian()) osg::swapBytes((S8*)i, sizeof(F32)); }
	inline void convertLEndianToHost(F64 * i){ if(!isLEndian()) osg::swapBytes((S8*)i, sizeof(F64)); }
	inline void convertLEndianToHost(S16 * i){ convertLEndianToHost((U16*)i); }
	inline void convertLEndianToHost(S32 * i){ convertLEndianToHost((U32*)i); }
	inline void convertLEndianToHost(S64 * i){ convertLEndianToHost((U64*)i); }
	
	class ReaderWriterTER : public osgDB::ReaderWriter
	{
	public:
		ReaderWriterTER();
		~ReaderWriterTER();
		inline const char* className() const
		{
			// Return a description of this class
			return "TGE Terrain File Reader";
		}
		
		
		bool acceptsExtension(const std::string& extension) const;
		
		
		ReaderWriter::ReadResult readNode(
										  const std::string& file,
										  const ReaderWriter::Options* options) const;
	};
}