/*
 *  DIFReader.cpp
 *  OpenSceneGraph
 *
 *  Created by Thomas Dickerson on 9/28/10.
 *
 */


#include <osg/TexEnv>
#include <osg/CullFace>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Image>
#include <osg/Texture2D>
#include <osg/PolygonMode>
#include <osg/BlendColor>
#include <osg/Endian>
#include <osg/LOD>
#include <osg/BlendFunc>
#include <osg/TexEnvCombine>
#include <osg/TexEnv>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/ReaderWriter>
#include <osgDB/PluginQuery>

#include <assert.h>
#include <fstream>

#include <string.h>
#include <iostream>

#include "BSPReader.h"
#include "DIFReader.h"

using namespace bsp;
using namespace osg;
using namespace osgDB;

DIFReader::DIFReader()
{
	//QT Plugin Implementation does a bad job reading png's from stream
	//osgDB::Registry::instance()->loadLibrary("osdb_png");
	//reader = osgDB::Registry::instance()->getReaderWriterForExtension("png");
	reader = osgDB::Registry::instance()->getReaderWriterForExtension("istreampng");
    root_node = NULL;
	dif_data = new DIFInteriorFile();
}

DIFReader::~DIFReader()
{
	// Nothing to do here yet
}


void DIFReader::buildInteriorResource(){
	osg::ref_ptr<osg::Group> nroot = new osg::Group();
	osg::LOD * lodControl = new osg::LOD();
	lodControl->setCenterMode(osg::LOD::USE_BOUNDING_SPHERE_CENTER);
	lodControl->setRangeMode(osg::LOD::PIXEL_SIZE_ON_SCREEN);
	U32 i, j;
	F32 min, max, ourMin;
	for(i = 0; i < dif_data->lod_list.size(); i++){
		ourMin = dif_data->lod_list[i]->minPixels;
		osg::notify(DEBUG_INFO) << "lodControl has " << lodControl->getNumChildren() << " children" << std::endl;
		for(j = 0; j<lodControl->getNumChildren(); j++){
			min = lodControl->getMinRange(i);
			max = lodControl->getMaxRange(i);
			if( min < ourMin && ourMin < max){
				lodControl->setRange(i, min, ourMin);
				min = ourMin;
				break;
			}
		}
		if(!j){
			osg::notify(DEBUG_INFO) << "\tInsert first one!" << std::endl;
			min = ourMin;
			max = F32_MAX;
		}
		osg::notify(DEBUG_INFO) << "\tsetting min,max: " << min << "," << max << std::endl;
		lodControl->addChild(dif_data->lod_list[i]->buildInteriorNode(), min, max);
	}
	
	// This lot probably don't have a maximum display size, just a minimum,
	// so let's go with F32_MAX all the way around
	osg::notify(DEBUG_INFO) << "Adding " << dif_data->subs_list.size() << " sub-objects to lodControl" << std::endl;
	for(i = 0; i < dif_data->subs_list.size(); i++)
		lodControl->addChild(dif_data->subs_list[i]->buildInteriorNode(), dif_data->subs_list[i]->minPixels, F32_MAX);
	
	
	nroot->addChild(lodControl);
	root_node = nroot;
}

osg::Node *  DIFInteriorObj::buildInteriorNode(){
	U32 i,j,k,l;
	U32 numMats, numLights, matMul, lightMul, minML;
	numMats = materialList.materialNames.size();
	numLights = lightmaps.size();
	minML = numMats < numLights ? numMats : numLights;
	// numLights in rows
	// numMats in columns
	// i.e. index = mn * nl + ln
	if(minML == numMats){
		matMul = 1;
		lightMul = numMats;
	} else{
		lightMul = 1;
		matMul = numLights;
	}
	osg::Group* intnode_root = new osg::Group();
	osg::StateSet* difRSS = new osg::StateSet();
	difRSS->setMode(GL_BLEND, osg::StateAttribute::ON);
	osg::BlendFunc* fn = new osg::BlendFunc(osg::BlendFunc::ONE, osg::BlendFunc::ZERO);
	difRSS->setAttributeAndModes(fn, osg::StateAttribute::ON);
	
	
	osg::TexEnvCombine* blendTexEnv = new osg::TexEnvCombine();
	blendTexEnv->setCombine_RGB (osg::TexEnvCombine::MODULATE);
	blendTexEnv->setSource0_RGB (osg::TexEnvCombine::TEXTURE);
	blendTexEnv->setOperand0_RGB (osg::TexEnvCombine::SRC_COLOR);
	blendTexEnv->setSource1_RGB (osg::TexEnvCombine::PREVIOUS);
	blendTexEnv->setOperand1_RGB (osg::TexEnvCombine::SRC_COLOR);
	blendTexEnv->setScale_RGB (LIGHTING_OVERBRIGHT_AMOUNT);
	blendTexEnv->setCombine_Alpha (osg::TexEnvCombine::MODULATE);
	blendTexEnv->setSource0_Alpha (osg::TexEnvCombine::TEXTURE);
	blendTexEnv->setOperand0_Alpha (osg::TexEnvCombine::SRC_ALPHA);
	blendTexEnv->setSource1_Alpha (osg::TexEnvCombine::PREVIOUS);
	blendTexEnv->setOperand1_Alpha (osg::TexEnvCombine::SRC_ALPHA);
	blendTexEnv->setScale_Alpha (LIGHTING_OVERBRIGHT_AMOUNT);
	//difRSS->setTextureAttribute(1,blendTexEnv,osg::StateAttribute::ON); // We don't have lightmaps yet, so leave this off
	intnode_root->setStateSet(difRSS);
		
	//osg::TexEnv* lmEnv = new osg::TexEnv(osg::TexEnv::REPLACE);
	osg::TexEnv* dfEnv = new osg::TexEnv(osg::TexEnv::MODULATE);
	
	osg::Vec3Array* verts = new osg::Vec3Array();
	osg::Vec3Array* norms = new osg::Vec3Array();
	osg::Vec2Array* dfUVs = new osg::Vec2Array();
	osg::Vec2Array* lmUVs = new osg::Vec2Array();
	//std::vector<U16> indices;
	
	verts->clear();
	norms->clear();
	dfUVs->clear();
	lmUVs->clear();
	//indices.clear();
	
	i = numMats * numLights;
	osg::notify(DEBUG_INFO) << "Allocating vector holding pen for " << i << " = ("<< numMats << "*" << numLights <<") materials" << std::endl;
	std::vector<osg::Geometry*> gPerTex(i,NULL);
	std::vector<std::map<U32,U32>* > mPerTex(i,NULL); // make sure we delete this vector afterwards
	std::vector<std::vector<U32>* > iPerTex(i,NULL); // make sure we delete this vector afterwards
	
	for(i = 0; i < zones.size(); i++){
		for(j = 0; j < iPerTex.size(); j++){
			iPerTex[j] = NULL;
			mPerTex[j] = NULL;
			gPerTex[j] = NULL;
		}
		osg::Geode* zoneI = new osg::Geode();
		osg::Geometry* zoneIG = NULL;
		std::vector<U32>* localIndices = NULL;	
		std::map<U32,U32>* vmapL2G = NULL;
		
		Zone z = zones[i];
		U8 flag;
		U16 texIndex;
		U32 normLMIndex;
		for(j = z.surfaceStart; j < z.surfaceStart+z.surfaceCount; j++){
			U32 surfaceIndex =zoneSurfaces[j];
			
			Surface s = surfaces[surfaceIndex];
			normLMIndex = normalLMapIndices[surfaceIndex];
			//k = (normLMIndex * lightMul) + (s.textureIndex * matMul);
			k = normLMIndex + s.textureIndex * numLights;
			// If this is the first pass, or we are changing textures, switch out zoneIG
			flag = (texIndex != k) || (j == z.surfaceStart);
			texIndex = k;
			if(flag) // First pass
			{	
				if(iPerTex[texIndex] != NULL){
					zoneIG = gPerTex[texIndex];
					localIndices = iPerTex[texIndex];
					vmapL2G = mPerTex[texIndex];
				} else{
					zoneIG = new osg::Geometry();
					zoneIG->setUseVertexBufferObjects(true);
					localIndices = new std::vector<U32>;
					vmapL2G = new std::map<U32,U32>;
					vmapL2G->clear();
					iPerTex[texIndex] = localIndices;
					mPerTex[texIndex] = vmapL2G;
					gPerTex[texIndex] = zoneIG;	// Geometry/material
				}
			}
			std::vector<osg::Vec3f> lNorms(s.windingCount);
			
			U32 startIndex = localIndices->size(); // this will come up in a moment
			for(k = 2; k < s.windingCount; k++){
				localIndices->push_back(windings[s.windingStart+k-2]);
				lNorms.push_back(getPointNormal(surfaceIndex, k-2));
				localIndices->push_back(windings[s.windingStart+k-1]);
				lNorms.push_back(getPointNormal(surfaceIndex, k-1));
				localIndices->push_back(windings[s.windingStart+k]);
				lNorms.push_back(getPointNormal(surfaceIndex, k));
				if(++k == s.windingCount) break;
				localIndices->push_back(windings[s.windingStart+k-1]);
				lNorms.push_back(getPointNormal(surfaceIndex, k-1));
				localIndices->push_back(windings[s.windingStart+k-2]);
				lNorms.push_back(getPointNormal(surfaceIndex, k-2));
				localIndices->push_back(windings[s.windingStart+k]);
				lNorms.push_back(getPointNormal(surfaceIndex, k));
			}
			
			for(k = 0; k < localIndices->size()-startIndex; k++){
				std::map<U32,U32>::iterator it = vmapL2G->find((*localIndices)[startIndex+k]);
				bool alreadyStored = it != vmapL2G->end();
				if(alreadyStored){
					(*localIndices)[startIndex+k] = it->second;
				} else{
					// Interleave this some day with a custom drawable -- http://forum.openscenegraph.org/viewtopic.php?t=1058
					// Especially given low-end specs
					l = (*localIndices)[startIndex+k];
					(*localIndices)[startIndex+k] = verts->size();
					osg::Vec3f spacePT = pointList[l].point;
					verts->push_back(spacePT);
					norms->push_back(lNorms[k]);
					// fill in tex coords :)
					DTexGen tg = texGenEQs[s.texGenIndex];
					osg::Vec4f xPlane = tg.xPlane;
					osg::Vec4f yPlane = tg.yPlane;
					osg::Vec2f uvPT(xPlane.x()*spacePT.x()+
									xPlane.y()*spacePT.y()+
									xPlane.z()*spacePT.z()+
									xPlane.w(),
									yPlane.x()*spacePT.x()+
									yPlane.y()*spacePT.y()+
									yPlane.z()*spacePT.z()+
									yPlane.w());
					dfUVs->push_back(uvPT);
					//....if we ever set up proper lightmaps this next bit may need to be changed a bit.
					tg = lmTexGenEQs[surfaceIndex];
					xPlane = tg.xPlane;
					yPlane = tg.yPlane;
					uvPT.set(xPlane.x()*spacePT.x()+
							 xPlane.y()*spacePT.y()+
							 xPlane.z()*spacePT.z()+
							 xPlane.w(),
							 yPlane.x()*spacePT.x()+
							 yPlane.y()*spacePT.y()+
							 yPlane.z()*spacePT.z()+
							 yPlane.w());
					lmUVs->push_back(uvPT);
					// okay, moving on
					(*vmapL2G)[l] = (*localIndices)[startIndex+k];
				}
			}
		}
		verts->dirty();
		norms->dirty();
		dfUVs->dirty();
		lmUVs->dirty();
		
		
		for(j = 0; j < iPerTex.size(); j++){
			if(iPerTex[j] != NULL){
				osg::StateSet* geoState = new osg::StateSet();
				
				zoneIG = gPerTex[j];
				localIndices = iPerTex[j];
				delete mPerTex[j];
				
				
				texIndex = j / numLights;
				normLMIndex = j % numLights;
				
				// Convert indices to DrawElementsUInt
				DrawElementsUInt* indices = new DrawElementsUInt(osg::PrimitiveSet::TRIANGLES);
				indices->reserve(localIndices->size());
				for(k = 0; k < localIndices->size(); k++){
					indices->push_back((*localIndices)[k]);
				}
				delete localIndices;
				iPerTex[j] = NULL;
				
				zoneIG->setVertexArray(verts);
				zoneIG->setNormalArray(norms);
				zoneIG->setTexCoordArray(1, dfUVs);
				zoneIG->setTexCoordArray(0, lmUVs);
				zoneIG->addPrimitiveSet(indices);
				// Set texture state! Go-go-go!
				osg::ref_ptr<osg::Texture2D> dfTex;
				if(!materialList.materials[texIndex].valid()){
					dfTex = new Texture2D;
					osg::Image* dfImg = NULL;
					char * name = materialList.materialNames[texIndex];
					std::string texFile = difFindTexture(*filename, name);
					if(texFile.size()){
						dfImg = osgDB::readImageFile(texFile);
					}
					if(!dfImg){
						osg::notify(FATAL) << "unable to load texture " << texFile << std::endl;
						//return NULL;
						exit(-1);
					}
					dfImg->flipVertical();
					dfTex->setImage(dfImg);
					dfTex->setDataVariance(osg::Object::STATIC);
					materialList.materials[texIndex] = dfTex;
					
				} else{
					dfTex = materialList.materials[texIndex];
				}
				
				osg::Texture2D* lmTexture = new osg::Texture2D;
				//We'll need two of these eventually
				lmTexture->setImage(lightmaps[normLMIndex].get());
				lmTexture->setDataVariance(osg::Object::STATIC);
				//geoState->setTextureAttribute(0, lmEnv, StateAttribute::ON);
				//geoState->setTextureAttributeAndModes(0, dfTex.get(),StateAttribute::ON);//lmTexture,StateAttribute::ON);
				geoState->setTextureAttribute(1, dfEnv, StateAttribute::ON);
				geoState->setTextureAttributeAndModes(1, dfTex.get(),StateAttribute::ON);
				
				
				zoneIG->setStateSet(geoState);
				zoneI->addDrawable(zoneIG);
			}
		}
		intnode_root->addChild(zoneI);
		
		
	}
	//Then embedded meshes
	//We'll need transform + scale state for these.
	
	// Unfortunately, zoning is untrustworth here we should
	// consider a bounding-box implementation of some sort
	// to place meshes in the zone groups. But...
	// don't worry about it for now
	
	std::vector<U16>** mlsIndices;
	std::map<U16,U16>** vmaps;
	osg::Geometry** mlGeo;
	U32 m,n;
	osg::Geode* meshI;
	for(i = 0; i < csMeshes.size(); i++){
		
		verts = new osg::Vec3Array();
		norms = new osg::Vec3Array();
		dfUVs = new osg::Vec2Array();
		lmUVs = new osg::Vec2Array();
		
		verts->clear();
		norms->clear();
		dfUVs->clear();
		lmUVs->clear();
		
		CSMesh* csm = &csMeshes[i];
		
		osg::Matrixf trans = csm->transform;
		trans.scale(csm->scale);
		F32* rows = trans.ptr();
		osg::Vec3f row0(rows[0],rows[1],rows[2]);
		osg::Vec3f row1(rows[4],rows[5],rows[6]);
		osg::Vec3f row2(rows[8],rows[9],rows[10]);
		F32 det =	row0.x() * (row1.y() * row2.z() - row1.z() * row2.y()) -
		row0.y() * (row1.x() * row2.z() - row1.z() * row2.x()) +
		row0.z() * (row1.x() * row2.y() - row1.y() * row2.x());
		U8 flipped = (det < 0.0f) & 0x01; // Until we figure out what's going on with transforms, best to ignore this.
		
		
		meshI = new osg::Geode();
		n = csm->csMaterialList.materials.materials.size();
		mlsIndices = new std::vector<U16>*[n*numLights];// Unlike with interior surfaces
		vmaps = new std::map<U16,U16>*[n*numLights];
		mlGeo = new osg::Geometry*[n*numLights];		 // all of the textures should be
		U32 texIndex;							 // in one place.
		
		for(j = 0; j < n*numLights; j++){
			mlsIndices[j] = NULL;
			vmaps[j] = NULL;
			mlGeo[j] = NULL;
		}
		
		bool flag;
		osg::Geometry* curGeo = NULL;
		std::vector<U16>* curIndices = NULL;
		std::map<U16,U16>* vmapL2G = NULL;
		osg::notify(DEBUG_INFO) << "mesh " << i << " has " << csm->primitives.size() << " primitives" << std::endl;
		for(j = 0; j < csm->primitives.size(); j++){
			CSMPrimitive * primitive = &(csm->primitives[j]);
			m = primitive->diffuseIndex * numLights + primitive->lightMapIndex;
			flag = (j == 0) || (texIndex != m);
			texIndex = m;
			if(flag){
				if(mlsIndices[texIndex] != NULL){
					curGeo = mlGeo[texIndex];
					curIndices = mlsIndices[texIndex];
					vmapL2G = vmaps[texIndex];
				} else{
					curGeo = new osg::Geometry();
					curGeo->setUseVertexBufferObjects(true);
					curIndices = new std::vector<U16>;
					vmapL2G = new std::map<U16,U16>;
					mlsIndices[texIndex] = curIndices;
					//mldIndices[texIndex] = dstIndices;
					mlGeo[texIndex] = curGeo;	// Geometry/material
					vmaps[texIndex] = vmapL2G;
					vmapL2G->clear();
				}
		//		if(i > 1) osg::notify(DEBUG_INFO) << "\tcurIndices has index " << texIndex << " and size " << curIndices->size() << std::endl;
			} else{
		//		if(i > 1) osg::notify(DEBUG_INFO) << "\ttexIndex didn't change" << std::endl;
			}
			
			U32 startIndex = curIndices->size();
			U8 direction;
			for(k = 2; k < primitive->count; k++){
				direction = U8(k & 0x01) ^ flipped;
				if(direction){
					curIndices->push_back(csm->indices[primitive->start + k - 1]);
					curIndices->push_back(csm->indices[primitive->start + k - 2]);
					curIndices->push_back(csm->indices[primitive->start + k]);
				} else{
					curIndices->push_back(csm->indices[primitive->start + k - 2]);
					curIndices->push_back(csm->indices[primitive->start + k - 1]);
					curIndices->push_back(csm->indices[primitive->start + k]);
				}

			}
			
			//if(i > 1) osg::notify(DEBUG_INFO) <<"\tnow curIndices has size " << curIndices->size() << std::endl;
			
			//U32 startVert = verts->size();
			for(k = 0; k < curIndices->size()-startIndex; k++){
				std::map<U16,U16>::iterator it = vmapL2G->find((*curIndices)[startIndex+k]);
				bool alreadyStored = it != vmapL2G->end();
				if(alreadyStored){
					(*curIndices)[startIndex+k] = it->second;
				} else{
					// Interleave this some day with a custom drawable -- http://forum.openscenegraph.org/viewtopic.php?t=1058
					// Especially given low-end specs
					l = (*curIndices)[startIndex+k];
					(*curIndices)[startIndex+k] = verts->size();
					if(l >= csm->verts.size()) osg::notify(osg::DEBUG_INFO) << "OH STINK" << std::endl;
					verts->push_back(csm->verts[l]);
					norms->push_back(csm->norms[l]);
					// fill in tex coords :)
					dfUVs->push_back(csm->diffuseUVs[l]);
					lmUVs->push_back(csm->lightmapUVs[l]);
					// okay, moving on
					(*vmapL2G)[l] = (*curIndices)[startIndex+k];
				}
			}
		}
		verts->dirty();
		norms->dirty();
		dfUVs->dirty();
		lmUVs->dirty();
		
		
		for(j = 0; j < n*numLights; j++){
			if(mlsIndices[j] != NULL){
				osg::StateSet* geoState = new osg::StateSet();
				curGeo = mlGeo[j];
				curIndices = mlsIndices[j];
				delete vmaps[j];
				
				texIndex = j / numLights;	// Diffuse texture index
				m = j % numLights;			// Light texture index
				
				// Convert indices to DrawElementsUInt
				DrawElementsUShort* indices = new DrawElementsUShort(osg::PrimitiveSet::TRIANGLES);
				indices->reserve(curIndices->size());
				for(k = 0; k < curIndices->size(); k++){
					indices->push_back((*curIndices)[k]);
				}
				delete curIndices;
				mlsIndices[j] = NULL;
				
				curGeo->setVertexArray(verts);
				curGeo->setNormalArray(norms);
				curGeo->setTexCoordArray(1, dfUVs);
				curGeo->setTexCoordArray(0, lmUVs);
				curGeo->addPrimitiveSet(indices);
				// Set texture state! Go-go-go!
				osg::ref_ptr<osg::Texture2D> dfTex;
				if(!csm->csMaterialList.materials.materials[texIndex].valid()){
					dfTex = new Texture2D;
					osg::Image* dfImg = NULL;
					std::string texFile;
					if(csm->embedFlags[texIndex]){
						osg::notify(DEBUG_INFO) << "embedded texture " << texIndex << " for CSMesh " << i << std::endl;
						dfImg = csm->embeddedMats[texIndex].get();
						texFile = std::string("EMBEDDED");
					} else{
						char * name = csm->csMaterialList.materials.materialNames[texIndex];
						texFile = difFindTexture(*filename, name);
						if(texFile.size()){
							osg::notify(DEBUG_INFO) << "reading " << texFile << " for texture " << texIndex << " in mesh " << i << std::endl;
							dfImg = osgDB::readImageFile(texFile);
						}	
					}
					
					if(!dfImg){
						osg::notify(FATAL) << "unable to load texture " << texFile << std::endl;
						exit(-1);
					}
					dfImg->flipVertical();
					dfTex->setImage(dfImg);
					dfTex->setDataVariance(osg::Object::STATIC);
					csm->csMaterialList.materials.materials[texIndex] = dfTex;
					
				} else{
					osg::notify(DEBUG_INFO) << "loading cached material " << csm->csMaterialList.materials.materialNames[texIndex] << " for texture " << texIndex << " in mesh " << i << std::endl;
					dfTex = csm->csMaterialList.materials.materials[texIndex];
				}
				
				//osg::Texture2D* lmTexture = new osg::Texture2D;
				//We'll need two of these eventually
				//lmTexture->setImage(lightmaps[normLMIndex].get());
				//lmTexture->setDataVariance(osg::Object::STATIC);
				//geoState->setTextureAttribute(0, lmEnv, StateAttribute::ON);
				//geoState->setTextureAttributeAndModes(0, dfTex.get()/*lmTexture*/,StateAttribute::ON);
				geoState->setTextureAttribute(1, dfEnv, StateAttribute::ON);
				geoState->setTextureAttributeAndModes(1, dfTex.get(),StateAttribute::ON);
				
				
				curGeo->setStateSet(geoState);
				osg::notify(DEBUG_INFO) << "adding drawable with " << indices->size() << " indices to mesh " << i << std::endl;
				meshI->addDrawable(curGeo);
			}
		}
		osg::Matrixf meshTransform = csm->transform;
		meshTransform.scale(csm->scale);
		osg::MatrixTransform* meshTransformGroup = new osg::MatrixTransform(meshTransform);
		meshTransformGroup->addChild(meshI);
		//if(i == 4)
		intnode_root->addChild(meshTransformGroup);
		//intnode_root->addChild(meshI);
		osg::notify(DEBUG_INFO)<< verts->size() << " vertices for mesh " << i << std::endl;
		delete [] mlsIndices;
		delete [] vmaps;
		delete [] mlGeo;
	}
	
	return intnode_root;
}


osg::Vec3f DIFReader::takeMin(osg::Vec3f vec1, osg::Vec3f vec2){
	vec1.set(vec1.x() < vec2.x() ? vec1.x() : vec2.x(),
			 vec1.y() < vec2.y() ? vec1.y() : vec2.y(),
			 vec1.z() < vec2.z() ? vec1.z() : vec2.z());
	return vec1;
}

osg::Vec3f DIFReader::takeMax(osg::Vec3f vec1, osg::Vec3f vec2){
	vec1.set(vec1.x() > vec2.x() ? vec1.x() : vec2.x(),
			 vec1.y() > vec2.y() ? vec1.y() : vec2.y(),
			 vec1.z() > vec2.z() ? vec1.z() : vec2.z());
	return vec1;
}

void DIFReader::readVec(osgDB::ifstream * inputStream, osg::Vec2f * vec){
	inputStream->read((S8*)&(vec->_v[0]), sizeof(F32));
	convertLEndianToHost(&(vec->_v[0]));
	inputStream->read((S8*)&(vec->_v[1]), sizeof(F32));
	convertLEndianToHost(&(vec->_v[1]));
}

void DIFReader::readVec(osgDB::ifstream * inputStream, osg::Vec3f * vec){
	inputStream->read((S8*)&(vec->_v[0]), sizeof(F32));
	convertLEndianToHost(&(vec->_v[0]));
	inputStream->read((S8*)&(vec->_v[1]), sizeof(F32));
	convertLEndianToHost(&(vec->_v[1]));
	inputStream->read((S8*)&(vec->_v[2]), sizeof(F32));
	convertLEndianToHost(&(vec->_v[2]));
	
}

void DIFReader::readVec(osgDB::ifstream * inputStream, osg::Vec4f * vec){
	inputStream->read((S8*)&(vec->_v[0]), sizeof(F32));
	convertLEndianToHost(&(vec->_v[0]));
	inputStream->read((S8*)&(vec->_v[1]), sizeof(F32));
	convertLEndianToHost(&(vec->_v[1]));
	inputStream->read((S8*)&(vec->_v[2]), sizeof(F32));
	convertLEndianToHost(&(vec->_v[2]));
	inputStream->read((S8*)&(vec->_v[3]), sizeof(F32));
	convertLEndianToHost(&(vec->_v[3]));
	
}

void DIFReader::readColor(osgDB::ifstream * inputStream, osg::Vec4f * col){
	const F32 inv255 = 1.0f / 255.0f;
	U8 c1, c2, c3, c4;
	//osg::notify(DEBUG_INFO) << "reading color" << std::endl;
	
	inputStream->read((S8*)&c1, sizeof(U8));
	inputStream->read((S8*)&c2, sizeof(U8));
	inputStream->read((S8*)&c3, sizeof(U8));
	inputStream->read((S8*)&c4, sizeof(U8));
	col->set(F32(c1)*inv255,
			 F32(c2)*inv255,
			 F32(c3)*inv255,
			 F32(c4)*inv255);
	
}

void DIFReader::readString(osgDB::ifstream * inputStream, char buf[256])
{
	U8 len;
	inputStream->read((S8*)&len, sizeof(U8));
	inputStream->read(buf, S32(len));
	buf[len] = 0;
	//osg::notify(DEBUG_INFO) << "read string \"" << buf << "\"" << std::endl;
}

void DIFReader::readLine(osgDB::ifstream * inputStream, U8 *buffer, U32 bufferSize)
{
	bufferSize--;  // account for NULL terminator
	U8 *buff = buffer;
	U8 *buffEnd = buff + bufferSize;
	*buff = '\r';
	
	// strip off preceding white space
	while ( *buff == '\r' )
	{
		if ( inputStream->read((S8*)buff, sizeof(S8)).fail() || *buff == '\n' )
		{
			*buff = 0;
			return;
		}
	}
	
	// read line
	while ( buff != buffEnd && !inputStream->read((S8*)++buff, sizeof(S8)).fail() && *buff != '\n' )
	{
		if ( *buff == '\r' )
		{
			
//#if defined(TORQUE_OS_MAC)
			//U32 pushPos = getPosition(); // in case we need to back up.
			if (!(inputStream->read((S8*)buff, sizeof(S8)).fail())) // feeling free to overwrite the \r as the NULL below will overwrite again...
				if (*buff != '\n') // then push our position back.
					inputStream->putback(*buff);
			break; // we're always done after seeing the CR...
/*#else
			buff--; // 'erases' the CR of a CRLF
#endif
//*/			
		}
	}
	*buff = 0;
}


//--------------------------------------
void DIFReader::readMatList(osgDB::ifstream * inputStream, MaterialList * matList)
{
	//free();
	
	// check the stream version
	
	U8 version;
	if ( inputStream->read((S8*)&version, sizeof(S8)) && version != BINARY_FILE_VERSION){
		readMatListText(inputStream, matList, version);
		return;
	}
	//osg::notify(DEBUG_INFO) << "read version " << S32(version) << std::endl;
	
	// how many materials?
	U32 count;
	if ( inputStream->read((S8*)&count,sizeof(U32)).fail() )
		return;
	convertLEndianToHost(&count);
	
	//osg::notify(DEBUG_INFO)<<"reading " << count << " materials" <<std::endl;
	
	// pre-size the vectors for efficiency
	matList->materials.reserve(count);
	matList->materialNames.reserve(count);
	
	// read in the materials
	for (U32 i=0; i<count; i++)
	{
		// Load the bitmap name
		char buffer[256];
		readString(inputStream, buffer);
		if( !buffer[0] )
		{
			osg::notify(FATAL) <<  "MaterialList::read: error reading stream" << std::endl;
			exit(-1);
		}
		
		// Material paths are a legacy of Tribes tools,
		// strip them off...
		char *name = &buffer[strlen(buffer)];
		while (name != buffer && name[-1] != '/' && name[-1] != '\\')
			name--;
		
		// Add it to the list
		matList->materials.push_back(NULL);
		matList->materialNames.push_back(new char[strlen(name) + 1]);
		strcpy(matList->materialNames.back(), name);
	}
	
	return;
}


//--------------------------------------
void DIFReader::readMatListText(osgDB::ifstream * inputStream, MaterialList * matList, U8 firstByte)
{
	//free();
	
	if (!firstByte)
		return;
	
	char buf[1024];
	buf[0] = firstByte;
	U32 offset = 1;
	
	for(;;)
	{
		readLine(inputStream, (U8*)(buf+offset), sizeof(buf)-offset);
		if(!buf[0])
			break;
		offset = 0;
		
		// Material paths are a legacy of Tribes tools,
		// strip them off...
		char *name = &buf[strlen(buf)];
		while (name != buf && name[-1] != '/' && name[-1] != '\\')
			name--;
		
		// Add it to the list
		matList->materials.push_back(NULL);
		matList->materialNames.push_back(new char[strlen(name) + 1]);
		strcpy(matList->materialNames.back(), name);
	}
	return;
}

void DIFReader::readMatListText(osgDB::ifstream * inputStream, MaterialList * matList)
{
	U8 firstByte;
	inputStream->read((S8*)&firstByte,sizeof(U8));
	readMatListText(inputStream,matList,firstByte);
}

void DIFReader::readCSMatList(osgDB::ifstream * inputStream, CSMaterialList * csMatList)
{
	readMatList(inputStream, &(csMatList->materials));
	U32 matCount = csMatList->materials.materials.size();
	U32 i,j;
	F32 tmpf;
	// Allocate the rest of the space we'll need.
	csMatList->flags.reserve(matCount);
	csMatList->reflectanceMaps.reserve(matCount);
	csMatList->bumpMaps.reserve(matCount);
	csMatList->detailMaps.reserve(matCount);
	csMatList->lightMaps.reserve(matCount);
	csMatList->detailScales.reserve(matCount);
	csMatList->reflectionAmounts.reserve(matCount);
	
	// We don't have to worry about old school data
	// we are version 25, and baked meshes are a new feature
	for(i = 0; i < matCount; i++){
		inputStream->read((S8*)&j,sizeof(U32));
		convertLEndianToHost(&j);
		csMatList->flags.push_back(j);
	}
	
	for (i=0; i<matCount; i++){
		inputStream->read((S8*)&j,sizeof(U32));
		convertLEndianToHost(&j);
		csMatList->reflectanceMaps.push_back(j);
	}
	for (i=0; i<matCount; i++){
		inputStream->read((S8*)&j,sizeof(U32));
		convertLEndianToHost(&j);
		csMatList->bumpMaps.push_back(j);
	}
	for (i=0; i<matCount; i++){
		inputStream->read((S8*)&j,sizeof(U32));
		convertLEndianToHost(&j);
		csMatList->detailMaps.push_back(j);
	}
		for (i=0; i<matCount; i++){
			inputStream->read((S8*)&j,sizeof(U32));
			convertLEndianToHost(&j);
			csMatList->lightMaps.push_back(j);
		}
	
	for (i=0; i<matCount; i++){
		inputStream->read((S8*)&tmpf,sizeof(F32));
		convertLEndianToHost(&tmpf);
		csMatList->detailScales.push_back(tmpf);
	}
	
	for (i=0; i<matCount; i++){
		inputStream->read((S8*)&tmpf,sizeof(F32));
		convertLEndianToHost(&tmpf);
		csMatList->reflectionAmounts.push_back(tmpf);
	}
		
	// get rid of name of any ifl material names
	for (i=0; i<matCount; i++)
	{
		if (csMatList->flags[i] & DIFInteriorObj::IflMaterial)
		{
			delete [] csMatList->materials.materialNames[i];
			csMatList->materials.materialNames[i] = NULL;
		}
	}
}

void DIFReader::readLMapTexGen(osgDB::ifstream * inputStream, DTexGen * texGen)
{
	F32 genX[4];
	F32 genY[4];
	
	for(U32 i = 0; i < 4; i++)
	{
		genX[i] = 0.0f;
		genY[i] = 0.0f;
	}
	
	U16 finalWord;
	inputStream->read((S8*)&finalWord, sizeof(U16));
	convertLEndianToHost(&finalWord);
	
	
	inputStream->read((S8*)&genX[3], sizeof(F32));
	convertLEndianToHost(&genX[3]);
	inputStream->read((S8*)&genY[3], sizeof(F32));
	convertLEndianToHost(&genY[3]);
	
	// Unpack the final word.
	U32 logScaleY = (finalWord >> 0) & ((1 << 6) - 1);
	U32 logScaleX = (finalWord >> 6) & ((1 << 6) - 1);
	U16 stEnc     = (finalWord >> 13) & 7;
	
	S32 sc, tc;
	switch(stEnc)
	{
		case 0: sc = 0; tc = 1; break;
		case 1: sc = 0; tc = 2; break;
		case 2: sc = 1; tc = 0; break;
		case 3: sc = 1; tc = 2; break;
		case 4: sc = 2; tc = 0; break;
		case 5: sc = 2; tc = 1; break;
			
		default:
			osg::notify(FATAL) << "Invalid st coord encoding in DIFReader::readLMapTG" << std::endl;
			exit(-1);
	}
	
	U32 invScaleX = 1 << logScaleX;
	U32 invScaleY = 1 << logScaleY;
	
	genX[sc] = F32(1.0 / F64(invScaleX));
	genY[tc] = F32(1.0 / F64(invScaleY));
	
	texGen->xPlane.set(genX[0],genX[1],genX[2],genX[3]);
	texGen->yPlane.set(genY[0],genY[1],genY[2],genY[3]);
}

osg::ref_ptr<osg::Image> DIFReader::readImage(osgDB::ifstream * mapFile){
	if(!reader.valid()){
		osg::notify(FATAL) << "Invalid image reader" << std::endl;
		exit(-1);
	}
	osgDB::ReaderWriter::ReadResult rr = reader->readImage(*mapFile);
	if(rr.success()){
		return rr.takeImage();
	} else{
		osg::notify(FATAL) << "DIFReader::readImage Sorry, we screwed this one up" << std::endl;
		exit(-1);
	}
}

void DIFReader::readCSMesh(osgDB::ifstream * mapFile, DIFInteriorObj::CSMesh * csm){
	U32 i = 0;
	//F32 tmpf;
	U32 j;
	U16 k;
	U8 flag;
	osg::Vec3f tmp1,tmp2;
	osg::Vec2f tmp2f;
	osg::Plane plane1, plane2;
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "\treading " << j << " csm primitives" << std::endl;
	csm->primitives.reserve(j);
	DIFInteriorObj::CSMPrimitive tmp_prim;
	for(i = 0; i < csm->primitives.capacity(); i++){
		mapFile->read((S8*)&tmp_prim.alpha,sizeof(U8));
		
		mapFile->read((S8*)&tmp_prim.texS, sizeof(U32));
		convertLEndianToHost(&tmp_prim.texS);
		mapFile->read((S8*)&tmp_prim.texT, sizeof(U32));
		convertLEndianToHost(&tmp_prim.texT);
		mapFile->read((S8*)&tmp_prim.diffuseIndex, sizeof(S32));
		convertLEndianToHost(&tmp_prim.diffuseIndex);
		mapFile->read((S8*)&tmp_prim.lightMapIndex, sizeof(S32));
		convertLEndianToHost(&tmp_prim.lightMapIndex);
		mapFile->read((S8*)&tmp_prim.start, sizeof(U32));
		convertLEndianToHost(&tmp_prim.start);
		mapFile->read((S8*)&tmp_prim.count, sizeof(U32));
		convertLEndianToHost(&tmp_prim.count);
		
		/*readVec(mapFile, &tmp1);
		mapFile->read((S8*)&tmpf, sizeof(F32));
		convertLEndianToHost(&tmpf);
		plane1 = osg::Plane(tmp1, tmpf);
		readVec(mapFile, &tmp2);
		mapFile->read((S8*)&tmpf, sizeof(F32));
		convertLEndianToHost(&tmpf);
		plane2 = osg::Plane(tmp2, tmpf);
		tmp_prim.lightMapEquation = new osg::TexGen();
		tmp_prim.lightMapEquation->setPlane(osg::TexGen::S, plane1);
		tmp_prim.lightMapEquation->setPlane(osg::TexGen::T, plane2);*/
		readVec(mapFile, &tmp_prim.lightMapEquation.xPlane);
		readVec(mapFile, &tmp_prim.lightMapEquation.yPlane);
		mapFile->read((S8*)&tmp_prim.lightMapOffsetX, sizeof(S32));
		convertLEndianToHost(&tmp_prim.lightMapOffsetX);
		mapFile->read((S8*)&tmp_prim.lightMapOffsetY, sizeof(S32));
		convertLEndianToHost(&tmp_prim.lightMapOffsetY);
		mapFile->read((S8*)&tmp_prim.lightMapSizeX, sizeof(S32));
		convertLEndianToHost(&tmp_prim.lightMapSizeX);
		mapFile->read((S8*)&tmp_prim.lightMapSizeY, sizeof(S32));
		convertLEndianToHost(&tmp_prim.lightMapSizeY);
		
		csm->primitives.push_back(tmp_prim);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "\treading " << j << " indices" << std::endl;
	csm->indices.reserve(j);
	for(i = 0; i<csm->indices.capacity(); i++){
		mapFile->read((S8*)&k,sizeof(U16));
		convertLEndianToHost(&k);
		csm->indices.push_back(k);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "\treading " << j << " vertices" << std::endl;
	csm->verts.reserve(j);
	for(i = 0; i<csm->verts.capacity(); i++){
		readVec(mapFile, &tmp1);
		csm->verts.push_back(tmp1);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "\treading " << j << " normals" << std::endl;
	csm->norms.reserve(j);
	for(i = 0; i<csm->norms.capacity(); i++){
		readVec(mapFile, &tmp1);
		csm->norms.push_back(tmp1);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "\treading " << j << " diffuse UVs" << std::endl;
	csm->diffuseUVs.reserve(j);
	for(i = 0; i<csm->diffuseUVs.capacity(); i++){
		readVec(mapFile, &tmp2f);
		csm->diffuseUVs.push_back(tmp2f);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "\treading " << j << " lightmap UVs" << std::endl;
	csm->lightmapUVs.reserve(j);
	for(i = 0; i<csm->lightmapUVs.capacity(); i++){
		readVec(mapFile, &tmp2f);
		csm->lightmapUVs.push_back(tmp2f);
	}
	
	mapFile->read((S8*)&csm->hasMaterialList,sizeof(U8));
	if(csm->hasMaterialList){
		readCSMatList(mapFile, &csm->csMaterialList);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "\tchecking " << j << " materials for PNG entries" << std::endl;
	csm->embedFlags.reserve(j);
	csm->embeddedMats.reserve(j);
	for(i = 0; i<csm->embedFlags.capacity(); i++){
		mapFile->read((S8*)&flag,sizeof(U8));
		csm->embedFlags.push_back(flag);
		osg::ref_ptr<osg::Image> tmpimg;
		tmpimg = flag ? readImage(mapFile) : NULL;
		csm->embeddedMats.push_back(tmpimg);
	}
	
	mapFile->read((S8*)&csm->hasSolid,sizeof(U8));
	mapFile->read((S8*)&csm->hasTranslucency,sizeof(U8));
	osg::notify(DEBUG_INFO) << "\t found isSolid(" << U32(csm->hasSolid) << ") and hasTranslucency(" << U32(csm->hasTranslucency) << ")" << std::endl;
	
	readVec(mapFile, &tmp1);
	readVec(mapFile, &tmp2);
	csm->bounds.set(tmp1, tmp2);
	
	osg::notify(DEBUG_INFO) << "\t setting transform" << std::endl;
	F32 mat[16]; 
	for(i = 0; i < 16; i++){
		mapFile->read((S8*)&mat[4*(i % 4) + (i/4)],sizeof(F32));
		convertLEndianToHost(&mat[4*(i % 4) + (i/4)]);
	}
	csm->transform.set(mat);
	
	readVec(mapFile, &csm->scale);
	
	tmp1.set(F32_MAX, F32_MAX, F32_MAX);	// Min extents -- doesn't look like it yet, but we pull the faces
	tmp2.set(-F32_MAX, -F32_MAX, -F32_MAX);	// Max extents --  out from an inside-out box of maximum extent
	osg::notify(DEBUG_INFO) << "\t setting bounds" << std::endl;
	for(i = 0; i < csm->verts.capacity(); i++){
		tmp1 = takeMin(tmp1, csm->verts[i]);
		tmp2 = takeMax(tmp2, csm->verts[i]);
	}
	csm->bounds.set(tmp1, tmp2);
	
	osg::notify(DEBUG_INFO) << std::endl;
}


bool DIFInteriorObj::getUnifiedZone(const U32 index, S32* zone)
{
	if(isBSPLeafIndex(index))
	{
		if(isBSPSolidLeaf(index))
			*zone = -1;
		else
			*zone = S32(getBSPEmptyLeafZone(index));
		return true;
	}
	else
	{
		S32 frontZone, backZone;
		bool frontUnified = getUnifiedZone(iBSPNodes[index].frontIndex, &frontZone);
		bool backUnified  = getUnifiedZone(iBSPNodes[index].backIndex, &backZone);
		if(frontUnified && backUnified)
		{
			if(frontZone == backZone)
			{
				*zone = frontZone;
				return true;
			}
			else
			{
				if(frontZone == -1 || backZone == -1)
				{
					// DMMFIX: Once the interior file format is able to distinguish
					//  between structural and detail nodes in the runtime bsp,
					//  we can make this work a little better.
					return false;
				}
				else
				{
					// Not equal, and neither is -1, no unified zone possible
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
}

void DIFInteriorObj::truncateZoneNode(const U32 index)
{
	S32 unifiedZone;
	if(getUnifiedZone(index, &unifiedZone))
	{
		// Aha!
		if(isBSPLeafIndex(index)) return;
		iBSPNodes[index].terminalZone = (unifiedZone == -1) ? U16(0xFFFF) : U16(0x8000) | U16(unifiedZone);
	}
	else
	{
		// Sigh.
		if(!isBSPLeafIndex(iBSPNodes[index].frontIndex))
			truncateZoneNode(iBSPNodes[index].frontIndex);
		if(!isBSPLeafIndex(iBSPNodes[index].backIndex))
			truncateZoneNode(iBSPNodes[index].backIndex);
	}
}

void DIFInteriorObj::truncateZoneTree()
{
	for(U32 i = 0; i < iBSPNodes.size(); i++) iBSPNodes[i].terminalZone = 0;
	if(iBSPNodes.size() > 0) truncateZoneNode(0);
}

void DIFInteriorObj::setupZonePlanes(){
	U32 tempSize = planeList.size() * zones.size();
	U16* temp = tempSize ? new U16[tempSize] : NULL;
	tempSize = 0;
	
	for(U32 i = 0; i < zones.size(); i++)
	{
		Zone& rZone = zones[i];
		
		bool usedPlanes[planeList.size()];
		
		U32 j;
		for(j = 0; j < rZone.surfaceCount; j++)
		{
			Surface& rSurface = surfaces[zoneSurfaces[rZone.surfaceStart + j]];
			usedPlanes[U16(rSurface.planeIndex & ~0x8000)] = true;
		}
		
		rZone.planeStart = tempSize;
		for(j = 0; j < planeList.size(); j++)
		{
			if(usedPlanes[j])
			{
				if(tempSize >= planeList.size() * zones.size())
				{
					osg::notify(FATAL) << "Error, out of bounds plane list!" << std::endl;
					exit(-1);
				}
				temp[tempSize++] = j;
			}
		}
		rZone.planeCount = tempSize - rZone.planeStart;
	}
	osg::notify(DEBUG_INFO) << "\tread " << tempSize << " zone planes" << std::endl;
	
	zonePlanes.reserve(tempSize);
	for(U32 j = 0; j < tempSize; j++)
		zonePlanes.push_back(temp[j]);
	if(temp) delete [] temp;
}


void DIFInteriorObj::buildSurfaceZones()
{
	surfaceZone.clear();
	surfaceZone.resize(surfaces.size(), -1);
	/*
	for(U32 i=0; i<surfaces.size(); i++)
	{
		mSurfaceZone.push_back(-1);
	}
	//*/
	
	for(U32 z=0; z<zones.size(); z++)
	{
		Zone &zone = zones[z];
		for(U32 s=zone.surfaceStart; s<(zone.surfaceStart + zone.surfaceCount); s++)
		{
			surfaceZone[zoneSurfaces[s]] = (z - 1);
		}
	}
}

bool DIFReader::readDIO(osgDB::ifstream * mapFile, DIFInteriorObj * curLevel){
	U32 i = 0;
	U8	flag = 0;
	U32 j;
	U16 k;
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->iFV = j;
	osg::notify(DEBUG_INFO) << "File Version " << j << std::endl;
	if(curLevel->iFV > IFV){
		return false;
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->detailLevel = (j > MIN_PIXELS) ? j : MIN_PIXELS; // Force "good" (reasonable) behavior
	osg::notify(DEBUG_INFO) << "Setting min pixels to " << curLevel->detailLevel << " (" << j << " read)" << std::endl;
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->minPixels = j;
	
	
	osg::Vec3f tmp1;
	osg::Vec3f tmp2;
	F32 tmpf;
	
	readVec(mapFile, &tmp1);
	readVec(mapFile, &tmp2);
	curLevel->bBox.set(tmp1, tmp2);
	readVec(mapFile, &tmp1);
	mapFile->read((S8*)&tmpf, sizeof(F32));
	convertLEndianToHost(&tmpf);
	curLevel->bSphere.set(tmp1,tmpf);
	
	mapFile->read((S8*)&(curLevel->hasAlarmState), sizeof(U8));
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->numLightStateEntries = j;
	
	osg::notify(DEBUG_INFO) << "alarm state? " << curLevel->hasAlarmState << " nlightstateentries " << curLevel->numLightStateEntries << std::endl;
	
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::Vec3f * normals = new osg::Vec3f[j];
	osg::notify(DEBUG_INFO) << "reading " << j << " normals" << std::endl;
	for(i = 0; i < j; i++){
		readVec(mapFile, &(normals[i]));
		//osg::notify(DEBUG_INFO) << normals[i].x() << " " << normals[i].y() << " " << normals[i].z() << std::endl;
	}
	
	U16 index;
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	
	curLevel->planeList.reserve(j);
	for(i = 0; i < curLevel->planeList.capacity(); i++)
	{
		mapFile->read((S8*)&index, sizeof(U16));  //Here, it seems the spec lies.
		convertLEndianToHost(&index);
		mapFile->read((S8*)&tmpf, sizeof(F32));
		convertLEndianToHost(&tmpf);
		
		curLevel->planeList.push_back(osg::Plane(normals[index], tmpf));
		
	}
	
	delete [] normals;
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	
	curLevel->pointList.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " points" << std::endl;
	for (i = 0; i < curLevel->pointList.capacity(); i++){
		
		readVec(mapFile, &tmp1);
		ItrPaddedPoint pt;
		pt.point = tmp1;
		curLevel->pointList.push_back(pt);
	}
	
	
	//We don't use this for anything, but it is in the file.
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	
	curLevel->pointVisList.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " point vis states" << std::endl;
	
	for( i = 0; i < curLevel->pointVisList.capacity(); i++)
	{
		mapFile->read((S8*)(&flag), sizeof(S8));
		curLevel->pointVisList.push_back(flag);
	}
	
	
	
	//s = x, t = y
	//figure the rest out later.
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->texGenEQs.reserve(j);
	
	osg::Plane plane1;
	osg::Plane plane2;
	osg::notify(DEBUG_INFO) << "reading " << j << " texGenEQs" << std::endl;
	
	DTexGen tmp_texgen;
//	osg::notify(DEBUG_INFO) << "PYSPLIT" << std::endl;
	for(i = 0; i < curLevel->texGenEQs.capacity(); i++){
		/*readVec(mapFile, &tmp1);
		mapFile->read((S8*)&tmpf, sizeof(F32));
		convertLEndianToHost(&tmpf);
		plane1 = osg::Plane(tmp1, tmpf);
		readVec(mapFile, &tmp2);
		mapFile->read((S8*)&tmpf, sizeof(F32));
		convertLEndianToHost(&tmpf);
		plane2 = osg::Plane(tmp2, tmpf);
		curLevel->texGenEQs.push_back(new TexGen());
		curLevel->texGenEQs[i]->setPlane(osg::TexGen::S, plane1);
		curLevel->texGenEQs[i]->setPlane(osg::TexGen::T, plane2);*/
		readVec(mapFile, &tmp_texgen.xPlane);
		readVec(mapFile, &tmp_texgen.yPlane);
		//if(i < 10){
		//	osg::notify(DEBUG_INFO) << "\t" << tmp_texgen.xPlane.x() << " " << tmp_texgen.xPlane.y() << " " << tmp_texgen.xPlane.z() << " " << tmp_texgen.xPlane.w() << std::endl;
		//	osg::notify(DEBUG_INFO) << "\t" << tmp_texgen.yPlane.x() << " " << tmp_texgen.yPlane.y() << " " << tmp_texgen.yPlane.z() << " " << tmp_texgen.yPlane.w() << std::endl;
		//	osg::notify(DEBUG_INFO) << std::endl;
		//}
		curLevel->texGenEQs.push_back(tmp_texgen);
	}
//	osg::notify(DEBUG_INFO) << "PYSPLIT" << std::endl;
	
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->iBSPNodes.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " IBSPNodes" << std::endl;
	DIFInteriorObj::IBSPNode tmp_ibspnode;
	for(i = 0; i < curLevel->iBSPNodes.capacity(); i++){
		mapFile->read((S8*)&k, sizeof(U16));
		convertLEndianToHost(&k);
		tmp_ibspnode.planeIndex = k;
		if(curLevel->iFV >= 14){
			mapFile->read((S8*)&j, sizeof(U32));
			convertLEndianToHost(&j);
			tmp_ibspnode.frontIndex = j;
			
			mapFile->read((S8*)&j, sizeof(U32));
			convertLEndianToHost(&j);
			tmp_ibspnode.backIndex = j;
		} else{
			
			mapFile->read((S8*)&k, sizeof(U16));
			convertLEndianToHost(&k);
			tmp_ibspnode.frontIndex = U32(k);
			
			mapFile->read((S8*)&k, sizeof(U16));
			convertLEndianToHost(&k);
			tmp_ibspnode.backIndex = U32(k);
		}
		curLevel->iBSPNodes.push_back(tmp_ibspnode);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->iBSPSolidLeaves.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " IBSPSolidLeaves" << std::endl;
	//	U32 surfaceIndex;
	//	U16 surfaceCount;
	DIFInteriorObj::IBSPLeafSolid tmp_leafsolid;
	for(i = 0; i < curLevel->iBSPSolidLeaves.capacity(); i++){
		
		
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		tmp_leafsolid.surfaceIndex = j;
		
		mapFile->read((S8*)&k, sizeof(U16));
		convertLEndianToHost(&k);
		tmp_leafsolid.surfaceCount = k;
		
		curLevel->iBSPSolidLeaves.push_back(tmp_leafsolid);
	}
	
	readMatList(mapFile, &(curLevel->materialList));
	osg::notify(DEBUG_INFO)<<"read " << curLevel->materialList.materialNames.size() << " material names" << std::endl;
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->windings.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " windings" << std::endl;
	
	for(i= 0; i < curLevel->windings.capacity(); i++){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->windings.push_back(j);
	}
	
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->windingIndices.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " TriFans (winding indices)" << std::endl;
	DIFInteriorObj::TriFan tmp_trifan;
	for(i= 0; i < curLevel->windingIndices.capacity(); i++){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		tmp_trifan.windingStart = j;
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		tmp_trifan.windingCount = j;
		curLevel->windingIndices.push_back(tmp_trifan);
	}
	
	if(curLevel->iFV >= 12){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		
		curLevel->edgeList.reserve(j);
		osg::notify(DEBUG_INFO) << "reading " << j << " edges" << std::endl;
		DIFInteriorObj::Edge tmp_edge;
		for(i= 0; i < curLevel->edgeList.capacity(); i++){
			mapFile->read((S8*)&j, sizeof(S32));
			convertLEndianToHost(&j);
			tmp_edge.vertexes[0] = j;
			mapFile->read((S8*)&j, sizeof(S32));
			convertLEndianToHost(&j);
			tmp_edge.vertexes[1] = j;
			mapFile->read((S8*)&j, sizeof(S32));
			convertLEndianToHost(&j);
			tmp_edge.faces[0] = j;
			mapFile->read((S8*)&j, sizeof(S32));
			convertLEndianToHost(&j);
			tmp_edge.faces[1] = j;
			curLevel->edgeList.push_back(tmp_edge);
		}
	}
	
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->zones.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " zones" << std::endl;
	DIFInteriorObj::Zone tmp_zone;
	for(i = 0; i < curLevel->zones.capacity(); i++){
		mapFile->read((S8*)&tmp_zone.portalStart, sizeof(U16));
		convertLEndianToHost(&tmp_zone.portalStart);
		mapFile->read((S8*)&tmp_zone.portalCount, sizeof(U16));
		convertLEndianToHost(&tmp_zone.portalCount);
		
		mapFile->read((S8*)&tmp_zone.surfaceStart, sizeof(U32));
		convertLEndianToHost(&tmp_zone.surfaceStart);
		mapFile->read((S8*)&tmp_zone.surfaceCount, sizeof(U16));
		convertLEndianToHost(&tmp_zone.surfaceCount);
		
		/*
		mapFile->read((S8*)&tmp_zone.planeStart, sizeof(U32));
		convertLEndianToHost(&tmp_zone.planeStart);
		mapFile->read((S8*)&tmp_zone.planeCount, sizeof(U16));
		convertLEndianToHost(&tmp_zone.planeCount);
		//*/
		
		if (curLevel->iFV >= 12)
		{
			mapFile->read((S8*)&tmp_zone.staticMeshStart, sizeof(U32));
			convertLEndianToHost(&tmp_zone.staticMeshStart);
			mapFile->read((S8*)&tmp_zone.staticMeshCount, sizeof(U32));
			convertLEndianToHost(&tmp_zone.staticMeshCount);
		}
		else{
			tmp_zone.staticMeshStart = 0;
			tmp_zone.staticMeshCount = 0;
		}
		mapFile->read((S8*)&tmp_zone.flags, sizeof(U16));
		convertLEndianToHost(&tmp_zone.flags);
		tmp_zone.zoneId = 0;
		curLevel->zones.push_back(tmp_zone);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->zoneSurfaces.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " zone surfaces" << std::endl;
	for(i= 0; i < curLevel->zoneSurfaces.capacity(); i++){
		mapFile->read((S8*)&k, sizeof(U16));
		convertLEndianToHost(&k);
		curLevel->zoneSurfaces.push_back(k);
	}
	
	if(curLevel->iFV >= 12){ // This seems to be missing from the spec...
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		
		curLevel->zoneStaticMeshes.reserve(j);
		osg::notify(DEBUG_INFO) << "reading " << j << " zone static meshes" << std::endl;
		for(i= 0; i < curLevel->zoneStaticMeshes.capacity(); i++){
			mapFile->read((S8*)&j, sizeof(U32));
			convertLEndianToHost(&j);
			curLevel->zoneStaticMeshes.push_back(j);
		}
		
	} 
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->zonePortalList.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " entries into zonePortalList" << std::endl;
	for(i= 0; i < curLevel->zonePortalList.capacity(); i++){
		mapFile->read((S8*)&k, sizeof(U16));
		convertLEndianToHost(&k);
		curLevel->zonePortalList.push_back(k);
	}
	

	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->portals.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " portals" << std::endl;
	DIFInteriorObj::Portal tmp_portal;
	for(i= 0; i < curLevel->portals.capacity(); i++){
		mapFile->read((S8*)&tmp_portal.planeIndex, sizeof(U16));
		convertLEndianToHost(&tmp_portal.planeIndex);
		
		mapFile->read((S8*)&tmp_portal.triFanCount, sizeof(U16));
		convertLEndianToHost(&tmp_portal.triFanCount);
		mapFile->read((S8*)&tmp_portal.triFanStart, sizeof(U32));
		convertLEndianToHost(&tmp_portal.triFanStart);
		
		mapFile->read((S8*)&tmp_portal.zoneFront, sizeof(U16));
		convertLEndianToHost(&tmp_portal.zoneFront);
		mapFile->read((S8*)&tmp_portal.zoneBack, sizeof(U16));
		convertLEndianToHost(&tmp_portal.zoneBack);
		curLevel->portals.push_back(tmp_portal);
	}
	
	
	//Surfaces! Yay! The Best Part!
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->surfaces.reserve(j);
	curLevel->lmTexGenEQs.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " surfaces and lmTexGenEQs" << std::endl;
	DIFInteriorObj::Surface tmp_surface;

	for(i= 0; i < curLevel->surfaces.capacity(); i++){
		mapFile->read((S8*)&tmp_surface.windingStart, sizeof(U32));
		convertLEndianToHost(&tmp_surface.windingStart);
		
		if(curLevel->iFV >= 13){
			mapFile->read((S8*)&tmp_surface.windingCount, sizeof(U32));
			convertLEndianToHost(&tmp_surface.windingCount);
		} else{
			mapFile->read((S8*)&flag,sizeof(U8));
			tmp_surface.windingCount = U32(flag);
		}

		
		mapFile->read((S8*)&tmp_surface.planeIndex, sizeof(U16));
		convertLEndianToHost(&tmp_surface.planeIndex);
		mapFile->read((S8*)&tmp_surface.textureIndex, sizeof(U16));
		convertLEndianToHost(&tmp_surface.textureIndex);
		mapFile->read((S8*)&tmp_surface.texGenIndex, sizeof(U32));
		convertLEndianToHost(&tmp_surface.texGenIndex);
		mapFile->read((S8*)&tmp_surface.surfaceFlags, sizeof(U8));
		mapFile->read((S8*)&tmp_surface.fanMask, sizeof(U32));
		convertLEndianToHost(&tmp_surface.fanMask);
		
		
		//readLMapTexGen(mapFile, &plane1, &plane2);
		//curLevel->lmTexGenEQs.push_back(new TexGen());
		//curLevel->lmTexGenEQs[i]->setPlane(osg::TexGen::S, plane1);
		//curLevel->lmTexGenEQs[i]->setPlane(osg::TexGen::T, plane2);
		readLMapTexGen(mapFile, &tmp_texgen);
		curLevel->lmTexGenEQs.push_back(tmp_texgen);
		
		mapFile->read((S8*)&tmp_surface.lightCount, sizeof(U16));
		convertLEndianToHost(&tmp_surface.lightCount);
		
		mapFile->read((S8*)&tmp_surface.lightStateInfoStart, sizeof(U32));
		convertLEndianToHost(&tmp_surface.lightStateInfoStart);
		
		if(curLevel->iFV >= 13){
			mapFile->read((S8*)&tmp_surface.mapOffsetX, sizeof(U32));
			convertLEndianToHost(&tmp_surface.mapOffsetX);
			mapFile->read((S8*)&tmp_surface.mapOffsetY, sizeof(U32));
			convertLEndianToHost(&tmp_surface.mapOffsetY);
			mapFile->read((S8*)&tmp_surface.mapSizeX, sizeof(U32));
			convertLEndianToHost(&tmp_surface.mapSizeX);
			mapFile->read((S8*)&tmp_surface.mapSizeY, sizeof(U32));
			convertLEndianToHost(&tmp_surface.mapSizeY);
		} else{
			mapFile->read((S8*)&flag,sizeof(U8));
			tmp_surface.mapOffsetX= U32(flag);
			mapFile->read((S8*)&flag,sizeof(U8));
			tmp_surface.mapOffsetY = U32(flag);
			mapFile->read((S8*)&flag,sizeof(U8));
			tmp_surface.mapSizeX = U32(flag);
			mapFile->read((S8*)&flag,sizeof(U8));
			tmp_surface.mapSizeY = U32(flag);
		}
		
		if(curLevel->iFV == 1 || curLevel->iFV >= 12){
			mapFile->read((S8*)&tmp_surface.unused, sizeof(U8));
		}
		curLevel->surfaces.push_back(tmp_surface);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->normalLMapIndices.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " normal LMap indices" << std::endl;
	for(i = 0; i < curLevel->normalLMapIndices.capacity(); i++){
		if(curLevel->iFV >= 13){
			mapFile->read((S8*)&j, sizeof(U32));
			convertLEndianToHost(&j);
			curLevel->normalLMapIndices.push_back(j);	
		} else{
			flag = 0;	// Not sure why this is needed, but the Torque implementation did it, 
						// so follow suit to avoid debugging subtleties.
						//This code mostly just needs to work
			mapFile->read((S8*)&flag,sizeof(U8));
			curLevel->normalLMapIndices.push_back(U32(flag));
		}
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->alarmLMapIndices.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " alarm LMap indices" << std::endl;
	for(i = 0; i < curLevel->alarmLMapIndices.capacity(); i++){
		if(curLevel->iFV >= 13){
			mapFile->read((S8*)&j, sizeof(U32));
			convertLEndianToHost(&j);
			curLevel->alarmLMapIndices.push_back(j);	
		} else{
			flag = 0;	// Not sure why this is needed, but the Torque implementation did it, 
			// so follow suit to avoid debugging subtleties.
			//This code mostly just needs to work
			mapFile->read((S8*)&flag,sizeof(U8));
			curLevel->alarmLMapIndices.push_back(U32(flag));
		}
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->nullSurfaces.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " null surfaces" << std::endl;
	DIFInteriorObj::NullSurface tmp_nsurface;
	for(i = 0; i < curLevel->nullSurfaces.capacity(); i++){
		mapFile->read((S8*)&tmp_nsurface.windingStart, sizeof(U32));
		convertLEndianToHost(&tmp_nsurface.windingStart);
		mapFile->read((S8*)&tmp_nsurface.planeIndex, sizeof(U16));
		convertLEndianToHost(&tmp_nsurface.planeIndex);
		mapFile->read((S8*)&tmp_nsurface.surfaceFlags, sizeof(U8));
		if(curLevel->iFV >= 13){
			mapFile->read((S8*)&tmp_nsurface.windingCount, sizeof(U32));
			convertLEndianToHost(&tmp_nsurface.windingCount);	
		} else{
			mapFile->read((S8*)&flag,sizeof(U8));
			tmp_nsurface.windingCount = U32(flag);
		}
		curLevel->nullSurfaces.push_back(tmp_nsurface);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->lightmaps.reserve(j);
	curLevel->lightDirMaps.reserve(j);
	curLevel->lightmapKeep.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " lightmaps and lightDirMaps and lightmapkeeps" << std::endl;
	for(i = 0; i < curLevel->lightmaps.capacity(); i++){
		osg::ref_ptr<osg::Image> tmp1;
		osg::ref_ptr<osg::Image> tmp2; // Should be autocleaned up by ref_ptr
		if((tmp1 = readImage(mapFile)).valid()){
			curLevel->lightmaps.push_back(tmp1);
		} else{
			return false;
		}
		
		if(curLevel->iFV == 1 || curLevel->iFV >= 12){
			if((tmp2 = readImage(mapFile)).valid()){
				curLevel->lightDirMaps.push_back(NULL); // Keep our place in the stream, but don't save the result of this
			} else{
				return false; // We don't care that we lost it, but that we lost our way in the stream
			}
		}
		
		mapFile->read((S8*)&flag,sizeof(U8));
		curLevel->lightmapKeep.push_back(flag);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	
	curLevel->solidLeafSurfaces.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " solid leaf surfaces" << std::endl;
	for(i = 0; i < curLevel->solidLeafSurfaces.capacity(); i++){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->alarmLMapIndices.push_back(j);	
	}
	
	curLevel->numTriggerableLights = 0;
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "skipping " << j << " animated lights (deprecated)" << std::endl;
	i = j*(3*sizeof(U32)+2*sizeof(U16));
	S8 * animBuf = new S8[i];
	mapFile->read(animBuf, i*sizeof(S8));
	delete[] animBuf;
	
	/*
	curLevel->animatedLights.reserve(j);
	DIFInteriorObj::AnimatedLight tmp_anilight;
	for(i = 0; i < curLevel->animatedLights.capacity(); i++){
		mapFile->read((S8*)&tmp_anilight.nameIndex, sizeof(U32));
		convertLEndianToHost(&tmp_anilight.nameIndex);
		mapFile->read((S8*)&tmp_anilight.stateIndex, sizeof(U32));
		convertLEndianToHost(&tmp_anilight.stateIndex);
		
		mapFile->read((S8*)&tmp_anilight.stateIndex, sizeof(U16));
		convertLEndianToHost(&tmp_anilight.stateIndex);
		mapFile->read((S8*)&tmp_anilight.flags, sizeof(U16));
		convertLEndianToHost(&tmp_anilight.flags);
		
		mapFile->read((S8*)&tmp_anilight.duration, sizeof(U32));
		convertLEndianToHost(&tmp_anilight.duration);
		
		curLevel->animatedLights.push_back(tmp_anilight);
		
		if(!(tmp_anilight.flags & DIFInteriorObj::AnimationAmbient)) curLevel->numTriggerableLights++;
	}
	osg::notify(DEBUG_INFO) << "\tfound " << j << " triggerable" << std::endl;
	//*/
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "skipping " << j << " light states (deprecated)" << std::endl;
	i = j*(3*sizeof(U8)+2*sizeof(U32)+sizeof(U16));
	animBuf = new S8[i];
	mapFile->read(animBuf, i*sizeof(S8));
	delete[] animBuf;
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "skipping " << j << " state datum (deprecated)" << std::endl;
	i = j*(2*sizeof(U32)+sizeof(U16));
	animBuf = new S8[i];
	mapFile->read(animBuf, i*sizeof(S8));
	delete[] animBuf;
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "skipping state datum buffer (deprecated) with " << j << " entries" << std::endl;
	j += sizeof(U32); // skipping 4 more bytes for "U32 flags"
	animBuf = new S8[j];
	mapFile->read(animBuf, j*sizeof(S8));
	delete[] animBuf;
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	osg::notify(DEBUG_INFO) << "skipping name buffer (deprecated) of length " << j << std::endl;
	animBuf = new S8[j];
	mapFile->read(animBuf, j*sizeof(S8));
	
	// Basically, we are reading any mirrors. This can probably be implemented somehow
	// or another in OSG, so go for it, even if we don't use them for now.
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->subObjects.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " interior sub-objects" << std::endl;
	InteriorSubObject * tmp_iso;
	for(i = 0; i <curLevel->subObjects.capacity(); i++){
		mapFile->read((S8*)&j, sizeof(U32));
		switch(j){
			case InteriorSubObject::MirrorSubObjectKey:
				tmp_iso = new MirrorSubObject;
				break;
			default:
				tmp_iso = new InteriorSubObject;
		}
		curLevel->subObjects.push_back(tmp_iso->read(mapFile) ? tmp_iso : NULL);
	}
	
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->convexHulls.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " convex hulls" << std::endl;
	DIFInteriorObj::ConvexHull tmp_hull;
	for(i = 0; i <curLevel->convexHulls.capacity(); i++){
		mapFile->read((S8*)&tmp_hull.hullStart, sizeof(U32));
		convertLEndianToHost(&tmp_hull.hullStart);
		mapFile->read((S8*)&tmp_hull.hullCount, sizeof(U16));
		convertLEndianToHost(&tmp_hull.hullCount);
		
		mapFile->read((S8*)&tmp_hull.minX, sizeof(F32));
		convertLEndianToHost(&tmp_hull.minX);
		mapFile->read((S8*)&tmp_hull.maxX, sizeof(F32));
		convertLEndianToHost(&tmp_hull.maxX);
		mapFile->read((S8*)&tmp_hull.minY, sizeof(F32));
		convertLEndianToHost(&tmp_hull.minY);
		mapFile->read((S8*)&tmp_hull.maxY, sizeof(F32));
		convertLEndianToHost(&tmp_hull.maxY);
		mapFile->read((S8*)&tmp_hull.minZ, sizeof(F32));
		convertLEndianToHost(&tmp_hull.minZ);
		mapFile->read((S8*)&tmp_hull.maxZ, sizeof(F32));
		convertLEndianToHost(&tmp_hull.maxZ);
		
		mapFile->read((S8*)&tmp_hull.surfaceStart, sizeof(U32));
		convertLEndianToHost(&tmp_hull.surfaceStart);
		mapFile->read((S8*)&tmp_hull.surfaceCount, sizeof(U16));
		convertLEndianToHost(&tmp_hull.surfaceCount);
		
		mapFile->read((S8*)&tmp_hull.planeStart, sizeof(U32));
		convertLEndianToHost(&tmp_hull.planeStart);
		mapFile->read((S8*)&tmp_hull.polyListPlaneStart, sizeof(U32));
		convertLEndianToHost(&tmp_hull.polyListPlaneStart);
		mapFile->read((S8*)&tmp_hull.polyListPointStart, sizeof(U32));
		convertLEndianToHost(&tmp_hull.polyListPointStart);
		mapFile->read((S8*)&tmp_hull.polyListStringStart, sizeof(U32));
		convertLEndianToHost(&tmp_hull.polyListStringStart);
		
		if(curLevel->iFV >= 12){
			mapFile->read((S8*)&tmp_hull.staticMesh, sizeof(U8));
		} else{
			tmp_hull.staticMesh = false;
		}
		curLevel->convexHulls.push_back(tmp_hull);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j); 
	curLevel->convexHullEmitStrings.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " convex hull emit strings" << std::endl;
	for(i = 0; i < curLevel->convexHullEmitStrings.capacity(); i++){
		mapFile->read((S8*)&flag, sizeof(U8));
		curLevel->convexHullEmitStrings.push_back(flag);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->hullIndices.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " hull indices" << std::endl;
	for(i = 0; i < curLevel->hullIndices.capacity(); i++){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->hullIndices.push_back(j);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->hullPlaneIndices.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " hull plane indices" << std::endl;
	for(i = 0; i < curLevel->hullPlaneIndices.capacity(); i++){
		mapFile->read((S8*)&k, sizeof(U16));
		convertLEndianToHost(&k);
		curLevel->hullPlaneIndices.push_back(k);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->hullEmitStringIndices.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " hull emit string indices" << std::endl;
	for(i = 0; i < curLevel->hullEmitStringIndices.capacity(); i++){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->hullEmitStringIndices.push_back(j);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->hullSurfaceIndices.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " hull surface indices" << std::endl;
	for(i = 0; i < curLevel->hullSurfaceIndices.capacity(); i++){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->hullSurfaceIndices.push_back(j);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->polyListPlanes.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " polylist planes" << std::endl;
	for(i = 0; i < curLevel->polyListPlanes.capacity(); i++){
		mapFile->read((S8*)&k, sizeof(U16));
		convertLEndianToHost(&k);
		curLevel->polyListPlanes.push_back(k);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->polyListPoints.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " polylist points" << std::endl;
	for(i = 0; i < curLevel->polyListPoints.capacity(); i++){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->polyListPoints.push_back(j);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j); 
	curLevel->polyListStrings.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " polylist strings" << std::endl;
	for(i = 0; i < curLevel->polyListStrings.capacity(); i++){
		mapFile->read((S8*)&flag, sizeof(U8));
		curLevel->polyListStrings.push_back(flag);
	}
	
	osg::notify(DEBUG_INFO) << "reading " << (NUMCOORDBINS * NUMCOORDBINS) << " coord bins" << std::endl;
	for(i = 0; i < NUMCOORDBINS * NUMCOORDBINS; i++){
		mapFile->read((S8*)&curLevel->coordBins[i].binStart, sizeof(U32));
		convertLEndianToHost(&curLevel->coordBins[i].binStart);
		mapFile->read((S8*)&curLevel->coordBins[i].binCount, sizeof(U32));
		convertLEndianToHost(&curLevel->coordBins[i].binCount);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->coordBinIndices.reserve(j);
	osg::notify(DEBUG_INFO) << "reading " << j << " coord bin indices" << std::endl;
	for(i = 0; i < curLevel->coordBinIndices.capacity(); i++){
		mapFile->read((S8*)&k, sizeof(U16));
		convertLEndianToHost(&k);
		curLevel->coordBinIndices.push_back(k);
	}
	
	mapFile->read((S8*)&j, sizeof(U32));
	convertLEndianToHost(&j);
	curLevel->coordBinMode = j;
	osg::notify(DEBUG_INFO) << "reading coord bin mode: " << j << std::endl;
	osg::notify(DEBUG_INFO) << "reading ambient colors" << std::endl;
	readColor(mapFile, &curLevel->baseAmbient);
	readColor(mapFile, &curLevel->alarmAmbient);
	osg::notify(DEBUG_INFO) << curLevel->baseAmbient.x() << " " << curLevel->baseAmbient.y() << " " << curLevel->baseAmbient.z() << " " << curLevel->baseAmbient.x() << std::endl;
	osg::notify(DEBUG_INFO) << curLevel->alarmAmbient.x() << " " << curLevel->alarmAmbient.y() << " " << curLevel->alarmAmbient.z() << " " << curLevel->alarmAmbient.x() << std::endl;
	
	if(curLevel->iFV >= 10){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->csMeshes.reserve(j);
		osg::notify(DEBUG_INFO) << "reading " << j << " CSMeshes" << std::endl;
		for(i = 0; i<curLevel->csMeshes.capacity(); i++){
			DIFInteriorObj::CSMesh * tmp_csmesh = new DIFInteriorObj::CSMesh;
			readCSMesh(mapFile, tmp_csmesh);
			curLevel->csMeshes.push_back(*tmp_csmesh);
			delete tmp_csmesh;
		}
		
	}
	
	if(curLevel->iFV >= 11){
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->normals.reserve(j);
		osg::notify(DEBUG_INFO) << "reading " << j << " normals" << std::endl;
		for(i = 0; i < curLevel->normals.capacity(); i++){
			readVec(mapFile, &tmp1);
			curLevel->normals.push_back(tmp1);
		}
		
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->texMatrices.reserve(j);
		osg::notify(DEBUG_INFO) << "reading " << j << " tex-matrices" << std::endl;
		DIFInteriorObj::TexMatrix tmp_texmat;
		for(i = 0; i < curLevel->texMatrices.capacity(); i++){
			mapFile->read((S8*)&tmp_texmat.T,sizeof(S32));
			convertLEndianToHost(&tmp_texmat.T);
			mapFile->read((S8*)&tmp_texmat.N,sizeof(S32));
			convertLEndianToHost(&tmp_texmat.B);
			mapFile->read((S8*)&tmp_texmat.N,sizeof(S32));
			convertLEndianToHost(&tmp_texmat.B);
			
			curLevel->texMatrices.push_back(tmp_texmat);
		}
		
		mapFile->read((S8*)&j, sizeof(U32));
		convertLEndianToHost(&j);
		curLevel->texMatIndices.reserve(j);
		osg::notify(DEBUG_INFO) << "reading " << j << " tex-mat indices" << std::endl;
		for(i = 0; i < curLevel->texMatIndices.capacity(); i++){
			mapFile->read((S8*)&j,sizeof(U32));
			convertLEndianToHost(&j);
			
			curLevel->texMatIndices.push_back(j);
		}
		
	}
	
	//num to read
	U32 dummy;
	/* // Test num dummy values
	dummy = curLevel->iFV;
	for(curLevel->iFV = 9; curLevel->iFV < 12; curLevel->iFV++){
		j = ((curLevel->iFV < 10) ? 1 : 0) + ((curLevel->iFV < 11) ? 2 : 0);
		osg::notify(DEBUG_INFO) << "would read " << j << " dummy values for iFV " << curLevel->iFV << std::endl;
	}
	curLevel->iFV = dummy;
	osg::notify(DEBUG_INFO) << std::endl;
	//*/
	j = ((curLevel->iFV < 10) ? 1 : 0) + ((curLevel->iFV < 11) ? 2 : 0);
	osg::notify(DEBUG_INFO) << "reading " << j << " dummy values for iFV " << curLevel->iFV << std::endl;
	for(i = 0; i < j; i++){
		mapFile->read((S8*)&dummy, sizeof(U32));
		// Don't bother converting endian-ness. ANY bit is a bad bit for zeroness.
		if(dummy) return false;
	}
	
	mapFile->read((S8*)&j,sizeof(U32));
	convertLEndianToHost(&j);
	if(j == 1){
		mapFile->read((S8*)&curLevel->lightmapBorderSize,sizeof(U32));
		convertLEndianToHost(&curLevel->lightmapBorderSize);
		
		mapFile->read((S8*)&dummy, sizeof(U32));
		// Don't bother converting endian-ness. ANY bit is a bad bit for zeroness.
		if(dummy) return false;
	} else{
		curLevel->lightmapBorderSize = 0;
	}
	
	osg::notify(DEBUG_INFO) << "Setting up zone planes " << std::endl;
	curLevel->setupZonePlanes();
	osg::notify(DEBUG_INFO) << "Truncating zone tree " << std::endl;
	curLevel->truncateZoneTree();
	osg::notify(DEBUG_INFO) << "Building surface zones " << std::endl;
	curLevel->buildSurfaceZones();
	osg::notify(DEBUG_INFO) << "Done!" << std::endl;
	
	return true;
	
}

bool DIFReader::readFile(const std::string & file,
						 const osgDB::ReaderWriter::Options* options)
{
	
    osgDB::ifstream *   mapFile = 0;
    U32	ldctr = 0;
	U32 i = 0;
	U8	flag = 0;
	const char * fname = file.c_str();
    // Remember the map name
    //map_name = getStrippedName(file);
	
    mapFile = new osgDB::ifstream(fname, std::ios::binary);
	if (!mapFile)
        return false;
	
	osg::notify(DEBUG_INFO) << "Reading " << file << " from " << osgDB::getFilePath(file) << " from " << osgDB::getFilePath(osgDB::getFilePath(file)) << std::endl;
	mapFile->read((S8*)&i, sizeof(U32));
	convertLEndianToHost(&i);
	dif_data->iRFV = i;
	osg::notify(DEBUG_INFO) << "File Resource Version " << i << std::endl;
	if(dif_data->iRFV != IRFV){
		return false;
	}
	dif_data->setFilename(file);
	mapFile->read((S8*)&flag,sizeof(U8));
	dif_data->previewIncluded = flag;
	if(flag && !(dif_data->previewBitmap = readImage(mapFile)).valid()){
		osg::notify(FATAL) << "We have a PNG preview - This is experimental: please don't eat us, but it failed" << std::endl;
		exit(-1);
	}
	mapFile->read((S8*)&i, sizeof(U32));
	convertLEndianToHost(&i);
	dif_data->lod_count = i;
	dif_data->lod_list.reserve(dif_data->lod_count);
	osg::notify(DEBUG_INFO) << "Reading " << dif_data->lod_count << " Interior LODs" << std::endl;
	for(ldctr = 0; ldctr < dif_data->lod_count; ldctr++){
		DIFInteriorObj * curLevel = new DIFInteriorObj(file);
		
		if(readDIO(mapFile, curLevel)){
			dif_data->lod_list.push_back(curLevel);
		} else{
			osg::notify(FATAL) << "Reading of detail level " << ldctr << " failed" << std::endl;
			return false;
		}
	}
	
	osg::notify(DEBUG_INFO) << std::endl << std::endl;
	
	mapFile->read((S8*)&i, sizeof(U32));
	convertLEndianToHost(&i);
	dif_data->subs_list.reserve(i);
	osg::notify(DEBUG_INFO) << "Reading " << i << " sub-interiors" <<  std::endl;
	for(ldctr = 0; ldctr < i; ldctr++){
		DIFInteriorObj * curLevel = new DIFInteriorObj(file);
		
		if(readDIO(mapFile, curLevel)){
			dif_data->subs_list.push_back(curLevel);
		} else{
			osg::notify(FATAL) << "Reading of sub-interior " << ldctr << " failed" << std::endl;
			return false;	
		}
	}
	mapFile->close();

	buildInteriorResource();
	
	return true;
	
	/*
	 
	 return true;
	 //*/
	
	/*
	 // None of this needs to be implemented for now. Keep it as a reference.
	
	// Triggers
	U32 numTriggers;
	stream.read(&numTriggers);
	mTriggers.setSize(numTriggers);
	for (i = 0; i < mTriggers.size(); i++)
		mTriggers[i] = NULL;
	
	for (i = 0; i < mTriggers.size(); i++) {
		mTriggers[i] = new InteriorResTrigger;
		if (mTriggers[i]->read(stream) == false) {
			AssertISV(false, avar("Unable to read trigger %d in interior resource", i));
			return false;
		}
	}
	
	U32 numChildren;
	stream.read(&numChildren);
	mInteriorPathFollowers.setSize(numChildren);
	for (i = 0; i < mInteriorPathFollowers.size(); i++)
		mInteriorPathFollowers[i] = NULL;
	
	for (i = 0; i < mInteriorPathFollowers.size(); i++) {
		mInteriorPathFollowers[i] = new InteriorPathFollower;
		if (mInteriorPathFollowers[i]->read(stream) == false) {
			AssertISV(false, avar("Unable to read child %d in interior resource", i));
			return false;
		}
	}
	
	U32 numFields;
	stream.read(&numFields);
	mForceFields.setSize(numFields);
	for (i = 0; i < mForceFields.size(); i++)
		mForceFields[i] = NULL;
	
	for (i = 0; i < mForceFields.size(); i++) {
		mForceFields[i] = new ForceField;
		if (mForceFields[i]->read(stream) == false) {
			AssertISV(false, avar("Unable to read field %d in interior resource", i));
			return false;
		}
	}
	
	U32 numSpecNodes;
	stream.read(&numSpecNodes);
	mAISpecialNodes.setSize(numSpecNodes);
	for (i = 0; i < mAISpecialNodes.size(); i++)
		mAISpecialNodes[i] = NULL;
	
	for (i = 0; i < mAISpecialNodes.size(); i++) {
		mAISpecialNodes[i] = new AISpecialNode;
		if (mAISpecialNodes[i]->read(stream) == false) {
			AssertISV(false, avar("Unable to read SpecNode %d in interior resource", i));
			return false;
		}
	}
	
	U32 dummyInt;
	stream.read(&dummyInt);
	if (dummyInt == 1)
	{
		if (mDetailLevels.size() != 0)
			getDetailLevel(0)->readVehicleCollision(stream);
	}
	
	// For expansion purposes
	stream.read(&dummyInt);
	if(dummyInt == 2)
	{
		U32 numGameEnts;
		stream.read(&numGameEnts);
		mGameEntities.setSize(numGameEnts);
		for (i = 0; i < numGameEnts; i++)
			mGameEntities[i] = new ItrGameEntity;
		
		for (i = 0; i < numGameEnts; i++) {
			if (mGameEntities[i]->read(stream) == false) {
				AssertISV(false, avar("Unable to read SpecNode %d in interior resource", i));
				return false;
			}
		}
		stream.read(&dummyInt);
	}
	//*/
}

osg::Node* DIFReader::junkData(){
	osg::Geode* pyramidGeode = new osg::Geode();
	osg::Geometry* pyramidGeometry = new osg::Geometry();
	pyramidGeode->addDrawable(pyramidGeometry); 
	
	// Specify the vertices:
	osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
	pyramidVertices->push_back( osg::Vec3(0, 0, 0) ); // front left 
	pyramidVertices->push_back( osg::Vec3(2, 0, 0) ); // front right 
	pyramidVertices->push_back( osg::Vec3(2, 2, 0) ); // back right 
	pyramidVertices->push_back( osg::Vec3( 0,2, 0) ); // back left 
	pyramidVertices->push_back( osg::Vec3( 1, 1,2) ); // peak
	
	// Associate this set of vertices with the geometry associated with the 
	// geode we added to the scene.
	pyramidGeometry->setVertexArray( pyramidVertices );
	
	// Create a QUAD primitive for the base by specifying the 
	// vertices from our vertex list that make up this QUAD:
	osg::DrawElementsUInt* pyramidBase = 
	new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	pyramidBase->push_back(3);
	pyramidBase->push_back(2);
	pyramidBase->push_back(1);
	pyramidBase->push_back(0);
	
	osg::DrawElementsUInt* pyramidFaceOne = 
	new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceOne->push_back(0);
	pyramidFaceOne->push_back(1);
	pyramidFaceOne->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceOne);
	
	osg::DrawElementsUInt* pyramidFaceTwo = 
	new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceTwo->push_back(1);
	pyramidFaceTwo->push_back(2);
	pyramidFaceTwo->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceTwo);
	
	osg::DrawElementsUInt* pyramidFaceThree = 
	new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceThree->push_back(2);
	pyramidFaceThree->push_back(3);
	pyramidFaceThree->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceThree);
	
	osg::DrawElementsUInt* pyramidFaceFour = 
	new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceFour->push_back(3);
	pyramidFaceFour->push_back(0);
	pyramidFaceFour->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceFour);
	
	
	// Add this primitive to the geometry: pyramidGeometry->addPrimitiveSet(pyramidBase);
	// code to create other faces goes here!
	// (removed to save space, see tutorial two)
	
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 0 red
	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) ); //index 1 green
	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) ); //index 2 blue
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 3 white
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 4 red
	
	pyramidGeometry->setColorArray(colors);
	pyramidGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	
	
	
	return pyramidGeode;
}

InteriorSubObject::~InteriorSubObject(){}

bool InteriorSubObject::read(osgDB::ifstream * mapFile){
		osg::notify(FATAL) << "Unknown subobject or subobject key" << std::endl;
	return false;
}

bool MirrorSubObject::read(osgDB::ifstream * mapFile){
	mapFile->read((S8*)&detailLevel, sizeof(U32));
	convertLEndianToHost(&detailLevel);
	mapFile->read((S8*)&zone, sizeof(U32));
	convertLEndianToHost(&zone);
	mapFile->read((S8*)&alphaLevel, sizeof(F32));
	convertLEndianToHost(&alphaLevel);
	mapFile->read((S8*)&surfaceCount, sizeof(U32));
	convertLEndianToHost(&surfaceCount);
	mapFile->read((S8*)&surfaceStart, sizeof(U32));
	convertLEndianToHost(&surfaceStart);
	
	F32 x,y,z;
	mapFile->read((S8*)&x, sizeof(F32));
	convertLEndianToHost(&x);
	mapFile->read((S8*)&y, sizeof(F32));
	convertLEndianToHost(&y);
	mapFile->read((S8*)&z, sizeof(F32));
	convertLEndianToHost(&z);
	centroid.set(x,y,z);
	return true;
}