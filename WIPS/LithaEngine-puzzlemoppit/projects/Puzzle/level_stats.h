
#ifndef LEVEL_STATS_H
#define LEVEL_STATS_H

#include "Litha.h"

enum E_SCORE_RESULT
{
	ESR_AWFUL = 0,
	ESR_FAIR,
	ESR_GOOD,
	ESR_EXCELLENT,
	ESR_PERFECT,
	ESR_EXTRAORDINARY, // user did better than I thought possible
	ESR_UNKNOWN
};

struct LevelStats
{
	LevelStats()
	{
		pushes = 0;
		elevatorMoves = 0;
		undos = 0;
		deaths = 0;
	}
	
	u32 pushes;
	u32 elevatorMoves;
	u32 undos;
	u32 deaths;
};

io::path getsavescoredir();

// a lower score is better
// this score is just used for comparisons, is not something to be shown to the user.
s32 get_score(const LevelStats &stats);

// Get special colour for text of that rating.
video::SColor get_rating_col(E_SCORE_RESULT scoreResult);

// Find the result a user has previously got for the named level.
// Returns ESR_UNKNOWN on fail. (i.e. no score exists yet)
E_SCORE_RESULT get_saved_score(const core::stringc &levelName);

// called at end of a level and a new score has been achieved.
// will update on disk save of score if this new score is better
// returns a result, compared with the built in scores.
E_SCORE_RESULT input_score(const core::stringc &levelName, const LevelStats &stats);

// Returns result as text, e.g. "Good", "Perfect" etc.
core::stringc get_result_description(E_SCORE_RESULT result);

#endif

