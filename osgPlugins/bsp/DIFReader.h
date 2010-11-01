/*
 *  DIFReader.h
 *  OpenSceneGraph
 *
 *  Created by Thomas Dickerson on 9/28/10.
 *  Copyright 2010 Vermont Sustainable Heating Initiative. All rights reserved.
 *
 */

#include <osg/Array>
#include <osg/Node>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/Endian>
#include <osg/TexGen>
#include "BSPReader.h"
#include <osg/Notify>

#ifndef _DIFREADER_H_
#define _DIFREADER_H_

#define BINARY_FILE_VERSION 1
#define IRFV 44
#define IFV 14
#define NUMCOORDBINS 16

#define CSM_3S_VERSION 25
#define F32_MIN F32(1.175494351e-38F)                 ///< Constant Min Limit F32
#define F32_MAX F32(3.402823466e+38F)
#define MIN_PIXELS 6
#define LIGHTING_OVERBRIGHT_AMOUNT 2.0

//USE_OSGPLUGIN(png) // QT implementation is broken

namespace bsp
{
	
	class DIFInteriorObj;
	typedef std::vector<DIFInteriorObj*> DIOList;
	typedef std::vector<osg::Plane> PlaneList;
	typedef std::vector<osg::Vec3f> Vec3fList;
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
	
	inline std::string difFindTexture(std::string location, char * tn){
		//try the following
		std::string texName(tn);
		U32 texLN = strlen(tn);
		char * buf = new char[texLN + 5];
		char * ext = &buf[texLN];
		strcpy(buf, tn);
		ext[4] = '0';
		std::string dir = osgDB::getFilePath(location);
		const char* extArray[4] = { ".jpg", ".png", ".gif", ".bmp" };
		U32 i,j;
		for(i = 0; i < 4; i++){
			strcpy(ext,extArray[i]);
			std::string find(buf);
			std::string found("");
			osgDB::DirectoryContents dc = osgDB::getDirectoryContents(dir);
			for(j = 0; j < dc.size(); j++){
				std::string fun = dc[j];
				U32 res = strcasecmp(fun.c_str(),find.c_str());
				if(!res){
					found = dc[j];
					break;
				}
			}
			//std::string found = osgDB::findFileInDirectory(find, dir, osgDB::CASE_INSENSITIVE);
			if (found.size()){
				delete buf;
				return osgDB::concatPaths(dir, found);
			}
			
		}
		
		
		if(dir.length()) return difFindTexture(dir, tn);
		else return std::string("");
	}
	
	class InteriorSubObject{
	public:
		enum SubObjectKeys {
			TranslucentSubObjectKey = 0,
			MirrorSubObjectKey      = 1
		};
		
		virtual bool read(osgDB::ifstream * mapFile);
		
		virtual ~InteriorSubObject();
	};
	
	class MirrorSubObject : public InteriorSubObject {
	public:
		
		U32   detailLevel;
		U32   zone;
		
		F32     alphaLevel;
		osg::Vec3f centroid;
		
		U32   surfaceCount;
		U32   surfaceStart;
		
		bool read(osgDB::ifstream * mapFile);
	};
	
	struct ItrPaddedPoint
	{
		osg::Vec3f point;
		union 
		{
			F32   fogCoord;
			U8    fogColor[4];
		};
	};
	
	//------------------------------------------------------------------------------
	//-------------------------------------- CLASS NOTES
	// Interior: Base for all interior geometries.  Contains all lighting, poly,
	//             portal zone, bsp info, etc. to render an interior.
	//
	// Internal Structure Notes:
	//    IBSPNode:
	//     planeIndex: Obv.
	//     frontIndex/backIndex: Top bit indicates if children are leaves.
	//                            Next bit indicates if leaf children are solid.
	//
	//    IBSPLeafSolid:
	//     planeIndex: obv.
	//     surfaceIndex/surfaceCount: Polys that are on the faces of this leaf.  Only
	//                                 used for collision/surface info detection.
	//
	//------------------------------------------------------------------------------
	
	
	typedef std::vector<ItrPaddedPoint> PointList;
	typedef std::vector<U8> PointVisibilityList;
	
	struct DTexGen{
		osg::Vec4f xPlane;
		osg::Vec4f yPlane;
	};
	
	//typedef std::vector<osg::ref_ptr<osg::TexGen> > TexGenList;
	typedef std::vector<DTexGen> TexGenList;
	
	class DIFInteriorObj{	// Interior
	public:
		
		DIFInteriorObj(const std::string fname){
			filename = new std::string(fname);
		}
		
		~DIFInteriorObj(){
			if(filename) delete filename;
		}
		
		osg::Node * buildInteriorNode();
		void setupZonePlanes();
		void buildSurfaceZones();
		bool getUnifiedZone(const U32 index, S32* zone);
		void truncateZoneNode(const U32 index);
		void truncateZoneTree();
		
		inline U32 getI(U32 des, U32 minML, U32 index){
			return (des == minML) ? index % minML : index / minML;
		}
		
		inline bool isBSPLeafIndex(U32 index) const
		{
			return (iFV >= 14) ? (index & 0x80000) != 0 : (index & 0x8000) != 0;
		}
		
		inline bool isBSPSolidLeaf(U32 index) const
		{
			if(!isBSPLeafIndex(index)) osg::notify(osg::WARN) << "Error(isBSPSolidLeaf), only call for leaves!" << std::endl;
			return (iFV >= 14) ? (index & 0x40000) != 0 : (index & 0x4000) != 0;
		}
		
		inline bool isBSPEmptyLeaf(U32 index) const
		{
			if(!isBSPLeafIndex(index)) osg::notify(osg::WARN) << "Error(isBSPEmptyLeaf), only call for leaves!" << std::endl;
			return (iFV >= 14) ? !(index & 0x40000) : !(index & 0x4000);
		}
		
		inline U16 getBSPSolidLeafIndex(U32 index) const
		{
			if(!isBSPLeafIndex(index)) osg::notify(osg::WARN) << "Error(getBSPSolidLeafIndex), only call for leaves!" << std::endl;
			return (iFV >= 14) ? U16(index & ~0xC0000) : U16(index & ~0xC000);
		}
		
		inline U16 getBSPEmptyLeafZone(U32 index) const
		{
			if(!isBSPLeafIndex(index)) osg::notify(osg::WARN) << "Error(getBSPEmptyLeafZone), only call for leaves!" << std::endl;
			return (iFV >= 14) ? U16(index & ~0xC0000) : U16(index & ~0xC000);
		}
		
		inline const osg::Vec3f getPointNormal(const U32 surfaceIndex, const U32 pointOffset) const
		{
			Surface rSurface = surfaces[surfaceIndex];
			osg::Vec3f normal(0.0f, 0.0f, 0.0f);
			
			if (iFV >= 11)
			{
				U32 texMatIndex = texMatIndices[rSurface.windingStart + pointOffset];
				TexMatrix texMat = texMatrices[texMatIndex];
				
				if (texMat.N > -1)
					normal = normals[texMat.N];
			}
			else{
				osg::Plane pNorm(planeList[rSurface.planeIndex & ~0x8000]);
				if(rSurface.planeIndex & 0x8000) pNorm.flip();
				normal.set(pNorm.getNormal());
			}
			return normal;
		}
		
		struct MaterialList{
			std::vector<char*> materialNames;
			std::vector<osg::ref_ptr<osg::Texture2D> > materials; //osgPathHandler may be useful
		};
		
		enum
		{
			S_Wrap             = 1,
			T_Wrap             = 2,
			Translucent        = 4,
			Additive           = 8,
			Subtractive        = 16,
			SelfIlluminating   = 32,
			NeverEnvMap        = 64,
			NoMipMap           = 128,
			MipMap_ZeroBorder  = 256,
			IflMaterial        = 1 << 27,
			IflFrame           = 1 << 28,
			DetailMapOnly      = 1 << 29,
			BumpMapOnly        = 1 << 30,
			ReflectanceMapOnly = 1 << 31,
			AuxiliaryMap       = DetailMapOnly | BumpMapOnly | ReflectanceMapOnly
		};
		
		struct CSMaterialList{
			MaterialList materials;
			std::vector<U32> flags;
			// Additional textures
			std::vector<U32> reflectanceMaps;
			std::vector<U32> bumpMaps;
			std::vector<U32> detailMaps;
			std::vector<U32> lightMaps;
			// Additional texture info
			std::vector<F32> detailScales;
			std::vector<F32> reflectionAmounts;
			
			U8 namesTransformed;
		};
		
		struct CSMPrimitive{
			U8 alpha;
			U32 texS;
			U32 texT;
			S32 diffuseIndex;
			S32 lightMapIndex;
			U32 start;
			U32 count;
			
			// used to relight the surface in-engine...
			//osg::ref_ptr<osg::TexGen> lightMapEquation;
			DTexGen lightMapEquation;
			S32 lightMapOffsetX; //Stand-ins for Point2I types.
			S32 lightMapOffsetY;
			S32 lightMapSizeX;
			S32 lightMapSizeY;
		};
		
		struct CSMesh{
			U8 hasSolid;
			U8 hasTranslucency;
			U8 hasMaterialList;
			osg::BoundingBoxf bounds;
			osg::Matrixf transform;
			osg::Vec3f scale;
			
			std::vector<CSMPrimitive> primitives;
			
			// same index relationship...
			std::vector<U16> indices;
			std::vector<osg::Vec3f> verts;
			std::vector<osg::Vec3f> norms;
			std::vector<osg::Vec2f> diffuseUVs;
			std::vector<osg::Vec2f> lightmapUVs;
			
			CSMaterialList csMaterialList;
			std::vector<U8>	embedFlags;
			std::vector<osg::ref_ptr<osg::Image> >	embeddedMats;
		};
		
		struct IBSPNode 
		{
			U16 planeIndex;
			U32 frontIndex;
			U32 backIndex;
			
			U16 terminalZone;   // if high bit set, then the lower 15 bits are the zone
			//  of any of the subsidiary nodes.  Note that this is
			//  going to overestimate some, since an object could be
			//  completely contained in solid, but it's probably
			//  going to turn out alright.
		};
		
		struct IBSPLeafSolid 
		{
			U32 surfaceIndex;
			U16 surfaceCount;
		};
		
		struct TriFan 
		{
			U32 windingStart;
			U32 windingCount;
		};
		
		struct Surface 
		{
			U32 windingStart;          // 1
			
			U16 planeIndex;            // 2
			U16 textureIndex;
			
			U32 texGenIndex;           // 3
			
			U16 lightCount;            // 4
			U8  surfaceFlags;
			U32 windingCount;
			
			U32 fanMask;               // 5
			
			U32 lightStateInfoStart;   // 6
			
			U32 mapOffsetX;            // 7
			U32 mapOffsetY;
			U32 mapSizeX;
			U32 mapSizeY;
			U8 unused;
		};
		
		struct NullSurface 
		{
			U32 windingStart;
			
			U16 planeIndex;
			U8  surfaceFlags;
			U32 windingCount;
		};
		
		struct Edge
		{
			S32 vertexes[2];
			S32 faces[2];
		};
		
		struct Zone 
		{
			U16 portalStart;
			U16 portalCount;
			
			U32 surfaceStart;
			U32 planeStart;
			
			U16 surfaceCount;
			U16 planeCount;
			
			U32 staticMeshStart;
			U32 staticMeshCount;
			
			U16 flags;
			U16 zoneId;       // This is ephemeral, not persisted out.
		};
		
		struct Portal 
		{
			U16 planeIndex;
			
			U16 triFanCount;
			U32 triFanStart;    // portals can have multiple windings
			
			U16 zoneFront;
			U16 zoneBack;
		};
		
		enum LightFlags 
		{
			AnimationAmbient  = 1,
			AnimationLoop     = 2,
			AnimationFlicker  = 4,
			AnimationTypeMask = 7,
			
			AlarmLight        = 8
		};
		
		enum LightType 
		{
			AmbientLooping     = AnimationAmbient | AnimationLoop,
			AmbientFlicker     = AnimationAmbient | AnimationFlicker,
			
			TriggerableLoop    = AnimationLoop,
			TriggerableFlicker = AnimationFlicker,
			TriggerableRamp    = 0
		};
		
		struct AnimatedLight 
		{
			U32 nameIndex;   ///< Light's name
			U32 stateIndex;  ///< start point in the state list
			
			U16 stateCount;  ///< number of states in this light
			U16 flags;       ///< flags (Apply AnimationTypeMask to get type)
			
			U32 duration;    ///< total duration of animation (ms)
		};
		
		struct LightState 
		{
			U8  red;                ///< state's color
			U8  green;
			U8  blue;
			U8  _color_padding_;
			
			U32 activeTime;         ///< Time (ms) at which this state becomes active
			
			U32 dataIndex;          ///< StateData count and index for this state
			U16 dataCount;
			
			U16 __32bit_padding__;
		};
		
		struct LightStateData 
		{
			U32   surfaceIndex;     ///< Surface affected by this data
			U32   mapIndex;         ///< Index into StateDataBuffer (0xFFFFFFFF indicates none)
			U16   lightStateIndex;  ///< Entry to modify in InteriorInstance
			U16   __32bit_padding__;
		};
		
		struct ConvexHull 
		{
			F32   minX;
			F32   maxX;
			F32   minY;
			F32   maxY;
			
			F32   minZ;
			F32   maxZ;
			U32   hullStart;
			U32   surfaceStart;
			
			U32   planeStart;
			U16   hullCount;
			U16   surfaceCount;
			U32   polyListPlaneStart;
			
			U32   polyListPointStart;
			U32   polyListStringStart;
			U16   searchTag;
			U8  staticMesh;
		};
		
		struct CoordBin 
		{
			U32   binStart;
			U32   binCount;
		};
		
		struct TexMatrix
		{
			S32 T;
			S32 N;
			S32 B;
			
			
		};
		
		typedef std::vector<CSMesh> CSMeshList;
		typedef std::vector<IBSPNode> IBSPNodeList;
		typedef std::vector<IBSPLeafSolid> IBSPLeafSolidList;
		typedef std::vector<TriFan> TriFanList;
		typedef std::vector<Surface> SurfaceList;
		typedef std::vector<NullSurface> NullSurfaceList;
		typedef std::vector<Zone> ZoneList;
		typedef std::vector<Portal> PortalList;
		typedef std::vector<AnimatedLight> AnimatedLightList;
		typedef std::vector<LightState> LightStateList;
		typedef std::vector<LightStateData> LightStateDataList;
		typedef std::vector<InteriorSubObject*> ISOList;
		typedef std::vector<ConvexHull> ConvexHullList;
		
		U32 iFV;
		U32 detailLevel;
		U32 minPixels;
		std::string * filename;
		
		osg::BoundingBoxf bBox;
		osg::BoundingSpheref bSphere;
		
		U8 hasAlarmState;		// Deprecated
		U32 numLightStateEntries;
		PlaneList planeList;
		PointList pointList;
		PointVisibilityList pointVisList;
		TexGenList texGenEQs;
		TexGenList lmTexGenEQs;
		
		IBSPNodeList iBSPNodes;
		IBSPLeafSolidList iBSPSolidLeaves;
		MaterialList materialList;
		
		std::vector<U32> windings;
		std::vector<Edge> edgeList;
		
		TriFanList windingIndices;
		SurfaceList surfaces;
		NullSurfaceList nullSurfaces;
		ZoneList	zones;
		PortalList portals;
		
		std::vector<U16> zoneSurfaces;
		std::vector<U32> zoneStaticMeshes;
		std::vector<U16> zonePortalList;
		std::vector<S32> surfaceZone;
		
		std::vector<U32> normalLMapIndices;
		std::vector<U32> alarmLMapIndices;
		
		std::vector<osg::ref_ptr<osg::Image> >  lightDirMaps;
		std::vector<osg::ref_ptr<osg::Image> >	lightmaps;
		std::vector<U8>	lightmapKeep; // really a bool
		
		
		std::vector<U32> solidLeafSurfaces; 
		
		AnimatedLightList animatedLights;
		U32 numTriggerableLights;
		LightStateList lightStates;
		LightStateDataList lightStateData;
		std::vector<U8> lightStateDataBuffer;
		
		ISOList subObjects;
		ConvexHullList convexHulls;
		std::vector<U8>              convexHullEmitStrings;
		std::vector<U32>             hullIndices;
		std::vector<U32>             hullEmitStringIndices;
		std::vector<U32>             hullSurfaceIndices;
		std::vector<U16>             hullPlaneIndices;
		std::vector<U16>             polyListPlanes;
		std::vector<U32>             polyListPoints;
		std::vector<U8>              polyListStrings;
		
		CoordBin	coordBins[NUMCOORDBINS*NUMCOORDBINS];
		
		std::vector<U16>             coordBinIndices;
		U32                     coordBinMode;
		
		osg::Vec4f baseAmbient;		//rgba
		osg::Vec4f alarmAmbient;	//rgba
		
		CSMeshList csMeshes;
		std::vector<osg::Vec3f> normals;
		std::vector<TexMatrix> texMatrices;
		std::vector<U32> texMatIndices;
		
		U32 lightmapBorderSize;
		
		std::vector<U16> zonePlanes;
	};
	
	class DIFInteriorFile{	// InteriorResource
	public:
		U32 iRFV;
		U8 previewIncluded;
		osg::ref_ptr<osg::Image> previewBitmap;
		U32 lod_count;
		std::string * filename;
		DIOList lod_list;
		DIOList subs_list;
		inline void setFilename(const std::string fname){
			filename = new std::string(fname);
		};
		
		DIFInteriorFile(){
			filename = NULL;
		}
		~DIFInteriorFile(){	if(filename) delete filename; }
	};
	
	typedef DIFInteriorObj::MaterialList MaterialList;
	typedef DIFInteriorObj::CSMaterialList CSMaterialList;
	
	class DIFReader : public BSPReader
	{
		friend class DIFInteriorObj;
		friend class DIFInteriorFile;
	protected:
		DIFInteriorFile * dif_data;
		osg::ref_ptr<osgDB::ReaderWriter> reader;
		
		osg::Vec3f takeMin(osg::Vec3f vec1, osg::Vec3f vec2);
		osg::Vec3f takeMax(osg::Vec3f vec1, osg::Vec3f vec2);
		void readVec(osgDB::ifstream * instream, osg::Vec2f * vec);
		void readVec(osgDB::ifstream * instream, osg::Vec3f * vec);
		void readVec(osgDB::ifstream * instream, osg::Vec4f * vec);
		void readColor(osgDB::ifstream * inputStream, osg::Vec4f * col);
		
		void readMatList(osgDB::ifstream * inputStream, MaterialList * matList);
		void readMatListText(osgDB::ifstream * inputStream, MaterialList * matList, U8 firstByte);
		void readMatListText(osgDB::ifstream * inputStream, MaterialList * matList);
		void readCSMatList(osgDB::ifstream * inputStream, CSMaterialList * csMatList);
		
		void readLine(osgDB::ifstream * instream, U8 *buffer, U32 bufferSize);
		void readString(osgDB::ifstream * instream, S8 stringBuf[256]);
		osg::ref_ptr<osg::Image> readImage(osgDB::ifstream * mapFile);
		void readCSMesh(osgDB::ifstream * mapFile, DIFInteriorObj::CSMesh * csm);
		bool readDIO(osgDB::ifstream * mapFile, DIFInteriorObj * cl);
		//void readLMapTexGen(osgDB::ifstream * inputStream, osg::Plane * planeX, osg::Plane * planeY);
		void readLMapTexGen(osgDB::ifstream * inputStream, DTexGen * texGen);
		void buildInteriorResource();
		static osg::Node* junkData();
		
	public:
		DIFReader();
		virtual ~DIFReader();
		
		bool   readFile(const std::string & file,
						const osgDB::ReaderWriter::Options* options);
	};
	
}


#endif