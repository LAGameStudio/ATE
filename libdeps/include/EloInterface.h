// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ELOPUBLICINTERFACE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ELOPUBLICINTERFACE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ELOPUBLICINTERFACE_EXPORTS
#define ELOPUBLICINTERFACE_API __declspec(dllexport)
#else
#define ELOPUBLICINTERFACE_API __declspec(dllimport)
#endif

#pragma pack( push, EloPubIF,1 )
// -----------------------------------------------------------------------------
// Elo Software Development Kit 
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------
//valid touch modes
#define CLICK_ON_TOUCH		0
#define CLICK_ON_RELEASE	1
#define	MOUSE_EMULATION		6

#define TOUCHSCREEN_TYPE_USB 0x01
#define TOUCHSCREEN_TYPE_PNP_SERIAL 0x02
#define TOUCHSCREEN_TYPE_NTSERIAL 0x04
#define TOUCHSCREEN_TYPE_LEGACYSERIAL 0x08

#define WM_ELOSWAPBUTTON WM_USER+4698

#define MAX_SUPPORTED_SCR			64

#define COMPORT_NAME_LENGTH			256
#define SERIALNUMBER_NAME_LENGTH	18
#define USB_PRODUCT_STRING_LENGTH	64

#ifndef ELOPUBLICINTERFACE_EXPORTS

#define MAX_ELO_TOUCH  10 // pCap 2205

// -----------------------------------------------------------------------------
typedef enum _CONTRL_STAT				// ctrl_status values
{
	CS_OK = 0,
	CS_ConstantTouch,
	CS_CanNotFindController,
	CS_NoResponse,
	CS_InvalidResponse,
	CS_CanNotSetBaudRate,
	CS_CommandNotSent,
	CS_SystemError,
	CS_InvalidCommPort,
	CS_CommPortFailedOpen,
	CS_CommPortCommandError,
	CS_CommPortNoController,
	CS_UndefinedController
} CONTRL_STAT;

// -----------------------------------------------------------------------------
typedef enum _GETPOINTS_STATUS	
{
	InitialTouch=1,
	StreamTouch = 2,
	UnTouch = 4
} TOUCH_STATUS;

// -----------------------------------------------------------------------------
typedef enum _GETPOINTS_CODE	
{
	ReturnImmediately=1,
 	ReturnOnTouch,
 	ReturnOnUntouch,
 	ReturnOnNextValidTouch
}GETPOINTS_CODE ;

typedef enum 
{
	enumVrtlDeskDisabled = 0,	// no virtual desktop
	enumVrtlNoBounds,			// NO virtual desktop bounds, No Clipping, Cursor visible
	enumVrtlBoundsClipped,   	// virtual desktop bounds enable, Clipping, Cursor moves at bounds
	enumVrtlBoundsFreeze		// virtual desktop bounds enable, Clipping, Cursor Freezes at bounds
} VrtlBoundMode;

// -----------------------------------------------------------------------------
// Define clipping rectangles
typedef struct _ClippingBounds		// IN Set calibrate data input buffer
{
	long			X_Max;
	long			X_Min;
	long			Y_Max;
	long			Y_Min;
	long			Z_Max;
	long			Z_Min;
} ClippingBounds, *PClippingBounds;
#define MAX_BOUND	64

typedef	struct tagTouch
{
	UCHAR  status; // 0: not touched, non-zero: touch down
	UCHAR  id;     // touch id: 0 or 1 repressenting the touch finger
	USHORT x;      // x of a touch point
	USHORT y;      // y of a touch point      
} TOUCH;

typedef	struct tagMultiTouch
{
	TOUCH			touch [MAX_ELO_TOUCH];	// Multi-touch points
	TOUCH_STATUS	status [MAX_ELO_TOUCH]; // touch status (1, 2, 4)
	UCHAR			count;					// actual number of touches (1 or 2).
} MULTI_TOUCH;

#endif


// -----------------------------------------------------------------------------
// Structure definitions
// -----------------------------------------------------------------------------

// Don't change the order of the members!
typedef enum _APR_PREFERENCES
{
	APR_Gaming = 0,
	APR_Signature,
	APR_General,
	APR_POS,
	APR_Custom	// used by EloGetAprPreference only, indicating a customized settings in use.
} APR_PREFERENCES;

// -----------------------------------------------------------------------------
// Screen Properties. This is exactly the same as PROPERTIESDATA in EloInterface.h
typedef struct _SCREEN_PROPERTIES
{
	int				iWindowsMonNo ;
	ULONG			Type;	// TOUCHSCREEN_TYPE_USB, TOUCHSCREEN_NT_SERIAL, ...
	wchar_t			Port[COMPORT_NAME_LENGTH];
	wchar_t			wcUsbProductString [USB_PRODUCT_STRING_LENGTH];
	unsigned char	SerialNumber[SERIALNUMBER_NAME_LENGTH];	// ASCII 8-digit serial#
	DWORD			HardwareHandshaking ; 
	CONTRL_STAT		ctrl_status;	// Controller Status 
	LONG			BaudRate;		// Baud rate of controler, 0 for bus
	unsigned char	crevminor;		// controller rev minor number
	unsigned char	crevmajor;		// controller rev major number
	unsigned char	trevminor;		// Unused
	unsigned char	trevmajor;		// Unused
	unsigned char	diagcodes[8];	// Diag codes ret from controller
	unsigned char	id[8];			// OEM ID string ret from controller
	unsigned char	cnt_id[8];		// Full Smartset controller ID packet 
	unsigned char	driver_id[32];	// Driver ID
} SCREEN_PROPERTIES, *PSCREEN_PROPERTIES ;

// -----------------------------------------------------------------------------
// Defines a touch coordinate buffer
typedef struct _TOUCH_POINT
{
	LONG				x;
	LONG				y;
	LONG				z;
	TOUCH_STATUS	Status;		
}TOUCH_POINT, *PTOUCH_POINT ;

// -----------------------------------------------------------------------------
// dragdelay data
typedef struct _Drag_Delay{
	DWORD MinDragDelay ;
	DWORD MaxDragDelay ;
	DWORD DragDelay ;
} DRAG_DELAY, *PDRAG_DELAY ;

// -----------------------------------------------------------------------------
// Beep Data

typedef enum Beep_Flag	// Direction of data in the ioctl call 	
{
    _NoBeep,
    _BeepOnTouch,
    _BeepOnRelease
} _BEEP_FLAG_;

typedef struct _Beep{
	_BEEP_FLAG_ BeepFlag;
	DWORD BeepFreq ;
	DWORD BeepTime ;
	BOOL  UseExternalSpeaker;
	BOOL  UseMotherboardBeeper;
} BEEP, *PBEEP ;

// -----------------------------------------------------------------------------
// clipping rectangles bound
typedef struct _Clip_Rect
{
	LONG			MonitorNumber ;
	DWORD			NumBounds;
	ClippingBounds	Bounds[64];
	DWORD			ExclusionFlag;
	VrtlBoundMode   ClippingMode;
} CLIP_RECT, *PCLIP_RECT ;

// -----------------------------------------------------------------------------
// touch boundary data
typedef struct _Touch_Boundary
{
	ClippingBounds	Bounds;
	VrtlBoundMode   ClippingMode;
} TOUCH_BOUNDARY, *PTOUCH_BOUNDARY ;

// -----------------------------------------------------------------------------
// Defines calibration data input buffer. Initialise this buffer to set calibration data to driver. 
// 06-27-2008: to work with G2 driver, the contents of the struct is the same as MMDevCalData.
// 10-14-2009: Remove data member "Header" because it requires the definition of MMDevIOHeader and other enum types which are not used.
//           : But the content is still the same as MMDevCalData except the first one.
typedef struct _Calibration		
{
  	LONG			EloDx;
	LONG			ScrDx;
	LONG			X_Offset;
	LONG			EloDy;
	LONG			ScrDy;
	LONG			Y_Offset;
	LONG			EloDz;
	LONG			ScrDz;
	LONG			Z_Offset;
	ULONG			WindowsMonitorNumber ;
	ULONG			xyswap;	
	LONG			xRes ;		// x, y monitor resolution at which these calculations were made
	LONG			yRes ;
	LONG			xVirtScrSize ;	// Width and height, in pixels, of the virtual screen. 
	LONG			yVirtScrSize ;	// The virtual screen is the bounding rectangle of all display monitors.
	LONG			xVirtScrCoord ;	// Coordinates for the left side and the top of the virtual screen. 
	LONG			yVirtScrCoord ;	
	LONG			xMonLocn ;		// monitor location
	LONG			yMonLocn ;

	LONG     VDeskMode ;
	int      nScreenIndex;
	POINT    xyTouch[3];
	POINT    xyTarget[3] ;
} CALIBRATION, *PCALIBRATION;

// -----------------------------------------------------------------------------
// Quick Touch data
typedef struct _Quick_Touch
{
	DWORD			bEnable ;		
	ULONG			Dx ;
	ULONG			Dy ;
} QUICK_TOUCH, *PQUICK_TOUCH;

// -----------------------------------------------------------------------------
// Edge acceleration data

// -----------------------------------------------------------------------------
typedef struct _Accel
{
	ULONG			Enable;
	ULONG			Scale;
	ClippingBounds	Bounds[1];
} EDGE_ACCEL, *PEDGE_ACCEL ;


// -----------------------------------------------------------------------------
// Untouch timeout
typedef struct _UNTOUCH_TIMEOUT
{
	ULONG			TimeOut;
	ULONG			Width;
	ULONG			Height;
}UNTOUCH_TIMEOUT, *PUNTOUCH_TIMEOUT ; 
// -----------------------------------------------------------------------------
// Right button on hold 
typedef struct _Right_Button
{
	DWORD			RightClickHW ;
	DWORD			InitialTimeout ;
	DWORD			DefaultRightClickDelay ;
	DWORD			MaxRightClickDelay ;
	DWORD			MinRightClickDelay ; 
	ULONG			ClickCount;
	ULONG			Active ;
}RIGHT_BUTTON, *PRIGHT_BUTTON;

// 04-15-2008: struct represents the 3-pt calibration data stored in NVRam
typedef struct _NVRam_Cal_Data
{
	DWORD dwEDID; // 4-byte serial number in monitor's EDID (Extended Display Identification Data)

	int nEloDx;   
	int nEloDy;
	int nScreenDx;
	int nScreenDy;
	int nOffsetX;
	int nOffsetY;
	BOOL bSwapXY;
} NVRAM_CAL_DATA;

// -----------------------------------------------------------------------------
// Structures used by Smartset 
typedef struct _Smartset_Data
{
	LONG ScreenNumber;
	union
	{
		char Command[8];
		USHORT Response;
	};
} SMARTSET_DATA;

typedef struct _Smartset_Pkt
{
	unsigned char pkt[8];
} SMARTSET_PACKET;

// -----------------------------------------------------------------------------
// API function calls 
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// This is the first call required to be made to enumerate & initialise the touch 
// screens. It returns the total number of screens found.
//
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetScreenInfo(DWORD dwMonNo[MAX_SUPPORTED_SCR], int &iScrCnt);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
//
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetTouch(PTOUCH_POINT tpxy, BOOL xlated, 
									   GETPOINTS_CODE getCode, UINT &nScrNo);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloCancel() ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloCancelEx (int nScreenIndex);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetMouseMode( WORD *pwMode , UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetMouseMode( WORD wMode , UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetTouchReportingState( BOOL *pbFlag , UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetTouchReportingState( BOOL bFlag , UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetDragDelay( PDRAG_DELAY pDragDelay , UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetDragDelay( PDRAG_DELAY pDragDelay , UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetBeep( PBEEP psndVal , UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetBeep( PBEEP psndVal , UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetTouchBoundary( PTOUCH_BOUNDARY ptBndry , UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetTouchBoundary( PTOUCH_BOUNDARY ptBndry , UINT nScrNo );
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetCalibrationData( PCALIBRATION pCalData , UINT nScrNum ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetCalibrationData( PCALIBRATION pCalData, UINT nScrNum ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSwapButton( HWND hWnd, DWORD dwCnt, UINT nScrNum );
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetLeftHandedMouse( BOOL bFlag ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetDiagnosticsData( PSCREEN_PROPERTIES pData, UINT nScrNo );
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetQuickTouch( PQUICK_TOUCH pQTouch, UINT nScrNo );
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetQuickTouch( PQUICK_TOUCH pQTouch, UINT nScrNo );
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetEdgeAccel( PEDGE_ACCEL pAccel, UINT nScrNo );
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetEdgeAccel( PEDGE_ACCEL pAccel, UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetUntouchTimeout( PUNTOUCH_TIMEOUT pUTimeout, UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetUntouchTimeout( PUNTOUCH_TIMEOUT pUTimeout, UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetRightClickOnHold( PRIGHT_BUTTON pRBHold, UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetRightClickOnHold( PRIGHT_BUTTON pRBHold, UINT nScrNo ) ;
#ifdef __cplusplus
}
#endif

// AIA: Smartset stuff For Debug use.
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetNVRamCalData (NVRAM_CAL_DATA *pNVRamCalData, UINT nScrNo);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSendSmartsetCommand (SMARTSET_DATA* pSmartsetData);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API BOOL EloSmartsetTransaction (SMARTSET_DATA* pSS, SMARTSET_PACKET* pSSPACKET=NULL, int nNumPkts=0, int* pnRespPkts=NULL, int nDelay=20);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetSmartsetResponseStatus (SMARTSET_DATA* pSmartsetData);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetSmartsetResponse (SMARTSET_DATA* pSmartsetData);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetAprPreference (int nScreenIndex);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetAprPreference (int nScreenIndex, const APR_PREFERENCES apr_pref);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetAprParameters (int nScreenIndex, int * pnParameters, int * pnParameterCount);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetAprParameters (int nScreenIndex, const int * pnParameters, int nParameterCount);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetClipRectangles (CLIP_RECT * pClipRect);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloSetClipRectangles (const CLIP_RECT * pClipRect);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloGetSerialNumbers (int nScreenIndex, 
												wchar_t * pszUsbControllerSN, int nUsbControllerSNBufLen,
												char * pszSensorSN, int nSensorSNBufLen);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloAcquireSmartsetLock (int nScreenIndex, int nRetry = 1);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API int EloReleaseSmartsetLock (int nScreenIndex);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API int EloInitialize ();
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API void EloCleanUp ();
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API void EloSetDeviceNotifyHandle (HANDLE hRecipient, BOOL bIsServiceStatusHandle);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API BOOL EloIsEloDeviceNotify (WPARAM wParam, LPARAM lParam);
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API BOOL EloGetIRInternalSound( int nScreenIndex, UCHAR &State, DWORD &Freq, DWORD &Dura );
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API BOOL EloSetIRInternalSound( int nScreenIndex, UCHAR State, DWORD Freq, DWORD Dura );
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API int EloGetAprFlashProgress( int nScreenIndex, int* pPagesToRead );
#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API int EloStartAprFlashDownload( int nScreenIndex );
#ifdef __cplusplus
}
#endif


// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API BOOL EloSetControllerReportMode (int nScreenIndex, BOOL bMouseMode);
#ifdef __cplusplus
}
#endif


// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
ELOPUBLICINTERFACE_API BOOL EloGetMultiTouch (int nScreenIndex, MULTI_TOUCH* pMultiTouch, BOOL bRawTouch);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif  
ELOPUBLICINTERFACE_API BOOL EloCancelGetMultiTouch (int nScreenIndex);
#ifdef __cplusplus
}
#endif


#pragma pack( pop, EloPubIF )