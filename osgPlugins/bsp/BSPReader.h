/*
 *  BSPReader.h
 *  OpenSceneGraph
 *
 *  Created by Thomas Dickerson on 9/27/10.
 *  Copyright 2010 Vermont Sustainable Heating Initiative. All rights reserved.
 *
 */

#ifndef _BSP_READER_H_
#define _BSP_READER_H_

#include <osg/Array>
#include <osg/Node>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include <iostream>

namespace bsp
{
	
	class BSPReader{
	public:
		BSPReader();
		virtual ~BSPReader(){};
		virtual bool readFile(const std::string& file,
					  const osgDB::ReaderWriter::Options*);
		
		osg::ref_ptr<osg::Node>  getRootNode(){
			return root_node;
		};
		
	protected:
		
		osg::ref_ptr<osg::Node>   root_node;
	};
	
}

#endif