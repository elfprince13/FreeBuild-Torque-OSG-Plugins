/*
 *  ReaderWriterLDR.h
 *  OpenSceneGraph
 *
 *  Created by Thomas Dickerson on 10/28/10.
 *  Copyright 2010 Vermont Sustainable Heating Initiative. All rights reserved.
 *
 */

#include "ldraw.h"

namespace ldr{
	
	class ReaderWriterLDR : public osgDB::ReaderWriter
	{
	public:
		ReaderWriterLDR();
		~ReaderWriterLDR();
		inline const char* className() const
		{
			// Return a description of this class
			return "LDraw File Reader";
		}
		
		
		bool acceptsExtension(const std::string& extension) const;
		
		
		ReaderWriter::ReadResult readNode(
										  const std::string& file,
										  const ReaderWriter::Options* options) const;
	};
}