
#include "level_stats.h"

const char *resultTexts[7] =
{
	"Awful",
	"Fair",
	"Good",
	"Excellent",
	"Perfect",
	"EXTRAORDINARY!",
	"???"
};

io::path getsavescoredir()
{
	return os::path::concat( GetEngine()->GetLocalSettingsDir(), "levelscores" );
}

E_SCORE_RESULT calc_score_result(s32 userScore, s32 perfectScore)
{
	if (userScore < perfectScore)
		return ESR_EXTRAORDINARY;
	else if (userScore == perfectScore)
		return ESR_PERFECT;
	else if (userScore <= perfectScore + 3)
		return ESR_EXCELLENT;
	else if (userScore <= perfectScore + 10)
		return ESR_GOOD;
	else if (userScore <= perfectScore + 25)
		return ESR_FAIR;
	else
		return ESR_AWFUL;
}

s32 get_score(const LevelStats &stats)
{
	return stats.pushes + stats.elevatorMoves + stats.undos + stats.deaths;
}

video::SColor get_rating_col(E_SCORE_RESULT scoreResult)
{
	switch (scoreResult)
	{
	case ESR_AWFUL:
		return video::SColor(150, 255,99,99);
	case ESR_FAIR:
		return video::SColor(100, 255,255,255);
	case ESR_GOOD:
		return video::SColor(100, 111,255,111);
	case ESR_EXCELLENT:
		return video::SColor(150, 255,255,100);
	case ESR_PERFECT:
		return video::SColor(150, 255,255, 0);
	default: // ESR_EXTRAORDINARY or anything else...
		return video::SColor(150, 255,255,0);
	}
}

LevelStats map_to_stats(VariantMap variantMap)
{
	LevelStats stats;
	stats.pushes = variantMap["pushes"];
	stats.elevatorMoves = variantMap["elevator_moves"];
	stats.undos = variantMap["undos"];
	stats.deaths = variantMap["deaths"];
	return stats;
}

VariantMap stats_to_map(const LevelStats &stats)
{
	VariantMap variantMap;
	variantMap["pushes"] = stats.pushes;
	variantMap["elevator_moves"] = stats.elevatorMoves;
	variantMap["undos"] = stats.undos;
	variantMap["deaths"] = stats.deaths;
	return variantMap;
}

E_SCORE_RESULT get_saved_score(const core::stringc &levelName)
{
	core::stringc shortScoreFile = os::path::splitext(levelName)[0] + ".ini";
	core::stringc perfectScoreFile = io::path("../projects/Puzzle/perfectscores/") + shortScoreFile;
	core::stringc saveScoreFile = os::path::concat( getsavescoredir(), shortScoreFile );
	
	// Get perfect stats
	
	VariantMap perfectStatsMap = file::loadsettings(perfectScoreFile);
	
	if (perfectStatsMap.size() == 0)
	{
		WARN << "get_saved_score: perfect score not found";
		return ESR_UNKNOWN;
	}
	
	LevelStats perfectStats = map_to_stats(perfectStatsMap);
	perfectStats.undos = 0; // perfect after all...
	perfectStats.deaths = 0;
	
	s32 perfectScore = get_score(perfectStats);
	
	// Get saved stats
	
	VariantMap savedStatsMap = file::loadsettings(saveScoreFile);
	
	if (savedStatsMap.size() != 0)
	{
		// was some saved stats
		LevelStats savedStats = map_to_stats(savedStatsMap);
		
		s32 savedScore = get_score(savedStats);
		
		return calc_score_result(savedScore, perfectScore);
	}
	
	return ESR_UNKNOWN;
}

E_SCORE_RESULT input_score(const core::stringc &levelName, const LevelStats &stats)
{
	NOTE << "Inputting score...";
	
	io::path saveScoreDir = getsavescoredir();
	
	if (!os::path::ensure_dir(saveScoreDir))
		WARN << "Unable to create level score folder.";
	
	core::stringc shortScoreFile = os::path::splitext(levelName)[0] + ".ini";
	core::stringc perfectScoreFile = io::path("../projects/Puzzle/perfectscores/") + shortScoreFile;
	core::stringc saveScoreFile = os::path::concat( saveScoreDir, shortScoreFile );
	
	NOTE << "Loading perfect score: " << perfectScoreFile;
	VariantMap perfectStatsMap = file::loadsettings(perfectScoreFile);
	
	LevelStats perfectStats = map_to_stats(perfectStatsMap);
	perfectStats.undos = 0; // perfect after all...
	perfectStats.deaths = 0;
	
	// calc scores
	s32 userScore = get_score(stats);
	
	
	
	NOTE << "Loading saved user score: " << saveScoreFile;
	VariantMap savedStatsMap = file::loadsettings(saveScoreFile);
	
	// should only save if new scores are better than old, or old scores do not exist.
	bool saveScores = false;
	
	if (savedStatsMap.size() != 0)
	{
		// was some saved stats
		LevelStats savedStats = map_to_stats(savedStatsMap);
		
		// are new stats better?
		s32 oldUserScore = get_score(savedStats);
		
		if (userScore < oldUserScore)
		{
			// save latest scores as they are better than previous
			saveScores = true;
		}
	}
	else
	{
		// No old scores existed.
		saveScores = true;
	}
	
	if (saveScores)
	{
		VariantMap saveStatsMap = stats_to_map(stats);
		
		// No saved stats, so we save now.
		if (!file::savesettings(saveScoreFile, saveStatsMap))
			WARN << "Error saving scores: " << saveScoreFile;
	}
	
	if (perfectStatsMap.size() == 0)
	{
		WARN << "perfect score not found";
		return ESR_UNKNOWN;
	}
	
	s32 perfectScore = get_score(perfectStats);
	
	NOTE << "User score " << userScore << ", perfect score is " << perfectScore;
	
	return calc_score_result(userScore, perfectScore);
}

core::stringc get_result_description(E_SCORE_RESULT result)
{
	ASSERT( result >= 0 && result < 7 );
	return resultTexts[result];
}



