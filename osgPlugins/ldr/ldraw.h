/*
 *  ldraw.h
 *  torque_xcode_2_2_UB
 *
 *  Created by Thomas Dickerson on 1/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _LDRAW_H_
#define _LDRAW_H_

#define MAX_INCLUDE_DEPTH 64
#define MAX_TRANSFORM_DEPTH 128

#define TYPE_P 1
#define TYPE_PART 2
#define TYPE_MODEL 4
#define TYPE_OTHER 8

#define TYPE_F_NORMAL 		0x0000
#define TYPE_F_LOW_RES 		0x0001
#define TYPE_F_NO_POLYGONS 	0x0002
#define TYPE_F_NO_LINES 	0x0004
#define TYPE_F_BBOX_MODE 	0x0008
#define TYPE_F_INVISIBLE 	(TYPE_F_NO_POLYGONS | TYPE_F_NO_LINES)
#define TYPE_F_STUDLESS_MODE 	0x0010
#define TYPE_F_STUDONLY_MODE 	0x0020
#define TYPE_F_STUDLINE_MODE 	0x0040
#define TYPE_F_SHADED_MODE 	0x0080
#define TYPE_F_XOR_MODE 	0x0100
#define TYPE_F_XOR_PRIMITIVE 	0x0200

#ifndef M_PI
# define M_PI       3.14159265358979323846  /* pi */
#endif

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

/*#include "platform/platform.h"
#include "core/resManager.h"
#include "core/stringTable.h"
#include "console/console.h"
#include "math/mMath.h"
#include "math/mMatrix.h"
#include "math/mQuat.h"
#include "math/mathUtils.h"
*/
#include "ldrawcolors.h"
#include <osg/Array>
#include <osg/Node>
#include <osg/Texture2D>

#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

#include <osgDB/Registry>
#include <osgDB/FileNameUtils>

#include <iostream>

//do not include this ANYWHERE else.
#ifndef _LDTAB_H_
#define _LDTAB_H_
#include "y.tab.h"
#endif

namespace LDParse{
	
	typedef struct {
		S32 cached_files;
		S32 uncached_files;
		S32 cache_hits;
	} LDLITE_PROFILE;
	static LDLITE_PROFILE ldlite_profile;
	
	void pop_transform();
	inline void zStep(S32 step, S32 pause); // we have no interest in implementing this
	inline void zPause();					// just yet, but no reason to ditch it either
											// until we're sure
	
	inline void zWrite(char *message);
	inline void zClear();					// Not quite sure what to do with this, since we
											// don't render directly
	inline void zSave(S32 step);
	
	S32 LDyyparse(void);
	S32 yylex(void);
	S32 start_include_file(char * root_name);
	S32 stop_include_file(void);
	S32 defer_stop_include_file(void);
	S32 cache_mpd_subfiles(char *mpd_subfile_name);
	std::string findLDrawFile(std::string file);
	char * bufferFile(std::string filename);
	void checkLDrawDirectory();
	
	static S32 current_type[MAX_INCLUDE_DEPTH];
	
	// caching lex defines
	#define MAX_CACHED_TOKENS (16*1024)
	#define MAX_CACHED_FILES  (24*128)
	
	// bitfield
	#define CHS_UNUSED  0
	#define CHS_FILLING 1
	#define CHS_FILLED  2
	#define CHS_PROBLEM 4
	
	
	typedef struct {
		S32 valid; // one of the CHS_X defines
		char filename[256];
		S32 ftype; // one of the TYPE_X defines
		//S32 max_tokens;
		U32 max_tokens;
		//S32 next_token_index;  // points to the next token to be read/written
		U32 next_token_index;
		std::vector<S32>    tokens;
		std::vector<YYSTYPE> values;
	} CACHED_STREAM;
	
	// This is the heap of cached files
	static CACHED_STREAM cached_streams[MAX_CACHED_FILES];
	
	static CACHED_STREAM *cached_file_stack[MAX_INCLUDE_DEPTH];
	static S32 cached_file_stack_index=0;  // points to unused entry

	extern osg::Matrixf *current_transform[MAX_INCLUDE_DEPTH];
	extern S32 include_stack_ptr;
	extern S32 transform_stack_ptr;
	
	static S32 deferred_flag[MAX_INCLUDE_DEPTH];
	
	osg::Matrixf* copymat(osg::Matrixf *mat);
	osg::Matrixf* savemat(F32 x, F32 y, F32 z, 
					  F32 a, F32 b, F32 c,
					  F32 d, F32 e, F32 f,
					  F32 g, F32 h, F32 i);
	
	
	osg::Vec3f * savept(F32 x, F32 y, F32 z);
	osg::Vec3f * copypt(osg::Vec3f * pt);
	S32 initParse(const char* filename);
	
	void transform_mat_inplace(osg::Matrixf* m);
	S32 print_transform(osg::Matrixf* m);
	S32 push_rotation(osg::Vec3f* axis, F64 degrees);
	S32 push_transform(osg::Matrixf* m);
	
	
	extern void znamelist_pop();
	extern void znamelist_push();
	extern void zcolor_modify(S32 index, char * name, S32 inverse_index, S32 p_r, S32 p_g, S32 p_b, S32 p_a, S32 d_r, S32 d_g, S32 d_b, S32 d_a);
	extern void zcolor_alias(S32 index, char * name);
	extern void zpoint_alias(char * name, osg::Vec3f * point);
	extern void zmatrix_alias(char * name, osg::Matrixf * matrix);
	extern S32 edge_color(S32 c);
	extern S32 zcolor_lookup(char * name);
	extern osg::Matrixf* zmatrix_lookup(char * name);
	extern osg::Vec3f* zpoint_lookup(char * name);
	inline char* dStrdup(char * s){
		char * sc = new char[strlen(s)+1];
		strcpy(sc, s);
		return sc;
	}
	
#define AssertFatal(x, y)         \
{ if (!x) \
{ std::cerr << y << std::endl; exit(-1); } }

	
};

#endif