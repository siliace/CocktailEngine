#include <algorithm>

#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Monitor/Xlib/Monitor.hpp>

namespace Ck::Detail::Xlib
{
	namespace
	{
		const XRRModeInfo* GetModeInfo(const XRRScreenResources* screenResources, RRMode mode)
		{
			for (int i = 0;  i < screenResources->nmode;  i++)
			{
				if (screenResources->modes[i].id == mode)
					return screenResources->modes + i;
			}

			return nullptr;
		}

		unsigned int ComputeModeFrequency(const XRRModeInfo* modeInfo)
		{
			if (!modeInfo->hTotal || !modeInfo->vTotal)
				return 0;

			return (unsigned int) ((double) modeInfo->dotClock / ((double) modeInfo->hTotal * (double) modeInfo->vTotal));
		}

		VideoMode ModeInfoToVideoMode(::Display* display, const XRRCrtcInfo* crtcInfo, const XRRModeInfo* modeInfo)
		{
			int screen = DefaultScreen(display);
			int depth = DefaultDepth(display, screen);

			VideoMode videoMode;
			videoMode.Size.Width = modeInfo->width;
			videoMode.Size.Height = modeInfo->height;
			videoMode.Frequency = ComputeModeFrequency(modeInfo);

		    if (crtcInfo->rotation == RR_Rotate_90 || crtcInfo->rotation == RR_Rotate_270)
				std::swap(videoMode.Size.Width, videoMode.Size.Height);

			videoMode.ColorDepth = depth == 32 ? DisplayColorDepth::e32 : DisplayColorDepth::e24;
			videoMode.AlphaDepth = depth == 32 ? DisplayAlphaDepth::e8 : DisplayAlphaDepth::e0;

			return videoMode;
		}

		Monitor::Orientation RotationToOrientation(Rotation rotation)
		{
			switch (rotation)
			{
			case RR_Rotate_0: return Monitor::Orientation::Landscape;
			case RR_Rotate_90: return Monitor::Orientation::RightPortait;
			case RR_Rotate_180: return Monitor::Orientation::UpsideDownLandscape;
			case RR_Rotate_270: return Monitor::Orientation::LeftPortait;
			}

			COCKTAIL_UNREACHABLE();
		}
	}

    Monitor::Monitor(::Display* display, RRCrtc crtc, RROutput output) :
		mDisplay(display),
		mCrtc(crtc),
		mOutput(output)
    {
		::Window root = DefaultRootWindow(mDisplay);

		mScreenResources = XRRGetScreenResourcesCurrent(mDisplay, root);
		mCrtcInfo = XRRGetCrtcInfo(mDisplay, mScreenResources, crtc);
		mOutputInfo = XRRGetOutputInfo(mDisplay, mScreenResources, mOutput);
	}

    Monitor::~Monitor()
    {
		XRRFreeOutputInfo(mOutputInfo);
		XRRFreeCrtcInfo(mCrtcInfo);
		XRRFreeScreenResources(mScreenResources);
    }

    bool Monitor::IsPrimary() const
    {
		::Window root = DefaultRootWindow(mDisplay);
		return XRRGetOutputPrimary(mDisplay, root) == mOutput;
	}

    Extent2D<unsigned int> Monitor::GetSize() const
    {
        return MakeExtent<unsigned int>(mCrtcInfo->width, mCrtcInfo->height);
    }

    Monitor::Orientation Monitor::GetOrientation() const
    {
        return RotationToOrientation(mCrtcInfo->rotation);
    }

    std::string Monitor::GetName() const
    {
		return std::string(mOutputInfo->name, mOutputInfo->nameLen);;
	}

    VideoMode Monitor::GetCurrentVideoMode() const
    {
		const XRRModeInfo* currentModeInfo = GetModeInfo(mScreenResources, mCrtcInfo->mode);
		return ModeInfoToVideoMode(mDisplay, mCrtcInfo, currentModeInfo);
	}

    void Monitor::SetCurrentVideoMode(const VideoMode& videoMode)
    {
		if (mCurrentVideoMode == videoMode)
			return;
		
		RRMode nativeMode = 0;
		for (int i = 0; i < mOutputInfo->nmode && nativeMode == 0; i++)
		{
			const XRRModeInfo* modeInfo = GetModeInfo(mScreenResources, mOutputInfo->modes[i]);
			assert(modeInfo);

			if ((modeInfo->modeFlags & RR_Interlace) != 0)
				continue;

			if (ModeInfoToVideoMode(mDisplay, mCrtcInfo, modeInfo) != videoMode)
				continue;
			
			nativeMode = mOutputInfo->modes[i];
		}

		if (!nativeMode)
			return;

		XRRSetCrtcConfig(
			mDisplay,
			mScreenResources,
			mCrtc,
			CurrentTime,
			mCrtcInfo->x, mCrtcInfo->y,
			nativeMode,
			mCrtcInfo->rotation,
			mCrtcInfo->outputs,
			mCrtcInfo->noutput
		);

		mCurrentVideoMode = videoMode;
    }

    Array<VideoMode> Monitor::GetSupportedVideoModes() const
    {
		Array<VideoMode> videoModes;

		for (int i = 0; i < mOutputInfo->nmode; i++)
		{
			const XRRModeInfo* modeInfo = GetModeInfo(mScreenResources, mOutputInfo->modes[i]);
			assert(modeInfo);

			if ((modeInfo->modeFlags & RR_Interlace) != 0)
				continue;

			VideoMode videoMode = ModeInfoToVideoMode(mDisplay, mCrtcInfo, modeInfo);
			if (!videoModes.Contains(videoMode))
				videoModes.Add(videoMode);
		}

		return videoModes;
    }

    void* Monitor::GetSystemHandle() const
    {
		return mCrtcInfo;
	}
}
