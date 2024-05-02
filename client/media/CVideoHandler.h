/*
 * CVideoHandler.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#pragma once

#ifndef DISABLE_VIDEO

#include "IVideoPlayer.h"

#include "../lib/Rect.h"

struct SDL_Surface;
struct SDL_Texture;
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVFrame;
struct AVIOContext;

VCMI_LIB_NAMESPACE_BEGIN
class CInputStream;
VCMI_LIB_NAMESPACE_END

class CVideoPlayer final : public IVideoPlayer
{
	int stream;					// stream index in video
	AVFormatContext *format;
	AVCodecContext *codecContext; // codec context for stream
	const AVCodec *codec;
	AVFrame *frame;
	struct SwsContext *sws;

	AVIOContext * context;

	VideoPath fname;  //name of current video file (empty if idle)

	// Destination. Either overlay or dest.

	SDL_Texture *texture;
	SDL_Surface *dest;
	Rect destRect;			// valid when dest is used
	Rect pos;				// destination on screen

	/// video playback currnet progress, in seconds
	double frameTime;
	bool doLoop;				// loop through video

	bool playVideo(int x, int y, bool stopOnKey, bool overlay);
	bool open(const VideoPath & fname, bool loop, bool useOverlay, bool scale);
public:
	CVideoPlayer();
	~CVideoPlayer();

	bool init();
	bool open(const VideoPath & fname, bool scale) override;
	void close() override;
	bool nextFrame() override;			// display next frame

	void show(int x, int y, SDL_Surface *dst, bool update) override; //blit current frame
	void redraw(int x, int y, SDL_Surface *dst, bool update) override; //reblits buffer
	void update(int x, int y, SDL_Surface *dst, bool forceRedraw, bool update, std::function<void()> onVideoRestart) override; //moves to next frame if appropriate, and blits it or blits only if redraw parameter is set true

	// Opens video, calls playVideo, closes video; returns playVideo result (if whole video has been played)
	bool openAndPlayVideo(const VideoPath & name, int x, int y, EVideoType videoType) override;

	std::pair<std::unique_ptr<ui8 []>, si64> getAudio(const VideoPath & videoToOpen) override;

	Point size() override;

	// public to allow access from ffmpeg IO functions
	std::unique_ptr<CInputStream> data;
	std::unique_ptr<CInputStream> dataAudio;
};

#endif
