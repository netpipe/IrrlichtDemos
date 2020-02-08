// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef AI_TRACK_H
#define AI_TRACK_H

#include "irrlicht_manager.h"
#include "recorder.h"
#include <vector>

class TiXmlNode;
class TiXmlElement;
struct TrackMarker;

struct AiTrackInfoSettings
{
	void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    irr::core::vector3df mCenter;
    irr::core::vector3df mBorderLeft;      // left border position
    irr::core::vector3df mBorderRight;     // right border position
    float mMaxSpeed;
    irr::core::vector3df mIdealLine;     // point for ideal line
};

struct AiTrackInfo
{
	AiTrackInfo()
	: mCushionLeft(false), mCushionRight(false), mSafetyLeft(false), mSafetyRight(false), mAngleLeft(0.f), mAngleRight(0.f)
	{
	}

    AiTrackInfoSettings mSettings;
    bool mCushionLeft;	// border-lines to and from mLeft will be used cushion hover's driving against it.
    bool mCushionRight; // border-lines to and from mRight will be used cushion hover's driving against it.
    bool mSafetyLeft;	// has already safety walls from geometry
    bool mSafetyRight;	// has already safety walls from geometry

    // for debugging
    float mAngleLeft;
    float mAngleRight;
};

// Track to help the AI. Is by now also used by players.
class AiTrack
{
public:
	AiTrack() : mHasCushions(false)
	{
	}

    bool WriteToXml(TiXmlNode * node_) const;
    bool ReadFromXml(const TiXmlNode * node_);

//    bool CreateTrack(Record & record_, float hoverRadius_);
    void MakeFixedStepSize(float stepSize_ = 100.f);
    void SmoothTrack(bool withBorders_);
    void SmoothBorders(bool left_, bool right_);
    void SmoothIdealLine();
    void AddTrackBehindTp(TrackMarker * tpSource_);
    void MakeBorders(float centerToBorderDist_);
    void FindRealBorders(float hoverRadius_);
    void CalculateCenters();	// based on border-points

    // cushions prevent hovers from dropping off the track in some places
    void CalculateCushions();
    bool HasCushions() const { return mHasCushions; }

    bool RecordCenters(const Record & record_);    // not sure - maybe to center - does
    void RecordSpeed(const Record & record_);
    void RecordIdealLine(const Record & record_);

    void Draw();

    // For given index and pos_ find the nearest position on the center-lines before and after this index
    // and get the closest positions on those lines to the given pos_.
    // delta_ is a value between 0 and 1. 0 = nearestPos_ is at the start of the centerline and 1 it's at the end
    // nearestPos_ is the position found.
    // dist_ is the distance between pos_ and nearestPos_, but positive or negative depending on which side of the center-line pos_ was.
    // return index or nearest pos was closer to the line leading to this index (aka before the point) then the previous index
    int GetNearestPosOnCenterLine(const irr::core::vector3df& pos_, int index_, float &delta_, irr::core::vector3df& nearestPos_, float &dist_) const;

    // For given index and pos_ find the nearest position on the border-lines before and after this index
    // and get the closest positions on those lines to the given pos_.
    // nearestBorderPos nearest point on the border to pos
    // onlyCushions When true return FLT_MAX and no pos when there are no cushions at the line which is nearest
    // driver - pass driver for debug-output. transformation and material must be set already.
    // returns distance between pos and resulting nearestBorderPos
    float GetNearestPosOnBorder(const irr::core::vector3df& pos, int index, irr::core::vector3df& nearestBorderPos, bool onlyCushions=true, irr::video::IVideoDriver * driver=0) const;

    // Given a point on the centerline (by origIndex_ + origDelta_) search for points which are ahead on the track
    // stepDist_ Distance to look forward. Distance is calculated by going along center-lines - so in corners we look not as far ahead
    // centerTarget_ The found point on the centerline
    // idealTarget_  The found oint on the idealline
    void GetPosAtDist(int origIndex_, float origDelta_, float stepDist_, int &targetIndex_, irr::core::vector3df& centerTarget_, irr::core::vector3df& idealTarget_) const;

	// Search for the entry with the track-center closest to pos_
    // lastIndex_: used as start for the search
    // pos_: used as point for the search
    // nearestDist_: return the dist to the center of the found index
    // searchAll_: when true not just a few points around lastIndex_ are search but the whole track
    int GetIndexForNearestCenter(int lastIndex_, const irr::core::vector3df& pos_, float &nearestDist_, bool searchAll_=false) const;

    // return distance to ai-track border from given position and direction.
    // index_: start index used for searching, index must be close to pos_
    // pos_: position for line start
    // dir_: line direction to check
    // maxDist_: maximum distance searched for borders (speed optimization)
    // dist_: return the distance to the border. Will be negative for left border and positive for right border.
    // returns: true when a border was found, otherwise false
    bool GetBorderDist(int index_, const irr::core::vector3df& pos_, const irr::core::vector3df& dir_, float maxDist_, float &dist_) const;

    // Average the lines coming to and leading from a center and return them normalized
    irr::core::vector3df GetCenterLineDirection(int index) const;

    // check which marker is "ahead". Cares about wraparound.
    // return 1 if marker1_ is ahead 0 if they are the same and -1 if marker2_ is ahead.
    int GetMarkerSequence( int marker1_, int marker2_) const;

    unsigned int GetNumTrackInfos() const { return mAiTrackInfo.size(); }
    const AiTrackInfo& GetTrackInfo(unsigned index) const { return mAiTrackInfo[index]; }

protected:
    // get intersection of a line with a section of the track (not border, but lines from left to right cross the track)
    // return index to trackinfo or -1 for "no collision"
    int GetLineTrackIntersection(const irr::core::vector3df& lineStart_, const irr::core::vector3df& lineEnd_, irr::core::vector3df &target_, int startIndex_, float maxHeightDiff_=500.f) const;

    // Given the 3 points assume they form a curve starting at the first vector.
    // Return the angle formed by the 2 lines which those 3 points make.
    // returns values in range -PI to PI. Negative values when the curve is left.
    float GetCurveAngle(const irr::core::vector3df& a, const irr::core::vector3df& b, const irr::core::vector3df& c) const;

private:
    std::vector<AiTrackInfo>    mAiTrackInfo;
    bool mHasCushions;
};

#endif // AI_TRACK_H

