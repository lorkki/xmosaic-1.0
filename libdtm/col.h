#define COL_TRIPLET	DTM_TRIPLET

#define COLclass 	"COL"
#define COLtitle	"TTL"
#define COLdims		"DIM"
#define COLwidth	"WTH"
#define COLfunc		"FNC"
#define COLsel		"SEL"
#define COLid		"ID"

#define	COL_POINT	1
#define	COL_LINE	2
#define	COL_AREA	3
#define	COL_DOODLE_CONT 4
#define	COL_DOODLE_DISC	5

#ifdef DTM1
#define COLsetClass(h)		DTMsetClass(h,COLclass)
#define COLcompareClass(h)	DTMcompareClass(h,COLclass)
#else
#define COLsetClass(h)		dtm_set_class(h,COLclass)
#define COLcompareClass(h)	dtm_compare_class(h,COLclass)
#endif

#ifdef DTM1
#define COLsetTitle		DTMsetTitle
#define COLgetTitle		DTMgetTitle
#else
#define COLsetTitle		dtm_set_title
#define COLgetTitle		dtm_get_title
#endif

#define COLsetID(h,i)		dtm_set_char(h,COLid,i)
#define COLgetID(h,s,l)		dtm_get_char(h,COLid,s,l)

#define COLsetFunc(h,f,s)	{ dtm_set_char(h,COLfunc,f); \
				  dtm_set_int(h,COLsel,s); }

/* COLgetFunc(char *header, char *function, int length, int *selType) */
#define COLgetFunc(h,f,l,i)	{ dtm_get_char(h,COLfunc,f,l); \
				  dtm_get_int(h,COLsel,i); }

#define COLsetDimension(h,i)	dtm_set_int(h,COLdims,i)
#define COLgetDimension(h,i)	dtm_get_int(h,COLdims,i)

#define COLsetWidth(h,i)	dtm_set_int(h,COLwidth,i)
#define COLgetWidth(h,i)	dtm_get_int(h,COLwidth,i)

