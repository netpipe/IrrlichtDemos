// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef RECORDER_H
#define RECORDER_H

#include <vector>
#include <string>
#include "irrlicht_manager.h"

class TiXmlElement;
class IArchive;
class MemArchive;

struct Record;

class IRecordable
{
public:
    virtual ~IRecordable()  {}
    virtual void Recording(IArchive * ar_) = 0;
    virtual void Playing(IArchive * ar_) = 0;
    virtual void PlayingInterpolated(Record & record_, irr::u32 time_) = 0;
    virtual void PlayingInterpolated(Record & record_, irr::f32 time_) = 0;
};

struct RecordTimeEntry
{
    RecordTimeEntry(irr::u32 time_, unsigned long streamPos_) : mTime(time_), mStreamPos(streamPos_) {}

    bool operator<(const RecordTimeEntry& entry_) const     	{ return mTime < entry_.mTime; }
    bool operator<(irr::u32 time_) const                        { return mTime < time_; }
    friend bool operator<(irr::u32 time_, const RecordTimeEntry& entry_);

    irr::u32        mTime;
    unsigned long   mStreamPos;
};

class Record : public irr::IReferenceCounted
{
friend class Recorder;
public:
    Record();
    virtual ~Record();

    Record(const Record &other_);
    Record& operator=(const Record &other_);

    void Clear();
    MemArchive * GetStreamArchive() const                    { return mStreamArchive; }
    MemArchive * GetSerializeArchive() const                 { return mSerializeArchive; }
    const std::vector<RecordTimeEntry>& GetTimeTable() const { return mTimeTable; }

    bool operator<(int id_) const         { return mId < id_; }
    friend bool operator<(int id_, const Record &obj_);

    bool Save(const std::string &filename_) const;
    bool Load(const std::string &filename_, bool onlySerializeArchive_=false);

    void UpdateRecording(irr::u32 time_, IRecordable * recordable_);
    void UpdatePlaying(irr::u32 time_, IRecordable * recordable_);
    void UpdatePlaying(irr::f32 time_, IRecordable * recordable_);

private:
    int             mId;
    MemArchive *    mStreamArchive;                 // used for streaming
    MemArchive *    mSerializeArchive;              // used for serializing data
    std::vector<RecordTimeEntry>    mTimeTable;     // index to stream positions in the archive based on time
};

struct RecordingObject
{
    inline bool operator<(IRecordable * ptr_)  const { return mRecordable < ptr_; }
    friend bool operator<(IRecordable * ptr_, const RecordingObject &obj_);

    IRecordable *   mRecordable;
    int             mRecordId;
};

struct PlayingObject
{
    bool operator<(IRecordable * ptr_) const { return mRecordable < ptr_; }
    friend bool operator<(IRecordable * ptr_, const PlayingObject &obj_);

    IRecordable *   mRecordable;
    int             mRecordId;
};

class Recorder
{
public:
    Recorder();
    ~Recorder();

    unsigned int GetNumRecords()  const         { return mRecords.size(); }
    unsigned int GetNumRecordingObjects() const { return mRecordingObjects.size(); }
    unsigned int GetNumPlayingObjects() const   { return mPlayingObjects.size(); }

    int AddRecord(); // returns recordId
    void RemoveRecord(int recordId_);
    void RemoveAllRecords();
    void RemoveAllUnusedRecords();  // all records for which there is no playing or recording object
    Record* GetRecord(int recordId_) const;

    void AddRecordingObject(IRecordable *recordable_, int recordId_);
    void RemoveRecordingObject(IRecordable *recordable_);
    void RemoveAllRecordingObjects();

    void AddPlayingObject(IRecordable *recordable_, int recordId_);
    void RemovePlayingObject(IRecordable *recordable_);
    void RemoveAllPlayingObjects();

    void UpdateRecording(irr::u32 time_);
    void UpdatePlaying(irr::u32 time_);
    void UpdatePlaying(irr::f32 time_);

private:
    long mHighestRecordId;
    std::vector<Record*> mRecords;
    std::vector<RecordingObject> mRecordingObjects;
    std::vector<PlayingObject> mPlayingObjects;
};

#endif // RECORDER_H
