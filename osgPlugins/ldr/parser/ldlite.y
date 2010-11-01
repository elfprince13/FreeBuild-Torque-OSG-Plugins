/*
 * Grammer for LDRAW
 */

%{
	
#define yyparse LDParse::LDyyparse
	
#include <stdio.h>
#include <stdlib.h>
#include "ldraw.h"
	
#undef NO_LINES
	
	using namespace LDParse;
	
	char yyfilename[256];			/* Input filename */
	char *ldraw_model=NULL;
	int tmp_i;
	void yyerror ( char * );
	int stepcount=0;
	char *mpd_subfile_name;
	
	// we don't use this anymore---Torque combines translation and transformation
	// into a single 4x4 matrix
	
	/* x y z is the x y z coordinate of the part
	 * a b c d e f g h i is a top left 3x3 matrix of a standard 4x4 homogeneous
	 * transformation matrix. This represents the rotation and scaling of the
	 * part. The entire 4x4 3D transformation matrix would then take either of
	 * the following forms:
	 *	 / a d g 0 \   / a b c x \
	 *	 | b e h 0 |   | d e f y |
	 *	 | c f i 0 |   | g h i z |
	 *	 \ x y z 1 /   \ 0 0 0 1 /
	 * The above two forms are essentially equivalent, but note the location of
	 * the transformation portion (x, y, z) relative to the other terms.
	 */
	
	//MatrixF *current_transform[MAX_INCLUDE_DEPTH];
	
	//vector3d *current_translation[MAX_INCLUDE_DEPTH];
	//matrix3d *current_transform[MAX_INCLUDE_DEPTH];
	
	S32 current_color[MAX_INCLUDE_DEPTH];
	//S32 current_type[MAX_INCLUDE_DEPTH];
	S32 is_current_file_not_cached(void);
	
	
	%}

%union {
	char *c;
	S32 i;
	F64 d;
	osg::Vec3f *v;
	osg::Matrixf *m;
}

%token <c> tIDENT
%token <c> tGARBAGE
%token <i> tINT
%token <d> tFLOAT
%token <i> tZERO
%token <i> tONE
%token <i> tTWO
%token <i> tTHREE
%token <i> tFOUR
%token <i> tFIVE
%token <c> tSTEP
%token <c> tPAUSE
%token <c> tWRITE
%token <c> tCLEAR
%token <c> tSAVE
%token <c> tEOL
%token <c> tEOF
%token <c> tROTATE
%token <c> tTRANSLATE
%token <c> tEND
%token <c> tSCALE
%token <c> tTRANSFORM
%token <c> tCOLOR
%token <c> tALIAS
%token <c> tPOINT
%token <c> tMATRIX
%token <c> tFILE

%type <c> PartName
%type <d> Number
%type <c> Words
%type <c> Word
%type <i> Color
%type <v> Position
%type <m> TransMatrix
%%

LdrawModel	: LdrawLines
;

LdrawLines	: /* empty */
| LdrawLines LdrawLine
;

LdrawLine	: StepLine
| PauseLine
| WriteLine
| ClearLine
| SaveLine
| ObjectLine
| LineLine
| TriLine
| QuadLine
| FiveLine
| TranslateStartLine LdrawLines TranslateEndLine
| RotateStartLine LdrawLines RotateEndLine
| ScaleStartLine LdrawLines ScaleEndLine
| TransformStartLine LdrawLines TransformEndLine
| ColorLine
| ColorAliasLine
| PointAliasLine
| MatrixAliasLine
| MPDFileLine
| BlankLine
| CommentLine
| error tEOL
| error tEOF
;

Number		: tZERO
{
	$$ = 0.0;
}
| tONE
{
	$$ = 1.0;
}
| tTWO
{
	$$ = 2.0;
}
| tTHREE
{
	$$ = 3.0;
}
| tFOUR
{
	$$ = 4.0;
}
| tFIVE
{
	$$ = 5.0;
}
| tINT
{
	$$ = 1.0 * ($1);
	
}
| tFLOAT
{
	$$ =1.0 * ($1);
}
;

StepLine	: tZERO tSTEP Words tEOL
{
	zStep(stepcount,1);
	stepcount++;
	//dFree($3);
	delete [] $3;
}
| tZERO tSTEP Words tEOF
{
	zStep(stepcount,1);
	stepcount++;
	
	//dFree($3);
	delete [] $3;
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

PauseLine	: tZERO tPAUSE Words tEOL
{
	zPause();
	
	//dFree($3);
	delete [] $3;
}
| tZERO tPAUSE Words tEOF
{
	
	zPause();
	
	//dFree($3);
	delete [] $3;
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

WriteLine	: tZERO tWRITE Words tEOL
{
	
	zWrite($3);
	
	//dFree($3);
	delete [] $3;
}
| tZERO tWRITE Words tEOF
{
	
	zWrite($3);
	//dFree($3);
	delete [] $3;
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

ClearLine	: tZERO tCLEAR Words tEOL
{
	
	zClear();
	//dFree($3);
	delete [] $3;
	
}
| tZERO tCLEAR Words tEOF
{
	zClear();
	//dFree($3);
	delete [] $3;
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

ColorLine	: tZERO tCOLOR Number tIDENT Number Number Number Number Number Number Number Number Number tEOL
{
	zcolor_modify((int)$3,$4,(int)$5,(int)$6,(int)$7,(int)$8,(int)$9,(int)$10,(int)$11,(int)$12,(int)$13);
}
| tZERO tCOLOR Number tIDENT Number Number Number Number Number Number Number Number Number tEOF
{
	zcolor_modify((int)$3,$4,(int)$5,(int)$6,(int)$7,(int)$8,(int)$9,(int)$10,(int)$11,(int)$12,(int)$13);
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

ColorAliasLine	: tZERO tALIAS tIDENT Color tEOL
{
	zcolor_alias((int)$4,$3);
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] $3;
	}
}
| tZERO tALIAS tIDENT Color tEOF
{
	zcolor_alias((int)$4,$3);
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] $3;
	}
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

PointAliasLine	: tZERO tPOINT tIDENT Position tEOL
{
	zpoint_alias($3,$4);
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] $3;
	}
}
| tZERO tPOINT tIDENT Position tEOF
{
	zpoint_alias($3,$4);
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] $3;
	}
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

MatrixAliasLine	: tZERO tMATRIX tIDENT TransMatrix tEOL
{
	zmatrix_alias($3,$4);
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] $3;
	}
}
| tZERO tMATRIX tIDENT TransMatrix tEOF
{
	zmatrix_alias($3,$4);
	if (is_current_file_not_cached()) {
		//dFree($3);
		delete [] $3;
	}
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

MPDFileLine	: tZERO tFILE tIDENT tEOL
{
	if (cache_mpd_subfiles($3) == 0) {
		mpd_subfile_name = $3;
	} else {
		//dFree($3);
		delete [] $3;
	}
	YYACCEPT;
}
| tZERO tFILE tIDENT tEOF
{
	if (cache_mpd_subfiles($3) == 0) {
		mpd_subfile_name = $3;
	} else {
		//dFree($3);
		delete [] $3;
	}
	YYACCEPT;
}
;

SaveLine	: tZERO tSAVE Words tEOL
{
	
	zSave(stepcount);
	stepcount++;
	//dFree($3);
	delete [] $3;
}
| tZERO tSAVE Words tEOF
{
	zSave(stepcount);
	stepcount++;
	//dFree($3);
	delete [] $3;
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

TranslateStartLine	: tZERO tTRANSLATE Position tEOL
{
	osg::Matrixf tmp = osg::Matrixf::identity();
	tmp.setTrans(*$3);
	push_transform(&tmp);
}
| tZERO tTRANSLATE Position tEOF
{
	osg::Matrixf tmp = osg::Matrixf::identity();
	tmp.setTrans(*$3);
	push_transform(&tmp);
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

TranslateEndLine	: tZERO tTRANSLATE tEND tEOL
{
	pop_transform();
}
| tZERO tTRANSLATE tEND tEOF
{
	pop_transform();
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

RotateStartLine	: tZERO tROTATE Number Position tEOL
{
	/* axis degree representation */
	push_rotation($4, $3);
}
| tZERO tROTATE Number Position tEOF
{
	/* axis degree representation */
	push_rotation($4, $3);
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

RotateEndLine	: tZERO tROTATE tEND tEOL
{
	pop_transform();
}
| tZERO tROTATE tEND tEOF
{
	pop_transform();
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

ScaleStartLine	: tZERO tSCALE Number tEOL
{
	push_transform(savemat(0, 0, 0, (float)($3),0,0,0,(float)($3),0,0,0,(float)($3)));
}
| tZERO tSCALE Number tEOF
{
	push_transform(savemat(0, 0, 0, (float)($3),0,0,0,(float)($3),0,0,0,(float)($3)));
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

ScaleEndLine	: tZERO tSCALE tEND tEOL
{
	pop_transform();
}
| tZERO tSCALE tEND tEOF
{
	pop_transform();
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

TransformStartLine	: tZERO tTRANSFORM TransMatrix tEOL
{
	push_transform($3);
}
| tZERO tTRANSFORM TransMatrix tEOF
{
	push_transform($3);
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

TransformEndLine	: tZERO tTRANSFORM tEND tEOL
{
	pop_transform();
}
| tZERO tTRANSFORM tEND tEOF
{
	pop_transform();
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;


Word		: tIDENT
{
	if (is_current_file_not_cached()) {
		$$ = $1;
	} else {
		$$ = new char[128];
		strcpy($$, $1);
	}
}
| tGARBAGE
{ 
	if (is_current_file_not_cached()) {
		$$ = $1;
	} else {
		$$ = new char[128];
		strcpy($$, $1);
	}
}
| Number
{ 
	
	$$ = new char[128];
	sprintf($$, "%g",$1); 
}
| tSTEP
{
	$$ = new char[128];
	strcpy($$, $1);
}
| tPAUSE
{
	$$ = new char[128];
	strcpy($$, $1);
}
| tWRITE
{
	$$ = new char[128];
	strcpy($$, $1);
}
| tCLEAR
{
	
	$$ = new char[128];
	strcpy($$, $1);
}
| tSAVE
{
	
	$$ = new char[128];
	strcpy($$, $1);
}
| tTRANSLATE
{
	
	$$ = new char[128];
	strcpy($$, $1);
}
| tROTATE
{
	
	$$ = new char[128];
	strcpy($$, $1);
}
| tEND
{
	
	$$ = new char[128];
	strcpy($$, $1);
}
| tCOLOR
{
	
	$$ = new char[128];
	strcpy($$, $1);
}
| tPOINT
{
	
	$$ = new char[128];
	strcpy($$, $1);
}
| tSCALE
{
	
	$$ = new char[128];
	strcpy($$, $1);
}
| tTRANSFORM
{
	
	$$ = new char[128];
	strcpy($$, $1);
}
| tALIAS
{
	$$ = new char[128];
	strcpy($$, $1);
}
| tMATRIX
{
	$$ = new char[128];
	strcpy($$, $1);
}
;

Words		: 
{
	$$ = new char[128];
	strcpy($$, "");
}
| Words Word
{ 
	// any Word string should be long enough hold all concatinated Words,
	// so we do not need to malloc a new string, just strcat them.
	$$ = $1;
	strcat($1, " ");
	strcat($1, $2);
	delete [] $2;
}
;

CommentLine	: tZERO Words tEOL
{
	
	//osg::notify(osg::DEBUG_INFO) << "Remark \"" << $2 << "\" at depth " << include_stack_ptr << std::endl;
	
	delete [] $2;
}
| tZERO Words tEOF
{
	//osg::notify(osg::DEBUG_INFO) << "Remark \"" << $2 <<"\" at depth " << include_stack_ptr <<std::endl;
	
	delete [] $2;
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

BlankLine	: tEOL
{
}
| tEOF
{
	if (stop_include_file() == (-1)) { YYACCEPT; };
}
;

Color		: Number
{
	if ($1 == 16.0) {
		$$ = current_color[include_stack_ptr];
	} else if ($1 == 24.0) {
		$$ = edge_color(current_color[include_stack_ptr]);
	} else {
		$$ = (int) $1;
	}
}
| tIDENT
{
	$$ = zcolor_lookup($1);
	if (is_current_file_not_cached()) {
		
		delete [] $1;
	}
}
;

TransMatrix	: Number Number Number Number Number Number Number Number Number Number Number Number
{
	$$ = savemat($1,$2,$3,$4,$5,$6,$7,$8,$9, $10, $11, $12);
}
| tIDENT
{
	$$ = zmatrix_lookup($1);
	if (is_current_file_not_cached()) {
		
		delete [] $1;
	}
}
;

Position	: Number Number Number
{
	float a, b, c;
	osg::Vec3f *p;
	a = $1;
	b = $2;
	c = $3;
	p = savept(a,b,c);
	$$ = p;
}
| tIDENT
{
	$$ = zpoint_lookup($1);
	if (is_current_file_not_cached()) {
		
		delete [] $1;
	}
}
;

PartName	: tIDENT
{
	if (is_current_file_not_cached()) {
		$$ = $1;
	} else {
		$$ = new char[128];
		strcpy($$, $1);
	}
}
;


ObjectLine	: tONE Color TransMatrix PartName tEOL
{
	osgDB::ReaderWriter::ReadResult rr = start_include_file($4);
	if (rr.status() == osgDB::ReaderWriter::ReadResult::FILE_LOADED || rr.status() == osgDB::ReaderWriter::ReadResult::FILE_LOADED_FROM_CACHE) {
		/* update transform matrices */
		push_transform($3);
		znamelist_push();
		current_color[include_stack_ptr] = $2;
	} else {
		osg::notify(osg::DEBUG_INFO) << "Cannot find " << $4 << ", ignoring." << std::endl;
		}
		
		delete [] $4;
		}
		| tONE Color TransMatrix PartName tEOF
		{
			
			osgDB::ReaderWriter::ReadResult rr = start_include_file($4);
			if (rr.status() == osgDB::ReaderWriter::ReadResult::FILE_LOADED || rr.status() == osgDB::ReaderWriter::ReadResult::FILE_LOADED_FROM_CACHE) {
				/* update transform matrices */
				push_transform($3);
				znamelist_push();
				current_color[include_stack_ptr] = $2;
				defer_stop_include_file();
			} else {
				osg::notify(osg::DEBUG_INFO) << "Cannot find " << $4 << ", ignoring."<< std::endl;
			}
			
			delete [] $4;
		}
		;
		
		
		LineLine	: tTWO Color Position Position tEOL
		{
			*$3 = *$3 * *current_transform[transform_stack_ptr];
			*$4 = *$4 * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			
			emit_line($3, $4, $2);
			
			
			delete $3;
			delete $4;
			
		}
		| tTWO Color Position Position tEOF
		{
			*$3 = *$3 * *current_transform[transform_stack_ptr];
			*$4 = *$4 * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			
			emit_line($3, $4, $2);
			
			
			delete $3;
			delete $4;
			
			if (stop_include_file() == (-1)) { YYACCEPT; };
		}
		
		;
		
		TriLine	: tTHREE Color Position Position Position tEOL
		{
			*$3 = *$3 * *current_transform[transform_stack_ptr];
			*$4 = *$4 * *current_transform[transform_stack_ptr];
			*$5 = *$5 * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			
			emit_triangle($3, $4, $5, $2);
			
			
			delete $3;
			delete $4;
			delete $5;
			
		}
		| tTHREE Color Position Position Position tEOF
		{
			
			*$3 = *$3 * *current_transform[transform_stack_ptr];
			*$4 = *$4 * *current_transform[transform_stack_ptr];
			*$5 = *$5 * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			
			emit_triangle($3, $4, $5, $2);
			
			
			delete $3;
			delete $4;
			delete $5;
			
			if (stop_include_file() == (-1)) { YYACCEPT; };
		}
		;
		
		QuadLine	: tFOUR Color Position Position Position Position tEOL
		{
			
			*$3 = *$3 * *current_transform[transform_stack_ptr];
			*$4 = *$4 * *current_transform[transform_stack_ptr];
			*$5 = *$5 * *current_transform[transform_stack_ptr];
			*$6 = *$6 * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			//current_transform[transform_stack_ptr]->mulP(*$6);
			
			emit_quad($3, $4, $5, $6, $2);
			
			
			delete $3;
			delete $4;
			delete $5;
			delete $6;
			
		}
		| tFOUR Color Position Position Position Position tEOF
		{
			*$3 = *$3 * *current_transform[transform_stack_ptr];
			*$4 = *$4 * *current_transform[transform_stack_ptr];
			*$5 = *$5 * *current_transform[transform_stack_ptr];
			*$6 = *$6 * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			//current_transform[transform_stack_ptr]->mulP(*$6);
			
			emit_quad($3, $4, $5, $6, $2);
			
			
			delete $3;
			delete $4;
			delete $5;
			delete $6;
			
			if (stop_include_file() == (-1)) { YYACCEPT; };
		}
		;
		
		FiveLine	: tFIVE Color Position Position Position Position tEOL
		{
			*$3 = *$3 * *current_transform[transform_stack_ptr];
			*$4 = *$4 * *current_transform[transform_stack_ptr];
			*$5 = *$5 * *current_transform[transform_stack_ptr];
			*$6 = *$6 * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			//current_transform[transform_stack_ptr]->mulP(*$6);
			
			emit_optline($3, $4, $5, $6, $2);
			
			
			delete $3;
			delete $4;
			delete $5;
			delete $6;
			
		}
		| tFIVE Color Position Position Position Position tEOF
		{
			*$3 = *$3 * *current_transform[transform_stack_ptr];
			*$4 = *$4 * *current_transform[transform_stack_ptr];
			*$5 = *$5 * *current_transform[transform_stack_ptr];
			*$6 = *$6 * *current_transform[transform_stack_ptr];
			//current_transform[transform_stack_ptr]->mulP(*$3);
			//current_transform[transform_stack_ptr]->mulP(*$4);
			//current_transform[transform_stack_ptr]->mulP(*$5);
			//current_transform[transform_stack_ptr]->mulP(*$6);
			
			emit_optline($3, $4, $5, $6, $2);
			
			
			delete $3;
			delete $4;
			delete $5;
			delete $6;
			
			if (stop_include_file() == (-1)) { YYACCEPT; };
		}
		;
		
		%%
		
		void yyerror(char *s)
		{
			osg::notify(osg::FATAL) << "Syntax error on line \"" << s << "\""<< std::endl;
		}
		
		osg::Vec3f * LDParse::savept(float x, float y, float z)
		{
			osg::Vec3f *tmp = new osg::Vec3f;
			tmp->set(x,y,z);
			return tmp;
		}
		
		osg::Vec3f * LDParse::copypt(osg::Vec3f *pt)
		{
			osg::Vec3f *tmp;
			tmp = new osg::Vec3f;
			*tmp = *pt;
			return tmp;
		}
		
		
		void LDParse::transform_mat_inplace(osg::Matrixf *m)
		{
			osg::Matrixf newm;
			osg::Matrixf *oldm;
			
			oldm = current_transform[transform_stack_ptr];
			
			(&newm)->mult(*oldm, *m);
			*m = newm;
			return;
		}
		
		S32 LDParse::print_transform(osg::Matrixf *m)
		{
			//m->dumpMatrix();
			osg::notify(osg::DEBUG_INFO) << "We don't print matrices now";
			return 0;
		}
		
		void LDParse::pop_transform()
		{
			AssertFatal(transform_stack_ptr >= 0, "Error in transform stack");
			delete current_transform[transform_stack_ptr];
			transform_stack_ptr--;
		}
		
		S32 LDParse::push_rotation(osg::Vec3f *axis, F64 degrees)
		{
			// convert axis - degrees into rotation matrix
			osg::Matrixf *m = new osg::Matrixf;
			/*
			F64 a,b,c,s, veclen_inv, sin2a;
			
			// convert axis and degrees into a quaternion
			veclen_inv = 1.0/sqrt(axis->x*axis->x + axis->y*axis->y + axis->z*axis->z);
			sin2a = sin((3.1415927*degrees)/360.0);
			a = sin2a * axis->x * veclen_inv;
			b = sin2a * axis->y * veclen_inv;
			c = sin2a * axis->z * veclen_inv;
			s = cos((3.1415927*degrees)/360.0);
			// convert quaternion into a rotation matrix.
			//*/
			m->makeIdentity();
			m->rotate(degrees * M_PI/180, osg::Vec3d(*axis));
			//m->setRow(0, Point3F((float)(1 - 2*b*b-2*c*c), (float)(2*a*b - 2*s*c), (float)(2*a*c + 2*s*b)));
			//m->setRow(1, Point3F((float)(2*a*b+2*s*c), (float)(1 - 2*a*a - 2*c*c), (float)(2*b*c - 2*s*a)));
			//m->setRow(2, Point3F((float)(2*a*c - 2*s*b), (float)(2*b*c + 2*s*a), (float)(1 - 2*a*a - 2*b*b)));
			
			push_transform(m);
			
			return 0;
		}
		
		S32 LDParse::push_transform(osg::Matrixf *m)
		{
			osg::Matrixf *oldm; 
			
			AssertFatal(transform_stack_ptr < (MAX_TRANSFORM_DEPTH-1),"Transform stack overflow");
			if (transform_stack_ptr < 0) {
				if (m == NULL) {
					m = new osg::Matrixf;
					m->makeIdentity();
				}
			} else {
				oldm = current_transform[transform_stack_ptr];
				if (m == NULL) {
					m = new osg::Matrixf;
					
					*m = *oldm;
				} else {
					osg::Matrixf newm;
					newm.mult(*oldm, *m);
					*m = newm;
				}
			}
			transform_stack_ptr++;
			current_transform[transform_stack_ptr] = m;
			return 0;
		}
		
		inline void LDParse::zStep(S32 step, S32 pause){
			return;
		}
		
		inline void LDParse::zPause(){
			return;
		}
		
		inline void LDParse::zWrite(char *message){
			osg::notify(osg::DEBUG_INFO) << "LDraw Write: " << message << std::endl;
		}
		
		inline void LDParse::zClear(){ ; // Do nothing for now
			//Con::warnf("LDraw encountered CLEAR. Ignoring");
		}
		
		inline void LDParse::zSave(S32 step){ ; // Do nothing for now
			//Con::warnf("Generating LDraw step imagery is best done with other software");
		}
		
		osg::Matrixf * LDParse::savemat(F32 x, F32 y, F32 z, 
								   F32 a, F32 b, F32 c,
								   F32 d, F32 e, F32 f,
								   F32 g, F32 h, F32 i)
		{
			osg::Matrixf *tmp = new osg::Matrixf;
			tmp->set(a, b, c, x, d, e, f, y, g, h, i, z, 0, 0, 0, 1);
			
			return tmp;
		}
		
		osg::Matrixf * LDParse::copymat(osg::Matrixf *mat)
		{
			osg::Matrixf *tmp;
			tmp = new osg::Matrixf;
			*tmp = *mat;
			return tmp;
		}
		
