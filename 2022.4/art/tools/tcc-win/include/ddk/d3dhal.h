/*
 * Direct3D driver interface
 * (DirectX 7 version)
 *
 * Copyright (C) 2001 Ove Kaaven
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _D3DHAL_H_
#define _D3DHAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ddraw.h>
#include <d3dtypes.h>
#include <d3dcaps.h>
#include <d3d.h>

#if (DIRECT3D_VERSION >= 0x0900)
#include <d3d9.h>
#endif

#ifndef DDRAWI_DIRECTDRAW_DEFINED
typedef struct _DDRAWI_DIRECTDRAW_GBL FAR *LPDDRAWI_DIRECTDRAW_GBL;
typedef struct _DDRAWI_DIRECTDRAW_LCL FAR *LPDDRAWI_DIRECTDRAW_LCL;
#define DDRAWI_DIRECTDRAW_DEFINED
#endif

#ifndef DDRAWI_DDRAWSURFACE_DEFINED
typedef struct _DDRAWI_DDRAWSURFACE_LCL FAR *LPDDRAWI_DDRAWSURFACE_LCL;
#define DDRAWI_DDRAWSURFACE_DEFINED
#endif

#define D3DERR_COMMAND_UNPARSED 0x88760BB8;

/*****************************************************************************
 * device info structures
 */
typedef struct _D3DDeviceDesc_V1 {
  DWORD			dwSize;
  DWORD			dwFlags;
  D3DCOLORMODEL		dcmColorModel;
  DWORD			dwDevCaps;
  D3DTRANSFORMCAPS	dtcTransformCaps;
  BOOL			bClipping;
  D3DLIGHTINGCAPS	dlcLightingCaps;
  D3DPRIMCAPS		dpcLineCaps;
  D3DPRIMCAPS		dpcTriCaps;
  DWORD			dwDeviceRenderBitDepth;
  DWORD			dwDeviceZBufferBitDepth;
  DWORD			dwMaxBufferSize;
  DWORD			dwMaxVertexCount;
} D3DDEVICEDESC_V1,*LPD3DDEVICEDESC_V1;

/* this is to allow keeping the bulk of our OpenGL code out of x11drv */
#define D3DDD_WINE_OPENGL_DEVICE 0x00008000

typedef struct _D3DHAL_GLOBALDRIVERDATA {
  DWORD			dwSize;
  D3DDEVICEDESC_V1	hwCaps;
  DWORD			dwNumVertices;
  DWORD			dwNumClipVertices;
  DWORD			dwNumTextureFormats;
  LPDDSURFACEDESC	lpTextureFormats;
} D3DHAL_GLOBALDRIVERDATA;

#ifndef D3DHAL_GLOBALDRIVERDATA_DEFINED
typedef D3DHAL_GLOBALDRIVERDATA *LPD3DHAL_GLOBALDRIVERDATA;
#define D3DHAL_GLOBALDRIVERDATA_DEFINED
#endif

typedef struct _D3DHAL_D3DEXTENDEDCAPS {
  DWORD			dwSize;
  /* DirectX 5 */
  DWORD			dwMinTextureWidth, dwMaxTextureWidth;
  DWORD			dwMinTextureHeight, dwMaxTextureHeight;
  DWORD			dwMinStippleWidth, dwMaxStippleWidth;
  DWORD			dwMinStippleHeight, dwMaxStippleHeight;
  /* DirectX 6 */
  DWORD			dwMaxTextureRepeat;
  DWORD			dwMaxTextureAspectRatio;
  DWORD			dwMaxAnisotropy;
  D3DVALUE		dvGuardBandLeft;
  D3DVALUE		dvGuardBandTop;
  D3DVALUE		dvGuardBandRight;
  D3DVALUE		dvGuardBandBottom;
  D3DVALUE		dvExtentsAdjust;
  DWORD			dwStencilCaps;
  DWORD			dwFVFCaps;
  DWORD			dwTextureOpCaps;
  WORD			wMaxTextureBlendStages;
  WORD			wMaxSimultaneousTextures;
  /* DirectX 7 */
  DWORD			dwMaxActiveLights;
  D3DVALUE		dvMaxVertexW;
  WORD			wMaxUserClipPlanes;
  WORD			wMaxVertexBlendMatrices;
  DWORD			dwVertexProcessingCaps;
  DWORD			dwReserved1;
  DWORD			dwReserved2;
  DWORD			dwReserved3;
  DWORD			dwReserved4;
} D3DHAL_D3DEXTENDEDCAPS,*LPD3DHAL_D3DEXTENDEDCAPS;

/*****************************************************************************
 * d3d->driver callbacks
 */
typedef struct _D3DHAL_CONTEXTCREATEDATA	*LPD3DHAL_CONTEXTCREATEDATA;
typedef struct _D3DHAL_CONTEXTDESTROYDATA	*LPD3DHAL_CONTEXTDESTROYDATA;
typedef struct _D3DHAL_CONTEXTDESTROYALLDATA	*LPD3DHAL_CONTEXTDESTROYALLDATA;
typedef struct _D3DHAL_SCENECAPTUREDATA		*LPD3DHAL_SCENECAPTUREDATA;
typedef struct _D3DHAL_RENDERSTATEDATA		*LPD3DHAL_RENDERSTATEDATA;
typedef struct _D3DHAL_RENDERPRIMITIVEDATA	*LPD3DHAL_RENDERPRIMITIVEDATA;
typedef struct _D3DHAL_TEXTURECREATEDATA	*LPD3DHAL_TEXTURECREATEDATA;
typedef struct _D3DHAL_TEXTUREDESTROYDATA	*LPD3DHAL_TEXTUREDESTROYDATA;
typedef struct _D3DHAL_TEXTURESWAPDATA		*LPD3DHAL_TEXTURESWAPDATA;
typedef struct _D3DHAL_TEXTUREGETSURFDATA	*LPD3DHAL_TEXTUREGETSURFDATA;
typedef struct _D3DHAL_GETSTATEDATA		*LPD3DHAL_GETSTATEDATA;

typedef DWORD (PASCAL *LPD3DHAL_CONTEXTCREATECB)    (LPD3DHAL_CONTEXTCREATEDATA);
typedef DWORD (PASCAL *LPD3DHAL_CONTEXTDESTROYCB)   (LPD3DHAL_CONTEXTDESTROYDATA);
typedef DWORD (PASCAL *LPD3DHAL_CONTEXTDESTROYALLCB)(LPD3DHAL_CONTEXTDESTROYALLDATA);
typedef DWORD (PASCAL *LPD3DHAL_SCENECAPTURECB)	    (LPD3DHAL_SCENECAPTUREDATA);
typedef DWORD (PASCAL *LPD3DHAL_RENDERSTATECB)	    (LPD3DHAL_RENDERSTATEDATA);
typedef DWORD (PASCAL *LPD3DHAL_RENDERPRIMITIVECB)  (LPD3DHAL_RENDERPRIMITIVEDATA);
typedef DWORD (PASCAL *LPD3DHAL_TEXTURECREATECB)    (LPD3DHAL_TEXTURECREATEDATA);
typedef DWORD (PASCAL *LPD3DHAL_TEXTUREDESTROYCB)   (LPD3DHAL_TEXTUREDESTROYDATA);
typedef DWORD (PASCAL *LPD3DHAL_TEXTURESWAPCB)	    (LPD3DHAL_TEXTURESWAPDATA);
typedef DWORD (PASCAL *LPD3DHAL_TEXTUREGETSURFCB)   (LPD3DHAL_TEXTUREGETSURFDATA);
typedef DWORD (PASCAL *LPD3DHAL_GETSTATECB)	    (LPD3DHAL_GETSTATEDATA);

typedef struct _D3DHAL_CALLBACKS {
  DWORD				dwSize;
  LPD3DHAL_CONTEXTCREATECB	ContextCreate;
  LPD3DHAL_CONTEXTDESTROYCB	ContextDestroy;
  LPD3DHAL_CONTEXTDESTROYALLCB	ContextDestroyAll;
  LPD3DHAL_SCENECAPTURECB	SceneCapture;
  LPVOID			lpReserved10;
  LPVOID			lpReserved11;
  LPD3DHAL_RENDERSTATECB	RenderState;
  LPD3DHAL_RENDERPRIMITIVECB	RenderPrimitive;
  DWORD				dwReserved;
  LPD3DHAL_TEXTURECREATECB	TextureCreate;
  LPD3DHAL_TEXTUREDESTROYCB	TextureDestroy;
  LPD3DHAL_TEXTURESWAPCB	TextureSwap;
  LPD3DHAL_TEXTUREGETSURFCB	TextureGetSurf;
  /* now why did MS create CALLBACKS2 and CALLBACKS3 structures if
   * all these reserved fields were available? we may never know */
  LPVOID			lpReserved12;
  LPVOID			lpReserved13;
  LPVOID			lpReserved14;
  LPVOID			lpReserved15;
  LPVOID			lpReserved16;
  LPVOID			lpReserved17;
  LPVOID			lpReserved18;
  LPVOID			lpReserved19;
  LPVOID			lpReserved20;
  LPVOID			lpReserved21;
  LPD3DHAL_GETSTATECB		GetState;
  DWORD				dwReserved0;
  DWORD				dwReserved1;
  DWORD				dwReserved2;
  DWORD				dwReserved3;
  DWORD				dwReserved4;
  DWORD				dwReserved5;
  DWORD				dwReserved6;
  DWORD				dwReserved7;
  DWORD				dwReserved8;
  DWORD				dwReserved9;
} D3DHAL_CALLBACKS;

#ifndef D3DHAL_CALLBACKS_DEFINED
typedef D3DHAL_CALLBACKS *LPD3DHAL_CALLBACKS;
#define D3DHAL_CALLBACKS_DEFINED
#endif

typedef struct _D3DHAL_SETRENDERTARGETDATA		*LPD3DHAL_SETRENDERTARGETDATA;
typedef struct _D3DHAL_CLEARDATA			*LPD3DHAL_CLEARDATA;
typedef struct _D3DHAL_DRAWONEPRIMITIVEDATA		*LPD3DHAL_DRAWONEPRIMITIVEDATA;
typedef struct _D3DHAL_DRAWONEINDEXEDPRIMITIVEDATA	*LPD3DHAL_DRAWONEINDEXEDPRIMITIVEDATA;
typedef struct _D3DHAL_DRAWPRIMITIVESDATA		*LPD3DHAL_DRAWPRIMITIVESDATA;

typedef DWORD (PASCAL *LPD3DHAL_SETRENDERTARGETCB)	  (LPD3DHAL_SETRENDERTARGETDATA);
typedef DWORD (PASCAL *LPD3DHAL_CLEARCB)		  (LPD3DHAL_CLEARDATA);
typedef DWORD (PASCAL *LPD3DHAL_DRAWONEPRIMITIVECB)	  (LPD3DHAL_DRAWONEPRIMITIVEDATA);
typedef DWORD (PASCAL *LPD3DHAL_DRAWONEINDEXEDPRIMITIVECB)(LPD3DHAL_DRAWONEINDEXEDPRIMITIVEDATA);
typedef DWORD (PASCAL *LPD3DHAL_DRAWPRIMITIVESCB)	  (LPD3DHAL_DRAWPRIMITIVESDATA);

typedef struct _D3DHAL_CALLBACKS2 {
  DWORD					dwSize;
  DWORD					dwFlags;
  LPD3DHAL_SETRENDERTARGETCB		SetRenderTarget;
  LPD3DHAL_CLEARCB			Clear;
  LPD3DHAL_DRAWONEPRIMITIVECB		DrawOnePrimitive;
  LPD3DHAL_DRAWONEINDEXEDPRIMITIVECB	DrawOneIndexedPrimitive;
  LPD3DHAL_DRAWPRIMITIVESCB		DrawPrimitives;
} D3DHAL_CALLBACKS2,*LPD3DHAL_CALLBACKS2;

typedef struct _D3DHAL_CLEAR2DATA			*LPD3DHAL_CLEAR2DATA;
typedef struct _D3DHAL_VALIDATETEXTURESTAGESTATEDATA	*LPD3DHAL_VALIDATETEXTURESTAGESTATEDATA;
typedef struct _D3DHAL_DRAWPRIMITIVES2DATA		*LPD3DHAL_DRAWPRIMITIVES2DATA;

typedef DWORD (PASCAL *LPD3DHAL_CLEAR2CB)		  (LPD3DHAL_CLEAR2DATA);
typedef DWORD (PASCAL *LPD3DHAL_VALIDATETEXTURESTAGESTATECB)(LPD3DHAL_VALIDATETEXTURESTAGESTATEDATA);
typedef DWORD (PASCAL *LPD3DHAL_DRAWPRIMITIVES2CB)	  (LPD3DHAL_DRAWPRIMITIVES2DATA);

typedef struct _D3DHAL_CALLBACKS3 {
  DWORD					dwSize;
  DWORD					dwFlags;
  LPD3DHAL_CLEAR2CB			Clear2;
  LPVOID				lpvReserved;
  LPD3DHAL_VALIDATETEXTURESTAGESTATECB	ValidateTextureStageState;
  LPD3DHAL_DRAWPRIMITIVES2CB		DrawPrimitives2;
} D3DHAL_CALLBACKS3,*LPD3DHAL_CALLBACKS3;

/*****************************************************************************
 * parameter structures
 */
typedef struct _D3DHAL_CONTEXTCREATEDATA {
  union {
    LPDDRAWI_DIRECTDRAW_GBL	lpDDGbl; /* pre-DirectX 7 */
    LPDDRAWI_DIRECTDRAW_LCL	lpDDLcl; /* DirectX 7 */
  } DUMMYUNIONNAME1;
  union {
    LPDIRECTDRAWSURFACE		lpDDS;
    LPDDRAWI_DDRAWSURFACE_LCL	lpDDSLcl; /* DirectX 7 */
  } DUMMYUNIONNAME2;
  union {
    LPDIRECTDRAWSURFACE		lpDDSZ;
    LPDDRAWI_DDRAWSURFACE_LCL	lpDDSZLcl; /* DirectX 7 */
  } DUMMYUNIONNAME3;
  union {
    DWORD			dwPID;
    ULONG_PTR			dwrstates;
  } DUMMYUNIONNAME4;
  ULONG_PTR			dwhContext;
  HRESULT			ddrval;
} D3DHAL_CONTEXTCREATEDATA;

typedef struct _D3DHAL_CONTEXTDESTROYDATA {
  ULONG_PTR			dwhContext;
  HRESULT			ddrval;
} D3DHAL_CONTEXTDESTROYDATA;

typedef struct _D3DHAL_CONTEXTDESTROYALLDATA {
  DWORD				dwPID;
  HRESULT			ddrval;
} D3DHAL_CONTEXTDESTROYALLDATA;

typedef struct _D3DHAL_SCENECAPTUREDATA {
  ULONG_PTR			dwhContext;
  DWORD				dwFlag;
  HRESULT			ddrval;
} D3DHAL_SCENECAPTUREDATA;

#define D3DHAL_SCENE_CAPTURE_START	0x00000000
#define D3DHAL_SCENE_CAPTURE_END	0x00000001

typedef struct _D3DHAL_SETRENDERTARGETDATA {
  ULONG_PTR			dwhContext;
  union {
    LPDIRECTDRAWSURFACE		lpDDS;
    LPDDRAWI_DDRAWSURFACE_LCL	lpDDSLcl;
  } DUMMYUNIONNAME1;
  union {
    LPDIRECTDRAWSURFACE		lpDDSZ;
    LPDDRAWI_DDRAWSURFACE_LCL	lpDDSZLcl;
  } DUMMYUNIONNAME2;
  HRESULT			ddrval;
} D3DHAL_SETRENDERTARGETDATA;

typedef struct _D3DHAL_DRAWPRIMITIVES2DATA {
  ULONG_PTR			dwhContext;
  DWORD				dwFlags;
  DWORD				dwVertexType;
  LPDDRAWI_DDRAWSURFACE_LCL	lpDDCommands;
  DWORD				dwCommandOffset;
  DWORD				dwCommandLength;
  union {
    LPDDRAWI_DDRAWSURFACE_LCL	lpDDVertex;
    LPVOID			lpVertices;
  } DUMMYUNIONNAME1;
  DWORD				dwVertexOffset;
  DWORD				dwVertexLength;
  DWORD				dwReqVertexBufSize;
  DWORD				dwReqCommandBufSize;
  LPDWORD			lpdwRStates;
  union {
    DWORD			dwVertexSize;
    HRESULT			ddrval;
  } DUMMYUNIONNAME2;
  DWORD				dwErrorOffset;
} D3DHAL_DRAWPRIMITIVES2DATA;

#define D3DHALDP2_USERMEMVERTICES	0x00000001
#define D3DHALDP2_EXECUTEBUFFER		0x00000002
#define D3DHALDP2_SWAPVERTEXBUFFER	0x00000004
#define D3DHALDP2_SWAPCOMMANDBUFFER	0x00000008
#define D3DHALDP2_REQVERTEXBUFSIZE	0x00000010
#define D3DHALDP2_REQCOMMANDBUFSIZE	0x00000020
#define D3DHALDP2_VIDMEMVERTEXBUF	0x00000040
#define D3DHALDP2_VIDMEMCOMMANDBUF	0x00000080

/*****************************************************************************
 * DrawPrimitives2 command structures
 */
typedef struct _D3DHAL_DP2COMMAND {
  BYTE bCommand;
  BYTE bReserved;
  union {
    WORD wPrimitiveCount;
    WORD wStateCount;
  } DUMMYUNIONNAME;
} D3DHAL_DP2COMMAND,*LPD3DHAL_DP2COMMAND;

typedef enum _D3DHAL_DP2OPERATION {
  D3DDP2OP_POINTS		= 1,
  D3DDP2OP_INDEXEDLINELIST	= 2,
  D3DDP2OP_INDEXEDTRIANGLELIST	= 3,
  D3DDP2OP_RENDERSTATE		= 8,
  D3DDP2OP_LINELIST		= 15,
  D3DDP2OP_LINESTRIP		= 16,
  D3DDP2OP_INDEXEDLINESTRIP	= 17,
  D3DDP2OP_TRIANGLELIST		= 18,
  D3DDP2OP_TRIANGLESTRIP	= 19,
  D3DDP2OP_INDEXEDTRIANGLESTRIP	= 20,
  D3DDP2OP_TRIANGLEFAN		= 21,
  D3DDP2OP_INDEXEDTRIANGLEFAN	= 22,
  D3DDP2OP_TRIANGLEFAN_IMM	= 23,
  D3DDP2OP_LINELIST_IMM		= 24,
  D3DDP2OP_TEXTURESTAGESTATE	= 25,
  D3DDP2OP_INDEXEDTRIANGLELIST2	= 26,
  D3DDP2OP_INDEXEDLINELIST2	= 27,
  D3DDP2OP_VIEWPORTINFO		= 28,
  D3DDP2OP_WINFO		= 29,
  /* pre-DirectX 7 interfaces */
  D3DDP2OP_SETPALETTE		= 30,
  D3DDP2OP_UPDATEPALETTE	= 31,
  /* DirectX 7 interfaces */
  D3DDP2OP_ZRANGE		= 32,
  D3DDP2OP_SETMATERIAL		= 33,
  D3DDP2OP_SETLIGHT		= 34,
  D3DDP2OP_CREATELIGHT		= 35,
  D3DDP2OP_SETTRANSFORM		= 36,
  D3DDP2OP_TEXBLT		= 38,
  D3DDP2OP_STATESET		= 39,
  D3DDP2OP_SETPRIORITY		= 40,
  /* all interfaces */
  D3DDP2OP_SETRENDERTARGET	= 41,
  D3DDP2OP_CLEAR		= 42,
  /* DirectX 7 interfaces */
  D3DDP2OP_SETTEXLOD		= 43,
  D3DPP2OP_SETCLIPPLANE		= 44,
#if(DIRECT3D_VERSION >= 0x0800)
  D3DDP2OP_CREATEVERTEXSHADER   = 45,
  D3DDP2OP_DELETEVERTEXSHADER   = 46,
  D3DDP2OP_SETVERTEXSHADER      = 47,
  D3DDP2OP_SETVERTEXSHADERCONST = 48,
  D3DDP2OP_SETSTREAMSOURCE      = 49,
  D3DDP2OP_SETSTREAMSOURCEUM    = 50,
  D3DDP2OP_SETINDICES           = 51,
  D3DDP2OP_DRAWPRIMITIVE        = 52,
  D3DDP2OP_DRAWINDEXEDPRIMITIVE = 53,
  D3DDP2OP_CREATEPIXELSHADER    = 54,
  D3DDP2OP_DELETEPIXELSHADER    = 55,
  D3DDP2OP_SETPIXELSHADER       = 56,
  D3DDP2OP_SETPIXELSHADERCONST  = 57,
  D3DDP2OP_CLIPPEDTRIANGLEFAN   = 58,
  D3DDP2OP_DRAWPRIMITIVE2       = 59,
  D3DDP2OP_DRAWINDEXEDPRIMITIVE2= 60,
  D3DDP2OP_DRAWRECTPATCH        = 61,
  D3DDP2OP_DRAWTRIPATCH         = 62,
  D3DDP2OP_VOLUMEBLT            = 63,
  D3DDP2OP_BUFFERBLT            = 64,
  D3DDP2OP_MULTIPLYTRANSFORM    = 65,
  D3DDP2OP_ADDDIRTYRECT         = 66,
  D3DDP2OP_ADDDIRTYBOX          = 67,
#endif
#if(DIRECT3D_VERSION >= 0x0900)
  D3DDP2OP_CREATEVERTEXSHADERDECL   = 71,
  D3DDP2OP_DELETEVERTEXSHADERDECL   = 72,
  D3DDP2OP_SETVERTEXSHADERDECL      = 73,
  D3DDP2OP_CREATEVERTEXSHADERFUNC   = 74,
  D3DDP2OP_DELETEVERTEXSHADERFUNC   = 75,
  D3DDP2OP_SETVERTEXSHADERFUNC      = 76,
  D3DDP2OP_SETVERTEXSHADERCONSTI    = 77,
  D3DDP2OP_SETSCISSORRECT           = 79,
  D3DDP2OP_SETSTREAMSOURCE2         = 80,
  D3DDP2OP_BLT                      = 81,
  D3DDP2OP_COLORFILL                = 82,
  D3DDP2OP_SETVERTEXSHADERCONSTB    = 83,
  D3DDP2OP_CREATEQUERY              = 84,
  D3DDP2OP_SETRENDERTARGET2         = 85,
  D3DDP2OP_SETDEPTHSTENCIL          = 86,
  D3DDP2OP_RESPONSECONTINUE         = 87,
  D3DDP2OP_RESPONSEQUERY            = 88,
  D3DDP2OP_GENERATEMIPSUBLEVELS     = 89,
  D3DDP2OP_DELETEQUERY              = 90,
  D3DDP2OP_ISSUEQUERY               = 91,
  D3DDP2OP_SETPIXELSHADERCONSTI     = 93,
  D3DDP2OP_SETPIXELSHADERCONSTB     = 94,
  D3DDP2OP_SETSTREAMSOURCEFREQ      = 95,
  D3DDP2OP_SURFACEBLT               = 96
#endif
} D3DHAL_DP2OPERATION;

/* point primitives */

typedef struct _D3DHAL_POINTS {
  WORD wCount;
  WORD wVStart;
} D3DHAL_DP2POINTS,*LPD3DHAL_DP2POINTS;

/* line primitives */

typedef struct _D3DHAL_DP2STARTVERTEX {
  WORD wVStart;
} D3DHAL_DP2STARTVERTEX,*LPD3DHAL_DP2STARTVERTEX;

typedef struct _D3DHAL_DP2LINELIST {
  WORD wVStart;
} D3DHAL_DP2LINELIST,*LPD3DHAL_DP2LINELIST;

typedef struct _D3DHAL_DP2INDEXEDLINELIST {
  WORD wV1;
  WORD wV2;
} D3DHAL_DP2INDEXEDLINELIST,*LPD3DHAL_DP2INDEXEDLINELIST;

typedef struct _D3DHAL_DP2LINESTRIP {
  WORD wVStart;
} D3DHAL_DP2LINESTRIP,*LPD3DHAL_DP2LINESTRIP;

typedef struct _D3DHAL_DP2INDEXEDLINESTRIP {
  WORD wV[2];
} D3DHAL_DP2INDEXEDLINESTRIP,*LPD3DHAL_DP2INDEXEDLINESTRIP;

/* triangle primitives */

typedef struct _D3DHAL_DP2TRIANGLELIST {
  WORD wVStart;
} D3DHAL_DP2TRIANGLELIST,*LPD3DHAL_DP2TRIANGLELIST;

typedef struct _D3DHAL_DP2INDEXEDTRIANGLELIST {
  WORD wV1;
  WORD wV2;
  WORD wV3;
  WORD wFlags;
} D3DHAL_DP2INDEXEDTRIANGLELIST,*LPD3DHAL_DP2INDEXEDTRIANGLELIST;

typedef struct _D3DHAL_DP2INDEXEDTRIANGLELIST2 {
  WORD wV1;
  WORD wV2;
  WORD wV3;
} D3DHAL_DP2INDEXEDTRIANGLELIST2,*LPD3DHAL_DP2INDEXEDTRIANGLELIST2;

typedef struct _D3DHAL_DP2TRIANGLESTRIP {
  WORD wVStart;
} D3DHAL_DP2TRIANGLESTRIP,*LPD3DHAL_DP2TRIANGLESTRIP;

typedef struct _D3DHAL_DP2INDEXEDTRIANGLESTRIP {
  WORD wV[3];
} D3DHAL_DP2INDEXEDTRIANGLESTRIP,*LPD3DHAL_DP2INDEXEDTRIANGLESTRIP;

typedef struct _D3DHAL_DP2TRIANGLEFAN {
  WORD wVStart;
} D3DHAL_DP2TRIANGLEFAN,*LPD3DHAL_DP2TRIANGLEFAN;

typedef struct _D3DHAL_DP2INDEXEDTRIANGLEFAN {
  WORD wV[3];
} D3DHAL_DP2INDEXEDTRIANGLEFAN,*LPD3DHAL_DP2INDEXEDTRIANGLEFAN;

typedef struct _D3DHAL_DP2TRIANGLEFAN_IMM {
  DWORD dwEdgeFlags;
} D3DHAL_DP2TRIANGLEFAN_IMM,*LPD3DHAL_DP2TRIANGLEFAN_IMM;

/* render states */
typedef struct _D3DHAL_DP2RENDERSTATE {
  D3DRENDERSTATETYPE	RenderState;
  union {
    D3DVALUE dvState;
    DWORD dwState;
  } DUMMYUNIONNAME;
} D3DHAL_DP2RENDERSTATE,*LPD3DHAL_DP2RENDERSTATE;

typedef struct _D3DHAL_DP2TEXTURESTAGESTATE {
  WORD	wStage;
  WORD	TSState;
  DWORD dwValue;
} D3DHAL_DP2TEXTURESTAGESTATE,*LPD3DHAL_DP2TEXTURESTAGESTATE;

#define D3DTSS_TEXTUREMAP 0

typedef struct _D3DHAL_DP2VIEWPORTINFO {
  DWORD dwX;
  DWORD dwY;
  DWORD dwWidth;
  DWORD dwHeight;
} D3DHAL_DP2VIEWPORTINFO,*LPD3DHAL_DP2VIEWPORTINFO;

typedef struct _D3DHAL_DP2WINFO {
  D3DVALUE dwWNear;
  D3DVALUE dwWFar;
} D3DHAL_DP2WINFO,*LPD3DHAL_DP2WINFO;

typedef struct _D3DHAL_DP2SETPALETTE {
  DWORD dwPaletteHandle;
  DWORD dwPaletteFlags;
  DWORD dwSurfaceHandle;
} D3DHAL_DP2SETPALETTE,*LPD3DHAL_DP2SETPALETTE;

typedef struct _D3DHAL_DP2UPDATEPALETTE {
  DWORD dwPaletteHandle;
  WORD	wStartIndex;
  WORD	wNumEntries;
} D3DHAL_DP2UPDATEPALETTE,*LPD3DHAL_DP2UPDATEPALETTE;

typedef struct _D3DHAL_DP2ZRANGE {
  D3DVALUE dvMinZ;
  D3DVALUE dvMaxZ;
} D3DHAL_DP2ZRANGE,*LPD3DHAL_DP2ZRANGE;

typedef D3DMATERIAL7 D3DHAL_DP2SETMATERIAL,*LPD3DHAL_DP2SETMATERIAL;

typedef struct _D3DHAL_DP2SETLIGHT {
  DWORD dwIndex;
  DWORD dwDataType;
} D3DHAL_DP2SETLIGHT,*LPD3DHAL_DP2SETLIGHT;

#define D3DHAL_SETLIGHT_ENABLE	0
#define D3DHAL_SETLIGHT_DISABLE	1
#define D3DHAL_SETLIGHT_DATA	2

typedef struct _D3DHAL_DP2CREATELIGHT {
  DWORD dwIndex;
} D3DHAL_DP2CREATELIGHT,*LPD3DHAL_DP2CREATELIGHT;

typedef struct _D3DHAL_DP2SETTRANSFORM {
  D3DTRANSFORMSTATETYPE	xfrmType;
  D3DMATRIX		matrix;
} D3DHAL_DP2SETTRANSFORM,*LPD3DHAL_DP2SETTRANSFORM;

typedef struct _D3DHAL_DP2TEXBLT {
  DWORD dwDDDestSurface;
  DWORD dwDDSrcSurface;
  POINT pDest;
  RECTL rSrc;
  DWORD dwFlags;
} D3DHAL_DP2TEXBLT,*LPD3DHAL_DP2TEXBLT;

typedef struct _D3DHAL_DP2STATESET {
  DWORD               dwOperation;
  DWORD               dwParam;
  D3DSTATEBLOCKTYPE   sbType;
} D3DHAL_DP2STATESET,*LPD3DHAL_DP2STATESET;

#define D3DHAL_STATESETBEGIN	0
#define D3DHAL_STATESETEND	1
#define D3DHAL_STATESETDELETE	2
#define D3DHAL_STATESETEXECUTE	3
#define D3DHAL_STATESETCAPTURE	4

typedef struct _D3DHAL_DP2SETPRIORITY {
  DWORD dwDDSurface;
  DWORD dwPriority;
} D3DHAL_DP2SETPRIORITY,*LPD3DHAL_DP2SETPRIORITY;

typedef struct _D3DHAL_DP2SETRENDERTARGET {
  DWORD hRenderTarget;
  DWORD hZBuffer;
} D3DHAL_DP2SETRENDERTARGET,*LPD3DHAL_DP2SETRENDERTARGET;

typedef struct _D3DHAL_DP2CLEAR {
  DWORD		dwFlags;
  DWORD		dwFillColor;
  D3DVALUE	dvFillDepth;
  DWORD		dwFillStencil;
  RECT		Rects[1];
} D3DHAL_DP2CLEAR,*LPD3DHAL_DP2CLEAR;

typedef struct _D3DHAL_DP2SETTEXLOD {
  DWORD dwDDSurface;
  DWORD dwLOD;
} D3DHAL_DP2SETTEXLOD,*LPD3DHAL_DP2SETTEXLOD;


#if (DIRECT3D_VERSION >= 0x0900)

#define DX9_DDI_VERSION     4

#ifndef D3DVTXPCAPS_NO_VSDT_UBYTE4
#define D3DVTXPCAPS_NO_VSDT_UBYTE4          0x00000080
#endif

#define D3DPMISCCAPS_LINEPATTERNREP         0x00000004

#define D3DDEVCAPS_HWVERTEXBUFFER           0x02000000
#define D3DDEVCAPS_HWINDEXBUFFER            0x04000000
#define D3DDEVCAPS_SUBVOLUMELOCK            0x08000000

#ifndef D3DPMISCCAPS_FOGINFVF
#define D3DPMISCCAPS_FOGINFVF               0x00002000
#endif

#ifndef D3DFVF_FOG
#define D3DFVF_FOG                          0x00002000
#endif

typedef struct _DD_GETDRIVERINFO2DATA
{
    DWORD       dwReserved;
    DWORD       dwMagic;
    DWORD       dwType;
    DWORD       dwExpectedSize;
} DD_GETDRIVERINFO2DATA;

/**
 * A driver must check DD_GETDRIVERINFO2DATA.dwHeight for
 * D3DGDI2_MAGIC to see if the passed GUID is GUID_GetDriverInfo2
 * or a GUID_DDStereoMode.
 */
#define GUID_GetDriverInfo2                         GUID_DDStereoMode

/**
 * Magic number used in DD_GETDRIVERINFO2DATA.dwHeight when
 * GUID_GetDriverInfo2/GUID_DDStereoMode is specified in a
 * GetDriverInfo call.
 */
#define D3DGDI2_MAGIC                               0xFFFFFFFF

#define D3DGDI2_TYPE_GETD3DCAPS8                    0x00000001
#define D3DGDI2_TYPE_GETFORMATCOUNT                 0x00000002
#define D3DGDI2_TYPE_GETFORMAT                      0x00000003
#define D3DGDI2_TYPE_DXVERSION                      0x00000004  /* Make driver aware of currently used DirectX version */
#define D3DGDI2_TYPE_GETD3DCAPS9                    0x00000010
#define D3DGDI2_TYPE_GETEXTENDEDMODECOUNT           0x00000011
#define D3DGDI2_TYPE_GETEXTENDEDMODE                0x00000012
#define D3DGDI2_TYPE_GETADAPTERGROUP                0x00000013
#define D3DGDI2_TYPE_GETMULTISAMPLEQUALITYLEVELS    0x00000016
#define D3DGDI2_TYPE_DEFERRED_AGP_AWARE             0x00000018
#define D3DGDI2_TYPE_FREE_DEFERRED_AGP              0x00000019
#define D3DGDI2_TYPE_DEFER_AGP_FREES                0x00000020
#define D3DGDI2_TYPE_GETD3DQUERYCOUNT               0x00000021
#define D3DGDI2_TYPE_GETD3DQUERY                    0x00000022
#define D3DGDI2_TYPE_GETDDIVERSION                  0x00000023  /* Returns DX9_DDI_VERSION, used to check which DDK version the driver is compiled against */

typedef struct _D3DCAPS8
{
  D3DDEVTYPE DeviceType;
  UINT AdapterOrdinal;
  DWORD Caps;
  DWORD Caps2;
  DWORD Caps3;
  DWORD PresentationIntervals;
  DWORD CursorCaps;
  DWORD DevCaps;
  DWORD PrimitiveMiscCaps;
  DWORD RasterCaps;
  DWORD ZCmpCaps;
  DWORD SrcBlendCaps;
  DWORD DestBlendCaps;
  DWORD AlphaCmpCaps;
  DWORD ShadeCaps;
  DWORD TextureCaps;
  DWORD TextureFilterCaps;
  DWORD CubeTextureFilterCaps;
  DWORD VolumeTextureFilterCaps;
  DWORD TextureAddressCaps;
  DWORD VolumeTextureAddressCaps;
  DWORD LineCaps;
  DWORD MaxTextureWidth;
  DWORD MaxTextureHeight;
  DWORD MaxVolumeExtent;
  DWORD MaxTextureRepeat;
  DWORD MaxTextureAspectRatio;
  DWORD MaxAnisotropy;
  float MaxVertexW;
  float GuardBandLeft;
  float GuardBandTop;
  float GuardBandRight;
  float GuardBandBottom;
  float ExtentsAdjust;
  DWORD StencilCaps;
  DWORD FVFCaps;
  DWORD TextureOpCaps;
  DWORD MaxTextureBlendStages;
  DWORD MaxSimultaneousTextures;
  DWORD VertexProcessingCaps;
  DWORD MaxActiveLights;
  DWORD MaxUserClipPlanes;
  DWORD MaxVertexBlendMatrices;
  DWORD MaxVertexBlendMatrixIndex;
  float MaxPointSize;
  DWORD MaxPrimitiveCount;
  DWORD MaxVertexIndex;
  DWORD MaxStreams;
  DWORD MaxStreamStride;
  DWORD VertexShaderVersion;
  DWORD MaxVertexShaderConst;
  DWORD PixelShaderVersion;
  float MaxPixelShaderValue;
} D3DCAPS8;

typedef struct _DD_GETFORMATCOUNTDATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    DWORD dwFormatCount;
    DWORD dwReserved;
} DD_GETFORMATCOUNTDATA;

typedef struct _DD_GETFORMATDATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    DWORD dwFormatIndex;
    DDPIXELFORMAT format;
} DD_GETFORMATDATA;

typedef struct _DD_DXVERSION
{
    DD_GETDRIVERINFO2DATA gdi2;
    DWORD dwDXVersion;
    DWORD dwReserved;
} DD_DXVERSION;

typedef struct _DD_DEFERRED_AGP_AWARE_DATA
{
    DD_GETDRIVERINFO2DATA gdi2;
} DD_DEFERRED_AGP_AWARE_DATA;

typedef struct _DD_FREE_DEFERRED_AGP_DATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    DWORD dwProcessId;
} DD_FREE_DEFERRED_AGP_DATA;

typedef struct _DD_GETEXTENDEDMODECOUNTDATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    DWORD dwModeCount;
    DWORD dwReserved;
} DD_GETEXTENDEDMODECOUNTDATA;

typedef struct _DD_GETEXTENDEDMODEDATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    DWORD                 dwModeIndex;
    D3DDISPLAYMODE        mode;
} DD_GETEXTENDEDMODEDATA;

typedef struct _DD_GETADAPTERGROUPDATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    ULONG_PTR ulUniqueAdapterGroupId;
    DWORD dwReserved1;
    DWORD dwReserved2;
} DD_GETADAPTERGROUPDATA;

typedef struct _DD_MULTISAMPLEQUALITYLEVELSDATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    D3DFORMAT Format;
    BOOL                    bFlip  :  1;
    D3DMULTISAMPLE_TYPE     MSType : 31;
    DWORD QualityLevels;
} DD_MULTISAMPLEQUALITYLEVELSDATA;

typedef struct _DD_GETD3DQUERYCOUNTDATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    DWORD dwNumQueries;
} DD_GETD3DQUERYCOUNTDATA;

typedef struct _DD_GETD3DQUERYDATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    union
    {
        DWORD dwQueryIndex;
        D3DQUERYTYPE QueryType;
    };
} DD_GETD3DQUERYDATA;

typedef struct _DD_GETDDIVERSIONDATA
{
    DD_GETDRIVERINFO2DATA gdi2;
    DWORD dwDXVersion;
    DWORD dwDDIVersion;
} DD_GETDDIVERSIONDATA;

#endif /* (DIRECT3D_VERSION >= 0x0900) */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _D3DHAL_H_ */
