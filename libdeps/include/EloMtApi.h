// EloMtApi.h

#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ELOMTAPI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ELOMT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ELOMTAPI_EXPORTS
#define ELOMT_API __declspec(dllexport)
#else
#define ELOMT_API __declspec(dllimport)
#endif

#pragma warning (disable: 4251)

#include "..\..\Common\structs.h"

// This class is exported from the EloMtApi.dll
class ELOMT_API CEloMtApi
{
public:
	CEloMtApi ();
	virtual ~CEloMtApi ();

	int getScreenCount ();
	bool enableTouch (int nScreenIndex, BOOL bEnableTouch);
	bool isTouchEnabled (int nScreenIndex);

	bool getCalData (int nScreenIndex, MMDevCalData* pCalData);
	bool setCalData (int nScreenIndex, MMDevCalData* pCalData);
	bool setCalData (int nScreenIndex, MONITOR* pMonitor, const POINT* pTargets, const POINT* pTouchs);

	bool getEdgeAccel (int nScreenIndex, int w_now, int h_now, ACCELDATA *pEA);
	bool setEdgeAccel (int nScreenIndex, int w_now, int h_now, const ACCELDATA * pAccel);

	int getAprFlashProgress (int nScreenIndex, int * cur_page); // return total pages

	int  getAprParams (int nScreenIndex, int * params, int param_count);
	bool setAprParams (int nScreenIndex, int * params, int param_count);

	bool getControllerFWVersion (int nScreenIndex, TCHAR * ver, size_t nLen);

	DWORD getDiag (int nScreenIndex, MMDevIODIAGNOSTICS * pDiag, bool bUseSmartSet=false);
	int getUAlignCounts (int nScreenIndex, int* u1_cycles, int* u2_cycles);

	bool getApr700XSerialNumbers (int nScreenIndex, MMDevIOGET_SERIAL_NUMBERS *pSN);

	bool flushControllerSmartsetBuffer (int nScreenIndex);
	void cleanUpControllerSmartsetBuffer (int nScreenIndex);

	unsigned long getDebugLevels (int nScreenIndex);
	bool setDebugLevels (int nScreenIndex, ULONG ulDbgLevels);

	bool enableBeep (bool bEnable);
	bool isBeepEnabled ();
	HANDLE configKernelBeepEvent (); 

	bool getTouchRects (int nScreenIndex, MMDevIO_CLIPPING_MODE* pClipMode);
	bool setTouchRects (int nScreenIndex, MMDevIO_CLIPPING_MODE* pClipMode);

    bool getFullScreenClippingRectangle (int nScreenIndex, MMDevIO_FULL_SCREEN_CLIPPING* pFullScreenClipMode);
    bool setFullScreenClippingRectangle (int nScreenIndex, MMDevIO_FULL_SCREEN_CLIPPING* pFullScreenClipMode);

	void clipBoundsToDisplayRect (const ClippingBounds* pClipBounds, RECT* pRect, const MONITOR* pMonitor);
	void displayRectToClipBounds (const RECT* pRects, ClippingBounds* pClipBounds, const MONITOR * pMonitor);

	bool getTouchPoint (int nScreenIndex, POINT * pt);
	bool getTouchPoint (int nScreenIndex, POINT * pt, int * pz, TOUCH_STATUS *eStatus);
	bool startGetMultiTouch (int nScreenIndex, bool bStart);	
	bool getMultiTouch (int nScreenIndex, MT_TOUCH* pMtTouch);
	
	bool clearGetPoint (int nScreenIndex);
	bool clearAllGetPoint ();

	MONITOR * getMonitorByWinMonNum (int nWinMonNum);
	MONITOR * getMonitorByIndex (int nMonitorIndex);
	int getMonitorIndexByWinMonNum (int nWinMonNum);

	void gotoMonitor (HWND hWnd, int nMonitorIndex);
	SCREEN* getScreenByIndex (int nScreenIndex);
	SCREEN* getScreenByMonitorNum (int nMonitorNum);

	int getMonitorCount () { return m_nMonitorCount; }

	void enumTouchscreens();
	void closeEloDeviceHandles();
    void setDeviceNotifyHandle(HANDLE hRecipient);
    BOOL isEloDeviceNotify (LPARAM lParam);
    
	int SendSmartsetCommand( PSMARTSETDATA pSmartsetData );
	int GetSmartsetResponse( PSMARTSETDATA pSmartsetData );
	int GetSmartsetResponseStatus( PSMARTSETDATA pSmartsetData );	
	BOOL SmartsetTransaction(SMARTSETDATA* pSS, SMARTSET_PKT* pSSPACKET=NULL, int nNumPkts=0, int* pnRespPkts=NULL, int nDelay=50);

	bool GetIRInternalSound( int nScreenIndex, UCHAR &State, DWORD &Freq, DWORD &Dura );
	bool SetIRInternalSound( int nScreenIndex, UCHAR State, DWORD Freq, DWORD Dura );
	bool copyDeviceParametersToGlobalKey(int nScreenIndex);
	bool copyGlobalKeyToDeviceParameters(int nScreenIndex);
	bool storeDevKeysToGlobal (int nScreenIndex);
	bool applyGlobalKeys (int nScreenIndex);

	bool isIR (int nScreenIndex);
	bool isCoach (int nScreenIndex);
	bool is2701 (int nScreenIndex);
	bool isElo2521 (int nScreenIndex);

	bool doAutoSizing (int nScreenIndex, MMDevIOAutoSize* io);
	bool isAprFlashDownloaded (int nScreenIndexce);
	int getSystemEdidArray (DWORD * pdwEdidArray);

	bool devio (int nScreenIndex, int nIoctlCode, VOID * pBuf, int nBufSize);

	int getLiveSound (int nScreenIndex, int* pChannel, unsigned char *buf, int buf_size);
	int startRecord(int nScreenIndex, AUDIO_RECORD eRecord, int nFrames);
	int getSoundThresholds(int nScreenIndex, unsigned int *pnTouchlevel, unsigned int *pnUntouchlevel, unsigned int *pnVariance);
	int setSoundThresholds(int nScreenIndex, unsigned int *pnTouchlevel, unsigned int *pnUntouchlevel, unsigned int *pnVariance);
    bool MakeBeep(unsigned int frequency, unsigned int duration);

	void lock();
	void unlock();

	void reset (HWND hWnd);
	HANDLE m_hMonitoringEvents [32];
	
	bool getMaxTouch( int nScreenIndex, ULONG &nTouch );
	bool setMaxTouch( int nScreenIndex, ULONG nMaxTouch = 10 );

	bool getForceMouse( int nScreenIndex, ULONG &lMouse );
	bool setForceMouse( int nScreenIndex, ULONG lMouse = FALSE );

	bool AcquireSmartsetLock( int nScreenIndex, int nRetry = 3 );
	bool ReleaseSmartsetLock( int nScreenIndex );

	bool isTouchMonitorCalibrated (int nScreenIndex);

	bool getControllerCalibrationValues (int nScreenIndex, MMDevCalData* pCalData);
	bool setControllerCalibrationValues (int nScreenIndex, MMDevCalData* pCalData);

	bool getControllerScalingValues (int nScreenIndex, MMDevCalData* pCalData);
	bool setControllerScalingValues (int nScreenIndex, const MMDevCalData* pCalData);

	bool getDriverCalibrationValues (int nScreenIndex, MMDevCalData* pCalData);
	bool setDriverCalibrationValues (int nScreenIndex, MMDevCalData* pCalData);

	bool writeMCommand (int nScreenIndex, unsigned char byte2, unsigned char byte3);

	bool setDriverCalibrationDefault (int nScreenIndex, int win_mon_num);
	bool setControllerScalingDefault (int nScreenIndex);

	bool isEloTrueHid (int nScreenIndex);
	bool isEloSmartsetApr (int nScreenIndex);
	bool isEloAprTechnology (int nScreenIndex);
	bool isSmartsetSupported (int nScreenIndex);
	bool isEloSerialInterface (int nScreenIndex);
	bool isEloPlanBC (int nScreenIndex);
	bool initController (int nScreenIndex);
	bool finalizeController (int nScreenIndex);
	bool updateCalibrationData (int nScreenIndex);
	unsigned short calculateCalibrationChecksum (const MMDevCalData* pCalData);
	bool readMonitorSNandChecksumFromNVRam (int nScreenIndex, DWORD* pdwEdid, unsigned short* pwhecksum);
	bool writeMonitorSNandChecksumToNVRam (int nScreenIndex, DWORD dwEdid, unsigned short checksum);
	
	bool mapTouchScreenToDisplay (const wchar_t* szDevicePath, const wchar_t* szMonitorDeviceID);
	bool restoreCalibrationAndMapDisplay (int nScreenIndex);

	int getMouseMode (int nScreenIndex);
	bool setMouseMode (int nScreenIndex, int nMouseMode);
	bool isPCapTouchScreen (int nScreenIndex);
	bool isOpticalTouchScreen (int nScreenIndex);
    bool isIr6TTouchScreen (int nScreenIndex);

	void enumMonitors ();
	bool isFirmwareNewerThan (int nScreenIndex, int major, int minor);
	void setControllerReportMode (int nScreenIndex, UCHAR mode);
	bool needDriverCalibration (int nScreenIndex);


private:

	// Monitors
	int	     m_nMonitorCount;
	MONITOR	 m_Monitors [MAX_ELO_TOUCHSCREENS];		// fill out in constructor

	// Screens
	int m_nScreenCount;
	SCREEN m_Screens [MAX_ELO_TOUCHSCREENS];	// fill out in constructor with enumDevices()
	static HANDLE m_hMutex;

    int enumDevices (SCREEN * pScreens);
    int enumDevicesXP64 (SCREEN * pScreens);
	static BOOL CALLBACK enumDisplay (HMONITOR hMon, HDC hDC, LPRECT rectMon, LPARAM _pMonitors);
	
	bool isXySwapped (const POINT* pTarget, const POINT* pTouch);
	void processVideoRotation (int dwMonitorOrientation, int xResolution, int yResolution, POINT* tgt);

	bool isXYUPlanABC (int nScreenIndex);
	HKEY OpenDeviceParameterKey(int nScreenIndex);
	unsigned short getFletcher16Checksum (byte* data, int count);
	bool convertToCalibrationValues (int nScreenIndex, MMDevCalData* cd);
	static bool getMatchingEdid (const wchar_t* szMonitorDeviceName, wchar_t* szMonitorDeviceID, DWORD* pdwMonitorSN);
	static DWORD WINAPI thread_clearAllGetPoint (void * param);
	void processVideoRotation (DWORD dwVideoOrientation, int xResolution, int yResolution, const POINT* pTouch, POINT* tgt, POINT* t);
	void getGlassOrientation (bool bIsUsb, const POINT* t, bool* xy_swap, bool* x_mirror, bool* y_mirror);
	void swapXY (POINT* pt);
	bool isEloSurfaceCapacitive (int nScreenIndex);
};