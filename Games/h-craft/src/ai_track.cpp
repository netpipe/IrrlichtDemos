// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "ai_track.h"
#include "helper_irr.h"
#include "main.h"
#include "physics.h"
#include "player.h"
#include "level.h"
#include "logging.h"
#include "config.h"
#include "streaming.h"
#include "tinyxml/tinyxml.h"

using namespace irr;

namespace
{
	const float SEARCH_ABOVE_TRACK = 150.f;
	const float SEARCH_BELOW_TRACK = 500.f;
};

// --------------------------------------------------------
void AiTrackInfoSettings::ReadFromXml(const TiXmlElement * settings_)
{
    assert(settings_);

    settings_->QueryFloatAttribute("c_x", &mCenter.X);
    settings_->QueryFloatAttribute("c_y", &mCenter.Y);
    settings_->QueryFloatAttribute("c_z", &mCenter.Z);

    // Sorry, I had switched left&right in the past, so lf/rg names are swapped in serialization
    mBorderRight = mCenter;
    settings_->QueryFloatAttribute("lf_x", &mBorderRight.X);
    settings_->QueryFloatAttribute("lf_y", &mBorderRight.Y);
    settings_->QueryFloatAttribute("lf_z", &mBorderRight.Z);

    mBorderLeft = mCenter;
    settings_->QueryFloatAttribute("rg_x", &mBorderLeft.X);
    settings_->QueryFloatAttribute("rg_y", &mBorderLeft.Y);
    settings_->QueryFloatAttribute("rg_z", &mBorderLeft.Z);

    mMaxSpeed = 2000.f;
    settings_->QueryFloatAttribute("ms", &mMaxSpeed);

    mIdealLine = mCenter;
    settings_->QueryFloatAttribute("i_x", &mIdealLine.X);
    settings_->QueryFloatAttribute("i_y", &mIdealLine.Y);
    settings_->QueryFloatAttribute("i_z", &mIdealLine.Z);
}

void AiTrackInfoSettings::WriteToXml(TiXmlElement * settings_) const
{
    assert(settings_);

    settings_->SetDoubleAttribute("c_x", mCenter.X);
    settings_->SetDoubleAttribute("c_y", mCenter.Y);
    settings_->SetDoubleAttribute("c_z", mCenter.Z);

    // Sorry, I had switched left&right in the past, so lf/rg names are swapped in serialization
    settings_->SetDoubleAttribute("lf_x", mBorderRight.X);
    settings_->SetDoubleAttribute("lf_y", mBorderRight.Y);
    settings_->SetDoubleAttribute("lf_z", mBorderRight.Z);

    settings_->SetDoubleAttribute("rg_x", mBorderLeft.X);
    settings_->SetDoubleAttribute("rg_y", mBorderLeft.Y);
    settings_->SetDoubleAttribute("rg_z", mBorderLeft.Z);

    settings_->SetDoubleAttribute("ms",  mMaxSpeed);
    settings_->SetDoubleAttribute("i_x", mIdealLine.X);
    settings_->SetDoubleAttribute("i_y", mIdealLine.Y);
    settings_->SetDoubleAttribute("i_z", mIdealLine.Z);
}


// --------------------------------------------------------
// set all centers to the positions used in the record
bool AiTrack::RecordCenters(const Record & record_)
{
    mHasCushions = false;
    mAiTrackInfo.clear();
    PlayerStreamData data;
    MemArchive * archive = record_.GetStreamArchive();
    if ( !archive )
        return false;

    int nrInfos = 0;
    while ( data.StreamFrom(archive, false) )
    {
        AiTrackInfo trackInfo;
        trackInfo.mSettings.mCenter = data.mPos;
        if (    !nrInfos
            ||  (trackInfo.mSettings.mCenter-mAiTrackInfo[nrInfos-1].mSettings.mCenter).getLength() > 10.f
            )
        {
            mAiTrackInfo.push_back(trackInfo);
            ++nrInfos;
        }
    }

    return true;
}

// set to fixed stepsize
void AiTrack::MakeFixedStepSize(float stepSize_)
{
    mHasCushions = false;
    std::vector<AiTrackInfo> workTrackInfo;

    workTrackInfo = mAiTrackInfo;
    mAiTrackInfo.clear();
    bool finished = false;
    core::vector3df last(mAiTrackInfo[0].mSettings.mCenter);
    core::vector3df sumVector(last);
    float length = 0.f;
    float sum = 1.f;
    int counter = 0;
    float dRest = 0.f;
    while(!finished)
    {
        while ( length < stepSize_ )
        {
            float l1 = (workTrackInfo[counter].mSettings.mCenter-last).getLength();
            if ( l1+length < stepSize_ )
            {
                sumVector += workTrackInfo[counter].mSettings.mCenter;
                sum += 1.f;
                length += l1;
                last = workTrackInfo[counter].mSettings.mCenter;
                dRest = 0.f;
                if ( ++counter >= (int)workTrackInfo.size() )
                {
                    counter = 0;
                    finished = true;
                }
            }
            else
            {
                float d = (stepSize_-length) / l1;
                sumVector += workTrackInfo[counter].mSettings.mCenter * d;
                sum += d;
                length = stepSize_;
                dRest = 1.f - d;
            }
        }

        AiTrackInfo trackInfo;
        trackInfo.mSettings.mCenter = sumVector / sum;
        mAiTrackInfo.push_back(trackInfo);

        last = trackInfo.mSettings.mCenter;
        length = 0.f;
        sumVector = workTrackInfo[counter].mSettings.mCenter * dRest;
        sum = dRest;
    }
}

// smooth
void AiTrack::SmoothTrack(bool withBorders_)
{
    const float MAX_DIST = 3000.f;
    std::vector<AiTrackInfo> workTrackInfo;
    workTrackInfo = mAiTrackInfo;
    mAiTrackInfo.clear();
    int nrInfos = workTrackInfo.size();
    for ( int i=0; i<nrInfos; i++ )
    {
        int previous = i > 0 ? i-1 : nrInfos-1;
        int next = i < nrInfos-1 ? i+1 : 0;
        AiTrackInfo trackInfo;
        trackInfo.mSettings = workTrackInfo[i].mSettings;

        float l1 = (workTrackInfo[i].mSettings.mCenter-workTrackInfo[previous].mSettings.mCenter).getLength();
        float l2 = (workTrackInfo[i].mSettings.mCenter-workTrackInfo[next].mSettings.mCenter).getLength();
        if ( l1 != 0.f && l2 != 0.f && l1 < MAX_DIST && l2 < MAX_DIST)
        {
            float d = l1 / (l1+l2);
            core::vector3df dir(workTrackInfo[next].mSettings.mCenter-workTrackInfo[previous].mSettings.mCenter);
            dir.setLength( dir.getLength() * d );
            core::vector3df diff = (workTrackInfo[previous].mSettings.mCenter + dir) - workTrackInfo[i].mSettings.mCenter;
            trackInfo.mSettings.mCenter = diff + workTrackInfo[i].mSettings.mCenter;
            if ( withBorders_ )
            {
                trackInfo.mSettings.mBorderRight = diff + workTrackInfo[i].mSettings.mBorderRight;
                trackInfo.mSettings.mBorderLeft = diff + workTrackInfo[i].mSettings.mBorderLeft;
            }
        }

        mAiTrackInfo.push_back(trackInfo);
    }
}

// smooth borders without changing theire direction
void AiTrack::SmoothBorders(bool left_, bool right_)
{
    mHasCushions = false;
    const float MAX_DIST = 3000.f;
    std::vector<AiTrackInfo> workTrackInfo;
    workTrackInfo = mAiTrackInfo;
    mAiTrackInfo.clear();
    int nrInfos = workTrackInfo.size();
    for ( int i=0; i<nrInfos; i++ )
    {
        int previous = i > 0 ? i-1 : nrInfos-1;
        int next = i < nrInfos-1 ? i+1 : 0;
        AiTrackInfo trackInfo;
        trackInfo.mSettings = workTrackInfo[i].mSettings;

        float l1 = (workTrackInfo[i].mSettings.mCenter-workTrackInfo[previous].mSettings.mCenter).getLength();
        float l2 = (workTrackInfo[i].mSettings.mCenter-workTrackInfo[next].mSettings.mCenter).getLength();

        if ( l1 != 0.f && l2 != 0.f && l1 < MAX_DIST && l2 < MAX_DIST)
        {
            if ( right_ )
            {
                float lenBefore = (workTrackInfo[previous].mSettings.mBorderRight-workTrackInfo[previous].mSettings.mCenter).getLength();
                float lenAfter = (workTrackInfo[next].mSettings.mBorderRight-workTrackInfo[next].mSettings.mCenter).getLength();
                core::vector3df dir(workTrackInfo[i].mSettings.mBorderRight-workTrackInfo[i].mSettings.mCenter);
                dir.setLength((lenBefore+lenAfter)*0.5f);
                trackInfo.mSettings.mBorderRight = workTrackInfo[i].mSettings.mCenter+dir;
            }

            if ( left_ )
            {
                float lenBefore = (workTrackInfo[previous].mSettings.mBorderLeft-workTrackInfo[previous].mSettings.mCenter).getLength();
                float lenAfter = (workTrackInfo[next].mSettings.mBorderLeft-workTrackInfo[next].mSettings.mCenter).getLength();
                core::vector3df dir(workTrackInfo[i].mSettings.mBorderLeft-workTrackInfo[i].mSettings.mCenter);
                dir.setLength((lenBefore+lenAfter)*0.5f);
                trackInfo.mSettings.mBorderLeft = workTrackInfo[i].mSettings.mCenter+dir;
            }
        }
        mAiTrackInfo.push_back(trackInfo);
    }
}

void AiTrack::SmoothIdealLine()
{
    const float MAX_DIST = 3000.f;
    std::vector<AiTrackInfo> workTrackInfo;
    workTrackInfo = mAiTrackInfo;
    mAiTrackInfo.clear();
    int nrInfos = workTrackInfo.size();
    for ( int i=0; i<nrInfos; i++ )
    {
        int previous = i > 0 ? i-1 : nrInfos-1;
        int next = i < nrInfos-1 ? i+1 : 0;

        float l1 = (workTrackInfo[i].mSettings.mIdealLine-workTrackInfo[previous].mSettings.mIdealLine).getLength();
        float l2 = (workTrackInfo[i].mSettings.mIdealLine-workTrackInfo[next].mSettings.mIdealLine).getLength();

        AiTrackInfo trackInfo;
        trackInfo.mSettings = workTrackInfo[i].mSettings;

        if ( l1 != 0.f && l2 != 0.f && l1 < MAX_DIST && l2 < MAX_DIST)
        {
            float d = l1 / (l1+l2);
            core::vector3df dir(workTrackInfo[next].mSettings.mIdealLine-workTrackInfo[previous].mSettings.mIdealLine);
            dir.setLength( dir.getLength() * d );
            irr::core::vector3df pointBetween(workTrackInfo[previous].mSettings.mIdealLine + dir);

            trackInfo.mSettings.mIdealLine = (workTrackInfo[i].mSettings.mIdealLine + pointBetween) * 0.5f;
        }
        mAiTrackInfo.push_back(trackInfo);
    }
}

void AiTrack::AddTrackBehindTp(TrackMarker * tpSource_)
{
    mHasCushions = false;
    if ( !tpSource_  )
        return;
    if ( !tpSource_->mSettings.mIsValid )
        return;

    float dummyDist;
    int index = GetIndexForNearestCenter(0, tpSource_->mSettings.mCenter, dummyDist, true);
    if ( index < 0 )
        return;

    // we expect that there is always a larger gap behind the teleport
    int oldIndex = index;
    int index2 = (index+1) % mAiTrackInfo.size();
    const float MIN_GAP = 1000.f;
    while ( (mAiTrackInfo[index2].mSettings.mCenter - mAiTrackInfo[index].mSettings.mCenter).getLength()  < MIN_GAP )
    {
        index = index2;
        index2 = (index+1) % mAiTrackInfo.size();

        if ( index == oldIndex )    // one time around track - not gap found
            return;
    }

    core::vector3df bonusTrack(0, 0, 1);
    core::matrix4 rotMat;
    rotMat.setRotationDegrees(tpSource_->mSettings.mRotation);
    rotMat.rotateVect(bonusTrack);
    bonusTrack.setLength( MIN_GAP - 100.f ); // just make sure it's long enough

    AiTrackInfo info;
    info = mAiTrackInfo[index];

    info.mSettings.mCenter += bonusTrack;
    info.mSettings.mBorderRight += bonusTrack;
    info.mSettings.mBorderLeft += bonusTrack;
    info.mSettings.mIdealLine += bonusTrack;

    LOG.LogLn(LP_DEBUG, "index: ", index);
    LOG.LogLn(LP_DEBUG, "size: ", (int)mAiTrackInfo.size() );
    mAiTrackInfo.insert( mAiTrackInfo.begin()+index+1, info);
}

void AiTrack::MakeBorders(float centerToBorderDist_)
{
    mHasCushions = false;
    // set borders
    int nrInfos = mAiTrackInfo.size();
    for ( int i=0; i<nrInfos; i++ )
    {
        core::vector3df toNext = mAiTrackInfo[(i+1)%nrInfos].mSettings.mCenter - mAiTrackInfo[i].mSettings.mCenter;
        core::vector3df toRight( core::vector3df(0,1,0).crossProduct(toNext) );
        toRight.setLength(centerToBorderDist_);
        mAiTrackInfo[i].mSettings.mBorderRight = mAiTrackInfo[i].mSettings.mCenter + toRight;
        mAiTrackInfo[i].mSettings.mBorderLeft = mAiTrackInfo[i].mSettings.mCenter - toRight;
    }
}

void AiTrack::FindRealBorders(float hoverRadius_)
{
    mHasCushions = false;

    // find real borders and center
    // TODO: sorry, I was young and bad at documenting - older me has to figure out what "real" means some day.
    int nrInfos = mAiTrackInfo.size();
    for ( int i=0; i<nrInfos; i++ )
    {
        core::vector3df intersection;
        core::vector3df searchPos(mAiTrackInfo[i].mSettings.mCenter);
        core::line3d<float> line(searchPos+core::vector3df(0, SEARCH_ABOVE_TRACK, 0)
                                , searchPos+core::vector3df(0, -SEARCH_BELOW_TRACK, 0) );

        // If no floor below center we except to jump and use the same distance to borders as previous info.
        // This has to be true - if it isn't we can just record the track once more and take more care.
        bool useLastRightBorder = i > 0 ? true : false;
        bool useLastLeftBorder = i > 0 ? true : false;
        if ( APP.GetPhysics()->GetTrackIntersection(line, searchPos, intersection) )
        {
            // find borders
            LOG.LogLn(LP_DEBUG, "border ", i);
            core::vector3df dir = mAiTrackInfo[i].mSettings.mBorderRight - mAiTrackInfo[i].mSettings.mCenter;
            if ( APP.GetPhysics()->FindBorder(mAiTrackInfo[i].mSettings.mCenter, dir, mAiTrackInfo[i].mSettings.mBorderRight, hoverRadius_) )
            {
                useLastRightBorder = false;
            }
            dir = mAiTrackInfo[i].mSettings.mBorderLeft - mAiTrackInfo[i].mSettings.mCenter;
            if ( APP.GetPhysics()->FindBorder(mAiTrackInfo[i].mSettings.mCenter, dir, mAiTrackInfo[i].mSettings.mBorderLeft, hoverRadius_) )
            {
                useLastLeftBorder = false;
            }
        }
        if ( useLastRightBorder )
        {
            float lastRadius = (mAiTrackInfo[i-1].mSettings.mBorderRight - mAiTrackInfo[i-1].mSettings.mCenter).getLength();
            core::vector3df toLeft ( mAiTrackInfo[i].mSettings.mBorderRight - mAiTrackInfo[i].mSettings.mCenter );
            toLeft.setLength(lastRadius);
            mAiTrackInfo[i].mSettings.mBorderRight = mAiTrackInfo[i].mSettings.mCenter + toLeft;
        }
        if ( useLastLeftBorder )
        {
            float lastRadius = (mAiTrackInfo[i-1].mSettings.mBorderLeft - mAiTrackInfo[i-1].mSettings.mCenter).getLength();
            core::vector3df toRight ( mAiTrackInfo[i].mSettings.mBorderLeft - mAiTrackInfo[i].mSettings.mCenter );
            toRight.setLength(lastRadius);
            mAiTrackInfo[i].mSettings.mBorderLeft = mAiTrackInfo[i].mSettings.mCenter + toRight;
        }
    }
}

void AiTrack::CalculateCenters()
{
    // new center is just between borders
    int nrInfos = mAiTrackInfo.size();
    for ( int i=0; i<nrInfos; i++ )
    {
        mAiTrackInfo[i].mSettings.mCenter = (mAiTrackInfo[i].mSettings.mBorderRight+mAiTrackInfo[i].mSettings.mBorderLeft) * 0.5f;
    }
}

float AiTrack::GetCurveAngle(const irr::core::vector3df& a, const irr::core::vector3df& b, const irr::core::vector3df& c) const
{
	core::vector2df prev(b.X-a.X, b.Z-a.Z);
	core::vector2df next(c.X-b.X, c.Z-b.Z);
	prev.normalize();
	next.normalize();
	float angle = atan2(prev.Y, prev.X) - atan2(next.Y, next.X);
	if ( angle < -M_PI )
		angle += 2*M_PI;
	if ( angle > M_PI )
		angle -= 2*M_PI;
	return angle;
}

void AiTrack::CalculateCushions()
{
	HC1_PROFILE(CProfileScope ps1(L"CalculateCushions", L"START_LVL");)
    mHasCushions = false;
	const size_t INTERVAL = 5; // try&error value. Track centers are relatively regular so we just use a index based interval to look ahead
	if ( mAiTrackInfo.size() < INTERVAL )
		return;

	// Check if the geometry already offers a safety.
    // We find those by checking if the border points are close to real border (aka where we fall off the road)
    // That works (or should work) because the way border points are calculated they are moved a inside the
    // road when the border has some geometry protection. It's that way so hovers allways can be on top of
    // border points and when there are walls those move the points inside thereby automatically.
    const float searchDist = APP.GetConfig()->GetHoverRadius() * 1.1f; // try & error value
	core::vector3df intersectionDummy;	// we don't care here.
	core::vector3df searchPos;
	core::line3d<float> searchLine;
    for ( size_t i=0; i< mAiTrackInfo.size(); ++i )
    {
		const AiTrackInfoSettings& thisSettings = mAiTrackInfo[i].mSettings;

		core::vector3df toOutside(thisSettings.mBorderRight-thisSettings.mCenter);
		toOutside.setLength(searchDist);
		searchPos = core::vector3df(thisSettings.mBorderRight+toOutside);
		searchLine.start = searchPos+core::vector3df(0, SEARCH_ABOVE_TRACK, 0);
		searchLine.end   = searchPos+core::vector3df(0, -SEARCH_BELOW_TRACK, 0);
		mAiTrackInfo[i].mSafetyRight = APP.GetPhysics()->GetTrackIntersection(searchLine, searchPos, intersectionDummy);

		toOutside = core::vector3df(thisSettings.mBorderLeft-thisSettings.mCenter);
		toOutside.setLength(searchDist);
		searchPos = core::vector3df(thisSettings.mBorderLeft+toOutside);
		searchLine.start = searchPos+core::vector3df(0, SEARCH_ABOVE_TRACK, 0);
		searchLine.end   = searchPos+core::vector3df(0, -SEARCH_BELOW_TRACK, 0);
		mAiTrackInfo[i].mSafetyLeft = APP.GetPhysics()->GetTrackIntersection(searchLine, searchPos, intersectionDummy);
	}


	// Cushion all borders except at the inside of curves
	// Also not when we already have protecting safety-walls there
	const f32 MAX_INSIDE_CURVE = core::degToRad(5.f);	// No cushion at inside-curves more narrow than that angle. Found by try&error
	const f32 MAX_DIST_NEXT_SQ = 200.f * 200.f;	// No cushion if track-centers are far apart. Found by try&error.
	for ( size_t i=0; i< mAiTrackInfo.size(); ++i )
	{
		size_t prevInterval = (i >= INTERVAL) ? (i-INTERVAL) : mAiTrackInfo.size() - (INTERVAL-i);
		size_t nextInterval = (i+INTERVAL)%mAiTrackInfo.size();

		const AiTrackInfoSettings& prevSettings = mAiTrackInfo[prevInterval].mSettings;
		const AiTrackInfoSettings& thisSettings = mAiTrackInfo[i].mSettings;
		const AiTrackInfoSettings& nextSettings = mAiTrackInfo[nextInterval].mSettings;

		// check if this place has no real connection to neighbors. Usually all tracks have similar distances
		// (around 100 units or below). If that's not the case that's usually a sign of special-case like a huge drop
		// in the track or a teleport.
		size_t p = i > 0 ? i-1 : mAiTrackInfo.size()-1;
		size_t n = (i+1) % mAiTrackInfo.size();
		bool hugeStep =     (thisSettings.mCenter-mAiTrackInfo[p].mSettings.mCenter).getLengthSQ() > MAX_DIST_NEXT_SQ
					     ||	(thisSettings.mCenter-mAiTrackInfo[p].mSettings.mCenter).getLengthSQ() > MAX_DIST_NEXT_SQ;

		// left and right side of track can swap in cases of drops+180° turns. Happens only in 2 levels but we have to catch those.
		f32 largerSide = core::max_((thisSettings.mBorderRight-mAiTrackInfo[p].mSettings.mBorderRight).getLengthSQ()
		                           ,(thisSettings.mBorderLeft-mAiTrackInfo[p].mSettings.mBorderLeft).getLengthSQ());
		f32 smallerCross = core::min_((thisSettings.mBorderRight-mAiTrackInfo[p].mSettings.mBorderLeft).getLengthSQ()
		                             ,(thisSettings.mBorderLeft-mAiTrackInfo[p].mSettings.mBorderRight).getLengthSQ());
		bool sideSwap = largerSide > smallerCross;
		if ( !sideSwap )
		{
			f32 largerSide = core::max_((thisSettings.mBorderRight-mAiTrackInfo[n].mSettings.mBorderRight).getLengthSQ()
									   ,(thisSettings.mBorderLeft-mAiTrackInfo[n].mSettings.mBorderLeft).getLengthSQ());
			f32 smallerCross = core::min_((thisSettings.mBorderRight-mAiTrackInfo[n].mSettings.mBorderLeft).getLengthSQ()
										 ,(thisSettings.mBorderLeft-mAiTrackInfo[n].mSettings.mBorderRight).getLengthSQ());
			sideSwap = largerSide > smallerCross;
		}

		if ( mAiTrackInfo[i].mSafetyRight || hugeStep || sideSwap )
		{
			mAiTrackInfo[i].mCushionRight = false;
			mAiTrackInfo[i].mAngleRight = 0.f;
		}
		else
		{
			float angle = GetCurveAngle(prevSettings.mBorderRight, thisSettings.mBorderRight, nextSettings.mBorderRight);
			mAiTrackInfo[i].mAngleRight = angle;
			mAiTrackInfo[i].mCushionRight = angle < MAX_INSIDE_CURVE;
		}

		if ( mAiTrackInfo[i].mSafetyLeft || hugeStep || sideSwap )
		{
			mAiTrackInfo[i].mCushionLeft = false;
			mAiTrackInfo[i].mAngleLeft = 0.f;
		}
		else
		{
			float angle = GetCurveAngle(prevSettings.mBorderLeft, thisSettings.mBorderLeft, nextSettings.mBorderLeft);
			mAiTrackInfo[i].mAngleLeft = angle;
			mAiTrackInfo[i].mCushionLeft = angle > -MAX_INSIDE_CURVE;
		}
	}

	mHasCushions = true;
}

void AiTrack::RecordSpeed(const Record & record_)
{
    float timeStep = APP.GetPhysics()->GetSettings().mTimeStep;
    const std::vector<RecordTimeEntry>& timeTable = record_.GetTimeTable();
    MemArchive * archive = record_.GetStreamArchive();
    int trackIndex = 0;

    for ( unsigned int t=0; t < timeTable.size()-1; ++t )
    {
        u32 timeDelta = timeTable[t+1].mTime - timeTable[t].mTime;
        if ( timeDelta > 0 )
        {
            float timeDeltaSec = timeDelta * timeStep;   // records are in physics timesteps
            archive->SetReadPos(timeTable[t].mStreamPos);
            PlayerStreamData data1;
            data1.StreamFrom(archive);

            archive->SetReadPos(timeTable[t+1].mStreamPos);
            PlayerStreamData data2;
            data2.StreamFrom(archive);

            core::vector3df dist(data2.mPos - data1.mPos);

            float distDummy;
            trackIndex = GetIndexForNearestCenter(trackIndex, data1.mPos, distDummy, false);
            mAiTrackInfo[trackIndex].mSettings.mMaxSpeed = dist.getLength() / timeDeltaSec;

//            LOG.DebugLn("speed ", mAiTrackInfo[trackIndex].mSettings.mMaxSpeed);
//            LOG.DebugLn("time %f", timeDeltaSec);
        }
    }
}

void AiTrack::RecordIdealLine(const Record & record_)
{
    // reset ideal line to centers
    for ( unsigned int i=0; i<mAiTrackInfo.size(); ++i )
    {
        mAiTrackInfo[i].mSettings.mIdealLine = mAiTrackInfo[i].mSettings.mCenter;
    }

    const std::vector<RecordTimeEntry>& timeTable = record_.GetTimeTable();
    MemArchive * archive = record_.GetStreamArchive();

    float maxHeightDiff=300.f;
    int index = -1;
    for ( unsigned int t=0; t < timeTable.size(); ++t )
    {
        unsigned int t2 = (t+1) % timeTable.size();

        archive->SetReadPos(timeTable[t].mStreamPos);
        PlayerStreamData data1;
        data1.StreamFrom(archive);

        archive->SetReadPos(timeTable[t2].mStreamPos);
        PlayerStreamData data2;
        data2.StreamFrom(archive);

        core::vector3df intersection;
        index = GetLineTrackIntersection(data1.mPos, data2.mPos, intersection, index+1, maxHeightDiff);
        if ( index >= 0 )
        {
            mAiTrackInfo[index].mSettings.mIdealLine = intersection;
            int secondIndex = GetLineTrackIntersection(data1.mPos, data2.mPos, intersection, index+1, maxHeightDiff);
            while ( secondIndex-index == 1 )
            {
                mAiTrackInfo[secondIndex].mSettings.mIdealLine = intersection;
                index = secondIndex;
                secondIndex = GetLineTrackIntersection(data1.mPos, data2.mPos, intersection, index+1, maxHeightDiff);
            }
        }
    }
}

int AiTrack::GetLineTrackIntersection(const irr::core::vector3df& lineStart_, const irr::core::vector3df& lineEnd_, irr::core::vector3df &target_, int startIndex_, float maxHeightDiff_) const
{
    if ( startIndex_ < 0 )
        startIndex_ = 0;

    core::line2df line(lineStart_.X, lineStart_.Z, lineEnd_.X, lineEnd_.Z);
    core::vector2df bonusLength( line.end - line.start );
    bonusLength.normalize();
    line.start -= bonusLength * 5.f;
    line.end += bonusLength * 5.f;

    float bottom = lineStart_.Y < lineEnd_.Y ? lineStart_.Y : lineEnd_.Y;
    float top = lineStart_.Y > lineEnd_.Y ? lineStart_.Y : lineEnd_.Y;
    bottom -= maxHeightDiff_;
    top += maxHeightDiff_;

    for ( unsigned int i=0; i<mAiTrackInfo.size(); ++i )
    {
        int idx = (i+startIndex_) % mAiTrackInfo.size();

        if (    (mAiTrackInfo[idx].mSettings.mBorderRight.Y > bottom || mAiTrackInfo[idx].mSettings.mBorderLeft.Y > bottom)
            &&  (mAiTrackInfo[idx].mSettings.mBorderRight.Y < top || mAiTrackInfo[idx].mSettings.mBorderLeft.Y < top)
            )
        {
//            core::line2df lineSection(mAiTrackInfo[idx].mSettings.mBorderRight.X, mAiTrackInfo[idx].mSettings.mBorderRight.Z,
//                                mAiTrackInfo[idx].mSettings.mBorderLeft.X, mAiTrackInfo[idx].mSettings.mBorderLeft.Z );
            core::line2df lineSection(mAiTrackInfo[idx].mSettings.mBorderLeft.X, mAiTrackInfo[idx].mSettings.mBorderLeft.Z,
                                        mAiTrackInfo[idx].mSettings.mBorderRight.X, mAiTrackInfo[idx].mSettings.mBorderRight.Z );

            core::vector2df out;

            if ( lineSection.intersectWith(line, out) )
            {
                target_.X = out.X;
                target_.Z = out.Y;
                core::line3df lineSection3d(mAiTrackInfo[idx].mSettings.mBorderRight, mAiTrackInfo[idx].mSettings.mBorderLeft);
                ExtIrr::CalcYForPointOnLine(lineSection3d, target_);

                return idx;
            }
        }
    }
    return -1;
}

/*
// Doesn't seem to be used currently.
// I guess I wrote that backj then to try automizing the ai-track generation more.
// Not sure why it didn't work - code does not look too bad - so might try that again
// when improving editor some day.
bool AiTrack::CreateTrack(Record & record_, float hoverRadius_)
{
    if ( !RecordCenters(record_) )
        return false;

    MakeFixedStepSize();

    for ( int s = 0; s < 10; ++s )
    {
        SmoothTrack(false);
    }

    MakeBorders(100.f);
    FindRealBorders(hoverRadius_);
    CalculateCenters();

    for ( int s = 0; s < 5; ++s )
    {
        SmoothTrack(true);
    }

    return true;
}
*/

// Drawing debug information
void AiTrack::Draw()
{
    if ( !mHasCushions )
		CalculateCushions();

    APP.GetIrrlichtManager()->SetDriverDrawMode();
    video::IVideoDriver * driver = APP.GetIrrlichtManager()->GetVideoDriver();
    assert(driver);

    float DRAW_ABOVE = 0.f; // hm - data is already in the air because of hover radius
    float CUSHION_ABOVE = 20.f;
    size_t nrInfos = mAiTrackInfo.size();
    for ( size_t i=0; i<nrInfos; i++ )
    {
        size_t i2 = (i+1)%nrInfos;
        core::vector3df start = mAiTrackInfo[i].mSettings.mCenter;
        core::vector3df end = mAiTrackInfo[i2].mSettings.mCenter;
        start.Y += DRAW_ABOVE;
        end.Y += DRAW_ABOVE;
        driver->draw3DLine (start, end, video::SColor(255, 0, 0, 255));

        start = mAiTrackInfo[i].mSettings.mBorderRight;
        end = mAiTrackInfo[i2].mSettings.mBorderRight;
        start.Y += DRAW_ABOVE;
        end.Y += DRAW_ABOVE;
        driver->draw3DLine (start, end, video::SColor(255, 0, 0, 255));
        if ( mAiTrackInfo[i].mCushionRight && mAiTrackInfo[i2].mCushionRight )
        {
			start.Y += CUSHION_ABOVE;
			end.Y += CUSHION_ABOVE;
			driver->draw3DLine (start, end, video::SColor(255, 0, 0, 255));
        }

        start = mAiTrackInfo[i].mSettings.mBorderLeft;
        end = mAiTrackInfo[i2].mSettings.mBorderLeft;
        start.Y += DRAW_ABOVE;
        end.Y += DRAW_ABOVE;
        driver->draw3DLine (start, end, video::SColor(255, 0, 0, 255));
        if ( mAiTrackInfo[i].mCushionLeft && mAiTrackInfo[i2].mCushionLeft )
        {
			start.Y += CUSHION_ABOVE;
			end.Y += CUSHION_ABOVE;
			driver->draw3DLine (start, end, video::SColor(255, 0, 0, 255));
        }

        start = mAiTrackInfo[i].mSettings.mCenter;
        end = mAiTrackInfo[i].mSettings.mBorderLeft;
        driver->draw3DLine (start, end, video::SColor(255, 0, 0, 255));

        end = mAiTrackInfo[i].mSettings.mBorderRight;
        driver->draw3DLine (start, end, video::SColor(255, 0, 0, 255));

        start = mAiTrackInfo[i].mSettings.mIdealLine;
        end = mAiTrackInfo[i2].mSettings.mIdealLine;
        driver->draw3DLine (start, end, video::SColor(255, 0, 255, 255));
    }

#if 0
    // some text-output (slow)
    irr::gui::IGUIFont * font = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getGUIEnvironment()->getSkin()->getFont();
    irr::scene::ICameraSceneNode * camera = APP.GetIrrlichtManager()->GetActiveCamera();
    irr::scene::ISceneCollisionManager * collMan = APP.GetIrrlichtManager()->GetSceneManager()->getSceneCollisionManager();
    if ( font && camera && collMan )
    {
		bool left = false;
		bool right = true;
		bool numbers = false;
		core::stringw text;
		core::position2d<s32> pos;
		core::rect<s32> r;
		for ( size_t i=0; i<nrInfos; i++ )
		{
			if ( right )
			{
				text = core::stringw (mAiTrackInfo[i].mAngleRight);
				pos = collMan->getScreenCoordinatesFrom3DPosition(mAiTrackInfo[i].mSettings.mBorderRight, camera);
				r = core::rect<s32> (pos, core::dimension2d<s32>(1,1));
				font->draw(text, r, video::SColor(255,255,255,255), true, true);
			}
			if ( left )
			{
				text = core::stringw(mAiTrackInfo[i].mAngleLeft);
				core::position2d<s32> pos = collMan->getScreenCoordinatesFrom3DPosition(mAiTrackInfo[i].mSettings.mBorderLeft, camera);
				r = core::rect<s32>(pos, core::dimension2d<s32>(1,1));
				font->draw(text, r, video::SColor(255,255,255,255), true, true);
			}
			if ( numbers )
			{
				text = core::stringw(i);
				pos = collMan->getScreenCoordinatesFrom3DPosition(mAiTrackInfo[i].mSettings.mCenter, camera);
				r = core::rect<s32>(pos, core::dimension2d<s32>(1,1));
				font->draw(text, r, video::SColor(255,255,255,255), true, true);
			}
		}
	}
#endif
}

bool AiTrack::WriteToXml(TiXmlNode * node_) const
{
    TiXmlElement  * eleAiTrack = node_->ToElement();
    eleAiTrack->Clear();
    for ( unsigned int i=0; i < mAiTrackInfo.size(); ++i )
    {
        TiXmlElement ele("sector");
        mAiTrackInfo[i].mSettings.WriteToXml(&ele);
        eleAiTrack->InsertEndChild(ele);
    }
    return true;
}

bool AiTrack::ReadFromXml(const TiXmlNode * node_)
{
	mHasCushions = false;
    mAiTrackInfo.clear();
    const TiXmlNode * nodeAiTrack = node_->IterateChildren("sector", NULL);
    while ( nodeAiTrack )
    {
        AiTrackInfo info;
        info.mSettings.ReadFromXml(nodeAiTrack->ToElement());
        mAiTrackInfo.push_back(info);

        nodeAiTrack = node_->IterateChildren("sector", nodeAiTrack);
    }
    return true;
}

void AiTrack::GetPosAtDist(int origIndex_, float origDelta_, float stepDist_, int &targetIndex_, core::vector3df& centerTarget_, irr::core::vector3df& idealTarget_) const
{
    if ( !mAiTrackInfo.size() )
        return;
    int idx1 = origIndex_;
    int idx2 = idx1 < (int)mAiTrackInfo.size()-1 ? idx1+1 : 0;
    //float trackStepSize = (mAiTrackInfo[idx2].mSettings.mCenter-mAiTrackInfo[idx1].mSettings.mCenter).getLength();
    float trackStepSize = ExtIrr::GetLengthXZ(mAiTrackInfo[idx2].mSettings.mCenter-mAiTrackInfo[idx1].mSettings.mCenter);
    targetIndex_ = origIndex_;
    stepDist_ += origDelta_*trackStepSize;
    while ( stepDist_ > trackStepSize )
    {
        stepDist_ -= trackStepSize;

        idx1 = idx2;
        idx2 = idx1 < (int)mAiTrackInfo.size()-1 ? idx1+1 : 0;

        // using 3D
        //trackStepSize = (mAiTrackInfo[idx2].mSettings.mCenter-mAiTrackInfo[idx1].mSettings.mCenter).getLength();

        // only using 2D here
        trackStepSize = ExtIrr::GetLengthXZ(mAiTrackInfo[idx2].mSettings.mCenter-mAiTrackInfo[idx1].mSettings.mCenter);

        if ( ++targetIndex_ >= (int)mAiTrackInfo.size() )
            targetIndex_ = 0;
    }
    float delta = stepDist_ > 0.f ? stepDist_ / trackStepSize : 0.f;
    targetIndex_ = idx1;
    core::vector3df toTarget(mAiTrackInfo[idx2].mSettings.mCenter-mAiTrackInfo[targetIndex_].mSettings.mCenter);
    toTarget *= delta;
    centerTarget_ = mAiTrackInfo[targetIndex_].mSettings.mCenter + toTarget;

    toTarget = mAiTrackInfo[idx2].mSettings.mIdealLine-mAiTrackInfo[targetIndex_].mSettings.mIdealLine;
    toTarget *= delta;
    idealTarget_ = mAiTrackInfo[targetIndex_].mSettings.mIdealLine + toTarget;
}

int AiTrack::GetNearestPosOnCenterLine(const core::vector3df& pos_, int index_, float &delta_, core::vector3df& nearestPos_, float &dist_) const
{
    if ( !mAiTrackInfo.size() )
        return -1;

	if ( index_ < 0 )
		index_ = 0;

    int idxBefore = index_ > 0 ? index_-1 : mAiTrackInfo.size()-1;
    int idxNext = (index_+1) % mAiTrackInfo.size();
    int idxNext2 = (index_+2) % mAiTrackInfo.size();

    core::line3df line1(mAiTrackInfo[idxBefore].mSettings.mCenter, mAiTrackInfo[index_].mSettings.mCenter);
    core::line3df line2(mAiTrackInfo[index_].mSettings.mCenter, mAiTrackInfo[idxNext].mSettings.mCenter);
    core::line3df line3(mAiTrackInfo[idxNext].mSettings.mCenter, mAiTrackInfo[idxNext2].mSettings.mCenter);

    core::vector3df target1, target2, target3;
    float d1 = ExtIrr::GetClosestPointOnLineWithDelta(line1, pos_, target1);
    float d2 = ExtIrr::GetClosestPointOnLineWithDelta(line2, pos_, target2);
    float d3 = ExtIrr::GetClosestPointOnLineWithDelta(line3, pos_, target3);
    float dist1 = (target1-pos_).getLength();
    float dist2 = (target2-pos_).getLength();
    float dist3 = (target3-pos_).getLength();

    if ( dist1 < dist2 && dist1 < dist3)
    {
        delta_ = d1;
        nearestPos_ = target1;
        dist_ = dist1;

        core::vector3df trackDir(line1.end-line1.start);
        trackDir.Y = 0.f;
        trackDir.normalize();
        core::vector3df targetToPos(pos_-nearestPos_);
        targetToPos.Y = 0.f;
        if ( trackDir.crossProduct(targetToPos).Y < 0 )
            dist_ *= -1.f;
		return idxBefore;
    }
    else if ( dist2 < dist3 )
    {
        delta_ = d2;
        nearestPos_ = target2;
        dist_ = dist2;

        core::vector3df trackDir(line2.end-line2.start);
        trackDir.Y = 0.f;
        trackDir.normalize();
        core::vector3df targetToPos(pos_-nearestPos_);
        targetToPos.Y = 0.f;
        if ( trackDir.crossProduct(targetToPos).Y < 0 )
            dist_ *= -1.f;
		return index_;
    }
    else
    {
        delta_ = d3;
        nearestPos_ = target3;
        dist_ = dist3;

        core::vector3df trackDir(line3.end-line3.start);
        trackDir.Y = 0.f;
        trackDir.normalize();
        core::vector3df targetToPos(pos_-nearestPos_);
        targetToPos.Y = 0.f;
        if ( trackDir.crossProduct(targetToPos).Y < 0 )
            dist_ *= -1.f;
		return index_+1;
    }
}

float AiTrack::GetNearestPosOnBorder(const irr::core::vector3df& pos, int index, irr::core::vector3df& nearestBorderPos, bool onlyCushions, irr::video::IVideoDriver * driver) const
{
    float nearestDist = FLT_MAX;

    if ( index < 0 || index >= (int)mAiTrackInfo.size() )
        return nearestDist;

    int idxBefore = index > 0 ? index-1 : mAiTrackInfo.size()-1;
    int idxNext = (index + 1) % mAiTrackInfo.size();
    int idxNext2 = (index + 2) % mAiTrackInfo.size();

    const AiTrackInfo& infoBefore = mAiTrackInfo[idxBefore];
    const AiTrackInfo& infoThis = mAiTrackInfo[index];
    const AiTrackInfo& infoNext = mAiTrackInfo[idxNext];
    const AiTrackInfo& infoNext2 = mAiTrackInfo[idxNext2];

    core::line3df lines[6];
    bool cushions[6];
    lines[0] = core::line3df(infoBefore.mSettings.mBorderRight, infoThis.mSettings.mBorderRight);
    cushions[0] = infoBefore.mCushionRight && infoThis.mCushionRight;
    lines[1] = core::line3df(infoBefore.mSettings.mBorderLeft, infoThis.mSettings.mBorderLeft);
    cushions[1] = infoBefore.mCushionLeft && infoThis.mCushionLeft;
    lines[2] = core::line3df(infoThis.mSettings.mBorderRight, infoNext.mSettings.mBorderRight);
    cushions[2] = infoThis.mCushionRight && infoNext.mCushionRight;
    lines[3] = core::line3df(infoThis.mSettings.mBorderLeft, infoNext.mSettings.mBorderLeft);
    cushions[3] = infoThis.mCushionLeft && infoNext.mCushionLeft;
    lines[4] = core::line3df(infoNext.mSettings.mBorderRight, infoNext2.mSettings.mBorderRight);
    cushions[4] = infoNext.mCushionRight && infoNext2.mCushionRight;
    lines[5] = core::line3df(infoNext.mSettings.mBorderLeft, infoNext2.mSettings.mBorderLeft);
    cushions[5] = infoNext.mCushionLeft && infoNext2.mCushionLeft;

    if ( driver )
    {
		for ( int i=0; i < 6; ++i )
			driver->draw3DLine(lines[i].start, lines[i].end, video::SColor(255, 0, 0, 255));
    }

    core::vector3df target;
    int nearestLineIdx = -1;
    for ( int i=0; i<6; ++ i )
	{
		ExtIrr::GetClosestPointOnLineWithDelta(lines[i], pos, target);
		float d = (target-pos).getLength();
		if ( d < nearestDist )
		{
			nearestDist = d;
			nearestBorderPos = target;
			nearestLineIdx = i;
		}
	}

	// Note that we can't avoid the calculations above even when we only check for cushions.
	// Because to know if the closest line is a cushion or not we need the closest line first.
	if ( onlyCushions && nearestLineIdx >= 0 && !cushions[nearestLineIdx] )
	{
		nearestDist = FLT_MAX;
	}

    return nearestDist;
}

// return 1 if marker1_ is ahead 0 if they are the same and -1 if marker2_ is ahead.
int AiTrack::GetMarkerSequence( int marker1_, int marker2_) const
{
    if ( marker1_ == marker2_ )
        return 0;

    int halfsize = (int)(GetNumTrackInfos() * 0.5);
    if ( marker1_ > marker2_ && marker1_ - marker2_ < halfsize )
        return 1;

    if ( marker1_ < marker2_ && marker2_ - marker1_ > halfsize )
        return 1;

    return -1;
}


int AiTrack::GetIndexForNearestCenter(int lastIndex_, const core::vector3df& pos_, float &nearestDist_, bool searchAll_) const
{
    if ( !mAiTrackInfo.size() )
        return -1;

    if ( lastIndex_ < 0 || lastIndex_ >= (int)mAiTrackInfo.size() )
        lastIndex_ = 0;

    int bestIndex = lastIndex_;
    float bestDist = (pos_-mAiTrackInfo[bestIndex].mSettings.mCenter).getLengthSQ();
    const int SEARCH_RANGE = 15;    // indices checked in each direction
    int frontIndex = bestIndex >= SEARCH_RANGE ? bestIndex - SEARCH_RANGE : mAiTrackInfo.size() - (SEARCH_RANGE-bestIndex);
    int backIndex = (bestIndex + SEARCH_RANGE) % mAiTrackInfo.size();

    if ( searchAll_ )
    {
        frontIndex = 0;
        backIndex = mAiTrackInfo.size()-1;
    }

    int i=frontIndex;
    bool finished = false;
    while ( !finished )
    {
        if ( i == backIndex )
            finished = true;

        float dist = (pos_-mAiTrackInfo[i].mSettings.mCenter).getLengthSQ();
        if ( dist < bestDist )
        {
            bestDist = dist;
            bestIndex = i;
        }

        ++i;
        i %= mAiTrackInfo.size();
    }

    nearestDist_ = sqrt(bestDist);

    return bestIndex;
}

bool AiTrack::GetBorderDist(int index_, const core::vector3df& pos_, const core::vector3df& dir_, float maxDist_, float &dist_) const
{
    dist_ = maxDist_;
    if ( !mAiTrackInfo.size() )
    {
        return false;
    }

    if ( index_ < 0 || index_ >= (int)mAiTrackInfo.size() )
        index_ = 0;

    float maxDistSq = maxDist_ * maxDist_;
    int startIndex = index_;
    int idx1 = index_;
    int idx2 = index_ < (int)mAiTrackInfo.size()-1 ? index_ + 1 : 0;
    bool searchForward = (mAiTrackInfo[idx2].mSettings.mCenter-mAiTrackInfo[idx1].mSettings.mCenter).dotProduct(dir_) >= 0 ? true : false;
    core::vector3df end( dir_ );
    end.normalize();
    end *= maxDist_;
    end += pos_;
    core::line2df line1( pos_.X, pos_.Z, end.X, end.Z);

    while(1)
    {
        core::line2df lineRight(mAiTrackInfo[idx1].mSettings.mBorderRight.X, mAiTrackInfo[idx1].mSettings.mBorderRight.Z, mAiTrackInfo[idx2].mSettings.mBorderRight.X, mAiTrackInfo[idx2].mSettings.mBorderRight.Z);
        core::line2df lineLeft(mAiTrackInfo[idx1].mSettings.mBorderLeft.X, mAiTrackInfo[idx1].mSettings.mBorderLeft.Z, mAiTrackInfo[idx2].mSettings.mBorderLeft.X, mAiTrackInfo[idx2].mSettings.mBorderLeft.Z);

        if (    (pos_-mAiTrackInfo[idx1].mSettings.mBorderRight).getLengthSQ() > maxDistSq
            &&  (pos_-mAiTrackInfo[idx2].mSettings.mBorderRight).getLengthSQ() > maxDistSq
            &&  (pos_-mAiTrackInfo[idx1].mSettings.mBorderLeft).getLengthSQ() > maxDistSq
            &&  (pos_-mAiTrackInfo[idx2].mSettings.mBorderLeft).getLengthSQ() > maxDistSq )
        {
            break;
        }

        core::vector2df out;
        if ( line1.intersectWith(lineRight, out) )
        {
            core::vector3df realOut(out.X, 0.f, out.Y);
            core::line3df lineLeft3f(mAiTrackInfo[idx1].mSettings.mBorderRight, mAiTrackInfo[idx2].mSettings.mBorderRight);
            ExtIrr::CalcYForPointOnLine(lineLeft3f, realOut);
            float d = (realOut-pos_).getLength() * -1.f;
            if ( fabs(d) < fabs(dist_) )
            {
                dist_ = d;
            }
        }

        if ( line1.intersectWith(lineLeft, out) )
        {
            core::vector3df realOut(out.X, 0.f, out.Y);
            core::line3df lineRight3f(mAiTrackInfo[idx1].mSettings.mBorderLeft, mAiTrackInfo[idx2].mSettings.mBorderLeft);
            ExtIrr::CalcYForPointOnLine(lineRight3f, realOut);
            float d = (realOut-pos_).getLength();
            if ( fabs(d) < fabs(dist_) )
            {
                dist_ = d;
            }
        }

        if ( fabs(dist_) < maxDist_ )
        {
            return true;
        }

        if ( searchForward )
        {
            idx1 = idx2;
            idx2 = idx2 < (int)mAiTrackInfo.size()-1 ? idx2 + 1 : 0;
        }
        else
        {
            idx2 = idx1;
            idx1 = idx1 > 0 ? idx1-1 : (int)mAiTrackInfo.size()-1;
        }

        if ( idx1 == startIndex )
        {
            break;
        }
    }

    return false;
}

core::vector3df AiTrack::GetCenterLineDirection(int index) const
{
    if ( index < 0 || index >= (int)mAiTrackInfo.size() || mAiTrackInfo.size() < 2 )
        return core::vector3df(0,0,0);

    int idxBefore = index > 0 ? index-1 : mAiTrackInfo.size()-1;
    int idxBehind = index < (int)mAiTrackInfo.size()-1 ? index + 1 : 0;

    irr::core::vector3df dir1(mAiTrackInfo[index].mSettings.mCenter - mAiTrackInfo[idxBefore].mSettings.mCenter);
    irr::core::vector3df dir2(mAiTrackInfo[idxBehind].mSettings.mCenter - mAiTrackInfo[index].mSettings.mCenter);
    dir1.normalize();
    dir2.normalize();

    return irr::core::vector3df(dir1+dir2).normalize();
}
