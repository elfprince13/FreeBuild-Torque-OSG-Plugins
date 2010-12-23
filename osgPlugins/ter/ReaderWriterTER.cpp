/*
 *  ReaderWriterTER.cpp
 *  OpenSceneGraph
 *
 *  Created by Thomas Dickerson on 12/22/10.
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

#include <osgTerrain/TerrainTile>
#include <osgTerrain/GeometryTechnique>
#include <osgTerrain/Layer>


#include "ReaderWriterTER.h"

#include <iostream>

using namespace ter;
using namespace osg;
using namespace osgDB;


// We *will* want to implement this in FreeBuild
// http://www.openscenegraph.org/projects/osg/wiki/Support/Tutorials/VFS
ReaderWriterTER::ReaderWriterTER(){
	supportsExtension("ter", "Torque Game Engine terrain file");
}

ReaderWriterTER::~ReaderWriterTER(){}



bool ReaderWriterTER::acceptsExtension(const std::string& extension) const
{
	// If the extension is empty or "bsp", we accept it
	return osgDB::equalCaseInsensitive(extension, "ter");
}


ReaderWriter::ReadResult ReaderWriterTER::readNode(
												   const std::string& file,
												   const ReaderWriter::Options* options) const
{
	ref_ptr<Node>              result;
	osgDB::ifstream            stream;
	std::string ext = osgDB::getFileExtension(file);
	//const FilePathList fpl = options->getDatabasePathList();
	std::string dir;
	if(options)
		dir = options->getOptionString();
	
	osg::ref_ptr<osg::Group> scene;
	osg::ref_ptr<osgTerrain::TerrainTile> terrainTile;
	
	// See if we handle this kind of file
	if (!acceptsExtension(ext))
		return ReadResult::FILE_NOT_HANDLED;

	// See if we can find the requested file
	std::string fileName = osgDB::findDataFile(file, options);
	if (fileName.empty()) 
		return ReadResult::FILE_NOT_FOUND;
	
	S8 terVers = 0;
	U16 heightMap[TER_AREA];
	
	stream.open(fileName.c_str(), std::ios::binary);
	stream.read((S8*) &terVers, sizeof(S8));
	
	// See which kind of BSP file this is
	if (terVers > TER_VERSION)
	{
		
		notify(FATAL) << "This terrain file is too recent for us!" << std::endl;
	} else if(terVers < TER_VERSION){
		notify(WARN) << "This terrain file is old! It should be opened and resaved in TGE 1.5 to bring it up to date..." << std::endl;
	}
	
	if(terVers <= TER_VERSION){
		stream.read((char *)heightMap, TER_AREA*sizeof(unsigned short));
		
		terrainTile = new osgTerrain::TerrainTile;
		osg::ref_ptr<osgTerrain::Locator> locator = new osgTerrain::Locator;
		osg::ref_ptr<osgTerrain::ValidDataOperator> validDataOperator = new osgTerrain::NoDataValue(0.0);
		osg::ref_ptr<osgTerrain::Layer> lastAppliedLayer;
		
		locator->setCoordinateSystemType(osgTerrain::Locator::PROJECTED);
		locator->setTransformAsExtents(0,0,TER_BOUNDS * 100,TER_BOUNDS * 100);//-osg::PI, -osg::PI*0.5, osg::PI, osg::PI*0.5);
		
		U32 i;
		
		std::string filterName;
		
		osg::Texture::FilterMode filter = osg::Texture::LINEAR;
		
		osg::ref_ptr<osg::HeightField> hf = new osg::HeightField;// = osgDB::readHeightFieldFile(filename);
		hf->allocate(TER_BOUNDS, TER_BOUNDS);
		for(i = 0; i < TER_AREA; i++){
			// Make this endian safe!
			convertLEndianToHost(&heightMap[i]);
			hf->setHeight(i / TER_BOUNDS, TER_BOUNDS - 1 - i%TER_BOUNDS, heightMap[i]);
		}
		
		osg::ref_ptr<osgTerrain::HeightFieldLayer> hfl = new osgTerrain::HeightFieldLayer;
		hfl->setHeightField(hf.get());
		
		hfl->setLocator(locator.get());
		hfl->setValidDataOperator(validDataOperator.get());
		hfl->setMagFilter(filter);
		
		
		terrainTile->setElevationLayer(hfl.get());
		
		lastAppliedLayer = hfl.get();
		
		/*
		 for (S32 j=0; j < (TerrainBlock::BlockSize * TerrainBlock::BlockSize); j++)
		 {
		 U8 val;
		 stream.read(&val);
		 
		 //
		 ret->mBaseMaterialMap[j] = val & TerrainFile::MATERIAL_GROUP_MASK;
		 materialMap[j].flags = val & TerrainBlock::Material::PersistMask;
		 }
		 
		 // read the MaterialList Info
		 S32 k, maxMaterials = TerrainBlock::MaterialGroups;
		 for(k=0; k < maxMaterials;)
		 {
		 ret->mMaterialFileName[k] = stream.readSTString(true);
		 if(ret->mMaterialFileName[k] && ret->mMaterialFileName[k][0])
         k++;
		 else
         maxMaterials--;
		 }
		 for(;k < TerrainBlock::MaterialGroups; k++)
		 ret->mMaterialFileName[k] = NULL;
		 
		 
		 if(version == 1)
		 {
		 for(S32 j = 0; j < (TerrainBlock::BlockSize * TerrainBlock::BlockSize); j++) {
         if (ret->mMaterialAlphaMap[ret->mBaseMaterialMap[j]] == NULL) {
		 ret->mMaterialAlphaMap[ret->mBaseMaterialMap[j]] = new U8[TerrainBlock::BlockSize * TerrainBlock::BlockSize];
		 dMemset(ret->mMaterialAlphaMap[ret->mBaseMaterialMap[j]], 0, TerrainBlock::BlockSize * TerrainBlock::BlockSize);
         }
		 
         ret->mMaterialAlphaMap[ret->mBaseMaterialMap[j]][j] = 255;
		 }
		 }
		 else
		 {
		 for(S32 k=0; k < TerrainBlock::MaterialGroups; k++) {
         if(ret->mMaterialFileName[k] && ret->mMaterialFileName[k][0]) {
		 AssertFatal(ret->mMaterialAlphaMap[k] == NULL, "Bad assumption.  There should be no alpha map at this point...");
		 ret->mMaterialAlphaMap[k] = new U8[TerrainBlock::BlockSize * TerrainBlock::BlockSize];
		 stream.read(TerrainBlock::BlockSize * TerrainBlock::BlockSize, ret->mMaterialAlphaMap[k]);
         }
		 }
		 }
		 if(version >= 3)
		 {
		 U32 len;
		 stream.read(&len);
		 ret->mTextureScript = (char *) dMalloc(len + 1);
		 stream.read(len, ret->mTextureScript);
		 ret->mTextureScript[len] = 0;
		 
		 stream.read(&len);
		 ret->mHeightfieldScript = (char *) dMalloc(len + 1);
		 stream.read(len, ret->mHeightfieldScript);
		 ret->mHeightfieldScript[len] = 0;
		 }
		 else
		 {
		 ret->mTextureScript = 0;
		 ret->mHeightfieldScript = 0;
		 }
		 
		 ret->buildGridMap();
		 return ret;
		 */
		 
		 /*
		 
		 osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filename);
		if (image.valid())
		{
			osg::ref_ptr<osgTerrain::ImageLayer> imageLayer = new osgTerrain::ImageLayer;
			imageLayer->setImage(image.get());
			imageLayer->setLocator(locator.get());
			imageLayer->setValidDataOperator(validDataOperator.get());
			imageLayer->setMagFilter(filter);
			
			if (offset!=0.0f || scale!=1.0f)
			{
				imageLayer->transform(offset,scale);
			}
			
			terrainTile->setElevationLayer(imageLayer.get());
			
			lastAppliedLayer = imageLayer.get();
			
			osg::notify(osg::NOTICE)<<"created Elevation osgTerrain::ImageLayer"<<std::endl;
		}
		else
		{
			osg::notify(osg::NOTICE)<<"failed to create osgTerrain::ImageLayer"<<std::endl;
		}
		
		scale = 1.0f;
		offset = 0.0f;
		 */
		
		/*
		readParameter = true;
		osg::notify(osg::NOTICE)<<"--image "<<filename<<" x="<<x<<" y="<<y<<" w="<<w<<" h="<<h<<std::endl;
		
		osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filename);
		if (image.valid())
		{
			osg::ref_ptr<osgTerrain::ImageLayer> imageLayer = new osgTerrain::ImageLayer;
			imageLayer->setImage(image.get());
			imageLayer->setLocator(locator.get());
			imageLayer->setValidDataOperator(validDataOperator.get());
			imageLayer->setMagFilter(filter);
			
			if (offset!=0.0f || scale!=1.0f)
			{
				imageLayer->transform(offset,scale);
			}
			
			terrainTile->setColorLayer(layerNum, imageLayer.get());
			
			lastAppliedLayer = imageLayer.get();
			
			osg::notify(osg::NOTICE)<<"created Color osgTerrain::ImageLayer"<<std::endl;
		}
		else
		{
			osg::notify(osg::NOTICE)<<"failed to create osgTerrain::ImageLayer"<<std::endl;
		}
		
		scale = 1.0f;
		offset = 0.0f;
		 */
		
		
		scene = new osg::Group;
		
		if (terrainTile.valid() && terrainTile->getElevationLayer())
		{
			scene->addChild(terrainTile.get());
			
			osg::ref_ptr<osgTerrain::GeometryTechnique> geometryTechnique = new osgTerrain::GeometryTechnique;
			terrainTile->setTerrainTechnique(geometryTechnique.get());
		}
		
	}
	
	
    
	stream.close();
	
	
	ReadResult retval = (terrainTile.valid() && terrainTile->getElevationLayer()) ? ReadResult(scene.get()) : ReadResult::ERROR_IN_READING_FILE;
	return retval;
	
}

REGISTER_OSGPLUGIN(ter, ReaderWriterTER)
